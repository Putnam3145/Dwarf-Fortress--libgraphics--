#ifndef WIN32

#include "audio.hpp"

#include <SDL2/SDL_mixer.h>

//#include <cstdint>

static int volume_ambience = 128;

static int volume_music = 128;

static int volume_sfx = 128;

static int volume_master = 128;

enum struct StreamAudioType : int8_t {
  SONG,
  CARD,
  AMBIENCE,
  NONE = -1
};

static StreamAudioType stream_type;
static int32_t cur_stream_id;
static std::array<Mix_Chunk*, MAXSOUNDNUM> samp;
static std::array<Mix_Music*, MAXSOUNDNUM> stream;

extern "C"
{
  bool init_sound()
  {
    stream_type = StreamAudioType::NONE;
    samp.fill(NULL);
    stream.fill(NULL);
    Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 2048);
    return true;
  }
  void deinit_sound()
  {
    Mix_CloseAudio();
  }

  bool is_ambience_playing(int32_t ambience_active_channel) 
  {
    return stream_type == StreamAudioType::AMBIENCE && Mix_PlayingMusic();
  }
  void stop_ambience(int32_t ambience_active_channel) 
  {
    Mix_PauseMusic();
  }
  void play_ambience(int32_t new_ambience, int32_t ambience_active_channel) 
  {
    switch (stream_type)
    {
    case StreamAudioType::AMBIENCE:
      if (new_ambience == cur_stream_id)
      {
        if (Mix_PausedMusic()) {
          Mix_ResumeMusic();
          return;
        }
      }
      else
      {
        Mix_HaltMusic();
      }
    case StreamAudioType::NONE:
      Mix_PlayMusic(stream[new_ambience], -1);
      Mix_VolumeMusic(volume_ambience/2);
      stream_type=StreamAudioType::AMBIENCE;
    }
  }
  bool is_card_playing()
  {
    return stream_type == StreamAudioType::CARD && Mix_PlayingMusic();
  }
  void stop_card()
  {
    Mix_PauseMusic();
  }
  void start_card(int32_t new_song) 
  {
    switch (stream_type)
    {
    case StreamAudioType::CARD:
      Mix_HaltMusic();
    case StreamAudioType::AMBIENCE:
    case StreamAudioType::NONE:
      Mix_PlayMusic(stream[new_song], -1);
      Mix_VolumeMusic(volume_music/2);
      stream_type=StreamAudioType::CARD;
    }
  }
  bool is_song_playing()
  {
    return stream_type == StreamAudioType::SONG && Mix_PlayingMusic();
  }
  void stop_song()
  {
    Mix_PauseMusic();
  }
  void start_song(int32_t new_song) 
  {
    switch (stream_type)
    {
    case StreamAudioType::SONG:
      Mix_HaltMusic();
    case StreamAudioType::CARD:
    case StreamAudioType::AMBIENCE:
    case StreamAudioType::NONE:
      Mix_PlayMusic(stream[new_song], -1);
      Mix_VolumeMusic(volume_music / 2);
      stream_type=StreamAudioType::SONG;
    }
  }
  void play_sound(int s, int32_t vol,bool use_media_sound_volume) 
  {
    for (int i = 0; i < 8; i++) {
      if (Mix_Playing(i) == 0)
      {
        Mix_Volume(i, volume_sfx * volume_master / 2);
        Mix_SetPanning(i, 255, s == SOUND_ALERT ? 64 : 255);
        Mix_PlayChannelTimed(i, samp[s], 0, -1);
      }
    }
  }
  bool set_song(const std::string& filename, int slot,bool loops)
  {
    if (stream[slot] != NULL)
    {
      Mix_FreeMusic(stream[slot]);
      stream[slot] = NULL;
    }
    stream[slot] = Mix_LoadMUS(filename.c_str());
    return stream[slot] != NULL;
  }
  bool set_sound(const std::string& filename, int slot)
  {
    if (samp[slot] != NULL)
    {
      Mix_FreeChunk(samp[slot]);
      samp[slot] = NULL;
    }
    samp[slot] = Mix_LoadWAV_RW(SDL_RWFromFile(filename.c_str(), "rb"), 1);
    return stream[slot] != NULL;
  }
  void set_init_volumes(int master_volume, int snd_volume, int ambience_volume, int music_volume)
  {
    volume_master = master_volume/2;
    volume_sfx = snd_volume/2;
    volume_ambience = ambience_volume / 2;
    volume_music = music_volume / 2;
  }
  void set_song_volume(float vol)
  {
    if(is_song_playing())
      Mix_VolumeMusic(vol * 128);
  }
  void set_card_volume(float vol)
  {
    if(is_card_playing())
      Mix_VolumeMusic(vol * 128);
  }
  void set_ambience_volume(int32_t chan, float vol)
  {
    if(is_ambience_playing(chan))
      Mix_VolumeMusic(vol * 128);
  }
  float get_song_volume()
  {
    if (is_song_playing())
      return (float)Mix_VolumeMusic(-1)/128.0f;
    return 0.0f;
  };
  float get_card_volume()
  {
    if (is_card_playing())
      return (float)Mix_VolumeMusic(-1) / 128.0f;
    return 0.0f;
  };
  float get_ambience_volume(int32_t chan)
  {
    if (is_ambience_playing(chan))
      return (float)Mix_VolumeMusic(-1) / 128.0f;
    return 0.0f;
  };
  void update()
  {
    // sorry, nothing
    // no need to apologize!
  }
};

#endif