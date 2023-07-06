#ifndef DF_GSRC_FIND_FILES_HPP
#define DF_GSRC_FIND_FILES_HPP

#include "../util/svector.hpp"
#include "../text/stringvec.hpp"

void find_files_by_pattern(const char* pattern, svector<char *>& vec);
void find_files_by_pattern_with_exception(const char* pattern, svector<char *>& vec, const char* exception);
void find_files_by_pattern(const char* pattern, stringvectst &vec);
void find_files_by_pattern_with_exception(const char* pattern, stringvectst &vec, const char* exception);
void find_directories_by_pattern(const char* pattern, stringvectst &filenames);
void find_directories_by_pattern_with_exception(const char* pattern, stringvectst &filenames,const char *exception);

#endif