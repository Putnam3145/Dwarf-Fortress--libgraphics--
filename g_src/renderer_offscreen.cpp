#include "renderer_2d.hpp"


bool renderer_offscreen::init_video(int w, int h) {
  // Create an offscreen buffer
  return true;
}

renderer_offscreen::~renderer_offscreen() {
  //ASSUMES renderer_offscreen IS NEVER gps_allocate()'d THROUGH reshape()/grid_resize()
		//to-do: flag for those calls on the renderer to control this behavior?
  	//*************************** OFFSCREEN RENDER
		//will probably need to watch out for this behavior vs. viewports
  renderer::screen = NULL;
  renderer::screentexpos = NULL;
  renderer::screentexpos_lower = NULL;
  renderer::screentexpos_anchored = NULL;
  renderer::screentexpos_anchored_x = NULL;
  renderer::screentexpos_anchored_y = NULL;
  renderer::screentexpos_flag = NULL;
  renderer::screen_old = NULL;
  renderer::screentexpos_old = NULL;
  renderer::screentexpos_lower_old = NULL;
  renderer::screentexpos_anchored_old = NULL;
  renderer::screentexpos_anchored_x_old = NULL;
  renderer::screentexpos_anchored_y_old = NULL;
  renderer::screentexpos_flag_old = NULL;

  renderer::screen_top = NULL;
  renderer::screentexpos_top = NULL;
  renderer::screentexpos_top_lower = NULL;
  renderer::screentexpos_top_anchored = NULL;
  renderer::screentexpos_top_anchored_x = NULL;
  renderer::screentexpos_top_anchored_y = NULL;
  renderer::screentexpos_top_flag = NULL;
  renderer::screen_top_old = NULL;
  renderer::screentexpos_top_old = NULL;
  renderer::screentexpos_top_lower_old = NULL;
  renderer::screentexpos_top_anchored_old = NULL;
  renderer::screentexpos_top_anchored_x_old = NULL;
  renderer::screentexpos_top_anchored_y_old = NULL;
  renderer::screentexpos_top_flag_old = NULL;

  renderer::screentexpos_refresh_buffer = NULL;

}

// Create an offscreen renderer of a given grid-size
renderer_offscreen::renderer_offscreen(int grid_x, int grid_y) {
  screen = NULL;
  dispx = enabler.is_fullscreen() ?
    init.font.large_font_dispx :
    init.font.small_font_dispx;
  dispy = enabler.is_fullscreen() ?
    init.font.large_font_dispy :
    init.font.small_font_dispy;
  dispx_z = dispx;
  dispy_z = dispy;
  origin_x = origin_y = 0;
  zoom_steps = forced_steps = 0;
  natural_w = dispx * grid_x;
  natural_h = dispy * grid_y;
  dimx = grid_x;
  dimy = grid_y;
  init_video(natural_w, natural_h);
  // Copy the GPS pointers here
  renderer::screen = gps.screen;
  renderer::screentexpos = gps.screentexpos;
  renderer::screentexpos_lower = gps.screentexpos_lower;
  renderer::screentexpos_anchored = gps.screentexpos_anchored;
  renderer::screentexpos_anchored_x = gps.screentexpos_anchored_x;
  renderer::screentexpos_anchored_y = gps.screentexpos_anchored_y;
  renderer::screentexpos_flag = gps.screentexpos_flag;
  renderer::screen_top = gps.screen_top;
  renderer::screentexpos_top = gps.screentexpos_top;
  renderer::screentexpos_top_lower = gps.screentexpos_top_lower;
  renderer::screentexpos_top_anchored = gps.screentexpos_top_anchored;
  renderer::screentexpos_top_anchored_x = gps.screentexpos_top_anchored_x;
  renderer::screentexpos_top_anchored_y = gps.screentexpos_top_anchored_y;
  renderer::screentexpos_top_flag = gps.screentexpos_top_flag;
  renderer::screentexpos_refresh_buffer = gps.screentexpos_refresh_buffer;
}

// Slurp the entire gps content into the renderer at some given offset
void renderer_offscreen::update_all(int offset_x, int offset_y) {
  for (int x = 0; x < gps.dimx; x++) {
    for (int y = 0; y < gps.dimy; y++) {
      // Read tiles from gps, create cached texture
      Either<texture_fullid,int32_t/*texture_ttfid*/> id = screen_to_texid(x, y);
      SDL_Texture *tex = //id.isL ?
        tile_cache_lookup(id.left) /*:
        ttf_manager.get_texture(id.right)*/;
      if (id.isL) {
  		id.left.flag|=TEXTURE_FULLID_FLAG_DO_RECOLOR;
        id.left.flag|=TEXTURE_FULLID_FLAG_CONVERT;
        tex = tile_cache_lookup(id.left);
      }/* else {
        tex = enabler.textures.get_texture_data(id.right);
      }*/
      // Figure out where to blit
      SDL_Rect dst;
      dst.x = dispx * (x+offset_x);
      dst.y = dispy * (y+offset_y);
      // And blit.
      //if(tex!=NULL)SDL_RenderCopy(renderer_sdl, tex, NULL, &dst);
    }
  }
}

// Save the image to some file
void renderer_offscreen::save_to_file(const string &file) {
  // TODO: Support png, etc.
}
