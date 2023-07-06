using std::string;

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
#include "../files/files.hpp"
#include "../util/definitions.hpp"

#include "../render/enabler.hpp"
#include "../render/init.hpp"
#include "../audio/music_and_sound_g.hpp"

struct openal_musicsound_info : public musicsound_info
{
  virtual bool is_ambience_playing(int32_t ambience_active_channel)=0;
  virtual bool is_card_playing()=0;
  virtual bool is_song_playing()=0;
  virtual void stop_ambience(int32_t ambience_active_channel)=0;
  virtual void stop_card()=0;
  virtual void stop_song()=0;
  virtual void start_card(int32_t new_song)=0;
  virtual void start_song(int32_t new_song)=0;
  virtual void play_ambience(int32_t new_ambience,int32_t ambience_active_channel)=0;
  virtual void play_sound(int s,int32_t vol,bool use_media_sound_volume)=0;
  virtual void set_song(const std::string &filename,int slot,bool loops=false)=0;
  virtual void set_sound(const std::string &filename,int slot)=0;
  virtual void set_song_volume(float vol)=0;
  virtual void set_card_volume(float vol)=0;
  virtual void set_ambience_volume(int32_t chan,float vol)=0;
  virtual float get_song_volume()=0;
  virtual float get_card_volume()=0;
  virtual float get_ambience_volume(int32_t chan)=0;
  virtual bool init_sound() { return false; }
  virtual void deinit_sound() {}
  virtual void update() {}
};