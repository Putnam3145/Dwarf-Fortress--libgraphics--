#ifndef WIN32

#include "find_files.hpp"

#include "../platform/platform.hpp"
#include "../platform/types.hpp"

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

void find_files_by_pattern(const char* pattern, svector<char *>& filenames){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){

    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);

      if(!S_ISREG(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){

        int len = strlen(++src);
        char* c = new char[len + 1];  // caller will free this
        strcpy(c, src);
        filenames.push_back(c);
      }

    }
  }

  globfree(&g);

}

void find_files_by_pattern_with_exception(const char* pattern, svector<char *>& filenames,const char *exception){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){

    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);
      
      if(!S_ISREG(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){

        int len = strlen(++src);
        if(!strcmp(src,exception))
          continue;
        
        char* c = new char[len + 1];  // caller will free this
        strcpy(c, src);
        filenames.push_back(c);
      }
    }
  }

  globfree(&g);

}

void find_files_by_pattern(const char* pattern, stringvectst& filenames){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){

    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);

      if(!S_ISREG(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){
        ++src;
        filenames.add_string(src);
      }

    }
  }

  globfree(&g);

}

void find_files_by_pattern_with_exception(const char* pattern, stringvectst& filenames,const char *exception){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){

    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);

      if(!S_ISREG(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){
        ++src;
        if(!strcmp(src,exception))
          continue;
        
        filenames.add_string(src);
      }

    }
  }

  globfree(&g);

}

void find_directories_by_pattern(const char* pattern, stringvectst &filenames){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){

    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);

      if(!S_ISDIR(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){
        ++src;
        filenames.add_string(src);
      }

    }
  }

  globfree(&g);

}

void find_directories_by_pattern_with_exception(const char* pattern, stringvectst &filenames,const char *exception){

  glob_t g;
  if (!glob(pattern, 0, NULL, &g)){
    for (int i = 0; i < g.gl_pathc; ++i){

      struct stat cstat;
      stat(g.gl_pathv[i],&cstat);
      
      if(!S_ISDIR(cstat.st_mode))
        continue;

      // don't include the path
      char* src = strrchr(g.gl_pathv[i], '/');
      if (src){
        ++src;
        if(!strcmp(src,exception))continue;
        
        filenames.add_string(src);
      }

    }
  }

  globfree(&g);

}

#endif // WIN32