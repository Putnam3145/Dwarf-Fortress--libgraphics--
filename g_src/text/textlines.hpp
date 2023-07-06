#ifndef DF_GSRC_TEXT_TEXTLINES_HPP
#define DF_GSRC_TEXT_TEXTLINES_HPP

struct textlinesst {

  stringvectst text;

  void load_raw_to_lines(const char* filename);

  void clean_lines(){
    text.clean();
  }

  void copy_from(textlinesst& ot){
    text.copy_from(ot.text);
  }

};

#endif // DF_GSRC_TEXT_TEXTLINES_HPP