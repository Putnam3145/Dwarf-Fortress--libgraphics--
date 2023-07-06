#include "logger.hpp"

#include <fstream>
#include <syncstream>

extern thread_local std::string errorlog_prefix;

// opening/closing these too often seems to cause crashes on linux--std::osyncstream makes sure they can be used between threads, too
static std::ofstream error_file("errorlog.txt", std::ios::out|std::ios::app);
static std::ofstream gamelog_file("gamelog.txt", std::ios::out|std::ios::app);

//#define FAST_ERRORLOG

void emit_logs(){
  error_file.close();
  gamelog_file.close();
}

#ifdef FAST_ERRORLOG

std::ofstream error_feed;
bool error_opened = false;

void errorlog_string(const std::string &str){

  if(str.empty())return;

  //SAVE AN ERROR TO THE LOG FILE
  if(!error_opened)
  {
    error_feed.open("errorlog.txt", std::ios::out | std::ios::app);
    error_opened=true;
  }
  if(error_feed.is_open())
  {
    if(!errorlog_prefix.empty())
    {
      error_feed<<errorlog_prefix.c_str()<<std::endl;
      errorlog_prefix.clear();
    }
    error_feed<<str.c_str()<<std::endl;
  }
  //fseed.close();
}

#else

void errorlog_string(const std::string &str)
{
  if(str.empty())return;
  if (!error_file.is_open()) error_file.open("errorlog.txt",std::ios::out|std::ios::app);
  //SAVE AN ERROR TO THE LOG FILE
  std::osyncstream fseed(error_file);
  if(!errorlog_prefix.empty())
  {
    fseed<<errorlog_prefix.c_str()<<std::endl;
    errorlog_prefix.clear();
  }
  fseed<<str.c_str()<<std::endl;
}

#endif

void gamelog_string(const std::string &str){

  if(str.empty())return;

  if(!gamelog_file.is_open()) gamelog_file.open("gamelog.txt",std::ios::out|std::ios::app);

  std::osyncstream fseed(gamelog_file);

  fseed<<str.c_str()<<std::endl;
}

void errorlog_string(const char *ptr){

  if(ptr == NULL)
    return;

  //SAVE AN ERROR TO THE LOG FILE
  std::osyncstream fseed(error_file);
  if(!errorlog_prefix.empty())
  {
    fseed<<errorlog_prefix.c_str()<<std::endl;
    errorlog_prefix.clear();
  }
  fseed<<ptr<<std::endl;
}