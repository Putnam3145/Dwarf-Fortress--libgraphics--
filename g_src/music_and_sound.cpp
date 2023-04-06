//copyright (c) 2006 by tarn adams

#include <windows.h>
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

#include "svector.h"

#ifndef INTEGER_TYPES

#define INTEGER_TYPES

#ifdef WIN32
	typedef signed char int8_t;
	typedef short int16_t;
	typedef int int32_t;
	typedef long long int64_t;
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int uint32_t;
	typedef unsigned long long uint64_t;
#endif

typedef int32_t VIndex;
typedef int32_t Ordinal;

#endif

#include "random.h"

using std::string;

#include "endian.h"

#include "files.h"

#include "basics.h"

#include "enabler.h"

#include "init.h"

#ifndef NO_FMOD

#include "music_and_sound_g.h"
#include "music_and_sound_v.h"

//MUSIC AND SOUND FUNCTIONS

#ifdef DEBUGOPT
	#define MUSIC_DEBUG
#endif

//MUSIC
void musicsoundst::play_ambience(int32_t new_ambience)
{
	if(!on)return;

	bool active_is_playing=false;
	if(ambience_channel[ambience_active_channel]!=NULL)
		{
		result=ambience_channel[ambience_active_channel]->isPlaying(&active_is_playing);
		}

	if((new_ambience!=ambience_active||
		!active_is_playing)&&
		!ambience_crossfading)
		{
		if(active_is_playing)
			{
			if(ambience_channel[1-ambience_active_channel]!=NULL)
				{
				ambience_channel[1-ambience_active_channel]->stop();
				ambience_channel[1-ambience_active_channel]=NULL;
				}

			ambience_fading=ambience_active;
			ambience_active=new_ambience;

			ambience_active_channel=1-ambience_active_channel;
			ambience_crossfading=true;
			ambience_crossfade_start=GetTickCount();
			}
		else
			{
			if(ambience_channel[ambience_active_channel]!=NULL)
				{
				ambience_channel[ambience_active_channel]->stop();
				ambience_channel[ambience_active_channel]=NULL;
				}
			if(ambience_channel[1-ambience_active_channel]!=NULL)
				{
				ambience_channel[1-ambience_active_channel]->stop();
				ambience_channel[1-ambience_active_channel]=NULL;
				}

			ambience_active=new_ambience;
			}

		result=system->playSound(stream[new_ambience],NULL,true,&ambience_channel[ambience_active_channel]);
		ambience_channel[ambience_active_channel]->setPriority(2);
		ambience_channel[ambience_active_channel]->setPan(0.0f);
		//ambience continues from where it left off
		//ambience_channel[ambience_active_channel]->setPosition(0,FMOD_TIMEUNIT_MS);
		ambience_channel[ambience_active_channel]->setVolume(0.0f);
		ambience_channel[ambience_active_channel]->setPaused(false);

		++total_plays[new_ambience];
		}
}

void musicsoundst::start_card(int new_song)
{
	if(!on)return;

	bool is_playing=false;
	if(card_channel!=NULL)
		{
		result=card_channel->isPlaying(&is_playing);
		}

	if(!is_playing)
		{
		if(card_channel!=NULL)
			{
			card_channel->stop();
			card_channel=NULL;
			}

		result=system->playSound(stream[new_song],NULL,true,&card_channel);
		card_channel->setPriority(1);
		card_channel->setPan(0.0f);
		card_channel->setPosition(0,FMOD_TIMEUNIT_MS);
		card_channel->setVolume(0.0f);
		card_channel->setPaused(false);

		++total_plays[new_song];

		card_playing=new_song;

		switch(new_song)
			{
			case SONG_NEUTRAL_CARD_1:
			case SONG_NEUTRAL_CARD_2:
			case SONG_NEUTRAL_CARD_3:
			case SONG_NEUTRAL_CARD_4:
			case SONG_NEUTRAL_CARD_5:
			case SONG_NEUTRAL_CARD_6:
			case SONG_NEUTRAL_CARD_7:
			case SONG_NEUTRAL_CARD_8:
			case SONG_NEUTRAL_CARD_9:
			case SONG_NEUTRAL_CARD_10:
			case SONG_NEUTRAL_CARD_11:
			case SONG_NEUTRAL_CARD_12:
			case SONG_NEUTRAL_CARD_13:
			case SONG_NEUTRAL_CARD_14:
			case SONG_NEUTRAL_CARD_15:
			case SONG_NEUTRAL_CARD_16:
				neutral_card_history[2]=neutral_card_history[1];
				neutral_card_history[1]=neutral_card_history[0];
				neutral_card_history[0]=new_song;
				break;
			}

		card_started_count=GetTickCount();
		}
}

int32_t musicsoundst::get_song_playing()
{
	if(!on)return -1;

	if(song_channel==NULL)return -1;

	bool is_playing;
	result=song_channel->isPlaying(&is_playing);

	if(is_playing)
		{
		return song;
		}
	else return -1;
}

int32_t musicsoundst::get_card_playing()
{
	if(!on)return -1;

	if(card_channel==NULL)return -1;

	bool is_playing;
	result=card_channel->isPlaying(&is_playing);

	if(is_playing)
		{
		return card_playing;
		}
	else return -1;
}

void musicsoundst::startbackgroundmusic(int new_song)
{
	if(!on)return;

#ifdef DEBUGOPT
	string str="enter start background music: new ";
		add_long_to_string(new_song,str);str+=" vs old ";
		add_long_to_string(song,str);
	errorlog_string(str);
#endif

	bool is_playing=false;
	if(song_channel!=NULL)
		{
		result=song_channel->isPlaying(&is_playing);
		}

	if(new_song!=song||!is_playing)
		{
		if(!is_playing)
			{
#ifdef DEBUGOPT
	string str=" no song was playing, play ";
		add_long_to_string(new_song,str);
		str+=" immediately";
	errorlog_string(str);
#endif

			if(song!=new_song&&song>=0&&song<SONGNUM)
				{
				if(song_channel!=NULL)
					{
					song_channel->stop();
					song_channel=NULL;
					}
				}

			song=new_song;
			if(queued_song==song)queued_song=-1;

			result=system->playSound(stream[new_song],NULL,true,&song_channel);
			song_channel->setPriority(0);
			song_channel->setPan(0.0f);
			song_channel->setPosition(0,FMOD_TIMEUNIT_MS);
			song_channel->setVolume(0.0f);
			song_channel->setPaused(false);

			++total_plays[new_song];

			play_history[2]=play_history[1];
			play_history[1]=play_history[0];
			play_history[0]=new_song;

			song_started_count=GetTickCount();
			}
		else
			{
#ifdef DEBUGOPT
	string str=" old song is still playing, queuing up ";
		add_long_to_string(new_song,str);
	errorlog_string(str);
#endif

			queued_song=new_song;
			queued_song_count=GetTickCount();
			}
		}
}

//SOUND
void musicsoundst::playsound(int s,int32_t vol,bool use_media_sound_volume)
{
	if(!on)return;

	int32_t i;
	for(i=0;i<10;++i)
		{
		if(sound_channel[i]==NULL)
			{
			result=system->playSound(samp[s],0,true,&sound_channel[i]);
			sound_channel[i]->setPriority(128);
			if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.75f);
			else sound_channel[i]->setPan(0.0f);
			sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
			if(use_media_sound_volume)
				{
				if(vol==255)sound_channel[i]->setVolume(((float)init.media.volume_sfx/255.0f)*((float)init.media.volume_master/255.0f));
				else sound_channel[i]->setVolume(((float)vol/255.0f)*((float)init.media.volume_sfx/255.0f)*((float)init.media.volume_master/255.0f));
				}
			else
				{
				if(vol==255)sound_channel[i]->setVolume(((float)init.media.volume_master/255.0f));
				else sound_channel[i]->setVolume(((float)vol/255.0f)*((float)init.media.volume_master/255.0f));
				}
			sound_channel[i]->setPaused(false);
			return;
			}
		}
	for(i=0;i<10;++i)
		{
		if(sound_channel[i]!=NULL)
			{
			bool is_playing=false;
			sound_channel[i]->isPlaying(&is_playing);
			if(!is_playing)
				{
				result=system->playSound(samp[s],0,true,&sound_channel[i]);
				sound_channel[i]->setPriority(128);
				if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.5f);
				else sound_channel[i]->setPan(0.0f);
				sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
				sound_channel[i]->setVolume(((float)init.media.volume_sfx/255.0f)*((float)init.media.volume_master/255.0f));
				sound_channel[i]->setPaused(false);
				return;
				}
			}
		}
	i=0;
	if(sound_channel[i]!=NULL)
		{
		sound_channel[i]->stop();

		result=system->playSound(samp[s],0,true,&sound_channel[i]);
		sound_channel[i]->setPriority(128);
		if(s==SOUND_ALERT)sound_channel[i]->setPan(-0.5f);
		else sound_channel[i]->setPan(0.0f);
		sound_channel[i]->setPosition(0,FMOD_TIMEUNIT_MS);
		sound_channel[i]->setVolume(((float)init.media.volume_sfx/255.0f)*((float)init.media.volume_master/255.0f));
		sound_channel[i]->setPaused(false);
		}
}

void musicsoundst::initsound()
{
	if(on)return;

    result=FMOD::System_Create(&system);
	if(system==NULL)return;
    result=system->init(SOUND_CHANNELNUM,FMOD_INIT_NORMAL,NULL);

	on=1;

	musicactive=0;
	soundplaying=-1;
	soundpriority=0;
	song=-1;
	forcesongtime=-1;

	system->setStreamBufferSize(2000,FMOD_TIMEUNIT_MS);
}

void musicsoundst::deinitsound()
{
	if(!on)return;

	int32_t s;
	for(s=0;s<10;++s)
		{
		if(sound_channel[s]!=NULL)sound_channel[s]->stop();
		}
	if(song_channel!=NULL)song_channel->stop();
	if(ambience_channel[0]!=NULL)ambience_channel[0]->stop();
	if(ambience_channel[1]!=NULL)ambience_channel[1]->stop();
	if(card_channel!=NULL)card_channel->stop();

	for(s=0;s<MAXSONGNUM;s++)
		{
		if(mod[s]!=NULL)
			{
			mod[s]->release();
			mod[s]=NULL;
			}
		}

	for(s=0;s<MAXSOUNDNUM;s++)
		{
		if(samp[s]!=NULL)
			{
			samp[s]->release();
			samp[s]=NULL;
			}
		}

	system->close();
	system->release();
	system=NULL;

	on=0;
}

void musicsoundst::set_song(string &filename,int slot,bool loops)
{
	if(!on)return;

	if(stream[slot]!=NULL)
		{
		stream[slot]->release();
		stream[slot]=NULL;
		}

    result=system->createStream(filename.c_str(),(loops?FMOD_LOOP_NORMAL:0)|FMOD_2D,0,&stream[slot]);
}

void musicsoundst::set_sound(string &filename,int slot)
{
	if(!on)return;

	if(samp[slot]!=NULL)
		{
		samp[slot]->release();
		samp[slot]=NULL;
		}

	result=system->createSound(filename.c_str(),FMOD_LOOP_OFF,0,&samp[slot]);
}

bool musicsoundst::song_is_playing()
{
	bool is_playing=false;
	if(song_channel!=NULL)
		{
		result=song_channel->isPlaying(&is_playing);
		}
	return is_playing;
}

void musicsoundst::stop_song()
{
	if(song_channel!=NULL)
		{
		song_channel->stop();
		song_channel=NULL;
		}
}

bool musicsoundst::card_is_playing()
{
	bool is_playing=false;
	if(card_channel!=NULL)
		{
		result=card_channel->isPlaying(&is_playing);
		}
	return is_playing;
}

void musicsoundst::stop_card()
{
	if(card_channel!=NULL)
		{
		card_channel->stop();
		card_channel=NULL;
		}
}

bool musicsoundst::active_ambience_is_playing()
{
	bool is_playing=false;
	if(ambience_channel[ambience_active_channel]!=NULL)
		{
		result=ambience_channel[ambience_active_channel]->isPlaying(&is_playing);
		}
	return is_playing;
}

void musicsoundst::stop_active_ambience()
{
	if(ambience_channel[ambience_active_channel]!=NULL)
		{
		ambience_channel[ambience_active_channel]->stop();
		ambience_channel[ambience_active_channel]=NULL;
		}
}

bool musicsoundst::fading_ambience_is_playing()
{
	bool is_playing=false;
	if(ambience_channel[1-ambience_active_channel]!=NULL)
		{
		result=ambience_channel[1-ambience_active_channel]->isPlaying(&is_playing);
		}
	return is_playing;
}

void musicsoundst::stop_fading_ambience()
{
	if(ambience_channel[1-ambience_active_channel]!=NULL)
		{
		ambience_channel[1-ambience_active_channel]->stop();
		ambience_channel[1-ambience_active_channel]=NULL;
		}
}

float musicsoundst::get_song_volume()
{
	float volume=0.0f;
	if(song_channel!=NULL)
		{
		song_channel->getVolume(&volume);
		}
	return volume;
}

float musicsoundst::get_card_volume()
{
	float volume=0.0f;
	if(card_channel!=NULL)
		{
		card_channel->getVolume(&volume);
		}
	return volume;
}

float musicsoundst::get_active_ambience_volume()
{
	float volume=0.0f;
	if(ambience_channel[ambience_active_channel]!=NULL)
		{
		ambience_channel[ambience_active_channel]->getVolume(&volume);
		}
	return volume;
}

float musicsoundst::get_fading_ambience_volume()
{
	float volume=0.0f;
	if(ambience_channel[1-ambience_active_channel]!=NULL)
		{
		ambience_channel[1-ambience_active_channel]->getVolume(&volume);
		}
	return volume;
}

void musicsoundst::set_song_volume(float vol)
{
	if(song_channel!=NULL)
		{
		song_channel->setVolume(vol);
		}
}

void musicsoundst::set_card_volume(float vol)
{
	if(card_channel!=NULL)
		{
		card_channel->setVolume(vol);
		}
}

void musicsoundst::set_active_ambience_volume(float vol)
{
	if(ambience_channel[ambience_active_channel]!=NULL)
		{
		ambience_channel[ambience_active_channel]->setVolume(vol);
		}
}

void musicsoundst::set_fading_ambience_volume(float vol)
{
	if(ambience_channel[1-ambience_active_channel]!=NULL)
		{
		ambience_channel[1-ambience_active_channel]->setVolume(vol);
		}
}

void musicsoundst::update()
{
	if(!on)return;
	if(system==NULL)return;

	system->update();
}

#endif