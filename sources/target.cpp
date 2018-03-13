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

#include <stdlib.h>
#include <string.h>
#include "target.h"

#include "doc.h"

//------------------------------------------------------------------------------
// The game target (Yuck global!)
// Thus - Only a single GameTarget is allowed
//------------------------------------------------------------------------------
CGameTarget *GLOBAL_GameTarget = 0;

const char Shader_Vertex_White[] =
"	attribute vec4 in_position;\n"
"	attribute vec2 in_texcoord;\n"
"	uniform mat4 cl_ModelViewProjectionMatrix;"
"	varying vec2 texCoord;\n"
"	void main()\n"
"	{\n"
"		gl_Position = cl_ModelViewProjectionMatrix * in_position;\n"
"		texCoord.xy = in_texcoord.xy;\n"
"	}\n";

const char Shader_Fragment_White[] =
"	varying vec2 texCoord;\n"
"	uniform sampler2D decalMap;\n"
"	uniform float Lighting;\n"
"	void main()\n"
"	{\n"
"		vec4 decalCol = texture2D(decalMap, texCoord);\n"
"		if (decalCol.a == 1.0)\n"
"		{\n"
"			gl_FragColor.r = clamp(1.0 + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.g = clamp(1.0 + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.b = clamp(1.0 + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.a = 1.0;\n"
"		}else\n"
"		{\n"
"			gl_FragColor.r = 0.0;\n"
"			gl_FragColor.g = 0.0;\n"
"			gl_FragColor.b = 0.0;\n"
"			gl_FragColor.a = 0.0;\n"
"		}\n"
"	}\n";

const char Shader_Vertex_Standard[] =
"	attribute vec4 in_position;\n"
"	attribute vec2 in_texcoord;\n"
"	uniform mat4 cl_ModelViewProjectionMatrix;"
"	varying vec2 texCoord;\n"
"	void main()\n"
"	{\n"
"		gl_Position = cl_ModelViewProjectionMatrix * in_position;\n"
"		texCoord.xy = in_texcoord.xy;\n"
"	}\n";

const char Shader_Fragment_Standard[] =
"	varying vec2 texCoord;\n"
"	uniform sampler2D decalMap;\n"
"	uniform float Lighting;\n"
"	void main()\n"
"	{\n"
"		vec4 decalCol = texture2D(decalMap, texCoord);\n"
"		if (decalCol.a == 1.0)\n"
"		{\n"
"			gl_FragColor.r = clamp(decalCol.r + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.g = clamp(decalCol.g + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.b = clamp(decalCol.b + Lighting, 0.0, 1.0);\n"
"			gl_FragColor.a = 1.0;\n"
"		}else\n"
"		{\n"
"			gl_FragColor.r = 1.0;\n"
"			gl_FragColor.g = 0.0;\n"
"			gl_FragColor.b = 0.0;\n"
"			gl_FragColor.a = 0.0;\n"
"		}\n"
"	}\n";

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

	m_LastShaderType = shader_none;
}

//------------------------------------------------------------------------------
//! \brief Initialisation
//!
//! 	\param pdoc = Pointer to the document this target belongs to
//!	\param window = Screen to draw to
//------------------------------------------------------------------------------
void CGameTarget::Init(CMethDoc *pdoc, CL_DisplayWindow &window)
{
	m_pWindow = &window;
	m_pDoc = pdoc;
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CGameTarget::InitGame()
{
	m_Game.Init(this, &m_Joy1, &m_Joy2);

	CL_GraphicContext gc = m_pWindow->get_gc();

	// Find the resources directory:
	CL_String resource_dir = CL_Directory::get_resourcedata("methane");
	CL_String dataname("page_01.png");
	CL_String filename = resource_dir + dataname;
	if (!CL_FileHelp::file_exists(filename))
	{
		resource_dir = CL_String("resources/");
		filename = resource_dir + dataname;
		if (!CL_FileHelp::file_exists(filename))
		{
#ifdef WIN32
			throw CL_Exception("Unable to locate resources");
#else
			resource_dir = CL_String("/usr/share/methane/resources/");
			filename = resource_dir + dataname;
			if (!CL_FileHelp::file_exists(filename))
			{
				resource_dir = CL_String("/usr/share/methane/");
				filename = resource_dir + dataname;
				if (!CL_FileHelp::file_exists(filename))
				{
					throw CL_Exception("Unable to locate resources");
				}
			}
#endif
		}
	}
	

	CL_VirtualDirectory vdir;
	filename = resource_dir + "page_01.png";
	CL_PixelBuffer image = CL_ImageProviderFactory::load(filename);
	m_Texture[0] = CL_Texture(gc, image.get_width(), image.get_height());
	m_Texture[0].set_image(image);
	m_Texture[0].set_min_filter(cl_filter_nearest);
	m_Texture[0].set_mag_filter(cl_filter_nearest);

	filename = resource_dir + "page_02.png";
	image = CL_ImageProviderFactory::load(CL_StringRef(filename));
	m_Texture[1] = CL_Texture(gc, image.get_width(), image.get_height());
	m_Texture[1].set_image(image);
	m_Texture[1].set_min_filter(cl_filter_nearest);
	m_Texture[1].set_mag_filter(cl_filter_nearest);

	filename = resource_dir + "page_03.png";
	image = CL_ImageProviderFactory::load(filename);
	m_Texture[2] = CL_Texture(gc, image.get_width(), image.get_height());
	m_Texture[2].set_image(image);
	m_Texture[2].set_min_filter(cl_filter_nearest);
	m_Texture[2].set_mag_filter(cl_filter_nearest);

	filename = resource_dir + "page_04.png";
	image = CL_ImageProviderFactory::load(filename);
	m_Texture[3] = CL_Texture(gc, image.get_width(), image.get_height());
	m_Texture[3].set_image(image);
	m_Texture[3].set_min_filter(cl_filter_nearest);
	m_Texture[3].set_mag_filter(cl_filter_nearest);

	filename = resource_dir + "page_05.png";
	image = CL_ImageProviderFactory::load(filename);
	m_Texture[4] = CL_Texture(gc, image.get_width(), image.get_height());
	m_Texture[4].set_image(image);
	m_Texture[4].set_min_filter(cl_filter_nearest);
	m_Texture[4].set_mag_filter(cl_filter_nearest);

	if (GLOBAL_SoundEnable)
	{

		filename = resource_dir + "blow.wav";
		m_WAV_blow = CL_SoundBuffer(filename);

		filename = resource_dir + "bowling.wav";
		m_WAV_bowling = CL_SoundBuffer(filename);

		filename = resource_dir + "candle.wav";
		m_WAV_candle = CL_SoundBuffer(filename);

		filename = resource_dir + "card.wav";
		m_WAV_card = CL_SoundBuffer(filename);

		filename = resource_dir + "car.wav";
		m_WAV_car = CL_SoundBuffer(filename);

		filename = resource_dir + "chicken.wav";
		m_WAV_chicken = CL_SoundBuffer(filename);

		filename = resource_dir + "cookie.wav";
		m_WAV_cookie = CL_SoundBuffer(filename);

		filename = resource_dir + "crying.wav";
		m_WAV_crying = CL_SoundBuffer(filename);

		filename = resource_dir + "day.wav";
		m_WAV_day = CL_SoundBuffer(filename);

		filename = resource_dir + "die2.wav";
		m_WAV_die2 = CL_SoundBuffer(filename);

		filename = resource_dir + "duck.wav";
		m_WAV_duck = CL_SoundBuffer(filename);

		filename = resource_dir + "feather.wav";
		m_WAV_feather = CL_SoundBuffer(filename);

		filename = resource_dir + "finlev1.wav";
		m_WAV_finlev1 = CL_SoundBuffer(filename);

		filename = resource_dir + "hurry.wav";
		m_WAV_hurry = CL_SoundBuffer(filename);

		filename = resource_dir + "marble.wav";
		m_WAV_marble = CL_SoundBuffer(filename);

		filename = resource_dir + "mask.wav";
		m_WAV_mask = CL_SoundBuffer(filename);

		filename = resource_dir + "moon.wav";
		m_WAV_moon = CL_SoundBuffer(filename);

		filename = resource_dir + "oil.wav";
		m_WAV_oil = CL_SoundBuffer(filename);

		filename = resource_dir + "pickup1.wav";
		m_WAV_pickup1 = CL_SoundBuffer(filename);

		filename = resource_dir + "pstar.wav";
		m_WAV_pstar = CL_SoundBuffer(filename);

		filename = resource_dir + "redstar.wav";
		m_WAV_redstar = CL_SoundBuffer(filename);

		filename = resource_dir + "spiningtop.wav";
		m_WAV_spiningtop = CL_SoundBuffer(filename);

		filename = resource_dir + "spit.wav";
		m_WAV_spit = CL_SoundBuffer(filename);

		filename = resource_dir + "splat.wav";
		m_WAV_splat = CL_SoundBuffer(filename);

		filename = resource_dir + "tap.wav";
		m_WAV_tap = CL_SoundBuffer(filename);

		filename = resource_dir + "train.wav";
		m_WAV_train = CL_SoundBuffer(filename);

		filename = resource_dir + "tribble.wav";
		m_WAV_tribble = CL_SoundBuffer(filename);

		filename = resource_dir + "turbo.wav";
		m_WAV_turbo = CL_SoundBuffer(filename);

		filename = resource_dir + "twinkle.wav";
		m_WAV_twinkle = CL_SoundBuffer(filename);

		filename = resource_dir + "wings.wav";
		m_WAV_wings = CL_SoundBuffer(filename);

		filename = resource_dir + "wpotion.wav";
		m_WAV_wpotion = CL_SoundBuffer(filename);

		filename = resource_dir + "xylo.wav";
		m_WAV_xylo = CL_SoundBuffer(filename);

		filename = resource_dir + "boss.mod";
		m_MOD_boss = CL_SoundBuffer(filename);

		filename = resource_dir + "complete.mod";
		m_MOD_complete = CL_SoundBuffer(filename);

		filename = resource_dir + "empty.mod";
		m_MOD_empty = CL_SoundBuffer(filename);

		filename = resource_dir + "title.mod";
		m_MOD_title = CL_SoundBuffer(filename);

		filename = resource_dir + "tune1.mod";
		m_MOD_tune1 = CL_SoundBuffer(filename);

		filename = resource_dir + "tune2.mod";
		m_MOD_tune2 = CL_SoundBuffer(filename);
	}

	if (GLOBAL_RenderTarget == opengl2)
	{

		CL_ShaderObject vertex_shader(gc, cl_shadertype_vertex, Shader_Vertex_White);
		if(!vertex_shader.compile())
		{
			throw CL_Exception(cl_format("Unable to compile vertex shader object: %1", vertex_shader.get_info_log()));
		}

		CL_ShaderObject fragment_shader(gc, cl_shadertype_fragment, Shader_Fragment_White);
		if(!fragment_shader.compile())
		{
			throw CL_Exception(cl_format("Unable to compile fragment shader object: %1", fragment_shader.get_info_log()));
		}

		m_Shader_DrawWhite = CL_ProgramObject(gc);
		m_Shader_DrawWhite.attach(vertex_shader);
		m_Shader_DrawWhite.attach(fragment_shader);
		m_Shader_DrawWhite.bind_attribute_location(0, "in_position");
		m_Shader_DrawWhite.bind_attribute_location(1, "in_texcorrd");

		if (!m_Shader_DrawWhite.link())
		{
			throw CL_Exception(cl_format("Unable to link program object: %1", m_Shader_DrawWhite.get_info_log()));
		}

		vertex_shader = CL_ShaderObject(gc, cl_shadertype_vertex, Shader_Vertex_Standard);
		if(!vertex_shader.compile())
		{
			throw CL_Exception(cl_format("Unable to compile vertex shader object: %1", vertex_shader.get_info_log()));
		}

		fragment_shader = CL_ShaderObject(gc, cl_shadertype_fragment, Shader_Fragment_Standard);
		if(!fragment_shader.compile())
		{
			throw CL_Exception(cl_format("Unable to compile fragment shader object: %1", fragment_shader.get_info_log()));
		}

		m_Shader_Standard = CL_ProgramObject(gc);
		m_Shader_Standard.attach(vertex_shader);
		m_Shader_Standard.attach(fragment_shader);
		m_Shader_Standard.bind_attribute_location(0, "in_position");
		m_Shader_Standard.bind_attribute_location(1, "in_texcorrd");

		if (!m_Shader_Standard.link())
		{
			throw CL_Exception(cl_format("Unable to link program object: %1", m_Shader_Standard.get_info_log()));
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Redraw screen (Called by the game)
//------------------------------------------------------------------------------
void CGameTarget::RedrawScreen(void)
{
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CGameTarget::StartGame(void)
{
	m_Game.StartGame();
}

//------------------------------------------------------------------------------
//! \brief Do the game main loop (Call every cycle)
//------------------------------------------------------------------------------
void CGameTarget::MainLoop(void)
{
	m_Game.MainLoop();
}

//------------------------------------------------------------------------------
//! \brief Prepare the sound driver (call before the game starts)
//------------------------------------------------------------------------------
void CGameTarget::PrepareSoundDriver(void)
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

	CL_SoundBuffer *sound_ptr = NULL;

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
			throw CL_Exception("Unknown music module id");
	}

	StopModule();
	m_Session = CL_SoundBuffer_Session(sound_ptr->prepare());
	m_Session.play();
	m_bSessionActive = true;
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CGameTarget::StopModule(void)
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

	CL_SoundBuffer *sound_ptr = NULL;

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
			throw CL_Exception("Unknown sound sample id");
	}

	// Calculate panning from -1 to 1 (from 0 to 255)
	float pan = pos - 128;
	pan = pan / 128.0f;
	if (pan < -1.0f)
		pan = -1.0f;
	if (pan > 1.0f)
		pan = 1.0f;

	sound_ptr->set_pan(pan);

	CL_SoundBuffer_Session session = sound_ptr->prepare();
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
	CL_GraphicContext gc = m_pWindow->get_gc();

	float dest_width = gc.get_width();
	float dest_height = gc.get_height();
	float scr_width = SCR_WIDTH;
	float scr_height = SCR_HEIGHT;

	CL_Rectf dest = CL_Rectf((dest_xpos * dest_width) / scr_width, (dest_ypos * dest_height) / scr_height, 
		((dest_xpos + width) * (dest_width) / scr_width), ((dest_ypos + height) * (dest_height) / scr_height));

	CL_Rectf source = CL_Rectf(texture_xpos, texture_ypos, (texture_xpos+width), (texture_ypos+height));

	if (GLOBAL_RenderTarget == opengl2)
	{
		gc.set_texture(0, m_Texture[texture_number]);

		if (draw_white)
		{
			m_Shader_DrawWhite.set_uniform1f("Lighting", m_Lighting);
			if (m_LastShaderType != shader_drawwhite)
			{
				gc.set_program_object(m_Shader_DrawWhite);
				m_LastShaderType = shader_drawwhite;
			}
		}else
		{
			m_Shader_Standard.set_uniform1f("Lighting", m_Lighting);
			if (m_LastShaderType != shader_standard)
			{
				gc.set_program_object(m_Shader_Standard);
				m_LastShaderType = shader_standard;
			}
		}
		CL_Vec2f positions[6] =
		{
			CL_Vec2f(dest.left, dest.top),
			CL_Vec2f(dest.right, dest.top),
			CL_Vec2f(dest.left, dest.bottom),
			CL_Vec2f(dest.right, dest.top),
			CL_Vec2f(dest.left, dest.bottom),
			CL_Vec2f(dest.right, dest.bottom)
		};

		float tex_width = m_Texture[texture_number].get_width();
		float tex_height = m_Texture[texture_number].get_width();

		source.left /= tex_width;
		source.right /= tex_width;
		source.top /= tex_height;
		source.bottom /= tex_height;

		CL_Vec2f tex1_coords[6] =
		{
			CL_Vec2f(source.left, source.top),
			CL_Vec2f(source.right, source.top),
			CL_Vec2f(source.left, source.bottom),
			CL_Vec2f(source.right, source.top),
			CL_Vec2f(source.left, source.bottom),
			CL_Vec2f(source.right, source.bottom)
		};

		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, positions);
		prim_array.set_attributes(1, tex1_coords);
		gc.draw_primitives(cl_triangles, 6, prim_array);
	}
	else
	{
		CL_Subtexture sub_texture(m_Texture[texture_number], source);
		CL_Image image(gc, sub_texture);
		image.draw(gc, dest);
	}

} 

