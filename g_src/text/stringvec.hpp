#ifndef DF_GSRC_TEXT_STRINGVEC_HPP
#define DF_GSRC_TEXT_STRINGVEC_HPP

#include <string>

#include "../files/file_compressor.hpp"
#include "../util/svector.hpp"

struct pstringst {
  std::string dat;
};

class stringvectst {
public:
  svector<pstringst*> str;

  ~stringvectst() { clean(); }

  void operator=(stringvectst& two);

  void clean();

  void add_string(const std::string& st);
  long add_unique_string(const std::string& st);
  void add_string(const char* st);
  void insert_string(long k, const std::string& st);
  void copy_from(const stringvectst& src);
  bool has_string(const std::string& st);
  void remove_string(const std::string& st);

  void read_file(file_compressorst& filecomp, long loadversion);
  void write_file(file_compressorst& filecomp);
};

#endif // DF_GSRC_TEXT_STRINGVEC_HPP
