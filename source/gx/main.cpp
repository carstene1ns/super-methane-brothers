/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

/*
 * GameCube/Wii Port by Infact <infact quantentunnel de> 2012
 */

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------

#include <gccore.h>
#include <ogcsys.h>
#include <gctypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>
#include <string.h>
#include <malloc.h>

#include "global.h"
#include "doc.h"

/* GC/Wii Scaler */

static unsigned int *xfb[2] = { NULL, NULL }; /* Double buffered GC/Wii framebuffer */
static int whichfb = 0; /* Framebuffer switch */
GXRModeObj *vmode = NULL; /* Current video mode */
static bool vmode_60hz = true;
bool progressive = false;
u32 FrameTimer = 0;

/* GX */
#define TEX_WIDTH SCR_WIDTH
#define TEX_HEIGHT SCR_HEIGHT
#define DEFAULT_FIFO_SIZE ( 256 * 1024 )
static u8 gp_fifo[DEFAULT_FIFO_SIZE] ATTRIBUTE_ALIGN(32);
static u32 copynow = GX_FALSE;
static GXTexObj texobj;
static Mtx view;
static unsigned char texturemem[TEX_WIDTH * TEX_HEIGHT * 4] ATTRIBUTE_ALIGN (32);

/* Set initial video mode update */
static int UpdateVideo = 1;

/* Default 4:3 aspect */
#define HASPECT 320
#define VASPECT 240

static unsigned short rgb565[256]; /* Texture palette */

/* New texture based scaler */
typedef struct tagcamera
{
	guVector pos;
	guVector up;
	guVector view;
}
camera;

/* Square Matrix
 * This structure controls the size of the image on the screen.
 * Think of the output as a -80 x 80 by -60 x 60 graph.
 */
s16 square[] ATTRIBUTE_ALIGN (32) =
{
	/*     X,        Y, Z */
	-HASPECT,  VASPECT, 0,	// 0
	 HASPECT,  VASPECT, 0,	// 1
	 HASPECT, -VASPECT, 0,	// 2
	-HASPECT, -VASPECT, 0	// 3
};


static camera cam = {
	{0.0F, 0.0F, 0.0F},
	{0.0F, 0.5F, 0.0F},
	{0.0F, 0.0F, -0.5F}
};

/* Video Threading */
#define TSTACK 16384
static lwp_t vbthread = LWP_THREAD_NULL;
static unsigned char vbstack[TSTACK];

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

/* Video threading support functions */
/****************************************************************************
 * vbgetback
 *
 * This callback enables the game to keep running while waiting for a
 * vertical blank.
 ***************************************************************************/
static void *
vbgetback (void *arg)
{
	while(1)
	{
		VIDEO_WaitVSync(); /* Wait for video vertical blank */
		LWP_SuspendThread(vbthread);
	}
	return NULL;
}

/****************************************************************************
 * copy_to_xfb
 *
 * Copies the GX buffer to the current display mode. Also increments
 * the frameticker, as it's called for each vb.
 ***************************************************************************/
static inline void
copy_to_xfb (u32 arg)
{
	if (copynow == GX_TRUE)
	{
		GX_CopyDisp(xfb[whichfb], GX_TRUE);
		GX_Flush();
		copynow = GX_FALSE;
	}
	++FrameTimer;
}

/* GX scaler support functions */
static inline void
draw_init ()
{
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_CLR0, GX_INDEX8);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GX_SetArray(GX_VA_POS, square, 3 * sizeof(s16));

	GX_SetNumTexGens(1);
	GX_SetNumChans(0);

	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);

	memset(&view, 0, sizeof(Mtx));
	guLookAt(view, &cam.pos, &cam.up, &cam.view);
	GX_LoadPosMtxImm(view, GX_PNMTX0);

	GX_InvVtxCache(); /* update vertex cache */
}

static inline void
draw_vert (u8 pos, u8 c, f32 s, f32 t)
{
	GX_Position1x8(pos);
	GX_Color1x8(c);
	GX_TexCoord2f32(s, t);
}

static inline void
draw_square (Mtx v)
{
	Mtx m, mv; /* Model, Modelview matrizes. */

	guMtxIdentity(m);
	guMtxTransApply(m, m, 0, 0, -100);
	guMtxConcat(v, m, mv);

	GX_LoadPosMtxImm(mv, GX_PNMTX0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
	draw_vert(0, 0, 0.0, 0.0);
	draw_vert(1, 0, 1.0, 0.0);
	draw_vert(2, 0, 1.0, 1.0);
	draw_vert(3, 0, 0.0, 1.0);
	GX_End();
}

/****************************************************************************
 * FindVideoMode
 *
 * Finds the optimal video mode
 ***************************************************************************/
static
GXRModeObj * FindVideoMode()
{
	GXRModeObj * mode;

	mode = VIDEO_GetPreferredMode(NULL);

	if(mode == &TVPal576IntDfScale)
		mode = &TVPal528IntDf;

#ifdef HW_DOL
	/* Component cable detected, switch interlaced mode to progressive */
	if(VIDEO_HaveComponentCable())
		mode = &TVNtsc480Prog;
#endif /* HW_DOL */

	switch (mode->viTVMode >> 2)
	{
		case VI_PAL:
			// 576 lines (PAL 50Hz)
			vmode_60hz = false;
			break;

		case VI_NTSC:
			// 480 lines (NTSC 60Hz)
			vmode_60hz = true;
			break;

		default:
			// 480 lines (PAL 60Hz)
			vmode_60hz = true;
			break;
	}

	/* Check for progressive scan */
	if (mode->viTVMode == VI_TVMODE_NTSC_PROG)
		progressive = true;
	else
		progressive = false;

#ifdef HW_RVL
	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
		mode->viWidth = 678;
	else
		mode->viWidth = 672;

	/* Center the output */
	if (vmode_60hz)
	{
		mode->viXOrigin = (VI_MAX_WIDTH_NTSC - mode->viWidth) / 2;
		mode->viYOrigin = (VI_MAX_HEIGHT_NTSC - mode->viHeight) / 2;
	}
	else
	{
		mode->viXOrigin = (VI_MAX_WIDTH_PAL - mode->viWidth) / 2;
		mode->viYOrigin = (VI_MAX_HEIGHT_PAL - mode->viHeight) / 2;
	}
#endif /* HW_RVL */

	return mode;
}

/****************************************************************************
 * SetupVideoMode
 *
 * Sets up the given video mode
 ***************************************************************************/
static void
SetupVideoMode(GXRModeObj * mode)
{
	if(vmode == mode)
		return;

	VIDEO_SetPostRetraceCallback(NULL);
	copynow = GX_FALSE;
	VIDEO_Configure(mode);
	VIDEO_Flush();

	/* Clear framebuffers etc. */
	VIDEO_ClearFrameBuffer(mode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer(mode, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer(xfb[0]);

	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();

	if (mode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	else
		while (VIDEO_GetNextField())
			VIDEO_WaitVSync();

	/* Execute callback on every frame */
	VIDEO_SetPostRetraceCallback((VIRetraceCallback) copy_to_xfb);
	vmode = mode;
}

/****************************************************************************
 * DeInit_GX_Video
 *
 * Deinitializes GC/Wii Video, shuts down GX
 ****************************************************************************/
void
DeInit_GX_Video()
{
	GX_AbortFrame();
	GX_Flush();

	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
}

/****************************************************************************
 * Init_GX_Video
 *
 * Initializes GC/Wii Video, starts up GX
 ****************************************************************************/
void
Init_GX_Video ()
{
	VIDEO_Init();

	/* Allocate the video buffers */
	xfb[0] = (u32 *) memalign(32, 640*576*2);
	xfb[1] = (u32 *) memalign(32, 640*576*2);
	DCInvalidateRange(xfb[0], 640*576*2);
	DCInvalidateRange(xfb[1], 640*576*2);
	xfb[0] = (u32 *) MEM_K0_TO_K1(xfb[0]);
	xfb[1] = (u32 *) MEM_K0_TO_K1(xfb[1]);

	/* Find and setup video mode */
	GXRModeObj *rmode = FindVideoMode();
	SetupVideoMode(rmode);

	/* Start thread */
	LWP_CreateThread(&vbthread, vbgetback, NULL, vbstack, TSTACK, 68);

	/* Initialize GX */
	GXColor background = { 0, 0, 0, 0xff }; /* Set background to black */
	memset(&gp_fifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init(&gp_fifo, DEFAULT_FIFO_SIZE);
	GX_SetCopyClear(background, 0x00ffffff);
	GX_SetDispCopyGamma(GX_GM_1_0);
	GX_SetCullMode(GX_CULL_NONE);
}

void
ResetFbWidth(int width, GXRModeObj *rmode)
{
	if(rmode->fbWidth == width)
		return;
	
	rmode->fbWidth = width;
	
	if(rmode != vmode)
		return;
	
	GX_InvVtxCache();
	VIDEO_Configure(rmode);
	VIDEO_Flush();
}

/****************************************************************************
 * ResetVideo
 *
 * Reset the video/rendering mode
****************************************************************************/
void
ResetVideo()
{
	GXRModeObj *rmode;
	Mtx44 p;

	rmode = FindVideoMode();

	SetupVideoMode(rmode); /* Reconfigure VI */

	GXColor background = {0, 0, 0, 255}; /* Set background to black */
	GX_SetCopyClear(background, 0x00ffffff);

	GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
	GX_SetDispCopyYScale((f32) rmode->xfbHeight / (f32) rmode->efbHeight);
	GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);

	GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth, rmode->xfbHeight);
	GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, (rmode->xfbMode == VI_XFBMODE_SF) ? GX_FALSE : GX_TRUE, rmode->vfilter);

	GX_SetFieldMode(rmode->field_rendering, ((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

	if (rmode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);

	/* Matrix,                  t,                     b,                   l,                r,   n,    f */
	guOrtho(p, rmode->efbHeight/2, -(rmode->efbHeight/2), -(rmode->fbWidth/2), rmode->fbWidth/2, 100, 1000);
	GX_LoadProjectionMtx(p, GX_ORTHOGRAPHIC);

	draw_init();

	/* Reinitialize texture */
	GX_InvalidateTexAll();
	/* Initialize the texture obj we are going to use */
	GX_InitTexObj(&texobj, texturemem, TEX_WIDTH, TEX_HEIGHT, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
	/* Load the texture */
	GX_LoadTexObj(&texobj, GX_TEXMAP0);

	// Clear texture memory
	memset(texturemem, 0, TEX_WIDTH * TEX_HEIGHT * 2);
}

/****************************************************************************
 * Update_GX_Video
 *
 * Copies the rendered frame to a GX texture and draws it to
 * GC/Wii framebuffer.
 ****************************************************************************/
void
Update_GX_Video(unsigned char *screen_buffer)
{
	/* Ensure previous vb has completed */
	while ((LWP_ThreadIsSuspended (vbthread) == 0) || (copynow == GX_TRUE))
		usleep(50);

	/* Swap framebuffers */
	whichfb ^= 1;

	/* video mode has changed */
	if(UpdateVideo)
	{
		UpdateVideo = 0;
		ResetVideo(); /* reset video */
	}

	METHANE_RGB *pal_ptr;
	int i, width, height;

	// Set the game palette
	pal_ptr = Game.m_GameTarget.m_rgbPalette;

	for (i = 0; i < PALETTE_SIZE; i++, pal_ptr++) {
		/* Generate RGB565 texture palette for GX rendering */
		rgb565[i] = ((pal_ptr->red & 0xf8) << 8) |
					((pal_ptr->green & 0xfc) << 3) |
					((pal_ptr->blue & 0xf8) >> 3);
	}

	u16 *texture = (unsigned short *) texturemem;

	/* Set offsets */
	u8 *src1 = screen_buffer;
	u8 *src2 = screen_buffer + SCR_WIDTH;
	u8 *src3 = screen_buffer + SCR_WIDTH * 2;
	u8 *src4 = screen_buffer + SCR_WIDTH * 3;

	/* Fill the texture */
	for (height = 0; height < SCR_HEIGHT; height += 4)
	{
		for (width = 0; width < SCR_WIDTH; width += 4)
		{
			/* Row one */
			*texture++ = rgb565[*src1++];
			*texture++ = rgb565[*src1++];
			*texture++ = rgb565[*src1++];
			*texture++ = rgb565[*src1++];

			/* Row two */
			*texture++ = rgb565[*src2++];
			*texture++ = rgb565[*src2++];
			*texture++ = rgb565[*src2++];
			*texture++ = rgb565[*src2++];

			/* Row three */
			*texture++ = rgb565[*src3++];
			*texture++ = rgb565[*src3++];
			*texture++ = rgb565[*src3++];
			*texture++ = rgb565[*src3++];

			/* Row four */
			*texture++ = rgb565[*src4++];
			*texture++ = rgb565[*src4++];
			*texture++ = rgb565[*src4++];
			*texture++ = rgb565[*src4++];
		}

		/* Skip 4 rows, set new offsets */
		src1 += SCR_WIDTH * 3;
		src2 += SCR_WIDTH * 3;
		src3 += SCR_WIDTH * 3;
		src4 += SCR_WIDTH * 3;
	}

	/* Load texture into GX */
	DCFlushRange(texturemem, TEX_WIDTH * TEX_HEIGHT * 4);

	/* Clear texture objects */
	GX_InvalidateTexAll();

	/* Render textured quad */
	draw_square(view);
	GX_DrawDone();

	/* EFB is ready to be copied into XFB */
	VIDEO_SetNextFramebuffer(xfb[whichfb]);
	VIDEO_Flush();
	copynow = GX_TRUE;

	/* Return to caller, don't waste time waiting for vb */
	LWP_ResumeThread (vbthread);
}

int main (int argc, char **argv)
{
	Init_GX_Video();

	PAD_Init();

#ifdef HW_RVL
	WPAD_Init();
#endif /* HW_RVL */

	Game.InitSoundDriver();
	Game.InitGame();
	Game.LoadScores();
	Game.StartGame();

	int run = 1;
	int game_paused = 0;
	int game_speed = 55;
	u32 last_time = gettime() / 1000;

	// Some globals
	int music_volume = 10; // max
	int sound_volume = 10; // max
	int players = 1;

	// load config here :)

	while(run)
	{
		// Get Controller state
		JOYSTICK *jptr1 = &Game.m_GameTarget.m_Joy1;
		JOYSTICK *jptr2 = &Game.m_GameTarget.m_Joy2;

		PAD_ScanPads();
		int bu_down, bu_up = 0;

		// Player 1
		bu_down = PAD_ButtonsDown(0);
		bu_up = PAD_ButtonsUp(0);

		// left
		if (PAD_StickX(0) <= -58)
			jptr1->left = 1;
		else
			jptr1->left = 0;

		// right
		if (PAD_StickX(0) >= 58)
			jptr1->right = 1;
		else
			jptr1->right = 0;

		// down
		if (PAD_StickY(0) <= -58)
			jptr1->down = 1;
		else
			jptr1->down = 0;

		// up
		if (PAD_StickY(0) >= 58)
			jptr1->up = 1;
		else
			jptr1->up = 0;

		// fire
		if (bu_down & PAD_BUTTON_A)
			jptr1->fire = 1;

		if (bu_up & PAD_BUTTON_A)
			jptr1->fire = 0;

		// Switch Player sprites
		if (bu_down & PAD_TRIGGER_Z)
			Game.m_GameTarget.m_Game.TogglePuffBlow();

		// Only Player 1 can Pause the Game
		if (bu_down & PAD_BUTTON_START)
			game_paused ^= 1;

		// DEBUG: alter game speed + warp levels
		if (bu_down & PAD_TRIGGER_L) {
			game_speed += 5;
			if (game_speed > 100)
				game_speed = 100;
		}
		if (bu_down & PAD_TRIGGER_R) {
			game_speed -= 5;
			if (game_speed < 10)
				game_speed = 10;
		}
		if ((PAD_SubStickX(0) > 60) && (PAD_SubStickY(0) <= -60))
			jptr1->next_level = 1;
		else
			jptr1->next_level = 0;

		// EXIT
		if (bu_down & PAD_TRIGGER_Z)
			run = 0;
		// DEBUG: end */

		// Player 2
		bu_down = PAD_ButtonsDown(1);
		bu_up = PAD_ButtonsUp(1);

		// left
		if (PAD_StickX(1) <= -58)
			jptr2->left = 1;
		else
			jptr2->left = 0;

		// right
		if (PAD_StickX(1) >= 58)
			jptr2->right = 1;
		else
			jptr2->right = 0;

		// down
		if (PAD_StickY(1) <= -58)
			jptr2->down = 1;
		else
			jptr2->down = 0;

		// up
		if (PAD_StickY(1) >= 58)
			jptr2->up = 1;
		else
			jptr2->up = 0;

		// fire
		if (bu_down & PAD_BUTTON_A)
			jptr2->fire = 1;

		if (bu_up & PAD_BUTTON_A)
			jptr2->fire = 0;

		// Fake a key press (to pass getPlayerName screen)
		jptr1->key = 13;

		// Add a delay
		int time_diff = 0;
		do
		{
			u32 time_now = gettime() / 1000;
			time_diff = diff_usec(last_time, time_now);
			time_diff = game_speed - time_diff;

			// Plenty of time left, sleep for a while
			if (time_diff > 10) usleep(1000);
		} while(time_diff > 0);
		last_time = gettime() / 1000;

		// Execute game logic
		Game.MainLoop(NULL, game_paused);
	}

	Game.SaveScores();

	// save config here :)

	Game.RemoveSoundDriver();

	DeInit_GX_Video();

	//return 0;
	exit(0);
}
