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

#include "precomp.h"
#include "target.h"

#include "doc.h"
#include "render_batch_triangle.h"

//------------------------------------------------------------------------------
// The game target (Yuck global!)
// Thus - Only a single GameTarget is allowed
//------------------------------------------------------------------------------
CGameTarget *GLOBAL_GameTarget = 0;

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
	m_bSessionActive = false;

	m_Lighting = 0.0;
}

//------------------------------------------------------------------------------
//! \brief Initialisation
//!
//! 	\param pdoc = Pointer to the document this target belongs to
//!	\param canvas = Screen to draw to
//------------------------------------------------------------------------------
void CGameTarget::Init(CMethDoc *pdoc, clan::Canvas& canvas)
{
	m_Canvas = canvas;
	m_pDoc = pdoc;
	m_Batcher = std::make_shared<RenderBatchTriangle>(canvas);
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CGameTarget::InitGame()
{
	m_Game.Init(this, &m_Joy1, &m_Joy2);

	// Find the resources directory:
	std::string resource_dir = clan::Directory::get_resourcedata("methane");
	std::string dataname("page_01.png");
	std::string filename = resource_dir + dataname;
	if (!clan::FileHelp::file_exists(filename))
	{
		resource_dir = std::string("resources/");
		filename = resource_dir + dataname;
		if (!clan::FileHelp::file_exists(filename))
		{
#ifdef WIN32
			throw clan::Exception("Unable to locate resources");
#else
			resource_dir = std::string("/usr/share/methane/resources/");
			filename = resource_dir + dataname;
			if (!clan::FileHelp::file_exists(filename))
			{
				resource_dir = std::string("/usr/share/methane/");
				filename = resource_dir + dataname;
				if (!clan::FileHelp::file_exists(filename))
				{
					throw clan::Exception("Unable to locate resources");
				}
			}
#endif
		}
	}
	

	filename = resource_dir + "page_01.png";
	clan::PixelBuffer image = clan::ImageProviderFactory::load(filename);
	m_Texture[0] = clan::Texture2D(m_Canvas, image.get_width(), image.get_height());
	m_Texture[0].set_image(m_Canvas, image);
	m_Texture[0].set_min_filter(clan::TextureFilter::nearest);
	m_Texture[0].set_mag_filter(clan::TextureFilter::nearest);

	filename = resource_dir + "page_02.png";
	image = clan::ImageProviderFactory::load(filename);
	m_Texture[1] = clan::Texture2D(m_Canvas, image.get_width(), image.get_height());
	m_Texture[1].set_image(m_Canvas, image);
	m_Texture[1].set_min_filter(clan::TextureFilter::nearest);
	m_Texture[1].set_mag_filter(clan::TextureFilter::nearest);

	filename = resource_dir + "page_03.png";
	image = clan::ImageProviderFactory::load(filename);
	m_Texture[2] = clan::Texture2D(m_Canvas, image.get_width(), image.get_height());
	m_Texture[2].set_image(m_Canvas, image);
	m_Texture[2].set_min_filter(clan::TextureFilter::nearest);
	m_Texture[2].set_mag_filter(clan::TextureFilter::nearest);

	filename = resource_dir + "page_04.png";
	image = clan::ImageProviderFactory::load(filename);
	m_Texture[3] = clan::Texture2D(m_Canvas, image.get_width(), image.get_height());
	m_Texture[3].set_image(m_Canvas, image);
	m_Texture[3].set_min_filter(clan::TextureFilter::nearest);
	m_Texture[3].set_mag_filter(clan::TextureFilter::nearest);

	filename = resource_dir + "page_05.png";
	image = clan::ImageProviderFactory::load(filename);
	m_Texture[4] = clan::Texture2D(m_Canvas, image.get_width(), image.get_height());
	m_Texture[4].set_image(m_Canvas, image);
	m_Texture[4].set_min_filter(clan::TextureFilter::nearest);
	m_Texture[4].set_mag_filter(clan::TextureFilter::nearest);

	if (GLOBAL_SoundEnable)
	{

		filename = resource_dir + "blow.wav";
		m_WAV_blow = clan::SoundBuffer(filename);

		filename = resource_dir + "bowling.wav";
		m_WAV_bowling = clan::SoundBuffer(filename);

		filename = resource_dir + "candle.wav";
		m_WAV_candle = clan::SoundBuffer(filename);

		filename = resource_dir + "card.wav";
		m_WAV_card = clan::SoundBuffer(filename);

		filename = resource_dir + "car.wav";
		m_WAV_car = clan::SoundBuffer(filename);

		filename = resource_dir + "chicken.wav";
		m_WAV_chicken = clan::SoundBuffer(filename);

		filename = resource_dir + "cookie.wav";
		m_WAV_cookie = clan::SoundBuffer(filename);

		filename = resource_dir + "crying.wav";
		m_WAV_crying = clan::SoundBuffer(filename);

		filename = resource_dir + "day.wav";
		m_WAV_day = clan::SoundBuffer(filename);

		filename = resource_dir + "die2.wav";
		m_WAV_die2 = clan::SoundBuffer(filename);

		filename = resource_dir + "duck.wav";
		m_WAV_duck = clan::SoundBuffer(filename);

		filename = resource_dir + "feather.wav";
		m_WAV_feather = clan::SoundBuffer(filename);

		filename = resource_dir + "finlev1.wav";
		m_WAV_finlev1 = clan::SoundBuffer(filename);

		filename = resource_dir + "hurry.wav";
		m_WAV_hurry = clan::SoundBuffer(filename);

		filename = resource_dir + "marble.wav";
		m_WAV_marble = clan::SoundBuffer(filename);

		filename = resource_dir + "mask.wav";
		m_WAV_mask = clan::SoundBuffer(filename);

		filename = resource_dir + "moon.wav";
		m_WAV_moon = clan::SoundBuffer(filename);

		filename = resource_dir + "oil.wav";
		m_WAV_oil = clan::SoundBuffer(filename);

		filename = resource_dir + "pickup1.wav";
		m_WAV_pickup1 = clan::SoundBuffer(filename);

		filename = resource_dir + "pstar.wav";
		m_WAV_pstar = clan::SoundBuffer(filename);

		filename = resource_dir + "redstar.wav";
		m_WAV_redstar = clan::SoundBuffer(filename);

		filename = resource_dir + "spiningtop.wav";
		m_WAV_spiningtop = clan::SoundBuffer(filename);

		filename = resource_dir + "spit.wav";
		m_WAV_spit = clan::SoundBuffer(filename);

		filename = resource_dir + "splat.wav";
		m_WAV_splat = clan::SoundBuffer(filename);

		filename = resource_dir + "tap.wav";
		m_WAV_tap = clan::SoundBuffer(filename);

		filename = resource_dir + "train.wav";
		m_WAV_train = clan::SoundBuffer(filename);

		filename = resource_dir + "tribble.wav";
		m_WAV_tribble = clan::SoundBuffer(filename);

		filename = resource_dir + "turbo.wav";
		m_WAV_turbo = clan::SoundBuffer(filename);

		filename = resource_dir + "twinkle.wav";
		m_WAV_twinkle = clan::SoundBuffer(filename);

		filename = resource_dir + "wings.wav";
		m_WAV_wings = clan::SoundBuffer(filename);

		filename = resource_dir + "wpotion.wav";
		m_WAV_wpotion = clan::SoundBuffer(filename);

		filename = resource_dir + "xylo.wav";
		m_WAV_xylo = clan::SoundBuffer(filename);

		filename = resource_dir + "boss.mod";
		m_MOD_boss = clan::SoundBuffer(filename);

		filename = resource_dir + "complete.mod";
		m_MOD_complete = clan::SoundBuffer(filename);

		filename = resource_dir + "empty.mod";
		m_MOD_empty = clan::SoundBuffer(filename);

		filename = resource_dir + "title.mod";
		m_MOD_title = clan::SoundBuffer(filename);

		filename = resource_dir + "tune1.mod";
		m_MOD_tune1 = clan::SoundBuffer(filename);

		filename = resource_dir + "tune2.mod";
		m_MOD_tune2 = clan::SoundBuffer(filename);
	}



}

//------------------------------------------------------------------------------
//! \brief Redraw screen (Called by the game)
//------------------------------------------------------------------------------
void CGameTarget::RedrawScreen()
{
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
	m_Game.MainLoop();
}

//------------------------------------------------------------------------------
//! \brief Prepare the sound driver (call before the game starts)
//------------------------------------------------------------------------------
void CGameTarget::PrepareSoundDriver()
{
	m_Game.m_Sound.PrepareAudio();
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

	clan::SoundBuffer *sound_ptr = NULL;

	switch(id)
	{
		case SMOD_TUNE1: 
			sound_ptr = &m_MOD_tune1;
			break;
		case SMOD_TUNE2: 
			sound_ptr = &m_MOD_tune2;
			break;
		case SMOD_BOSS: 
			sound_ptr = &m_MOD_boss;
			break;
		case SMOD_COMPLETE: 
			sound_ptr = &m_MOD_complete;
			break;
		case SMOD_TITLE: 
			sound_ptr = &m_MOD_title;
			break;	
		case SMOD_EMPTY: 
			sound_ptr = &m_MOD_empty;
			break;
		default:
			throw clan::Exception("Unknown music module id");
	}

	StopModule();
	m_Session = clan::SoundBuffer_Session(sound_ptr->prepare());
	m_Session.play();
	m_bSessionActive = true;
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CGameTarget::StopModule()
{
	if (!GLOBAL_SoundEnable)
		return;

	if (m_bSessionActive)
	{
		m_Session.stop();
		m_bSessionActive = false;
	}
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

	clan::SoundBuffer *sound_ptr = NULL;

	switch(id)
	{
		case SND_CAR:
			sound_ptr = &m_WAV_car;
			break;
		case SND_TRAIN:
			sound_ptr = &m_WAV_train;
			break;
		case SND_DUCK:
			sound_ptr = &m_WAV_duck;
			break;
		case SND_PICKUP1:
			sound_ptr = &m_WAV_pickup1;
			break;
		case SND_TRIBBLE:
			sound_ptr = &m_WAV_tribble;
			break;
		case SND_HURRY:
			sound_ptr = &m_WAV_hurry;
			break;
		case SND_DAY:
			sound_ptr = &m_WAV_day;
			break;
		case SND_CRYING:
			sound_ptr = &m_WAV_crying;
			break;
		case SND_DIE2:
			sound_ptr = &m_WAV_die2;
			break;
		case SND_SPIT:
			sound_ptr = &m_WAV_spit;
			break;
		case SND_SPLAT:
			sound_ptr = &m_WAV_splat;
			break;
		case SND_BLOW:
			sound_ptr = &m_WAV_blow;
			break;
		case SND_TWINKLE:
			sound_ptr = &m_WAV_twinkle;
			break;
		case SND_FINLEV1:
			sound_ptr = &m_WAV_finlev1;
			break;
		case SND_PSTAR:
			sound_ptr = &m_WAV_pstar;
			break;
		case SND_XYLO:
			sound_ptr = &m_WAV_xylo;
			break;
		case SND_CARDSOUND:
			sound_ptr = &m_WAV_card;
			break;
		case SND_BOWLINGSOUND:
			sound_ptr = &m_WAV_bowling;
			break;
		case SND_CANDLESOUND:
			sound_ptr = &m_WAV_candle;
			break;
		case SND_MARBLESOUND:
			sound_ptr = &m_WAV_marble;
			break;
		case SND_TAPSOUND:
			sound_ptr = &m_WAV_tap;
			break;
		case SND_OILSOUND:
			sound_ptr = &m_WAV_oil;
			break;
		case SND_SPININGTOPSOUND:
			sound_ptr = &m_WAV_spiningtop;
			break;
		case SND_WINGSSOUND:
			sound_ptr = &m_WAV_wings;
			break;
		case SND_MOONSOUND:
			sound_ptr = &m_WAV_moon;
			break;
		case SND_MASKSOUND:
			sound_ptr = &m_WAV_mask;
			break;
		case SND_REDSTARSOUND:
			sound_ptr = &m_WAV_redstar;
			break;
		case SND_TURBOSOUND:
			sound_ptr = &m_WAV_turbo;
			break;
		case SND_CHICKENSOUND:
			sound_ptr = &m_WAV_chicken;
			break;
		case SND_FEATHERSOUND:
			sound_ptr = &m_WAV_feather;
			break;
		case SND_WPOTIONSOUND:
			sound_ptr = &m_WAV_wpotion;
			break;
		case SND_COOKIESOUND:
			sound_ptr = &m_WAV_cookie;
			break;

		default:
			throw clan::Exception("Unknown sound sample id");
	}

	// Calculate panning from -1 to 1 (from 0 to 255)
	float pan = pos - 128;
	pan = pan / 128.0f;
	if (pan < -1.0f)
		pan = -1.0f;
	if (pan > 1.0f)
		pan = 1.0f;

	sound_ptr->set_pan(pan);

	clan::SoundBuffer_Session session = sound_ptr->prepare();
	session.set_frequency(rate);
	session.play();
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CGameTarget::UpdateModule(int id)
{
	if (!GLOBAL_SoundEnable)
		return;

	if (m_bSessionActive)
	{
		if (!m_Session.is_playing())
		{
			m_Session.play();
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Draw
//! 
//!	\param 
//! 
//!	\return 
//------------------------------------------------------------------------------
void CGameTarget::Draw(int dest_xpos, int dest_ypos, int width, int height, int texture_number, int texture_xpos, int texture_ypos, bool draw_white)
{
	float dest_width = m_Canvas.get_width();
	float dest_height = m_Canvas.get_height();
	float scr_width = SCR_WIDTH;
	float scr_height = SCR_HEIGHT;

	clan::Rectf dest = clan::Rectf((dest_xpos * dest_width) / scr_width, (dest_ypos * dest_height) / scr_height, 
		((dest_xpos + width) * (dest_width) / scr_width), ((dest_ypos + height) * (dest_height) / scr_height));

	clan::Rectf source = clan::Rectf(texture_xpos, texture_ypos, (texture_xpos+width), (texture_ypos+height));

	/*
	clan::Subtexture sub_texture(m_Texture[texture_number], source);
	clan::Image image(sub_texture);
	image.draw(m_Canvas, dest);
	*/

	m_Batcher->draw_image(m_Canvas, source, dest, draw_white ? 1.0f : 0.0f, m_Texture[texture_number], m_Lighting);
} 

