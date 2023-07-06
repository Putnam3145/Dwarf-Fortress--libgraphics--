#ifndef FILES_H
#define FILES_H

//Copyright (c) 2006, Tarn Adams
//All rights reserved.  See game.cpp or license.txt for more information.

#include <string>
#include <string_view>
#include <filesystem>

#define FILE_IN_BUFF 1000000
#define FILE_OUT_BUFF 2000000

std::string_view get_base_path_str();
std::filesystem::path get_base_path();

void copy_file(const std::string &src,const std::string &dst);
// Replaces dst with src, removing src in the process. Atomic if possible.
void replace_file(const std::string &src, const std::string &dst);

#endif
