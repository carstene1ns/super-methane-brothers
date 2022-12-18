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
// Methane brothers main source file
//------------------------------------------------------------------------------
#include "precomp.h"
#include "global.h"

#include "doc.h"
#include "ClanMikmod/setupmikmod.h"

bool GLOBAL_SoundEnable = true;

//------------------------------------------------------------------------------
// Keyboard stuff

class SetupMikMod;

class SuperMethaneBrothers : public clan::Application
{
public:
	SuperMethaneBrothers();
	bool update();
private:
	void on_button_press(const clan::InputEvent& key);
	void on_window_close();

	void init_game();
	void run_game();

	enum class ProgramState
	{
		init_game,
		run_game,
		quit
	};
	ProgramState m_ProgramState = ProgramState::init_game;

	int m_LastKey = 0;

	clan::SoundOutput m_SoundOutput;
	std::shared_ptr<SetupMikMod> m_SetupMikmod;
	clan::DisplayWindow m_Window;
	clan::Slot m_SlotQuit;
	clan::Slot m_SlotInput;
	clan::Canvas m_Canvas;

	clan::Font m_Font;

	std::shared_ptr<CMethDoc> m_Game;


	int disable_scale_flag = 0;
	int full_screen_flag = 0;
	int on_options_screen = 1;

	clan::GameTime m_GameTime;


};
clan::ApplicationInstance<SuperMethaneBrothers> clanapp;

void SuperMethaneBrothers::on_button_press(const clan::InputEvent &key)
{
	m_LastKey = key.id;
}
void SuperMethaneBrothers::on_window_close()
{
	m_LastKey = clan::keycode_escape;
}

SuperMethaneBrothers::SuperMethaneBrothers()
{
	clan::OpenGLTarget::set_current();
}

bool SuperMethaneBrothers::update()
{
	switch (m_ProgramState)
	{
	case ProgramState::init_game:
		init_game();
		break;
	case ProgramState::run_game:
		run_game();
		break;
	case ProgramState::quit:

		// We have a suspect race condition on program exit. Unsure where the source is
		m_Game.reset();
		m_SoundOutput.stop_all();
		clan::System::sleep(125);
		return false;
	default:
		return false;
	}
	return true;
}

void SuperMethaneBrothers::init_game()
{
	m_SoundOutput = clan::SoundOutput(44100, 192);
	m_SetupMikmod = std::make_shared<SetupMikMod>();

	// Set the video mode
	clan::DisplayWindowDescription desc;
	desc.set_title("Super Methane Brothers");
	desc.set_size(clan::Size(SCR_WIDTH * 2, SCR_HEIGHT * 2), true);
	desc.set_allow_resize(true);
	m_Window = clan::DisplayWindow(desc);
	m_Canvas = clan::Canvas(m_Window);

	m_Game = std::make_shared<CMethDoc>(m_Canvas);

	// Connect the Window close event
	m_SlotQuit = m_Window.sig_window_close().connect(this, &SuperMethaneBrothers::on_window_close);

	// Connect a keyboard handler to on_key_up()
	m_SlotInput = m_Window.get_keyboard().sig_key_down().connect(this, &SuperMethaneBrothers::on_button_press);

	m_Game->InitGame();
	m_Game->LoadScores();
	m_Game->StartGame();

	m_Font = clan::Font("tahoma", 32);

	m_LastKey = 0;

	m_ProgramState = ProgramState::run_game;
	m_GameTime = clan::GameTime(25, 25);

}

void SuperMethaneBrothers::run_game()
{
	m_GameTime.update();
	if (m_LastKey == clan::keycode_escape)
	{
		m_Game->SaveScores();
		m_ProgramState = ProgramState::quit;
		return;
	}

	//------------------------------------------------------------------------------
	// Joystick Emulation and Video mode Control
	//------------------------------------------------------------------------------

	JOYSTICK* jptr1;
	JOYSTICK* jptr2;
	jptr1 = &m_Game->m_GameTarget.m_Joy1;
	jptr2 = &m_Game->m_GameTarget.m_Joy2;

	if (m_LastKey)
	{
		if (on_options_screen)
		{
			on_options_screen = 0;	// Start the game
			m_LastKey = 0;
		}

		jptr1->key = jptr2->key = ':';	// Fake key press (required for high score table)
		if ((m_LastKey >= clan::keycode_a) && (m_LastKey <= clan::keycode_z)) jptr1->key = jptr2->key = m_LastKey - clan::keycode_a + 'A';
		if ((m_LastKey >= clan::keycode_0) && (m_LastKey <= clan::keycode_9)) jptr1->key = jptr2->key = m_LastKey - clan::keycode_0 + '0';
		if (m_LastKey == clan::keycode_space) jptr1->key = jptr2->key = ' ';
		if (m_LastKey == clan::keycode_enter) jptr1->key = jptr2->key = 10;
		if (m_LastKey == clan::keycode_tab)
		{
			m_Game->m_GameTarget.m_Game.TogglePuffBlow();
		}
		m_LastKey = 0;
	}

	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_left)) jptr1->left = 1; else jptr1->left = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_right)) jptr1->right = 1; else jptr1->right = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_up)) jptr1->up = 1; else jptr1->up = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_down)) jptr1->down = 1; else jptr1->down = 0;

	if (((m_Window.get_keyboard()).get_keycode(clan::keycode_lcontrol)) || ((m_Window.get_keyboard()).get_keycode(clan::keycode_rcontrol)))
	{
		jptr1->fire = 1;
	}
	else	jptr1->fire = 0;


	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_a)) jptr2->left = 1; else jptr2->left = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_d)) jptr2->right = 1; else jptr2->right = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_w)) jptr2->up = 1; else jptr2->up = 0;
	if ((m_Window.get_keyboard()).get_keycode(clan::keycode_s)) jptr2->down = 1; else jptr2->down = 0;

	if (((m_Window.get_keyboard()).get_keycode(clan::keycode_lshift)) || ((m_Window.get_keyboard()).get_keycode(clan::keycode_rshift)))
	{
		jptr2->fire = 1;
	}
	else   jptr2->fire = 0;

	// (CHEAT MODE ENABLED)
	//if ((m_Window.get_keyboard()).get_keycode(clan::keycode_f11)) jptr1->next_level = 1; else jptr1->next_level = 0;

//------------------------------------------------------------------------------
// Do game main loop
//------------------------------------------------------------------------------
	m_Canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));
	if (on_options_screen)
	{
		m_Game->DisplayOptions(m_Canvas, m_Font);
	}
	else
	{
		m_Game->MainLoop();
	}
	//------------------------------------------------------------------------------
	// Output the graphics
	//------------------------------------------------------------------------------

	m_Window.flip(0);

}
