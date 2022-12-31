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

//------------------------------------------------------------------------------
// GameTarget class. Contains a wrapper to connect the game to the OS (Source File)
//------------------------------------------------------------------------------

#include <stdexcept>
#include <unistd.h>
#include "precomp.h"
#include "target.h"
#include "doc.h"
#include "assets.h"
#include "player.h"
#include "misc.h"
#include "objtypes.h"

//------------------------------------------------------------------------------
// The game target (Yuck global!)
// Thus - Only a single GameTarget is allowed
//------------------------------------------------------------------------------
CGameTarget *GLOBAL_GameTarget = 0;

static bool in_frame = false;
static int sprites_counter;
static int background_sprite;
static u32 last_frames = 0;
static u64 last_frame_time = 0;
static C2D_TextBuf g_dynamicBuf;

#define SAMPLERATE 44110
#define SAMPLESPERBUF (SAMPLERATE / 30)
#define BYTESPERSAMPLE 4
#define MUSIC_CHANNEL 23
#define NUM_MUSICBUFFS 2
static ndspWaveBuf musicBuf[NUM_MUSICBUFFS];
static u32 *musicBuffer;
volatile int musicBufFill = 0;
#define AUDIOTHREAD_STACKSIZE (32 * 1024)
static LightLock audio_lock;
static LightEvent audio_event;
static Thread audio_thread;
static bool term_stream = false;

void n3ds_dspcallback(void*) {
	if(LightLock_TryLock(&audio_lock) != 0) return;

	if (musicBuf[musicBufFill].status == NDSP_WBUF_DONE)
		LightEvent_Signal(&audio_event);

	LightLock_Unlock(&audio_lock);
}

void n3ds_audiothread(void*) {
	auto fillaudio = [](void *buffer, size_t size) {
		VC_WriteBytes((SBYTE *) buffer, size);
		DSP_FlushDataCache(buffer, size);
	};

	while (!term_stream) {
		LightEvent_Wait(&audio_event);

		LightLock_Lock(&audio_lock);
		fillaudio(musicBuf[musicBufFill].data_pcm16, musicBuf[musicBufFill].nsamples * BYTESPERSAMPLE);
		ndspChnWaveBufAdd(MUSIC_CHANNEL, &musicBuf[musicBufFill]);
		musicBufFill = (musicBufFill + 1) % NUM_MUSICBUFFS;
		LightLock_Unlock(&audio_lock);
	}
	threadExit(0);
}

int n3ds_getPlayingChannels() {
	int channels = 0;
	for (int i = 0; i <= MUSIC_CHANNEL; i++) {
		if(ndspChnIsPlaying(i)){
			channels++;
		}
	}
	return channels;
}

#define NUM_TICKSAMPLES 50
int tickIndex = 0;
int tickSum = 0;
int tickList[NUM_TICKSAMPLES];
void n3ds_calcAverageFPS(int newTick) {
	tickSum -= tickList[tickIndex];
	tickSum += newTick;
	tickList[tickIndex] = newTick;
	tickIndex = (tickIndex + 1) % NUM_TICKSAMPLES;
}

float n3ds_getAverageFps() {
	return ((float)tickSum / NUM_TICKSAMPLES);
}

//------------------------------------------------------------------------------
//! \brief Constructor
//------------------------------------------------------------------------------
CGameTarget::CGameTarget()
{
	// Clear the joystick!
	memset(&m_Joy1, 0, sizeof(m_Joy1));
	memset(&m_Joy2, 0, sizeof(m_Joy2));

	m_FadeChangeFlag = 0;	// Palete has not changed

	GLOBAL_GameTarget = this;	// The global target

	m_Lighting = 0.0;
}

//------------------------------------------------------------------------------
//! \brief Initialisation
//!
//! 	\param pdoc = Pointer to the document this target belongs to
//!	\param canvas = Screen to draw to
//------------------------------------------------------------------------------
void CGameTarget::Init(CMethDoc *pdoc)
{
	m_Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	m_Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	m_pDoc = pdoc;
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CGameTarget::InitGame()
{
	m_Game.Init(this, &m_Joy1, &m_Joy2);

	bool ok;
	if(C3D_TexInitVRAM(&m_Canvas.tex, 512, m_Canvas.subtex.height, GPU_RGBA8)) {
		C3D_TexSetFilter(&m_Canvas.tex, GPU_NEAREST, GPU_NEAREST);
		m_Canvas.target = C3D_RenderTargetCreateFromTex(&m_Canvas.tex, GPU_TEXFACE_2D, 0, -1);
		if(m_Canvas.target) {
			m_Canvas.image = { &m_Canvas.tex, &m_Canvas.subtex };
			ok = true;
		}
	}
	if(!ok) {
		ErrorOut("Couldn't create canvas");
	}

	blocks = C2D_SpriteSheetLoadFromMem(blocks_t3x, blocks_t3x_size);
	sprites = C2D_SpriteSheetLoadFromMem(sprites_t3x, sprites_t3x_size);

	g_dynamicBuf = C2D_TextBufNew(4096);
}

void CGameTarget::DeinitGame()
{
	C2D_TextBufDelete(g_dynamicBuf);

	C2D_SpriteSheetFree(sprites);
	C2D_SpriteSheetFree(blocks);

	C3D_RenderTargetDelete(m_Canvas.target);
	C3D_TexDelete(&m_Canvas.tex);
}

// based on CGame::DrawPlayer(s)Info methods
void CGameTarget::DrawPlayerInfo() {
	CPlayerObj *play = m_Game.GetPlayer(OBJ_PLAYER_ONE);

	auto draw = [this](float x, float y, int sprite, float size = 4.f, float z = 0.5f) {
		C2D_ImageTint tint;
		C2D_AlphaImageTint(&tint, 1.0f - m_Game.m_PanelOrigin/31.f);

		if(m_Game.m_MainCounter > 0) {
			C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites,
				sprite-SPR_START_NUMBER), x, y, z, &tint, size, size);

			sprites_counter++;
		}
	};

	char txtbuffer[16];
	char *txtptr;

	auto message = [&](const char *msg) {
		int xpos = 0;
		int ypos = 100;
		txtbuffer[0] = 0;
		snprintf(txtbuffer, sizeof(txtbuffer), "%s", msg);
		int len = (int) ::strlen(txtbuffer);
		float size = 2.f;
		xpos = (320/2) - ((len*size*16)/2);
		for (int cnt=0; cnt<len; cnt++) {
			int offset = txtbuffer[cnt] - ' ';
			if ((offset>0) && (offset<NUM_FONT_TABLE)) {
				draw(xpos, ypos, font_table[offset], size, 0.7f);
			}
			xpos+=16*size;
		}
	};

	if (m_Game.IsGameOver()) {
		// gameover message
		message("GAME OVER");
	} else if (play) { 	// in game
		int xpos = 16, ypos = 16;

		// Draw level number left
		int level = m_Game.m_LevelNumber;
		//if(m_Lighting != 0.0f) level--;
		if ((!m_Game.m_BonusLevelFlag) && (level < 100)) {
			draw(xpos, ypos, SPR_FNT_Y1 + (level / 10) - 1);
			draw(xpos + 33, ypos, SPR_FNT_Y1 + (level % 10) - 1);
		}

		// Draw lives and heart right
		xpos = 320 - 33 - 33 - 36 - 16;
		draw(xpos, ypos+1, SPR_FNT_R0 + 10);
		xpos+=36;
		int lives = play->m_Lives < 99 ? play->m_Lives : 99;
		draw(xpos, ypos, SPR_FNT_R0 + (lives / 10));
		draw(xpos+33, ypos, SPR_FNT_R0 + (lives % 10));

		// Draw scores centered, second line
		ypos =+ 72;
		txtbuffer[0] = 0;
		snprintf(txtbuffer, sizeof(txtbuffer), "%.8u", play->m_Score);
		int len = (int) ::strlen(txtbuffer);
		xpos = (320/2) - ((len*28)/2);
		txtptr = txtbuffer;
		while (*txtptr)	{
			draw(xpos, ypos, SPR_FNT_R0 + (*(txtptr++)) - '0', 3.f);
			xpos += 28;
		}

		// Level name, third line
		ypos += 50;
		len = (int) ::strlen(level_text_names[level-1]);
		float size = 1.8f;
		do {
			size -= 0.2f;
			xpos = (320/2) - ((len*size*16)/2);
		} while (xpos < 0 && xpos + (len*size*16)>320);
		for (int cnt=0; cnt<len; cnt++) {
			int offset = level_text_names[level-1][cnt] - ' ';
			if ((offset>0) && (offset<NUM_FONT_TABLE)) {
				draw(xpos, ypos, font_table[offset], size);
			}
			xpos+=16*size;
		}

		// Draw 4 Cards, centered, last line
		xpos = (320/2) - ((4*54)/2);
		ypos += 40;
		int cards = play->m_Cards;
		for (int card = SPR_CARD_SSPADE; card < SPR_CARD_SSPADE + 4; card++) {
			if (cards&1) draw(xpos, ypos, card);
			xpos += 54;
			cards = cards >> 1;
		}
	} else {

	}
}

//------------------------------------------------------------------------------
//! \brief Redraw screen (Called by the game)
//------------------------------------------------------------------------------
void CGameTarget::RedrawScreen()
{
	C2D_Image background = C2D_SpriteSheetGetImage(blocks, background_sprite);
	auto draw_background = [&]() {
		if(background_sprite > -1)
			C2D_DrawImageAt(background, -10, -10, 0.3f, nullptr, 500.f, 300.f);
	};

	// draw game screen top
	C2D_SceneBegin(m_Top);
	draw_background();

	// scale content a bit when ingame
	float scale_x = 1.f;
	float scale_y = 1.f;
	if (m_Game.IsInGame()) {
		scale_x = 1.08f;
		scale_y = 1.04f;
	}
	C2D_DrawImageAt(m_Canvas.image, (400 - SCR_WIDTH*scale_x)/2,
		(240 - SCR_HEIGHT*scale_y)/2, 0.8f, nullptr, scale_x, scale_y);

	// draw information bottom
	C2D_SceneBegin(m_Bottom);
	draw_background();
	C2D_TextBufClear(g_dynamicBuf);
	if (m_Game.IsInGame()) DrawPlayerInfo();

	// debug
#if DEBUG
	C2D_Text dynText;
	static char buf[200];
	C2D_DrawRectSolid(0, 210, 0.5f, 320, 60, C2D_Color32(0xB0, 0xD8, 0x68, 0xAA));

	snprintf(buf, sizeof(buf), "FPS: %0.2f | Spr: %d | Fade: %0.1f",
		n3ds_getAverageFps(), sprites_counter, m_Lighting);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_AtBaseline, 20.0f, 222.0f, 0.8f, 0.5f, 0.5f);
	snprintf(buf, sizeof(buf), "DSP Chans: %d/%d | BGM Buf: %d/%d | Drops: %ld",
		n3ds_getPlayingChannels(), MUSIC_CHANNEL, musicBufFill+1, NUM_MUSICBUFFS,
		ndspGetDroppedFrames());
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_AtBaseline, 20.0f, 237.0f, 0.8f, 0.5f, 0.5f);
#endif

	// fading
	float color, alpha;
	if(m_Lighting >= 0.0f) {
		color = 1;
		alpha = m_Lighting;
	} else {
		color = 0;
		alpha = m_Lighting * -1;
	}
	C2D_Fade(C2D_Color32f(color, color, color, alpha));

	// show
	in_frame = false;
	C3D_FrameEnd(0);
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CGameTarget::StartGame()
{
	m_Game.StartGame();
}

//------------------------------------------------------------------------------
//! \brief Do the game main loop (Call every cycle)
//------------------------------------------------------------------------------
void CGameTarget::MainLoop()
{
	in_frame = true;
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

	// clear
	C2D_TargetClear(m_Canvas.target, C2D_Color32f(0, 0, 0, 1));
	C2D_TargetClear(m_Top, C2D_Color32f(0, 0, 0, 1));
	C2D_TargetClear(m_Bottom, C2D_Color32f(0, 0, 0, 1));
	sprites_counter = 0;
	background_sprite = -1;

	C2D_SceneBegin(m_Canvas.target);

	m_Game.MainLoop();

	// in case game is fading, update screen manually
	if(in_frame)
		RedrawScreen();

	// calculate FPS
	u32 counter = C3D_FrameCounter(0);
	u32 frames = counter - last_frames;
	last_frames = counter;
	u64 time = osGetTime();
	u64 frame_time = time - last_frame_time;
	last_frame_time = time;
	n3ds_calcAverageFPS(frames / (frame_time / 1000.0f));
}

//------------------------------------------------------------------------------
//! \brief Prepare the sound driver (call before the game starts)
//------------------------------------------------------------------------------
void CGameTarget::PrepareSoundDriver()
{
	m_Game.m_Sound.PrepareAudio();

	if (!GLOBAL_SoundEnable)
		return;

	MikMod_InitThreads();
	MikMod_RegisterDriver(&drv_nos);
	MikMod_RegisterLoader(&load_mod);

	md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX | DMODE_NOISEREDUCTION;
	md_mixfreq = SAMPLERATE;
	if (MikMod_Init("")) {
		ErrorOut("Could not initialize MikMod");
	}

	musicBuffer = (u32*)linearAlloc(SAMPLESPERBUF * BYTESPERSAMPLE * NUM_MUSICBUFFS);

	// Initialise the audio subsystem
	ndspInit();
	ndspSetOutputCount(1);
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspSetClippingMode(NDSP_CLIP_NORMAL);
	ndspChnReset(MUSIC_CHANNEL);
	ndspChnSetInterp(MUSIC_CHANNEL, NDSP_INTERP_LINEAR);
	ndspChnSetRate(MUSIC_CHANNEL, md_mixfreq);
	ndspChnSetFormat(MUSIC_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

	memset(musicBuf, 0, sizeof(musicBuf));
	for (int i = 0; i<NUM_MUSICBUFFS; i++) {
		musicBuf[i].data_vaddr = &musicBuffer[i*SAMPLESPERBUF];
		musicBuf[i].nsamples = SAMPLESPERBUF;
		musicBuf[i].status = NDSP_WBUF_DONE;
	}

	LightEvent_Init(&audio_event, RESET_ONESHOT);
	LightLock_Init(&audio_lock);

	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	audio_thread = threadCreate(n3ds_audiothread, nullptr, AUDIOTHREAD_STACKSIZE, prio-1, 1, true);
	if(!audio_thread) {
		ErrorOut("Couldn't create bgm thread.");
	}

	// load music
	for (int i = 0; i < MODULE_COUNT-1; i++) {
		m_Musicfile[i] = Player_LoadMem((const char*)assets[i], asset_sizes[i], 64, 0);
		if(!m_Musicfile[i]) {
			ErrorOut("Couldn't load music.");
		}
		m_Musicfile[i]->wrap = true;
	}

	// load samples
	memset(m_Soundfile, 0, sizeof(m_Soundfile));
	for (int i = 0; i < SND_COUNT; i++) {
		size_t s = asset_sizes[ASSET_RAWSTART + i];
		u8* data = (u8*)linearAlloc(s);
		if(!data) {
			ErrorOut("Couldn't load sample.");
		}
		memcpy(data, assets[ASSET_RAWSTART + i], s);
		m_Soundfile[i].data_vaddr = data;
		m_Soundfile[i].nsamples = s;
		m_Soundfile[i].status = NDSP_WBUF_FREE;
		DSP_FlushDataCache(data, s);
	}

	// start decoding music
	LightEvent_Signal(&audio_event);
	ndspSetCallback(n3ds_dspcallback, nullptr);
}

void CGameTarget::FreeSoundDriver()
{
	if (!GLOBAL_SoundEnable)
		return;

	term_stream = true;
	LightEvent_Signal(&audio_event);
	threadJoin(audio_thread, U64_MAX);
	LightEvent_Clear(&audio_event);

	for (int i = 0; i <= MUSIC_CHANNEL; i++)
		ndspChnWaveBufClear(i);

	for (int i = 0; i < SND_COUNT; i++)
		linearFree((void*)m_Soundfile[i].data_vaddr);
	for (int i = 0; i < MODULE_COUNT-1; i++)
		Player_Free(m_Musicfile[i]);

	MikMod_Exit();

	ndspExit();
	linearFree(musicBuffer);
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CGameTarget::PlayModule(int id)
{
	if (!GLOBAL_SoundEnable)
		return;

	// no need to play empty music
	if (id == SMOD_EMPTY) {
		StopModule();
		return;
	}

	// determine music
	//MODULE *old = Player_GetModule();
	MODULE *music = m_Musicfile[id - MODULE_START_NUMBER];
	if (!music) {
		ErrorOut("PlayModule: Unknown module");
		return;
	}

	// keep playing, if next song is the same
	//if(old == music) return;

	Player_SetPosition(0);
	Player_Stop();
	Player_Start(music);
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CGameTarget::StopModule()
{
	if (!GLOBAL_SoundEnable)
		return;

	Player_Stop();
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CGameTarget::PlaySample(int id, int pos, int rate)
{
	if (!GLOBAL_SoundEnable)
		return;

	int channel = -1;
	for (int i = 0; i < MUSIC_CHANNEL; i++) {
		if(!ndspChnIsPlaying(i)) {
			channel = i;
		}
	}
	if(channel == -1) {
		printf("No free channels.\n");
		return;
	}

	ndspChnReset(channel);
	ndspChnSetInterp(MUSIC_CHANNEL, NDSP_INTERP_LINEAR);
	ndspChnSetRate(channel, rate);
	ndspChnSetFormat(channel, NDSP_FORMAT_MONO_PCM8);

	float mix[12] = {0};
	mix[0] = 0.9f - pos/255.f;
	mix[1] = 0.9f * pos/255.f;
	ndspChnSetMix(channel, mix);

	ndspChnWaveBufAdd(channel, &m_Soundfile[id - SND_START_NUMBER]);
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CGameTarget::UpdateModule(int id)
{
}

//------------------------------------------------------------------------------
//! \brief Draw
//! 
//!	\param 
//! 
//!	\return 
//------------------------------------------------------------------------------
void CGameTarget::Draw(int dest_xpos, int dest_ypos, int sprite_number, bool draw_white, bool is_block) {
	C2D_ImageTint tint;
	if (draw_white) C2D_PlainImageTint(&tint, C2D_Color32f(1, 1, 1, 1), 0.5f);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(is_block?blocks:sprites, sprite_number),
		dest_xpos, dest_ypos, 0.5f, draw_white?&tint:nullptr);

	// save topleft sprite for background drawing
	if (is_block && !dest_ypos && !dest_xpos && background_sprite < 0) {
		background_sprite = sprite_number;
	}
	sprites_counter++;
}

void CGameTarget::ErrorOut(const char *message) {
	errorConf err;

	// HLE in citra emulator
	svcOutputDebugString(message, strlen(message));

	errorInit(&err, ERROR_TEXT_WORD_WRAP, CFG_LANGUAGE_EN);
	errorText(&err, message);
	errorDisp(&err);

	throw std::runtime_error(message);
}

static SwkbdCallbackResult MyCallback(void* user, const char** ppMessage, const char* text, size_t textlen) {
	for (unsigned int i = 0; i < textlen; i++) {
		char c = text[i];
		if(!((c >= '0' && c <= '9') ||
			(c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
			c == ' ' || c == '!' || c == '?')) {
			*ppMessage = "Only a-z, 0-9, ?, ! and space are accepted.";
			return SWKBD_CALLBACK_CONTINUE;
		}
	}

	return SWKBD_CALLBACK_OK;
}

void CGameTarget::GetName(char *name) {
	SwkbdState swkbd;

	swkbdInit(&swkbd, SWKBD_TYPE_QWERTY, 2, 4);
	swkbdSetInitialText(&swkbd, name);
	swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, SWKBD_FILTER_CALLBACK, 0);
	swkbdSetFeatures(&swkbd, SWKBD_DEFAULT_QWERTY|SWKBD_DARKEN_TOP_SCREEN);
	swkbdSetHintText(&swkbd, "Enter 4 symbols nickname...");
	swkbdSetFilterCallback(&swkbd, MyCallback, nullptr);
	swkbdInputText(&swkbd, name, 5);

	for (int i = 0; i < 5; i++) {
		name[i] = toupper(name[i]);
	}
}
