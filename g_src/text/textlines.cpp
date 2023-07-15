#include "../text/textlines.hpp"

#include <fstream>

void textlinesst::load_raw_to_lines(const char* filename) {
  clean_lines();

  std::ifstream fseed(filename); // Load the File
  if (fseed.is_open()) {
    std::string str;

    while (std::getline(fseed, str)) {
      long end = (long)str.length();

      while (end > 0) {
        if (isspace(str[end - 1]))
          end--;
        else
          break;
      }

      str.resize(end);
      if (str.length() > 0)
        text.add_string(str);
    }
  }

  fseed.close();
}