/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
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
#include <ClanLib/gl.h>

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
	void on_button_press(const CL_InputEvent &key)
	{
		LastKey = key.id;
	}
        void on_window_close()
        {
                LastKey = CL_KEY_ESCAPE;
        }

	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		try
		{
			std::cout << "Super Methane Brothers\n";  
			std::cout << "Licensed using the GNU General Public License Version 2\n";  
			std::cout << "http://methane.sourceforge.net\n";  
			std::cout << "Instructions:\n\n";
			std::cout << "Press Fire to start. Use keyboard to enter player names.\n";
			std::cout << "Press Return to start game. Tap Fire to fire gas from the gun.\n";
			std::cout << "Hold Fire to suck a trapped baddie into the gun.\n";
			std::cout << "Release Fire to throw the trapped baddie from the gun.\n";
			std::cout << "Throw baddies at the wall to destroy them.\n\n";
			std::cout << "Keys:\n";  
			std::cout << "Player One - Use Cursor keys and control (CTRL) to fire.\n";
			std::cout << "Player Two - Use A,W,S,D and shift to fire.\n\n";
			std::cout << "TAB:Change Graphic\n";
			std::cout << "ESCAPE:Exit Program\n";
			std::cout << "F1:Pause (Press any key to continue)\n";
			std::cout << "F2:Toggle Scaling\n";
			std::cout << "F3:Toggle Full Screen\n";
			std::cout << "F9:Increase Game Speed (Lower frame rate)\n";
			std::cout << "F10:Decrease Game Speed (Raise frame rate)\n";
			std::cout << "(High Scores written to /var/games/methanescores)\n";


			// Initialize ClanLib base components
			CL_SetupCore setupCore;

			// Initialize the ClanLib display component
			CL_SetupDisplay setupDisplay;

			// Initilize the OpenGL drivers
			CL_SetupGL setupGL;

			// Set the video mode
			CL_DisplayWindow window("Super Methane Brothers", SCR_WIDTH*2, SCR_HEIGHT*2);

			CL_PixelBuffer pixel_screen(SCR_WIDTH, SCR_HEIGHT, SCR_WIDTH*4, CL_PixelFormat::rgba8888);
			CL_Surface game_screen(pixel_screen);
			CL_Canvas game_canvas(game_screen);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &SuperMethaneBrothers::on_window_close);

                        // Connect a keyboard handler to on_key_down()
			CL_Slot slot_keyboard_down = CL_Keyboard::sig_key_down().connect(this, &SuperMethaneBrothers::on_button_press);

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
			int disable_scale_flag = 0;
			int full_screen_flag = 0;
			int on_options_screen = 1;
			int option_page = 0;
			int game_speed = 60;

			LastKey = 0;
			// Run until someone presses escape
			while (!quit_flag)
			{
//------------------------------------------------------------------------------
// Joystick Emulation and Video mode Control
//------------------------------------------------------------------------------

				JOYSTICK *jptr1;
				JOYSTICK *jptr2;
				jptr1 = &Game.m_GameTarget.m_Joy1;
				jptr2 = &Game.m_GameTarget.m_Joy2;

				if (LastKey)
				{
					if (LastKey == CL_KEY_ESCAPE) quit_flag = 1;
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
							option_page++;
							if (option_page==3)
							{
								on_options_screen = 0;	// Start the game
							}
						}
					}

				
					// Video mode changed
					if ( (LastKey >= CL_KEY_F2) && (LastKey <= CL_KEY_F3) )
					{
						if (LastKey == CL_KEY_F2)
						{
							disable_scale_flag = disable_scale_flag ^ 1;
						}
						if (LastKey == CL_KEY_F3)
						{
							full_screen_flag = full_screen_flag ^ 1;
							if (full_screen_flag)
							{
								window.set_fullscreen(640,480, 16);
								CL_Mouse::hide();
							}else
							{
								window.set_windowed();
								CL_Mouse::show();
							}
						}
						CL_Display::clear(CL_Color::black);
						window.flip();
						CL_Display::clear(CL_Color::black);

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

					jptr1->key = jptr2->key = ':';	// Fake key press (required for high score table
					if ((LastKey >= CL_KEY_A) && (LastKey <= CL_KEY_Z)) jptr1->key = jptr2->key = LastKey - CL_KEY_A + 'A';
					if ((LastKey >= CL_KEY_0) && (LastKey <= CL_KEY_9)) jptr1->key = jptr2->key = LastKey - CL_KEY_0 + '0';
					if (LastKey == CL_KEY_SPACE) jptr1->key = jptr2->key = ' ';
					if (LastKey == CL_KEY_ENTER) jptr1->key = jptr2->key = 10;
					if (LastKey == CL_KEY_TAB)
					{
						Game.m_GameTarget.m_Game.TogglePuffBlow();
					}
					LastKey = 0;
				}

				if (CL_Keyboard::get_keycode(CL_KEY_ESCAPE)) quit_flag = 1;
				if (CL_Keyboard::get_keycode(CL_KEY_LEFT)) jptr1->left = 1; else jptr1->left = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_RIGHT)) jptr1->right = 1; else jptr1->right = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_UP)) jptr1->up = 1; else jptr1->up = 0;
				if (CL_Keyboard::get_keycode(CL_KEY_DOWN)) jptr1->down = 1; else jptr1->down = 0;

				if ( (CL_Keyboard::get_keycode(CL_KEY_LCONTROL)) || (CL_Keyboard::get_keycode(CL_KEY_RCONTROL)) )
				{
					jptr1->fire = 1;
				}else	jptr1->fire = 0;


                                if (CL_Keyboard::get_keycode(CL_KEY_A)) jptr2->left = 1; else jptr2->left = 0;
                                if (CL_Keyboard::get_keycode(CL_KEY_D)) jptr2->right = 1; else jptr2->right = 0;
                                if (CL_Keyboard::get_keycode(CL_KEY_W)) jptr2->up = 1; else jptr2->up = 0;
                                if (CL_Keyboard::get_keycode(CL_KEY_S)) jptr2->down = 1; else jptr2->down = 0;

                                if ( (CL_Keyboard::get_keycode(CL_KEY_LSHIFT)) || (CL_Keyboard::get_keycode(CL_KEY_RSHIFT)) )
                                {
                                        jptr2->fire = 1;
                                }else   jptr2->fire = 0;

// (CHEAT MODE DISABLED)	if (CL_Keyboard::get_keycode(CL_KEY_F12)) jptr1->next_level = 1; else jptr1->next_level = 0;

//------------------------------------------------------------------------------
// Do game main loop
//------------------------------------------------------------------------------
				pixel_screen.lock();
				if (on_options_screen)
				{
					Game.DisplayOptions( pixel_screen.get_data(), option_page);
				}else
				{
					Game.MainLoop(pixel_screen.get_data(), game_paused);
				}
				pixel_screen.unlock();
				game_screen.set_pixeldata(pixel_screen);
//------------------------------------------------------------------------------
// Output the graphics
//------------------------------------------------------------------------------

				if (!disable_scale_flag)
				{
					CL_Rect rect(0,0,(int)CL_Display::get_width(), (int)CL_Display::get_height());
					game_screen.draw(rect);
				}else
				{
					int xoff, yoff;

					xoff = CL_Display::get_width();
					yoff = CL_Display::get_height();

					xoff = (xoff >> 1) - (SCR_WIDTH >> 1);
					yoff = (yoff >> 1) - (SCR_HEIGHT >> 1);

					CL_Rect rect(xoff,yoff,SCR_WIDTH,SCR_HEIGHT);
					game_screen.draw(rect);
				}

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

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

			Game.RemoveSoundDriver();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			
			return -1;
		}

		return 0;
	}
} my_app;

