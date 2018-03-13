/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 * Project Email: rombust@postmaster.co.uk                                 *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// The RISCOS swi calls header file
//------------------------------------------------------------------------------

#ifndef _swicalls_h
#define _swicalls_h 1

#ifdef METHANE_OLD_CPP
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#else
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>
#endif

#include "kernel.h"
#include "swis.h"

//------------------------------------------------------------------------------
// QTM Defines (Included here just incase the QTM header is not available)
//------------------------------------------------------------------------------
#define SMB_QTM_Clear		0x47E44
#define SMB_QTM_Load		0x47E40
#define SMB_QTM_PlayRawSample	0x47E57
#define SMB_QTM_SoundControl	0x47E58
#define SMB_QTM_Start		0x47E41
#define SMB_QTM_Stereo 		0x47E4D
#define SMB_QTM_Stop 		0x47E42

//------------------------------------------------------------------------------
// General Stuff
//------------------------------------------------------------------------------
#define uchar unsigned char

// Offsets for the smb_keybank buffer
#define SMB_KB_LEFT	0
#define SMB_KB_RIGHT	1
#define SMB_KB_UP	2
#define SMB_KB_DOWN	3
#define SMB_KB_CHEAT	4
#define SMB_KB_FIRE	5
#define SMB_KB_A	6
#define SMB_KB_D	7
#define SMB_KB_W	8
#define SMB_KB_S	9
#define SMB_KB_SHIFT	10

#define SMB_KB_MAX	(10)

#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)
#define BIT8 (1<<8)

//------------------------------------------------------------------------------
// Video mode depths
//------------------------------------------------------------------------------
#define AV_16_COLOUR	2	// 16 Colour mode
#define AV_256_COLOUR	3	// 256 Colour mode
#define AV_32K_COLOUR	4	// 32K Colour mode
#define AV_16M_COLOUR	5	// 16M Colour mode

//------------------------------------------------------------------------------
// Gcol modes
//------------------------------------------------------------------------------

#define OVER	 0x0
#define OR	 0x1
#define AND	 0x2
#define EOR	 0x3
#define INV	 0x4
#define NOP	 0x5
#define NAND	 0x6
#define NOR	 0x7

#define TRANSPARENT 0x8
#define BACKGROUND 0x80

#define PATTERN1	0x10
#define PATTERN2	0x20
#define PATTERN3	0x40
#define PATTERN4	0x80
#define GIANT		0x90

//------------------------------------------------------------------------------
// Sound Player Stuff
//------------------------------------------------------------------------------
#define QTMPlayer_Recognise 0x4cEc0u
#define QTMPlayer_Load 0x4cec1u
#define QTMPlayer_Unload 0x4cec2u
#define QTMPlayer_Pause 0x4cec3u
#define QTMPlayer_Restart 0x4cec4u
#define QTMPlayer_Stop 0x4cec5u
#define QTMPlayer_Position 0x4cec6u
#define QTMPlayer_Sample 0x4cec7u
#define QTMPlayer_Volume 0x4cec8u
#define QTMPlayer_Quality 0x4cec9u
#define QTMPlayer_SoundSystem 0x4cecau
#define QTMPlayer_SongInfo 0x4cecbu
#define QTMPlayer_Info 0x4ceccu
#define QTMPlayer_Layer 0x4cecdu
#define QTMPlayer_Clear 0x4ceceu
#define QTMPlayer_CarouselInfo 0x4cecfu
#define QTMPlayer_CarouselPlay 0x4ced0u
#define QTMPlayer_CarouselStop 0x4ced1u
#define QTMPlayer_Poll 0x4ced2u
#define QTMPlayer_Choices 0x4ced3u
#define QTMPlayer_AddItem 0x4ced4u
#define QTMPlayer_DeleteItem 0x4ced5u
#define QTMPlayer_ReadItem 0x4ced6u
#define QTMPlayer_SetItem 0x4ced7u

//------------------------------------------------------------------------------
// Vdu modes
//------------------------------------------------------------------------------

#define PRINTER_ONLY	1
#define PRINTER_ON	2
#define PRINTER_OFF	3
#define SPLIT		4
#define JOIN		5
#define ENABLE		6
#define BELL		7
#define BACK		8
#define FORWARD		9
#define DOWN		10
#define LF		10
#define UP		11
#define FF		12
#define CLS		12
#define CR		13
#define PAGE_ON		14
#define PAGE_OFF	15
#define CLG		16
#define RESTORE_COL	20
#define DISABLE		21
#define RESTORE_WIND	26
#define HOME		30
#define DELETE		127

//------------------------------------------------------------------------------
// Sprite Handling
//------------------------------------------------------------------------------

#define UserSprites		256
#define UserSpritePointer	512
#define PutSprite	 	( 28 + UserSpritePointer )
#define PutAtSprite	 	( 34 + UserSpritePointer )
#define LoadSpriteFile	 	( 10 + UserSprites )
#define ReturnSpriteName	( 13 + UserSprites )
#define SelectSprite		( 24 + UserSprites )

//------------------------------------------------------------------------------
// Sprite Structures
//------------------------------------------------------------------------------

typedef struct _sprite_id {
	int   *sprite_addr;
	int   sprite_count;
	int   *sprite_ptrs;
	}Sprite_id;

//------------------------------------------------------------------------------
// A useful macro to rotate bits in a word
//------------------------------------------------------------------------------
#define ROR(x, n) (((x) << (32-(n))) | ((x) >> (n)))

//------------------------------------------------------------------------------
// A macro to convert 32k pixel (xBGR) to a 16m pixel (xxBBGGRR)
//------------------------------------------------------------------------------
#define VGFX_P32K_TO_P16M(value) ( ( ( value & 0x1f) << 3) | ( ( value & 0x3e0) << 6) | ( ( value & 0x7c00) << 9) )

//------------------------------------------------------------------------------
// A macro to convert 16m pixel (xxBBGGRR) to a 32k pixel(xBGR)
//------------------------------------------------------------------------------
#define VGFX_P16M_TO_P32K(value) ( ( ( value >> 3 ) & 0x1f ) | ( ( value >> 6 ) & 0x3e0 ) | ( ( value >> 9 ) & 0x7c00 ) )

//------------------------------------------------------------------------------
// A macro to convert 32k pixel (xBGR) to a 16m Colour (BBGGRRxx)
//------------------------------------------------------------------------------
#define VGFX_P32K_TO_C16M(value) ( ( ( value & 0x1f) << 11) | ( ( value & 0x3e0) << 14) | ( ( value & 0x7c00) << 17) )

//------------------------------------------------------------------------------
// A macro to convert 16m Colour (BBGGRRxx) to a 32k pixel (xBGR)
//------------------------------------------------------------------------------
#define VGFX_C16M_TO_P32K(value) ( ( ( value >> 11 ) & 0x1f ) | ( ( value >> 14 ) & 0x3e0 ) | ( ( value >> 17 ) & 0x7c00 ) )

//------------------------------------------------------------------------------
// A macro to convert a 16m BBGGRRxx Colour to a 16m xxBBGGRR pixel (PRM 5-88)
//------------------------------------------------------------------------------
#define VGFX_C16M_TO_P16M(value) (value >> 8)

//------------------------------------------------------------------------------
// A macro to convert a 16m xxBBGGRR pixel to a 16m BBGGRRxx Colour (PRM 5-88)
//------------------------------------------------------------------------------
#define VGFX_P16M_TO_C16M(value) (value << 8)

//------------------------------------------------------------------------------
// General Defines
//------------------------------------------------------------------------------

// This is the maximum length of a line being drawn.
// This is also the maximum width and height of a textured line in pixels
// (This value should NEVER decrease)
#define VGFX_MAX_LINE_LENGTH	(2048)

#define DECIMAL_POINT_BIT	20			// Bit where the decimal point is located
#define DECIMAL_POINT_VALUE	(1<<DECIMAL_POINT_BIT)	// Value where the decimal point is located
#define DECIMAL_POINT_VALUE2	((1<<DECIMAL_POINT_BIT)-1)	// Value where the decimal point is located minus one

//------------------------------------------------------------------------------
// Frame Lock flags (See smb_vgfx_framelock_wait() )
// (BITMASK)
//------------------------------------------------------------------------------
#define VGFX_FRMLOCK_DBLBUFFER	(BIT0)	// Swap the screen using smb_vgfx_dblbuffer_swap() (else smb_sync())

//------------------------------------------------------------------------------
// The Layer Flags (Bitmask)
//------------------------------------------------------------------------------
#define LAYER_ALLOC_BITMAP	(BIT0)	// The bitmap was allocated by this library
#define LAYER_PAGE_1		(BIT1)	// This layer is actually the entire screen - page 1
#define LAYER_PAGE_2		(BIT2)	// This layer is actually the entire screen - page 2
#define LAYER_WORD_ALIGNED	(BIT3)	// TRUE = The number of pixels in a line fits exactly into a multiple of a word (ie no wastage)
#define LAYER_SPRITE		(BIT4)	// TRUE = This layer contains a valid sprite
#define LAYER_TRANSPARENT	(BIT5)	// TRUE = This layer is transparent when writing to (IE colour 0 from the source is see-through)
#define LAYER_SOURCE_CLIPPED	(BIT6)	// TRUE = Clip when reading the source layer (else assumed client code will clip)
#define LAYER_DEST_CLIPPED	(BIT7)	// TRUE = Clip when reading the destination layer (else assumed client code will clip)
#define LAYER_REMAP		(BIT8)	// TRUE = This layer structure contains remapping information. See LAYER.palette_ptr and LAYER.palette_conversion_ptr
					// Important: The LAYER_REMAP flag MUST be set in 32K and 16M colours (done automatically by this library during layer creation)

#define LAYER_USER_FLAG1	(BIT12)	// User flag (Ignored by this library)
#define LAYER_USER_FLAG2	(BIT13)	// User flag (Ignored by this library)
#define LAYER_USER_FLAG3	(BIT14)	// User flag (Ignored by this library)
#define LAYER_USER_FLAG4	(BIT15)	// User flag (Ignored by this library)

//------------------------------------------------------------------------------
// The Layer Creation sprite information flags (Bitmask)
//------------------------------------------------------------------------------
#define SPRINFO_MODE_OLD	(BIT0)	// A sprite using the old mode number (See PRM 1-749)
#define SPRINFO_MODE_NEW	(BIT1)	// (Default) A sprite using the new mode number (See PRM 5-87)

#define SPRINFO_MASK_OLD	(BIT2)	// A mask in the same byte per pixel format as the image 
#define SPRINFO_MASK_NEW	(BIT3)	// A mask in 1 one byte per pixel format

// (Each colour uses 2 words, each word is an identical 0xBBGGRR00 colour - why? ask Acorn!!!)
#define SPRINFO_PALETTE		(BIT4)	// A palette (Entries set to zero)

// (Note: The Dots Per Inch are the same in vertical and horizontal (no need to separate them) )
#define SPRINFO_DPI_180		(BIT5)	// (Default if MODE_NEW) Dots Per Inch (same as X&Y EIG = 0 (PRM 5-81)) (MODE_NEW ONLY)
#define SPRINFO_DPI_90		(BIT6)	// (Default if MODE_OLD) DPI (same as X&Y EIG = 1)
#define SPRINFO_DPI_45		(BIT7)	// DPI (same as X&Y EIG = 2)
#define SPRINFO_DPI_22		(BIT8)	// DPI (same as X&Y EIG = 3) (MODE_NEW ONLY)

//------------------------------------------------------------------------------
// Defines used in sprite modes (See PRM 5-87)
// See the SPRINFO_DPI_xxx flags for more info
//------------------------------------------------------------------------------
#define VGFX_SPRMODE_C16_DPI180		((3<<27) | (180<<14) | (180<<1) | 1)	// 16 Colours, 180 DPI (Normal)
#define VGFX_SPRMODE_C16_DPI90		((3<<27) | (90<<14) | (90<<1) | 1)	// 16 Colours, 90 DPI (Apt Image style)
#define VGFX_SPRMODE_C16_DPI45		((3<<27) | (45<<14) | (45<<1) | 1)	// 16 Colours, 45 DPI
#define VGFX_SPRMODE_C16_DPI22		((3<<27) | (22<<14) | (22<<1) | 1)	// 16 Colours, 22 DPI

#define VGFX_SPRMODE_C256_DPI180	((4<<27) | (180<<14) | (180<<1) | 1)	// 256 Colours, 180 DPI (Normal)
#define VGFX_SPRMODE_C256_DPI90		((4<<27) | (90<<14) | (90<<1) | 1)	// 256 Colours, 90 DPI (Apt Image style)
#define VGFX_SPRMODE_C256_DPI45		((4<<27) | (45<<14) | (45<<1) | 1)	// 256 Colours, 45 DPI
#define VGFX_SPRMODE_C256_DPI22		((4<<27) | (22<<14) | (22<<1) | 1)	// 256 Colours, 22 DPI

#define VGFX_SPRMODE_C32K_DPI180	((5<<27) | (180<<14) | (180<<1) | 1)	// 32K Colours, 180 DPI (Normal)
#define VGFX_SPRMODE_C32K_DPI90		((5<<27) | (90<<14) | (90<<1) | 1)	// 32K Colours, 90 DPI (Apt Image style)
#define VGFX_SPRMODE_C32K_DPI45		((5<<27) | (45<<14) | (45<<1) | 1)	// 32K Colours, 45 DPI
#define VGFX_SPRMODE_C32K_DPI22		((5<<27) | (22<<14) | (22<<1) | 1)	// 32K Colours, 22 DPI

#define VGFX_SPRMODE_C16M_DPI180	((6<<27) | (180<<14) | (180<<1) | 1)	// 16M Colours, 180 DPI (Normal)
#define VGFX_SPRMODE_C16M_DPI90		((6<<27) | (90<<14) | (90<<1) | 1)	// 16M Colours, 90 DPI (Apt Image style)
#define VGFX_SPRMODE_C16M_DPI45		((6<<27) | (45<<14) | (45<<1) | 1)	// 16M Colours, 45 DPI
#define VGFX_SPRMODE_C16M_DPI22		((6<<27) | (22<<14) | (22<<1) | 1)	// 16M Colours, 22 DPI

//------------------------------------------------------------------------------
// The palette structure
//------------------------------------------------------------------------------
typedef struct _smb_RGB
	{
		// DO NOT CHANGE STRUCTURE ORDER - AN INT CAST IS USED TO CONVERT THE STRUCTURE INTO BBGGRR00
		// LIBRARY FUNCTIONS THAT USE THIS FUNCTION WILL ONLY WORK ON RISCOS
		uchar unused;	// MUST Keep at zero
		// Colour component from 0 to 255
		uchar red;
		uchar green;
		uchar blue;
	} smb_RGB;
typedef struct _SMB_PAL
	{
		int num_colours;	// Number of colours in the palette (max 256)
		smb_RGB *rgb;		// Pointer to the palette rgb's
	} SMB_PAL;

//------------------------------------------------------------------------------
// Standard OS sprite structure
//------------------------------------------------------------------------------
typedef struct		// Format of a sprite area control block
	{
		// All offsets are relative to the sprite area
		int size;	// Total size of the sprite area (including the sprites)
		int number;	// Number of sprite in area
		int sproff;	// Byte offset to first sprite
		int freeoff;	// Byte offset to first free word (ie byte after last sprite)
	} smb_VGFX_SPRITE_AREA;

typedef struct		// The new RiscOS format of a sprite header
	{
		int next;	// Offset to next sprite
		char name[12];	// Sprite name
		int width;	// Width in words-1
		int height;	// Height in scanlines-1
		int lbit;	// First bit used (Ignored by this library) (New Modes - Reserved - KEEP AT ZERO)
		int rbit;	// Last bit used (right end of row)
		int image;	// Offset to sprite image
		int mask;	// Offset to transparency mask (offset to sprite image if no mask)
		int mode;	// Mode sprite was defined in (PRM 5-87 and PRM 1-749)
			// Palette data optionally follows here in memory
	} smb_VGFX_SPRITE_HEADER;

//------------------------------------------------------------------------------
// The Layer Structure (Public access to this structure)
// This contains information about an image rectangle inside a bitmap
// Each line in the bitmap is word aligned
//------------------------------------------------------------------------------
typedef struct _LAYER
	{
		int flags;		// LAYER_xxx flags

		int width;		// Width of the image inside the layer bitmap in pixels
		int height;		// Height of the image inside the layer in pixels
		int byte_width;		// Width of the image inside the layer bitmap in bytes (next word aligned)
		int word_width;		// Width of the image inside the layer bitmap in words (next word aligned)

		// The 'true' variables refer to the actual memory size of a line in the bitmap
		// This is used if the image inside the bitmap is smaller than the bitmap
		// (ie if the bitmap is the visual screen)

		int true_word_width;	// Real width of the bitmap in words
		int true_byte_width;	// Real width of the bitmap in bytes
		int true_pixel_width;	// Real width of the bitmap in pixels

		int true_byte_size;	// Real memory size of the image (true_byte_width * height) (in bytes)

		int depth;		// Depth (AV_xxx_COLOUR)
		int colours;		// Number of colours (16,256,32K,16M)
		int **line_table_ptr;	// Pointer to the line table (0 = Does not exist)

		int *bitmap_ptr;	// The bitmap (0 = Does not exist) (MUST BE ON A WORD BOUNDARY)

		int *memory_base;	// PRIVATE (Contains a pointer to the bitmap or sprite for deallocating)

		// Pointer to this layer palette, this is used when remapping from a 16M palette
		// Note: The LAYER_REMAP flag needs set for remapping to take place
		// The function smb_vgfx_pal_create_remap_list() requires this pointer set in 16/256 colour layers
		SMB_PAL *palette_ptr;		// (0 = Does not exist)

		// Pointer to the conversion list which is used when remapping to convert a 16/256 colour image to this layer.
		// Note: The LAYER_REMAP flag needs set for remapping to take place
		// Note: When converting from 256 colour to 16M colour, this cannot be a cast from SMB_PAL.rgb,
		//       as the smb_RGB needs shifted right 8 bits - See VGFX_C16M_TO_P16M
		// See: smb_vgfx_pal_create_remap_list()
		int *palette_conversion_ptr;	// (0 = Does not exist) This list should contain 256 entries

		// If the layer contains a sprite - The structure below contains the smb_graph sprite information
		// (The LAYER_SPRITE flag is set)
		// (If any pointers are 0, the sprite LAYER_SPRITE flag is not set)
		Sprite_id sprite_base;	// Base of sprite area (compatible with smb_graph library) (Will only contain 1 sprite - used by this layer)
		smb_VGFX_SPRITE_AREA 	*spr_area;	// Sprite area pointer ( = origin_sptr->sprite_addr) (May contain many sprites)
		smb_VGFX_SPRITE_HEADER	*spr_header;	// Sprite header ( = sprite_base->sprite_ptrs ) (The sprite used by this layer)
		int spr_info;		// SPRINFO_xxx flags
		int *spr_mask_ptr;	// Pointer to mask (0 = Does not exist) (Format depends on the SPRINFO_MASK flags)
		int *spr_palette;	// Pointer to palette (0 = Does not exist) (Note: Each colour uses 2 words, each word is an identical 0xBBGGRR00 colour - why? ask Acorn!!!)

		// Info of where the layer sprite came from (may be useful?)
		Sprite_id *spr_origin;	// Base of sprite area (0 = Unknown) (created by smb_load_sprite) (May contain many sprites)
		int spr_origin_index;	// Layer is using this sprite index in spr_origin

	} LAYER;

//------------------------------------------------------------------------------
// Global data
//------------------------------------------------------------------------------
extern int SMB_Screen_Width;
extern int SMB_Screen_Height;
extern int SMB_Screen_HZ;
extern int SMB_Screen_Depth;
extern int QTM_Available;

// Our internal keyboard bank buffer translation codes
// 0 = Not Pressed
// 1 = Key Pressed
extern char SMB_Keybank_Buffer[SMB_KB_MAX+1];

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
extern int smb_readkey( int time );
extern int smb_getkeyready( void );
extern int smb_getkey( void );
extern void smb_keyrep_off( void );
extern void smb_keyrep_on( void );
extern void smb_cursor_off( void );
extern void smb_fastvdu( int vdu_code, int count );
extern void smb_clg( void );
extern void smb_sync( void );
extern void smb_setactivepage( int n );
extern void smb_setvisualpage( int n );
extern void smb_setgraphcol( int colour );
extern void smb_setscreen(int width, int height, int bpp, int framerate, int xeig, int yeig);
extern void smb_view_screen( void );
extern void smb_vgfx_layer_free(LAYER *lptr);
extern void smb_vgfx_baderror(char *txt, int num);
extern LAYER *smb_vgfx_layer_create_sprite(int width, int height, int depth, int flags);
extern void smb_vgfx_sprite_draw( LAYER *lptr, int xpos, int ypos, int gcol );
extern void smb_put_at_sprite( Sprite_id *sprite_info, int sprite_index, int x, int y, int GCOLaction );
extern LAYER *smb_vgfx_layer_alloc(int width, int true_word_width, int height, int depth);
extern void smb_windrestore( void );
extern void smb_vgfx_layer_realise(LAYER *lptr);
extern void smb_setpalette( int l , int p ,  int r ,  int g ,  int b );
extern void smb_keybank_flush(void);
extern void smb_keybank_scan(void);
extern int smb_keybank_getkey(void);
extern unsigned int smb_gettime( unsigned int compval );
extern void smb_snd_library_close(void);
extern void smb_snd_library_init(void);
extern void smb_snd_play_module(unsigned char *mptr);
extern void smb_snd_play_sample(unsigned char *sptr, int len, int chan, int note, int volume);
extern void smb_snd_stereo(int chan, int value);

#endif



