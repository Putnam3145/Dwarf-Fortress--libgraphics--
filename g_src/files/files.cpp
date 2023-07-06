#include "files.hpp"

#include "../platform/platform.hpp"
#include "../platform/types.hpp"

#include <SDL2/SDL.h>
#include <fstream>

std::string_view get_base_path_str(){
  static char *base_folder;
  if(!base_folder){
    base_folder=SDL_GetBasePath();
  }
  return base_folder;
}

std::filesystem::path get_base_path(){
  return std::filesystem::path(get_base_path_str());
}

void copy_file(const std::string& src, const std::string& dst){

  std::ifstream in_stream(src.c_str(),std::ios_base::binary);
  std::ofstream out_stream(dst.c_str(),std::ios_base::binary);
  if(in_stream.is_open()&&out_stream.is_open()){
    out_stream<<in_stream.rdbuf();
  }

  in_stream.close();
  out_stream.close();

}

void replace_file(const std::string& src, const std::string& dst) {
#ifdef WIN32
  remove(dst.c_str());
#endif
  rename(src.c_str(), dst.c_str());
}
