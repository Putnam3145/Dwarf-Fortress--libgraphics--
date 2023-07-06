#include "files.hpp"
#include "../platform/platform.hpp"

#include <cerrno>
#include <string>
#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <stdio.h>

extern "C" {
#include <zlib.h>
#include "../_external/zlib/contrib/minizip/unzip.h"
#ifndef WIN32
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <signal.h>
#endif
}

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
#include "../util/basics.hpp"
#include "../util/endian.hpp"
#include "../files/files.hpp"
#include "../render/enabler.hpp"
#include "../files/find_files.hpp"

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

void copy_file(const std::string &src,const std::string &dst){

  std::ifstream in_stream(src.c_str(),std::ios_base::binary);
  std::ofstream out_stream(dst.c_str(),std::ios_base::binary);
  if(in_stream.is_open()&&out_stream.is_open())
  {
    out_stream<<in_stream.rdbuf();
  }
  in_stream.close();
  out_stream.close();

}

void replace_file(const std::string &src, const std::string &dst) {
#ifdef WIN32
  remove(dst.c_str());
#endif
  rename(src.c_str(), dst.c_str());
}
