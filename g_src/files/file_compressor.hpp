#ifndef DF_GSRC_FILES_COMPRESSOR_HPP
#define DF_GSRC_FILES_COMPRESSOR_HPP

//Copyright (c) 2006, Tarn Adams
//All rights reserved.  See game.cpp or license.txt for more information.

#include "../util/endian.hpp"
#include "../util/svector.hpp"

#include <string>
#include <fstream>
#include <string_view>
#include <filesystem>

// Constant File-Buffer Sizes

#define FILE_IN_BUFF 1000000
#define FILE_OUT_BUFF 2000000

// Global Functions

std::string_view get_base_path_str();
std::filesystem::path get_base_path();

void copy_file(const std::string &src,const std::string &dst);
// Replaces dst with src, removing src in the process. Atomic if possible.
void replace_file(const std::string &src, const std::string &dst);

// File Compressor Class

class file_compressorst {
public:

  bool compressed;
  std::fstream f;

  file_compressorst();
  file_compressorst(char *new_in_buffer, long new_in_buffersize, char *new_out_buffer, long new_out_buffersize){
    compressed=true;

    in_buffer=def_ibuff;
    in_buffersize=FILE_IN_BUFF;
    in_buffer_amount_loaded=0;
    in_buffer_position=0;

    out_buffer=def_obuff;
    out_buffersize=FILE_OUT_BUFF;
    out_buffer_amount_written=0;
    
    f.clear();
  }
  
  ~file_compressorst(){
    close_file();
  }

  char open_file(const std::string& filename, char existing_only);
  void close_file();

  char write_file(std::string& str);
  char read_file(std::string& str);

  char load_posnull_pointer();
  char save_posnull_pointer(void* ptr);

  void write_file_fixedlength(char* var);
  void read_file_fixedlength(char* var);

  char write_file(int32_t var);
  char read_file(int32_t& var);
  char write_file(int16_t var);
  char read_file(int16_t& var);
  char write_file(int8_t var);
  char read_file(int8_t& var);
  char write_file(bool var);
  char read_file(bool& var);
  char write_file(uint32_t var);
  char read_file(uint32_t& var);
  char write_file(uint16_t var);
  char read_file(uint16_t& var);
  char write_file(uint8_t var);
  char read_file(uint8_t& var);
  void write_file(svector<bool>& vect);
  void read_file(svector<bool>& vect);
  void write_file(svector<int16_t>& vect);
  void read_file(svector<int16_t>& vect);
  void write_file(svector<uint16_t>& vect);
  void read_file(svector<uint16_t>& vect);
  void write_file(svector<uint8_t>& vect);
  void read_file(svector<uint8_t>& vect);
  void write_file(svector<int8_t>& vect);
  void read_file(svector<int8_t>& vect);
  void write_file(svector<int32_t>& vect);
  void read_file(svector<int32_t>& vect);
  void write_file(svector<uint32_t>& vect);
  void read_file(svector<uint32_t>& vect);

  char load_new_in_buffer();
  char flush_in_buffer();

  void set_buffer_info(char *new_in_buffer,long new_in_buffersize, char *new_out_buffer, long new_out_buffersize);

private:

  char write_file(void *write_var,long write_size);
  char read_file(void *read_var,long read_size);

  char *in_buffer;
  long in_buffersize;
  long in_buffer_amount_loaded;
  long in_buffer_position;

  char *out_buffer;
  long out_buffersize;
  int32_t out_buffer_amount_written;

  static char def_ibuff[FILE_IN_BUFF];
  static char def_obuff[FILE_OUT_BUFF];

};

#endif // DF_GSRC_FILES_COMPRESSOR_HPP