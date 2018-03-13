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
// The RISCOS swi calls file
//------------------------------------------------------------------------------
#include "swicalls.h"
#include "signal.h"
#include "kernel.h"

//------------------------------------------------------------------------------
// The below char is a buffer used by many smb_graph related functions
// DO NOT REDUCE - ONLY MAKE LARGER
//------------------------------------------------------------------------------
static char VDU_Buffer[64] = {0};

//------------------------------------------------------------------------------
// The QTM sound driver status
//------------------------------------------------------------------------------
int QTM_Available = 0;

//------------------------------------------------------------------------------
// The screen size
//------------------------------------------------------------------------------
int SMB_Screen_Width = 1024;
int SMB_Screen_Height = 768;
int SMB_Screen_HZ = 70;
int SMB_Screen_Depth = AV_16_COLOUR;

//------------------------------------------------------------------------------
// The signal handlers. Note pointers <=0 are invalid
//------------------------------------------------------------------------------
static void ((*SMB_SND_Signal_Handler[7])(int))={0,0,0,0,0,0,0};
static int SMB_SND_Library_Active = 0;

_kernel_oserror *SwiError = 0;

//------------------------------------------------------------------------------
// Private Globals
//------------------------------------------------------------------------------
static int KeyRepValue = 8;
static int KeyRepRate = 32;

//------------------------------------------------------------------------------
// Our internal keyboard bank buffer translation codes
// 0 = Not Pressed
// 1 = Key Pressed
//------------------------------------------------------------------------------
char SMB_Keybank_Buffer[SMB_KB_MAX+1];

//------------------------------------------------------------------------------
//! \brief Display a bad error
//!
//! 	\param txt = The text
//!	\param num = The number
//------------------------------------------------------------------------------
void smb_vgfx_baderror(char *txt, int num)
{
	while(1)
	{
		printf("%s : %d\n", txt, num);
	}
}

//------------------------------------------------------------------------------
//! \brief Display a swi error
//!
//! Does not exit if eptr is not 0;
//!
//! 	\param eptr = The swi error (0 = Ignore)
//------------------------------------------------------------------------------
void swi_check(_kernel_oserror *eptr)
{
	if (eptr)
	{
		while(1)
		{
			printf("%s : %d\n", eptr->errmess, eptr->errnum);
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Release the all signal handlers (Private Function)
//------------------------------------------------------------------------------
static void smb_snd_signal_release(void)
{
	int cnt;

	if (SMB_SND_Library_Active)	// Only if library active
	{
		// Set the signal handlers to default if unneeded
		for (cnt=0; cnt<7; cnt++)
		{
			if (( (int) SMB_SND_Signal_Handler[cnt]) <=0 )
			{
				SMB_SND_Signal_Handler[cnt] = SIG_DFL;
			}
		}

		// Note, this order should correspond with smb_snd_signal_init
		signal( SIGABRT, SMB_SND_Signal_Handler[0] );
		signal( SIGFPE, SMB_SND_Signal_Handler[1] );
		signal( SIGILL, SMB_SND_Signal_Handler[2] );
		signal( SIGINT, SMB_SND_Signal_Handler[3] );
		signal( SIGSEGV, SMB_SND_Signal_Handler[4] );
		signal( SIGTERM, SMB_SND_Signal_Handler[5] );
		signal( SIGSTAK, SMB_SND_Signal_Handler[6] );

		// Clear the handler pointers
		for (cnt=0; cnt<7; cnt++) SMB_SND_Signal_Handler[cnt] = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief The Signal Handler (Private Function)
//!
//! 	\param sig = The signal number
//------------------------------------------------------------------------------
static void smb_snd_signal(int sig)
{
	void (*sigfunx)(int sig);

	sigfunx = 0;	// Default to unknown
	if (SMB_SND_Library_Active)	// Only if library active
	{
		// Get the old signal function
		switch (sig)
		{
			case SIGABRT:
					sigfunx = SMB_SND_Signal_Handler[0];
					break;
			case SIGFPE:
					sigfunx = SMB_SND_Signal_Handler[1];
					break;
			case SIGILL:
					sigfunx = SMB_SND_Signal_Handler[2];
					break;
			case SIGINT:
					sigfunx = SMB_SND_Signal_Handler[3];
					break;
			case SIGSEGV:
					sigfunx = SMB_SND_Signal_Handler[4];
					break;
			case SIGTERM:
					sigfunx = SMB_SND_Signal_Handler[5];
					break;
			case SIGSTAK:
					sigfunx = SMB_SND_Signal_Handler[6];
					break;
		}

		// Restore key repeat on abnormal program termination
		smb_keyrep_on();
		smb_snd_library_close();
	}

	if (( (int) sigfunx) >= 0)	// Got a callback function
	{
		sigfunx(sig);	// Call it
	}
	raise(sig);	// Call the default handler
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound library
//!
//! This function MUST be called before using any functions of this sound library\n
//! Note: smb_snd_library_close MUST be called at the end of the program
//------------------------------------------------------------------------------
void smb_snd_library_init(void)
{
	_kernel_swi_regs regs ;
	smb_snd_library_close();

	// Note, this order should correspond with aptsnd_signal_release
	SMB_SND_Signal_Handler[0] = signal( SIGABRT, smb_snd_signal );
	SMB_SND_Signal_Handler[1] = signal( SIGFPE, smb_snd_signal );
	SMB_SND_Signal_Handler[2] = signal( SIGILL, smb_snd_signal );
	SMB_SND_Signal_Handler[3] = signal( SIGINT, smb_snd_signal );
	SMB_SND_Signal_Handler[4] = signal( SIGSEGV, smb_snd_signal );
	SMB_SND_Signal_Handler[5] = signal( SIGTERM, smb_snd_signal );
	SMB_SND_Signal_Handler[6] = signal( SIGSTAK, smb_snd_signal );

	SMB_SND_Library_Active = 1;	// Flag library is active

	regs.r[ 0 ] = 0x8;
	regs.r[ 1 ] = 0;
	regs.r[ 2 ] = -1;

	SwiError = _kernel_swi( SMB_QTM_SoundControl , &regs , &regs );			// Do command
	if (SwiError)
	{
		QTM_Available = 0;
	}else
	{
		QTM_Available = 1;
	}
}

//------------------------------------------------------------------------------
//! \brief Close the sound library
//!
//! This function MUST be called at the end of the program
//------------------------------------------------------------------------------
void smb_snd_library_close(void)
{
	_kernel_swi_regs regs ;

	if (SMB_SND_Library_Active)		// Only attempt to close if library is active
	{
		smb_snd_signal_release();	// Release the signal handlers
		SMB_SND_Library_Active = 0;
	}

	if (QTM_Available)
	{
		_kernel_swi( SMB_QTM_Stop , &regs , &regs );			// Do command

		regs.r[0] = 0;
		_kernel_swi( SMB_QTM_Clear , &regs , &regs );			// Do command
	}

}

//------------------------------------------------------------------------------
//! \brief Play a module
//!
//! 	\param mptr = The module
//------------------------------------------------------------------------------
void smb_snd_play_module(unsigned char *mptr)
{
	_kernel_swi_regs regs ;

	if (QTM_Available)
	{

		regs.r[ 0 ] = -1;
		_kernel_swi( SMB_QTM_Clear , &regs , &regs );			// Do command
	
		regs.r[ 0 ] = 0;
		regs.r[ 1 ] = (int) mptr;

		SwiError = _kernel_swi( SMB_QTM_Load , &regs , &regs );			// Do command
		swi_check(SwiError);

		SwiError = _kernel_swi( SMB_QTM_Start , &regs , &regs );			// Do command
		swi_check(SwiError);
	}
}

//------------------------------------------------------------------------------
//! \brief Play a sample
//!
//! 	\param sptr = The sample address (logarithmic sample)
//!	\param len = The sample length
//!	\param chan = Channel number (5,6,7 or 8)
//!	\param note = Note number
//!	\param volume = volume 0 - 64
//------------------------------------------------------------------------------
void smb_snd_play_sample(unsigned char *sptr, int len, int chan, int note, int volume)
{
	_kernel_swi_regs regs ;

	if (QTM_Available)
	{

		regs.r[ 0 ] = chan;
		regs.r[ 1 ] = (int) sptr;
		regs.r[ 2 ] = len;
		regs.r[ 3 ] = 0;
		regs.r[ 4 ] = 0;
		regs.r[ 5 ] = note;
		regs.r[ 6 ] = volume;

		_kernel_swi( SMB_QTM_PlayRawSample , &regs , &regs );			// Do command
	}
}

//------------------------------------------------------------------------------
//! \brief Read a key within a time limit (See PRM 1-870)
//!
//!	\param Time to wait for in centiseconds - max 32767
//!
//!	\return Charactor read. ( -1 = timeout )
//------------------------------------------------------------------------------
int smb_readkey( int time )
{
	_kernel_swi_regs regs ;

	regs.r[ 0 ] = 0x081 ;					// Read keyboard command
	regs.r[ 1 ] = time & 0xff ;				// time low byte
	regs.r[ 2 ] = (time>>8) & 0x7f ;			// time high byte

	_kernel_swi( OS_Byte , &regs , &regs );			// Do command

	return( ( int ) ( regs.r[ 2 ] == 0 ) ? regs.r[ 1 ] : -1 );	// Return result
}

//------------------------------------------------------------------------------
//! \brief Looks to see if a character is in the buffer (See PRM 1-168)
//!
//!	\return 0 = No character in buffer. x = Preview of key in buffer (not taken out)
//------------------------------------------------------------------------------
int smb_getkeyready( void )
{
	_kernel_swi_regs regs ;
	int key ;

	if( ( key = smb_readkey(0) ) != -1 )			// Check key
	{
		regs.r[ 0 ] = 0x08A ;				// Insert byte command
		regs.r[ 1 ] =     0 ;				// Keyboard buffer
		regs.r[ 2 ] =   key ; 				// Insert the read key
		_kernel_swi( OS_Byte , &regs , &regs );		// Insert a byte into buffer
		return( key );					// Return key
	}

	return( 0 );						// Return none found
}

//------------------------------------------------------------------------------
//! \brief Read a character from the input stream (See PRM 1-852)
//!
//! Note: When reading 'ABCD' from the keyboard, the character may be in
//! upper or lower case ( use key = toupper(key) (in ctype.h) ) 
//!
//!	\return Charactor read
//------------------------------------------------------------------------------
int smb_getkey( void )
{
	_kernel_swi_regs regs ;

	_kernel_swi( OS_ReadC , &regs , &regs );		// Read character

	return( regs.r[0] );					// return response
}

//------------------------------------------------------------------------------
//! \brief Sets the keyboard repeat off.
//!
//! Call the function ONCE at the start of the game\n
//! Global - KeyRepValue and KeyRepRate defined.\n
//! This has been changed by PFJ to use OS_Byte 196 and 197 instead of 11
//------------------------------------------------------------------------------
void smb_keyrep_off( void )
{
	_kernel_oserror *er;
	_kernel_swi_regs regs;

	regs.r[0]=197;
	regs.r[1]=0;
	regs.r[2]=0;
	er = _kernel_swi(OS_Byte,&regs,&regs);
	if (er == NULL)
	{
		KeyRepValue = regs.r[1];
	}else
	{
		KeyRepValue = 8;
	}

	regs.r[0]=196;
	regs.r[1]=0;
	regs.r[2]=0;
	er = _kernel_swi(OS_Byte,&regs,&regs);
	if (er == NULL)
	{
		KeyRepRate = regs.r[2];
	}else
	{
		KeyRepRate = 32;
	}
}

//------------------------------------------------------------------------------
//! \brief Sets the keyboard repeat on
//!
//! This has been added by PFJ
//------------------------------------------------------------------------------
void smb_keyrep_on( void )
{
	_kernel_swi_regs regs;

	regs.r[0] = 11;
	regs.r[1] = KeyRepValue;
	_kernel_swi(OS_Byte,&regs,&regs);

	regs.r[0] = 12;
	regs.r[1] = KeyRepRate;
	_kernel_swi(OS_Byte,&regs,&regs);
}

//------------------------------------------------------------------------------
//! \brief Set the text cursor off (See PRM 1-714)
//------------------------------------------------------------------------------
void smb_cursor_off( void )
{
	_kernel_swi_regs regs ;
	_kernel_swi( OS_RemoveCursors , &regs , &regs );
}


//------------------------------------------------------------------------------
//! \brief A very fast vdu call
//!
//! Use the global VDU_Buffer[48] to store the parameter
//!
//!	\param vdu_code = VDU Code
//!	\param count = Number of vdu parameters in the call
//------------------------------------------------------------------------------
void smb_fastvdu( int vdu_code, int count )
{
	_kernel_swi_regs regs ;

	regs.r[ 0 ] = vdu_code ;				// Set the command
	_kernel_swi( OS_WriteC , &regs , &regs );		// Start the command
	if (count)						// only if something is there
	{
		regs.r[0] = (int) VDU_Buffer;			// set string address
		regs.r[1] = count;				// size
		_kernel_swi( OS_WriteN , &regs , &regs );	// Send the command parameters
	}
}
//------------------------------------------------------------------------------
//! \brief Clear the graphics window (see PRM 1-564)
//------------------------------------------------------------------------------
void smb_clg( void )
{
	smb_fastvdu( CLG, 0 );					// Clear the graphics window
}

//------------------------------------------------------------------------------
//! \brief Wait for vertical sync (call this AFTER choosing visual page) (see PRM 1-631)
//------------------------------------------------------------------------------
void smb_sync( void )
{
	_kernel_osbyte( 19 , 0 , 0 );			// Wait for Vsync pulse
}

//------------------------------------------------------------------------------
//! \brief Activate a screen page (see PRM 1-639)
//!
//!	\param n = Page to activate (1 = first page)
//------------------------------------------------------------------------------
void smb_setactivepage( int n )
{
	_kernel_osbyte( 112 , n , 0 );			// Activate it
}

//------------------------------------------------------------------------------
//! \brief Display a screen page (see PRM 1-641)
//!
//!	\param Page to display (1 = first page)
//------------------------------------------------------------------------------
void smb_setvisualpage( int n )
{
	_kernel_osbyte( 113 , n , 0 );			// Display it
}

//------------------------------------------------------------------------------
//! \brief Set the graphics colour. (see PRM 1-566)
//!
//! Note: This also clears the graphics mode to 'OVERwrite'
//!
//!	\param Colour to use	(0-127 = foreground colour : 128-255 = background colour).
//!		(except in 256 colour mode) (BACKGROUND, defined to aptgraph.h = 128)
//------------------------------------------------------------------------------
void smb_setgraphcol( int colour )
{
	VDU_Buffer[0] = OVER;					// set action
	VDU_Buffer[1] = colour;					// set colour
	smb_fastvdu( 18, 2 );					// Set the colour
}

//------------------------------------------------------------------------------
//! \brief Set the screen specifier (See PRM 5-108 5-80)
//!
//! (Does not exit on an error)
//!
//! 	\param width = screen width
//!	\param height = screen height
//!	\param bpp = pixel depth (2 = 16 colour - See PRM 5-80) 
//!	\param framerate = frame rate Hz (-1 = Highest available) (75Hz or 60Hz)
//!	\param xeig = X Eig Factor (0 = None, 1 = Normal)
//!	\param yeig = Y Eig Factor (0 = None, 1 = Normal)
//!
//------------------------------------------------------------------------------
void smb_setscreen(int width, int height, int bpp, int framerate, int xeig, int yeig)
{
	_kernel_swi_regs regs ;
	static int mode_select[] =
	{
		1,				// flags
		0,0,0,				// width,height,pixel depth
		-1,				// frame rate
		4,0,				// XEig
		5,0,				// YEig
		0,0,				// Mode Flags - NOTE IF NOT 256 COLOURS, THIS IS SET TO -1
		3,0,				// NColour
		-1				// end of list
	};

	smb_windrestore();
	smb_setactivepage( 1 );
	smb_setgraphcol(0 | BACKGROUND);
	smb_clg();
	smb_setvisualpage( 1 );

	// Have an adjustable 256 colour palette (only in 256 colour mode)
	if (bpp==3)
	{
		mode_select[9] = 3;	// Mode item
		mode_select[10] = 128;	// Mode flags
		mode_select[12] = 255;	// 256 colours
	}else
	{
		mode_select[9] = -1;	// End of list item
	}
	
	mode_select[1] = width;			// store width
	mode_select[2] = height;		// store height
	mode_select[3] = bpp;			// store bpp
	mode_select[4] = framerate;		// store framerate
	mode_select[6] = xeig;			// store X Eig Factor
	mode_select[8] = yeig;			// store Y Eig Factor

	regs.r[0] = 0 ;				// Reason Code
	regs.r[1] = (int) mode_select;		// Mode Specifier
	_kernel_swi( 0x65 , &regs , &regs );	// do command

	smb_cursor_off();			// Remove the cursor by default
}

//------------------------------------------------------------------------------
//! \brief Set up the standard common image screen
//------------------------------------------------------------------------------
void smb_view_screen( void )
{
	smb_setscreen( SMB_Screen_Width, SMB_Screen_Height, SMB_Screen_Depth, SMB_Screen_HZ, 0, 0 );	// Set up the screen mode
	smb_cursor_off();						// Cursor off
	smb_clg();							// Clear the screen for a blue background
}

//------------------------------------------------------------------------------
//! \brief De-allocate a layer structure
//!
//! 	\param lptr = The layer (Can be null)
//------------------------------------------------------------------------------
void smb_vgfx_layer_free(LAYER *lptr)
{
	if (lptr)	// Only if exists
	{
		if (lptr->flags & LAYER_ALLOC_BITMAP)	// Has the library allocated the bitmap?
		{
			if (lptr->memory_base)		// Does a bitmap exist?
			{
				free(lptr->memory_base);	// Deallocate the bitmap
			}
		}
		if (lptr->line_table_ptr)	// A line table exists
		{
			free(lptr->line_table_ptr);	// Deallocate it
		}
		free(lptr);	// Deallocate the layer
	}
}

//------------------------------------------------------------------------------
//! \brief Create a new sprite layer
//!
//! This function calls smb_vgfx_layer_alloc() and smb_vgfx_layer_realise()\n
//! (See also: Other smb_vgfx_layer_create functions)\n
//! (Does not exit on error)
//!
//!	\param width = Pixel width of the image in pixels
//!	\param height = Image height
//!	\param depth = Layer depth (AV_xx_COLOUR)
//!	\param flags: SPRINFO_xxx flags (If the flag pairs are not set, the default flags will be used)
//!
//! 	\return The Layer
//------------------------------------------------------------------------------
LAYER *smb_vgfx_layer_create_sprite(int width, int height, int depth, int flags)
{
	int true_word_width;
	LAYER *lptr;
	int bitmap_size;
	int palette_size;
	int mode;
	char *bptr;
	Sprite_id *spr_base;
	smb_VGFX_SPRITE_AREA *spr_area;
	smb_VGFX_SPRITE_HEADER *spr_header;

	// Perform a check on the flags
	if (flags & SPRINFO_MODE_OLD)		// Is old flags enabled
	{
		// Only old flags should be enabled		
		if (flags & (SPRINFO_MODE_NEW | SPRINFO_MASK_NEW | SPRINFO_DPI_180 | SPRINFO_DPI_22) )
		{
			smb_vgfx_baderror("Layer: MODE_OLD flags mixed with NEW flags", 0);
		}

		// Make a default DPI?
		if (!(flags & (SPRINFO_DPI_90 | SPRINFO_DPI_45) ) )	// Bits not set
		{
			flags |= SPRINFO_DPI_90;
		}

	}else					// Is new flags (or default) enabled
	{
		flags |= SPRINFO_MODE_NEW;	// Set the default mode (if not already set)
		if (flags & (SPRINFO_MODE_OLD | SPRINFO_MASK_OLD ) )
		{
			smb_vgfx_baderror("Layer: MODE_NEW flags mixed with OLD flags", 0);
		}

		// Make a default DPI?
		if (!(flags & (SPRINFO_DPI_180 | SPRINFO_DPI_90 | SPRINFO_DPI_45 | SPRINFO_DPI_22) ) )	// Bits not set
		{
			flags |= SPRINFO_DPI_180;
		}
	}

	// Palette not supported if the pixel is a colour component (although in theory it is possible!)
	if (depth >= AV_32K_COLOUR)
	{
		flags = flags & (~(SPRINFO_PALETTE));
	}

	lptr = smb_vgfx_layer_alloc(width, 0, height, depth);	// Allocate the basic layer structure

	lptr->spr_info = flags;				// Store the SPRINFO_xxx flags

	true_word_width = lptr->true_word_width;	// Use the correct word width value

	bitmap_size = lptr->true_byte_size;		// Size of the bitmap to allocate

	// Is an old format mask wanted?
	if (flags & SPRINFO_MASK_OLD)
	{
		bitmap_size += lptr->true_byte_size;		// Mask is the same size as the image
	}

	// Is a new format mask wanted?
	if (flags & SPRINFO_MASK_NEW)
	{
		bitmap_size += (lptr->true_pixel_width >> 8) * height;	// 8 bits(pixels) in a byte for the new mask format
	}

	// Is a palette wanted?
	if (flags & SPRINFO_PALETTE)
	{
		palette_size = 2 * sizeof(int) * lptr->colours;	// Includes the repeated RGB pairs
		bitmap_size += palette_size;			// Add on the bitmap size (ie amount to allocate)
	}else	palette_size = 0;

	bitmap_size += sizeof(smb_VGFX_SPRITE_AREA);	// Add size of sprite structure
	bitmap_size += sizeof(smb_VGFX_SPRITE_HEADER);	// Add size of sprite header

	bptr = (char *) calloc(1, bitmap_size);		// Allocate the memory needed for the sprite layer data
	if (!bptr)	// Out of memory?
	{
		smb_vgfx_baderror("Layer: No memory for bitmap", bitmap_size);
	}
	lptr->memory_base = (int *) bptr;	// Store the memory address

	lptr->spr_origin = spr_base = &(lptr->sprite_base);		// Point to itself (because I created it)
	lptr->spr_area = spr_area = (smb_VGFX_SPRITE_AREA *) bptr;	// Sprite area memory
	bptr += sizeof(smb_VGFX_SPRITE_AREA);				// Past this structure
	lptr->spr_header = spr_header = (smb_VGFX_SPRITE_HEADER *) bptr;	// Sprite header memory
	bptr += sizeof(smb_VGFX_SPRITE_HEADER);				// Past this structure

	// Copy the size of the layer into the sprite
	spr_base->sprite_addr = (int *) spr_area;
	spr_base->sprite_count = 1;
	spr_base->sprite_ptrs = (int *) &(lptr->spr_header);	// Address of the sprite header
	spr_area->size = bitmap_size;	// Including the size of the structures
	spr_area->number = 1;
	spr_area->sproff = sizeof(smb_VGFX_SPRITE_AREA);	// IE no extension data
	spr_area->freeoff = bitmap_size;			// IE no buffer exists

	bitmap_size -= sizeof(smb_VGFX_SPRITE_AREA);		// Offsets relative to the header structure
	spr_header->next = bitmap_size;				// Store the size of the sprite
	spr_header->name[0] = 'a';				// A gash name

	spr_header->width = true_word_width - 1;		// Sprite width in words - 1
	spr_header->height = height - 1;			// Sprite height - 1

	width = lptr->true_pixel_width - lptr->width;		// Number of wasted pixels

	width = width << depth;		// Convert number of pixels into number of bits
	width = ( 31 - width );		// Calculate last bit used
	spr_header->rbit = width;	// Set the last bit used

	// Handle old video modes
	mode = 0;		// Set mode to clear, to avoid compiler warnings
	if (flags & SPRINFO_MODE_OLD)
	{
		switch (depth)		// See PRM 1-751 on sprite modes
		{
			case AV_16_COLOUR:
				if (flags & SPRINFO_DPI_90) mode = 20; else mode = 9;
				break;
			case AV_256_COLOUR:
				if (flags & SPRINFO_DPI_90) mode = 21; else mode = 13;
				break;
			default:
				smb_vgfx_baderror("Layer: Bad depth with old mode", depth );
				break;
		}
	}else				// New video modes
	{
		switch (depth)		// See PRM 5-87 on sprite modes
		{
			case AV_16_COLOUR:
				if (flags & SPRINFO_DPI_180) mode = VGFX_SPRMODE_C16_DPI180;
				else if (flags & SPRINFO_DPI_90) mode = VGFX_SPRMODE_C16_DPI90;
				else if (flags & SPRINFO_DPI_45) mode = VGFX_SPRMODE_C16_DPI45;
				else if (flags & SPRINFO_DPI_22) mode = VGFX_SPRMODE_C16_DPI22;
				break;
			case AV_256_COLOUR:
				if (flags & SPRINFO_DPI_180) mode = VGFX_SPRMODE_C256_DPI180;
				else if (flags & SPRINFO_DPI_90) mode = VGFX_SPRMODE_C256_DPI90;
				else if (flags & SPRINFO_DPI_45) mode = VGFX_SPRMODE_C256_DPI45;
				else if (flags & SPRINFO_DPI_22) mode = VGFX_SPRMODE_C256_DPI22;
				break;
			case AV_32K_COLOUR:
				if (flags & SPRINFO_DPI_180) mode = VGFX_SPRMODE_C32K_DPI180;
				else if (flags & SPRINFO_DPI_90) mode = VGFX_SPRMODE_C32K_DPI90;
				else if (flags & SPRINFO_DPI_45) mode = VGFX_SPRMODE_C32K_DPI45;
				else if (flags & SPRINFO_DPI_22) mode = VGFX_SPRMODE_C32K_DPI22;
				break;
			case AV_16M_COLOUR:
				if (flags & SPRINFO_DPI_180) mode = VGFX_SPRMODE_C16M_DPI180;
				else if (flags & SPRINFO_DPI_90) mode = VGFX_SPRMODE_C16M_DPI90;
				else if (flags & SPRINFO_DPI_45) mode = VGFX_SPRMODE_C16M_DPI45;
				else if (flags & SPRINFO_DPI_22) mode = VGFX_SPRMODE_C16M_DPI22;
				break;
		}
	}

	spr_header->mode = mode;	// Store the sprite mode

	// Is a palette wanted?
	if (flags & SPRINFO_PALETTE)
	{
		lptr->spr_palette = (int *) bptr;	// Store the address of the palette
		bptr += palette_size;		// Pointer past the palette
	}

	lptr->bitmap_ptr = (int *) bptr;	// Store the address of the bitmap
	spr_header->image = palette_size + sizeof(smb_VGFX_SPRITE_HEADER);	// Offset to sprite image

	if (flags & (SPRINFO_MASK_OLD | SPRINFO_MASK_NEW))	// Is a mask needed
	{
		spr_header->mask = spr_header->image + lptr->true_byte_size;	// The mask offset (after the image)
		bptr += lptr->true_byte_size;			// Address of the mask
		lptr->spr_mask_ptr = (int *) bptr;		// Store the address of the mask
	}else	spr_header->mask = spr_header->image;		// No mask needed (use image instead to avoid possible problems later on)

	smb_vgfx_layer_realise(lptr);		// Create the line table
	lptr->flags = lptr->flags | LAYER_ALLOC_BITMAP | LAYER_SPRITE;

	return lptr;
}

//------------------------------------------------------------------------------
//! \brief Draw a sprite layer to draw screen
//!
//! This function is a wrapper for smb_put_at_sprite()
//!
//!	\param lptr = The layer to use
//!	\param xpos = Where to draw to X
//!	\param ypos = Where to draw to Y
//!	\param gcol = See smb_put_at_sprite() in AptGraph
//------------------------------------------------------------------------------
void smb_vgfx_sprite_draw( LAYER *lptr, int xpos, int ypos, int gcol )
{
	smb_put_at_sprite(&lptr->sprite_base, 0, xpos, ypos, gcol);
}

//------------------------------------------------------------------------------
//! \brief Put a sprite at a coordinate (see PRM 1-785)
//!
//!	\param sprite_info = Address of the sprites structure
//!	\param sprite_index = The sprite number to draw (starting from 0)
//!	\param x = X Coordinate
//!	\param y = Y Coordinate
//!	\param gcol = The gcol plot action (see PRM 1-753) (found in aptgraph.h)
//------------------------------------------------------------------------------
void smb_put_at_sprite( Sprite_id *sprite_info, int sprite_index, int x, int y, int gcol )
{
	_kernel_swi_regs regs;

	if( sprite_index >= sprite_info->sprite_count ) return;			// unknown sprite?
	regs.r[ 0 ] = PutAtSprite;					// Put sprite at command
	regs.r[ 1 ] = ( int )sprite_info->sprite_addr;			// Sprite area pointer
	regs.r[ 2 ] = ( int )sprite_info->sprite_ptrs[ sprite_index ];	// The sprite pointer
	regs.r[ 3 ] = ( int ) x;					// x pos
	regs.r[ 4 ] = ( int ) y;					// y pos
	regs.r[ 5 ] = gcol;						// GCOL action

	_kernel_swi( OS_SpriteOp , &regs ,&regs );	// do command
}

//------------------------------------------------------------------------------
//! \brief Allocate a new layer structure
//!
//! (If the image pixel width is a multiple of words, the LAYER_WORD_ALIGNED flag is set)\n
//! (Note: The adress of the actual bitmap or sprite is NOT set - use the smb_vgfx_layer_create() functions)\n
//! (Does not exit on error)
//!
//!	\param width = Pixel width of the image inside the bitmap in pixels
//! 	\param true_word_width = Actual bitmap width in words (0 = Automatically convert the width into words)
//!	\param height = Layer image height
//!	\param depth = Layer depth (AV_16_COLOUR or AV_256_COLOUR)
//!
//! 	\return The Layer (The line_table is cleared to zero)
//------------------------------------------------------------------------------
LAYER *smb_vgfx_layer_alloc(int width, int true_word_width, int height, int depth)
{
	LAYER *lptr;
	int **line_ptr;

	static char er_txt[]="Layer: No memory for bitmap layer";

	// Allocate layer (Note: Structure is cleared)
	lptr = (LAYER *) calloc( 1, sizeof(LAYER) );
	if (!lptr)	// Out of memory?
	{
		smb_vgfx_baderror(er_txt, sizeof(LAYER));
	}
	// Allocate line table
	line_ptr = (int **) calloc( 1, sizeof(int) * height );
	if (!line_ptr)	// Out of memory?
	{
		smb_vgfx_baderror(er_txt, sizeof(LAYER));
	}
	lptr->line_table_ptr = line_ptr;	// Save the address of the line table

	// Save the layer information
	lptr->height = height;
	lptr->depth = depth;
	lptr->width = width;

	switch (depth)			// Calculate the width depending on the depth
	{
		case AV_16_COLOUR:
		{
			lptr->colours = 16;
			if (!(width & 7))	// Is an exact multiple of a word?
			{
				lptr->flags |= LAYER_WORD_ALIGNED;
			}else
			{
				width += 7;	// Make width multiple of a word
			}
			width = width >> 3;	// 8 Pixels in a word
			break;
		}
		case AV_256_COLOUR:
		{
			lptr->colours = 256;
			if (!(width & 3))	// Is an exact multiple of a word?
			{
				lptr->flags |= LAYER_WORD_ALIGNED;
			}else
			{
				width += 3;	// Make width multiple of a word
			}
			width = width >> 2;	// 4 Pixels in a word
			break;
		}
		case AV_32K_COLOUR:
		{
			lptr->colours = 32768;
			if (!(width & 1))	// Is an exact multiple of a word?
			{
				lptr->flags |= LAYER_WORD_ALIGNED;
			}else
			{
				width += 1;	// Make width multiple of a word
			}
			width = width >> 1;	// 2 Pixels in a word
			lptr->flags |= LAYER_REMAP;	// Remapping always required when converting from 16 / 256 colours
			break;
		}

		case AV_16M_COLOUR:
		{
			lptr->colours = 16777216;
			lptr->flags |= LAYER_WORD_ALIGNED | LAYER_REMAP;	// Always is a multiple of a word and remapping always required when converting from 16 / 256 colours
			break;
		}

		default:	// Validate the depth (if not supported)
		{
			smb_vgfx_baderror("Layer: Depth not supported", depth );
		}
	}
	lptr->word_width = width;	// (Width is in words)
	lptr->byte_width = width << 2;	// 4 bytes in a word

	if (!true_word_width) true_word_width = width;	// true word width is the same as the image word width if it was not given

	lptr->true_word_width = true_word_width;
	lptr->true_byte_width = true_word_width << 2;	// 4 Bytes in a word

	// Calculate true pixel width
	switch (depth)
	{
		case AV_16_COLOUR:
		{
			lptr->true_pixel_width = true_word_width << 3;	// 8 pixels in a word
			break;
		}
		case AV_256_COLOUR:
		{
			lptr->true_pixel_width = true_word_width << 2;	// 4 pixels in a word
			break;
		}
		case AV_32K_COLOUR:
		{
			lptr->true_pixel_width = true_word_width << 1;	// 2 pixels in a word
			break;
		}
		case AV_16M_COLOUR:
		{
			lptr->true_pixel_width = true_word_width;	// 1 pixel in a word
			break;
		}
	}

	lptr->true_byte_size = lptr->true_byte_width * height;

	return lptr;
}

//------------------------------------------------------------------------------
//! \brief Restore the text and graphic window the their default state. (See PRM 1-610)
//!
//! (Ie. Both Full Screen, Origins at 0,0)
//------------------------------------------------------------------------------
void smb_windrestore( void )
{
	smb_fastvdu( RESTORE_WIND, 0 );				// Restore both windows
}

//------------------------------------------------------------------------------
//! \brief Calculate the line table in the layer
//!
//! (The line table pointer MUST exist)\n
//! (Does not exit on error)
//!
//! 	\return lptr = The layer
//------------------------------------------------------------------------------
void smb_vgfx_layer_realise(LAYER *lptr)
{
	int cnt;
	int height;
	int true_width;
	int *memptr;
	int **lineptr;

	if (!lptr->bitmap_ptr)	// A bitmap MUST exist
	{
		smb_vgfx_baderror("Layer: No bitmap to realise", 0);
	}
	
	height = lptr->height;
	true_width = lptr->true_word_width;
	memptr = lptr->bitmap_ptr;
	lineptr = lptr->line_table_ptr;

	memptr += height * true_width;	// To the bottom of the bitmap

	for (cnt=0; cnt<height; cnt++)	// For all the lines in the bitmap
	{
		memptr = memptr - true_width;	// Move to the line above
		*(lineptr++) = memptr;		// Store the address of the layer line
	}
}

//------------------------------------------------------------------------------
//! \brief Set a palette entry (see PRM 1-679, 1-568)
//!
//!	\param l = logical colour (actual palette entry)
//!	\param p = mode (See PRM 1-568) (16 = Fully change colour)
//!	\param r = Red (0-255)
//!	\param g = Green (0-255)
//!	\param b = Blue (0-255)
//------------------------------------------------------------------------------
void smb_setpalette( int l , int p ,  int r ,  int g ,  int b )
{
	_kernel_swi_regs regs ;

	VDU_Buffer[0] = l;					// set logical colour
	VDU_Buffer[1] = p;					// set physical colour
	VDU_Buffer[2] = r;					// set red
	VDU_Buffer[3] = g;					// set green
	VDU_Buffer[4] = b;					// set blue

	regs.r[0] = 12;						// Write Palette command
	regs.r[1] = (int) VDU_Buffer;				// address of colour components
	_kernel_swi( OS_Word , &regs , &regs );	// Do command
}


//------------------------------------------------------------------------------
//! \brief Scan for a key-presses (Private function) (PRM 1-863)
//!
//!	\param code = The internel key code
//!
//! 	\return 0 = Not pressed
//------------------------------------------------------------------------------
static int check_key(int code)
{
	_kernel_swi_regs regs;

	regs.r[0] = 121;		// Keyboard scan PRM 1-863
	regs.r[1] = code ^ 0x80;
	_kernel_swi( OS_Byte, &regs, &regs );
	return regs.r[1];
}

//------------------------------------------------------------------------------
//! \brief Scan for the SHIFT key (Private function) (PRM 1-883)
//!
//! 	\return 0 = Not pressed
//------------------------------------------------------------------------------
static int check_shift(void)
{
	_kernel_swi_regs regs;

	regs.r[0] = 202;		// Keyboard scan PRM 1-883
	regs.r[1] = 0;
	regs.r[2] = 255;
	_kernel_swi( OS_Byte, &regs, &regs );
	return (regs.r[1] & BIT3);
}

//------------------------------------------------------------------------------
//! \brief Scan for the CTRL key (Private function) (PRM 1-883)
//!
//! 	\return 0 = Not pressed
//------------------------------------------------------------------------------
static int check_ctrl(void)
{
	_kernel_swi_regs regs;

	regs.r[0] = 202;		// Keyboard scan PRM 1-883
	regs.r[1] = 0;
	regs.r[2] = 255;
	_kernel_swi( OS_Byte, &regs, &regs );
	return (regs.r[1] & BIT6);
}

//------------------------------------------------------------------------------
//! \brief Process for a key-presses (Private function)
//!
//!	\param code = The SMB_KB_xx key code
//!	\param flag = 0 = Key Not Pressed
//!
//!
//------------------------------------------------------------------------------
static void process_key(int code, int flag)
{
	if (!flag)	// Key not pressed
	{
		SMB_Keybank_Buffer[code] = 0;
	}else		// Key is being pressed
	{
		SMB_Keybank_Buffer[code] = 1;
	}
}

//------------------------------------------------------------------------------
//! \brief Scan for new key-presses
//!
//! Call smb_keybank_flush() before using this function
//------------------------------------------------------------------------------
void smb_keybank_scan(void)
{
	// Check each key (PRM 1-849)
	process_key( SMB_KB_CHEAT, check_key(113) );
	process_key( SMB_KB_FIRE, check_ctrl() );
	process_key( SMB_KB_LEFT, check_key(25) );
	process_key( SMB_KB_RIGHT, check_key(121) );
	process_key( SMB_KB_UP, check_key(57) );
	process_key( SMB_KB_DOWN, check_key(41) );

	process_key( SMB_KB_SHIFT, check_shift() );
	process_key( SMB_KB_A, check_key(65) );
	process_key( SMB_KB_D, check_key(50) );
	process_key( SMB_KB_W, check_key(33) );
	process_key( SMB_KB_S, check_key(81) );

}

//------------------------------------------------------------------------------
//! \brief Flush the keyboard bank buffer
//------------------------------------------------------------------------------
void smb_keybank_flush(void)
{
	memset(SMB_Keybank_Buffer, 0, sizeof(SMB_Keybank_Buffer));
}

//------------------------------------------------------------------------------
//! \brief Get the value of the centisecond timer (See PRM 1-409)
//!
//! It is save to assume that this value won't overflow (unless the computer is left
//!	on for longer than a year!)
//!
//! 	\param compval = A Value that can be added onto the system timer value.
//!		Ie. (0 = Time Now) (100 = Time next second)
//!
//! 	\return System Time + compval
//------------------------------------------------------------------------------
unsigned int smb_gettime( unsigned int compval )
{
	_kernel_swi_regs regs ;

	 _kernel_swi( OS_ReadMonotonicTime , &regs , &regs );	// Get current time in centiseconds

	compval += regs.r[0];			// Add on the time

	return compval;
}

//------------------------------------------------------------------------------
//! \brief Set the stereo position of a channel
//!
//! 	\param chan = Channel number (1 to 8)
//!	\param value = Stereo position (-127 = Left, 128 = Right)
//------------------------------------------------------------------------------
void smb_snd_stereo(int chan, int value)
{
	_kernel_swi_regs regs;

	if (QTM_Available)
	{
		// Validate channel
		if (chan<1) return;
		if (chan>8) return;

		regs.r[0] = chan; 
		regs.r[1] = value; 
		_kernel_swi( SMB_QTM_Stereo, &regs , &regs );			// Do command
	}
}


