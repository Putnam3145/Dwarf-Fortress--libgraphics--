#include "../release_type.h"

#include "platform.h"
#include <string.h>
#include <math.h>
#include <iosfwd>
#include <iostream>
#include <ios>
#include <streambuf>
#include <istream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <zlib.h>
#include <unordered_map>
#include "../zlib/contrib/minizip/unzip.h"

#include "svector.h"
using std::string;

#include "endian.h"

#include "files.h"

#include "enabler.h"

#include "textlines.h"

#include "basics.h"

#include "g_basics.h"

#include "texture_handler.h"

#include "graphics.h"

#include "init.h"

#ifdef DEBUGOPT
//#define ULTRAWIDE_HACK
#endif

extern enablerst enabler;
extern graphicst gps;

int32_t convert_raw_to_ascii_texpos(uint8_t tile,uint8_t color_f,uint8_t color_b,uint8_t color_br);

void get_main_interface_dims(int32_t &main_interface_sx,int32_t &main_interface_ex)
	{
	main_interface_sx=0;
	main_interface_ex=(::init.display.grid_x)-1;
	//***************************** FONT SIZE DEPENDENCE
		//all the 8s and 12s in this need to be watched, in feed and render too
	int32_t adjusted_width=(::init.display.grid_x)*init.display.max_interface_percentage/100;
	if (adjusted_width<MIN_GRID_X)adjusted_width=MIN_GRID_X;
	if (main_interface_ex-main_interface_sx+1>adjusted_width)
		{
		int32_t excess=main_interface_ex-main_interface_sx-adjusted_width+1;
		if (main_interface_ex-main_interface_sx+1-excess<MIN_GRID_X)
			{
			excess=main_interface_ex-main_interface_sx+1-MIN_GRID_X;
			}
		if (excess>0)
			{
			main_interface_sx+=excess;
			main_interface_sx-=excess/2;
			main_interface_ex-=excess/2;
			}
		}
	}

init_displayst::init_displayst()
{
	flag.set_size_on_flag_num(INIT_DISPLAY_FLAGNUM);
		flag.add_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
		flag.add_flag(INIT_DISPLAY_FLAG_INTERFACE_SCALING_TO_DESIRED_HEIGHT_WIDTH);
		flag.add_flag(INIT_DISPLAY_FLAG_LOAD_TITLE_GRAPHICS);

	interface_scaling_desired_width=170;
	interface_scaling_desired_height=64;
	interface_scaling_percentage=100;

	windowed=INIT_DISPLAY_WINDOW_TRUE;
	filter_mode = InitDisplayFilterMode::AUTO;

	partial_print_count=0;

	max_interface_percentage=100;
}

void initst::begin()
{
  static bool called = false;
  if (called) return;
  called = true;
  
	string basic_font="data/art/curses_640x300.png";
	string small_font="data/art/curses_640x300.png";
	string large_font="data/art/curses_640x300.png";

	if ((filest(PORTABLE_FILENAME).any_location()))
		{
		media.flag.add_flag(INIT_MEDIA_FLAG_PORTABLE_MODE);
		}
	else
		{
		media.flag.remove_flag(INIT_MEDIA_FLAG_PORTABLE_MODE);
		}
	for(auto &f : {filest("data/init/init_default.txt"),filest("prefs/init.txt")})
		{
		std::ifstream fseed=f.to_ifstream();
		if(fseed.is_open())
			{
			string str;

			while(std::getline(fseed,str))
				{
				if(str.length()>1)
					{
					string token;
					string token2;

					grab_token_string_pos(token,str,1);
					if(str.length()>=token.length()+2)
						{
						grab_token_string_pos(token2,str,(int32_t)token.length()+2);
						}
                                
					if(!token.compare("BASIC_FONT"))
						{
						basic_font="data/art/";
						basic_font+=token2;
						}
					if(!token.compare("FONT"))
						{
						small_font="data/art/";
						small_font+=token2;
						}
					if(!token.compare("FULLFONT"))
						{
						large_font="data/art/";
						large_font+=token2;
						}
					if(!token.compare("WINDOWEDX"))
						{
						display.desired_windowed_width=convert_string_to_long(token2);
						if(display.desired_windowed_width<MINIMUM_WINDOW_WIDTH)display.desired_windowed_width=MINIMUM_WINDOW_WIDTH;
						}
					if(!token.compare("WINDOWEDY"))
						{
						display.desired_windowed_height=convert_string_to_long(token2);
						if(display.desired_windowed_height<MINIMUM_WINDOW_HEIGHT)display.desired_windowed_height=MINIMUM_WINDOW_HEIGHT;
						}
					if(!token.compare("RESIZABLE"))
						{
						if(token2=="NO")display.flag.add_flag(INIT_DISPLAY_FLAG_NOT_RESIZABLE);
						else display.flag.remove_flag(INIT_DISPLAY_FLAG_NOT_RESIZABLE);
						}
					if(!token.compare("FULLSCREENX"))
						{
						display.desired_fullscreen_width=convert_string_to_long(token2);
						}
					if(!token.compare("FULLSCREENY"))
						{
						display.desired_fullscreen_height=convert_string_to_long(token2);
						}

					//if(!token.compare("MAXIMUM_INTERFACE_PIXEL_WIDTH"));

					if(!token.compare("MAXIMUM_INTERFACE_PERCENTAGE"))
						{
						display.max_interface_percentage=convert_string_to_long(token2);
						if(display.max_interface_percentage>100)display.max_interface_percentage=100;
						if(display.max_interface_percentage<1)display.max_interface_percentage=1;
						}
					if(!token.compare("PRINT_MODE"))
						{
						std::unordered_map<std::string,int32_t> modes = {
								{"SOFTWARE", INIT_DISPLAY_FLAG_SOFTWARE},
								{"AUTO", -INIT_DISPLAY_FLAG_SOFTWARE},
								{"2D", INIT_DISPLAY_FLAG_SOFTWARE},
								{"STANDARD", -INIT_DISPLAY_FLAG_SOFTWARE},
								{"TEXT", INIT_DISPLAY_FLAG_TEXT}
								};
						auto it=modes.find(token2);
						if(it!=modes.end()) 
							{
							if (it->second<0)
								{
								display.flag.remove_flag(-it->second);
								}
							else
								{
								display.flag.add_flag(it->second);
								}
							}
						}
					if(token=="FPS")
						{
						if(token2=="YES")
							{
							gps.display_frames=1;
							}
						else gps.display_frames=0;
						}
					if(!token.compare("FPS_CAP"))
						{
						fps_cap=convert_string_to_long(token2);
						enabler.set_fps(fps_cap);
						}
					if(!token.compare("G_FPS_CAP"))
						{
						gfps_cap=convert_string_to_long(token2);
						enabler.set_gfps(gfps_cap);
						}
					if(!token.compare("INTERFACE_SCALING_TO_DESIRED_GRID"))
						{
						if(token2=="YES")
							{
							display.flag.add_flag(INIT_DISPLAY_FLAG_INTERFACE_SCALING_TO_DESIRED_HEIGHT_WIDTH);
							}
						else display.flag.remove_flag(INIT_DISPLAY_FLAG_INTERFACE_SCALING_TO_DESIRED_HEIGHT_WIDTH);
						}
					if(!token.compare("INTERFACE_SCALING_DESIRED_GRID_WIDTH"))
						{
						display.interface_scaling_desired_width=convert_string_to_long(token2);
						if(display.interface_scaling_desired_width<MIN_GRID_X)display.interface_scaling_desired_width=MIN_GRID_X;
						if(display.interface_scaling_desired_width>MAX_GRID_X)display.interface_scaling_desired_width=MAX_GRID_X;
						}
					if(!token.compare("INTERFACE_SCALING_DESIRED_GRID_HEIGHT"))
						{
						display.interface_scaling_desired_height=convert_string_to_long(token2);
						if(display.interface_scaling_desired_height<MIN_GRID_Y)display.interface_scaling_desired_height=MIN_GRID_Y;
						if(display.interface_scaling_desired_height>MAX_GRID_Y)display.interface_scaling_desired_height=MAX_GRID_Y;
						}
					if(!token.compare("INTERFACE_SCALING_PERCENTAGE"))
						{
						display.interface_scaling_percentage=convert_string_to_long(token2);
						if(display.interface_scaling_percentage<100)display.interface_scaling_percentage=100;
						if(display.interface_scaling_percentage>400)display.interface_scaling_percentage=400;
						}
					if(token=="WINDOWED")
						{
						if(token2=="YES")
							{
							display.windowed=INIT_DISPLAY_WINDOW_TRUE;
							}
						if(token2=="NO")
							{
							display.windowed=INIT_DISPLAY_WINDOW_FALSE;
							}
						if(token2=="PROMPT")
							{
							display.windowed=INIT_DISPLAY_WINDOW_PROMPT;
							}
						}
					if (token == "TEXTURE_PARAM")
						{
						if (token2 == "NEAREST") 
							{
							display.filter_mode = InitDisplayFilterMode::NEAREST;
							}
						if (token2 == "LANCZOS")
							{
							display.filter_mode = InitDisplayFilterMode::LANCZOS;
							}
						if (token2 == "AUTO")
							{
							display.filter_mode = InitDisplayFilterMode::AUTO;
							}
						}
					if(!token.compare("SOUND"))
						{
						if(token2!="YES")
							{
							media.flag.add_flag(INIT_MEDIA_FLAG_SOUND_OFF);
							}
						else media.flag.remove_flag(INIT_MEDIA_FLAG_SOUND_OFF);
						}
					if(!token.compare("MASTER_VOLUME"))
						{
						media.volume_master=convert_string_to_long(token2);
						}
					if(!token.compare("MUSIC_VOLUME"))
						{
						media.volume_music_fort=convert_string_to_long(token2);
						}
					if(!token.compare("AMBIENCE_VOLUME"))
						{
						media.volume_ambience_fort=convert_string_to_long(token2);
						}
					if(!token.compare("SFX_VOLUME"))
						{
						media.volume_sfx_fort=convert_string_to_long(token2);
						}
					if(!token.compare("AVERAGE_TIME_BETWEEN_SONGS"))
						{
						media.time_between_songs_fort=convert_string_to_long(token2);
						if(media.time_between_songs_fort<10)media.time_between_songs_fort=10;
						if(media.time_between_songs_fort>600)media.time_between_songs_fort=600;
						}
					if(!token.compare("MUSIC_VOLUME_ADV"))
						{
						media.volume_music_adv=convert_string_to_long(token2);
						}
					if(!token.compare("AMBIENCE_VOLUME_ADV"))
						{
						media.volume_ambience_adv=convert_string_to_long(token2);
						}
					if(!token.compare("SFX_VOLUME_ADV"))
						{
						media.volume_sfx_adv=convert_string_to_long(token2);
						}
					if(!token.compare("KEY_HOLD_MS"))
						{
						input.hold_time=convert_string_to_long(token2);

						if(input.hold_time<100)input.hold_time=100;
						}
					if(token=="KEY_REPEAT_MS")
						{
						input.repeat_time=convert_string_to_long(token2);

						if(input.repeat_time<100)input.repeat_time=100;
						}
					if(token=="KEY_REPEAT_ACCEL_LIMIT")
						{
						input.repeat_accel_limit=convert_string_to_long(token2);
						if(input.repeat_accel_limit<1)input.repeat_accel_limit=1;
						}
					if(token=="KEY_REPEAT_ACCEL_START")
						{
						input.repeat_accel_start=convert_string_to_long(token2);
						}
					if(token=="MACRO_MS")
						{
						input.macro_time=convert_string_to_long(token2);

						if(input.macro_time<1)input.macro_time=1;
						}
					if(token=="RECENTER_INTERFACE_SHUTDOWN_MS")
						{
						input.pause_zoom_no_interface_ms=convert_string_to_long(token2);

						if(input.pause_zoom_no_interface_ms<0)input.pause_zoom_no_interface_ms=0;
						}
					if(token=="COMPRESSED_SAVES")
						{
						if(token2=="YES")
							{
							media.flag.add_flag(INIT_MEDIA_FLAG_COMPRESS_SAVES);
							}
						else media.flag.remove_flag(INIT_MEDIA_FLAG_COMPRESS_SAVES);
						}
					if(token=="USE_CLASSIC_ASCII")
						{
						if(token2=="YES")
							{
							display.flag.remove_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
							}
						else display.flag.add_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS);
						}
					if(token=="TITLE_SHOW_GRAPHICS_LOGOS")
						{
						if(token2=="YES")
							{
							display.flag.add_flag(INIT_DISPLAY_FLAG_LOAD_TITLE_GRAPHICS);
							}
						else display.flag.remove_flag(INIT_DISPLAY_FLAG_LOAD_TITLE_GRAPHICS);
						}
					if(token=="TITLE_MUSIC")
						{
						media.title_music_str=token2;
						}
					}
				}
			}
		fseed.close();
		}

	std::ifstream fseed2=filest("data/init/colors.txt").to_ifstream();
	if(fseed2.is_open())
		{
		string str;

		while(std::getline(fseed2,str))
			{
			if(str.length()>1)
				{
				string token;
				string token2;

				grab_token_string_pos(token,str,1);
				if(str.length()>=token.length()+2)
					{
					grab_token_string_pos(token2,str,(int32_t)token.length()+2);
					}

				if(!token.compare("BLACK_R"))
					{
					gps.uccolor[0][0]=convert_string_to_long(token2);
					gps.ccolor[0][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BLACK_G"))
					{
					gps.uccolor[0][1]=convert_string_to_long(token2);
					gps.ccolor[0][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BLACK_B"))
					{
					gps.uccolor[0][2]=convert_string_to_long(token2);
					gps.ccolor[0][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BLUE_R"))
					{
					gps.uccolor[1][0]=convert_string_to_long(token2);
					gps.ccolor[1][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BLUE_G"))
					{
					gps.uccolor[1][1]=convert_string_to_long(token2);
					gps.ccolor[1][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BLUE_B"))
					{
					gps.uccolor[1][2]=convert_string_to_long(token2);
					gps.ccolor[1][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("GREEN_R"))
					{
					gps.uccolor[2][0]=convert_string_to_long(token2);
					gps.ccolor[2][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("GREEN_G"))
					{
					gps.uccolor[2][1]=convert_string_to_long(token2);
					gps.ccolor[2][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("GREEN_B"))
					{
					gps.uccolor[2][2]=convert_string_to_long(token2);
					gps.ccolor[2][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("CYAN_R"))
					{
					gps.uccolor[3][0]=convert_string_to_long(token2);
					gps.ccolor[3][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("CYAN_G"))
					{
					gps.uccolor[3][1]=convert_string_to_long(token2);
					gps.ccolor[3][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("CYAN_B"))
					{
					gps.uccolor[3][2]=convert_string_to_long(token2);
					gps.ccolor[3][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("RED_R"))
					{
					gps.uccolor[4][0]=convert_string_to_long(token2);
					gps.ccolor[4][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("RED_G"))
					{
					gps.uccolor[4][1]=convert_string_to_long(token2);
					gps.ccolor[4][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("RED_B"))
					{
					gps.uccolor[4][2]=convert_string_to_long(token2);
					gps.ccolor[4][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("MAGENTA_R"))
					{
					gps.uccolor[5][0]=convert_string_to_long(token2);
					gps.ccolor[5][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("MAGENTA_G"))
					{
					gps.uccolor[5][1]=convert_string_to_long(token2);
					gps.ccolor[5][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("MAGENTA_B"))
					{
					gps.uccolor[5][2]=convert_string_to_long(token2);
					gps.ccolor[5][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BROWN_R"))
					{
					gps.uccolor[6][0]=convert_string_to_long(token2);
					gps.ccolor[6][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BROWN_G"))
					{
					gps.uccolor[6][1]=convert_string_to_long(token2);
					gps.ccolor[6][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("BROWN_B"))
					{
					gps.uccolor[6][2]=convert_string_to_long(token2);
					gps.ccolor[6][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGRAY_R"))
					{
					gps.uccolor[7][0]=convert_string_to_long(token2);
					gps.ccolor[7][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGRAY_G"))
					{
					gps.uccolor[7][1]=convert_string_to_long(token2);
					gps.ccolor[7][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGRAY_B"))
					{
					gps.uccolor[7][2]=convert_string_to_long(token2);
					gps.ccolor[7][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("DGRAY_R"))
					{
					gps.uccolor[8][0]=convert_string_to_long(token2);
					gps.ccolor[8][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("DGRAY_G"))
					{
					gps.uccolor[8][1]=convert_string_to_long(token2);
					gps.ccolor[8][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("DGRAY_B"))
					{
					gps.uccolor[8][2]=convert_string_to_long(token2);
					gps.ccolor[8][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LBLUE_R"))
					{
					gps.uccolor[9][0]=convert_string_to_long(token2);
					gps.ccolor[9][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LBLUE_G"))
					{
					gps.uccolor[9][1]=convert_string_to_long(token2);
					gps.ccolor[9][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LBLUE_B"))
					{
					gps.uccolor[9][2]=convert_string_to_long(token2);
					gps.ccolor[9][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGREEN_R"))
					{
					gps.uccolor[10][0]=convert_string_to_long(token2);
					gps.ccolor[10][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGREEN_G"))
					{
					gps.uccolor[10][1]=convert_string_to_long(token2);
					gps.ccolor[10][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LGREEN_B"))
					{
					gps.uccolor[10][2]=convert_string_to_long(token2);
					gps.ccolor[10][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LCYAN_R"))
					{
					gps.uccolor[11][0]=convert_string_to_long(token2);
					gps.ccolor[11][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LCYAN_G"))
					{
					gps.uccolor[11][1]=convert_string_to_long(token2);
					gps.ccolor[11][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LCYAN_B"))
					{
					gps.uccolor[11][2]=convert_string_to_long(token2);
					gps.ccolor[11][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LRED_R"))
					{
					gps.uccolor[12][0]=convert_string_to_long(token2);
					gps.ccolor[12][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LRED_G"))
					{
					gps.uccolor[12][1]=convert_string_to_long(token2);
					gps.ccolor[12][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LRED_B"))
					{
					gps.uccolor[12][2]=convert_string_to_long(token2);
					gps.ccolor[12][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LMAGENTA_R"))
					{
					gps.uccolor[13][0]=convert_string_to_long(token2);
					gps.ccolor[13][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LMAGENTA_G"))
					{
					gps.uccolor[13][1]=convert_string_to_long(token2);
					gps.ccolor[13][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("LMAGENTA_B"))
					{
					gps.uccolor[13][2]=convert_string_to_long(token2);
					gps.ccolor[13][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("YELLOW_R"))
					{
					gps.uccolor[14][0]=convert_string_to_long(token2);
					gps.ccolor[14][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("YELLOW_G"))
					{
					gps.uccolor[14][1]=convert_string_to_long(token2);
					gps.ccolor[14][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("YELLOW_B"))
					{
					gps.uccolor[14][2]=convert_string_to_long(token2);
					gps.ccolor[14][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("WHITE_R"))
					{
					gps.uccolor[15][0]=convert_string_to_long(token2);
					gps.ccolor[15][0]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("WHITE_G"))
					{
					gps.uccolor[15][1]=convert_string_to_long(token2);
					gps.ccolor[15][1]=(float)convert_string_to_long(token2)/255.0f;
					}
				if(!token.compare("WHITE_B"))
					{
					gps.uccolor[15][2]=convert_string_to_long(token2);
					gps.ccolor[15][2]=(float)convert_string_to_long(token2)/255.0f;
					}
				}
			}
		}
	fseed2.close();
        
#ifdef _DEBUG
        enabler.window.isFullScreen = FALSE;
#else
		/// TODO TODO REMOVE THIS AS SOON AS TEXT MODE'S BACK IN
	if(display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT))
		{
		display.flag.remove_flag(INIT_DISPLAY_FLAG_TEXT);
		puts("Text mode is not supported for now");
		}
        
        //FULL SCREEN QUERY, UNLESS IT'S ALREADY SET IN INIT

        if (!display.flag.has_flag(INIT_DISPLAY_FLAG_TEXT)) {
          if(enabler.command_line.empty())
            {
              if(display.windowed==INIT_DISPLAY_WINDOW_TRUE)
                {
                  enabler.fullscreen_state = 0;
                }
              else if(display.windowed==INIT_DISPLAY_WINDOW_FALSE)
                {
                  enabler.fullscreen_state = FULLSCREEN;
                }
			  else if (display.windowed == INIT_DISPLAY_WINDOW_EXCLUSIVE)
			  {
				  enabler.fullscreen_state = FULLSCREEN|EXCLUSIVE;
			  }
              else
                {
                  if (MessageBox (NULL, "Run in Fullscreen Mode?  You can set your preferences in data/init/init.txt.\rUnless you've changed your bindings, you can press F11 to toggle this setting any time.", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
                    enabler.fullscreen_state = 0; // If Not, Run In Windowed Mode
                  } else {
					enabler.fullscreen_state = FULLSCREEN;
                  }
                }
            }
          else enabler.fullscreen_state = 0;
        }
#endif

	enabler.textures.load_multi_pdim(basic_font,font.basic_font_texpos,16,16,true,&font.basic_font_dispx,&font.basic_font_dispy);
	enabler.textures.load_multi_pdim(small_font,font.small_font_texpos,16,16,true,&font.small_font_dispx,&font.small_font_dispy);
	enabler.textures.load_multi_pdim(large_font,font.large_font_texpos,16,16,true,&font.large_font_dispx,&font.large_font_dispy);

#ifdef ULTRAWIDE_HACK
		font.basic_font_dispx=4;
		font.basic_font_dispy=12;
		font.small_font_dispx=4;
		font.small_font_dispy=12;
#endif

	if(init.display.flag.has_flag(INIT_DISPLAY_FLAG_LOAD_TITLE_GRAPHICS))
		{
		gps.tex[TEXTURE_MOUSE]=enabler.textures.load("data/art/mouse.png", true);
		gps.tex[TEXTURE_PUBLISHER]=enabler.textures.load("data/art/pixel_kf.png", true);
		gps.tex[TEXTURE_PUBLISHER_SMALL]=enabler.textures.load("data/art/pixel_kf_small.png", true);
		gps.tex[TEXTURE_PUBLISHER_TINY]=enabler.textures.load("data/art/pixel_kf_tiny.png", true);
		gps.tex[TEXTURE_TITLE]=enabler.textures.load("data/art/df_logo.png", true);
		gps.tex[TEXTURE_TITLE_BACKGROUND]=enabler.textures.load("data/art/title_background.png", true);
		gps.tex[TEXTURE_TITLE_ADV]=enabler.textures.load("data/art/title_adv.png", true);
		gps.tex[TEXTURE_DEVELOPER]=enabler.textures.load("data/art/bay12.png", true);
		gps.tex[TEXTURE_DEVELOPER_SMALL]=enabler.textures.load("data/art/bay12_small.png", true);
		gps.tex[TEXTURE_DEVELOPER_TINY]=enabler.textures.load("data/art/bay12_tiny.png", true);
		gps.tex[TEXTURE_SOUND_SYSTEM]=enabler.textures.load("data/art/fmod.png", true);
		}

	gps.last_display_background=TEXTURE_TITLE_BACKGROUND;

	long d3,d4;
	enabler.textures.load_multi_pdim("data/art/load_bar.png",load_bar_texpos,6,1,true,&d3,&d4);
	int32_t intro_button_total=(INIT_INTRO_BUTTON_TEXTURENUM/9)*3;
	enabler.textures.load_multi_pdim("data/art/intro_button.png",intro_button_texpos,intro_button_total,3,true,&d3,&d4);
		int32_t dx,dy;
		for(dy=0;dy<3;++dy)
			{
			for(dx=0;dx<3;++dx)
				{
				texpos_neutral_intro_button[dx+dy*3]=intro_button_texpos[dx+dy*intro_button_total];
				texpos_confirm_intro_button[dx+dy*3]=intro_button_texpos[dx+1*3+dy*intro_button_total];
				texpos_cancel_intro_button[dx+dy*3]=intro_button_texpos[dx+2*3+dy*intro_button_total];
				texpos_selected_intro_button[dx+dy*3]=intro_button_texpos[dx+3*3+dy*intro_button_total];
				texpos_unselected_intro_button[dx+dy*3]=intro_button_texpos[dx+4*3+dy*intro_button_total];
				texpos_open_list_button[dx+dy*3]=intro_button_texpos[dx+5*3+dy*intro_button_total];
				texpos_increase_button[dx+dy*3]=intro_button_texpos[dx+6*3+dy*intro_button_total];
				texpos_decrease_button[dx+dy*3]=intro_button_texpos[dx+7*3+dy*intro_button_total];
				texpos_nullify_button[dx+dy*3]=intro_button_texpos[dx+8*3+dy*intro_button_total];
				texpos_left_arrow_button[dx+dy*3]=intro_button_texpos[dx+9*3+dy*intro_button_total];
				texpos_right_arrow_button[dx+dy*3]=intro_button_texpos[dx+10*3+dy*intro_button_total];
				texpos_up_arrow_button[dx+dy*3]=intro_button_texpos[dx+11*3+dy*intro_button_total];
				texpos_down_arrow_button[dx+dy*3]=intro_button_texpos[dx+12*3+dy*intro_button_total];
				}
			}
	enabler.textures.load_multi_pdim("data/art/border.png",border_texpos,7,3,true,&d3,&d4);
		texpos_border_join_n=border_texpos[3+1*7];
		texpos_border_join_s=border_texpos[4+1*7];
		texpos_border_join_w=border_texpos[5+1*7];
		texpos_border_join_e=border_texpos[6+1*7];
		texpos_border_inside_nswe=border_texpos[3+0*7];
		texpos_border_inside_nsw=border_texpos[3+2*7];
		texpos_border_inside_nse=border_texpos[4+2*7];
		texpos_border_inside_nwe=border_texpos[5+2*7];
		texpos_border_inside_swe=border_texpos[6+2*7];
		texpos_border_inside_ns=border_texpos[4+0*7];
		texpos_border_inside_we=border_texpos[5+0*7];
		texpos_border_nw=border_texpos[0+0*7];
		texpos_border_n=border_texpos[1+0*7];
		texpos_border_ne=border_texpos[2+0*7];
		texpos_border_w=border_texpos[0+1*7];
		texpos_border_interior=border_texpos[1+1*7];
		texpos_border_e=border_texpos[2+1*7];
		texpos_border_sw=border_texpos[0+2*7];
		texpos_border_s=border_texpos[1+2*7];
		texpos_border_se=border_texpos[2+2*7];
	enabler.textures.load_multi_pdim("data/art/scrollbar.png",scrollbar_texpos,12,4,true,&d3,&d4);
		texpos_scrollbar[0][0]=scrollbar_texpos[0+0*12];
		texpos_scrollbar[1][0]=scrollbar_texpos[1+0*12];
		texpos_scrollbar[0][1]=scrollbar_texpos[0+1*12];
		texpos_scrollbar[1][1]=scrollbar_texpos[1+1*12];
		texpos_scrollbar[0][2]=scrollbar_texpos[0+2*12];
		texpos_scrollbar[1][2]=scrollbar_texpos[1+2*12];
		texpos_scrollbar_up_hover[0]=scrollbar_texpos[2+0*12];
		texpos_scrollbar_up_hover[1]=scrollbar_texpos[3+0*12];
		texpos_scrollbar_up_pressed[0]=scrollbar_texpos[4+0*12];
		texpos_scrollbar_up_pressed[1]=scrollbar_texpos[5+0*12];
		texpos_scrollbar_down_hover[0]=scrollbar_texpos[2+1*12];
		texpos_scrollbar_down_hover[1]=scrollbar_texpos[3+1*12];
		texpos_scrollbar_down_pressed[0]=scrollbar_texpos[4+1*12];
		texpos_scrollbar_down_pressed[1]=scrollbar_texpos[5+1*12];
		texpos_scrollbar_small_scroller[0][0]=scrollbar_texpos[2+2*12];
		texpos_scrollbar_small_scroller[1][0]=scrollbar_texpos[3+2*12];
		texpos_scrollbar_small_scroller[0][1]=scrollbar_texpos[2+3*12];
		texpos_scrollbar_small_scroller[1][1]=scrollbar_texpos[3+3*12];
		texpos_scrollbar_small_scroller_hover[0][0]=scrollbar_texpos[4+2*12];
		texpos_scrollbar_small_scroller_hover[1][0]=scrollbar_texpos[5+2*12];
		texpos_scrollbar_small_scroller_hover[0][1]=scrollbar_texpos[4+3*12];
		texpos_scrollbar_small_scroller_hover[1][1]=scrollbar_texpos[5+3*12];
		texpos_scrollbar_top_scroller[0]=scrollbar_texpos[6+0*12];
		texpos_scrollbar_top_scroller[1]=scrollbar_texpos[7+0*12];
		texpos_scrollbar_top_scroller_hover[0]=scrollbar_texpos[8+0*12];
		texpos_scrollbar_top_scroller_hover[1]=scrollbar_texpos[9+0*12];
		texpos_scrollbar_center_scroller[0]=scrollbar_texpos[6+1*12];
		texpos_scrollbar_center_scroller[1]=scrollbar_texpos[7+1*12];
		texpos_scrollbar_center_scroller_hover[0]=scrollbar_texpos[8+1*12];
		texpos_scrollbar_center_scroller_hover[1]=scrollbar_texpos[9+1*12];
		texpos_scrollbar_blank_scroller[0]=scrollbar_texpos[6+2*12];
		texpos_scrollbar_blank_scroller[1]=scrollbar_texpos[7+2*12];
		texpos_scrollbar_blank_scroller_hover[0]=scrollbar_texpos[8+2*12];
		texpos_scrollbar_blank_scroller_hover[1]=scrollbar_texpos[9+2*12];
		texpos_scrollbar_bottom_scroller[0]=scrollbar_texpos[6+3*12];
		texpos_scrollbar_bottom_scroller[1]=scrollbar_texpos[7+3*12];
		texpos_scrollbar_bottom_scroller_hover[0]=scrollbar_texpos[8+3*12];
		texpos_scrollbar_bottom_scroller_hover[1]=scrollbar_texpos[9+3*12];
		texpos_scrollbar_offcenter_scroller[0][0]=scrollbar_texpos[10+0*12];
		texpos_scrollbar_offcenter_scroller[1][0]=scrollbar_texpos[11+0*12];
		texpos_scrollbar_offcenter_scroller[0][1]=scrollbar_texpos[10+1*12];
		texpos_scrollbar_offcenter_scroller[1][1]=scrollbar_texpos[11+1*12];
		texpos_scrollbar_offcenter_scroller_hover[0][0]=scrollbar_texpos[10+2*12];
		texpos_scrollbar_offcenter_scroller_hover[1][0]=scrollbar_texpos[11+2*12];
		texpos_scrollbar_offcenter_scroller_hover[0][1]=scrollbar_texpos[10+3*12];
		texpos_scrollbar_offcenter_scroller_hover[1][1]=scrollbar_texpos[11+3*12];
	enabler.textures.load_multi_pdim("data/art/filter.png",filter_texpos,10,3,true,&d3,&d4);
		texpos_button_filter[0][0]=filter_texpos[0+0*10];
		texpos_button_filter[1][0]=filter_texpos[1+0*10];
		texpos_button_filter[2][0]=filter_texpos[2+0*10];
		texpos_button_filter[3][0]=filter_texpos[3+0*10];
		texpos_button_filter[4][0]=filter_texpos[4+0*10];
		texpos_button_filter[5][0]=filter_texpos[5+0*10];
		texpos_button_filter[0][1]=filter_texpos[0+1*10];
		texpos_button_filter[1][1]=filter_texpos[1+1*10];
		texpos_button_filter[2][1]=filter_texpos[2+1*10];
		texpos_button_filter[3][1]=filter_texpos[3+1*10];
		texpos_button_filter[4][1]=filter_texpos[4+1*10];
		texpos_button_filter[5][1]=filter_texpos[5+1*10];
		texpos_button_filter[0][2]=filter_texpos[0+2*10];
		texpos_button_filter[1][2]=filter_texpos[1+2*10];
		texpos_button_filter[2][2]=filter_texpos[2+2*10];
		texpos_button_filter[3][2]=filter_texpos[3+2*10];
		texpos_button_filter[4][2]=filter_texpos[4+2*10];
		texpos_button_filter[5][2]=filter_texpos[5+2*10];
		texpos_button_filter_name[0][0]=filter_texpos[6+0*10];
		texpos_button_filter_name[1][0]=filter_texpos[7+0*10];
		texpos_button_filter_name[2][0]=filter_texpos[8+0*10];
		texpos_button_filter_name[3][0]=filter_texpos[9+0*10];
		texpos_button_filter_name[0][1]=filter_texpos[6+1*10];
		texpos_button_filter_name[1][1]=filter_texpos[7+1*10];
		texpos_button_filter_name[2][1]=filter_texpos[8+1*10];
		texpos_button_filter_name[3][1]=filter_texpos[9+1*10];
		texpos_button_filter_name[0][2]=filter_texpos[6+2*10];
		texpos_button_filter_name[1][2]=filter_texpos[7+2*10];
		texpos_button_filter_name[2][2]=filter_texpos[8+2*10];
		texpos_button_filter_name[3][2]=filter_texpos[9+2*10];
	enabler.textures.load_multi_pdim("data/art/tabs.png",tabs_texpos,10,2,true,&d3,&d4);
		texpos_tab_unselected[0][0]=tabs_texpos[0+0*10];
		texpos_tab_unselected[1][0]=tabs_texpos[1+0*10];
		texpos_tab_unselected[2][0]=tabs_texpos[2+0*10];
		texpos_tab_unselected[3][0]=tabs_texpos[3+0*10];
		texpos_tab_unselected[4][0]=tabs_texpos[4+0*10];
		texpos_tab_unselected[0][1]=tabs_texpos[0+1*10];
		texpos_tab_unselected[1][1]=tabs_texpos[1+1*10];
		texpos_tab_unselected[2][1]=tabs_texpos[2+1*10];
		texpos_tab_unselected[3][1]=tabs_texpos[3+1*10];
		texpos_tab_unselected[4][1]=tabs_texpos[4+1*10];
		texpos_tab_selected[0][0]=tabs_texpos[5+0*10];
		texpos_tab_selected[1][0]=tabs_texpos[6+0*10];
		texpos_tab_selected[2][0]=tabs_texpos[7+0*10];
		texpos_tab_selected[3][0]=tabs_texpos[8+0*10];
		texpos_tab_selected[4][0]=tabs_texpos[9+0*10];
		texpos_tab_selected[0][1]=tabs_texpos[5+1*10];
		texpos_tab_selected[1][1]=tabs_texpos[6+1*10];
		texpos_tab_selected[2][1]=tabs_texpos[7+1*10];
		texpos_tab_selected[3][1]=tabs_texpos[8+1*10];
		texpos_tab_selected[4][1]=tabs_texpos[9+1*10];
	enabler.textures.load_multi_pdim("data/art/sort.png",sort_texpos,11,2,true,&d3,&d4);
		texpos_sort_ascending_inactive[0]=sort_texpos[0+0*11];
		texpos_sort_ascending_inactive[1]=sort_texpos[1+0*11];
		texpos_sort_ascending_inactive[2]=sort_texpos[2+0*11];
		texpos_sort_ascending_inactive[3]=sort_texpos[3+0*11];
		texpos_sort_descending_inactive[0]=sort_texpos[4+0*11];
		texpos_sort_descending_inactive[1]=sort_texpos[5+0*11];
		texpos_sort_descending_inactive[2]=sort_texpos[6+0*11];
		texpos_sort_descending_inactive[3]=sort_texpos[7+0*11];
		texpos_sort_text_inactive[0]=sort_texpos[8+0*11];
		texpos_sort_text_inactive[1]=sort_texpos[9+0*11];
		texpos_sort_text_inactive[2]=sort_texpos[10+0*11];
		texpos_sort_ascending_active[0]=sort_texpos[0+1*11];
		texpos_sort_ascending_active[1]=sort_texpos[1+1*11];
		texpos_sort_ascending_active[2]=sort_texpos[2+1*11];
		texpos_sort_ascending_active[3]=sort_texpos[3+1*11];
		texpos_sort_descending_active[0]=sort_texpos[4+1*11];
		texpos_sort_descending_active[1]=sort_texpos[5+1*11];
		texpos_sort_descending_active[2]=sort_texpos[6+1*11];
		texpos_sort_descending_active[3]=sort_texpos[7+1*11];
		texpos_sort_text_active[0]=sort_texpos[8+1*11];
		texpos_sort_text_active[1]=sort_texpos[9+1*11];
		texpos_sort_text_active[2]=sort_texpos[10+1*11];

		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_LEFT_FULL]=convert_raw_to_ascii_texpos('[',7,0,1);
		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_MID_FULL]=convert_raw_to_ascii_texpos(254,7,0,1);
		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_RIGHT_FULL]=convert_raw_to_ascii_texpos(']',7,0,1);
		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_LEFT_EMPTY]=convert_raw_to_ascii_texpos('[',7,0,0);
		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_MID_EMPTY]=convert_raw_to_ascii_texpos(250,7,0,0);
		classic_load_bar_texpos[INIT_LOAD_BAR_TEXTURE_RIGHT_EMPTY]=convert_raw_to_ascii_texpos(']',7,0,0);

		classic_texpos_neutral_intro_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,0);
		classic_texpos_neutral_intro_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_neutral_intro_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,0);
		classic_texpos_neutral_intro_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_neutral_intro_button[1+1*3]=convert_raw_to_ascii_texpos(' ',7,0,0);
		classic_texpos_neutral_intro_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_neutral_intro_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,0);
		classic_texpos_neutral_intro_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_neutral_intro_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,0);

		classic_texpos_confirm_intro_button[0+0*3]=convert_raw_to_ascii_texpos(218,2,0,1);
		classic_texpos_confirm_intro_button[0+1*3]=convert_raw_to_ascii_texpos(179,2,0,1);
		classic_texpos_confirm_intro_button[0+2*3]=convert_raw_to_ascii_texpos(192,2,0,1);
		classic_texpos_confirm_intro_button[1+0*3]=convert_raw_to_ascii_texpos(196,2,0,1);
		classic_texpos_confirm_intro_button[1+1*3]=convert_raw_to_ascii_texpos(' ',2,0,1);
		classic_texpos_confirm_intro_button[1+2*3]=convert_raw_to_ascii_texpos(196,2,0,1);
		classic_texpos_confirm_intro_button[2+0*3]=convert_raw_to_ascii_texpos(191,2,0,1);
		classic_texpos_confirm_intro_button[2+1*3]=convert_raw_to_ascii_texpos(179,2,0,1);
		classic_texpos_confirm_intro_button[2+2*3]=convert_raw_to_ascii_texpos(217,2,0,1);

		classic_texpos_cancel_intro_button[0+0*3]=convert_raw_to_ascii_texpos(218,4,0,1);
		classic_texpos_cancel_intro_button[0+1*3]=convert_raw_to_ascii_texpos(179,4,0,1);
		classic_texpos_cancel_intro_button[0+2*3]=convert_raw_to_ascii_texpos(192,4,0,1);
		classic_texpos_cancel_intro_button[1+0*3]=convert_raw_to_ascii_texpos(196,4,0,1);
		classic_texpos_cancel_intro_button[1+1*3]=convert_raw_to_ascii_texpos(' ',4,0,1);
		classic_texpos_cancel_intro_button[1+2*3]=convert_raw_to_ascii_texpos(196,4,0,1);
		classic_texpos_cancel_intro_button[2+0*3]=convert_raw_to_ascii_texpos(191,4,0,1);
		classic_texpos_cancel_intro_button[2+1*3]=convert_raw_to_ascii_texpos(179,4,0,1);
		classic_texpos_cancel_intro_button[2+2*3]=convert_raw_to_ascii_texpos(217,4,0,1);

		classic_texpos_selected_intro_button[0+0*3]=convert_raw_to_ascii_texpos(218,3,0,1);
		classic_texpos_selected_intro_button[0+1*3]=convert_raw_to_ascii_texpos(179,3,0,1);
		classic_texpos_selected_intro_button[0+2*3]=convert_raw_to_ascii_texpos(192,3,0,1);
		classic_texpos_selected_intro_button[1+0*3]=convert_raw_to_ascii_texpos(196,3,0,1);
		classic_texpos_selected_intro_button[1+1*3]=convert_raw_to_ascii_texpos(' ',3,0,1);
		classic_texpos_selected_intro_button[1+2*3]=convert_raw_to_ascii_texpos(196,3,0,1);
		classic_texpos_selected_intro_button[2+0*3]=convert_raw_to_ascii_texpos(191,3,0,1);
		classic_texpos_selected_intro_button[2+1*3]=convert_raw_to_ascii_texpos(179,3,0,1);
		classic_texpos_selected_intro_button[2+2*3]=convert_raw_to_ascii_texpos(217,3,0,1);

		classic_texpos_unselected_intro_button[0+0*3]=convert_raw_to_ascii_texpos(218,3,0,0);
		classic_texpos_unselected_intro_button[0+1*3]=convert_raw_to_ascii_texpos(179,3,0,0);
		classic_texpos_unselected_intro_button[0+2*3]=convert_raw_to_ascii_texpos(192,3,0,0);
		classic_texpos_unselected_intro_button[1+0*3]=convert_raw_to_ascii_texpos(196,3,0,0);
		classic_texpos_unselected_intro_button[1+1*3]=convert_raw_to_ascii_texpos(' ',3,0,0);
		classic_texpos_unselected_intro_button[1+2*3]=convert_raw_to_ascii_texpos(196,3,0,0);
		classic_texpos_unselected_intro_button[2+0*3]=convert_raw_to_ascii_texpos(191,3,0,0);
		classic_texpos_unselected_intro_button[2+1*3]=convert_raw_to_ascii_texpos(179,3,0,0);
		classic_texpos_unselected_intro_button[2+2*3]=convert_raw_to_ascii_texpos(217,3,0,0);

		classic_texpos_open_list_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_open_list_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_open_list_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_open_list_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_open_list_button[1+1*3]=convert_raw_to_ascii_texpos('V',7,0,1);
		classic_texpos_open_list_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_open_list_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_open_list_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_open_list_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_increase_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_increase_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_increase_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_increase_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_increase_button[1+1*3]=convert_raw_to_ascii_texpos('+',7,0,1);
		classic_texpos_increase_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_increase_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_increase_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_increase_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_decrease_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_decrease_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_decrease_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_decrease_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_decrease_button[1+1*3]=convert_raw_to_ascii_texpos('-',7,0,1);
		classic_texpos_decrease_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_decrease_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_decrease_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_decrease_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_nullify_button[0+0*3]=convert_raw_to_ascii_texpos(218,4,0,1);
		classic_texpos_nullify_button[0+1*3]=convert_raw_to_ascii_texpos(179,4,0,1);
		classic_texpos_nullify_button[0+2*3]=convert_raw_to_ascii_texpos(192,4,0,1);
		classic_texpos_nullify_button[1+0*3]=convert_raw_to_ascii_texpos(196,4,0,1);
		classic_texpos_nullify_button[1+1*3]=convert_raw_to_ascii_texpos('X',4,0,1);
		classic_texpos_nullify_button[1+2*3]=convert_raw_to_ascii_texpos(196,4,0,1);
		classic_texpos_nullify_button[2+0*3]=convert_raw_to_ascii_texpos(191,4,0,1);
		classic_texpos_nullify_button[2+1*3]=convert_raw_to_ascii_texpos(179,4,0,1);
		classic_texpos_nullify_button[2+2*3]=convert_raw_to_ascii_texpos(217,4,0,1);

		classic_texpos_left_arrow_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_left_arrow_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_left_arrow_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_left_arrow_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_left_arrow_button[1+1*3]=convert_raw_to_ascii_texpos(27,7,0,1);
		classic_texpos_left_arrow_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_left_arrow_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_left_arrow_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_left_arrow_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_right_arrow_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_right_arrow_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_right_arrow_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_right_arrow_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_right_arrow_button[1+1*3]=convert_raw_to_ascii_texpos(26,7,0,1);
		classic_texpos_right_arrow_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_right_arrow_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_right_arrow_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_right_arrow_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_up_arrow_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_up_arrow_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_up_arrow_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_up_arrow_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_up_arrow_button[1+1*3]=convert_raw_to_ascii_texpos(24,7,0,1);
		classic_texpos_up_arrow_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_up_arrow_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_up_arrow_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_up_arrow_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_down_arrow_button[0+0*3]=convert_raw_to_ascii_texpos(218,7,0,1);
		classic_texpos_down_arrow_button[0+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_down_arrow_button[0+2*3]=convert_raw_to_ascii_texpos(192,7,0,1);
		classic_texpos_down_arrow_button[1+0*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_down_arrow_button[1+1*3]=convert_raw_to_ascii_texpos(25,7,0,1);
		classic_texpos_down_arrow_button[1+2*3]=convert_raw_to_ascii_texpos(196,7,0,1);
		classic_texpos_down_arrow_button[2+0*3]=convert_raw_to_ascii_texpos(191,7,0,1);
		classic_texpos_down_arrow_button[2+1*3]=convert_raw_to_ascii_texpos(179,7,0,1);
		classic_texpos_down_arrow_button[2+2*3]=convert_raw_to_ascii_texpos(217,7,0,1);

		classic_texpos_border_nw=convert_raw_to_ascii_texpos(218,6,0,1);
		classic_texpos_border_n=convert_raw_to_ascii_texpos(196,6,0,1);
		classic_texpos_border_ne=convert_raw_to_ascii_texpos(191,6,0,1);
		classic_texpos_border_w=convert_raw_to_ascii_texpos(179,6,0,1);
		classic_texpos_border_interior=convert_raw_to_ascii_texpos(' ',6,0,1);
		classic_texpos_border_e=convert_raw_to_ascii_texpos(179,6,0,1);
		classic_texpos_border_sw=convert_raw_to_ascii_texpos(192,6,0,1);
		classic_texpos_border_s=convert_raw_to_ascii_texpos(196,6,0,1);
		classic_texpos_border_se=convert_raw_to_ascii_texpos(217,6,0,1);
		classic_texpos_border_join_n=convert_raw_to_ascii_texpos(194,6,0,1);
		classic_texpos_border_join_s=convert_raw_to_ascii_texpos(193,6,0,1);
		classic_texpos_border_join_w=convert_raw_to_ascii_texpos(195,6,0,1);
		classic_texpos_border_join_e=convert_raw_to_ascii_texpos(180,6,0,1);
		classic_texpos_border_inside_nswe=convert_raw_to_ascii_texpos(197,6,0,1);
		classic_texpos_border_inside_nsw=convert_raw_to_ascii_texpos(180,6,0,1);
		classic_texpos_border_inside_nse=convert_raw_to_ascii_texpos(195,6,0,1);
		classic_texpos_border_inside_nwe=convert_raw_to_ascii_texpos(193,6,0,1);
		classic_texpos_border_inside_swe=convert_raw_to_ascii_texpos(194,6,0,1);
		classic_texpos_border_inside_ns=convert_raw_to_ascii_texpos(179,6,0,1);
		classic_texpos_border_inside_we=convert_raw_to_ascii_texpos(196,6,0,1);

		classic_texpos_scrollbar[0][0]=convert_raw_to_ascii_texpos('/',3,0,0);
		classic_texpos_scrollbar[0][1]=convert_raw_to_ascii_texpos(176,0,0,1);
		classic_texpos_scrollbar[0][2]=convert_raw_to_ascii_texpos('\\',3,0,0);
		classic_texpos_scrollbar[1][0]=convert_raw_to_ascii_texpos('\\',3,0,0);
		classic_texpos_scrollbar[1][1]=convert_raw_to_ascii_texpos(176,0,0,1);
		classic_texpos_scrollbar[1][2]=convert_raw_to_ascii_texpos('/',3,0,0);
		classic_texpos_scrollbar_up_hover[0]=convert_raw_to_ascii_texpos('/',3,0,1);
		classic_texpos_scrollbar_up_hover[1]=convert_raw_to_ascii_texpos('\\',3,0,1);
		classic_texpos_scrollbar_up_pressed[0]=convert_raw_to_ascii_texpos('/',3,0,1);
		classic_texpos_scrollbar_up_pressed[1]=convert_raw_to_ascii_texpos('\\',3,0,1);
		classic_texpos_scrollbar_down_hover[0]=convert_raw_to_ascii_texpos('\\',3,0,1);
		classic_texpos_scrollbar_down_hover[1]=convert_raw_to_ascii_texpos('/',3,0,1);
		classic_texpos_scrollbar_down_pressed[0]=convert_raw_to_ascii_texpos('\\',3,0,1);
		classic_texpos_scrollbar_down_pressed[1]=convert_raw_to_ascii_texpos('/',3,0,1);
		classic_texpos_scrollbar_small_scroller[0][0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_small_scroller[0][1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_small_scroller[1][0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_small_scroller[1][1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_small_scroller_hover[0][0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_small_scroller_hover[0][1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_small_scroller_hover[1][0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_small_scroller_hover[1][1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_top_scroller[0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_top_scroller[1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_top_scroller_hover[0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_top_scroller_hover[1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_bottom_scroller[0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_bottom_scroller[1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_bottom_scroller_hover[0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_bottom_scroller_hover[1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_blank_scroller[0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_blank_scroller[1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_blank_scroller_hover[0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_blank_scroller_hover[1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_center_scroller[0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_center_scroller[1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_center_scroller_hover[0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_center_scroller_hover[1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_offcenter_scroller[0][0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_offcenter_scroller[0][1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_offcenter_scroller[1][0]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_offcenter_scroller[1][1]=convert_raw_to_ascii_texpos(219,3,0,0);
		classic_texpos_scrollbar_offcenter_scroller_hover[0][0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_offcenter_scroller_hover[0][1]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_offcenter_scroller_hover[1][0]=convert_raw_to_ascii_texpos(219,3,0,1);
		classic_texpos_scrollbar_offcenter_scroller_hover[1][1]=convert_raw_to_ascii_texpos(219,3,0,1);

		classic_texpos_button_filter[0][0]=convert_raw_to_ascii_texpos(218,7,0,0);
		classic_texpos_button_filter[0][1]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_button_filter[0][2]=convert_raw_to_ascii_texpos(192,7,0,0);
		classic_texpos_button_filter[1][0]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[1][1]=convert_raw_to_ascii_texpos(' ',7,0,0);
		classic_texpos_button_filter[1][2]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[2][0]=convert_raw_to_ascii_texpos(194,7,0,0);
		classic_texpos_button_filter[2][1]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_button_filter[2][2]=convert_raw_to_ascii_texpos(193,7,0,0);
		classic_texpos_button_filter[3][0]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[3][1]=convert_raw_to_ascii_texpos('-',7,0,1);
		classic_texpos_button_filter[3][2]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[4][0]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[4][1]=convert_raw_to_ascii_texpos('O',7,0,1);
		classic_texpos_button_filter[4][2]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter[5][0]=convert_raw_to_ascii_texpos(191,7,0,0);
		classic_texpos_button_filter[5][1]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_button_filter[5][2]=convert_raw_to_ascii_texpos(217,7,0,0);

		classic_texpos_button_filter_name[0][0]=convert_raw_to_ascii_texpos(218,7,0,0);
		classic_texpos_button_filter_name[0][1]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_button_filter_name[0][2]=convert_raw_to_ascii_texpos(192,7,0,0);
		classic_texpos_button_filter_name[1][0]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter_name[1][1]=convert_raw_to_ascii_texpos('_',0,0,1);
		classic_texpos_button_filter_name[1][2]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter_name[2][0]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter_name[2][1]=convert_raw_to_ascii_texpos(13,7,0,1);
		classic_texpos_button_filter_name[2][2]=convert_raw_to_ascii_texpos(196,7,0,0);
		classic_texpos_button_filter_name[3][0]=convert_raw_to_ascii_texpos(191,7,0,0);
		classic_texpos_button_filter_name[3][1]=convert_raw_to_ascii_texpos(179,7,0,0);
		classic_texpos_button_filter_name[3][2]=convert_raw_to_ascii_texpos(217,7,0,0);

		classic_texpos_tab_unselected[0][0]=convert_raw_to_ascii_texpos(' ',6,0,0);
		classic_texpos_tab_unselected[0][1]=convert_raw_to_ascii_texpos('/',6,0,0);
		classic_texpos_tab_unselected[1][0]=convert_raw_to_ascii_texpos('/',6,0,0);
		classic_texpos_tab_unselected[1][1]=convert_raw_to_ascii_texpos('-',6,0,0);
		classic_texpos_tab_unselected[2][0]=convert_raw_to_ascii_texpos('-',6,0,0);
		classic_texpos_tab_unselected[2][1]=convert_raw_to_ascii_texpos('-',6,0,0);
		classic_texpos_tab_unselected[3][0]=convert_raw_to_ascii_texpos('\\',6,0,0);
		classic_texpos_tab_unselected[3][1]=convert_raw_to_ascii_texpos('-',6,0,0);
		classic_texpos_tab_unselected[4][0]=convert_raw_to_ascii_texpos(' ',6,0,0);
		classic_texpos_tab_unselected[4][1]=convert_raw_to_ascii_texpos('\\',6,0,0);

		classic_texpos_tab_selected[0][0]=convert_raw_to_ascii_texpos(' ',6,0,1);
		classic_texpos_tab_selected[0][1]=convert_raw_to_ascii_texpos('/',6,0,1);
		classic_texpos_tab_selected[1][0]=convert_raw_to_ascii_texpos('/',6,0,1);
		classic_texpos_tab_selected[1][1]=convert_raw_to_ascii_texpos('-',6,0,1);
		classic_texpos_tab_selected[2][0]=convert_raw_to_ascii_texpos('-',6,0,1);
		classic_texpos_tab_selected[2][1]=convert_raw_to_ascii_texpos('-',6,0,1);
		classic_texpos_tab_selected[3][0]=convert_raw_to_ascii_texpos('\\',6,0,1);
		classic_texpos_tab_selected[3][1]=convert_raw_to_ascii_texpos('-',6,0,1);
		classic_texpos_tab_selected[4][0]=convert_raw_to_ascii_texpos(' ',6,0,1);
		classic_texpos_tab_selected[4][1]=convert_raw_to_ascii_texpos('\\',6,0,1);

		classic_texpos_sort_ascending_inactive[0]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_ascending_inactive[1]=convert_raw_to_ascii_texpos(31,0,7,0);
		classic_texpos_sort_ascending_inactive[2]=convert_raw_to_ascii_texpos(31,0,7,0);
		classic_texpos_sort_ascending_inactive[3]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_ascending_active[0]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_ascending_active[1]=convert_raw_to_ascii_texpos(31,7,7,1);
		classic_texpos_sort_ascending_active[2]=convert_raw_to_ascii_texpos(31,7,7,1);
		classic_texpos_sort_ascending_active[3]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_descending_inactive[0]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_descending_inactive[1]=convert_raw_to_ascii_texpos(30,0,7,0);
		classic_texpos_sort_descending_inactive[2]=convert_raw_to_ascii_texpos(30,0,7,0);
		classic_texpos_sort_descending_inactive[3]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_descending_active[0]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_descending_active[1]=convert_raw_to_ascii_texpos(30,7,7,1);
		classic_texpos_sort_descending_active[2]=convert_raw_to_ascii_texpos(30,7,7,1);
		classic_texpos_sort_descending_active[3]=convert_raw_to_ascii_texpos(' ',0,7,0);
		classic_texpos_sort_text_active[0]=convert_raw_to_ascii_texpos(' ',0,0,0);
		classic_texpos_sort_text_active[1]=convert_raw_to_ascii_texpos(' ',0,0,0);
		classic_texpos_sort_text_active[2]=convert_raw_to_ascii_texpos(' ',0,0,0);
		classic_texpos_sort_text_inactive[0]=convert_raw_to_ascii_texpos(' ',0,0,0);
		classic_texpos_sort_text_inactive[1]=convert_raw_to_ascii_texpos(' ',0,0,0);
		classic_texpos_sort_text_inactive[2]=convert_raw_to_ascii_texpos(' ',0,0,0);

	font.create_derived_font_textures();

	//************************************ MOVE BLACK BACKGROUND TO RAWS
	string black_background_filename="data/art/black_background.bmp";
	long dummy_dispx,dummy_dispy;
	enabler.textures.load_multi_pdim(black_background_filename,gps.black_background_texpos,1,1,false,&dummy_dispx,&dummy_dispy);
	//int32_t black_background_texpos;

	enabler.textures.set_init_texture_size();
	
	if(!display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
		{
		swap_basic_sets();
		}

	enabler.flag|=ENABLERFLAG_BASIC_TEXT;

	// compute the desired window size, if set to auto
		//got rid of this - this is computing the window size if they provide a desired dimension in characters, like 80x25
		/*
        if (display.desired_windowed_width < MAX_GRID_X && display.desired_windowed_height < MAX_GRID_Y) {
          int dimx = MAX(display.desired_windowed_width,80);
          int dimy = MAX(display.desired_windowed_height,25);
          display.desired_windowed_width = font.basic_font_dispx * dimx;
          display.desired_windowed_height = font.basic_font_dispy * dimy;
        }
		*/
}

void init_fontst::create_derived_font_textures()
{
	int32_t f,g;
	for(f=0;f<3;++f)
		{
		for(g=0;g<256;++g)
			{
			long src_tx;
			long *dst_top,*dst_bot;
			switch(f)
				{
				case 0:
					src_tx=small_font_texpos[g];
					dst_top=&small_font_texpos_top[g];
					dst_bot=&small_font_texpos_bot[g];
					break;
				case 1:
					src_tx=large_font_texpos[g];
					dst_top=&large_font_texpos_top[g];
					dst_bot=&large_font_texpos_bot[g];
					break;
				case 2:
					src_tx=basic_font_texpos[g];
					dst_top=&basic_font_texpos_top[g];
					dst_bot=&basic_font_texpos_bot[g];
					break;
				}
			*dst_top=0;
			*dst_bot=0;
			if(src_tx==0)continue;
			SDL_Surface *src=enabler.textures.get_texture_data(src_tx);
			if(src==NULL)continue;

			SDL_LockSurface(src);

			*dst_top=enabler.textures.create_texture(src->w,src->h);
			SDL_Surface *dst_top_px=enabler.textures.get_texture_data(*dst_top);
			if(dst_top_px==NULL)continue;
			*dst_bot=enabler.textures.create_texture(src->w,src->h);
			SDL_Surface *dst_bot_px=enabler.textures.get_texture_data(*dst_bot);
			if(dst_bot_px==NULL)continue;

			SDL_LockSurface(dst_top_px);
			SDL_LockSurface(dst_bot_px);

			memset(dst_top_px->pixels,0,sizeof(Uint8)*(dst_top_px->h)*dst_top_px->pitch);
			memset(dst_bot_px->pixels,0,sizeof(Uint8)*(dst_bot_px->h)*dst_bot_px->pitch);

			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				if(y<src->h/2)
					{
					pixel_dst=((Uint8*)dst_top_px->pixels)+((y+(src->h+1)/2)*dst_top_px->pitch);
					for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
						{
						if(pixel_src[3]!=0)
							{
							pixel_dst[0]=pixel_src[0];
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=pixel_src[2];
							pixel_dst[3]=pixel_src[3];
							}
						}
					}
				else
					{
					pixel_dst=((Uint8*)dst_bot_px->pixels)+((y-src->h/2)*dst_bot_px->pitch);
					for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
						{
						if(pixel_src[3]!=0)
							{
							pixel_dst[0]=pixel_src[0];
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=pixel_src[2];
							pixel_dst[3]=pixel_src[3];
							}
						}
					}
				}

			SDL_UnlockSurface(dst_top_px);
			SDL_UnlockSurface(dst_bot_px);

			SDL_UnlockSurface(src);
			}
		}
	/*
			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
				for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
					{
					if(pixel_src[3]!=0)
						{
						//************************************ DEBRIS
						pixel_dst[0]=(Uint8)((float)pixel_src[0]*gps.ccolor[d][0]);
						pixel_dst[1]=(Uint8)((float)pixel_src[1]*gps.ccolor[d][1]);
						pixel_dst[2]=(Uint8)((float)pixel_src[2]*gps.ccolor[d][2]);
						}
					}
				}
			*/
}

static void swap_long(long &a,long &b)
{
	int32_t sw=a;
	a=b;
	b=sw;
}

static void swap_int32_t(int32_t &a,int32_t &b)
{
	int32_t sw=a;
	a=b;
	b=sw;
}

void initst::swap_basic_sets()
{
	int32_t l;
	for(l=0;l<INIT_LOAD_BAR_TEXTURENUM;++l)swap_long(load_bar_texpos[l],classic_load_bar_texpos[l]);
	for(l=0;l<9;++l)
		{
		swap_int32_t(texpos_neutral_intro_button[l],classic_texpos_neutral_intro_button[l]);
		swap_int32_t(texpos_confirm_intro_button[l],classic_texpos_confirm_intro_button[l]);
		swap_int32_t(texpos_cancel_intro_button[l],classic_texpos_cancel_intro_button[l]);
		swap_int32_t(texpos_selected_intro_button[l],classic_texpos_selected_intro_button[l]);
		swap_int32_t(texpos_unselected_intro_button[l],classic_texpos_unselected_intro_button[l]);
		swap_int32_t(texpos_open_list_button[l],classic_texpos_open_list_button[l]);
		swap_int32_t(texpos_increase_button[l],classic_texpos_increase_button[l]);
		swap_int32_t(texpos_decrease_button[l],classic_texpos_decrease_button[l]);
		swap_int32_t(texpos_nullify_button[l],classic_texpos_nullify_button[l]);
		swap_int32_t(texpos_left_arrow_button[l],classic_texpos_left_arrow_button[l]);
		swap_int32_t(texpos_right_arrow_button[l],classic_texpos_right_arrow_button[l]);
		swap_int32_t(texpos_up_arrow_button[l],classic_texpos_up_arrow_button[l]);
		swap_int32_t(texpos_down_arrow_button[l],classic_texpos_down_arrow_button[l]);
		}
	swap_int32_t(texpos_border_nw,classic_texpos_border_nw);
	swap_int32_t(texpos_border_n,classic_texpos_border_n);
	swap_int32_t(texpos_border_ne,classic_texpos_border_ne);
	swap_int32_t(texpos_border_w,classic_texpos_border_w);
	swap_int32_t(texpos_border_interior,classic_texpos_border_interior);
	swap_int32_t(texpos_border_e,classic_texpos_border_e);
	swap_int32_t(texpos_border_sw,classic_texpos_border_sw);
	swap_int32_t(texpos_border_s,classic_texpos_border_s);
	swap_int32_t(texpos_border_se,classic_texpos_border_se);
	swap_int32_t(texpos_border_join_n,classic_texpos_border_join_n);
	swap_int32_t(texpos_border_join_s,classic_texpos_border_join_s);
	swap_int32_t(texpos_border_join_w,classic_texpos_border_join_w);
	swap_int32_t(texpos_border_join_e,classic_texpos_border_join_e);
	swap_int32_t(texpos_border_inside_nswe,classic_texpos_border_inside_nswe);
	swap_int32_t(texpos_border_inside_nsw,classic_texpos_border_inside_nsw);
	swap_int32_t(texpos_border_inside_nse,classic_texpos_border_inside_nse);
	swap_int32_t(texpos_border_inside_nwe,classic_texpos_border_inside_nwe);
	swap_int32_t(texpos_border_inside_swe,classic_texpos_border_inside_swe);
	swap_int32_t(texpos_border_inside_ns,classic_texpos_border_inside_ns);
	swap_int32_t(texpos_border_inside_we,classic_texpos_border_inside_we);
	int32_t k;
	for(l=0;l<2;++l)
		{
		for(k=0;k<3;++k)swap_int32_t(texpos_scrollbar[l][k],classic_texpos_scrollbar[l][k]);
		}
	for(l=0;l<2;++l)
		{
		swap_int32_t(texpos_scrollbar_up_hover[l],classic_texpos_scrollbar_up_hover[l]);
		swap_int32_t(texpos_scrollbar_up_pressed[l],classic_texpos_scrollbar_up_pressed[l]);
		swap_int32_t(texpos_scrollbar_down_hover[l],classic_texpos_scrollbar_down_hover[l]);
		swap_int32_t(texpos_scrollbar_down_pressed[l],classic_texpos_scrollbar_down_pressed[l]);
		swap_int32_t(texpos_scrollbar_top_scroller[l],classic_texpos_scrollbar_top_scroller[l]);
		swap_int32_t(texpos_scrollbar_top_scroller_hover[l],classic_texpos_scrollbar_top_scroller_hover[l]);
		swap_int32_t(texpos_scrollbar_bottom_scroller[l],classic_texpos_scrollbar_bottom_scroller[l]);
		swap_int32_t(texpos_scrollbar_bottom_scroller_hover[l],classic_texpos_scrollbar_bottom_scroller_hover[l]);
		swap_int32_t(texpos_scrollbar_blank_scroller[l],classic_texpos_scrollbar_blank_scroller[l]);
		swap_int32_t(texpos_scrollbar_blank_scroller_hover[l],classic_texpos_scrollbar_blank_scroller_hover[l]);
		swap_int32_t(texpos_scrollbar_center_scroller[l],classic_texpos_scrollbar_center_scroller[l]);
		swap_int32_t(texpos_scrollbar_center_scroller_hover[l],classic_texpos_scrollbar_center_scroller_hover[l]);
		}
	for(l=0;l<2;++l)
		{
		for(k=0;k<2;++k)
			{
			swap_int32_t(texpos_scrollbar_small_scroller[l][k],classic_texpos_scrollbar_small_scroller[l][k]);
			swap_int32_t(texpos_scrollbar_small_scroller_hover[l][k],classic_texpos_scrollbar_small_scroller_hover[l][k]);
			swap_int32_t(texpos_scrollbar_offcenter_scroller[l][k],classic_texpos_scrollbar_offcenter_scroller[l][k]);
			swap_int32_t(texpos_scrollbar_offcenter_scroller_hover[l][k],classic_texpos_scrollbar_offcenter_scroller_hover[l][k]);
			}
		}
	for(l=0;l<6;++l)
		{
		for(k=0;k<3;++k)swap_int32_t(texpos_button_filter[l][k],classic_texpos_button_filter[l][k]);
		}
	for(l=0;l<4;++l)
		{
		for(k=0;k<3;++k)swap_int32_t(texpos_button_filter_name[l][k],classic_texpos_button_filter_name[l][k]);
		}
	for(l=0;l<5;++l)
		{
		for(k=0;k<2;++k)
			{
			swap_int32_t(texpos_tab_unselected[l][k],classic_texpos_tab_unselected[l][k]);
			swap_int32_t(texpos_tab_selected[l][k],classic_texpos_tab_selected[l][k]);
			}
		}
	for(l=0;l<4;++l)
		{
		swap_int32_t(texpos_sort_ascending_active[l],classic_texpos_sort_ascending_active[l]);
		swap_int32_t(texpos_sort_ascending_inactive[l],classic_texpos_sort_ascending_inactive[l]);
		swap_int32_t(texpos_sort_descending_active[l],classic_texpos_sort_descending_active[l]);
		swap_int32_t(texpos_sort_descending_inactive[l],classic_texpos_sort_descending_inactive[l]);
		}
	for(l=0;l<3;++l)
		{
		swap_int32_t(texpos_sort_text_active[l],classic_texpos_sort_text_active[l]);
		swap_int32_t(texpos_sort_text_inactive[l],classic_texpos_sort_text_inactive[l]);
		}
}