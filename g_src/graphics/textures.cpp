#include "textures.hpp"
#include "../render/init.hpp"

cached_texturest::cached_texturest() {
  w = -1;
  h = -1;
  tex = (SDL_Surface*)NULL;
}

cached_texturest::cached_texturest(SDL_Surface* surf) {
  w = surf->w;
  h = surf->h;
  if (enabler.main_renderer()) {
    tex = SDL_CreateTextureFromSurface(enabler.main_renderer(), surf);
  }
  else {
    tex = surf;
  }
}

cached_texturest::~cached_texturest() {
  if(auto actual_tex = std::get_if<SDL_Texture*>(&tex))
    SDL_DestroyTexture(*actual_tex);
}

SDL_Texture* cached_texturest::get_texture() {
  if (std::holds_alternative<SDL_Surface*>(tex)) {
    if (enabler.main_renderer()) {
      tex = SDL_CreateTextureFromSurface(enabler.main_renderer(), std::get<SDL_Surface*>(tex));
    }
    else {
      return NULL;
    }
  }
  return std::get<SDL_Texture*>(tex);
}

SDL_Surface *textures::get_texture_data(long pos) {
  if (raws.size() > pos) {
    return raws[pos];
  } else {
  //this causes memory hard crash
    /*
    std::cerr << "Asked for nonexistent texture data\n";
    SDL_Surface *surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 8, 32, 0, 0, 0, 0);
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 0, 255));
    raws.resize(pos+1);
    raws[pos] = surf;
    return raws[pos];
  */
    return NULL;
  }
}

long textures::create_texture(long dimx,long dimy)
{
  Uint32 rmask = 0x000000ff,
  gmask = 0x0000ff00,
  bmask = 0x00ff0000,
  amask = 0xff000000;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
  #endif

  SDL_Surface *tile=SDL_CreateRGBSurface(SDL_SWSURFACE,dimx,dimy,32,rmask,gmask,bmask,amask);
  SDL_SetSurfaceAlphaMod(tile, 255);
  return enabler.textures.add_texture(tile);
}

long textures::clone_texture(long src) {
  long tx;
  
  if (raws.size() > src && raws[src]) {
    SDL_Surface *dst = SDL_ConvertSurface(raws[src], raws[src]->format, SDL_SWSURFACE);
    tx=add_texture(dst);
  }
  else {
  // Okay, we've been asked to clone a nonexistent texture. Riight...
  // Fortunately add_texture doesn't look at the pointer it gets at all.
    std::cerr << "Asked to clone nonexistent texture!\n";
    tx=add_texture(NULL);
  }

  enabler.reset_textures();

  return tx;
}

void textures::grayscale_texture(long pos) {
 SDL_Surface *s = get_texture_data(pos);
 if (!s) return;
 SDL_LockSurface(s);
 SDL_PixelFormat *f = s->format;
 Uint32 *pixels = (Uint32*)(s->pixels);
 if (f->BytesPerPixel != 4) {
   std::cerr << "grayscale_texture ran into mysteriously uncanonicalized texture\n";
   goto cleanup;
 }
 for (int i=0; i < s->w*s->h; i++) { // For each pixel
   int r = (pixels[i] & f->Rmask) >> f->Rshift;
   int g = (pixels[i] & f->Gmask) >> f->Gshift;
   int b = (pixels[i] & f->Bmask) >> f->Bshift;
   int alpha = (pixels[i] & f->Amask) >> f->Ashift;
   int luminosity=(int)((float)r*0.30f+(float)g*0.59f+(float)b*0.11f);
   if(luminosity<0)luminosity=0;
   if(luminosity>255)luminosity=255;
   pixels[i] = (luminosity << f->Rshift) |
   (luminosity << f->Gshift) |
   (luminosity << f->Bshift) |
   (alpha << f->Ashift);
 }

 cleanup:
 SDL_UnlockSurface(s);

 enabler.reset_textures();
}

// Converts an arbitrary Surface to something like the display format
// (32-bit RGBA), and converts magenta to transparency if convert_magenta is set
// and the source surface didn't already have an alpha channel.
// It also deletes the source surface.
//
// It uses the same pixel format (RGBA, R at lowest address) regardless of
// hardware.
SDL_Surface *canonicalize_format(SDL_Surface *src, bool convert_magenta) {
  SDL_PixelFormat fmt;
  fmt.palette = NULL;
  fmt.BitsPerPixel = 32;
  fmt.BytesPerPixel = 4;
  fmt.Rloss = fmt.Gloss = fmt.Bloss = fmt.Aloss = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  fmt.Rshift = 24; fmt.Gshift = 16; fmt.Bshift = 8; fmt.Ashift = 0;
#else
  fmt.Rshift = 0; fmt.Gshift = 8; fmt.Bshift = 16; fmt.Ashift = 24;
#endif
  fmt.Rmask = 255 << fmt.Rshift;
  fmt.Gmask = 255 << fmt.Gshift;
  fmt.Bmask = 255 << fmt.Bshift;
  fmt.Amask = 255 << fmt.Ashift;

  //************************************* ALWAYS CONVERT MAGENTA
  //this is to fix some hybrid files
  if (/*src->format->Amask == 0 && */convert_magenta) { // No alpha
  SDL_SetColorKey(src, SDL_TRUE,
    SDL_MapRGB(src->format, 255, 0, 255));
}
SDL_Surface *tgt = SDL_ConvertSurface(src, &fmt, SDL_SWSURFACE);
SDL_FreeSurface(src);
for (size_t x = 0; x < tgt->w; ++x) {
  for (size_t y = 0; y < tgt->h; ++y) {
    Uint8* p = (Uint8*)tgt->pixels + y * tgt->pitch + x * 4;
    if (p[3] == 0) {
      for (int c = 0; c < 3; c++) {
        p[c] = 0;
      }
    }
  }
}
return tgt;
}

// Finds or creates a free spot in the texture array, and inserts
// surface in that spot, then returns the location.
long textures::add_texture(SDL_Surface *surface) {
  if (free_spaces.empty())
  {
        // No free spot, make one
    raws.push_back(surface);
    return (int32_t)(raws.size()) - 1;
  }
  else
  {
        // We have a free spot, just put it in there
    int sz = free_spaces.back();
    raws[sz] = surface;
    free_spaces.pop_back();
    return sz;
  }
}

void textures::load_multi_pdim(const string &filename, long *tex_pos, long dimx,
 long dimy, bool convert_magenta,
 long *disp_x, long *disp_y) {
  SDL_Surface *raw = IMG_Load(filename.c_str());
  if (!raw) {
    MessageBox(NULL, ("Not found: " + filename).c_str(), "Tileset not found", MB_OK);
    exit(1);
  }
  SDL_Surface *src = canonicalize_format(raw, convert_magenta);
  SDL_SetSurfaceBlendMode(src,SDL_BLENDMODE_NONE);
  *disp_x = src->w / dimx;
  *disp_y = src->h / dimy;
  long idx = 0;
  for (int y=0; y < dimy; y++) {
    for (int x=0; x < dimx; x++,++idx) {
      SDL_Surface *tile = SDL_CreateRGBSurface(SDL_SWSURFACE, *disp_x, *disp_y,
       32, src->format->Rmask,
       src->format->Gmask,
       src->format->Bmask,
       src->format->Amask);
      SDL_SetSurfaceAlphaMod(tile, 255);
      SDL_Rect pos_src;
      pos_src.x = *disp_x * x;
      pos_src.y = *disp_y * y;
      pos_src.w =  *disp_x;
      pos_src.h =  *disp_y;
      SDL_BlitSurface(src, &pos_src, tile, NULL);
      tex_pos[idx] = add_texture(tile);
    }
  }
  SDL_FreeSurface(src);
  // Re-upload textures if necessary
  enabler.reset_textures();
}

void textures::load_multi_pdim(const string &filename, svector<long> &tex_pos, long dimx,
 long dimy, bool convert_magenta,
 long *disp_x, long *disp_y) {
  SDL_Surface *raw = IMG_Load(filename.c_str());
  if (!raw) {
    MessageBox(NULL, ("Not found: " + filename).c_str(), "Tileset not found", MB_OK);
    exit(1);
  }
  tex_pos.resize(dimx*dimy);
  auto tp_s = tex_pos.begin();
  SDL_Surface *src = canonicalize_format(raw, convert_magenta);
  SDL_SetSurfaceBlendMode(src,SDL_BLENDMODE_NONE);
  *disp_x = src->w / dimx;
  *disp_y = src->h / dimy;
  long idx = 0;
  for (int y=0; y < dimy; y++) {
    for (int x=0; x < dimx; x++,++tp_s) {
      SDL_Surface *tile = SDL_CreateRGBSurface(SDL_SWSURFACE, *disp_x, *disp_y,
       32, src->format->Rmask,
       src->format->Gmask,
       src->format->Bmask,
       src->format->Amask);
      SDL_SetSurfaceAlphaMod(tile, 255);
      SDL_Rect pos_src;
      pos_src.x = *disp_x * x;
      pos_src.y = *disp_y * y;
      pos_src.w =  *disp_x;
      pos_src.h =  *disp_y;
      SDL_BlitSurface(src, &pos_src, tile, NULL);
      *tp_s = add_texture(tile);
    }
  }
  SDL_FreeSurface(src);
  // Re-upload textures if necessary
  enabler.reset_textures();
}

void textures::refresh_multi_pdim(const string &filename, svector<long> &tex_pos, long dimx,
 long dimy, bool convert_magenta) {
  SDL_Surface *raw = IMG_Load(filename.c_str());
  if (!raw) {
    MessageBox(NULL, ("Not found: " + filename).c_str(), "Tileset not found", MB_OK);
    exit(1);
  }

  long ind=0;

  SDL_Surface *src = canonicalize_format(raw, convert_magenta);
  SDL_SetSurfaceBlendMode(src,SDL_BLENDMODE_NONE);
  long disp_x = src->w / dimx;
  long disp_y = src->h / dimy;
  long idx = 0;
  for (int y=0; y < dimy; y++) {
    for (int x=0; x < dimx; x++, ++ind) {
      SDL_Surface *tile = enabler.textures.get_texture_data(tex_pos[ind]);
      if(tile==NULL)continue;

      SDL_SetSurfaceAlphaMod(tile, 255);
      SDL_Rect pos_src;
      pos_src.x = disp_x * x;
      pos_src.y = disp_y * y;
      pos_src.w =  disp_x;
      pos_src.h =  disp_y;
      SDL_BlitSurface(src, &pos_src, tile, NULL);
    }
  }
  SDL_FreeSurface(src);
  // Re-upload textures if necessary
  enabler.reset_textures();
}

cached_texturest textures::load(const string &filename, bool convert_magenta) {
  SDL_Surface *raw = IMG_Load(filename.c_str());
  if (!raw) {
    MessageBox(NULL, ("Not found: " + filename).c_str(), "Image not found", MB_OK);
    exit(1);
  }
  SDL_Surface *tex = canonicalize_format(raw, convert_magenta);
  return cached_texturest(tex);
}

void textures::delete_all_post_init_textures()
{
 int32_t sz=(int32_t)raws.size();
 int32_t i;
 free_spaces.clear();
 for(i=init_texture_size;i<sz;++i)
 {
  if(raws[i]!=NULL)
  {
    SDL_FreeSurface(raws[i]);
    raws[i]=NULL;
    free_spaces.push_back(i);
  }
}
}

void textures::delete_texture(SDL_Surface *srf)
{
  int32_t sz=(int32_t)raws.size();
  int32_t i;
  for(i=sz-1;i>=0;--i)
  {
    if(raws[i]==srf)
    {
      raws[i]=NULL;
      free_spaces.push_back(i);
      break;
    }
  }
  SDL_FreeSurface(srf);
}

void textures::delete_texture(int32_t pos) {
  // We can't actually resize the array, as
  // (a) it'd be slow, and
  // (b) it'd change all the positions. Bad stuff would happen.
  if (raws[pos]) {
    SDL_FreeSurface(raws[pos]);
    raws[pos] = NULL;
    free_spaces.push_back(pos);
  }
}
