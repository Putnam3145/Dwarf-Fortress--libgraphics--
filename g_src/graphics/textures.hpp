#ifndef DF_GSRC_TEXTURES_HPP
#define DF_GSRC_TEXTURES_HPP

/*
This is a texture catalog interface,
with opengl, sdl and truetype support.
*/

#include "../platform/types.hpp"
#include "../util/svector.hpp"

#ifdef __APPLE__
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include <string>
#include <variant>
#include <vector>

#include "GL/glew.h"

struct vsize_pos {
  int h, w;
  SDL_Surface* s;
  long pos;
  int x, y; // Assigned texture-catalog coordinates

  bool operator<(struct vsize_pos y) const {
    // sort produces an ascending order. We want descending. Don't argue.
    if (h > y.h)
      return true;
    return false;
  }
};

class cached_texturest {
  int w, h;
  std::variant<SDL_Texture*, SDL_Surface*> tex;

public:
  cached_texturest();
  cached_texturest(SDL_Surface* s);
  ~cached_texturest();
  SDL_Texture* get_texture();

  void get_size(int& out_w, int& out_h) {
    out_w = w;
    out_h = h;
  }
};

class textures {
  friend class enablerst;
  friend class renderer_opengl;

private:
  std::vector<SDL_Surface*> raws;
  std::vector<int32_t> free_spaces;
  int32_t init_texture_size;
  bool uploaded;
  long add_texture(SDL_Surface*);

protected:
  GLuint gl_catalog;           // texture catalog gennum
  struct gl_texpos* gl_texpos; // Texture positions in the GL catalog, if any

public:
  textures() {
    uploaded = false; // Stateful Variables
    gl_texpos = NULL;
  }

  ~textures() {
    for (auto it = raws.cbegin(); it != raws.cend(); ++it)
      SDL_FreeSurface(*it);
  }

  int textureCount() { return (int)raws.size(); }

  // opengl textures were here--they're in the git history, circa february 1
  // 2023 Returns the most recent texture data
  SDL_Surface* get_texture_data(long pos);

  // create a texture
  long create_texture(long dimx, long dimy);

  void set_init_texture_size() { init_texture_size = (int32_t)raws.size(); }

  // Clone a texture
  long clone_texture(long src);
  // Remove all color, but not transparency
  void grayscale_texture(long pos);
  // Loads dimx*dimy textures from a file, assuming all tiles
  // are equally large and arranged in a grid
  // Texture positions are saved in row-major order to tex_pos
  // If convert_magenta is true and the file does not have built-in
  // transparency, any magenta (255,0,255 RGB) is converted to full transparency
  // The calculated size of individual tiles is saved to disp_x, disp_y
  void load_multi_pdim(const std::string& filename, svector<long>& tex_pos,
                       long dimx, long dimy, bool convert_magenta, long* disp_x,
                       long* disp_y);
  void load_multi_pdim(const std::string& filename, long* tex_pos, long dimx,
                       long dimy, bool convert_magenta, long* disp_x,
                       long* disp_y);
  void refresh_multi_pdim(const std::string& filename, svector<long>& tex_pos,
                          long dimx, long dimy, bool convert_magenta);
  // Loads a single texture from a file, returning the handle
  cached_texturest load(const std::string& filename, bool convert_magenta);
  // To delete a texture..
  void delete_texture(int32_t pos);
  void delete_texture(SDL_Surface* srf);

  void delete_all_post_init_textures();
};

#endif // DF_GSRC_TEXTURES_HPP
