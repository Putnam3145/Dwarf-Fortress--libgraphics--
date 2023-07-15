#ifndef DF_GSRC_UTIL_COMMANDLINE_HPP
#define DF_GSRC_UTIL_COMMANDLINE_HPP

#include <string>

#include "../text/stringvec.hpp"

struct command_linest {
  std::string original;
  stringvectst arg_vect;

  long gen_id;
  unsigned long world_seed;
  char use_seed;
  std::string world_param;
  char use_param;

  void init(const std::string& str);
  char grab_arg(std::string& source, long& pos);
  void handle_arg(std::string& arg);

  command_linest() {
    gen_id = -1;
    use_seed = 0;
    use_param = 0;
  }
};

#endif // DF_GSRC_UTIL_COMMANDLINE_HPP