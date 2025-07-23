//copyright (c) 2006 by tarn adams

// yeah sorry i just like the ergonomics
#include <array>

#include <string>

#include <cstddef>

#include "svector.h"

#include <unordered_map>

#include <future>

#include <variant>

#include "../platform.h"

#ifndef CUSTOM_SOUND_PLUGINS
#include "../../fmod/inc/fmod.hpp"
#include "../../fmod/inc/fmod_common.h"
#endif

#define SOUND_CHANNELNUM 32



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

using std::string;

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
	MUSIC_EVENT_FORT_LEVEL,
	MUSIC_EVENT_FIRST_GHOST,
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
	MUSIC_CONTEXT_EXCLUSIVE_MAIN,
	MUSIC_CONTEXT_EXCLUSIVE_TITLE,
	MUSIC_CONTEXTNUM,
	MUSIC_CONTEXT_NONE=-1
};
typedef int32_t MusicContext;

enum Song
	{
	SONG_DWARF_FORTRESS,
	SONG_KOGANUSAN,
	SONG_EXPANSIVE_CAVERN,
	SONG_EXPANSIVE_CAVERN_CARD_1,
	SONG_EXPANSIVE_CAVERN_CARD_2,
	SONG_EXPANSIVE_CAVERN_CARD_3,
	SONG_EXPANSIVE_CAVERN_CARD_4,
	SONG_DEATH_SPIRAL,
	SONG_HILL_DWARF,
	SONG_HILL_DWARF_CARD_1,
	SONG_HILL_DWARF_CARD_2,
	SONG_HILL_DWARF_CARD_3,
	SONG_HILL_DWARF_CARD_4,
	SONG_HILL_DWARF_CARD_5,
	SONG_FORGOTTEN_BEAST,
	SONG_FORGOTTEN_BEAST_CARD_1,
	SONG_FORGOTTEN_BEAST_CARD_2,
	SONG_FORGOTTEN_BEAST_CARD_3,
	SONG_FORGOTTEN_BEAST_CARD_4,
	SONG_FORGOTTEN_BEAST_CARD_5,
	SONG_DRINK_AND_INDUSTRY,
	SONG_VILE_FORCE_OF_DARKNESS,
	SONG_VILE_FORCE_OF_DARKNESS_CARD_1,
	SONG_VILE_FORCE_OF_DARKNESS_CARD_2,
	SONG_VILE_FORCE_OF_DARKNESS_CARD_3,
	SONG_VILE_FORCE_OF_DARKNESS_CARD_4,
	SONG_VILE_FORCE_OF_DARKNESS_CARD_5,
	SONG_FIRST_YEAR,
	SONG_FIRST_YEAR_CARD_1,
	SONG_FIRST_YEAR_CARD_2,
	SONG_FIRST_YEAR_CARD_3,
	SONG_FIRST_YEAR_CARD_4,
	SONG_ANOTHER_YEAR,
	SONG_ANOTHER_YEAR_CARD_1,
	SONG_ANOTHER_YEAR_CARD_2,
	SONG_ANOTHER_YEAR_CARD_3,
	SONG_ANOTHER_YEAR_CARD_4,
	SONG_ANOTHER_YEAR_CARD_5,
	SONG_STRIKE_THE_EARTH,
	SONG_STRIKE_THE_EARTH_CARD_1,
	SONG_STRIKE_THE_EARTH_CARD_2,
	SONG_STRIKE_THE_EARTH_CARD_3,
	SONG_STRIKE_THE_EARTH_CARD_4,
	SONG_STRANGE_MOODS,
	SONG_WINTER_ENTOMBS_YOU,
	SONG_WINTER_ENTOMBS_YOU_CARD_1,
	SONG_WINTER_ENTOMBS_YOU_CARD_2,
	SONG_WINTER_ENTOMBS_YOU_CARD_3,
	SONG_WINTER_ENTOMBS_YOU_CARD_4,
	SONG_CRAFTSDWARFSHIP,
	SONG_CRAFTSDWARFSHIP_CARD_1,
	SONG_CRAFTSDWARFSHIP_CARD_2,
	SONG_CRAFTSDWARFSHIP_CARD_3,
	SONG_CRAFTSDWARFSHIP_CARD_4,
	SONG_CRAFTSDWARFSHIP_CARD_5,
	SONG_MOUNTAINHOME,
	SONG_MOUNTAINHOME_CARD_1,
	SONG_MOUNTAINHOME_CARD_2,
	SONG_MOUNTAINHOME_CARD_3,
	SONG_MOUNTAINHOME_CARD_4,
	SONG_MOUNTAINHOME_CARD_5,
	SONG_NEUTRAL_CARD_1,
	SONG_NEUTRAL_CARD_2,
	SONG_NEUTRAL_CARD_3,
	SONG_NEUTRAL_CARD_4,
	SONG_NEUTRAL_CARD_5,
	SONG_NEUTRAL_CARD_6,
	SONG_NEUTRAL_CARD_7,
	SONG_NEUTRAL_CARD_8,
	SONG_NEUTRAL_CARD_9,
	SONG_NEUTRAL_CARD_10,
	SONG_NEUTRAL_CARD_11,
	SONG_NEUTRAL_CARD_12,
	SONG_NEUTRAL_CARD_13,
	SONG_NEUTRAL_CARD_14,
	SONG_NEUTRAL_CARD_15,
	SONG_NEUTRAL_CARD_16,
	SONG_AMBIENCE_BLIZZARD,
	SONG_AMBIENCE_CAVERN,
	SONG_AMBIENCE_COMBAT,
	SONG_AMBIENCE_DESERT,
	SONG_AMBIENCE_EVIL,
	SONG_AMBIENCE_FOREST,
	SONG_AMBIENCE_GLACIER,
	SONG_AMBIENCE_GOOD,
	SONG_AMBIENCE_GRASSLANDS,
	SONG_AMBIENCE_MAGMA_CLOSE,
	SONG_AMBIENCE_MAGMA_FAR,
	SONG_AMBIENCE_MAGMA_LOW,
	SONG_AMBIENCE_RAINFOREST,
	SONG_AMBIENCE_RIVER_HIGH,
	SONG_AMBIENCE_RIVER_LOW,
	SONG_AMBIENCE_RIVER_MEDIUM,
	SONG_AMBIENCE_SIEGE,
	SONG_AMBIENCE_SWAMP,
	SONG_AMBIENCE_TAVERN,
	SONG_AMBIENCE_THUNDERSTORM,
	SONG_AMBIENCE_TRADE_DEPOT,
	SONG_AMBIENCE_WORKSHOP,
	SONG_AMBIENCE_OUTSIDE,
	SONG_AMBIENCE_TERRIFYING,
	SONG_CLASSIC_TITLE_THEME,
	SONG_CLASSIC_MAIN_THEME,
	SONG_AMBIENCE_NEUTRAL_WINDS,
	SONG_AMBIENCE_NEUTRAL_WINDS_2,
	SONG_AMBIENCE_NEUTRAL_CAVERN,
	SONG_STRIKE_IT_AGAIN,
	SONG_THE_CAPITAL,
	SONG_BRUISING_THE_FAT,
	SONG_LIFE_OF_ADVENTURE,
	SONG_BEYOND_THE_GATES,
	SONG_HAMLET_MEMOIRS,
	SONG_NIGHT_CREATURE,
	SONG_INTO_THE_PITS,
	SONG_SISTERS_OF_WAR,
	SONG_CANNIBAL,
	SONG_PONY_RIDER,
	SONG_TWO_COINS,
	SONG_LOSING_IS_FUN,
	SONGNUM,
	SONG_NONE=-1
	};

enum SoundType
	{
	SOUND_ADAMANTINE,
	SOUND_AMBUSH,
	SOUND_ARTIFACT_CREATED,
	SOUND_BABY_BORN,
	SOUND_CAVERN_BREAK,
	SOUND_DEMON_ATTACK,
	SOUND_MEGABEAST,
	SOUND_SIEGE,
	SOUND_STRANGE_MOOD,
	SOUND_WEDDING,
	SOUND_ALERT,
	SOUND_HOWL_1,
	SOUND_HOWL_2,
	SOUND_HOWL_3,
	SOUND_HOWL_4,
	SOUND_HOWL_5,
	SOUND_GIANT_STEP_1,
	SOUND_GIANT_STEP_2,
	SOUND_GIANT_STEP_3,
	SOUNDNUM,
	SOUND_NONE=-1
	};
typedef int32_t Sound;

// human hearing is exponential, so this is too. 25 db range from 1 to 255, so ~0.1 decibels per

/*
* Generated using the following D program (don't judge me, it's what I use instead of python)
* 	import std.math : pow;
	immutable real c=2.5;
	immutable real a=pow(10,-c);
	immutable real b=c/255.0;
	foreach(i; 0..256)
	{
		real x = a*pow(10,(i*b));
		float acc = x;
		write(acc,"f,");
		if(i%4==3) write("\n");
	}
*/

constexpr static float volume_lookup[256]={
		0.0f,0.00323448f,0.00330832f,0.00338386f,
		0.00346111f,0.00354013f,0.00362096f,0.00370363f,
		0.00378819f,0.00387468f,0.00396314f,0.00405362f,
		0.00414617f,0.00424083f,0.00433765f,0.00443669f,
		0.00453798f,0.00464159f,0.00474756f,0.00485595f,
		0.00496682f,0.00508022f,0.0051962f,0.00531484f,
		0.00543618f,0.0055603f,0.00568725f,0.00581709f,
		0.0059499f,0.00608574f,0.00622469f,0.00636681f,
		0.00651217f,0.00666085f,0.00681292f,0.00696847f,
		0.00712756f,0.00729029f,0.00745674f,0.00762699f,
		0.00780112f,0.00797923f,0.0081614f,0.00834773f,
		0.00853832f,0.00873326f,0.00893265f,0.00913659f,
		0.00934519f,0.00955855f,0.00977679f,0.01f,
		0.0102283f,0.0104618f,0.0107007f,0.010945f,
		0.0111949f,0.0114505f,0.0117119f,0.0119793f,
		0.0122528f,0.0125325f,0.0128187f,0.0131113f,
		0.0134107f,0.0137169f,0.01403f,0.0143504f,
		0.014678f,0.0150131f,0.0153559f,0.0157065f,
		0.0160651f,0.0164318f,0.016807f,0.0171907f,
		0.0175832f,0.0179846f,0.0183953f,0.0188152f,
		0.0192448f,0.0196842f,0.0201336f,0.0205933f,
		0.0210634f,0.0215443f,0.0220362f,0.0225393f,
		0.0230539f,0.0235803f,0.0241186f,0.0246693f,
		0.0252325f,0.0258086f,0.0263979f,0.0270005f,
		0.027617f,0.0282475f,0.0288924f,0.0295521f,
		0.0302268f,0.0309169f,0.0316228f,0.0323448f,
		0.0330832f,0.0338386f,0.0346111f,0.0354013f,
		0.0362096f,0.0370363f,0.0378819f,0.0387468f,
		0.0396314f,0.0405362f,0.0414617f,0.0424083f,
		0.0433765f,0.0443669f,0.0453798f,0.0464159f,
		0.0474756f,0.0485595f,0.0496682f,0.0508022f,
		0.0519621f,0.0531484f,0.0543618f,0.055603f,
		0.0568725f,0.0581709f,0.059499f,0.0608574f,
		0.0622469f,0.063668f,0.0651217f,0.0666085f,
		0.0681292f,0.0696847f,0.0712757f,0.0729029f,
		0.0745674f,0.0762699f,0.0780112f,0.0797923f,
		0.081614f,0.0834773f,0.0853832f,0.0873326f,
		0.0893265f,0.0913659f,0.0934519f,0.0955855f,
		0.0977679f,0.1f,0.102283f,0.104618f,
		0.107007f,0.10945f,0.111949f,0.114505f,
		0.117119f,0.119793f,0.122528f,0.125325f,
		0.128187f,0.131113f,0.134107f,0.137169f,
		0.1403f,0.143504f,0.14678f,0.150131f,
		0.153559f,0.157065f,0.160651f,0.164318f,
		0.16807f,0.171907f,0.175832f,0.179846f,
		0.183953f,0.188152f,0.192448f,0.196842f,
		0.201336f,0.205933f,0.210634f,0.215443f,
		0.220362f,0.225393f,0.230539f,0.235803f,
		0.241186f,0.246693f,0.252325f,0.258086f,
		0.263979f,0.270005f,0.27617f,0.282475f,
		0.288924f,0.295521f,0.302268f,0.309169f,
		0.316228f,0.323448f,0.330832f,0.338386f,
		0.346111f,0.354013f,0.362096f,0.370363f,
		0.378819f,0.387468f,0.396314f,0.405362f,
		0.414617f,0.424083f,0.433765f,0.443669f,
		0.453798f,0.464159f,0.474756f,0.485595f,
		0.496682f,0.508022f,0.51962f,0.531484f,
		0.543618f,0.55603f,0.568725f,0.581709f,
		0.59499f,0.608574f,0.622469f,0.636681f,
		0.651217f,0.666085f,0.681292f,0.696847f,
		0.712756f,0.729029f,0.745674f,0.762699f,
		0.780112f,0.797923f,0.81614f,0.834773f,
		0.853832f,0.873326f,0.893265f,0.913659f,
		0.934519f,0.955855f,0.977679f,1.0f
	};

class musicsound_info // composition makes it at least a little easier to just "plug in" new systems here
{
#ifdef CUSTOM_SOUND_PLUGINS
private:
	bool (*is_ambience_playing_ptr)(int32_t);
	bool (*is_card_playing_ptr)();
	bool (*is_song_playing_ptr)();
	void (*stop_ambience_ptr)(int32_t);
	void (*stop_card_ptr)();
	void (*stop_song_ptr)();
	void (*start_card_ptr)(int32_t);
	void (*start_song_ptr)(int32_t);
	void (*play_ambience_ptr)(int32_t, int32_t);
	void (*play_sound_ptr)(int, int32_t, bool);
	bool (*set_song_ptr)(const std::string&, int, bool);
	bool (*set_sound_ptr)(const std::string&, int);
	void (*set_init_volumes_ptr)(int, int, int, int);
	void (*set_song_volume_ptr)(float);
	void (*set_card_volume_ptr)(float);
	void (*set_ambience_volume_ptr)(int32_t, float);
	float (*get_song_volume_ptr)();
	float (*get_card_volume_ptr)();
	float (*get_ambience_volume_ptr)(int);
	bool (*init_sound_ptr)();
	void (*deinit_sound_ptr)();
	void (*update_ptr)();
public:
	bool is_ambience_playing(int32_t ambience_active_channel) { return is_ambience_playing_ptr(ambience_active_channel); }
	bool is_card_playing() { return is_card_playing_ptr(); }
	bool is_song_playing() { return is_song_playing_ptr(); }
	void stop_ambience(int32_t ambience_active_channel) { stop_ambience_ptr(ambience_active_channel); }
	void stop_card() { stop_card_ptr(); }
	void stop_song() { stop_song_ptr(); }
	void start_card(int32_t new_song) { start_card_ptr(new_song); }
	void start_song(int32_t new_song) { start_song_ptr(new_song); }
	void play_ambience(int32_t new_ambience, int32_t ambience_active_channel) { play_ambience_ptr(new_ambience, ambience_active_channel); }
	void play_sound(int s, int32_t vol, bool use_media_sound_volume) { play_sound_ptr(s, vol, use_media_sound_volume); }
	bool set_song(const std::string& filename, int slot, bool loops = false) { return set_song_ptr(filename, slot, loops); }
	bool set_sound(const std::string& filename, int slot) { return set_sound_ptr(filename, slot); }
	void set_init_volumes(int master_volume, int sfx_volume, int ambience_volume, int music_volume) { set_init_volumes_ptr(master_volume, sfx_volume, ambience_volume, music_volume); }
	void set_song_volume(float vol) { set_song_volume_ptr(vol); }
	void set_card_volume(float vol) { set_card_volume_ptr(vol); }
	void set_ambience_volume(int32_t chan, float vol) { set_ambience_volume_ptr(chan, vol); }
	float get_song_volume() { return get_song_volume_ptr(); }
	float get_card_volume() { return get_card_volume_ptr(); }
	float get_ambience_volume(int chan) { return get_ambience_volume_ptr(chan); }
	bool init_sound() { return init_sound_ptr(); }
	void deinit_sound() { deinit_sound_ptr(); }
	void update() { update_ptr(); }
	musicsound_info(void* handle);
#else
private:
	FMOD::System* fmod_system;

	std::array<FMOD::Channel*, 10> sound_channel;
	FMOD::Channel* song_channel;
	FMOD::Channel* ambience_channel[2];
	FMOD::Channel* card_channel;

	std::array<FMOD::Sound*, MAXSONGNUM> mod;
	std::array<FMOD::Sound*, MAXSOUNDNUM> samp;
	std::array<FMOD::Sound*, MAXSOUNDNUM> stream;

	FMOD_RESULT result;
public:
	bool is_ambience_playing(int32_t ambience_active_channel);
	bool is_card_playing();
	bool is_song_playing();
	void stop_ambience(int32_t ambience_active_channel);
	void stop_card();
	void stop_song();
	void start_card(int32_t new_song);
	void start_song(int32_t new_song);
	void play_ambience(int32_t new_ambience, int32_t ambience_active_channel);
	void play_sound(int s, int32_t vol, bool use_media_sound_volume);
	bool set_song(const std::string& filename, int slot, bool loops = false);
	bool set_sound(const std::string& filename, int slot);
	void set_init_volumes(int master_volume, int sfx_volume, int ambience_volume, int music_volume);
	void set_song_volume(float vol);
	void set_card_volume(float vol);
	void set_ambience_volume(int32_t chan, float vol);
	float get_song_volume();
	float get_card_volume();
	float get_ambience_volume(int chan);
	bool init_sound();
	void deinit_sound();
	void update();
	musicsound_info();
#endif
};

struct music_datast
{
	int id;
	std::string title;
	std::string author;
};

struct loading_music_filest
{
	std::string token;
	std::variant<music_datast,int> info;
};

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
		std::array<int32_t, 20> ambience_combat_strikes;

		int queued_song;
		DWORD queued_song_count;

		int planned_song;
		svector<int> planned_cards;
		svector<int> neutral_card_queue;

		uint32_t flag;
		MusicEvent latest_event;

		std::array<int32_t, MAXSONGNUM> total_plays;
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
			std::array<int32_t,MAXSONGNUM> ambience_mode;
			std::array<DWORD,MAXSONGNUM> ambience_mode_start;
			DWORD ambience_last_sound;
		// custom music info
		int32_t next_song_id;
		int32_t next_sound_id;
		std::unordered_map<std::string,music_datast> loaded_music;
		std::unordered_map<int,music_datast> music_by_id;
		std::unordered_map<std::string,int> loaded_sounds;
		std::vector<std::future<loading_music_filest>> loading_files;
		musicsoundst()
			{

			ambience_active_channel=0;
			total_plays.fill(0); // see? ergonomics
			last_main_was_playing_count=0;
			last_any_was_playing_count=0;
			next_play_duration=0;
			last_event_count_tavern=0;
			last_event_count_death_spiral=0;
			queued_song_count=0;
			song_fade_start_count=0;
			card_fade_start_count=0;
			ambience_combat_strikes.fill(0);

			post_start_ambience_fade_count=0;

			ambience_vote_tavern=0;
			ambience_vote_workshop=0;

			doing_forced=0;

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
			ambience_mode.fill(0);
			ambience_mode_start.fill(0);
			ambience_last_sound=0;
			next_song_id=SONGNUM;
			next_sound_id=SOUNDNUM;
			initialize_internal_system();
			}

		~musicsoundst()
			{
			deinitsound();
			}

		void startbackgroundmusic(int new_song);
		void start_card(int new_song);
		void playsound(int s,int32_t vol=255,bool use_media_sound_volume=true);
		bool initsound();
		void deinitsound();
		bool set_song(string &filename,int slot,bool loops);
		bool set_sound(string &filename,int slot);
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
		void initialize_internal_system();

		void update();

		void finalize_loading_audio();

		music_datast get_custom_song(std::string &token);

		int get_custom_sound(std::string &token);

		void set_custom_song(std::string &token,filest &filename,string &title,string &author, bool loops);

		void set_custom_sound(std::string &token,filest &filename);

		void prepare_sounds(const std::filesystem::path &src_dir);

	private:

		musicsound_info *internal;

		int song;
		char musicactive;
		char soundpriority;
		int soundplaying;

		char doing_forced;
		uint32_t forcesongtime;
		uint32_t forcesongstart;

		char on;
};

struct adv_music_statest
{
	bool have_done_chargen_music;
	bool have_done_start_music;
	bool have_played_initial_trade_music;
	bool have_played_initial_civilization_music;
	bool have_played_initial_cannibal_music;
	bool have_played_first_quest_music;
	bool have_played_first_dangerous_music;
	bool have_played_death_music;
	bool have_played_initial_mounted_travel_music;
	bool have_played_tavern_music;
	
	DWORD adv_last_music_play_time;
	bool playing_combat_music;
		bool playing_combat_music_doing_layers_and_groups;
		DWORD adv_combat_music_start_time;
			DWORD adv_combat_music_time_started_last_layer;
			DWORD adv_combat_music_last_regular_time;
		int32_t adv_combat_music_group;
		int32_t adv_combat_music_layer;
	DWORD adv_start_music_time;



	adv_music_statest()
		{
		init();
		}
	void init()
		{
		set_all_played_to_false();

		playing_combat_music=false;
		playing_combat_music_doing_layers_and_groups=false;
		adv_combat_music_start_time=-1;
		adv_combat_music_group=0;
		adv_combat_music_layer=0;
		adv_combat_music_time_started_last_layer=-1;
		adv_combat_music_last_regular_time=-1;

		adv_last_music_play_time=-1;
		adv_start_music_time=-1;
		}
	void set_all_played_to_false()
		{
		have_done_chargen_music=false;
		have_done_start_music=false;
		have_played_initial_trade_music=false;
		have_played_initial_civilization_music=false;
		have_played_initial_cannibal_music=false;
		have_played_first_quest_music=false;
		have_played_first_dangerous_music=false;
		have_played_death_music=false;
		have_played_initial_mounted_travel_music=false;
		have_played_tavern_music=false;
		}

	void play_generic_song();
	void play_dangerous_song(bool is_terrifying_area);
};
#endif