#include <cassert>

#include "platform.h"
#include "enabler.h"
#include "random.h"
#include "init.h"
#include "music_and_sound_g.h"
#include "dfhooks.h"
#ifdef WIN32
#include "glaiel/crashlogs.h"
#endif
#ifndef NO_FMOD
extern musicsoundst musicsound;
extern adv_music_statest adv_music_state;
#endif

#ifdef unix
# include <locale.h>
#endif

using namespace std;

enablerst enabler;

// For the printGLError macro
int glerrorcount = 0;

// Set to 0 when the game wants to quit
static int loopvar = 1;

// Reports an error to the user, using a MessageBox and stderr.
void report_error(const char *error_preface, const char *error_message)
{
  char *buf = NULL;
  // +4 = +colon +space +newline +nul
  buf = new char[strlen(error_preface) + strlen(error_message) + 4];
  sprintf(buf, "%s: %s\n", error_preface, error_message);
  MessageBox(NULL, buf, "Error", MB_OK);
  fprintf(stderr, "%s", buf);
  delete [] buf;
}

Either<texture_fullid,int32_t/*texture_ttfid*/> renderer::screen_to_texid(int x, int y) {
  const int tile = x * gps.dimy + y;
  const unsigned char *s = screen + tile*8;

  struct texture_fullid ret;
  int ch;

  // TTF text does not get the full treatment.
  /*
  if (s[7] == GRAPHICSTYPE_TTF) {
    texture_ttfid texpos = *((unsigned int *)s) & 0xffffff;
    return Either<texture_fullid,texture_ttfid>(texpos);
  } else if (s[7] == GRAPHICSTYPE_TTFCONT) {
    // TTFCONT means this is a tile that does not have TTF anchored on it, but is covered by TTF.
    // Since this may actually be stale information, we'll draw it as a blank space,
    ch = 32;
  } else */{
    // Otherwise, it's a normal (graphical?) tile.
    ch   = s[0];
  }
  
	//removed static here because it literally doesn't recognize that this flag has changed otherwise
		//that is, use_graphics and init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS) return different values
  bool use_graphics = init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
  
	const uint32_t stp_flag	      = screentexpos_flag[tile];

  if (use_graphics) {
    const long texpos             = screentexpos[tile];

    if (texpos) {
      ret.texpos = texpos;
      if (stp_flag & SCREENTEXPOS_FLAG_GRAYSCALE) {
        ret.r = (float)s[1]/255.0f;
        ret.g = (float)s[2]/255.0f;
        ret.b = (float)s[3]/255.0f;
        ret.br = (float)s[4]/255.0f;
        ret.bg = (float)s[5]/255.0f;
        ret.bb = (float)s[6]/255.0f;
      } else if (stp_flag & SCREENTEXPOS_FLAG_ADDCOLOR) {
        goto use_ch;
      } else {
        ret.r = ret.g = ret.b = 1;
        ret.br = ret.bg = ret.bb = 0;
      }
      goto skip_ch;
    }
  }
  
	if(enabler.flag & ENABLERFLAG_BASIC_TEXT)
		{
		if(stp_flag & SCREENTEXPOS_FLAG_TOP_OF_TEXT)ret.texpos=init.font.basic_font_texpos_top[ch];
		else if(stp_flag & SCREENTEXPOS_FLAG_BOTTOM_OF_TEXT)ret.texpos=init.font.basic_font_texpos_bot[ch];
		else ret.texpos=init.font.basic_font_texpos[ch];
		}
	else if(stp_flag & SCREENTEXPOS_FLAG_TOP_OF_TEXT)
		{
		ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos_top[ch]:init.font.small_font_texpos_top[ch];
		}
	else if(stp_flag & SCREENTEXPOS_FLAG_BOTTOM_OF_TEXT)
		{
		ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos_bot[ch]:init.font.small_font_texpos_bot[ch];
		}
	else ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos[ch]:init.font.small_font_texpos[ch];

 use_ch:
    ret.r = (float)s[1]/255.0f;
    ret.g = (float)s[2]/255.0f;
    ret.b = (float)s[3]/255.0f;
    ret.br = (float)s[4]/255.0f;
    ret.bg = (float)s[5]/255.0f;
    ret.bb = (float)s[6]/255.0f;

 skip_ch:

  return Either<texture_fullid,int32_t/*texture_ttfid*/>(ret);
}

Either<texture_fullid,int32_t/*texture_ttfid*/> renderer::screen_top_to_texid(int x, int y) {
  const int tile = x * gps.dimy + y;
  const unsigned char *s = screen_top + tile*8;

  struct texture_fullid ret;
  int ch;

  // TTF text does not get the full treatment.
/*  if (s[7] == GRAPHICSTYPE_TTF) {
    texture_ttfid texpos = *((unsigned int *)s) & 0xffffff;
    return Either<texture_fullid,texture_ttfid>(texpos);
  } else if (s[7] == GRAPHICSTYPE_TTFCONT) {
    // TTFCONT means this is a tile that does not have TTF anchored on it, but is covered by TTF.
    // Since this may actually be stale information, we'll draw it as a blank space,
    ch = 32;
  } else */{
    // Otherwise, it's a normal (graphical?) tile.
    ch   = s[0];
  }
  
	//removed static here because it literally doesn't recognize that this flag has changed otherwise
		//that is, use_graphics and init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS) return different values
  bool use_graphics = init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
  
	const uint32_t stp_flag	      = screentexpos_top_flag[tile];

  if (use_graphics) {
    const long texpos             = screentexpos_top[tile];

    if (texpos) {
      ret.texpos = texpos;
      if (stp_flag & SCREENTEXPOS_FLAG_GRAYSCALE) {
        ret.r = (float)s[1]/255.0f;
        ret.g = (float)s[2]/255.0f;
        ret.b = (float)s[3]/255.0f;
        ret.br = (float)s[4]/255.0f;
        ret.bg = (float)s[5]/255.0f;
        ret.bb = (float)s[6]/255.0f;
      } else if (stp_flag & SCREENTEXPOS_FLAG_ADDCOLOR) {
        goto use_ch;
      } else {
        ret.r = ret.g = ret.b = 1;
        ret.br = ret.bg = ret.bb = 0;
      }
      goto skip_ch;
    }
  }
  
	if(enabler.flag & ENABLERFLAG_BASIC_TEXT)
		{
		ret.texpos=init.font.basic_font_texpos[ch];
		}
	else if(stp_flag & SCREENTEXPOS_FLAG_TOP_OF_TEXT)
		{
		ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos_top[ch]:init.font.small_font_texpos_top[ch];
		}
	else if(stp_flag & SCREENTEXPOS_FLAG_BOTTOM_OF_TEXT)
		{
		ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos_bot[ch]:init.font.small_font_texpos_bot[ch];
		}
	else ret.texpos=enabler.is_fullscreen()?init.font.large_font_texpos[ch]:init.font.small_font_texpos[ch];

 use_ch:
    ret.r = (float)s[1]/255.0f;
    ret.g = (float)s[2]/255.0f;
    ret.b = (float)s[3]/255.0f;
    ret.br = (float)s[4]/255.0f;
    ret.bg = (float)s[5]/255.0f;
    ret.bb = (float)s[6]/255.0f;

 skip_ch:

  return Either<texture_fullid,int32_t/*texture_ttfid*/>(ret);
}

#ifdef CURSES
# include "renderer_curses.cpp"
#endif
#include "renderer_2d.hpp"
//#include "renderer_opengl.hpp"

#ifndef FULL_RELEASE_VERSION
bool cinematic_mode=false;
int32_t cinematic_shift_x=0;
int32_t cinematic_shift_y=0;
int32_t cinematic_shift_dx=0;
int32_t cinematic_shift_dy=0;
int32_t cinematic_shift_velx=0;
int32_t cinematic_shift_vely=0;
int32_t cinematic_start_scrollx=0;
int32_t cinematic_start_scrolly=0;
#endif

enablerst::enablerst() : async_fromcomplete(false) {
  must_do_render_things_before_display=false;
  fullscreen_state = 0;
  renderer = NULL;
  calculated_fps = calculated_gfps = frame_sum = gframe_sum = frame_last = gframe_last = 0;
  fps = 100; gfps = 20;
  fps_per_gfps = fps / gfps;
  last_tick = 0;
}

void renderer::display()
{
	++gps.refresh_buffer_val;
	if (gps.refresh_buffer_val>100000000)
		{
		gps.refresh_buffer_val=0;
		if (gps.screentexpos_refresh_buffer!=NULL)memset(gps.screentexpos_refresh_buffer,0,sizeof(int32_t)*gps.dimx*gps.dimy);
		}

#ifndef FULL_RELEASE_VERSION
	if (cinematic_mode)gps.force_full_display_count=2;
#endif

	if (gps.force_full_display_count)
		{
		do_blank_screen_fill();

		int32_t lvp=LOWER_VIEWPORT_MAX-1;
		while (lvp>=0)
			{
			if (gps.lower_viewport[lvp]!=NULL)
				{
				if (gps.lower_viewport[lvp]->flag&GRAPHIC_VIEWPORT_FLAG_ACTIVE)
					{
					update_full_viewport(gps.lower_viewport[lvp]);
					}
				}
			--lvp;
			}
		if ((gps.main_viewport!=NULL)?(gps.main_viewport->flag&GRAPHIC_VIEWPORT_FLAG_ACTIVE):false)
			{
			update_full_viewport(gps.main_viewport);
			}

		if ((gps.main_map_port!=NULL)?(gps.main_map_port->flag&GRAPHIC_MAP_PORT_FLAG_ACTIVE):false)
			{
			update_full_map_port(gps.main_map_port);
			}
		}
	else
		{
		if ((gps.main_viewport!=NULL)?(gps.main_viewport->flag&GRAPHIC_VIEWPORT_FLAG_ACTIVE):false)
			{
			const int32_t dimx=gps.main_viewport->dim_x;
			const int32_t dimy=gps.main_viewport->dim_y;
			int32_t off=0,lvp;
			for (int32_t x2=0; x2<dimx; ++x2)
				{
				for (int32_t y2=0; y2<dimy; ++y2,++off)
					{
					bool refresh=false;

					if (gps.main_viewport->screentexpos[off]!=gps.main_viewport->screentexpos_old[off]||
						gps.main_viewport->screentexpos_background[off]!=gps.main_viewport->screentexpos_background_old[off]||
						gps.main_viewport->screentexpos_background_two[off]!=gps.main_viewport->screentexpos_background_two_old[off]||
						gps.main_viewport->screentexpos_floor_flag[off]!=gps.main_viewport->screentexpos_floor_flag_old[off]||
						gps.main_viewport->screentexpos_spatter_flag[off]!=gps.main_viewport->screentexpos_spatter_flag_old[off]||
						gps.main_viewport->screentexpos_spatter[off]!=gps.main_viewport->screentexpos_spatter_old[off]||
						gps.main_viewport->screentexpos_liquid_flag[off]!=gps.main_viewport->screentexpos_liquid_flag_old[off]||
						gps.main_viewport->screentexpos_ramp_flag[off]!=gps.main_viewport->screentexpos_ramp_flag_old[off]||
						gps.main_viewport->screentexpos_shadow_flag[off]!=gps.main_viewport->screentexpos_shadow_flag_old[off]||
						gps.main_viewport->screentexpos_building_one[off]!=gps.main_viewport->screentexpos_building_one_old[off]||
						gps.main_viewport->screentexpos_building_two[off]!=gps.main_viewport->screentexpos_building_two_old[off]||
						gps.main_viewport->screentexpos_top_shadow[off]!=gps.main_viewport->screentexpos_top_shadow_old[off]||
						gps.main_viewport->screentexpos_item[off]!=gps.main_viewport->screentexpos_item_old[off]||
						gps.main_viewport->screentexpos_vehicle[off]!=gps.main_viewport->screentexpos_vehicle_old[off]||
						gps.main_viewport->screentexpos_projectile[off]!=gps.main_viewport->screentexpos_projectile_old[off]||
						gps.main_viewport->screentexpos_high_flow[off]!=gps.main_viewport->screentexpos_high_flow_old[off]||
						gps.main_viewport->screentexpos_vermin[off]!=gps.main_viewport->screentexpos_vermin_old[off]||
						gps.main_viewport->screentexpos_signpost[off]!=gps.main_viewport->screentexpos_signpost_old[off]||
						gps.main_viewport->screentexpos_left_creature[off]!=gps.main_viewport->screentexpos_left_creature_old[off]||
						gps.main_viewport->screentexpos_right_creature[off]!=gps.main_viewport->screentexpos_right_creature_old[off]||
						gps.main_viewport->screentexpos_upleft_creature[off]!=gps.main_viewport->screentexpos_upleft_creature_old[off]||
						gps.main_viewport->screentexpos_up_creature[off]!=gps.main_viewport->screentexpos_up_creature_old[off]||
						gps.main_viewport->screentexpos_upright_creature[off]!=gps.main_viewport->screentexpos_upright_creature_old[off]||
						gps.main_viewport->screentexpos_designation[off]!=gps.main_viewport->screentexpos_designation_old[off]||
						gps.main_viewport->screentexpos_interface[off]!=gps.main_viewport->screentexpos_interface_old[off])refresh=true;
					if (!refresh)
						{
						lvp=0;
						while (lvp<LOWER_VIEWPORT_MAX)
							{
							graphic_viewportst *vp=gps.lower_viewport[lvp];
							if (vp==NULL)break;
							if (!(vp->flag&GRAPHIC_VIEWPORT_FLAG_ACTIVE))
								{
								++lvp;
								continue;
								}
							if (vp->screentexpos[off]!=vp->screentexpos_old[off]||
								vp->screentexpos_background[off]!=vp->screentexpos_background_old[off]||
								vp->screentexpos_background_two[off]!=vp->screentexpos_background_two_old[off]||
								vp->screentexpos_floor_flag[off]!=vp->screentexpos_floor_flag_old[off]||
								vp->screentexpos_spatter_flag[off]!=vp->screentexpos_spatter_flag_old[off]||
								vp->screentexpos_spatter[off]!=vp->screentexpos_spatter_old[off]||
								vp->screentexpos_liquid_flag[off]!=vp->screentexpos_liquid_flag_old[off]||
								vp->screentexpos_ramp_flag[off]!=vp->screentexpos_ramp_flag_old[off]||
								vp->screentexpos_shadow_flag[off]!=vp->screentexpos_shadow_flag_old[off]||
								vp->screentexpos_building_one[off]!=vp->screentexpos_building_one_old[off]||
								vp->screentexpos_building_two[off]!=vp->screentexpos_building_two_old[off]||
								vp->screentexpos_top_shadow[off]!=vp->screentexpos_top_shadow_old[off]||
								vp->screentexpos_item[off]!=vp->screentexpos_item_old[off]||
								vp->screentexpos_vehicle[off]!=vp->screentexpos_vehicle_old[off]||
								vp->screentexpos_projectile[off]!=vp->screentexpos_projectile_old[off]||
								vp->screentexpos_high_flow[off]!=vp->screentexpos_high_flow_old[off]||
								vp->screentexpos_vermin[off]!=vp->screentexpos_vermin_old[off]||
								vp->screentexpos_signpost[off]!=vp->screentexpos_signpost_old[off]||
								vp->screentexpos_left_creature[off]!=vp->screentexpos_left_creature_old[off]||
								vp->screentexpos_right_creature[off]!=vp->screentexpos_right_creature_old[off]||
								vp->screentexpos_upleft_creature[off]!=vp->screentexpos_upleft_creature_old[off]||
								vp->screentexpos_up_creature[off]!=vp->screentexpos_up_creature_old[off]||
								vp->screentexpos_upright_creature[off]!=vp->screentexpos_upright_creature_old[off]||
								vp->screentexpos_designation[off]!=vp->screentexpos_designation_old[off]||
								vp->screentexpos_interface[off]!=vp->screentexpos_interface_old[off])refresh=true;
							++lvp;
							if (refresh)break;
							}
						}

					if (!refresh)continue;

					lvp=LOWER_VIEWPORT_MAX-1;
					while (lvp>=0)
						{
						if (gps.lower_viewport[lvp]!=NULL)update_viewport_tile(gps.lower_viewport[lvp],x2,y2);
						--lvp;
						}
					update_viewport_tile(gps.main_viewport,x2,y2);
					}
				}
			}

		if ((gps.main_map_port!=NULL)?(gps.main_map_port->flag&GRAPHIC_MAP_PORT_FLAG_ACTIVE):false)
			{
			const int32_t dimx=gps.main_map_port->dim_x;
			const int32_t dimy=gps.main_map_port->dim_y;
			int32_t off=0;
			for (int32_t y2=0; y2<dimy; ++y2)
				{
				for (int32_t x2=0; x2<dimx; ++x2,++off)
					{
					bool refresh=false;
					if (gps.main_map_port->screentexpos_base[off]!=gps.main_map_port->screentexpos_base_old[off]||
						gps.main_map_port->screentexpos_edge[0][off]!=gps.main_map_port->screentexpos_edge_old[0][off]||
						gps.main_map_port->screentexpos_edge[1][off]!=gps.main_map_port->screentexpos_edge_old[1][off]||
						gps.main_map_port->screentexpos_edge[2][off]!=gps.main_map_port->screentexpos_edge_old[2][off]||
						gps.main_map_port->screentexpos_edge[3][off]!=gps.main_map_port->screentexpos_edge_old[3][off]||
						gps.main_map_port->screentexpos_edge[4][off]!=gps.main_map_port->screentexpos_edge_old[4][off]||
						gps.main_map_port->screentexpos_edge[5][off]!=gps.main_map_port->screentexpos_edge_old[5][off]||
						gps.main_map_port->screentexpos_edge[6][off]!=gps.main_map_port->screentexpos_edge_old[6][off]||
						gps.main_map_port->screentexpos_edge[7][off]!=gps.main_map_port->screentexpos_edge_old[7][off]||
						gps.main_map_port->screentexpos_edge2[0][off]!=gps.main_map_port->screentexpos_edge2_old[0][off]||
						gps.main_map_port->screentexpos_edge2[1][off]!=gps.main_map_port->screentexpos_edge2_old[1][off]||
						gps.main_map_port->screentexpos_edge2[2][off]!=gps.main_map_port->screentexpos_edge2_old[2][off]||
						gps.main_map_port->screentexpos_edge2[3][off]!=gps.main_map_port->screentexpos_edge2_old[3][off]||
						gps.main_map_port->screentexpos_edge2[4][off]!=gps.main_map_port->screentexpos_edge2_old[4][off]||
						gps.main_map_port->screentexpos_edge2[5][off]!=gps.main_map_port->screentexpos_edge2_old[5][off]||
						gps.main_map_port->screentexpos_edge2[6][off]!=gps.main_map_port->screentexpos_edge2_old[6][off]||
						gps.main_map_port->screentexpos_edge2[7][off]!=gps.main_map_port->screentexpos_edge2_old[7][off]||
						gps.main_map_port->screentexpos_detail[off]!=gps.main_map_port->screentexpos_detail_old[off]||
						gps.main_map_port->screentexpos_tunnel[off]!=gps.main_map_port->screentexpos_tunnel_old[off]||
						gps.main_map_port->screentexpos_river[off]!=gps.main_map_port->screentexpos_river_old[off]||
						gps.main_map_port->screentexpos_road[off]!=gps.main_map_port->screentexpos_road_old[off]||
						gps.main_map_port->screentexpos_site[off]!=gps.main_map_port->screentexpos_site_old[off]||
						gps.main_map_port->screentexpos_army[off]!=gps.main_map_port->screentexpos_army_old[off]||
						gps.main_map_port->screentexpos_interface[off]!=gps.main_map_port->screentexpos_interface_old[off]||
						gps.main_map_port->screentexpos_detail_to_n[off]!=gps.main_map_port->screentexpos_detail_to_n_old[off]||
						gps.main_map_port->screentexpos_detail_to_s[off]!=gps.main_map_port->screentexpos_detail_to_s_old[off]||
						gps.main_map_port->screentexpos_detail_to_w[off]!=gps.main_map_port->screentexpos_detail_to_w_old[off]||
						gps.main_map_port->screentexpos_detail_to_e[off]!=gps.main_map_port->screentexpos_detail_to_e_old[off]||
						gps.main_map_port->screentexpos_detail_to_nw[off]!=gps.main_map_port->screentexpos_detail_to_nw_old[off]||
						gps.main_map_port->screentexpos_detail_to_ne[off]!=gps.main_map_port->screentexpos_detail_to_ne_old[off]||
						gps.main_map_port->screentexpos_detail_to_sw[off]!=gps.main_map_port->screentexpos_detail_to_sw_old[off]||
						gps.main_map_port->screentexpos_detail_to_se[off]!=gps.main_map_port->screentexpos_detail_to_se_old[off]||
						gps.main_map_port->screentexpos_site_to_s[off]!=gps.main_map_port->screentexpos_site_to_s_old[off]||
						gps.main_map_port->screentexpos_cloud_bits[off]!=gps.main_map_port->screentexpos_cloud_bits_old[off])refresh=true;
					if (refresh)update_map_port_tile(gps.main_map_port,x2,y2);
					}
				}
			}
		}
	update_all();
}

void renderer::cleanup_arrays() {
  if (screen) delete[] screen;
  if (screentexpos) delete[] screentexpos;
  if (screentexpos_lower) delete[] screentexpos_lower;
  if (screentexpos_anchored) delete[] screentexpos_anchored;
  if (screentexpos_anchored_x) delete[] screentexpos_anchored_x;
  if (screentexpos_anchored_y) delete[] screentexpos_anchored_y;
  if (screentexpos_flag) delete[] screentexpos_flag;
  if (screen_old) delete[] screen_old;
  if (screentexpos_old) delete[] screentexpos_old;
  if (screentexpos_lower_old) delete[] screentexpos_lower_old;
  if (screentexpos_anchored_old) delete[] screentexpos_anchored_old;
  if (screentexpos_anchored_x_old) delete[] screentexpos_anchored_x_old;
  if (screentexpos_anchored_y_old) delete[] screentexpos_anchored_y_old;
  if (screentexpos_flag_old) delete[] screentexpos_flag_old;

  if (screen_top) delete[] screen_top;
  if (screentexpos_top) delete[] screentexpos_top;
  if (screentexpos_top_lower) delete[] screentexpos_top_lower;
  if (screentexpos_top_anchored) delete[] screentexpos_top_anchored;
  if (screentexpos_top_anchored_x) delete[] screentexpos_top_anchored_x;
  if (screentexpos_top_anchored_y) delete[] screentexpos_top_anchored_y;
  if (screentexpos_top_flag) delete[] screentexpos_top_flag;
  if (screen_top_old) delete[] screen_top_old;
  if (screentexpos_top_old) delete[] screentexpos_top_old;
  if (screentexpos_top_lower_old) delete[] screentexpos_top_lower_old;
  if (screentexpos_top_anchored_old) delete[] screentexpos_top_anchored_old;
  if (screentexpos_top_anchored_x_old) delete[] screentexpos_top_anchored_x_old;
  if (screentexpos_top_anchored_y_old) delete[] screentexpos_top_anchored_y_old;
  if (screentexpos_top_flag_old) delete[] screentexpos_top_flag_old;

  if(screentexpos_refresh_buffer)delete[] screentexpos_refresh_buffer;
}

void renderer::gps_allocate(int x, int y,int screen_x,int screen_y,int tile_dim_x,int tile_dim_y) {
  cleanup_arrays();
  
  gps.screen = screen = new unsigned char[x*y*8];
  memset(screen, 0, x*y*8);
  gps.screentexpos = screentexpos = new long[x*y];
  memset(screentexpos, 0, x*y*sizeof(long));
  gps.screentexpos_lower = screentexpos_lower = new long[x*y];
  memset(screentexpos_lower, 0, x*y*sizeof(long));
  gps.screentexpos_anchored = screentexpos_anchored = new long[x*y];
  memset(screentexpos_anchored, 0, x*y*sizeof(long));
  gps.screentexpos_anchored_x = screentexpos_anchored_x = new long[x*y];
  memset(screentexpos_anchored_x, 0, x*y*sizeof(long));
  gps.screentexpos_anchored_y = screentexpos_anchored_y = new long[x*y];
  memset(screentexpos_anchored_y, 0, x*y*sizeof(long));
  gps.screentexpos_flag = screentexpos_flag = new uint32_t[x*y];
  memset(screentexpos_flag, 0, x*y*sizeof(uint32_t));

  screen_old = new unsigned char[x*y*8];
  memset(screen_old, 0, x*y*8);
  screentexpos_old = new long[x*y];
  memset(screentexpos_old, 0, x*y*sizeof(long));
  screentexpos_lower_old = new long[x*y];
  memset(screentexpos_lower_old, 0, x*y*sizeof(long));
  screentexpos_anchored_old = new long[x*y];
  memset(screentexpos_anchored_old, 0, x*y*sizeof(long));
  screentexpos_anchored_x_old = new long[x*y];
  memset(screentexpos_anchored_x_old, 0, x*y*sizeof(long));
  screentexpos_anchored_y_old = new long[x*y];
  memset(screentexpos_anchored_y_old, 0, x*y*sizeof(long));
  screentexpos_flag_old = new uint32_t[x*y];
  memset(screentexpos_flag_old, 0, x*y*sizeof(uint32_t));

  gps.screen_top = screen_top = new unsigned char[x*y*8];
  memset(screen_top, 0, x*y*8);
  gps.screentexpos_top = screentexpos_top = new long[x*y];
  memset(screentexpos_top, 0, x*y*sizeof(long));
  gps.screentexpos_top_lower = screentexpos_top_lower = new long[x*y];
  memset(screentexpos_top_lower, 0, x*y*sizeof(long));
  gps.screentexpos_top_anchored = screentexpos_top_anchored = new long[x*y];
  memset(screentexpos_top_anchored, 0, x*y*sizeof(long));
  gps.screentexpos_top_anchored_x = screentexpos_top_anchored_x = new long[x*y];
  memset(screentexpos_top_anchored_x, 0, x*y*sizeof(long));
  gps.screentexpos_top_anchored_y = screentexpos_top_anchored_y = new long[x*y];
  memset(screentexpos_top_anchored_y, 0, x*y*sizeof(long));
  gps.screentexpos_top_flag = screentexpos_top_flag = new uint32_t[x*y];
  memset(screentexpos_top_flag, 0, x*y*sizeof(uint32_t));

  screen_top_old = new unsigned char[x*y*8];
  memset(screen_top_old, 0, x*y*8);
  screentexpos_top_old = new long[x*y];
  memset(screentexpos_top_old, 0, x*y*sizeof(long));
  screentexpos_top_lower_old = new long[x*y];
  memset(screentexpos_top_lower_old, 0, x*y*sizeof(long));
  screentexpos_top_anchored_old = new long[x*y];
  memset(screentexpos_top_anchored_old, 0, x*y*sizeof(long));
  screentexpos_top_anchored_x_old = new long[x*y];
  memset(screentexpos_top_anchored_x_old, 0, x*y*sizeof(long));
  screentexpos_top_anchored_y_old = new long[x*y];
  memset(screentexpos_top_anchored_y_old, 0, x*y*sizeof(long));
  screentexpos_top_flag_old = new uint32_t[x*y];
  memset(screentexpos_top_flag_old, 0, x*y*sizeof(uint32_t));

  gps.screentexpos_refresh_buffer=screentexpos_refresh_buffer=new int32_t[x*y];
  memset(screentexpos_refresh_buffer, 0, x*y*sizeof(int32_t));

  gps.screen_pixel_x=(int32_t)screen_x;
  gps.screen_pixel_y=(int32_t)screen_y;
  gps.tile_pixel_x=(int32_t)tile_dim_x;
  gps.tile_pixel_y=(int32_t)tile_dim_y;
  gps.resize(x,y);

  int32_t zf=gps.viewport_zoom_factor;
  set_viewport_zoom_factor(zf);
	gps.reshape_viewports(zf);
}

void renderer::swap_arrays() {
  screen = screen_old; screen_old = gps.screen; gps.screen = screen;
  screentexpos = screentexpos_old; screentexpos_old = gps.screentexpos; gps.screentexpos = screentexpos;
  screentexpos_lower = screentexpos_lower_old; screentexpos_lower_old = gps.screentexpos_lower; gps.screentexpos_lower = screentexpos_lower;
  screentexpos_anchored = screentexpos_anchored_old; screentexpos_anchored_old = gps.screentexpos_anchored; gps.screentexpos_anchored = screentexpos_anchored;
  screentexpos_anchored_x = screentexpos_anchored_x_old; screentexpos_anchored_x_old = gps.screentexpos_anchored_x; gps.screentexpos_anchored_x = screentexpos_anchored_x;
  screentexpos_anchored_y = screentexpos_anchored_y_old; screentexpos_anchored_y_old = gps.screentexpos_anchored_y; gps.screentexpos_anchored_y = screentexpos_anchored_y;
  screentexpos_flag = screentexpos_flag_old; screentexpos_flag_old = gps.screentexpos_flag; gps.screentexpos_flag = screentexpos_flag;

  screen_top = screen_top_old; screen_top_old = gps.screen_top; gps.screen_top = screen_top;
  screentexpos_top = screentexpos_top_old; screentexpos_top_old = gps.screentexpos_top; gps.screentexpos_top = screentexpos_top;
  screentexpos_top_lower = screentexpos_top_lower_old; screentexpos_top_lower_old = gps.screentexpos_top_lower; gps.screentexpos_top_lower = screentexpos_top_lower;
  screentexpos_top_anchored = screentexpos_top_anchored_old; screentexpos_top_anchored_old = gps.screentexpos_top_anchored; gps.screentexpos_top_anchored = screentexpos_top_anchored;
  screentexpos_top_anchored_x = screentexpos_top_anchored_x_old; screentexpos_top_anchored_x_old = gps.screentexpos_top_anchored_x; gps.screentexpos_top_anchored_x = screentexpos_top_anchored_x;
  screentexpos_top_anchored_y = screentexpos_top_anchored_y_old; screentexpos_top_anchored_y_old = gps.screentexpos_top_anchored_y; gps.screentexpos_top_anchored_y = screentexpos_top_anchored_y;
  screentexpos_top_flag = screentexpos_top_flag_old; screentexpos_top_flag_old = gps.screentexpos_top_flag; gps.screentexpos_top_flag = screentexpos_top_flag;

  gps.texblits.clear();

  gps.screen_limit = gps.screen + gps.dimx * gps.dimy * 8;
  gps.screen_top_limit = gps.screen_top + gps.dimx * gps.dimy * 8;

  int32_t *sw_i32;
  uint32_t *sw_ui32;
  uint64_t *sw_ui64;
  auto vp_s=gps.viewport.begin();
  auto vp_e=gps.viewport.end();
  for(;vp_s<vp_e;++vp_s)
	{
	graphic_viewportst *vp=*vp_s;
		sw_i32=vp->screentexpos_background;
			vp->screentexpos_background=vp->screentexpos_background_old;
			vp->screentexpos_background_old=sw_i32;
		sw_i32=vp->screentexpos_background_two;
			vp->screentexpos_background_two=vp->screentexpos_background_two_old;
			vp->screentexpos_background_two_old=sw_i32;
		sw_ui64=vp->screentexpos_floor_flag;
			vp->screentexpos_floor_flag=vp->screentexpos_floor_flag_old;
			vp->screentexpos_floor_flag_old=sw_ui64;
		sw_ui32=vp->screentexpos_spatter_flag;
			vp->screentexpos_spatter_flag=vp->screentexpos_spatter_flag_old;
			vp->screentexpos_spatter_flag_old=sw_ui32;
		sw_i32=vp->screentexpos_spatter;
			vp->screentexpos_spatter=vp->screentexpos_spatter_old;
			vp->screentexpos_spatter_old=sw_i32;
		sw_ui32=vp->screentexpos_liquid_flag;
			vp->screentexpos_liquid_flag=vp->screentexpos_liquid_flag_old;
			vp->screentexpos_liquid_flag_old=sw_ui32;
		sw_ui64=vp->screentexpos_ramp_flag;
			vp->screentexpos_ramp_flag=vp->screentexpos_ramp_flag_old;
			vp->screentexpos_ramp_flag_old=sw_ui64;
		sw_ui32=vp->screentexpos_shadow_flag;
			vp->screentexpos_shadow_flag=vp->screentexpos_shadow_flag_old;
			vp->screentexpos_shadow_flag_old=sw_ui32;
		sw_i32=vp->screentexpos_building_one;
			vp->screentexpos_building_one=vp->screentexpos_building_one_old;
			vp->screentexpos_building_one_old=sw_i32;
		sw_i32=vp->screentexpos_building_two;
			vp->screentexpos_building_two=vp->screentexpos_building_two_old;
			vp->screentexpos_building_two_old=sw_i32;
		sw_i32=vp->screentexpos_top_shadow;
			vp->screentexpos_top_shadow=vp->screentexpos_top_shadow_old;
			vp->screentexpos_top_shadow_old=sw_i32;
		sw_i32=vp->screentexpos_item;
			vp->screentexpos_item=vp->screentexpos_item_old;
			vp->screentexpos_item_old=sw_i32;
		sw_i32=vp->screentexpos_vehicle;
			vp->screentexpos_vehicle=vp->screentexpos_vehicle_old;
			vp->screentexpos_vehicle_old=sw_i32;
		sw_i32=vp->screentexpos_projectile;
			vp->screentexpos_projectile=vp->screentexpos_projectile_old;
			vp->screentexpos_projectile_old=sw_i32;
		sw_i32=vp->screentexpos_high_flow;
			vp->screentexpos_high_flow=vp->screentexpos_high_flow_old;
			vp->screentexpos_high_flow_old=sw_i32;
		sw_i32=vp->screentexpos_vermin;
			vp->screentexpos_vermin=vp->screentexpos_vermin_old;
			vp->screentexpos_vermin_old=sw_i32;
		sw_i32=vp->screentexpos_left_creature;
			vp->screentexpos_left_creature=vp->screentexpos_left_creature_old;
			vp->screentexpos_left_creature_old=sw_i32;
		sw_i32=vp->screentexpos;
			vp->screentexpos=vp->screentexpos_old;
			vp->screentexpos_old=sw_i32;
		sw_i32=vp->screentexpos_right_creature;
			vp->screentexpos_right_creature=vp->screentexpos_right_creature_old;
			vp->screentexpos_right_creature_old=sw_i32;
		sw_i32=vp->screentexpos_signpost;
			vp->screentexpos_signpost=vp->screentexpos_signpost_old;
			vp->screentexpos_signpost_old=sw_i32;
		sw_i32=vp->screentexpos_upleft_creature;
			vp->screentexpos_upleft_creature=vp->screentexpos_upleft_creature_old;
			vp->screentexpos_upleft_creature_old=sw_i32;
		sw_i32=vp->screentexpos_up_creature;
			vp->screentexpos_up_creature=vp->screentexpos_up_creature_old;
			vp->screentexpos_up_creature_old=sw_i32;
		sw_i32=vp->screentexpos_upright_creature;
			vp->screentexpos_upright_creature=vp->screentexpos_upright_creature_old;
			vp->screentexpos_upright_creature_old=sw_i32;
		sw_i32=vp->screentexpos_designation;
			vp->screentexpos_designation=vp->screentexpos_designation_old;
			vp->screentexpos_designation_old=sw_i32;
		sw_i32=vp->screentexpos_interface;
			vp->screentexpos_interface=vp->screentexpos_interface_old;
			vp->screentexpos_interface_old=sw_i32;
	}

  auto mp_s=gps.map_port.begin();
  auto mp_e=gps.map_port.end();
  for(;mp_s<mp_e;++mp_s)
	{
	graphic_map_portst *vp=*mp_s;
		sw_i32=vp->screentexpos_base;
			vp->screentexpos_base=vp->screentexpos_base_old;
			vp->screentexpos_base_old=sw_i32;
		int32_t ei;
		for(ei=0;ei<8;++ei)
			{
			sw_i32=vp->screentexpos_edge[ei];
				vp->screentexpos_edge[ei]=vp->screentexpos_edge_old[ei];
				vp->screentexpos_edge_old[ei]=sw_i32;
			sw_i32=vp->screentexpos_edge2[ei];
				vp->screentexpos_edge2[ei]=vp->screentexpos_edge2_old[ei];
				vp->screentexpos_edge2_old[ei]=sw_i32;
			}
		sw_i32=vp->screentexpos_detail;
			vp->screentexpos_detail=vp->screentexpos_detail_old;
			vp->screentexpos_detail_old=sw_i32;
		sw_i32=vp->screentexpos_tunnel;
			vp->screentexpos_tunnel=vp->screentexpos_tunnel_old;
			vp->screentexpos_tunnel_old=sw_i32;
		sw_i32=vp->screentexpos_river;
			vp->screentexpos_river=vp->screentexpos_river_old;
			vp->screentexpos_river_old=sw_i32;
		sw_i32=vp->screentexpos_road;
			vp->screentexpos_road=vp->screentexpos_road_old;
			vp->screentexpos_road_old=sw_i32;
		sw_i32=vp->screentexpos_site;
			vp->screentexpos_site=vp->screentexpos_site_old;
			vp->screentexpos_site_old=sw_i32;
		sw_i32=vp->screentexpos_army;
			vp->screentexpos_army=vp->screentexpos_army_old;
			vp->screentexpos_army_old=sw_i32;
		sw_i32=vp->screentexpos_interface;
			vp->screentexpos_interface=vp->screentexpos_interface_old;
			vp->screentexpos_interface_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_n;
			vp->screentexpos_detail_to_n=vp->screentexpos_detail_to_n_old;
			vp->screentexpos_detail_to_n_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_s;
			vp->screentexpos_detail_to_s=vp->screentexpos_detail_to_s_old;
			vp->screentexpos_detail_to_s_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_w;
			vp->screentexpos_detail_to_w=vp->screentexpos_detail_to_w_old;
			vp->screentexpos_detail_to_w_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_e;
			vp->screentexpos_detail_to_e=vp->screentexpos_detail_to_e_old;
			vp->screentexpos_detail_to_e_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_nw;
			vp->screentexpos_detail_to_nw=vp->screentexpos_detail_to_nw_old;
			vp->screentexpos_detail_to_nw_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_ne;
			vp->screentexpos_detail_to_ne=vp->screentexpos_detail_to_ne_old;
			vp->screentexpos_detail_to_ne_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_sw;
			vp->screentexpos_detail_to_sw=vp->screentexpos_detail_to_sw_old;
			vp->screentexpos_detail_to_sw_old=sw_i32;
		sw_i32=vp->screentexpos_detail_to_se;
			vp->screentexpos_detail_to_se=vp->screentexpos_detail_to_se_old;
			vp->screentexpos_detail_to_se_old=sw_i32;
		sw_i32=vp->screentexpos_site_to_s;
			vp->screentexpos_site_to_s=vp->screentexpos_site_to_s_old;
			vp->screentexpos_site_to_s_old=sw_i32;
		sw_ui64=vp->screentexpos_cloud_bits;
			vp->screentexpos_cloud_bits=vp->screentexpos_cloud_bits_old;
			vp->screentexpos_cloud_bits_old=sw_ui64;
	}
}

void enablerst::pause_async_loop()  {
  struct async_cmd cmd;
  cmd.cmd = async_cmd::pause;
  async_tobox.write(cmd);
  async_wait();
}

// Wait until the previous command has been acknowledged, /or/
// async_loop has quit. Incidentally execute any requests in the
// meantime.
void enablerst::async_wait() {
  if (loopvar == 0) return;
  async_msg r;
  bool reset_textures = false;
  for (;;) {
    async_frombox.read(r);
    switch (r.msg) {
    case async_msg::quit:
      loopvar = 0;
      return;
    case async_msg::complete:
      return;
    case async_msg::set_fps:
      set_fps(r.fps);
      async_fromcomplete.release();
      break;
    case async_msg::set_gfps:
      set_gfps(r.fps);
      async_fromcomplete.release();
      break;
    case async_msg::push_resize:
      override_grid_size(r.x, r.y);
      async_fromcomplete.release();
      break;
    case async_msg::pop_resize:
      release_grid_size();
      async_fromcomplete.release();
      break;
    case async_msg::reset_textures:
      reset_textures = true;
      break;
	case async_msg::show_message:
		{
		MessageBox(NULL,r.text,r.caption,r.type);
		async_fromcomplete.release();
		break;
		}
    default:
      puts("EMERGENCY: Unknown case in async_wait");
      abort();
    }
  }
}

void enablerst::async_loop() {
  async_paused = false;
  async_frames = 0;
  //int total_frames = 0;
  int fps = 100; // Just a thread-local copy // what?
  for (;;) {
    // cout << "FRAMES: " << frames << endl;
    // Check for commands
    async_cmd cmd;
    bool have_cmd = true;
    do {
      if (async_paused || (async_frames == 0 && !(enabler.flag & ENABLERFLAG_MAXFPS)))
        async_tobox.read(cmd);
      else
        have_cmd = async_tobox.try_read(cmd);
      // Obey the command, would you kindly.
      if (have_cmd) {
        switch (cmd.cmd) {
        case async_cmd::pause:
          async_paused = true;
          // puts("Paused");
          async_frombox.write(async_msg(async_msg::complete));
          break;
        case async_cmd::start:
          async_paused = false;
          async_frames = 0;
          // puts("UNpaused");
          break;
        case async_cmd::render:
          if (flag & ENABLERFLAG_RENDER) {

            //total_frames++;
            renderer->swap_arrays();
			/*
            if (total_frames % 1800 == 0)
              ttf_manager.gc();
			  */
            render_things();
            flag &= ~ENABLERFLAG_RENDER;
            update_gfps();
          }
          async_frombox.write(async_msg(async_msg::complete));
          break;
        case async_cmd::inc:
          async_frames += cmd.val;
          if (async_frames > fps*3) async_frames = fps*3; // Just in case
          break;
        case async_cmd::set_fps:
          fps = cmd.val;
          break;
        }
      }
    } while (have_cmd);
    // Run the main-loop, maybe
    if (!async_paused && (async_frames || (enabler.flag & ENABLERFLAG_MAXFPS))) {
      if (mainloop()) {
        async_frombox.write(async_msg(async_msg::quit));
        return; // We're done.
      }
      simticks++;
      async_frames--;
      if (async_frames < 0) async_frames = 0;
      update_fps();
	  clear_text_input();
    }
	SDL_NumJoysticks();
	hooks_update();
  }
}

void enablerst::do_frame() {
  // Check how long it's been, exactly
  const Uint32 now = SDL_GetTicks();
  const Uint32 interval = CLAMP(now - last_tick, 0, 1000); // Anything above a second doesn't count
  // cout << last_tick << " + " << interval << " = " << now << endl;
  last_tick = now;

  // Update outstanding-frame counts
  outstanding_frames += interval * fps / 1000;
  outstanding_gframes += interval * gfps / 1000;
  if (outstanding_gframes > 3) {
    outstanding_gframes = 3;
  }
  // cout << outstanding_frames << " " << outstanding_gframes << endl;
 
  // Update the loop's tick-counter suitably
  if (outstanding_frames >= 1) {
    async_cmd cmd(async_cmd::inc);
    cmd.val = outstanding_frames;
    outstanding_frames -= cmd.val;
    async_tobox.write(cmd);
  }

  // Store the current time, for things that are fine with approximations
  enabler.clock = SDL_GetTicks();

  // If it's time to render..
  if (outstanding_gframes >= 1) {
    // Get the async-loop to render_things
    async_cmd cmd(async_cmd::render);
    async_tobox.write(cmd);
    async_wait();
    // Then finish here
	if(gps.main_thread_requesting_reshape)
		{
		int32_t zf=gps.viewport_zoom_factor;

		renderer->set_viewport_zoom_factor(zf);

		gps.reshape_viewports(zf);

		gps.main_thread_requesting_reshape=false;
		}

	if(!must_do_render_things_before_display)
		{
		if(gps.do_post_init_texture_clear)//needs to be after clean tile cache
			{
			enabler.textures.delete_all_post_init_textures();

			gps.do_post_init_texture_clear=false;
			}

		if(gps.do_clean_tile_cache)
			{
			renderer->clean_tile_cache();

			gps.do_clean_tile_cache=false;
			}
		renderer->tidy_tile_cache();
		renderer->display();
		renderer->render();
		}
    gputicks++;
    outstanding_gframes--;
  }
  // Sleep until the next gframe
  if (outstanding_gframes < 1) {
    emit_logs(); // flush game/error logs whenever we have to sleep
    float fragment = 1 - outstanding_gframes;
    float milliseconds = fragment / gfps * 1000;
    // cout << milliseconds << endl;
    SDL_Delay(milliseconds);
  }
}

void enablerst::eventLoop_SDL()
{
  
  SDL_Event event;
  Uint32 mouse_lastused = 0;
  SDL_ShowCursor(SDL_DISABLE);
  int window_w, window_h;
  SDL_GetWindowSize(renderer->get_window(), &window_w, &window_h);
  // Initialize the grid
  renderer->resize(window_w, window_h);

  while (loopvar) {
    Uint32 now = SDL_GetTicks();
	bool already_wheeled = false;
    bool paused_loop = false;

    // Check for zoom commands
    zoom_commands zoom;
    while (async_zoom.try_read(zoom)) {
      if (overridden_grid_sizes.size())
        continue; // No zooming in movies
      if (!paused_loop) {
        pause_async_loop();
        paused_loop = true;
      }
      if (zoom == zoom_fullscreen)
        renderer->set_fullscreen();
      else
        renderer->zoom(zoom);
    }

	bool any_text_event=false;

    // Check for SDL events
    while (SDL_PollEvent(&event)) {
      // Make sure mainloop isn't running while we're processing input
      if (!paused_loop) {
        pause_async_loop();
        paused_loop = true;
      }
	  if (hooks_sdl_event(&event)) continue;
      // Handle SDL events
      switch (event.type) {
	  case SDL_MOUSEWHEEL:
		  if (!already_wheeled) {
			  already_wheeled = true;
			  enabler.add_input(event, now);
		  }
		  break;
	  case SDL_KEYDOWN:
		if (event.key.repeat) break;
        // Disable mouse if it's been long enough
        if (mouse_lastused + 5000 < now) {
          if(init.input.flag.has_flag(INIT_INPUT_FLAG_MOUSE_PICTURE)) {
            // hide the mouse picture
            // enabler.set_tile(0, TEXTURE_MOUSE, enabler.mouse_x, enabler.mouse_y);
          }
          SDL_ShowCursor(SDL_DISABLE);
        }
      case SDL_KEYUP:
	  case SDL_QUIT:
        enabler.add_input(event, now);
        break;
	  case SDL_TEXTINPUT:
		enabler.set_text_input(event);
		any_text_event=true;
		break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        if (!init.input.flag.has_flag(INIT_INPUT_FLAG_MOUSE_OFF)) {
          int isdown = (event.type == SDL_MOUSEBUTTONDOWN);

          if (event.button.button == SDL_BUTTON_LEFT) {
            enabler.mouse_lbut = isdown;
            enabler.mouse_lbut_down = isdown;
            if (!isdown)
              enabler.mouse_lbut_lift = 0;
          } else if (event.button.button == SDL_BUTTON_RIGHT) {
            enabler.mouse_rbut = isdown;
            enabler.mouse_rbut_down = isdown;
            if (!isdown)
              enabler.mouse_rbut_lift = 0;
          } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            enabler.mouse_mbut = isdown;
            enabler.mouse_mbut_down = isdown;
            if (!isdown)
              enabler.mouse_mbut_lift = 0;
          } else
            enabler.add_input(event, now);
        }
        break;
      case SDL_MOUSEMOTION:
        // Deal with the mouse hiding bit
        mouse_lastused = now;
        if(init.input.flag.has_flag(INIT_INPUT_FLAG_MOUSE_PICTURE)) {
          // turn on mouse picture
          // enabler.set_tile(gps.tex_pos[TEXTURE_MOUSE], TEXTURE_MOUSE,enabler.mouse_x, enabler.mouse_y);
        } else {
#ifndef FULL_RELEASE_VERSION
			if(cinematic_mode&&(cinematic_shift_velx!=0||cinematic_shift_vely!=0))
				{
				SDL_ShowCursor(SDL_DISABLE);
				}
          else SDL_ShowCursor(SDL_ENABLE);
#else
          SDL_ShowCursor(SDL_ENABLE);
#endif
        }
        break;
      case SDL_WINDOWEVENT:
        enabler.clear_input();
		switch (event.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
				enabler.flag |= ENABLERFLAG_RENDER;
				gps.force_full_display_count++;
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				gps.force_full_display_count++;
				enabler.flag |= ENABLERFLAG_RENDER;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				if (is_fullscreen());
				//errorlog << "Caught resize event in fullscreen??\n";
				else {
					//gamelog << "Resizing window to " << event.resize.w << "x" << event.resize.h << endl << flush;
					renderer->resize(event.window.data1, event.window.data2);
				}
				break;
			case SDL_WINDOWEVENT_ENTER:
				mouse_focus = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				mouse_focus = false;
				break;
		}
		break;
      } // switch (event.type)
    } //while have event

    // Update mouse state
    if(!init.input.flag.has_flag(INIT_INPUT_FLAG_MOUSE_OFF))
		{
		int mouse_x = -1, mouse_y = -1, mouse_state;
		int precise_mouse_x=-1,precise_mouse_y=-1;
		// Check whether the renderer considers this valid input or not, and write it to gps
		if(mouse_focus &&
			renderer->get_precise_mouse_coords(precise_mouse_x, precise_mouse_y,mouse_x,mouse_y))
			{
			mouse_state=1;
			}
		else mouse_state=0;

		if(precise_mouse_x!=gps.precise_mouse_x||
			precise_mouse_y!=gps.precise_mouse_y||
			mouse_state!=enabler.tracking_on)
			{
			// Pause rendering loop and update values
			if (!paused_loop) {
			  pause_async_loop();
			  paused_loop = true;}

			enabler.tracking_on=mouse_state;
			gps.mouse_x=mouse_x;
			gps.mouse_y=mouse_y;
			gps.precise_mouse_x=precise_mouse_x;
			gps.precise_mouse_y=precise_mouse_y;
			}
		}

    if (paused_loop)
      unpause_async_loop();

    hooks_sdl_loop_fn();
    do_frame();

#ifndef NO_FMOD
	musicsound.update();
#endif
  }
}

int enablerst::loop(string cmdline) {
  command_line = cmdline;
#ifdef WIN32
  glaiel::crashlogs::set_crashlog_folder("crashlogs");
  glaiel::crashlogs::set_crashlog_header_message("Crashed before main menu. Yikes!");
  glaiel::crashlogs::begin_monitoring();
#endif
  // Initialize the tick counters
  simticks=0;
  gputicks=0;
  
  // Call DF's initialization routine
  if (!beginroutine())
    exit(EXIT_FAILURE);
  
  // Allocate a renderer
  if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT)) {
#ifdef CURSES
    renderer = new renderer_curses();
#else
    report_error("PRINT_MODE", "TEXT not supported on windows");
    exit(EXIT_FAILURE);
#endif
  } else {
    renderer = new renderer_2d();
  }
  //*************************** ALLOW OTHER RENDER TYPES THAN 2D?
  /*
  else if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_ACCUM_BUFFER)) {
    renderer = new renderer_accum_buffer();
  } else if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_FRAME_BUFFER)) {
    renderer = new renderer_framebuffer();
  } else if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_PARTIAL_PRINT)) {
    if (init.display.partial_print_count)
      renderer = new renderer_partial();
    else
      renderer = new renderer_once();
  } else if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_VBO)) {
    renderer = new renderer_vbo();
  } else {
    renderer = new renderer_opengl();
  }
  */

  // At this point we should have a window that is setup to render DF.
  hooks_init();
  if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT)) {
#ifdef CURSES
    eventLoop_ncurses();
#endif
  } else {
    eventLoop_SDL();
  }

  hooks_shutdown();
  endroutine();

  // Clean up graphical resources
  delete renderer;

  return 0;
}

void enablerst::override_grid_size(int x, int y) {
  if (SDL_ThreadID() != renderer_threadid) {
    // Ask the renderer to do it
    async_msg m(async_msg::push_resize);
    m.x = x; m.y = y;
    async_frombox.write(m);
    async_fromcomplete.acquire();
  } else {
    // We are the renderer; do it.
    overridden_grid_sizes.push(make_pair(init.display.grid_x,init.display.grid_y));
    renderer->grid_resize(x, y);
  }
}

void enablerst::release_grid_size() {
  if (SDL_ThreadID() != renderer_threadid) {
    async_frombox.write(async_msg(async_msg::pop_resize));
    async_fromcomplete.acquire();
  } else {
    if (!overridden_grid_sizes.size()) return;
    // FIXME: Find out whatever is causing release to be called too rarely; right now
    // we're overriding once per movie but apparently only releasing for the last one.
    pair<int,int> sz;
    while (overridden_grid_sizes.size()) {
      sz = overridden_grid_sizes.top();
      overridden_grid_sizes.pop();
    }
    zoom_display(zoom_resetgrid);
  }
}

void enablerst::zoom_display(zoom_commands command) {
  async_zoom.write(command);
}

int enablerst::calculate_fps() {
  if (frame_timings.size() < 50)
    return get_fps();
  else
    return calculated_fps;
}
int enablerst::calculate_gfps() {
  if (gframe_timings.size() < 50)
    return get_gfps();
  else
    return calculated_gfps;
}

void enablerst::do_update_fps(queue<int> &q, int &sum, int &last, int &calc) {
  while (q.size() > 50 && sum > 10000) {
    sum -= q.front();
    q.pop();
  }
  const int now = SDL_GetTicks();
  const int interval = now - last;
  q.push(interval);
  sum += interval;
  last = now;
  if (sum)
    calc = (int)q.size() * 1000 / sum;
}

void enablerst::clear_fps() {
  while (frame_timings.size())
    frame_timings.pop();
  frame_sum = 0;
  frame_last = SDL_GetTicks();
  calculated_fps = get_fps();
}

void enablerst::update_fps() {
  do_update_fps(frame_timings, frame_sum, frame_last, calculated_fps);
}

void enablerst::update_gfps() {
  do_update_fps(gframe_timings, gframe_sum, gframe_last, calculated_gfps);
}

void enablerst::set_fps(int fps) {
  if (SDL_ThreadID() != renderer_threadid) {
    async_msg m(async_msg::set_fps);
    m.fps = fps;
    async_paused = true;
    async_frombox.write(m);
    async_fromcomplete.acquire();
  } else {
    if (fps <= 0) fps = 1048576;
    this->fps = fps;
    fps_per_gfps = fps / gfps;
    struct async_cmd cmd;
    cmd.cmd = async_cmd::set_fps;
    cmd.val = fps;
    async_tobox.write(cmd);
    async_tobox.write(async_cmd(async_cmd::start));
  }
}

void enablerst::set_gfps(int gfps) {
  if (SDL_ThreadID() != renderer_threadid) {
    async_msg m(async_msg::set_gfps);
    m.fps = gfps;
    async_frombox.write(m);
    async_fromcomplete.acquire();
  } else {
    if (gfps == 0)
      gfps = 50;
    this->gfps = gfps;
    fps_per_gfps = fps / gfps;
  }
}

void enablerst::set_listen_to_text(bool listen) 
{
	if (listen && !listening_to_text)
		{
		SDL_StartTextInput();
		}
	listening_to_text=listen;
}

void enablerst::set_text_input(SDL_Event ev) {
	std::memcpy(last_text_input.data(), ev.text.text, 32);
}

void enablerst::clear_text_input() {
	std::fill(last_text_input.begin(), last_text_input.end(), '\0');
}

int call_loop(void *dummy) {
  enabler.async_loop();
  return 0;
}

int main (int argc, char* argv[]) {
#ifdef unix
  setlocale(LC_ALL, "");
#endif

  // Initialise minimal SDL subsystems.
  int retval = SDL_Init(SDL_INIT_TIMER);
  // Report failure?
  if (retval != 0) {
    report_error("SDL initialization failure", SDL_GetError());
    return 0;
  }
  enabler.renderer_threadid = SDL_ThreadID();

  // Spawn simulation thread
  SDL_CreateThread(call_loop, NULL, NULL);

  init.begin(); // Load init.txt settings
  
#if !defined(__APPLE__) && defined(unix)
  if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT) &&
      init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS)) {
    puts("Graphical tiles are not compatible with text output, sorry");
    exit(EXIT_FAILURE);
  }
#endif

  // Initialize video, if we /use/ video
  retval = SDL_InitSubSystem(init.display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT) ? 0 : SDL_INIT_VIDEO);
  if (retval != 0) {
    report_error("SDL initialization failure", SDL_GetError());
    return 0;
  }

#ifdef WIN32
  // Attempt to get as good a timer as possible
  int ms = 1;
  while (timeBeginPeriod(ms) != TIMERR_NOERROR) ms++;
#endif

  // Load keyboard map
  keybinding_init();
  //NOTE: this order is important!  load_keybindings does not overwrite keys, so loading prefs first is correct
  enabler.load_keybindings(filest("prefs/interface.txt"));
  enabler.load_keybindings(filest("data/init/interface.txt").with_flags(FILE_FLAG_ALWAYS_BASE_FIRST));//only adds new keys from new versions etc.

  string cmdLine;
  for (int i = 1; i < argc; ++i) { 
    char *option = argv[i];
	string opt=option;
	if(opt.length()>=1)
		{
		//main removes quotes, unlike the winmain version, so it has to be rebuilt
		if(opt[0]=='-')
			{
			cmdLine += opt;
			cmdLine += " ";
			}
		else
			{
			cmdLine += "\"";
			cmdLine += opt;
			cmdLine += "\"";
			cmdLine += " ";
			}
		}
  }
  int result = enabler.loop(cmdLine);

  SDL_Quit();

#ifdef WIN32
  timeEndPeriod(ms);
#endif
  
  return result;
}

void curses_text_boxst::add_paragraph(const string &src,int32_t para_width)
{
	if(para_width<=0)return;

	stringvectst sp;
	sp.add_string(src);
	add_paragraph(sp,para_width);
}

void curses_text_boxst::add_paragraph(stringvectst &src,int32_t para_width)
{
	if(para_width<=0)return;

	bool skip_leading_spaces=false;

	//ADD EACH OF THE STRINGS ON IN TURN
	string curstr;
	long strlength=0;
	long s,pos;
	for(s=0;s<src.str.size();s++)
		{
		//GRAB EACH WORD, AND SEE IF IT FITS, IF NOT START A NEW LINE
		for(pos=0;pos<src.str[s]->dat.size();pos++)
			{
			if(skip_leading_spaces)
				{
				if(src.str[s]->dat[pos]==' ')continue;
				else skip_leading_spaces=false;
				}

			//ADD TO WORD
			curstr+=src.str[s]->dat[pos];

			//IF TOO LONG, CUT BACK TO FIRST SPACE
			if(curstr.length()>para_width)
				{
				long opos=pos;

				long minus=0;
				do
					{
					pos--;
					minus++;
					}while(src.str[s]->dat[pos]!=' '&&pos>0);

				//IF WENT ALL THE WAY BACK, INTRODUCE A SPACE
				if(minus==curstr.size())
					{
					src.str[s]->dat.insert(opos-1," ");
					}
				else
					{
					curstr.resize(curstr.size()-minus);
					text.add_string(curstr);
					skip_leading_spaces=true;
					}
				curstr.erase();
				}
			}
		}

	//FLUSH FINAL BIT
	if(!curstr.empty())text.add_string(curstr);
}