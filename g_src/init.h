#ifndef INIT_H
#define INIT_H

#include "enabler.h"

struct init_fontst {
	long basic_font_texpos[256];
	long small_font_texpos[256];
	long large_font_texpos[256];
	long basic_font_datapos[256];
	long small_font_datapos[256];
	long large_font_datapos[256];

	long basic_font_texpos_top[256];
	long small_font_texpos_top[256];
	long large_font_texpos_top[256];
	long basic_font_texpos_bot[256];
	long small_font_texpos_bot[256];
	long large_font_texpos_bot[256];

	float basic_font_adjx;
	float basic_font_adjy;
	float small_font_adjx;
	float small_font_adjy;
	float large_font_adjx;
	float large_font_adjy;
	long basic_font_dispx;
	long basic_font_dispy;
	long small_font_dispx;
	long small_font_dispy;
	long large_font_dispx;
	long large_font_dispy;

	void create_derived_font_textures();
};

enum InitDisplayFlag
{
	INIT_DISPLAY_FLAG_USE_GRAPHICS,
//	INIT_DISPLAY_FLAG_BLACK_SPACE,
//	INIT_DISPLAY_FLAG_PARTIAL_PRINT,
//	INIT_DISPLAY_FLAG_FRAME_BUFFER,
//	INIT_DISPLAY_FLAG_SINGLE_BUFFER,
//	INIT_DISPLAY_FLAG_ACCUM_BUFFER,
//	INIT_DISPLAY_FLAG_VBO,
//       INIT_DISPLAY_FLAG_2D,
	INIT_DISPLAY_FLAG_SOFTWARE,
//        INIT_DISPLAY_FLAG_2DHW,
//        INIT_DISPLAY_FLAG_2DASYNC,
//	INIT_DISPLAY_FLAG_UNUSED_01_08,//
        INIT_DISPLAY_FLAG_TEXT,
//        INIT_DISPLAY_FLAG_SHADER,
        INIT_DISPLAY_FLAG_NOT_RESIZABLE,
//        INIT_DISPLAY_FLAG_ARB_SYNC,
	INIT_DISPLAY_FLAG_INTERFACE_SCALING_TO_DESIRED_HEIGHT_WIDTH,
	INIT_DISPLAY_FLAGNUM
};

enum InitDisplayWindow
{
	INIT_DISPLAY_WINDOW_TRUE,
	INIT_DISPLAY_WINDOW_FALSE,
	INIT_DISPLAY_WINDOW_PROMPT,
	INIT_DISPLAY_WINDOW_EXCLUSIVE,
	INIT_DISPLAY_WINDOWNUM
};

enum struct InitDisplayFilterMode : int32_t {
	AUTO,
	NEAREST,
	LANCZOS,
	NUM
};

struct init_displayst {
  int32_t max_interface_percentage = 100;
  int32_t interface_scaling_desired_width = 170;
  int32_t interface_scaling_desired_height = 64;
  int32_t interface_scaling_percentage = 100;
  flagarrayst flag;

  init_displayst(){
		flag.set_size_on_flag_num(INIT_DISPLAY_FLAGNUM);
		flag.add_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
		flag.add_flag(INIT_DISPLAY_FLAG_INTERFACE_SCALING_TO_DESIRED_HEIGHT_WIDTH);
	}

  InitDisplayWindow windowed = INIT_DISPLAY_WINDOW_TRUE;

  int grid_x, grid_y; // The *current* display grid size, kept up to date
  int desired_fullscreen_width, desired_fullscreen_height;
  int actual_fullscreen_width, actual_fullscreen_height;
  int desired_windowed_width, desired_windowed_height;
  int actual_windowed_width, actual_windowed_height;

  char partial_print_count = 0;

  InitDisplayFilterMode filter_mode = InitDisplayFilterMode::AUTO;
};

enum InitMediaFlag
{
	INIT_MEDIA_FLAG_SOUND_OFF,
	INIT_MEDIA_FLAG_UNUSED_01_02,
	INIT_MEDIA_FLAG_COMPRESS_SAVES,
	INIT_MEDIA_FLAGNUM
};

struct init_mediast {
	int32_t volume_master = 255;
	int32_t volume_music = 255;
	int32_t volume_ambience = 220;
	int32_t volume_sfx = 255;
	int32_t time_between_songs = 240;
	flagarrayst flag;

	init_mediast(){
		flag.set_size_on_flag_num(INIT_MEDIA_FLAGNUM);
	}
};

enum InitInputFlag
{
	INIT_INPUT_FLAG_MOUSE_OFF,
	INIT_INPUT_FLAG_MOUSE_PICTURE,
	INIT_INPUT_FLAGNUM
};

struct init_inputst {
  int32_t hold_time = 150;
  int32_t repeat_time = 150;
  int32_t macro_time = 75;
  int32_t pause_zoom_no_interface_ms = 0;
  int32_t zoom_speed = 10;
  int32_t repeat_accel_start = 10;
  int32_t repeat_accel_limit = 1;
  flagarrayst flag;
  
  init_inputst(){
    flag.set_size_on_flag_num(INIT_INPUT_FLAGNUM);
  }
};

enum InitWindowFlag
{
	INIT_WINDOW_FLAG_VSYNC_ON,
	INIT_WINDOW_FLAG_VSYNC_OFF,
	INIT_WINDOW_FLAG_TEXTURE_LINEAR,
	INIT_WINDOW_FLAGNUM
};

struct init_windowst {
	flagarrayst flag;

	init_windowst(){
		flag.set_size_on_flag_num(INIT_WINDOW_FLAGNUM);
		flag.add_flag(INIT_WINDOW_FLAG_VSYNC_OFF);
		flag.add_flag(INIT_WINDOW_FLAG_TEXTURE_LINEAR);
	}
};

enum InitLoadBarTextureType
{
	INIT_LOAD_BAR_TEXTURE_LEFT_FULL,
	INIT_LOAD_BAR_TEXTURE_MID_FULL,
	INIT_LOAD_BAR_TEXTURE_RIGHT_FULL,
	INIT_LOAD_BAR_TEXTURE_LEFT_EMPTY,
	INIT_LOAD_BAR_TEXTURE_MID_EMPTY,
	INIT_LOAD_BAR_TEXTURE_RIGHT_EMPTY,
	INIT_LOAD_BAR_TEXTURENUM,
	INIT_LOAD_BAR_TEXTURE_NONE=-1
};
typedef int32_t InitLoadBarTexture;
#define INIT_INTRO_BUTTON_TEXTURENUM 13*9
#define INIT_BORDER_TEXTURENUM 21
#define INIT_SCROLLBAR_TEXTURENUM 48
#define INIT_FILTER_TEXTURENUM 30
#define INIT_TABS_TEXTURENUM 30

struct initst {

	init_displayst display;
	init_mediast media;
	init_inputst input;
	init_fontst font;
	init_windowst window;

	//these are just used for save/load purposes
	int32_t fps_cap;
	int32_t gfps_cap;

	long load_bar_texpos[INIT_LOAD_BAR_TEXTURENUM];
	long intro_button_texpos[INIT_INTRO_BUTTON_TEXTURENUM];
	int32_t texpos_neutral_intro_button[9];
	int32_t texpos_confirm_intro_button[9];
	int32_t texpos_cancel_intro_button[9];
	int32_t texpos_selected_intro_button[9];
	int32_t texpos_unselected_intro_button[9];
	int32_t texpos_open_list_button[9];
	int32_t texpos_increase_button[9];
	int32_t texpos_decrease_button[9];
	int32_t texpos_nullify_button[9];
	int32_t texpos_left_arrow_button[9];
	int32_t texpos_right_arrow_button[9];
	int32_t texpos_up_arrow_button[9];
	int32_t texpos_down_arrow_button[9];

	long border_texpos[INIT_BORDER_TEXTURENUM];
	int32_t texpos_border_nw;
	int32_t texpos_border_n;
	int32_t texpos_border_ne;
	int32_t texpos_border_w;
	int32_t texpos_border_interior;
	int32_t texpos_border_e;
	int32_t texpos_border_sw;
	int32_t texpos_border_s;
	int32_t texpos_border_se;
	int32_t texpos_border_join_n;
	int32_t texpos_border_join_s;
	int32_t texpos_border_join_w;
	int32_t texpos_border_join_e;
	int32_t texpos_border_inside_nswe;
	int32_t texpos_border_inside_nsw;
	int32_t texpos_border_inside_nse;
	int32_t texpos_border_inside_nwe;
	int32_t texpos_border_inside_swe;
	int32_t texpos_border_inside_ns;
	int32_t texpos_border_inside_we;

	long scrollbar_texpos[INIT_SCROLLBAR_TEXTURENUM];
	int32_t texpos_scrollbar[2][3];
	int32_t texpos_scrollbar_up_hover[2];
	int32_t texpos_scrollbar_up_pressed[2];
	int32_t texpos_scrollbar_down_hover[2];
	int32_t texpos_scrollbar_down_pressed[2];
	int32_t texpos_scrollbar_small_scroller[2][2];
	int32_t texpos_scrollbar_small_scroller_hover[2][2];
	int32_t texpos_scrollbar_top_scroller[2];
	int32_t texpos_scrollbar_top_scroller_hover[2];
	int32_t texpos_scrollbar_bottom_scroller[2];
	int32_t texpos_scrollbar_bottom_scroller_hover[2];
	int32_t texpos_scrollbar_blank_scroller[2];
	int32_t texpos_scrollbar_blank_scroller_hover[2];
	int32_t texpos_scrollbar_center_scroller[2];
	int32_t texpos_scrollbar_center_scroller_hover[2];
	int32_t texpos_scrollbar_offcenter_scroller[2][2];
	int32_t texpos_scrollbar_offcenter_scroller_hover[2][2];

	long filter_texpos[INIT_FILTER_TEXTURENUM];
	int32_t texpos_button_filter[6][3];
	int32_t texpos_button_filter_name[4][3];
	
	long tabs_texpos[INIT_TABS_TEXTURENUM];
	int32_t texpos_tab_unselected[5][2];
	int32_t texpos_tab_selected[5][2];

	//classic basic interface
	long classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURENUM];
	int32_t classic_texpos_neutral_intro_button[9];
	int32_t classic_texpos_confirm_intro_button[9];
	int32_t classic_texpos_cancel_intro_button[9];
	int32_t classic_texpos_selected_intro_button[9];
	int32_t classic_texpos_unselected_intro_button[9];
	int32_t classic_texpos_open_list_button[9];
	int32_t classic_texpos_increase_button[9];
	int32_t classic_texpos_decrease_button[9];
	int32_t classic_texpos_nullify_button[9];
	int32_t classic_texpos_left_arrow_button[9];
	int32_t classic_texpos_right_arrow_button[9];
	int32_t classic_texpos_up_arrow_button[9];
	int32_t classic_texpos_down_arrow_button[9];
	int32_t classic_texpos_border_nw;
	int32_t classic_texpos_border_n;
	int32_t classic_texpos_border_ne;
	int32_t classic_texpos_border_w;
	int32_t classic_texpos_border_interior;
	int32_t classic_texpos_border_e;
	int32_t classic_texpos_border_sw;
	int32_t classic_texpos_border_s;
	int32_t classic_texpos_border_se;
	int32_t classic_texpos_border_join_n;
	int32_t classic_texpos_border_join_s;
	int32_t classic_texpos_border_join_w;
	int32_t classic_texpos_border_join_e;
	int32_t classic_texpos_border_inside_nswe;
	int32_t classic_texpos_border_inside_nsw;
	int32_t classic_texpos_border_inside_nse;
	int32_t classic_texpos_border_inside_nwe;
	int32_t classic_texpos_border_inside_swe;
	int32_t classic_texpos_border_inside_ns;
	int32_t classic_texpos_border_inside_we;
	int32_t classic_texpos_scrollbar[2][3];
	int32_t classic_texpos_scrollbar_up_hover[2];
	int32_t classic_texpos_scrollbar_up_pressed[2];
	int32_t classic_texpos_scrollbar_down_hover[2];
	int32_t classic_texpos_scrollbar_down_pressed[2];
	int32_t classic_texpos_scrollbar_small_scroller[2][2];
	int32_t classic_texpos_scrollbar_small_scroller_hover[2][2];
	int32_t classic_texpos_scrollbar_top_scroller[2];
	int32_t classic_texpos_scrollbar_top_scroller_hover[2];
	int32_t classic_texpos_scrollbar_bottom_scroller[2];
	int32_t classic_texpos_scrollbar_bottom_scroller_hover[2];
	int32_t classic_texpos_scrollbar_blank_scroller[2];
	int32_t classic_texpos_scrollbar_blank_scroller_hover[2];
	int32_t classic_texpos_scrollbar_center_scroller[2];
	int32_t classic_texpos_scrollbar_center_scroller_hover[2];
	int32_t classic_texpos_scrollbar_offcenter_scroller[2][2];
	int32_t classic_texpos_scrollbar_offcenter_scroller_hover[2][2];
	int32_t classic_texpos_button_filter[6][3];
	int32_t classic_texpos_button_filter_name[4][3];
	int32_t classic_texpos_tab_unselected[5][2];
	int32_t classic_texpos_tab_selected[5][2];

	void begin();
	void swap_basic_sets();
};

extern initst init;

#endif
