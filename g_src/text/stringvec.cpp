#include "stringvec.hpp"

void stringvectst::add_string(const std::string& st) {
  pstringst* newp = new pstringst;
  newp->dat = st;
  str.push_back(newp);
}

long stringvectst::add_unique_string(const std::string& st) {
  long i;
  for (i = (long)str.size() - 1; i >= 0; i--) {
    if (str[i]->dat == st)
      return i;
  }
  add_string(st);
  return (long)str.size() - 1;
}

void stringvectst::add_string(const char* st) {
  if (st != NULL) {
    pstringst* newp = new pstringst;
    newp->dat = st;
    str.push_back(newp);
  }
}

void stringvectst::insert_string(long k, const std::string& st) {
  pstringst* newp = new pstringst;
  newp->dat = st;
  if (str.size() > k)
    str.insert(k, newp);
  else
    str.push_back(newp);
}

void stringvectst::clean() {
  while (str.size() > 0) {
    delete str[0];
    str.erase(0);
  }
}

void stringvectst::read_file(file_compressorst& filecomp, long loadversion) {
  int32_t dummy;
  filecomp.read_file(dummy);
  str.resize(dummy);

  long s;
  for (s = 0; s < dummy; s++) {
    str[s] = new pstringst;
    filecomp.read_file(str[s]->dat);
  }
}

void stringvectst::write_file(file_compressorst& filecomp) {
  int32_t dummy = (int32_t)str.size();
  filecomp.write_file(dummy);

  long s;
  for (s = 0; s < dummy; s++) {
    filecomp.write_file(str[s]->dat);
  }
}

void stringvectst::copy_from(const stringvectst& src) {
  clean();

  str.resize(src.str.size());

  long s;
  for (s = (long)src.str.size() - 1; s >= 0; s--) {
    str[s] = new pstringst;
    str[s]->dat = src.str[s]->dat;
  }
}

bool stringvectst::has_string(const std::string& st) {
  long i;
  for (i = (long)str.size() - 1; i >= 0; i--) {
    if (str[i]->dat == st)
      return true;
  }
  return false;
}

void stringvectst::remove_string(const std::string& st) {
  long i;
  for (i = (long)str.size() - 1; i >= 0; i--) {
    if (str[i]->dat == st) {
      delete str[i];
      str.erase(i);
    }
  }
}