#include "file_compressor.hpp"
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

using std::string;

#include "../util/basics.hpp"
#include "../util/endian.hpp"
#include "../files/files.hpp"
#include "../render/enabler.hpp"
#include "../files/find_files.hpp"

inline void CHECK_ERR(int err, const char* msg)
{
  if (err != Z_OK)
  {
    MessageBox(NULL, "One of the compressed files on disk has errors in it.  Restore from backup if you are able.", 0, 0);
    exit(1);
  }
}

using std::fstream;

char file_compressorst::def_ibuff[FILE_IN_BUFF];
char file_compressorst::def_obuff[FILE_OUT_BUFF];

char file_compressorst::load_posnull_pointer()
{
  int8_t dummy;
  read_file(dummy);

  if(!dummy)return 0;
  else return 1;
}

char file_compressorst::save_posnull_pointer(void *ptr)
{
  int8_t dummy;

  if(ptr==NULL)
  {
    dummy=0;
    write_file(dummy);
    return 0;
  }
  else
  {
    dummy=1;
    write_file(dummy);
    return 1;
  }
}

char file_compressorst::write_file(string &str)
{
  int16_t ln=(int16_t)str.length();
  if(ln>=10000||ln<0)ln=0;

  if(!write_file(ln))return 0;

  if(ln==0)return 1;

  if(!write_file((void *)str.c_str(),sizeof(char)*ln))return 0;

  return 1;
}

char file_compressorst::read_file(string &str)
{
  str.erase();

  int16_t ln;

  if(!read_file(ln))return 0;

  if(ln==0)return 1;

  char *strar=new char[ln+1];

  if(!read_file(strar,sizeof(char)*ln))
  {
    delete[] strar;
    return 0;
  }
  strar[ln]='\x0';

  str=strar;

  delete[] strar;
  return 1;
}

char file_compressorst::write_file(void *write_var,long write_size)
{
  if (!f.is_open())return 0;

  if(!compressed)
  {
    f.write((char *)write_var,write_size);
    return 1;
  }

  //WRITE OUT THE VARIABLE CHUNK BY CHUNK
  while(write_size>0)
  {
    //FLUSH THE BUFFER IF NECESSARY
    if(in_buffer_amount_loaded>=in_buffersize)
    {
      if(!flush_in_buffer())return 0;
    }

    //SET THE AMOUNT TO COPY
    long copy_size=in_buffersize-in_buffer_amount_loaded;
    if(write_size<copy_size)copy_size=write_size;

    //COPY THE NEXT CHUNK INTO THE BUFFER
    memmove(in_buffer+in_buffer_amount_loaded,write_var,copy_size);

    write_var=((char *)write_var) + copy_size;
    write_size-=copy_size;
    in_buffer_amount_loaded+=copy_size;
  }

  return 1;
}

char file_compressorst::flush_in_buffer()
{
  if (!f.is_open())return 0;
  if(!compressed)return 1;
  if(in_buffer_amount_loaded==0)return 1;//EXTRA CALLS TO FLUSH SHOULDN'T KILL YOU

  //TARN: adapted from zlib example files

  //SET UP THE COMPRESSOR
  z_stream c_stream;
  int err;

  c_stream.zalloc = (alloc_func)0;
  c_stream.zfree = (free_func)0;
  c_stream.opaque = (voidpf)0;

  err = deflateInit(&c_stream, 9);
  CHECK_ERR(err, "deflateInit");

  c_stream.next_out = (Bytef*)out_buffer;
  c_stream.avail_out = out_buffersize;

  //SEND IT TO THE COMPRESSOR
  c_stream.next_in = (Bytef*)in_buffer;
  c_stream.avail_in = in_buffer_amount_loaded;

  while (c_stream.total_in != in_buffer_amount_loaded && c_stream.total_out < out_buffersize)
  {
    err = deflate(&c_stream, Z_NO_FLUSH);
    CHECK_ERR(err, "deflate");
  }

  //FINISH UP THE STREAM
  while(1)
  {
    err = deflate(&c_stream, Z_FINISH);
    if (err == Z_STREAM_END) break;
    CHECK_ERR(err, "deflate");
  }

  err = deflateEnd(&c_stream);
  CHECK_ERR(err, "deflateEnd");

  //SAVE THE COMPRESSED BIT AS A GIANT BLOCK
  if(c_stream.total_out>0)
  {
    int32_t compsize=byteswap((int32_t)(c_stream.total_out));

    //WRITE IT
    f.write((char*)&compsize,sizeof(int32_t));
    f.write(out_buffer,c_stream.total_out);
  }

  in_buffer_position=0;
  in_buffer_amount_loaded=0;

  return 1;
}

char file_compressorst::read_file(void *read_var,long read_size)
{
  if (!f.is_open())return 0;

  if(!compressed)
  {
    f.read((char *)read_var,read_size);
    return 1;
  }

  //NOW LOAD INTO read_var UNTIL DONE
  while(read_size>0)
  {
    //GET A BUFFER IF NECESSARY
    if(in_buffer_amount_loaded==0||
      in_buffer_position>=in_buffer_amount_loaded)
    {
      if(!load_new_in_buffer())return 0;
    }

    //BAIL IF STILL NO BUFFER LEFT
    if(in_buffer_amount_loaded==0)return 0;

    //SET THE AMOUNT TO COPY
    long copy_size=in_buffer_amount_loaded-in_buffer_position;
    if(read_size<copy_size)copy_size=read_size;

    //COPY
    memmove(read_var,in_buffer+in_buffer_position,copy_size);

    read_var=((char *)read_var) + copy_size;
    read_size-=copy_size;
    in_buffer_position+=copy_size;
  }
  
  return 1;
}

char file_compressorst::load_new_in_buffer()
{
  if (!f.is_open())return 0;
  if(!compressed)return 1;

  in_buffer_position=0;
  in_buffer_amount_loaded=0;

  //LOAD THE BLOCK OF COMPRESSED DATA
  f.read((char*)&out_buffer_amount_written,sizeof(int32_t));
  out_buffer_amount_written=byteswap(out_buffer_amount_written);
  f.read(out_buffer,out_buffer_amount_written);

  int err;

  if(out_buffer_amount_written>0)
  {
    //TARN: adapted from zlib example files

    //UNCOMPRESS
    z_stream d_stream; // decompression stream

    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in  = (Bytef*)out_buffer;
    d_stream.avail_in = out_buffer_amount_written;

    err = inflateInit(&d_stream);
    CHECK_ERR(err, "inflateInit");

    d_stream.next_out = (Bytef*)in_buffer;
    d_stream.avail_out = in_buffersize;

    while (d_stream.total_out < in_buffersize && d_stream.total_in < out_buffer_amount_written)
    {
      //d_stream.avail_in = d_stream.avail_out = 1; // force small buffers
      err = inflate(&d_stream, Z_NO_FLUSH);

      if (err == Z_STREAM_END) break;
      CHECK_ERR(err, "inflate");
    }

    err = inflateEnd(&d_stream);
    CHECK_ERR(err, "inflateEnd");

    in_buffer_amount_loaded=d_stream.total_out;

    return 1;
  }
  else return 0;
}

void file_compressorst::close_file()
{
  if (f.is_open())
  {
    f.close();
  }
}

char file_compressorst::open_file(const string &filename,char existing_only)
{
  if(filename.empty())return 0;

  //RESET DATA
  in_buffer_amount_loaded=0;
  in_buffer_position=0;
  out_buffer_amount_written=0;

/*
  //CHECK IF FILE ALREADY EXISTS
  f.open(filename.c_str(), fstream::in);
  char file_exists = f.is_open();
  if (file_exists)f.close();

  //OPEN FILE
  if(!existing_only || file_exists)
    f.open(filename.c_str(), fstream::in | fstream::out | fstream::binary);
  else
    f.open(filename.c_str(), fstream::in | fstream::out | fstream::binary);
*/  
  //if(existing_only)h=CreateFile(filename.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  //else h=CreateFile(filename.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

  if(f.is_open())f.close();
  f.clear();

  if(existing_only)
    f.open(filename.c_str(), fstream::in | fstream::out | fstream::binary);
  else
    f.open(filename.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);

  if (f.is_open())return 1;
  else return 0;
}

void file_compressorst::set_buffer_info(char *new_in_buffer,long new_in_buffersize,
 char *new_out_buffer,long new_out_buffersize)
{
  in_buffer=new_in_buffer;
  in_buffersize=new_in_buffersize;
  in_buffer_amount_loaded=0;
  in_buffer_position=0;

  out_buffer=new_out_buffer;
  out_buffersize=new_out_buffersize;
  out_buffer_amount_written=0;
  
  f.clear();
}

// Class file_compressorst Member Functions

void file_compressorst::write_file_fixedlength(char *var){
  int16_t len=(int16_t)strlen(var);
  write_file(len);
  if(len>0)write_file(var,len*sizeof(char));
}

void file_compressorst::read_file_fixedlength(char *var){
  int16_t len;
  read_file(len);//DO NOT NEED TO ALLOCATE SPACE
  if(len>0)read_file(var,len*sizeof(char));
  var[len]='\x0';
}

char file_compressorst::write_file(int32_t var){
  var=byteswap(var);
  return write_file(&var,sizeof(int32_t));
}

char file_compressorst::read_file(int32_t &var){
  char ret = read_file(&var,sizeof(int32_t));
  var = byteswap(var);
  return ret;
}

char file_compressorst::write_file(int16_t var){
  var=byteswap(var);
  return write_file(&var,sizeof(int16_t));
}

char file_compressorst::read_file(int16_t &var){
  char ret = read_file(&var,sizeof(int16_t));
  var = byteswap(var);
  return ret;
}

char file_compressorst::write_file(int8_t var){
  return write_file(&var,sizeof(int8_t));
}

char file_compressorst::read_file(int8_t &var){
  return read_file(&var,sizeof(int8_t));
}

char file_compressorst::write_file(bool var){
  int8_t temp;
  if(var)temp=1;
  else temp=0;
  return write_file(&temp,sizeof(int8_t));
}

char file_compressorst::read_file(bool &var){
  int8_t temp;
  if(!read_file(&temp,sizeof(int8_t)))return 0;
  var=(temp!=0);
  return 1;
}

char file_compressorst::write_file(uint32_t var){
  var=byteswap(var);
  return write_file(&var,sizeof(uint32_t));
}

char file_compressorst::read_file(uint32_t &var){
  char ret = read_file(&var,sizeof(uint32_t));
  var = byteswap(var);
  return ret;
}

char file_compressorst::write_file(uint16_t var){
  var=byteswap(var);
  return write_file(&var,sizeof(uint16_t));
}

char file_compressorst::read_file(uint16_t &var){
  char ret = read_file(&var,sizeof(uint16_t));
  var = byteswap(var);
  return ret;
}

char file_compressorst::write_file(uint8_t var){
  return write_file(&var,sizeof(uint8_t));
}

char file_compressorst::read_file(uint8_t &var){
  return read_file(&var,sizeof(uint8_t));
}

void file_compressorst::write_file(svector<bool> &vect){

  int32_t s=(int32_t)vect.size();
  write_file(s);
  bool bl;//JUST FOR PARITY WITH read BELOW
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    bl=(*i_b);
    write_file(bl);
  }
}

void file_compressorst::read_file(svector<bool> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  bool bl;//NO IDEA WHY IT CAN'T JUST TAKE vect[i]
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file(bl);
    (*i_b)=bl;
  }
}

void file_compressorst::write_file(svector<int16_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<int16_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  }
}

void file_compressorst::write_file(svector<uint16_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<uint16_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  }
}

void file_compressorst::write_file(svector<uint8_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<uint8_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  }
}
void file_compressorst::write_file(svector<int8_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<int8_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  }
}

void file_compressorst::write_file(svector<int32_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<int32_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  } 
}

void file_compressorst::write_file(svector<uint32_t> &vect){
  int32_t s=(int32_t)vect.size();
  write_file(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    write_file((*i_b));
  }
}

void file_compressorst::read_file(svector<uint32_t> &vect){
  int32_t s;
  read_file(s);
  vect.resize(s);
  auto i_b=vect.begin(),i_e=vect.end();
  for(;i_b<i_e;++i_b){
    read_file((*i_b));
  }
}
