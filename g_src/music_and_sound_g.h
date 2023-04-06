#ifdef WIN32
//copyright (c) 2006 by tarn adams

#define SOUND_CHANNELNUM 32

#ifndef NO_FMOD

#include "../../fmod/inc/fmod.hpp"
#include "../../fmod/inc/fmod_common.h"

#define MAXSONGNUM 1000
#define MAXSOUNDNUM 1000

#ifndef MUSICSOUND
#define MUSICSOUND

#define MUSICSOUND_FLAG_JUST_ADDED_EVENT BIT1
#define MUSICSOUND_FLAG_WAS_PLAYING BIT2
#define MUSICSOUND_FLAG_FADE_SONG_OUT BIT3
#define MUSICSOUND_FLAG_TITLE_AMBIENCE_FADE_IN BIT4
#define MUSICSOUND_FLAG_NO_MUSIC_START_WHILE_MEGABOX_OPEN BIT5
#define MUSICSOUND_FLAG_POST_START_AMBIENCE_FADE BIT6
#define MUSICSOUND_FLAG_FADE_CARD_OUT BIT7

enum MusicEventType
{
	MUSIC_EVENT_FIRST_CAVERN_OPENED,
	MUSIC_EVENT_DEATH_SPIRAL,
	MUSIC_EVENT_MEGABEAST_ATTACK,
	MUSIC_EVENT_FORGOTTEN_BEAST_ATTACK,
	MUSIC_EVENT_TAVERN_MUSIC_PRESENT,
	MUSIC_EVENT_TAVERN_DANCE_PRESENT,
	MUSIC_EVENT_SIEGE,
	MUSIC_EVENT_JUST_EMBARKED,
	MUSIC_EVENT_LOST_FORT,
	MUSIC_EVENTNUM,
	MUSIC_EVENT_NONE=-1
};
typedef int32_t MusicEvent;

enum MusicContextType
{
	MUSIC_CONTEXT_ANY,
	MUSIC_CONTEXT_MAIN,
	MUSIC_CONTEXT_CAVERNS_OPENED,
	MUSIC_CONTEXT_FIRST_YEAR,
	MUSIC_CONTEXT_SECOND_YEAR_PLUS,
	MUSIC_CONTEXT_SPRING,
	MUSIC_CONTEXT_SUMMER,
	MUSIC_CONTEXT_AUTUMN,
	MUSIC_CONTEXT_WINTER,
	MUSIC_CONTEXTNUM,
	MUSIC_CONTEXT_NONE=-1
};
typedef int32_t MusicContext;

class musicsoundst
{
	public:
		int SoftChannelNumber;

		DWORD last_main_was_playing_count;
		DWORD last_any_was_playing_count;
		DWORD next_play_duration;
		DWORD song_started_count;
		DWORD last_event_count_tavern;
		DWORD last_event_count_death_spiral;
		DWORD song_fade_start_count;
		DWORD card_fade_start_count;
		DWORD title_ambience_fade_in_count;
		DWORD post_start_ambience_fade_count;

		int card_playing;
		DWORD card_started_count;

		int32_t ambience_vote_tavern;
		int32_t ambience_vote_workshop;
		int32_t ambience_vote_performance;
		int32_t ambience_combat_strikes[20];

		int queued_song;
		DWORD queued_song_count;

		int planned_song;
		svector<int> planned_cards;
		svector<int> neutral_card_queue;

		uint32_t flag;
		MusicEvent latest_event;

		int32_t total_plays[MAXSONGNUM];
		int32_t play_history[3];
		int32_t neutral_card_history[3];

		int32_t ambience_active;
			int32_t ambience_fading;
			int32_t ambience_active_channel;
			bool ambience_crossfading;
			DWORD ambience_crossfade_start;
			int32_t ambience_siege_enid;
				DWORD ambience_siege_start;
				bool ambience_siege_end_played;
			int32_t ambience_mode[MAXSONGNUM];
			DWORD ambience_mode_start[MAXSONGNUM];
			DWORD ambience_last_sound;


		musicsoundst()
			{
			int32_t s;

			system=NULL;

			for(s=0;s<10;++s)sound_channel[s]=NULL;
			song_channel=NULL;
			ambience_channel[0]=NULL;
			ambience_channel[1]=NULL;
			card_channel=NULL;

			ambience_active_channel=0;

			for(s=0;s<MAXSOUNDNUM;++s)samp[s]=NULL;
			for(s=0;s<MAXSONGNUM;++s)
				{
				mod[s]=NULL;
				stream[s]=NULL;
				}

			last_main_was_playing_count=0;
			last_any_was_playing_count=0;
			next_play_duration=0;
			last_event_count_tavern=0;
			last_event_count_death_spiral=0;
			queued_song_count=0;
			song_fade_start_count=0;
			card_fade_start_count=0;
			for(s=0;s<20;++s)ambience_combat_strikes[s]=0;

			post_start_ambience_fade_count=0;

			ambience_vote_tavern=0;
			ambience_vote_workshop=0;

			doing_forced=0;
			for(s=0;s<MAXSONGNUM;s++)
				{
				mod[s]=NULL;
				stream[s]=NULL;
				total_plays[s]=0;
				}
			for(s=0;s<MAXSOUNDNUM;s++)samp[s]=NULL;

			flag=0;
			latest_event=MUSIC_EVENT_NONE;

			song=-1;
			queued_song=-1;
			play_history[0]=-1;
			play_history[1]=-1;
			play_history[2]=-1;

			card_playing=-1;
			card_started_count=0;
			neutral_card_history[0]=-1;
			neutral_card_history[1]=-1;
			neutral_card_history[2]=-1;

			planned_song=-1;

			ambience_active=-1;
			ambience_fading=-1;
			ambience_crossfading=0;
			ambience_siege_enid=-1;
				ambience_siege_start=0;
 				ambience_siege_end_played=false;
			memset(ambience_mode,0,sizeof(int32_t)*MAXSONGNUM);
			memset(ambience_mode_start,0,sizeof(DWORD)*MAXSONGNUM);
			ambience_last_sound=0;
			}

		~musicsoundst()
			{
			deinitsound();
			}

		void startbackgroundmusic(int new_song);
		void start_card(int new_song);
		void playsound(int s,int32_t vol=255,bool use_media_sound_volume=true);
		void initsound();
		void deinitsound();
		void set_song(string &filename,int slot,bool loops);
		void set_sound(string &filename,int slot);
		void set_master_volume(int32_t newvol);
		void play_ambience(int32_t new_ambience);
		int32_t get_song_playing();
		int32_t get_card_playing();

		bool song_is_playing();
		void stop_song();
		bool card_is_playing();
		void stop_card();
		bool active_ambience_is_playing();
		void stop_active_ambience();
		bool fading_ambience_is_playing();
		void stop_fading_ambience();

		float get_song_volume();
		float get_card_volume();
		float get_active_ambience_volume();
		float get_fading_ambience_volume();
		void set_song_volume(float vol);
		void set_card_volume(float vol);
		void set_active_ambience_volume(float vol);
		void set_fading_ambience_volume(float vol);

		void update();

	private:
		FMOD::System *system;

	    FMOD::Channel *sound_channel[10];
	    FMOD::Channel *song_channel;
	    FMOD::Channel *ambience_channel[2];
	    FMOD::Channel *card_channel;

		FMOD_RESULT result;

		int song;
		char musicactive;
		char soundpriority;
		int soundplaying;

		char doing_forced;
		uint32_t forcesongtime;
		uint32_t forcesongstart;

		char on;

		FMOD::Sound *mod[MAXSONGNUM];
		FMOD::Sound *samp[MAXSOUNDNUM];
		FMOD::Sound *stream[MAXSONGNUM];
};
#endif
#endif
#else
#ifndef MUSIC_AND_SOUND_G
#define MUSIC_AND_SOUND_G

// Fmodex works well enough on windows/os x, but on some linux distributions it fails badly
#if defined(linux)
#include "music_and_sound_openal.h"
#else
#include "music_and_sound_fmodex.h"
#endif // unix

extern musicsoundst musicsound;

#endif
#endif