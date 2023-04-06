#include <string.h>
#include <math.h>
#include <iosfwd>
#include <iostream>
#include <ios>
#include <streambuf>
#include <istream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <fstream>

#include "music_and_sound_g.h"

#include "basics.h"

#include "../../fmod/inc/fmod.hpp"
#include "../../fmod/inc/fmod_common.h"

using std::string;

static float volume_sfx = 1.0f;

static float volume_master = 1.0f;

static FMOD::System *fmod_system;

static std::array<FMOD::Channel *,10> sound_channel;
static FMOD::Channel *song_channel;
static FMOD::Channel *ambience_channel[2];
static FMOD::Channel *card_channel;

static std::array<FMOD::Sound *,MAXSONGNUM> mod;
static std::array<FMOD::Sound *,MAXSOUNDNUM> samp;
static std::array<FMOD::Sound *,MAXSOUNDNUM> stream;

FMOD_RESULT result;

extern "C"
{
	 bool is_ambience_playing(int32_t ambience_active_channel) 
		{
		bool active_is_playing=false;
		if(ambience_channel[ambience_active_channel]!=NULL)
			{
			result=ambience_channel[ambience_active_channel]->isPlaying(&active_is_playing);
			}
		return active_is_playing;
		}
	 void stop_ambience(int32_t ambience_active_channel) 
		{
		if(ambience_channel[ambience_active_channel]!=NULL)
			{
			ambience_channel[ambience_active_channel]->stop();
			ambience_channel[ambience_active_channel]=NULL;
			}
		}
	 void play_ambience(int32_t new_ambience,int32_t ambience_active_channel) 
		{
		result=fmod_system->playSound(stream[new_ambience],NULL,true,&ambience_channel[ambience_active_channel]);
		ambience_channel[ambience_active_channel]->setPriority(2);
		ambience_channel[ambience_active_channel]->setPan(0.0f);
		//ambience continues from where it left off
		//ambience_channel[ambience_active_channel]->setPosition(0,FMOD_TIMEUNIT_MS);
		ambience_channel[ambience_active_channel]->setVolume(0.0f);
		ambience_channel[ambience_active_channel]->setPaused(false);
		}
	 bool is_card_playing() 
		{
		bool is_playing=false;
		if(card_channel!=NULL)
			{
			result=card_channel->isPlaying(&is_playing);
			}
		return is_playing;
		}
	 void stop_card() 
		{
		if(card_channel!=NULL)
			{
			card_channel->stop();
			card_channel=NULL;
			}
		}
	 void start_card(int32_t new_song) 
		{
		stop_card();
		result=fmod_system->playSound(stream[new_song],NULL,true,&card_channel);
		card_channel->setPriority(1);
		card_channel->setPan(0.0f);
		card_channel->setPosition(0,FMOD_TIMEUNIT_MS);
		card_channel->setVolume(0.0f);
		card_channel->setPaused(false);
		}
	 bool is_song_playing() 
		{
		bool is_playing=false;
		if(song_channel!=NULL)
			result=song_channel->isPlaying(&is_playing);
		return is_playing;
		}
	 void stop_song() 
		{
		if(song_channel!=NULL)
			{
			song_channel->stop();
			song_channel=NULL;
			}
		}
	 void start_song(int32_t new_song) 
		{
		result=fmod_system->playSound(stream[new_song],NULL,true,&song_channel);
		song_channel->setPriority(0);
		song_channel->setPan(0.0f);
		song_channel->setPosition(0,FMOD_TIMEUNIT_MS);
		song_channel->setVolume(0.0f);
		song_channel->setPaused(false);
		}
	 void play_sound(int s,int32_t vol,bool use_media_sound_volume) 
		{
		int32_t i;
		for(i=0; i<10; ++i)
			{
			if(sound_channel[i]==NULL)
				{
				result=fmod_system->playSound(samp[s],0,true,&sound_channel[i]);
				sound_channel[i]->setPriority(128);
				if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.75f);
				else sound_channel[i]->setPan(0.0f);
				sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
				if(use_media_sound_volume)
					{
					if(vol==255)sound_channel[i]->setVolume(volume_sfx*volume_master);
					else sound_channel[i]->setVolume(((float)vol/255.0f)*volume_sfx*volume_master);
					}
				else
					{
					if(vol==255)sound_channel[i]->setVolume(volume_master);
					else sound_channel[i]->setVolume(((float)vol/255.0f)*volume_master);
					}
				sound_channel[i]->setPaused(false);
				return;
				}
			}
		for(i=0; i<10; ++i)
			{
			if(sound_channel[i]!=NULL)
				{
				bool is_playing=false;
				sound_channel[i]->isPlaying(&is_playing);
				if(!is_playing)
					{
					result=fmod_system->playSound(samp[s],0,true,&sound_channel[i]);
					sound_channel[i]->setPriority(128);
					if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.5f);
					else sound_channel[i]->setPan(0.0f);
					sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
					sound_channel[i]->setVolume(volume_sfx*volume_master);
					sound_channel[i]->setPaused(false);
					return;
					}
				}
			}
		i=0;
		if(sound_channel[i]!=NULL)
			{
			sound_channel[i]->stop();

			result=fmod_system->playSound(samp[s],0,true,&sound_channel[i]);
			sound_channel[i]->setPriority(128);
			if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.5f);
			else sound_channel[i]->setPan(0.0f);
			sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
			sound_channel[i]->setVolume(volume_sfx*volume_master);
			sound_channel[i]->setPaused(false);
			}
		}
	 bool set_song(const string &filename,int slot,bool loops)
	{
		if(stream[slot]!=NULL)
			{
			stream[slot]->release();
			stream[slot]=NULL;
			}

		result=fmod_system->createStream(filename.c_str(),(loops?FMOD_LOOP_NORMAL:0)|FMOD_2D,0,&stream[slot]);
		return result==FMOD_OK;
	}
	 bool set_sound(const string &filename,int slot)
	{
		if(samp[slot]!=NULL)
			{
			samp[slot]->release();
			samp[slot]=NULL;
			}

		result=fmod_system->createSound(filename.c_str(),FMOD_LOOP_OFF,0,&samp[slot]);
		return result==FMOD_OK;
	}
	 void set_init_volumes(int master_volume, int snd_volume, int ambience_volume, int music_volume)
	 {
		 volume_master=volume_lookup[master_volume];
		 volume_sfx=volume_lookup[snd_volume];
	 }

	 void set_song_volume(float vol)
	{
		if(song_channel!=NULL)
			{
			song_channel->setVolume(vol);
			}
	}
	 void set_card_volume(float vol)
	{
		if(card_channel!=NULL)
			{
			card_channel->setVolume(vol);
			}
	}
	 void set_ambience_volume(int32_t chan,float vol)
	{
		if(ambience_channel[chan]!=NULL)
			{
			ambience_channel[chan]->setVolume(vol);
			}
	}
	 float get_song_volume()
	{
		float volume=0.0f;
		if(song_channel!=NULL)
			{
			song_channel->getVolume(&volume);
			}
		return volume;
	};
	 float get_card_volume()
	{
		float volume=0.0f;
		if(card_channel!=NULL)
			{
			card_channel->getVolume(&volume);
			}
		return volume;
	};
	 float get_ambience_volume(int32_t chan)
	{
		float volume=0.0f;
		if(ambience_channel[chan]!=NULL)
			{
			ambience_channel[chan]->getVolume(&volume);
			}
		return volume;
	};
	 bool init_sound() 
		{
		result=FMOD::System_Create(&fmod_system);
		if(fmod_system==NULL)return false;

		sound_channel.fill(NULL);
		song_channel = NULL;
		ambience_channel[0] = NULL;
		ambience_channel[1] = NULL;
		card_channel = NULL;
		mod.fill(NULL);
		samp.fill(NULL);
		stream.fill(NULL);
		result=fmod_system->init(SOUND_CHANNELNUM,FMOD_INIT_NORMAL,NULL);
		fmod_system->setStreamBufferSize(2000,FMOD_TIMEUNIT_MS);
		return true;
		}
	 void deinit_sound() 
	{
		for(auto &chan:sound_channel)
			{
			if(chan) chan->stop();
			};
		if(song_channel)song_channel->stop();
		if(ambience_channel[0]!=NULL)ambience_channel[0]->stop();
		if(ambience_channel[1]!=NULL)ambience_channel[1]->stop();
		if(card_channel!=NULL)card_channel->stop();
		for(auto &m:mod)
			{
			if(m) m->release();
			}
		for(auto &s:samp)
			{
			if(s) s->release();
			}

		fmod_system->close();
		fmod_system->release();
		fmod_system=NULL;
	}
	 void update() 
	{
		if(fmod_system==NULL)return;

		fmod_system->update();
	}
}