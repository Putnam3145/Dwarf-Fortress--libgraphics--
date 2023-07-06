#ifndef DF_GSRC_FILES_HPP
#define DF_GSRC_FILES_HPP

//Copyright (c) 2006, Tarn Adams
//All rights reserved.  See game.cpp or license.txt for more information.

#include <string>
#include <string_view>
#include <filesystem>

std::string_view get_base_path_str();
std::filesystem::path get_base_path();

void copy_file(const std::string &src,const std::string &dst);
// Replaces dst with src, removing src in the process. Atomic if possible.
void replace_file(const std::string &src, const std::string &dst);

#endif // DF_GSRC_FILES_HPP
