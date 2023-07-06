#ifndef DF_GSRC_TEXTURE_HANDLER_H
#define DF_GSRC_TEXTURE_HANDLER_H

#include "../platform/types.hpp"
#include "../util/svector.hpp"
#include "../text/stringvec.hpp"
#include "../text/textlines.hpp"

struct palette_pagest {

  std::string token;
  std::string graphics_dir;
  std::string filename;

  int32_t default_row;
  stringvectst color_token;
  svector<int32_t> color_row;//linked

  palette_pagest(){
    default_row=-1;
  }

};

struct tile_pagest {

  std::string token;
  std::string graphics_dir;

  std::string filename;
  short tile_dim_x;
  short tile_dim_y;
  short page_dim_x;
  short page_dim_y;

  svector<long> texpos;
  svector<long> datapos;
  svector<long> texpos_gs;
  svector<long> datapos_gs;

  char loaded;

  tile_pagest(){
    loaded=0;
  }

  void load_graphics();
  void refresh_graphics();

};

class texture_handlerst {
public:

  svector<tile_pagest*> page;
  svector<palette_pagest*> palette;

  ~texture_handlerst(){
    clean();
  }

  void clean();
  void adopt_new_lines(textlinesst& lines, const std::string& graphics_dir);

  tile_pagest *get_tile_page_by_token(std::string &tk){
    for(int32_t t = 0; t < page.size(); t++){
      if(page[t]->token == tk)
        return page[t];
    }
    return NULL;
  }

  palette_pagest *get_palette_page_by_token(std::string &tk){
    for(int32_t t = 0; t < palette.size(); t++){
      if(palette[t]->token == tk)
        return palette[t];
    }
    return NULL;
  }

};

#endif // DF_GSRC_TEXTURE_HANDLER_H
