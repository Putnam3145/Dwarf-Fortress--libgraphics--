//copyright (c) 2006 by tarn adams

#ifdef WIN32
#include <windows.h>
#endif
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

#include "../util/svector.hpp"

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

#include "../util/random.hpp"

using std::string;

#include "../util/endian.hpp"

#include "../files/files.hpp"

#include "../util/basics.hpp"

#include "../render/enabler.hpp"

#include "../render/init.hpp"

#include "../audio/music_and_sound_g.hpp"
#include "../audio/music_and_sound_v.hpp"

//MUSIC AND SOUND FUNCTIONS

#ifdef DEBUGOPT
  #define MUSIC_DEBUG
#endif

//MUSIC
void musicsoundst::play_ambience(int32_t new_ambience)
{
  if(!on)return;

  bool active_is_playing=internal->is_ambience_playing(ambience_active_channel);

  if((new_ambience!=ambience_active||
    !active_is_playing)&&
    !ambience_crossfading)
  {
    internal->set_init_volumes(init.media.volume_master, init.media.volume_sfx, init.media.volume_ambience, init.media.volume_music);
    if(active_is_playing)
    {
      internal->stop_ambience(1-ambience_active_channel);

      ambience_fading=ambience_active;
      ambience_active=new_ambience;

      ambience_active_channel=1-ambience_active_channel;
      ambience_crossfading=true;
      ambience_crossfade_start=GetTickCount();
    }
    else
    {
      internal->stop_ambience(ambience_active_channel);
      internal->stop_ambience(1-ambience_active_channel);
      ambience_active=new_ambience;
    }

    internal->play_ambience(new_ambience,ambience_active_channel);

    ++total_plays[new_ambience];
  }
}

void musicsoundst::start_card(int new_song)
{
  if(!on)return;

  bool is_playing=internal->is_card_playing();

  if(!is_playing)
  {
    internal->set_init_volumes(init.media.volume_master, init.media.volume_sfx, init.media.volume_ambience, init.media.volume_music);
    internal->start_card(new_song);

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

  if(internal->is_song_playing())
  {
    return song;
  }
  else return -1;
}

int32_t musicsoundst::get_card_playing()
{
  if(!on)return -1;

  if(internal->is_card_playing())
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

  bool is_playing=internal->is_song_playing();

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
        internal->stop_song();
      internal->set_init_volumes(init.media.volume_master, init.media.volume_sfx, init.media.volume_ambience, init.media.volume_music);
      song=new_song;
      if(queued_song==song)queued_song=-1;
      internal->start_song(new_song);

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

  internal->set_init_volumes(init.media.volume_master, init.media.volume_sfx, init.media.volume_ambience, init.media.volume_music);

  // this one's all internal stuff really
  internal->play_sound(s,vol,use_media_sound_volume);
}

bool musicsoundst::initsound()
{
  if(on)return true;

  if (!internal->init_sound()) 
  { 
    std::cout << "init_sound returned false!" << std::endl;
    return false;
  }

  on=1;

  musicactive=0;
  soundplaying=-1;
  soundpriority=0;
  song=-1;
  forcesongtime=-1;

  return true;
}

void musicsoundst::deinitsound()
{
  if(!on)return;

  internal->deinit_sound();

  on=0;
}

bool musicsoundst::set_song(string &filename,int slot,bool loops)
{
  if(!on)return false;
  return internal->set_song(filename,slot,loops);
}

bool musicsoundst::set_sound(string &filename,int slot)
{
  if(!on)return false;
  return internal->set_sound(filename,slot);
}

bool musicsoundst::song_is_playing()
{
  // the different convention was a bit of a mistake
  return internal->is_song_playing();
}

void musicsoundst::stop_song()
{
  internal->stop_song();
}

bool musicsoundst::card_is_playing()
{
  return internal->is_card_playing();
}

void musicsoundst::stop_card()
{
  internal->stop_card();
}

bool musicsoundst::active_ambience_is_playing()
{
  return internal->is_ambience_playing(ambience_active_channel);
}

void musicsoundst::stop_active_ambience()
{
  internal->stop_ambience(ambience_active_channel);
}

bool musicsoundst::fading_ambience_is_playing()
{
  return internal->is_ambience_playing(1-ambience_active_channel);
}

void musicsoundst::stop_fading_ambience()
{
  internal->stop_ambience(1-ambience_active_channel);
}

float musicsoundst::get_song_volume()
{
  return internal->get_song_volume();
}

float musicsoundst::get_card_volume()
{
  return internal->get_card_volume();
}

float musicsoundst::get_active_ambience_volume()
{
  return internal->get_ambience_volume(ambience_active_channel);
}

float musicsoundst::get_fading_ambience_volume()
{
  return internal->get_ambience_volume(1-ambience_active_channel);
}

void musicsoundst::set_song_volume(float vol)
{
  internal->set_song_volume(vol);
}

void musicsoundst::set_card_volume(float vol)
{
  internal->set_card_volume(vol);
}

void musicsoundst::set_active_ambience_volume(float vol)
{
  internal->set_ambience_volume(ambience_active_channel,vol);
}

void musicsoundst::set_fading_ambience_volume(float vol)
{
  internal->set_ambience_volume(1-ambience_active_channel,vol);
}

#ifndef WIN32
#include <dlfcn.h>
void musicsoundst::initialize_internal_system()
{
  auto handle = dlopen("./libfmod_plugin.so", RTLD_NOW);
  std::cout << "Loading audio...";
  auto err = dlerror();
  if (err) std::cout << err;
  if (handle == NULL) 
  {
    std::cout << "Failed to load fmod, trying SDL_mixer...";
    handle = dlopen("./libsdl_mixer_plugin.so", RTLD_NOW);
  }
  err = dlerror();
  if (err) std::cout << err;
  if (handle == NULL)
  {
    std::cout << "Failed to load SDL_mixer too, turning off audio";
  }
  else
  {
    std::cout << "Audio loaded successfully!";
  }
  internal = new musicsound_info(handle);
}
#else
void musicsoundst::initialize_internal_system()
{
  internal = new musicsound_info();
}
#endif


void musicsoundst::update()
{
  if(!on)return;
  internal->update();
}

void musicsoundst::finalize_loading_audio()
{
  for (auto &l:loading_files)
  {
    loading_music_filest v=l.get();
    if (!v.token.empty())
    {
      if (std::holds_alternative<int>(v.info))
      {
        int i=std::get<int>(v.info);
        if(i != -1) loaded_sounds[v.token]=i;
      }
      else
      {
        music_datast d=std::get<music_datast>(v.info);
        if (d.id!=-1)
        {
          loaded_music[v.token]=d;
          music_by_id[d.id]=d;
        }
      }
    }
  }
  loading_files.clear();
}

music_datast musicsoundst::get_custom_song(string &token)
{
  finalize_loading_audio();
  if (auto it=loaded_music.find(token); it!=loaded_music.end())
  {
    return it->second;
  }
  else
  {
    auto r=music_datast{ -1,"","" };
    return r;
  }
}

int musicsoundst::get_custom_sound(string &token)
{
  finalize_loading_audio();
  if (auto it=loaded_sounds.find(token); it!=loaded_sounds.end())
  {
    return it->second;
  }
  else
  {
    return -1;
  }

}

void errorlog_string(const string &str);

void musicsoundst::set_custom_song(string &token,string &file,string &title,string &author, bool loops)
{
  if (loaded_music.count(token)) return;

  int id=next_song_id;
  next_song_id++;

  loading_files.push_back(std::async([this, token, file, title, author, id, loops]() mutable -> loading_music_filest
  {
    if (set_song(file,id,loops))
    {
      return loading_music_filest{ token, music_datast{id, title, author} };
    }
    else
    {
      errorlog_string("Could not load music file "+file);
      return loading_music_filest{ token, music_datast{-1, title, author} };
    }
  }));
}

void musicsoundst::set_custom_sound(string &token,string &file)
{
  if (loaded_sounds.count(token)) return;

  int id=next_sound_id;
  next_sound_id++;

  loading_files.push_back(std::async([this, token,file, id]() mutable -> loading_music_filest
  {
    if (set_sound(file,id))
    {
      return loading_music_filest{ token, id };
    }
    else
    {
      errorlog_string("Could not load sound file "+file);
      return loading_music_filest{ token, -1 };
    }
  }));
}


#include "../text/textlines.hpp"

#include "../files/find_files.hpp"

extern thread_local string errorlog_prefix;

struct sound_file_infost
{
  std::string token;
  std::string filename;
  std::string author;
  std::string title;
  bool loops;
};

void musicsoundst::prepare_sounds(const string &src_dir)
{
  svector<char *> processfilename;
  long f;
  textlinesst lines;
  char str[400];

  {
    string chk=src_dir;
    chk+="sound/music_file_*";
#ifdef WIN32
    chk+=".*";
#endif
    find_files_by_pattern_with_exception(chk.c_str(),processfilename,"text");

    string chktype="MUSIC_FILE";
    for (f=0; f<processfilename.size(); f++)
    {
      strcpy(str,src_dir.c_str());
      strcat(str,"sound/");
      strcat(str,processfilename[f]);
      lines.load_raw_to_lines(str);

      std::vector<sound_file_infost> files;

      errorlog_prefix="*** Error(s) found in the file \"";
      errorlog_prefix+=str;
      errorlog_prefix+='\"';

      for (int t=1; t<lines.text.str.size(); t++)
      {
        string &curstr=lines.text.str[t]->dat;

        for (int pos=0; pos<curstr.length(); pos++)
        {
          if (curstr[pos]=='[')
          {
            string token;
            if (!grab_token_string(token,curstr,pos))continue;
            if (token=="MUSIC_FILE")
            {
              if (!grab_token_string(token,curstr,pos))continue;
              files.emplace_back();
              files.back().token=token;
            }
            if (!files.empty())
            {
              if (token=="FILE")
              {
                if (!grab_token_string(token,curstr,pos))continue;
                files.back().filename=src_dir + "sound/"+token;
              }
              if (token=="LOOPS")
              {
                files.back().loops=true;
              }
              if (token=="AUTHOR")
              {
                if (!grab_token_string(token,curstr,pos))continue;
                files.back().author=token;
              }
              if (token=="TITLE")
              {
                if (!grab_token_string(token,curstr,pos))continue;
                files.back().title=token;
              }
            }
          }
        }
      }
      errorlog_prefix.clear();
      for (auto &mus:files)
      {
        set_custom_song(mus.token,mus.filename,mus.title, mus.author, mus.loops);
      }
      delete[] processfilename[f];
    }
    processfilename.clear();
  }
  {
    string chk=src_dir;
    chk+="sound/sound_file_*";
#ifdef WIN32
    chk+=".*";
#endif
    find_files_by_pattern_with_exception(chk.c_str(),processfilename,"text");

    string chktype="SOUND_FILE";
    for (f=0; f<processfilename.size(); f++)
    {
      strcpy(str,src_dir.c_str());
      strcat(str,"sound/");
      strcat(str,processfilename[f]);
      lines.load_raw_to_lines(str);

      std::vector<sound_file_infost> files;

      errorlog_prefix="*** Error(s) found in the file \"";
      errorlog_prefix+=str;
      errorlog_prefix+='\"';

      for (int t=1; t<lines.text.str.size(); t++)
      {
        string &curstr=lines.text.str[t]->dat;

        for (int pos=0; pos<curstr.length(); pos++)
        {
          if (curstr[pos]=='[')
          {
            string token;
            if (!grab_token_string(token,curstr,pos))continue;
            if (token=="SOUND_FILE")
            {
              if (!grab_token_string(token,curstr,pos))continue;
              files.emplace_back();
              files.back().token=token;
              files.back().loops=false;
            }
            if (!files.empty())
            {
              if (token=="FILE")
              {
                if (!grab_token_string(token,curstr,pos))continue;
                files.back().filename=src_dir+"sound/"+token;
              }
            }
          }
        }
      }
      errorlog_prefix.clear();
      for (auto &snd:files)
      {
        set_custom_sound(snd.token,snd.filename);
      }
      delete[] processfilename[f];
    }
    processfilename.clear();
  }

}
#ifndef WIN32
musicsound_info::musicsound_info(void* handle)
{
  if (handle != NULL)
  {
    is_ambience_playing_ptr = (bool(*)(int32_t))(dlsym(handle, "is_ambience_playing"));
    is_card_playing_ptr = (bool(*)())(dlsym(handle, "is_card_playing"));
    is_song_playing_ptr = (bool(*)())(dlsym(handle, "is_song_playing"));
    stop_ambience_ptr = (void(*)(int32_t))(dlsym(handle, "stop_ambience"));
    stop_card_ptr = (void(*)())(dlsym(handle, "stop_card"));
    stop_song_ptr = (void(*)())(dlsym(handle, "stop_song"));
    start_card_ptr = (void(*)(int32_t))(dlsym(handle, "start_card"));
    start_song_ptr = (void(*)(int32_t))(dlsym(handle, "start_song"));
    play_ambience_ptr = (void(*)(int32_t, int32_t))(dlsym(handle, "play_ambience"));
    play_sound_ptr = (void(*)(int, int32_t, bool))(dlsym(handle, "play_sound"));
    set_song_ptr = (bool(*)(const std::string&, int, bool))(dlsym(handle, "set_song"));
    set_sound_ptr = (bool(*)(const std::string&, int))(dlsym(handle, "set_sound"));
    set_init_volumes_ptr = (void(*)(int, int, int, int))(dlsym(handle, "set_init_volumes"));
    set_song_volume_ptr = (void(*)(float))(dlsym(handle, "set_song_volume"));
    set_card_volume_ptr = (void(*)(float))(dlsym(handle, "set_card_volume"));
    set_ambience_volume_ptr = (void(*)(int32_t, float))(dlsym(handle, "set_ambience_volume"));
    get_song_volume_ptr = (float(*)())(dlsym(handle, "get_song_volume"));
    get_card_volume_ptr = (float(*)())(dlsym(handle, "get_card_volume"));
    get_ambience_volume_ptr = (float(*)(int))(dlsym(handle, "get_ambience_volume"));
    init_sound_ptr = (bool(*)())(dlsym(handle, "init_sound"));
    deinit_sound_ptr = (void(*)())(dlsym(handle, "deinit_sound"));
    update_ptr = (void(*)())(dlsym(handle, "update"));
  }
  else
  {
    static auto no_op = []() {};
    static auto returns_false = []() {return false; };
    static auto returns_zero = []() { return 0.0f; };
    static auto takes_int = [](int32_t a) {};
    is_ambience_playing_ptr = [](int32_t a) {return false; };
    is_card_playing_ptr = returns_false;
    is_song_playing_ptr = returns_false;
    stop_ambience_ptr = takes_int;
    stop_card_ptr = no_op;
    stop_song_ptr = no_op;
    start_card_ptr = takes_int;
    start_song_ptr = takes_int;
    play_ambience_ptr = [](int32_t a, int32_t b) {};
    play_sound_ptr = [](int a, int32_t b, bool c) {};
    set_song_ptr=[](const std::string &a,int b,bool c) { return false; };
    set_sound_ptr=[](const std::string &a,int b) { return false; };
    set_init_volumes_ptr = [](int a, int b, int c, int d) {};
    set_song_volume_ptr = [](float a) {};
    set_card_volume_ptr = [](float a) {};
    set_ambience_volume_ptr = [](int32_t a, float b) {};
    get_song_volume_ptr = returns_zero;
    get_card_volume_ptr = returns_zero;
    get_ambience_volume_ptr = [](int32_t a) {return 0.0f; };
    init_sound_ptr = returns_false;
    deinit_sound_ptr = no_op;
    update_ptr = no_op;
  }
}
#else

musicsound_info::musicsound_info() {
  fmod_system = NULL;
  sound_channel.fill(NULL);
  song_channel = NULL;
  ambience_channel[0] = NULL;
  ambience_channel[1] = NULL;
  card_channel = NULL;
  mod.fill(NULL);
  samp.fill(NULL);
  stream.fill(NULL);
}

bool musicsound_info::is_ambience_playing(int32_t ambience_active_channel)
{
  bool active_is_playing = false;
  if (ambience_channel[ambience_active_channel] != NULL)
  {
    result = ambience_channel[ambience_active_channel]->isPlaying(&active_is_playing);
  }
  return active_is_playing;
}
void musicsound_info::stop_ambience(int32_t ambience_active_channel)
{
  if (ambience_channel[ambience_active_channel] != NULL)
  {
    ambience_channel[ambience_active_channel]->stop();
    ambience_channel[ambience_active_channel] = NULL;
  }
}
void musicsound_info::play_ambience(int32_t new_ambience, int32_t ambience_active_channel)
{
  result = fmod_system->playSound(stream[new_ambience], NULL, true, &ambience_channel[ambience_active_channel]);
  ambience_channel[ambience_active_channel]->setPriority(2);
  ambience_channel[ambience_active_channel]->setPan(0.0f);
  //ambience continues from where it left off
  //ambience_channel[ambience_active_channel]->setPosition(0,FMOD_TIMEUNIT_MS);
  ambience_channel[ambience_active_channel]->setVolume(0.0f);
  ambience_channel[ambience_active_channel]->setPaused(false);
}
bool musicsound_info::is_card_playing()
{
  bool is_playing = false;
  if (card_channel != NULL)
  {
    result = card_channel->isPlaying(&is_playing);
  }
  return is_playing;
}
void musicsound_info::stop_card()
{
  if (card_channel != NULL)
  {
    card_channel->stop();
    card_channel = NULL;
  }
}
void musicsound_info::start_card(int32_t new_song)
{
  stop_card();
  result = fmod_system->playSound(stream[new_song], NULL, true, &card_channel);
  card_channel->setPriority(1);
  card_channel->setPan(0.0f);
  card_channel->setPosition(0, FMOD_TIMEUNIT_MS);
  card_channel->setVolume(0.0f);
  card_channel->setPaused(false);
}
bool musicsound_info::is_song_playing()
{
  bool is_playing = false;
  if (song_channel != NULL)
    result = song_channel->isPlaying(&is_playing);
  return is_playing;
}
void musicsound_info::stop_song()
{
  if (song_channel != NULL)
  {
    song_channel->stop();
    song_channel = NULL;
  }
}
void musicsound_info::start_song(int32_t new_song)
{
  result = fmod_system->playSound(stream[new_song], NULL, true, &song_channel);
  song_channel->setPriority(0);
  song_channel->setPan(0.0f);
  song_channel->setPosition(0, FMOD_TIMEUNIT_MS);
  song_channel->setVolume(0.0f);
  song_channel->setPaused(false);
}

float int_vol_to_float(int n) {
  constexpr static float coeff = 1.0f / 255.0f;
  return n==255 ? 1.0f : (float)n * coeff;
}

void musicsound_info::play_sound(int s, int32_t vol, bool use_media_sound_volume)
{
  int32_t i;
  for (i = 0; i < 10; ++i)
  {
    if (sound_channel[i] == NULL)
    {
      result = fmod_system->playSound(samp[s], 0, true, &sound_channel[i]);
      sound_channel[i]->setPriority(128);
      sound_channel[i]->setPan(s == SOUND_ALERT ? -0.75f : 0.0f);
      sound_channel[i]->setPosition(0, FMOD_TIMEUNIT_MS);
      sound_channel[i]->setVolume(int_vol_to_float(vol) * use_media_sound_volume?int_vol_to_float(init.media.volume_sfx):1.0f * int_vol_to_float(init.media.volume_master));
      sound_channel[i]->setPaused(false);
      return;
    }
  }
  for (i = 0; i < 10; ++i)
  {
    if (sound_channel[i] != NULL)
    {
      bool is_playing = false;
      sound_channel[i]->isPlaying(&is_playing);
      if (!is_playing)
      {
        result = fmod_system->playSound(samp[s], 0, true, &sound_channel[i]);
        sound_channel[i]->setPriority(128);
        sound_channel[i]->setPan(s == SOUND_ALERT ? -0.5f : 0.0f);
        sound_channel[i]->setPosition(0, FMOD_TIMEUNIT_MS);
        sound_channel[i]->setVolume(int_vol_to_float(init.media.volume_sfx) * int_vol_to_float(init.media.volume_master));
        sound_channel[i]->setPaused(false);
        return;
      }
    }
  }
  i = 0;
  if (sound_channel[i] != NULL)
  {
    sound_channel[i]->stop();

    result = fmod_system->playSound(samp[s], 0, true, &sound_channel[i]);
    sound_channel[i]->setPriority(128);
    sound_channel[i]->setPan(s == SOUND_ALERT ? -0.5f : 0.0f);
    sound_channel[i]->setPosition(0, FMOD_TIMEUNIT_MS);
    sound_channel[i]->setVolume(int_vol_to_float(init.media.volume_sfx) * int_vol_to_float(init.media.volume_master));
    sound_channel[i]->setPaused(false);
  }
}
bool musicsound_info::set_song(const string& filename, int slot, bool loops)
{
  if (stream[slot] != NULL)
  {
    stream[slot]->release();
    stream[slot] = NULL;
  }

  result = fmod_system->createStream(filename.c_str(), (loops ? FMOD_LOOP_NORMAL : 0) | FMOD_2D, 0, &stream[slot]);
  switch (result)
  {
  case FMOD_OK: 
    return true;
  case FMOD_ERR_FILE_BAD:
    errorlog_string("Could not load file "+filename+": error loading file.");
    return false;
  case FMOD_ERR_FILE_COULDNOTSEEK:
    errorlog_string("Could not load file "+filename+": couldn't perform seek operation.");
    return false;
  case FMOD_ERR_FILE_DISKEJECTED:
    errorlog_string("Could not load file "+filename+": media was ejected while reading.");
    return false;
  case FMOD_ERR_FILE_EOF:
    errorlog_string("Could not load file "+filename+": end of file unexpectedly reached while trying to read essential data.");
    return false;
  case FMOD_ERR_FILE_ENDOFDATA:
    errorlog_string("Could not load file "+filename+": End of current chunk reached while trying to read data.");
    return false;
  case FMOD_ERR_FILE_NOTFOUND:
    errorlog_string("Could not load file "+filename+": file not found. ");
    return false;
  case FMOD_ERR_FORMAT:
    errorlog_string("Could not load file "+filename+": unsupported file or audio format.");
    return false;
  default:
    return false;
  }
}
bool musicsound_info::set_sound(const string& filename, int slot)
{
  if (samp[slot] != NULL)
  {
    samp[slot]->release();
    samp[slot] = NULL;
  }

  result = fmod_system->createSound(filename.c_str(), FMOD_LOOP_OFF, 0, &samp[slot]);
  switch (result)
  {
  case FMOD_OK:
    return true;
  case FMOD_ERR_FILE_BAD:
    errorlog_string("Could not load file "+filename+": error loading file.");
    return false;
  case FMOD_ERR_FILE_COULDNOTSEEK:
    errorlog_string("Could not load file "+filename+": couldn't perform seek operation.");
    return false;
  case FMOD_ERR_FILE_DISKEJECTED:
    errorlog_string("Could not load file "+filename+": media was ejected while reading.");
    return false;
  case FMOD_ERR_FILE_EOF:
    errorlog_string("Could not load file "+filename+": end of file unexpectedly reached while trying to read essential data.");
    return false;
  case FMOD_ERR_FILE_ENDOFDATA:
    errorlog_string("Could not load file "+filename+": End of current chunk reached while trying to read data.");
    return false;
  case FMOD_ERR_FILE_NOTFOUND:
    errorlog_string("Could not load file "+filename+": file not found. ");
    return false;
  case FMOD_ERR_FORMAT:
    errorlog_string("Could not load file "+filename+": unsupported file or audio format.");
    return false;
  default:
    return false;
  }
}
void musicsound_info::set_init_volumes(int master_volume, int snd_volume, int ambience_volume, int music_volume) {} // technically only here to keep the API clean elsewhere

void musicsound_info::set_song_volume(float vol)
{
  if (song_channel != NULL)
  {
    song_channel->setVolume(vol);
  }
}
void musicsound_info::set_card_volume(float vol)
{
  if (card_channel != NULL)
  {
    card_channel->setVolume(vol);
  }
}
void musicsound_info::set_ambience_volume(int32_t chan, float vol)
{
  if (ambience_channel[chan] != NULL)
  {
    ambience_channel[chan]->setVolume(vol);
  }
}
float musicsound_info::get_song_volume()
{
  float volume = 0.0f;
  if (song_channel != NULL)
  {
    song_channel->getVolume(&volume);
  }
  return volume;
};
float musicsound_info::get_card_volume()
{
  float volume = 0.0f;
  if (card_channel != NULL)
  {
    card_channel->getVolume(&volume);
  }
  return volume;
};
float musicsound_info::get_ambience_volume(int32_t chan)
{
  float volume = 0.0f;
  if (ambience_channel[chan] != NULL)
  {
    ambience_channel[chan]->getVolume(&volume);
  }
  return volume;
};
bool musicsound_info::init_sound()
{
  result = FMOD::System_Create(&fmod_system);
  if (fmod_system == NULL)return false;

  sound_channel.fill(NULL);
  song_channel = NULL;
  ambience_channel[0] = NULL;
  ambience_channel[1] = NULL;
  card_channel = NULL;
  mod.fill(NULL);
  samp.fill(NULL);
  stream.fill(NULL);
  result = fmod_system->init(SOUND_CHANNELNUM, FMOD_INIT_NORMAL, NULL);
  fmod_system->setStreamBufferSize(2000, FMOD_TIMEUNIT_MS);
  return true;
}
void musicsound_info::deinit_sound()
{
  for (auto& chan : sound_channel)
  {
    if (chan) chan->stop();
  };
  if (song_channel)song_channel->stop();
  if (ambience_channel[0] != NULL)ambience_channel[0]->stop();
  if (ambience_channel[1] != NULL)ambience_channel[1]->stop();
  if (card_channel != NULL)card_channel->stop();
  for (auto& m : mod)
  {
    if (m) m->release();
  }
  for (auto& s : samp)
  {
    if (s) s->release();
  }

  fmod_system->close();
  fmod_system->release();
  fmod_system = NULL;
}
void musicsound_info::update()
{
  if (fmod_system == NULL)return;

  fmod_system->update();
}
#endif