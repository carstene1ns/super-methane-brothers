
#include "precomp.h"

#ifndef DISABLE_SOUND

#include "soundprovider_mikmod_session.h"
#include "soundprovider_mikmod_impl.h"
#include "module_reader.h"

SoundProvider_MikMod_Session::SoundProvider_MikMod_Session(SoundProvider_MikMod &source) :
	source(source), num_samples(0), position(0), stream_eof(false)
{
	std::unique_ptr<clan::MemoryDevice> input(new clan::MemoryDevice(source.impl->buffer));

	MREADER *reader = new_clanlib_reader(input.get());
	if (reader == 0) throw clan::Exception("new_clanlib_reader failed!");

	module = Player_LoadGeneric(reader,CLANLIB_READER_CHANNELS,0);
	if (module == 0)
	{
		delete_clanlib_reader(reader);
		throw clan::Exception(MikMod_strerror(MikMod_errno));
	}

	module->wrap = false;
	module->loop = false;
	module->fadeout = true;

	delete_clanlib_reader(reader);

	frequency = md_mixfreq;
	format = (md_mode & DMODE_16BITS) ? clan::SoundFormat::sf_16bit_signed : clan::SoundFormat::sf_8bit_unsigned;
	num_channels = (md_mode&DMODE_STEREO) ? 2 : 1;
}

SoundProvider_MikMod_Session::~SoundProvider_MikMod_Session()
{
	Player_Stop();
	Player_Free(module);
}

int SoundProvider_MikMod_Session::get_num_samples() const
{
	return num_samples;
}

int SoundProvider_MikMod_Session::get_frequency() const
{
	return frequency;
}

int SoundProvider_MikMod_Session::get_num_channels() const
{
	return num_channels;
}

int SoundProvider_MikMod_Session::get_position() const
{
	return position;
}

bool SoundProvider_MikMod_Session::set_looping(bool loop)
{
	module->wrap = loop;
	module->loop = loop;

	return true;
}

bool SoundProvider_MikMod_Session::eof() const
{
	return !Player_Active();
}

void SoundProvider_MikMod_Session::stop()
{
	Player_Stop();
}

bool SoundProvider_MikMod_Session::play()
{
	Player_Start (module);
	return true;
}
	
bool SoundProvider_MikMod_Session::set_position(int pos)
{
	Player_SetPosition(pos);
	position = pos;
	if (num_samples < position) num_samples = position;
	return true;
}

int SoundProvider_MikMod_Session::get_data(float **channels, int data_requested)
{

	int total_written = 0;
	int bytes_per_sample = (format == clan::SoundFormat::sf_16bit_signed) ? 2 : 1;
	bytes_per_sample *= num_channels;

	while (data_requested > 0)
	{
		MikMod_Update();
		int written = VC_WriteBytes(buffer, ((data_requested > 16*1024) ? 16*1024 : data_requested) * bytes_per_sample);
		written /= bytes_per_sample;

		if (format == clan::SoundFormat::sf_16bit_signed)
		{
			short *src = (short *) buffer;
			if (num_channels == 2)
			{
				float *temp_data_ptr[2];
				temp_data_ptr[0] = channels[0] + total_written;
				temp_data_ptr[1] = channels[1] + total_written;
				clan::SoundSSE::unpack_16bit_stereo(src, written*2, temp_data_ptr);
			}
			else
			{
				clan::SoundSSE::unpack_16bit_mono(src, written*2, channels[0] + total_written);
			}
		}
		else if (format == clan::SoundFormat::sf_8bit_unsigned)
		{
			unsigned char *src = (unsigned char *) buffer;
			if (num_channels == 2)
			{
				float *temp_data_ptr[2];
				temp_data_ptr[0] = channels[0] + total_written;
				temp_data_ptr[1] = channels[1] + total_written;

				clan::SoundSSE::unpack_8bit_stereo(src, written, temp_data_ptr);
			}
			else
			{
				clan::SoundSSE::unpack_8bit_mono(src, written, channels[0] + total_written);
			}
		}

		position += written;
		total_written += written;
		data_requested -= written;

		// Check if we reached the end of the song:
		if (module->sngpos >= module->numpos)
		{
			stop();
			break;
		}

		// If no more bytes to write from input stream:
		// (This never happens with this version of mikmod, but its a good check anyway)
		if (written < 16*1024) break;
	}

	if (num_samples < position) num_samples = position;
	return total_written;

}

#endif //DISABLE_SOUND
