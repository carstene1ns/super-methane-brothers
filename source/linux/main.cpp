/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://www.methane.fsnet.co.uk/index.html              *
 * Email: rombust@postmaster.co.uk                                         *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------
#ifdef METHANE_OLD_CPP
#include <iostream.h>
#else
#include <iostream>
#endif

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "global.h"

#include "doc.h"

//------------------------------------------------------------------------------
// The Game Instance
//------------------------------------------------------------------------------
CMethDoc Game;

//------------------------------------------------------------------------------
// Keyboard stuff
//------------------------------------------------------------------------------
static int LastKey = 0;

//------------------------------------------------------------------------------
// The Program
//------------------------------------------------------------------------------
class SuperMethaneBrothers : public CL_ClanApplication
{
public:
	// This function returns the name of your game
	virtual char *get_title()
	{
		return "Super Methane Brothers";
	}
	void on_button_press(CL_InputDevice *device, const CL_Key &key)
	{
		LastKey = key.id;
	}
	virtual int main(int argc, char **argv)
	{
		try
		{
			// Create a console window for text-output if not available
			CL_ConsoleWindow console("Console");
			console.redirect_stdio();

			std::cout << "Super Methane Brothers" << std::endl;  
			std::cout << "Licensed using the GNU General Public License Version 2" << std::endl;  
			std::cout << "http://www.methane.fsnet.co.uk" << std::endl;  
			std::cout << "Instructions:" << std::endl;  
			std::cout << "Use the cursor keys to move around the screen." << std::endl;  
			std::cout << "Tap the CTRL key to fire gas from the gun." << std::endl;  
			std::cout << "Hold the CTRL key to suck a trapped baddie into the gun." << std::endl;  
			std::cout << "Release the CTRL key to throw the trapped baddie from the gun" << std::endl;  
			std::cout << "Throw baddies at the wall to destroy them." << std::endl;
			std::cout << "Keys:" << std::endl;  
			std::cout << "CTRL:Start Game" << std::endl;
			std::cout << "CTRL:Create or Suck Gas" << std::endl;
			std::cout << "CURSOR KEYS: Move Player" << std::endl;
			std::cout << "TAB:Change Graphic" << std::endl;
			std::cout << "ESCAPE:Exit Program" << std::endl;
			std::cout << "F1:Pause (Press any key to continue)" << std::endl;
			std::cout << "F2:Toggle Scaling - Use if running a 16bit (65536 colours) screen" << std::endl;
			std::cout << "F3:Full Screen" << std::endl;
			std::cout << "F4:800*600 Screen Mode" << std::endl;
			std::cout << "F5:1024*768 Screen Mode" << std::endl;
			std::cout << "F6:1152*870 Screen Mode" << std::endl;
			std::cout << "F9:Increase Game Speed (Lower frame rate)" << std::endl;
			std::cout << "F10:Decrease Game Speed (Raise frame rate)" << std::endl;
			std::cout << "This game requires ClanLib (v0.6.5) and Hermes (v1.3.3) http://clanlib.org/hermes" << std::endl;
			std::cout << "(High Scores written to /var/games/methanescores)" << std::endl;

			// Initialize ClanLib base components
			CL_SetupCore::init();

			// Initialize the ClanLib display component
			CL_SetupDisplay::init();

			// Set the video mode
			CL_Display::set_videomode(SCR_WIDTH*2, SCR_HEIGHT*2, 16, false, true);

			CL_Canvas canvas(SCR_WIDTH, SCR_HEIGHT, 1, 0xff000000, 0xff0000, 0xff00, 0);	// No alpha channel
			CL_Surface *game_screen = CL_Surface::create_dynamic(&canvas);

			CL_Slot button = CL_Input::sig_button_press().connect(this, &SuperMethaneBrothers::on_button_press);

			Game.InitSoundDriver();
			Game.InitGame();
			Game.LoadScores();
			Game.StartGame();

//------------------------------------------------------------------------------
// The main game
//------------------------------------------------------------------------------

			int last_time = CL_System::get_time();

			int game_paused = 0;
			CL_InputBuffer mykey;
			int quit_flag = 0;
			int mouse_pointer_flag = 0;
			int disable_scale_flag = 1;
			int full_screen_first_time = 1;
			int on_options_screen = 1;
			int game_speed = 60;

			LastKey = 0;
			// Run until someone presses escape
			while (!quit_flag)
			{
//------------------------------------------------------------------------------
// Joystick Emulation and Video mode Control
//------------------------------------------------------------------------------

				JOYSTICK *jptr;
				jptr = &Game.m_GameTarget.m_Joy1;

				if (LastKey)
				{
					if (game_paused)	// On pause
					{
						// Any key to continue
						if (LastKey <= CL_KEY_PRINT)	// Ignore mouse buttons
						{
							game_paused = 0;
						}
					}else
					{
						if (!on_options_screen)
						{
							if (LastKey == CL_KEY_F1) game_paused = 1;
						}
					}
				
					if (on_options_screen)
					{
						if (LastKey == CL_KEY_SPACE)
						{
							on_options_screen = 0;	// Start the game
						}
					}

				
					// Video mode changed
					if ( (LastKey >= CL_KEY_F2) && (LastKey <= CL_KEY_F6) )
					{
						// Restore the mouse pointer
						// Note: The mouse pointer will not be hidden again. This is intentional
						// as sometimes the program crashes when repeatedly switching the mouse
						// pointer on and off
						if ((mouse_pointer_flag!=0) && (LastKey != CL_KEY_F2))
						{
							mouse_pointer_flag = 0;
							CL_MouseCursor::show();
						}
						switch (LastKey)
						{
							case CL_KEY_F2:
							{
								disable_scale_flag = disable_scale_flag ^ 1;
								break;
							}
							case CL_KEY_F3:
							{

								CL_Display::set_videomode(640,480, 16, true, false);
								if (full_screen_first_time)	// Only allow mouse pointer to be switched off once
								{
									full_screen_first_time = 0;
									mouse_pointer_flag = 1;
									CL_MouseCursor::hide();
								}
								break;
							}
							case CL_KEY_F4:
							{
								CL_Display::set_videomode(800, 600, 16, true, false);
								CL_Display::set_videomode(SCR_WIDTH*2, SCR_HEIGHT*2, 16, false, true);
								break;
							}
							case CL_KEY_F5:
							{
								CL_Display::set_videomode(1024, 768, 16, true, false);
								CL_Display::set_videomode(SCR_WIDTH*2, SCR_HEIGHT*2, 16, false, true);
								break;
							}
							case CL_KEY_F6:
							{
								CL_Display::set_videomode(1152, 870, 16, true, false);
								CL_Display::set_videomode(SCR_WIDTH*2, SCR_HEIGHT*2, 16, false, true);
								break;
							}
						}
						CL_Display::clear_display();
						CL_Display::flip_display();
						CL_Display::clear_display();

					}
					if (LastKey == CL_KEY_F9)
					{
						game_speed -= 20;
						if (game_speed <0) game_speed = 0;
					}
					if (LastKey == CL_KEY_F10)
					{
						game_speed += 20;
						if (game_speed >200) game_speed = 200;
					}

					jptr->key = ':';	// Fake key press (required for high score table
					if ((LastKey >= CL_KEY_A) && (LastKey <= CL_KEY_Z)) jptr->key = LastKey - CL_KEY_A + 'A';
					if ((LastKey >= CL_KEY_0) && (LastKey <= CL_KEY_9)) jptr->key = LastKey - CL_KEY_0 + '0';
					if (LastKey == CL_KEY_SPACE) jptr->key = ' ';
					if (LastKey == CL_KEY_ENTER) jptr->key = 10;
					if (LastKey == CL_KEY_TAB)
					{
						Game.m_GameTarget.m_Game.TogglePuffBlow();
					}
					LastKey = 0;
				}

				if (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) quit_flag = 1;
				if (CL_Keyboard::get_keycode(CL_KEY_LEFT)) jptr->left = 1; else jptr->left = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_RIGHT)) jptr->right = 1; else jptr->right = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_UP)) jptr->up = 1; else jptr->up = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_DOWN)) jptr->down = 1; else jptr->down = 0;

				if ( (CL_Keyboard::get_keycode(CL_KEY_LCTRL)) || (CL_Keyboard::get_keycode(CL_KEY_RCTRL)) )
				{
					jptr->fire = 1;
				}else	jptr->fire = 0;

// (CHEAT MODE DISABLED)	if (CL_Keyboard::get_keycode(CL_KEY_F12)) jptr->next_level = 1; else jptr->next_level = 0;

//------------------------------------------------------------------------------
// Do game main loop
//------------------------------------------------------------------------------
				canvas.lock();
				if (on_options_screen)
				{
					Game.DisplayOptions(canvas.get_data());
				}else
				{
					Game.MainLoop(canvas.get_data(), game_paused);
				}
				canvas.unlock();

//------------------------------------------------------------------------------
// Output the graphics
//------------------------------------------------------------------------------
				game_screen->reload();

				if (!disable_scale_flag)
				{
					game_screen->put_screen(0,0,(int)CL_Display::get_width(), (int)CL_Display::get_height());
				}else
				{
					int xoff, yoff;

					xoff = CL_Display::get_width();
					yoff = CL_Display::get_height();

					xoff = (xoff >> 1) - (SCR_WIDTH >> 1);
					yoff = (yoff >> 1) - (SCR_HEIGHT >> 1);

					game_screen->put_screen(xoff,yoff,SCR_WIDTH,SCR_HEIGHT);
				}

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip_display();

//------------------------------------------------------------------------------
// Control game speed
//------------------------------------------------------------------------------
				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive();

				int time_diff = 0;
				do
				{
					int time_now = CL_System::get_time();
					time_diff = time_now - last_time;
					time_diff = game_speed - time_diff;
					if (time_diff > 50) CL_System::sleep(20);
				}while(time_diff > 0);
				last_time = last_time + game_speed; 
			}
			Game.SaveScores();

			// Restore the mouse pointer
			if (mouse_pointer_flag)
			{
				mouse_pointer_flag = 0;
				CL_MouseCursor::show();
			}

			Game.RemoveSoundDriver();
			delete game_screen;

			// De-Initialize the ClanLib display component
			CL_SetupDisplay::deinit();

			// De-Initialize ClanLib base components
			CL_SetupCore::deinit();


		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			
			return -1;
		}

		return 0;
	}
} my_app;

