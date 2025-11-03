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
#include "../zlib/contrib/minizip/unzip.h"
#include <cassert>

#include "svector.h"
#include "dfhooks.h"
//#include "ttf_manager.hpp"

/*
#ifndef INTEGER_TYPES

#define INTEGER_TYPES

#ifdef WIN32
	typedef signed char int8_t;
	typedef short int16_t;
	typedef int int32_t;
	typedef long long int64_t;
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int uint32_t;
	typedef unsigned long long uint64_t;
#endif

typedef int32_t VIndex;
typedef int32_t Ordinal;

#endif
*/

#include "random.h"

#include "endian.h"

#include "files.h"

#include "enabler.h"

#include "textlines.h"

#include "find_files.h"

#include "basics.h"

#include "g_basics.h"

#include "texture_handler.h"

#include "graphics.h"

#include "music_and_sound_g.h"

#include "init.h"

#include "interface.h"

#ifdef __APPLE__
#define _XOPEN_SOURCE_EXTENDED
#endif
#ifdef CURSES
//#include "curses.h"
#endif
using namespace std;


#pragma comment( lib, "opengl32.lib" )			// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )				// Search For GLu32.lib While Linking

extern enablerst enabler;
extern texture_handlerst texture;
graphicst gps;
extern interfacest gview;

extern thread_local string errorlog_prefix;

#ifndef FULL_RELEASE_VERSION
extern bool cinematic_mode;
extern int32_t cinematic_shift_x;
extern int32_t cinematic_shift_y;
extern int32_t cinematic_shift_dx;
extern int32_t cinematic_shift_dy;
extern int32_t cinematic_shift_velx;
extern int32_t cinematic_shift_vely;
extern int32_t cinematic_start_scrollx;
extern int32_t cinematic_start_scrolly;
#endif

const unordered_bimap<string,Sphere> SPHERESTRINGS={
	{"AGRICULTURE",SPHERE_AGRICULTURE},
	{"ANIMALS",SPHERE_ANIMALS},
	{"ART",SPHERE_ART},
	{"BALANCE",SPHERE_BALANCE},
	{"BEAUTY",SPHERE_BEAUTY},
	{"BIRTH",SPHERE_BIRTH},
	{"BLIGHT",SPHERE_BLIGHT},
	{"BOUNDARIES",SPHERE_BOUNDARIES},
	{"CAVERNS",SPHERE_CAVERNS},
	{"CHAOS",SPHERE_CHAOS},
	{"CHARITY",SPHERE_CHARITY},
	{"CHILDREN",SPHERE_CHILDREN},
	{"COASTS",SPHERE_COASTS},
	{"CONSOLATION",SPHERE_CONSOLATION},
	{"COURAGE",SPHERE_COURAGE},
	{"CRAFTS",SPHERE_CRAFTS},
	{"CREATION",SPHERE_CREATION},
	{"DANCE",SPHERE_DANCE},
	{"DARKNESS",SPHERE_DARKNESS},
	{"DAWN",SPHERE_DAWN},
	{"DAY",SPHERE_DAY},
	{"DEATH",SPHERE_DEATH},
	{"DEFORMITY",SPHERE_DEFORMITY},
	{"DEPRAVITY",SPHERE_DEPRAVITY},
	{"DISCIPLINE",SPHERE_DISCIPLINE},
	{"DISEASE",SPHERE_DISEASE},
	{"DREAMS",SPHERE_DREAMS},
	{"DUSK",SPHERE_DUSK},
	{"DUTY",SPHERE_DUTY},
	{"EARTH",SPHERE_EARTH},
	{"FAMILY",SPHERE_FAMILY},
	{"FAME",SPHERE_FAME},
	{"FATE",SPHERE_FATE},
	{"FERTILITY",SPHERE_FERTILITY},
	{"FESTIVALS",SPHERE_FESTIVALS},
	{"FIRE",SPHERE_FIRE},
	{"FISH",SPHERE_FISH},
	{"FISHING",SPHERE_FISHING},
	{"FOOD",SPHERE_FOOD},
	{"FORGIVENESS",SPHERE_FORGIVENESS},
	{"FORTRESSES",SPHERE_FORTRESSES},
	{"FREEDOM",SPHERE_FREEDOM},
	{"GAMBLING",SPHERE_GAMBLING},
	{"GAMES",SPHERE_GAMES},
	{"GENEROSITY",SPHERE_GENEROSITY},
	{"HAPPINESS",SPHERE_HAPPINESS},
	{"HEALING",SPHERE_HEALING},
	{"HOSPITALITY",SPHERE_HOSPITALITY},
	{"HUNTING",SPHERE_HUNTING},
	{"INSPIRATION",SPHERE_INSPIRATION},
	{"JEALOUSY",SPHERE_JEALOUSY},
	{"JEWELS",SPHERE_JEWELS},
	{"JUSTICE",SPHERE_JUSTICE},
	{"LABOR",SPHERE_LABOR},
	{"LAKES",SPHERE_LAKES},
	{"LAWS",SPHERE_LAWS},
	{"LIES",SPHERE_LIES},
	{"LIGHT",SPHERE_LIGHT},
	{"LIGHTNING",SPHERE_LIGHTNING},
	{"LONGEVITY",SPHERE_LONGEVITY},
	{"LOVE",SPHERE_LOVE},
	{"LOYALTY",SPHERE_LOYALTY},
	{"LUCK",SPHERE_LUCK},
	{"LUST",SPHERE_LUST},
	{"MARRIAGE",SPHERE_MARRIAGE},
	{"MERCY",SPHERE_MERCY},
	{"METALS",SPHERE_METALS},
	{"MINERALS",SPHERE_MINERALS},
	{"MISERY",SPHERE_MISERY},
	{"MIST",SPHERE_MIST},
	{"MOON",SPHERE_MOON},
	{"MOUNTAINS",SPHERE_MOUNTAINS},
	{"MUCK",SPHERE_MUCK},
	{"MURDER",SPHERE_MURDER},
	{"MUSIC",SPHERE_MUSIC},
	{"NATURE",SPHERE_NATURE},
	{"NIGHT",SPHERE_NIGHT},
	{"NIGHTMARES",SPHERE_NIGHTMARES},
	{"OATHS",SPHERE_OATHS},
	{"OCEANS",SPHERE_OCEANS},
	{"ORDER",SPHERE_ORDER},
	{"PAINTING",SPHERE_PAINTING},
	{"PEACE",SPHERE_PEACE},
	{"PERSUASION",SPHERE_PERSUASION},
	{"PLANTS",SPHERE_PLANTS},
	{"POETRY",SPHERE_POETRY},
	{"PREGNANCY",SPHERE_PREGNANCY},
	{"RAIN",SPHERE_RAIN},
	{"RAINBOWS",SPHERE_RAINBOWS},
	{"REBIRTH",SPHERE_REBIRTH},
	{"REVELRY",SPHERE_REVELRY},
	{"REVENGE",SPHERE_REVENGE},
	{"RIVERS",SPHERE_RIVERS},
	{"RULERSHIP",SPHERE_RULERSHIP},
	{"RUMORS",SPHERE_RUMORS},
	{"SACRIFICE",SPHERE_SACRIFICE},
	{"SALT",SPHERE_SALT},
	{"SCHOLARSHIP",SPHERE_SCHOLARSHIP},
	{"SEASONS",SPHERE_SEASONS},
	{"SILENCE",SPHERE_SILENCE},
	{"SKY",SPHERE_SKY},
	{"SONG",SPHERE_SONG},
	{"SPEECH",SPHERE_SPEECH},
	{"STARS",SPHERE_STARS},
	{"STORMS",SPHERE_STORMS},
	{"STRENGTH",SPHERE_STRENGTH},
	{"SUICIDE",SPHERE_SUICIDE},
	{"SUN",SPHERE_SUN},
	{"THEFT",SPHERE_THEFT},
	{"THRALLDOM",SPHERE_THRALLDOM},
	{"THUNDER",SPHERE_THUNDER},
	{"TORTURE",SPHERE_TORTURE},
	{"TRADE",SPHERE_TRADE},
	{"TRAVELERS",SPHERE_TRAVELERS},
	{"TREACHERY",SPHERE_TREACHERY},
	{"TREES",SPHERE_TREES},
	{"TRICKERY",SPHERE_TRICKERY},
	{"TRUTH",SPHERE_TRUTH},
	{"TWILIGHT",SPHERE_TWILIGHT},
	{"VALOR",SPHERE_VALOR},
	{"VICTORY",SPHERE_VICTORY},
	{"VOLCANOS",SPHERE_VOLCANOS},
	{"VOLCANOES",SPHERE_VOLCANOS}, // This is allowed, SPHERESTRINGS.maybe_get(SPHERE_VOLCANOS).value() ought to return "VOLCANOS" this way
	{"WAR",SPHERE_WAR},
	{"WATER",SPHERE_WATER},
	{"WEALTH",SPHERE_WEALTH},
	{"WEATHER",SPHERE_WEATHER},
	{"WIND",SPHERE_WIND},
	{"WISDOM",SPHERE_WISDOM},
	{"WRITING",SPHERE_WRITING},
	{"YOUTH",SPHERE_YOUTH},
	};

void process_object_lines(textlinesst &lines,const string &chktype,const std::filesystem::path &graphics_dir);

// Add, then increment to the (possible) PBO alignment requirement
static void align(size_t &sz, off_t inc) {
  sz += inc;
  sz += (64-(sz % 64));
}

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

void graphicst::resize(int x, int y)
{
	dimx = x; dimy = y;
	init.display.grid_x = x;
	init.display.grid_y = y;
	setclipping(0, x-1, 0, y-1);
	force_full_display_count++;
	screen_limit = screen + dimx * dimy * 8;
	screen_top_limit = screen_top + dimx * dimy * 8;
}

void graphicst::reshape_viewports(int32_t zoom_factor)
{
	if(main_viewport!=NULL)
		{
		main_viewport->shape_viewport_according_by_pixel_size(screen_pixel_x,screen_pixel_y,zoom_factor);
		}
	int32_t lvp=LOWER_VIEWPORT_MAX-1;
	while(lvp>=0)
		{
		if(lower_viewport[lvp]!=NULL)lower_viewport[lvp]->shape_viewport_according_by_pixel_size(screen_pixel_x,screen_pixel_y,zoom_factor);
		--lvp;
		}

	if(main_map_port!=NULL)
		{
		int32_t px=screen_pixel_x*main_map_port->pixel_perc_x/100;
		int32_t py=screen_pixel_y*main_map_port->pixel_perc_y/100;

												//*********************** TEXTURE SIZE DEPENDENCE
													//also: need to be more careful about this cutoff -- perc introduces some error
														//so at certain zooms it is way off (the -16 is a cludge around this)
		main_map_port->top_left_corner_x=gps.screen_pixel_x-(px+15)/16*16-16;
		//main_map_port->top_left_corner_y=32;//*********************** TEXTURE SIZE DEPENDENCE
		main_map_port->top_left_corner_y=12*8+8;//this doesn't work when the top portraits etc. are interface-scaled
		if(main_map_port->pixel_perc_x==100)main_map_port->top_left_corner_x=0;
		if(main_map_port->pixel_perc_y==100)main_map_port->top_left_corner_y=0;

		main_map_port->shape_map_port_according_by_pixel_size(px,py);
		}
}

void graphic_viewportst::shape_viewport_according_by_pixel_size(int32_t px,int32_t py,int32_t zoom_factor)
{
	if(zoom_factor!=128)
		{
		int32_t dim=zoom_factor*32/128;

		int32_t ndx=px/dim;
		int32_t ndy=py/dim;
		if(px%dim!=0)++ndx;
		if(py%dim!=0)++ndy;
		set_dims_and_allocate(ndx,ndy);
		}
	else
		{
		//*************************** TEXTURE SIZE DEPENDENCE
		int32_t ndx=px/32;
		int32_t ndy=py/32;
		if(px%32!=0)++ndx;
		if(py%32!=0)++ndy;
		set_dims_and_allocate(ndx,ndy);
		}
}

void graphic_map_portst::shape_map_port_according_by_pixel_size(int32_t px,int32_t py)
{
	//*************************** TEXTURE SIZE DEPENDENCE
	int32_t ndx=px/16;
	int32_t ndy=py/16;
	if(px%16!=0)++ndx;
	if(py%16!=0)++ndy;

	set_dims_and_allocate(ndx,ndy);
}

void graphicst::addcoloredst(const char *str,const char *colorstr)
{
  const int slen = (int)strlen(str);
  int s;
  for(s=0; s < slen && screenx < init.display.grid_x; s++)
    {
      if(screenx<0)
        {
          s-=screenx;
          screenx=0;
          if (s >= slen) break;
        }
      
      changecolor((colorstr[s] & 7),((colorstr[s] & 56))>>3,((colorstr[s] & 64))>>6);
      addchar(str[s]);
    }
}

static list<ttf_id> ttfstr;

static void abbreviate_string_helper_hackaroundmissingcode(string &str, int len) {
       if(str.length()>=2)
		{
		if((str[0]=='A'||str[0]=='a')&&
			str[1]==' ')
			{
			str.erase(str.begin()+1);
			str.erase(str.begin());

			if(str.length()<=len)return;
			}

		if(str.length()>=3)
			{
			if((str[0]=='A'||str[0]=='a')&&
				(str[1]=='N'||str[1]=='n')&&
				str[2]==' ')
				{
				str.erase(str.begin()+2);
				str.erase(str.begin()+1);
				str.erase(str.begin());

				if(str.length()<=len)return;
				}

			if(str.length()>=4)
				{
				if((str[0]=='T'||str[0]=='t')&&
					(str[1]=='H'||str[1]=='h')&&
					(str[2]=='E'||str[2]=='e')&&
					str[3]==' ')
					{
					str.erase(str.begin()+3);
					str.erase(str.begin()+2);
					str.erase(str.begin()+1);
					str.erase(str.begin());

					if(str.length()<=len)return;
					}
				}
			}
		}

	int32_t l;
	for(l=(int32_t)str.length()-1;l>=1;l--)
		{
		if(str[l-1]==' ')continue;

		if(str[l]=='a'||
			str[l]=='e'||
			str[l]=='i'||
			str[l]=='o'||
			str[l]=='u'||
			str[l]=='A'||
			str[l]=='E'||
			str[l]=='I'||
			str[l]=='O'||
			str[l]=='U')
			{
			str.erase(str.begin()+l);
			if(str.length()<=len)return;
			}
		}

	if(str.length()>len)str.resize(len);
}


static void abbreviate_string_hackaroundmissingcode(string &str, int32_t len)
{
  /* (ttf_manager.ttf_active()) {
    // We'll need to use TTF-aware text shrinking.
    while (ttf_manager.size_text(str) > len)
      abbreviate_string_helper_hackaroundmissingcode(str, str.length() - 1);
  } else */if(str.length()>len){
    // 1 letter = 1 tile.
    abbreviate_string_helper_hackaroundmissingcode(str, len);
  }
}

void graphicst::addst_flag(const string &str_orig, justification just, int space, uint32_t sflag)
{
  if (!str_orig.size())
    return;
  string str = str_orig;
  if (space)
    abbreviate_string_hackaroundmissingcode(str, space);

	int s;
	for(s=0;s<str.length()&&screenx<init.display.grid_x;s++)
		{
		if(screenx<0)
			{
			s-=screenx;
			screenx=0;
			if(s>=str.length())break;
			}
        
		addchar_flag(str[s],1,sflag);
		}
}

void graphicst::addst(const string &str_orig, justification just, int space)
{
  if (!str_orig.size())
    return;
  string str = str_orig;
  if (space)
    abbreviate_string_hackaroundmissingcode(str, space);
  //if (just == not_truetype | !ttf_manager.ttf_active()) {
    int s;
    for(s=0;s<str.length()&&screenx<init.display.grid_x;s++)
      {
        if(screenx<0)
          {
            s-=screenx;
            screenx=0;
            if(s>=str.length())break;
          }
        
        addchar(str[s]);
      }
	/*
  } else {
    // Truetype
    if (str.size() > 2 && str[0] == ':' && str[1] == ' ')
      str[1] = '\t'; // EVIL HACK
    struct ttf_id id = {str, (unsigned char)screenf, (unsigned char)screenb, (unsigned char)screenbright};
    ttfstr.push_back(id);
    // if (str.size() == 80) {
    //   cout << "(" << int(str[0]) << ") ";
    // }
    // cout << screeny << "," << str.size() << ":" << str;
    // if (just == justify_cont)
    //   cout << "|";
    // else
    //   cout << endl;
    if (just == justify_cont)
      return; // More later
    // This string is done. Time to render.
    ttf_details details = ttf_manager.get_handle(ttfstr, just);
    const int handle = details.handle;
    const int offset = details.offset;
    int width = details.width;
    const int ourx = screenx + offset;
    Uint64 * const s = ((Uint64*)screen + ourx*dimy + screeny);
    if (s < (Uint64*)screen_limit)
      s[0] = (((Uint64)GRAPHICSTYPE_TTF) << 56) | handle;
    // Also set the other tiles this text covers, but don't write past the end.
    if (width + ourx >= dimx)
      width = dimx - ourx - 1;
    for (int x = 1; x < width; ++x)
      s[x * dimy] = (((Uint64)GRAPHICSTYPE_TTFCONT) << 56) | handle;
    // Clean up, prepare for next string.
    screenx = ourx + width;
    ttfstr.clear();
  }
  */
}

void graphicst::top_addst_flag(const string &str_orig, justification just, int space, uint32_t sflag)
{
  if (!str_orig.size())
    return;
  string str = str_orig;
  if (space)
    abbreviate_string_hackaroundmissingcode(str, space);

	int s;
	for(s=0;s<str.length()&&screenx<init.display.grid_x;s++)
		{
		if(screenx<0)
			{
			s-=screenx;
			screenx=0;
			if(s>=str.length())break;
			}
        
		top_addchar_flag(str[s],1,sflag);
		}
}

void graphicst::top_addst(const string &str_orig, justification just, int space)
{
  if (!str_orig.size())
    return;
  string str = str_orig;
  if (space)
    abbreviate_string_hackaroundmissingcode(str, space);
  //if (just == not_truetype || !ttf_manager.ttf_active()) {
    int s;
    for(s=0;s<str.length()&&screenx<init.display.grid_x;s++)
      {
        if(screenx<0)
          {
            s-=screenx;
            screenx=0;
            if(s>=str.length())break;
          }
        
        top_addchar(str[s]);
      }
	/*
  } else {
    // Truetype
    if (str.size() > 2 && str[0] == ':' && str[1] == ' ')
      str[1] = '\t'; // EVIL HACK
    struct ttf_id id = {str, (unsigned char)screenf, (unsigned char)screenb, (unsigned char)screenbright};
    ttfstr.push_back(id);
    // if (str.size() == 80) {
    //   cout << "(" << int(str[0]) << ") ";
    // }
    // cout << screeny << "," << str.size() << ":" << str;
    // if (just == justify_cont)
    //   cout << "|";
    // else
    //   cout << endl;
    if (just == justify_cont)
      return; // More later
    // This string is done. Time to render.
    ttf_details details = ttf_manager.get_handle(ttfstr, just);
    const int handle = details.handle;
    const int offset = details.offset;
    int width = details.width;
    const int ourx = screenx + offset;
    Uint64 * const s = ((Uint64*)screen_top + ourx*dimy + screeny);
    if (s < (Uint64*)screen_top_limit)
      s[0] = (((Uint64)GRAPHICSTYPE_TTF) << 56) | handle;
    // Also set the other tiles this text covers, but don't write past the end.
    if (width + ourx >= dimx)
      width = dimx - ourx - 1;
    for (int x = 1; x < width; ++x)
      s[x * dimy] = (((Uint64)GRAPHICSTYPE_TTFCONT) << 56) | handle;
    // Clean up, prepare for next string.
    screenx = ourx + width;
    ttfstr.clear();
  }
  */
}

void graphicst::erasescreen_clip()
{
	changecolor(0,0,0);
	short x2,y2;
	for(x2=clipx[0];x2<=clipx[1];x2++)
		{
		for(y2=clipy[0];y2<=clipy[1];y2++)
			{
			locate(y2,x2);
			addchar(' ');
			}
		}
}

void graphicst::erasescreen_rect(int x1, int x2, int y1, int y2)
{ 
  changecolor(0,0,0);
  for (int x = x1; x <= x2; x++) {
    for (int y = y1; y <= y2; y++) {
      locate(y, x);
      addchar(' ');
    }
  }
}

void graphicst::erasescreen()
{
	int32_t sz=dimx*dimy*8;
	memset(screen, 0, sz);
	unsigned char *c=screen;
	int32_t i;
	for(i=0;i<sz;i+=8,c+=8)c[0]=' ';

	memset(screentexpos, 0, dimx*dimy*sizeof(long));
	memset(screentexpos_lower, 0, dimx*dimy*sizeof(long));
	memset(screentexpos_anchored, 0, dimx*dimy*sizeof(long));
	memset(screentexpos_flag, 0, dimx*dimy*sizeof(uint32_t));

	if(gps.top_in_use)
		{
		memset(screen_top, 0, sz);
		unsigned char *c=screen_top;
		int32_t i;
		for(i=0;i<sz;i+=8,c+=8)c[0]=' ';

		memset(screentexpos_top, 0, dimx*dimy*sizeof(long));
		memset(screentexpos_top_lower, 0, dimx*dimy*sizeof(long));
		memset(screentexpos_top_anchored, 0, dimx*dimy*sizeof(long));
		memset(screentexpos_top_flag, 0, dimx*dimy*sizeof(uint32_t));
		}
}

void graphicst::setclipping(long x1,long x2,long y1,long y2)
{
	if(x1<0)x1=0;
	if(x2>init.display.grid_x-1)x2=init.display.grid_x-1;
	if(y1<0)y1=0;
	if(y2>init.display.grid_y-1)y2=init.display.grid_y-1;

	clipx[0]=x1;
	clipx[1]=x2;
	clipy[0]=y1;
	clipy[1]=y2;
}

void graphicst::dim_colors(long x,long y,char dim)
{
	if(x>=clipx[0]&&x<=clipx[1]&&
		y>=clipy[0]&&y<=clipy[1])
		{
		//******************************** DIM COLORS
			//beware -- *4 should be *8 in most places now
		/*
		switch(dim)
			{
			case 4:
				switch(screen[x*dimy*4 + y*4 + 2])
					{
					case 4:
					case 5:
					case 6:
						screen[x*dimy*4 + y*4 + 2]=1;
						break;
					case 2:
					case 7:
						screen[x*dimy*4 + y*4 + 2]=3;
						break;
					}
				switch(screen[x*dimy*4 + y*4 + 1])
					{
					case 4:
					case 5:
					case 6:
						screen[x*dimy*4 + y*4 + 1]=1;
						break;
					case 2:
					case 7:
						screen[x*dimy*4 + y*4 + 1]=3;
						break;
					}
				if(screen[x*dimy*4 + y*4 + 1]==screen[x*dimy*4 + y*4 + 2])screen[x*dimy*4 + y*4 + 1]=0;
				screen[x*dimy*4 + y*4 + 3]=0;
				if(screen[x*dimy*4 + y*4 + 1]==0&&screen[x*dimy*4 + y*4 + 2]==0&&screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 3]=1;
				break;
			case 3:
				switch(screen[x*dimy*4 + y*4 + 2])
					{
					case 4:
					case 5:
						screen[x*dimy*4 + y*4 + 2]=6;
						break;
					case 2:
					case 7:
						screen[x*dimy*4 + y*4 + 2]=3;
						break;
					}
				switch(screen[x*dimy*4 + y*4 + 1])
					{
					case 1:
						screen[x*dimy*4 + y*4 + 3]=0;
						break;
					case 4:
					case 5:
						screen[x*dimy*4 + y*4 + 1]=6;
						break;
					case 2:
						screen[x*dimy*4 + y*4 + 1]=3;
						break;
					case 7:
						screen[x*dimy*4 + y*4 + 1]=3;
						break;
					}
				if(screen[x*dimy*4 + y*4 + 1]!=7)screen[x*dimy*4 + y*4 + 3]=0;
				if(screen[x*dimy*4 + y*4 + 1]==screen[x*dimy*4 + y*4 + 2]&&
					screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 1]=0;
				if(screen[x*dimy*4 + y*4 + 1]==0&&screen[x*dimy*4 + y*4 + 2]==0&&screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 3]=1;
				break;
			case 2:
				switch(screen[x*dimy*4 + y*4 + 2])
					{
					case 4:
					case 5:
						screen[x*dimy*4 + y*4 + 2]=6;
						break;
					}
				switch(screen[x*dimy*4 + y*4 + 1])
					{
					case 4:
					case 5:
						screen[x*dimy*4 + y*4 + 1]=6;
						break;
					}
				if(screen[x*dimy*4 + y*4 + 1]!=7)screen[x*dimy*4 + y*4 + 3]=0;
				if(screen[x*dimy*4 + y*4 + 1]==screen[x*dimy*4 + y*4 + 2]&&
					screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 1]=0;
				if(screen[x*dimy*4 + y*4 + 1]==0&&screen[x*dimy*4 + y*4 + 2]==0&&screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 3]=1;
				break;
			case 1:
				if(screen[x*dimy*4 + y*4 + 1]!=7)screen[x*dimy*4 + y*4 + 3]=0;
				if(screen[x*dimy*4 + y*4 + 1]==screen[x*dimy*4 + y*4 + 2]&&
					screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 1]=0;
				if(screen[x*dimy*4 + y*4 + 1]==0&&screen[x*dimy*4 + y*4 + 2]==0&&screen[x*dimy*4 + y*4 + 3]==0)screen[x*dimy*4 + y*4 + 3]=1;
				break;
			}
		*/
		}
}

void graphicst::rain_color_square(long x,long y)
{
	if(x>=clipx[0]&&x<=clipx[1]&&
		y>=clipy[0]&&y<=clipy[1])
		{
		//******************************** DIM COLORS
			//beware -- *4 should be *8 in most places now
		/*
		screen[x*dimy*4 + y*4 + 1]=1;
		screen[x*dimy*4 + y*4 + 2]=0;
		screen[x*dimy*4 + y*4 + 3]=1;
		*/
		}
}

void graphicst::snow_color_square(long x,long y)
{
	if(x>=clipx[0]&&x<=clipx[1]&&
		y>=clipy[0]&&y<=clipy[1])
		{
		//******************************** DIM COLORS
			//beware -- *4 should be *8 in most places now
		/*
		screen[x*dimy*4 + y*4 + 1]=7;
		screen[x*dimy*4 + y*4 + 2]=0;
		screen[x*dimy*4 + y*4 + 3]=1;
		*/
		}
}

void graphicst::color_square(long x,long y,unsigned char f,unsigned char b,unsigned char br)
{
	if(x>=clipx[0]&&x<=clipx[1]&&
		y>=clipy[0]&&y<=clipy[1])
		{
		//******************************** DIM COLORS
			//beware -- *4 should be *8 in most places now
		/*
		screen[x*dimy*4 + y*4 + 1]=f;
		screen[x*dimy*4 + y*4 + 2]=b;
		screen[x*dimy*4 + y*4 + 3]=br;
		*/
		}
}

void graphicst::prepare_graphics(const std::filesystem::path &src_dir)
{
	//GET READY TO LOAD
	textlinesst setuplines;

	//LOAD THE OBJECT FILES UP INTO MEMORY
		//MUST INSURE THAT THEY ARE LOADED IN THE PROPER ORDER, IN CASE THEY REFER TO EACH OTHER
	{

	auto dir=filest(src_dir).any_location_unchecked()/"graphics";
	if (!std::filesystem::exists(dir)) return;
	std::vector<std::filesystem::path> tile_page_files;
	std::vector<std::filesystem::path> graphics_files;
	std::vector<std::filesystem::path> palette_files;
	for (auto &dir_entry : std::filesystem::recursive_directory_iterator(dir))
		{
		auto path=dir_entry.path();
		if (!(dir_entry.is_regular_file() && path.extension()==".txt")) continue;
		auto filename=path.filename().string();
		if (filename.starts_with("tile_page_"))
			{
			tile_page_files.push_back(path);
			}
		if (filename.starts_with("graphics_"))
			{
			graphics_files.push_back(path);
			}
		if (filename.starts_with("palette_"))
			{
			palette_files.push_back(path);
			}
		}
	string chktype="TILE_PAGE";
	for (auto &tile_page_file : tile_page_files)
		{
		auto str=tile_page_file.string();
		setuplines.load_raw_to_lines(str.c_str());

		errorlog_prefix="*** Error(s) found in the file \"";
		errorlog_prefix+=str;
		errorlog_prefix+='\"';
		process_object_lines(setuplines,chktype,src_dir);
		errorlog_prefix.clear();
		}
	chktype="GRAPHICS";
	for (auto &graphics_file : graphics_files)
		{
		auto str=graphics_file.string();
		setuplines.load_raw_to_lines(str.c_str());

		errorlog_prefix="*** Error(s) found in the file \"";
		errorlog_prefix+=str;
		errorlog_prefix+='\"';
		process_object_lines(setuplines,chktype,src_dir);
		errorlog_prefix.clear();
		}
	chktype="PALETTE";
	for (auto &palette_file : palette_files)
		{
		auto str=palette_file.string();
		setuplines.load_raw_to_lines(str.c_str());

		errorlog_prefix="*** Error(s) found in the file \"";
		errorlog_prefix+=str;
		errorlog_prefix+='\"';
		process_object_lines(setuplines,chktype,src_dir);
		errorlog_prefix.clear();
		}
	}

	//reset_textures called later, as there are still universe-specific textures to derive
}

void graphicst::add_lower_tile(long texp)
{
	if(screenx>=clipx[0]&&screenx<=clipx[1]&&
		screeny>=clipy[0]&&screeny<=clipy[1])
		{
		if(!init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
			{
			/* assert (screen_limit == screen + dimy * dimx * 8); */
			unsigned char *s = screen + screenx*dimy*8 + screeny*8;
			if (s < screen_limit)
				{
				unsigned char c=(unsigned char)(texp & (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8));
				int32_t color_f=(texp & (BIT9|BIT10|BIT11))>>8;
				int32_t color_b=(texp & (BIT12|BIT13|BIT14))>>11;
				int32_t color_br=(texp & BIT15)>>14;

				*s++ = c;
				int32_t fg=color_f+(color_br?8:0);
				*s++ = uccolor[fg][0];
				*s++ = uccolor[fg][1];
				*s++ = uccolor[fg][2];
				*s++ = uccolor[color_b][0];
				*s++ = uccolor[color_b][1];
				*s++ = uccolor[color_b][2];
				++s;//extra byte

				screentexpos[screenx*dimy + screeny]=0;
				}
			return;
			}

		screentexpos_lower[screenx*dimy + screeny]=texp;
		}
}

void graphicst::add_top_lower_tile(long texp)
{
	if(screenx>=clipx[0]&&screenx<=clipx[1]&&
		screeny>=clipy[0]&&screeny<=clipy[1])
		{
		if(!init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
			{
			/* assert (screen_limit == screen + dimy * dimx * 8); */
			unsigned char *s = screen + screenx*dimy*8 + screeny*8;
			if (s < screen_limit)
				{
				unsigned char c=(unsigned char)(texp & (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8));
				int32_t color_f=(texp & (BIT9|BIT10|BIT11))>>8;
				int32_t color_b=(texp & (BIT12|BIT13|BIT14))>>11;
				int32_t color_br=(texp & BIT15)>>14;

				*s++ = c;
				int32_t fg=color_f+(color_br?8:0);
				*s++ = uccolor[fg][0];
				*s++ = uccolor[fg][1];
				*s++ = uccolor[fg][2];
				*s++ = uccolor[color_b][0];
				*s++ = uccolor[color_b][1];
				*s++ = uccolor[color_b][2];
				++s;//extra byte

				screentexpos[screenx*dimy + screeny]=0;
				}
			return;
			}

		screentexpos_top_lower[screenx*dimy + screeny]=texp;
		}
}

void graphicst::add_anchored_tile(long texp,long offset_x,long offset_y,long tdx,long tdy,bool use_color)
{
	int32_t sx,sy;
	uint32_t ox,oy;
	for(sx=screenx,ox=0;sx<screenx+tdx;++sx,++ox)
		{
		for(sy=screeny,oy=0;sy<screeny+tdy;++sy,++oy)
			{
			if(sx>=clipx[0]&&sx<=clipx[1]&&
				sy>=clipy[0]&&sy<=clipy[1])
				{
				screentexpos_anchored[sx*dimy + sy]=texp;
				screentexpos_anchored_x[sx*dimy + sy]=offset_x;
				screentexpos_anchored_y[sx*dimy + sy]=offset_y;
				if(sx!=screenx||sy!=screeny)
					{
					uint32_t *stp_f=&(screentexpos_flag[sx*dimy + sy]);
					*stp_f=SCREENTEXPOS_FLAG_ANCHOR_SUBORDINATE;
					//64x64 limit on offset process
					*stp_f|=((ox>63?0:ox)<<SCREENTEXPOS_FLAG_ANCHOR_X_COORD_SHIFT);
					*stp_f|=((oy>63?0:oy)<<SCREENTEXPOS_FLAG_ANCHOR_Y_COORD_SHIFT);
					}
				else screentexpos_flag[sx*dimy + sy]&=~SCREENTEXPOS_FLAG_ANCHOR_SUBORDINATE;
				if(use_color)
					{
					int32_t ind=(sx*dimy+sy)*8;
					if(use_old_16_colors)
						{
						int32_t fg=screenf+(screenbright?8:0);
						screen[ind+1] = uccolor[fg][0];
						screen[ind+2] = uccolor[fg][1];
						screen[ind+3] = uccolor[fg][2];
						screen[ind+4] = uccolor[screenb][0];
						screen[ind+5] = uccolor[screenb][1];
						screen[ind+6] = uccolor[screenb][2];
						}
					else
						{
						screen[ind+1] = screen_color_r;
						screen[ind+2] = screen_color_g;
						screen[ind+3] = screen_color_b;
						screen[ind+4] = screen_color_br;
						screen[ind+5] = screen_color_bg;
						screen[ind+6] = screen_color_bb;
						}
					screentexpos_flag[sx*dimy + sy]|=SCREENTEXPOS_FLAG_ANCHOR_USE_SCREEN_COLOR;
					}
				else screentexpos_flag[sx*dimy + sy]&=~SCREENTEXPOS_FLAG_ANCHOR_USE_SCREEN_COLOR;
				}
			}
		}
}

void graphicst::add_top_anchored_tile(long texp,long offset_x,long offset_y,long tdx,long tdy,bool use_color)
{
	int32_t sx,sy;
	uint32_t ox,oy;
	for(sx=screenx,ox=0;sx<screenx+tdx;++sx,++ox)
		{
		for(sy=screeny,oy=0;sy<screeny+tdy;++sy,++oy)
			{
			if(sx>=clipx[0]&&sx<=clipx[1]&&
				sy>=clipy[0]&&sy<=clipy[1])
				{
				screentexpos_top_anchored[sx*dimy + sy]=texp;
				screentexpos_top_anchored_x[sx*dimy + sy]=offset_x;
				screentexpos_top_anchored_y[sx*dimy + sy]=offset_y;
				if(sx!=screenx||sy!=screeny)
					{
					uint32_t *stp_f=&(screentexpos_top_flag[sx*dimy + sy]);
					*stp_f=SCREENTEXPOS_FLAG_ANCHOR_SUBORDINATE;
					//64x64 limit on offset process
					*stp_f|=((ox>63?0:ox)<<SCREENTEXPOS_FLAG_ANCHOR_X_COORD_SHIFT);
					*stp_f|=((oy>63?0:oy)<<SCREENTEXPOS_FLAG_ANCHOR_Y_COORD_SHIFT);
					}
				else screentexpos_top_flag[sx*dimy + sy]&=~SCREENTEXPOS_FLAG_ANCHOR_SUBORDINATE;
				if(use_color)
					{
					int32_t ind=(sx*dimy+sy)*8;
					if(use_old_16_colors)
						{
						int32_t fg=screenf+(screenbright?8:0);
						screen_top[ind+1] = uccolor[fg][0];
						screen_top[ind+2] = uccolor[fg][1];
						screen_top[ind+3] = uccolor[fg][2];
						screen_top[ind+4] = uccolor[screenb][0];
						screen_top[ind+5] = uccolor[screenb][1];
						screen_top[ind+6] = uccolor[screenb][2];
						}
					else
						{
						screen_top[ind+1] = screen_color_r;
						screen_top[ind+2] = screen_color_g;
						screen_top[ind+3] = screen_color_b;
						screen_top[ind+4] = screen_color_br;
						screen_top[ind+5] = screen_color_bg;
						screen_top[ind+6] = screen_color_bb;
						}
					screentexpos_top_flag[sx*dimy + sy]|=SCREENTEXPOS_FLAG_ANCHOR_USE_SCREEN_COLOR;
					}
				else screentexpos_top_flag[sx*dimy + sy]&=~SCREENTEXPOS_FLAG_ANCHOR_USE_SCREEN_COLOR;
				}
			}
		}
}

void graphicst::add_texture_blit(int32_t which) {
	texblits.emplace_back(texblitst{ screenx, screeny, (int8_t)which });
}

void graphicst::add_tile(long texp,char addcolor)
{
	if(screenx>=clipx[0]&&screenx<=clipx[1]&&
		screeny>=clipy[0]&&screeny<=clipy[1])
		{
		if(!init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
			{
			/* assert (screen_limit == screen + dimy * dimx * 8); */
			unsigned char *s = screen + screenx*dimy*8 + screeny*8;
			if (s < screen_limit)
				{
				unsigned char c=(unsigned char)(texp & (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8));
				int32_t color_f=(texp & (BIT9|BIT10|BIT11))>>8;
				int32_t color_b=(texp & (BIT12|BIT13|BIT14))>>11;
				int32_t color_br=(texp & BIT15)>>14;

				*s++ = c;
				int32_t fg=color_f+(color_br?8:0);
				*s++ = uccolor[fg][0];
				*s++ = uccolor[fg][1];
				*s++ = uccolor[fg][2];
				*s++ = uccolor[color_b][0];
				*s++ = uccolor[color_b][1];
				*s++ = uccolor[color_b][2];
				++s;//extra byte

				screentexpos[screenx*dimy + screeny]=0;
				}
			return;
			}

		screentexpos[screenx*dimy + screeny]=texp;
		screentexpos_flag[screenx*dimy + screeny]=0;
		if(addcolor)screentexpos_flag[screenx*dimy + screeny]|=SCREENTEXPOS_FLAG_ADDCOLOR;
		}
}

void graphicst::add_top_tile(long texp,char addcolor)
{
	if(screenx>=clipx[0]&&screenx<=clipx[1]&&
		screeny>=clipy[0]&&screeny<=clipy[1])
		{
		if(!init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
			{
			/* assert (screen_limit == screen + dimy * dimx * 8); */
			unsigned char *s = screen + screenx*dimy*8 + screeny*8;
			if (s < screen_limit)
				{
				unsigned char c=(unsigned char)(texp & (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7|BIT8));
				int32_t color_f=(texp & (BIT9|BIT10|BIT11))>>8;
				int32_t color_b=(texp & (BIT12|BIT13|BIT14))>>11;
				int32_t color_br=(texp & BIT15)>>14;

				*s++ = c;
				int32_t fg=color_f+(color_br?8:0);
				*s++ = uccolor[fg][0];
				*s++ = uccolor[fg][1];
				*s++ = uccolor[fg][2];
				*s++ = uccolor[color_b][0];
				*s++ = uccolor[color_b][1];
				*s++ = uccolor[color_b][2];
				++s;//extra byte

				screentexpos[screenx*dimy + screeny]=0;
				}
			return;
			}

		screentexpos_top[screenx*dimy + screeny]=texp;
		screentexpos_top_flag[screenx*dimy + screeny]=0;
		if(addcolor)screentexpos_top_flag[screenx*dimy + screeny]|=SCREENTEXPOS_FLAG_ADDCOLOR;
		}
}

void graphicst::add_tile_grayscale(long texp,char cf,char cbr)
{
	if(screenx>=clipx[0]&&screenx<=clipx[1]&&
		screeny>=clipy[0]&&screeny<=clipy[1])
		{
		screentexpos[screenx*dimy + screeny]=texp;
		screentexpos_flag[screenx*dimy + screeny]=SCREENTEXPOS_FLAG_GRAYSCALE;
		if(use_old_16_colors)
			{
			int32_t fg=screenf+(screenbright?8:0);
			screen[screenx*dimy*8+screeny*8+1]=uccolor[fg][0];
			screen[screenx*dimy*8+screeny*8+2]=uccolor[fg][1];
			screen[screenx*dimy*8+screeny*8+3]=uccolor[fg][2];
			screen[screenx*dimy*8+screeny*8+4]=uccolor[screenb][0];
			screen[screenx*dimy*8+screeny*8+5]=uccolor[screenb][1];
			screen[screenx*dimy*8+screeny*8+6]=uccolor[screenb][2];
			}
		else
			{
			screen[screenx*dimy*8+screeny*8+1]=screen_color_r;
			screen[screenx*dimy*8+screeny*8+2]=screen_color_g;
			screen[screenx*dimy*8+screeny*8+3]=screen_color_b;
			screen[screenx*dimy*8+screeny*8+4]=screen_color_br;
			screen[screenx*dimy*8+screeny*8+5]=screen_color_bg;
			screen[screenx*dimy*8+screeny*8+6]=screen_color_bb;
			}
		}
}

void graphicst::draw_border(int x1, int x2, int y1, int y2) {
  // Upper and lower
  for (int x = x1; x <= x2; x++) {
    locate(y1, x);
    addchar(' ');
    locate(y2, x);
    addchar(' ');
  }
  // Left and right
  for (int y = y1+1; y < y2; y++) {
    locate(y, x1);
    addchar(' ');
    locate(y, x2);
    addchar(' ');
  }
}

void render_things()
{
  //GRAB CURRENT SCREEN AT THE END OF THE LIST
  viewscreenst *currentscreen=&gview.view;
  while(currentscreen->child!=NULL)currentscreen=currentscreen->child;
  
  //NO INTERFACE LEFT, LEAVE
  if(currentscreen==&gview.view)
	{
	enabler.must_do_render_things_before_display=false;
	return;
	}

  hooks_prerender();

  if(currentscreen->breakdownlevel==INTERFACE_BREAKDOWN_NONE)
	{
	uint32_t curtick=GetTickCount();
	currentscreen->render(curtick);
	// WANT THE WIDGETS TO RENDER LAST, BASICALLY ALWAYS--MOSTLY DFHACK, BUT OTHER REASONS TOO
	currentscreen->widgets.render(curtick);
	}
  else gps.erasescreen();

  // Render REC when recording macros. Definitely want this screen-specific. Or do we?
  const Time now = SDL_GetTicks();
  if (enabler.is_recording() && now % 1000 > 500) {
    gps.locate(0,0);
    gps.changecolor(4,1,1);
    gps.addst("REC");
  }
  // Render PLAY when playing a macro
  if (enabler.is_macro_playing() && now % 1000 <= 500) {
    gps.locate(0,0);
    gps.changecolor(2,1,1);
    gps.addst("PLAY");
  }
  // Render # <i> when building a repetition prefix
  if (enabler.prefix_building()) {
    gps.locate(0,0);
    gps.changecolor(3,1,1);
    gps.addst("#" + enabler.prefix());
  }
  if (gps.display_frames) {
    ostringstream fps_stream;
    fps_stream << "FPS: " << setw(3) << enabler.calculate_fps() << " (" << enabler.calculate_gfps() << ")";
    string fps = fps_stream.str();
    gps.changecolor(0,3,0);
    //static gps_locator fps_locator(init.display.grid_y-1, 40);
    //fps_locator(fps.size());
	gps.locate(init.display.grid_y-gps.frame_display_dy, gps.frame_display_sx);
    gps.addst(fps);
  }

	if(!gps.main_thread_requesting_reshape)enabler.must_do_render_things_before_display=false;
}

void graphic_viewportst::add_building_one_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_building_one[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_building_two_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_building_two[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_top_shadow_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_top_shadow[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_vehicle_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_vehicle[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_spatter_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_spatter[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_projectile_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_projectile[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_high_flow_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_high_flow[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_vermin_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_vermin[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_left_creature_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_left_creature[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_right_creature_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_right_creature[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_upleft_creature_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_upleft_creature[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_up_creature_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_up_creature[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_upright_creature_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_upright_creature[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_signpost_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_signpost[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_designation_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_designation[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_interface_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_interface[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_item_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_item[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_spatter_flag_fire(uint32_t nspatter_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_spatter_flag[screen_x*dim_y + screen_y]&=~VIEWPORT_SPATTER_FLAG_FIRE_BITS;
		screentexpos_spatter_flag[screen_x*dim_y + screen_y]|=nspatter_flag;
		}
}

void graphic_viewportst::add_spatter_flag(uint32_t nspatter_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_spatter_flag[screen_x*dim_y + screen_y]=nspatter_flag;
		}
}

void graphic_viewportst::add_liquid_flag(uint32_t nliquid_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_liquid_flag[screen_x*dim_y + screen_y]=nliquid_flag;
		}
}

void graphic_viewportst::add_ramp_flag(uint64_t nramp_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_ramp_flag[screen_x*dim_y + screen_y]=nramp_flag;
		}
}

void graphic_viewportst::add_shadow_flag(uint32_t nshadow_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_shadow_flag[screen_x*dim_y + screen_y]=nshadow_flag;
		}
}

void graphic_viewportst::add_floor_flag(uint64_t nfloor_flag)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_floor_flag[screen_x*dim_y + screen_y]=nfloor_flag;
		}
}

void graphic_viewportst::add_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_background_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_background[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::add_background_two_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_background_two[screen_x*dim_y + screen_y]=texp;
		}
}

void graphic_viewportst::set_dims_and_allocate(int32_t ndx,int32_t ndy)
{
	clean_arrays();

	dim_x=ndx;
	dim_y=ndy;

	screentexpos=new int32_t[dim_x*dim_y];
	screentexpos_background=new int32_t[dim_x*dim_y];
	screentexpos_background_two=new int32_t[dim_x*dim_y];
	screentexpos_floor_flag=new uint64_t[dim_x*dim_y];
	screentexpos_spatter_flag=new uint32_t[dim_x*dim_y];
	screentexpos_spatter=new int32_t[dim_x*dim_y];
	screentexpos_liquid_flag=new uint32_t[dim_x*dim_y];
	screentexpos_ramp_flag=new uint64_t[dim_x*dim_y];
	screentexpos_shadow_flag=new uint32_t[dim_x*dim_y];
	screentexpos_building_one=new int32_t[dim_x*dim_y];
	screentexpos_building_two=new int32_t[dim_x*dim_y];
	screentexpos_top_shadow=new int32_t[dim_x*dim_y];
	screentexpos_item=new int32_t[dim_x*dim_y];
	screentexpos_vehicle=new int32_t[dim_x*dim_y];
	screentexpos_projectile=new int32_t[dim_x*dim_y];
	screentexpos_high_flow=new int32_t[dim_x*dim_y];
	screentexpos_vermin=new int32_t[dim_x*dim_y];
	screentexpos_signpost=new int32_t[dim_x*dim_y];
	screentexpos_left_creature=new int32_t[dim_x*dim_y];
	screentexpos_right_creature=new int32_t[dim_x*dim_y];
	screentexpos_upleft_creature=new int32_t[dim_x*dim_y];
	screentexpos_up_creature=new int32_t[dim_x*dim_y];
	screentexpos_upright_creature=new int32_t[dim_x*dim_y];
	screentexpos_designation=new int32_t[dim_x*dim_y];
	screentexpos_interface=new int32_t[dim_x*dim_y];

	screentexpos_old=new int32_t[dim_x*dim_y];
	screentexpos_background_old=new int32_t[dim_x*dim_y];
	screentexpos_background_two_old=new int32_t[dim_x*dim_y];
	screentexpos_floor_flag_old=new uint64_t[dim_x*dim_y];
	screentexpos_spatter_flag_old=new uint32_t[dim_x*dim_y];
	screentexpos_spatter_old=new int32_t[dim_x*dim_y];
	screentexpos_liquid_flag_old=new uint32_t[dim_x*dim_y];
	screentexpos_ramp_flag_old=new uint64_t[dim_x*dim_y];
	screentexpos_shadow_flag_old=new uint32_t[dim_x*dim_y];
	screentexpos_building_one_old=new int32_t[dim_x*dim_y];
	screentexpos_building_two_old=new int32_t[dim_x*dim_y];
	screentexpos_top_shadow_old=new int32_t[dim_x*dim_y];
	screentexpos_item_old=new int32_t[dim_x*dim_y];
	screentexpos_vehicle_old=new int32_t[dim_x*dim_y];
	screentexpos_projectile_old=new int32_t[dim_x*dim_y];
	screentexpos_high_flow_old=new int32_t[dim_x*dim_y];
	screentexpos_vermin_old=new int32_t[dim_x*dim_y];
	screentexpos_signpost_old=new int32_t[dim_x*dim_y];
	screentexpos_left_creature_old=new int32_t[dim_x*dim_y];
	screentexpos_right_creature_old=new int32_t[dim_x*dim_y];
	screentexpos_upleft_creature_old=new int32_t[dim_x*dim_y];
	screentexpos_up_creature_old=new int32_t[dim_x*dim_y];
	screentexpos_upright_creature_old=new int32_t[dim_x*dim_y];
	screentexpos_designation_old=new int32_t[dim_x*dim_y];
	screentexpos_interface_old=new int32_t[dim_x*dim_y];

	core_tree_species_plus_one=new int16_t[dim_x*dim_y];
	shadow_tree_species_plus_one=new int16_t[dim_x*dim_y];

	clear_screen();

	memset(screentexpos_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_background_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_background_two_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_floor_flag_old,0,sizeof(uint64_t)*dim_x*dim_y);
	memset(screentexpos_spatter_flag_old,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_spatter_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_liquid_flag_old,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_ramp_flag_old,0,sizeof(uint64_t)*dim_x*dim_y);
	memset(screentexpos_shadow_flag_old,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_building_one_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_building_two_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_top_shadow_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_item_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_vehicle_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_projectile_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_high_flow_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_vermin_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_signpost_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_designation_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_interface_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_left_creature_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_right_creature_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_upleft_creature_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_up_creature_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_upright_creature_old,0,sizeof(int32_t)*dim_x*dim_y);

	int32_t i;
	for(i=0;i<dim_x*dim_y;++i)
		{
		screentexpos_background_old[i]=gps.black_background_texpos[0];
		}

	set_clipping(0,ndx-1,0,ndy-1);
}

void graphic_viewportst::clear_screen()
{
	if(screentexpos==NULL)return;

	memset(screentexpos,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_background,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_background_two,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_floor_flag,0,sizeof(uint64_t)*dim_x*dim_y);
	memset(screentexpos_spatter_flag,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_spatter,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_liquid_flag,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_ramp_flag,0,sizeof(uint64_t)*dim_x*dim_y);
	memset(screentexpos_shadow_flag,0,sizeof(uint32_t)*dim_x*dim_y);
	memset(screentexpos_building_one,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_building_two,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_top_shadow,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_item,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_vehicle,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_projectile,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_high_flow,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_vermin,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_signpost,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_designation,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_interface,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_left_creature,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_right_creature,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_upleft_creature,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_up_creature,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_upright_creature,0,sizeof(int32_t)*dim_x*dim_y);

	int32_t i;
	for(i=0;i<dim_x*dim_y;++i)
		{
		screentexpos_background[i]=gps.black_background_texpos[0];
		}

	memset(core_tree_species_plus_one,0,sizeof(int16_t)*dim_x*dim_y);
	memset(shadow_tree_species_plus_one,0,sizeof(int16_t)*dim_x*dim_y);
}

void graphic_map_portst::clear_screen()
{
	if(screentexpos_base==NULL)return;

	memset(screentexpos_base,0,sizeof(int32_t)*dim_x*dim_y);
	int32_t ei;
	for(ei=0;ei<8;++ei)
		{
		memset(screentexpos_edge[ei],0,sizeof(int32_t)*dim_x*dim_y);
		memset(screentexpos_edge2[ei],0,sizeof(int32_t)*dim_x*dim_y);
		}
	memset(screentexpos_detail,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_tunnel,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_river,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_road,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_site,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_army,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_interface,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_n,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_s,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_w,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_e,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_nw,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_ne,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_sw,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_se,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_site_to_s,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_cloud_bits,0,sizeof(uint64_t)*dim_x*dim_y);
}

int32_t graphicst::create_alpha_texture(int32_t texpos,Uint8 alpha_level)
{
	int32_t new_tex=0;

	SDL_Surface *src=enabler.textures.get_texture_data(texpos);
	if(src!=NULL)
		{

		new_tex=enabler.textures.create_texture(src->w,src->h);
		SDL_Surface *dst=enabler.textures.get_texture_data(new_tex);
		if(dst!=NULL)
			{
			Uint8 orig_alpha;
			SDL_BlendMode orig_mode;
			SDL_GetSurfaceAlphaMod(src,&orig_alpha);
			SDL_GetSurfaceBlendMode(src,&orig_mode);
			SDL_SetSurfaceAlphaMod(src,alpha_level);
			SDL_SetSurfaceBlendMode(src,SDL_BLENDMODE_NONE);
			SDL_BlitSurface(src,NULL,dst,NULL);
			SDL_SetSurfaceAlphaMod(src,orig_alpha);
			SDL_SetSurfaceBlendMode(src,orig_mode);
			}

		}

	return new_tex;
}

void graphicst::copy_pixel_data_with_alpha_mask(SDL_Surface *src,SDL_Surface *alpha_src,SDL_Surface *dst)
{
	//assumes all surfaces are locked, same dimensions for src/dst
	Uint8 *pixel_src,*pixel_alpha_src,*pixel_dst;
	int x,y;
	for(y=0;y<src->h;y++)
		{
		pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
		pixel_alpha_src=((Uint8*)alpha_src->pixels)+(y*alpha_src->pitch);
		pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
		for(x=0;x<src->w;++x,pixel_src+=4,pixel_alpha_src+=4,pixel_dst+=4)
			{
			pixel_dst[0]=pixel_src[0];
			pixel_dst[1]=pixel_src[1];
			pixel_dst[2]=pixel_src[2];
			pixel_dst[3]=(Uint8)((int32_t)pixel_src[3]*(int32_t)pixel_alpha_src[3]/255);
			}
		}
}
	
void graphicst::copy_transformed_pixel_data(SDL_Surface *src,SDL_Surface *dst,int32_t cw_rotate,bool flip_vert)
{
	//assumes both surfaces are locked, same dimensions for src/dst, square
	if(flip_vert)
		{
		if(cw_rotate==0)
			{
			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				pixel_dst=((Uint8*)dst->pixels)+((src->h-1-y)*dst->pitch);
				for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
					{
					pixel_dst[0]=pixel_src[0];
					pixel_dst[1]=pixel_src[1];
					pixel_dst[2]=pixel_src[2];
					pixel_dst[3]=pixel_src[3];
					}
				}
			}
		else if(cw_rotate==90)
			{
			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				for(x=0;x<src->w;++x,pixel_src+=4)
					{
					pixel_dst=((Uint8*)dst->pixels)+((src->w-1-x)*dst->pitch)+(src->h-1-y)*4;

					pixel_dst[0]=pixel_src[0];
					pixel_dst[1]=pixel_src[1];
					pixel_dst[2]=pixel_src[2];
					pixel_dst[3]=pixel_src[3];
					}
				}
			}
		else if(cw_rotate==180)//horizontal flip
			{
			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch)+(src->w-1)*4;
				for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst-=4)
					{
					pixel_dst[0]=pixel_src[0];
					pixel_dst[1]=pixel_src[1];
					pixel_dst[2]=pixel_src[2];
					pixel_dst[3]=pixel_src[3];
					}
				}
			}
		else if(cw_rotate==270)
			{
			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				for(x=0;x<src->w;++x,pixel_src+=4)
					{
					pixel_dst=((Uint8*)dst->pixels)+(x*dst->pitch)+(y*4);

					pixel_dst[0]=pixel_src[0];
					pixel_dst[1]=pixel_src[1];
					pixel_dst[2]=pixel_src[2];
					pixel_dst[3]=pixel_src[3];
					}
				}
			}
		return;
		}
	if(cw_rotate==0)
		{
		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<src->h;y++)
			{
			pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
			pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
			for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
				{
				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}
		}
	else if(cw_rotate==90)
		{
		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<src->h;y++)
			{
			pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
			for(x=0;x<src->w;++x,pixel_src+=4)
				{
				pixel_dst=((Uint8*)dst->pixels)+(x*dst->pitch)+(src->h-1-y)*4;

				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}
		}
	else if(cw_rotate==180)
		{
		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<src->h;y++)
			{
			pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
			pixel_dst=((Uint8*)dst->pixels)+((src->h-1-y)*dst->pitch)+(src->w-1)*4;
			for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst-=4)
				{
				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}
		}
	else if(cw_rotate==270)
		{
		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<src->h;y++)
			{
			pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
			for(x=0;x<src->w;++x,pixel_src+=4)
				{
				pixel_dst=((Uint8*)dst->pixels)+((src->w-1-x)*dst->pitch)+y*4;

				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}
		}
}

void graphicst::copy_pixel_data_nonzero_only_no_blend(SDL_Surface *src,SDL_Surface *dst)
{
	//assumes all surfaces are locked, same dimensions for src/dst
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
				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}
		}
}

void graphic_map_portst::add_base_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_base[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_edge_biome_data(int32_t dt)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		edge_biome_data[screen_x + screen_y*dim_x]=dt;
		}
}

void graphic_map_portst::add_edge_tile(int32_t i,long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_edge[i][screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_edge2_tile(int32_t i,long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_edge2[i][screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_tunnel_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_tunnel[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_river_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_river[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_road_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_road[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_site_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_site[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_army_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_army[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_interface_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_interface[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_n_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_n[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_s_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_s[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_w_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_w[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_e_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_e[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_nw_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_nw[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_ne_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_ne[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_sw_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_sw[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_detail_to_se_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_detail_to_se[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_site_to_s_tile(long texp)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_site_to_s[screen_x + screen_y*dim_x]=texp;
		}
}

void graphic_map_portst::add_cloud_bits(uint64_t cloud_bits)
{
	if(screen_x>=clipx[0]&&screen_x<=clipx[1]&&
		screen_y>=clipy[0]&&screen_y<=clipy[1])
		{
		screentexpos_cloud_bits[screen_x + screen_y*dim_x]|=cloud_bits;
		}
}

void graphic_map_portst::set_dims_and_allocate(int32_t ndx,int32_t ndy)
{
	clean_arrays();

	dim_x=ndx;
	dim_y=ndy;

	screentexpos_base=new int32_t[dim_x*dim_y];
	int32_t ei;
	for(ei=0;ei<8;++ei)
		{
		screentexpos_edge[ei]=new int32_t[dim_x*dim_y];
		screentexpos_edge2[ei]=new int32_t[dim_x*dim_y];
		}
	screentexpos_detail=new int32_t[dim_x*dim_y];
	screentexpos_tunnel=new int32_t[dim_x*dim_y];
	screentexpos_river=new int32_t[dim_x*dim_y];
	screentexpos_road=new int32_t[dim_x*dim_y];
	screentexpos_site=new int32_t[dim_x*dim_y];
	screentexpos_army=new int32_t[dim_x*dim_y];
	screentexpos_interface=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_n=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_s=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_w=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_e=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_nw=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_ne=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_sw=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_se=new int32_t[dim_x*dim_y];
	screentexpos_site_to_s=new int32_t[dim_x*dim_y];
	screentexpos_cloud_bits=new uint64_t[dim_x*dim_y];

	clear_screen();

	screentexpos_base_old=new int32_t[dim_x*dim_y];
	for(ei=0;ei<8;++ei)
		{
		screentexpos_edge_old[ei]=new int32_t[dim_x*dim_y];
		screentexpos_edge2_old[ei]=new int32_t[dim_x*dim_y];
		}
	screentexpos_detail_old=new int32_t[dim_x*dim_y];
	screentexpos_tunnel_old=new int32_t[dim_x*dim_y];
	screentexpos_river_old=new int32_t[dim_x*dim_y];
	screentexpos_road_old=new int32_t[dim_x*dim_y];
	screentexpos_site_old=new int32_t[dim_x*dim_y];
	screentexpos_army_old=new int32_t[dim_x*dim_y];
	screentexpos_interface_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_n_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_s_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_w_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_e_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_nw_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_ne_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_sw_old=new int32_t[dim_x*dim_y];
	screentexpos_detail_to_se_old=new int32_t[dim_x*dim_y];
	screentexpos_site_to_s_old=new int32_t[dim_x*dim_y];
	screentexpos_cloud_bits_old=new uint64_t[dim_x*dim_y];

	memset(screentexpos_base_old,0,sizeof(int32_t)*dim_x*dim_y);
	for(ei=0;ei<8;++ei)
		{
		memset(screentexpos_edge_old[ei],0,sizeof(int32_t)*dim_x*dim_y);
		memset(screentexpos_edge2_old[ei],0,sizeof(int32_t)*dim_x*dim_y);
		}
	memset(screentexpos_detail_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_tunnel_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_river_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_road_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_site_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_army_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_interface_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_n_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_s_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_w_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_e_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_nw_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_ne_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_sw_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_detail_to_se_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_site_to_s_old,0,sizeof(int32_t)*dim_x*dim_y);
	memset(screentexpos_cloud_bits_old,0,sizeof(uint64_t)*dim_x*dim_y);

	edge_biome_data=new int32_t[dim_x*dim_y];
	edge_type_n=new int32_t[dim_x*dim_y];
	edge_type_s=new int32_t[dim_x*dim_y];
	edge_type_w=new int32_t[dim_x*dim_y];
	edge_type_e=new int32_t[dim_x*dim_y];
	edge_type_nw=new int32_t[dim_x*dim_y];
	edge_type_ne=new int32_t[dim_x*dim_y];
	edge_type_sw=new int32_t[dim_x*dim_y];
	edge_type_se=new int32_t[dim_x*dim_y];
	edge_biome_n=new int32_t[dim_x*dim_y];
	edge_biome_s=new int32_t[dim_x*dim_y];
	edge_biome_w=new int32_t[dim_x*dim_y];
	edge_biome_e=new int32_t[dim_x*dim_y];
	edge_biome_nw=new int32_t[dim_x*dim_y];
	edge_biome_ne=new int32_t[dim_x*dim_y];
	edge_biome_sw=new int32_t[dim_x*dim_y];
	edge_biome_se=new int32_t[dim_x*dim_y];

	//************************ MAP PORT set dims (clear 2)
	/*
	int32_t i;
	for(i=0;i<dim_x*dim_y;++i)
		{
		screentexpos_background_old[i]=gps.black_background_texpos[0];
		}
	*/

	set_clipping(0,ndx-1,0,ndy-1);
}

void graphicst::create_derived_textures()
{
	/*
	if(wall_shadow_straight_n!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(wall_shadow_straight_n);
		if(tex!=NULL)
			{
			SDL_LockSurface(tex);

			int32_t *tpp;
			int32_t i;
			for(i=0;i<3;++i)
				{
				switch(i)
					{
					case 0:tpp=&wall_shadow_straight_s;break;
					case 1:tpp=&wall_shadow_straight_w;break;
					case 2:tpp=&wall_shadow_straight_e;break;
					}
				*tpp=enabler.textures.create_texture(32,32);

				SDL_Surface *color=enabler.textures.get_texture_data(*tpp);
				if(color!=NULL)
					{
					SDL_LockSurface(color);
      
					switch(i)
						{
						case 0:copy_transformed_pixel_data(tex,color,0,true);break;
						case 1:copy_transformed_pixel_data(tex,color,270,false);break;
						case 2:copy_transformed_pixel_data(tex,color,90,false);break;
						}

					SDL_UnlockSurface(color);
					}
				}
			SDL_UnlockSurface(tex);
			}
		}
	if(wall_shadow_near_n_open_ne!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(wall_shadow_near_n_open_ne);
		if(tex!=NULL)
			{
			SDL_LockSurface(tex);

			int32_t *tpp;
			int32_t i;
			for(i=0;i<7;++i)
				{
				switch(i)
					{
					case 0:tpp=&wall_shadow_near_n_open_nw;break;
					case 1:tpp=&wall_shadow_near_s_open_sw;break;
					case 2:tpp=&wall_shadow_near_s_open_se;break;
					case 3:tpp=&wall_shadow_near_w_open_nw;break;
					case 4:tpp=&wall_shadow_near_w_open_sw;break;
					case 5:tpp=&wall_shadow_near_e_open_ne;break;
					case 6:tpp=&wall_shadow_near_e_open_se;break;
					}
				*tpp=enabler.textures.create_texture(32,32);

				SDL_Surface *color=enabler.textures.get_texture_data(*tpp);
				if(color!=NULL)
					{
					SDL_LockSurface(color);
      
					switch(i)
						{
						case 0:copy_transformed_pixel_data(tex,color,180,true);break;
						case 1:copy_transformed_pixel_data(tex,color,180,false);break;
						case 2:copy_transformed_pixel_data(tex,color,0,true);break;
						case 3:copy_transformed_pixel_data(tex,color,270,false);break;
						case 4:copy_transformed_pixel_data(tex,color,270,true);break;
						case 5:copy_transformed_pixel_data(tex,color,90,true);break;
						case 6:copy_transformed_pixel_data(tex,color,90,false);break;
						}

					SDL_UnlockSurface(color);
					}
				}
			SDL_UnlockSurface(tex);
			}
		}
	if(wall_shadow_corner_nw!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(wall_shadow_corner_nw);
		if(tex!=NULL)
			{
			SDL_LockSurface(tex);

			int32_t *tpp;
			int32_t i;
			for(i=0;i<3;++i)
				{
				switch(i)
					{
					case 0:tpp=&wall_shadow_corner_ne;break;
					case 1:tpp=&wall_shadow_corner_sw;break;
					case 2:tpp=&wall_shadow_corner_se;break;
					}
				*tpp=enabler.textures.create_texture(32,32);

				SDL_Surface *color=enabler.textures.get_texture_data(*tpp);
				if(color!=NULL)
					{
					SDL_LockSurface(color);
      
					switch(i)
						{
						case 0:copy_transformed_pixel_data(tex,color,90,false);break;
						case 1:copy_transformed_pixel_data(tex,color,270,false);break;
						case 2:copy_transformed_pixel_data(tex,color,180,false);break;
						}

					SDL_UnlockSurface(color);
					}
				}
			SDL_UnlockSurface(tex);
			}
		}
		*/
//**************************** MISSING END WALL
	if(wall_shadow_end_wall_w!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(wall_shadow_end_wall_w);
		if(tex!=NULL)
			{
			SDL_LockSurface(tex);

			int32_t *tpp;
			int32_t i;
			for(i=0;i<3;++i)
				{
				switch(i)
					{
					case 0:tpp=&wall_shadow_end_wall_n;break;
					case 1:tpp=&wall_shadow_end_wall_s;break;
					case 2:tpp=&wall_shadow_end_wall_e;break;
					}
				*tpp=enabler.textures.create_texture(tex->w,tex->h);

				SDL_Surface *color=enabler.textures.get_texture_data(*tpp);
				if(color!=NULL)
					{
					SDL_LockSurface(color);
      
					switch(i)
						{
						case 0:copy_transformed_pixel_data(tex,color,90,false);break;
						case 1:copy_transformed_pixel_data(tex,color,270,false);break;
						case 2:copy_transformed_pixel_data(tex,color,180,false);break;
						}

					SDL_UnlockSurface(color);
					}
				}
			SDL_UnlockSurface(tex);
			}
		}
	if(multilevel_depth_texpos[0]!=0)
		{
		SDL_Surface *src=enabler.textures.get_texture_data(multilevel_depth_texpos[0]);
		if(src!=NULL)
			{
			SDL_LockSurface(src);

			int32_t i;
			for(i=1;i<LOWER_VIEWPORT_MAX;++i)
				{
				multilevel_depth_texpos[i]=enabler.textures.create_texture(src->w,src->h);
				SDL_Surface *dst=enabler.textures.get_texture_data(multilevel_depth_texpos[i]);
				if(dst!=NULL)
					{
					SDL_LockSurface(dst);

					Uint8 *pixel_src,*pixel_dst;
					int x,y;
					for(y=0;y<src->h;y++)
						{
						pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
						pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
						for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
							{
							pixel_dst[0]=pixel_src[0];
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=pixel_src[2];
							int32_t dist=255-(int32_t)pixel_src[3];
							int32_t shift=dist*i/LOWER_VIEWPORT_MAX;
							pixel_dst[3]=pixel_src[3]+shift;
							}
						}

					SDL_UnlockSurface(dst);
					}
				}

			SDL_UnlockSurface(src);
			}
		}

	int32_t alpha_level=128;
	if(gps.texpos_item_cage_metal!=0)gps.texpos_planned_bld_cage=create_alpha_texture(gps.texpos_item_cage_metal,alpha_level);
	if(gps.texpos_item_bag!=0)gps.texpos_planned_bld_bag=create_alpha_texture(gps.texpos_item_bag,alpha_level);
	if(gps.texpos_item_box!=0)gps.texpos_planned_bld_box=create_alpha_texture(gps.texpos_item_box,alpha_level);
	if(gps.texpos_item_cabinet!=0)gps.texpos_planned_bld_cabinet=create_alpha_texture(gps.texpos_item_cabinet,alpha_level);
	if(gps.texpos_item_table!=0)gps.texpos_planned_bld_table=create_alpha_texture(gps.texpos_item_table,alpha_level);
	if(gps.texpos_item_animal_trap_metal!=0)gps.texpos_planned_bld_animal_trap=create_alpha_texture(gps.texpos_item_animal_trap_metal,alpha_level);
	if(gps.texpos_item_chair!=0)gps.texpos_planned_bld_chair=create_alpha_texture(gps.texpos_item_chair,alpha_level);
	if(gps.texpos_item_bed!=0)gps.texpos_planned_bld_bed=create_alpha_texture(gps.texpos_item_bed,alpha_level);
	if(gps.texpos_item_statue!=0)gps.texpos_planned_bld_statue=create_alpha_texture(gps.texpos_item_statue,alpha_level);
	if(gps.texpos_item_slab_blank!=0)gps.texpos_planned_bld_slab=create_alpha_texture(gps.texpos_item_slab_blank,alpha_level);
	if(gps.texpos_item_hatch_cover_stone!=0)gps.texpos_planned_bld_hatch_cover=create_alpha_texture(gps.texpos_item_hatch_cover_stone,alpha_level);
	if(gps.texpos_item_hatch_cover_wooden!=0)gps.texpos_planned_bld_hatch_cover=create_alpha_texture(gps.texpos_item_hatch_cover_wooden,alpha_level);
	if(gps.texpos_item_hatch_cover_metal!=0)gps.texpos_planned_bld_hatch_cover=create_alpha_texture(gps.texpos_item_hatch_cover_metal,alpha_level);
	if(gps.texpos_item_hatch_cover_glass!=0)gps.texpos_planned_bld_hatch_cover=create_alpha_texture(gps.texpos_item_hatch_cover_glass,alpha_level);
	if(gps.texpos_item_coffin_wood!=0)texpos_planned_bld_coffin_wooden=create_alpha_texture(gps.texpos_item_coffin_wood,alpha_level);
	if(gps.texpos_item_coffin_stone!=0)texpos_planned_bld_coffin_stone=create_alpha_texture(gps.texpos_item_coffin_stone,alpha_level);
	if(gps.texpos_item_coffin_metal!=0)texpos_planned_bld_coffin_metal=create_alpha_texture(gps.texpos_item_coffin_metal,alpha_level);
	if(gps.texpos_item_coffin_glass!=0)texpos_planned_bld_coffin_glass=create_alpha_texture(gps.texpos_item_coffin_glass,alpha_level);
	if(gps.texpos_item_traction_bench_stone_chain!=0)texpos_planned_bld_traction_bench_stone_chain=create_alpha_texture(gps.texpos_item_traction_bench_stone_chain,alpha_level);
	if(gps.texpos_item_traction_bench_stone_rope!=0)texpos_planned_bld_traction_bench_stone_rope=create_alpha_texture(gps.texpos_item_traction_bench_stone_rope,alpha_level);
	if(gps.texpos_item_traction_bench_wooden_chain!=0)texpos_planned_bld_traction_bench_wooden_chain=create_alpha_texture(gps.texpos_item_traction_bench_wooden_chain,alpha_level);
	if(gps.texpos_item_traction_bench_wooden_rope!=0)texpos_planned_bld_traction_bench_wooden_rope=create_alpha_texture(gps.texpos_item_traction_bench_wooden_rope,alpha_level);
	if(gps.texpos_item_traction_bench_metal_chain!=0)texpos_planned_bld_traction_bench_metal_chain=create_alpha_texture(gps.texpos_item_traction_bench_metal_chain,alpha_level);
	if(gps.texpos_item_traction_bench_metal_rope!=0)texpos_planned_bld_traction_bench_metal_rope=create_alpha_texture(gps.texpos_item_traction_bench_metal_rope,alpha_level);
	if(gps.texpos_item_traction_bench_glass_chain!=0)texpos_planned_bld_traction_bench_glass_chain=create_alpha_texture(gps.texpos_item_traction_bench_glass_chain,alpha_level);
	if(gps.texpos_item_traction_bench_glass_rope!=0)texpos_planned_bld_traction_bench_glass_rope=create_alpha_texture(gps.texpos_item_traction_bench_glass_rope,alpha_level);
	if(gps.texpos_item_door_stone!=0)gps.texpos_planned_bld_door_stone=create_alpha_texture(gps.texpos_item_door_stone,alpha_level);
	if(gps.texpos_item_door_wooden!=0)gps.texpos_planned_bld_door_wooden=create_alpha_texture(gps.texpos_item_door_wooden,alpha_level);
	if(gps.texpos_item_door_metal!=0)gps.texpos_planned_bld_door_metal=create_alpha_texture(gps.texpos_item_door_metal,alpha_level);
	if(gps.texpos_item_door_glass!=0)gps.texpos_planned_bld_door_glass=create_alpha_texture(gps.texpos_item_door_glass,alpha_level);

	if(gps.workshop_quern_texpos[1][0]!=0&&gps.workshop_quern_texpos[0][0]==0)gps.workshop_quern_texpos[0][0]=create_alpha_texture(gps.workshop_quern_texpos[1][0],alpha_level);
	if(gps.workshop_quern_texpos[1][1]!=0&&gps.workshop_quern_texpos[0][1]==0)gps.workshop_quern_texpos[0][1]=create_alpha_texture(gps.workshop_quern_texpos[1][1],alpha_level);
	if(gps.workshop_millstone_texpos[1][0]!=0&&gps.workshop_millstone_texpos[0][0]==0)gps.workshop_millstone_texpos[0][0]=create_alpha_texture(gps.workshop_millstone_texpos[1][0],alpha_level);
	if(gps.workshop_millstone_texpos[1][1]!=0&&gps.workshop_millstone_texpos[0][1]==0)gps.workshop_millstone_texpos[0][1]=create_alpha_texture(gps.workshop_millstone_texpos[1][1],alpha_level);

	if(gps.texpos_bld_chain_metal[0][0]!=0)gps.texpos_planned_bld_chain_metal=create_alpha_texture(gps.texpos_bld_chain_metal[0][0],alpha_level);
	if(gps.texpos_bld_chain_rope[0][0]!=0)gps.texpos_planned_bld_chain_rope=create_alpha_texture(gps.texpos_bld_chain_rope[0][0],alpha_level);
	if(gps.texpos_bld_archery_target_stone!=0)gps.texpos_planned_bld_archery_target_stone=create_alpha_texture(gps.texpos_bld_archery_target_stone,alpha_level);
	if(gps.texpos_bld_archery_target_metal!=0)gps.texpos_planned_bld_archery_target_metal=create_alpha_texture(gps.texpos_bld_archery_target_metal,alpha_level);
	if(gps.texpos_bld_archery_target_wood!=0)gps.texpos_planned_bld_archery_target_wood=create_alpha_texture(gps.texpos_bld_archery_target_wood,alpha_level);
	if(gps.texpos_bld_well!=0)gps.texpos_planned_bld_well=create_alpha_texture(gps.texpos_bld_well,alpha_level);
	if(gps.texpos_item_grate_metal_floor_closed!=0)gps.texpos_planned_bld_floor_grate=create_alpha_texture(gps.texpos_item_grate_metal_floor_closed,alpha_level);
	if(gps.texpos_item_grate_metal_wall_closed!=0)gps.texpos_planned_bld_wall_grate=create_alpha_texture(gps.texpos_item_grate_metal_wall_closed,alpha_level);
	if(gps.texpos_item_floodgate_stone!=0)gps.texpos_planned_bld_floodgate_stone=create_alpha_texture(gps.texpos_item_floodgate_stone,alpha_level);
	if(gps.texpos_item_floodgate_wooden!=0)gps.texpos_planned_bld_floodgate_wooden=create_alpha_texture(gps.texpos_item_floodgate_wooden,alpha_level);
	if(gps.texpos_item_floodgate_metal!=0)gps.texpos_planned_bld_floodgate_metal=create_alpha_texture(gps.texpos_item_floodgate_metal,alpha_level);
	if(gps.texpos_item_floodgate_glass!=0)gps.texpos_planned_bld_floodgate_glass=create_alpha_texture(gps.texpos_item_floodgate_glass,alpha_level);
	if(gps.texpos_bld_paved_road!=0)gps.texpos_planned_bld_paved_road=create_alpha_texture(gps.texpos_bld_paved_road,alpha_level);
	if(gps.texpos_bld_dirt_road!=0)gps.texpos_planned_bld_dirt_road=create_alpha_texture(gps.texpos_bld_dirt_road,alpha_level);

	if(gps.texpos_bld_vertical_bars_w!=0&&
		gps.texpos_bld_vertical_bars_e!=0&&
		gps.texpos_bld_vertical_bars_n_back!=0&&
		gps.texpos_bld_vertical_bars_s!=0)
		{
		{int32_t i;
		for(i=0;i<10;++i)
		{
		int32_t src[3]={0,0,0};
		int32_t *dst;
		switch(i)
			{
			case 0:
				dst=&gps.texpos_bld_vertical_bars_we;
				src[0]=gps.texpos_bld_vertical_bars_w;
				src[1]=texpos_bld_vertical_bars_e;
				break;
			case 1:
				dst=&gps.texpos_bld_vertical_bars_nwe;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=gps.texpos_bld_vertical_bars_w;
				src[2]=texpos_bld_vertical_bars_e;
				break;
			case 2:
				dst=&gps.texpos_bld_vertical_bars_nw;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=texpos_bld_vertical_bars_w;
				break;
			case 3:
				dst=&gps.texpos_bld_vertical_bars_ne;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=texpos_bld_vertical_bars_e;
				break;
			case 4:
				dst=&gps.texpos_bld_vertical_bars_sw;
				src[0]=gps.texpos_bld_vertical_bars_w;
				src[1]=texpos_bld_vertical_bars_s;
				break;
			case 5:
				dst=&gps.texpos_bld_vertical_bars_se;
				src[0]=gps.texpos_bld_vertical_bars_e;
				src[1]=texpos_bld_vertical_bars_s;
				break;
			case 6:
				dst=&gps.texpos_bld_vertical_bars_nsw;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=gps.texpos_bld_vertical_bars_w;
				src[2]=texpos_bld_vertical_bars_s;
				break;
			case 7:
				dst=&gps.texpos_bld_vertical_bars_nse;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=gps.texpos_bld_vertical_bars_e;
				src[2]=texpos_bld_vertical_bars_s;
				break;
			case 8:
				dst=&gps.texpos_bld_vertical_bars_swe;
				src[0]=gps.texpos_bld_vertical_bars_w;
				src[1]=gps.texpos_bld_vertical_bars_e;
				src[2]=texpos_bld_vertical_bars_s;
				break;
			case 9:
				dst=&gps.texpos_bld_vertical_bars_ns;
				src[0]=gps.texpos_bld_vertical_bars_n_back;
				src[1]=texpos_bld_vertical_bars_s;
				break;
			}
		if(src[0]==0)continue;
		SDL_Surface *tex=enabler.textures.get_texture_data(src[0]);
		if(tex==NULL)continue;
		*dst=enabler.textures.create_texture(tex->w,tex->h);
		SDL_Surface *dsts=enabler.textures.get_texture_data(*dst);
		if(dsts==NULL)continue;

		//copy the first one over
		SDL_LockSurface(tex);
		SDL_LockSurface(dsts);

		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<tex->h;y++)
			{
			pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
			pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
			for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4)
				{
				pixel_dst[0]=pixel_src[0];
				pixel_dst[1]=pixel_src[1];
				pixel_dst[2]=pixel_src[2];
				pixel_dst[3]=pixel_src[3];
				}
			}

		SDL_UnlockSurface(dsts);
		SDL_UnlockSurface(tex);

		int32_t si;
		for(si=1;si<3;++si)
			{
			if(src[si]==0)continue;

			SDL_Surface *tex=enabler.textures.get_texture_data(src[si]);
			if(tex==NULL)continue;

			//NOTE: a blit did not work here, it overwrote instead of doing alpha blending
			SDL_LockSurface(tex);
			SDL_LockSurface(dsts);

			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<tex->h;y++)
				{
				pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
				pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
				for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4)
					{
					//******************************** BROOKBED
						//instead of using the most alpha pixel, can we get the blend formula?
							//note that for bars here, we have if(pixel_src[3]!=0) instead of the dst<src
					if(pixel_src[3]!=0)
						{
						pixel_dst[0]=pixel_src[0];
						pixel_dst[1]=pixel_src[1];
						pixel_dst[2]=pixel_src[2];
						pixel_dst[3]=pixel_src[3];
						}
					}
				}

			SDL_UnlockSurface(dsts);
			SDL_UnlockSurface(tex);
			}
		}}
		}

	if(gps.texpos_farmplot!=0)gps.texpos_planned_farmplot=create_alpha_texture(gps.texpos_farmplot,alpha_level);

	if(gps.texpos_lever_1!=0)gps.texpos_planned_lever=create_alpha_texture(gps.texpos_lever_1,alpha_level);
	if(gps.texpos_trap_cage!=0)gps.texpos_planned_trap_cage=create_alpha_texture(gps.texpos_trap_cage,alpha_level);
	if(gps.texpos_trap_stone!=0)gps.texpos_planned_trap_stone=create_alpha_texture(gps.texpos_trap_stone,alpha_level);
	if(gps.texpos_trap_weapon!=0)gps.texpos_planned_trap_weapon=create_alpha_texture(gps.texpos_trap_weapon,alpha_level);
	if(gps.texpos_trap_plate!=0)gps.texpos_planned_trap_plate=create_alpha_texture(gps.texpos_trap_plate,alpha_level);
	if(gps.texpos_upright_weapon_retracted[9]!=0)gps.texpos_planned_upright_weapon=create_alpha_texture(gps.texpos_upright_weapon_retracted[9],alpha_level);

	if(gps.texpos_gear_assembly[0]!=0)gps.texpos_planned_gear_assembly=create_alpha_texture(gps.texpos_gear_assembly[0],alpha_level);

	{int32_t sp;
	for(sp=0;sp<21;++sp)
		{
		bool n_up=false,n=false;
		bool s=false;
		bool w_up=false,w=false;
		bool e_up=false,e=false;
		bool has_floor=false;
		bool already_exists=false;

		int32_t *spp=NULL;
		switch(sp)
			{
			case 0:spp=&gps.stockpile_nw_up_texpos;n_up=true;w_up=true;break;
			case 1:spp=&gps.stockpile_nw_texpos;n=true;w=true;has_floor=true;break;
			case 2:spp=&gps.stockpile_ne_up_texpos;n_up=true;e_up=true;break;
			case 3:spp=&gps.stockpile_ne_texpos;n=true;e=true;has_floor=true;break;
			case 4:spp=&gps.stockpile_sw_texpos;s=true;w=true;has_floor=true;break;
			case 5:spp=&gps.stockpile_se_texpos;s=true;e=true;has_floor=true;break;
			case 6:spp=&gps.stockpile_ns_up_texpos;n_up=true;break;
			case 7:spp=&gps.stockpile_ns_texpos;n=true;s=true;has_floor=true;break;
			case 8:spp=&gps.stockpile_we_texpos;w=true;e=true;has_floor=true;break;
			case 9:spp=&gps.stockpile_nsw_up_texpos;n_up=true;w_up=true;break;
			case 10:spp=&gps.stockpile_nsw_texpos;n=true;s=true;w=true;has_floor=true;break;
			case 11:spp=&gps.stockpile_nse_up_texpos;n_up=true;e_up=true;break;
			case 12:spp=&gps.stockpile_nse_texpos;n=true;s=true;e=true;has_floor=true;break;
			case 13:spp=&gps.stockpile_nwe_up_texpos;n_up=true;w_up=true;e_up=true;break;
			case 14:spp=&gps.stockpile_nwe_texpos;n=true;w=true;e=true;has_floor=true;break;
			case 15:spp=&gps.stockpile_swe_texpos;s=true;w=true;e=true;has_floor=true;break;
			case 16:spp=&gps.stockpile_nswe_up_texpos;n_up=true;w_up=true;e_up=true;break;
			case 17:spp=&gps.stockpile_nswe_texpos;n=true;s=true;w=true;e=true;has_floor=true;break;
			case 18:spp=&gps.stockpile_n_texpos;has_floor=true;already_exists=true;break;
			case 19:spp=&gps.stockpile_w_texpos;has_floor=true;already_exists=true;break;
			case 20:spp=&gps.stockpile_e_texpos;has_floor=true;already_exists=true;break;
			}

		SDL_Surface *dst=NULL;
		int32_t i;
		for(i=0;i<8;++i)
			{
			bool *chkb;
			int32_t *src_pos;
			switch(i)
				{
				case 0:chkb=&s;src_pos=&stockpile_s_texpos;break;
				case 1:chkb=&w_up;src_pos=&stockpile_w_up_texpos;break;
				case 2:chkb=&w;src_pos=&stockpile_w_texpos;break;
				case 3:chkb=&e_up;src_pos=&stockpile_e_up_texpos;break;
				case 4:chkb=&e;src_pos=&stockpile_e_texpos;break;
				case 5:chkb=&n_up;src_pos=&stockpile_n_up_texpos;break;
				case 6:chkb=&n;src_pos=&stockpile_n_texpos;break;
				case 7:chkb=&has_floor;src_pos=&stockpile_floor_texpos;break;
				}
			if(*chkb)
				{
				SDL_Surface *src=enabler.textures.get_texture_data(*src_pos);
				if(src!=NULL)
					{
					if(dst==NULL)
						{
						if(!already_exists)*spp=enabler.textures.create_texture(src->w,src->h);
						dst=enabler.textures.get_texture_data(*spp);
						if(dst==NULL)break;
						SDL_LockSurface(dst);
						if(!already_exists)memset(dst->pixels,0,sizeof(Uint8)*(dst->h)*dst->pitch);
						}

					SDL_LockSurface(src);

					Uint8 *pixel_src,*pixel_dst;
					int x,y;
					for(y=0;y<src->h;y++)
						{
						pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
						pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
						for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
							{
							if(pixel_dst[3]==0&&pixel_src[3]!=0)
								{
								pixel_dst[0]=pixel_src[0];
								pixel_dst[1]=pixel_src[1];
								pixel_dst[2]=pixel_src[2];
								pixel_dst[3]=pixel_src[3];
								}
							}
						}

					SDL_UnlockSurface(src);
					}
				}
			}
		if(dst!=NULL)SDL_UnlockSurface(dst);
		}}
	{SDL_Surface *src=enabler.textures.get_texture_data(stockpile_ne_up_texpos);
	if(src!=NULL)
		{
		SDL_LockSurface(src);
		int32_t sp;
		for(sp=0;sp<19;++sp)
			{
			int32_t tp;
			switch(sp)
				{
				case 0:tp=stockpile_blank_texpos;break;
				case 1:tp=stockpile_ammo_texpos;break;
				case 2:tp=stockpile_animals_texpos;break;
				case 3:tp=stockpile_armor_texpos;break;
				case 4:tp=stockpile_bars_texpos;break;
				case 5:tp=stockpile_cloth_texpos;break;
				case 6:tp=stockpile_coins_texpos;break;
				case 7:tp=stockpile_corpses_texpos;break;
				case 8:tp=stockpile_finished_goods_texpos;break;
				case 9:tp=stockpile_food_texpos;break;
				case 10:tp=stockpile_furniture_texpos;break;
				case 11:tp=stockpile_gems_texpos;break;
				case 12:tp=stockpile_leather_texpos;break;
				case 13:tp=stockpile_refuse_texpos;break;
				case 14:tp=stockpile_sheets_texpos;break;
				case 15:tp=stockpile_stone_texpos;break;
				case 16:tp=stockpile_weapons_texpos;break;
				case 17:tp=stockpile_wood_texpos;break;
				case 18:tp=stockpile_custom_texpos;break;
				}
			SDL_Surface *dst=enabler.textures.get_texture_data(tp);
			if(dst==NULL)continue;

			SDL_LockSurface(dst);

			Uint8 *pixel_src,*pixel_dst;
			int x,y;
			for(y=0;y<src->h;y++)
				{
				pixel_src=((Uint8*)src->pixels)+(y*src->pitch);
				pixel_dst=((Uint8*)dst->pixels)+(y*dst->pitch);
				for(x=0;x<src->w;++x,pixel_src+=4,pixel_dst+=4)
					{
					if(pixel_dst[3]==0&&pixel_src[3]!=0)
						{
						pixel_dst[0]=pixel_src[0];
						pixel_dst[1]=pixel_src[1];
						pixel_dst[2]=pixel_src[2];
						pixel_dst[3]=pixel_src[3];
						}
					}
				}

			SDL_UnlockSurface(dst);
			}
		SDL_UnlockSurface(src);
		}}

	//debris -- uses EGA colors, based on 15 (which is white debris), two variants
	{int32_t d,v;
	for(v=0;v<2;++v)
		{
		int32_t *src_tx=&texpos_debris[15][v];
		if(*src_tx==0)continue;
		SDL_Surface *src=enabler.textures.get_texture_data(*src_tx);
		if(src==NULL)continue;

		SDL_LockSurface(src);

		for(d=0;d<15;++d)
			{
			gps.texpos_debris[d][v]=enabler.textures.create_texture(src->w,src->h);
			SDL_Surface *dst=enabler.textures.get_texture_data(gps.texpos_debris[d][v]);
			if(dst==NULL)continue;

			SDL_LockSurface(dst);
			memset(dst->pixels,0,sizeof(Uint8)*(dst->h)*dst->pitch);

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
						//*********************************** DEBRIS
						pixel_dst[0]=(Uint8)((float)pixel_src[0]*ccolor[d][0]);
						pixel_dst[1]=(Uint8)((float)pixel_src[1]*ccolor[d][1]);
						pixel_dst[2]=(Uint8)((float)pixel_src[2]*ccolor[d][2]);
						}
					}
				}

			SDL_UnlockSurface(dst);
			}

		SDL_UnlockSurface(src);
		}}

	//small world map corner cutups
	{int32_t i,var_num;
	for(i=0;i<57;++i)
		{
		if(i<25)var_num=REGION_SQUARE_MOUNTAIN_NUM;
		else var_num=REGION_SQUARE_FOREST_NUM;
		int32_t v,c;
		for(v=0;v<var_num;++v)
			{
			int32_t *src_tx=NULL;
			switch(i)
				{
				case 0:src_tx=&texpos_mountain_peak[v];break;
				case 1:src_tx=&texpos_mountain_peak_evil[v];break;
				case 2:src_tx=&texpos_mountain_peak_evilsav[v];break;
				case 3:src_tx=&texpos_mountain_peak_good[v];break;
				case 4:src_tx=&texpos_mountain_peak_goodsav[v];break;
				case 5:src_tx=&texpos_volcano[v];break;
				case 6:src_tx=&texpos_volcano_evil[v];break;
				case 7:src_tx=&texpos_volcano_evilsav[v];break;
				case 8:src_tx=&texpos_volcano_good[v];break;
				case 9:src_tx=&texpos_volcano_goodsav[v];break;
				case 10:src_tx=&texpos_mountain_high[v];break;
				case 11:src_tx=&texpos_mountain_high_evil[v];break;
				case 12:src_tx=&texpos_mountain_high_evilsav[v];break;
				case 13:src_tx=&texpos_mountain_high_good[v];break;
				case 14:src_tx=&texpos_mountain_high_goodsav[v];break;
				case 15:src_tx=&texpos_mountain_mid[v];break;
				case 16:src_tx=&texpos_mountain_mid_evil[v];break;
				case 17:src_tx=&texpos_mountain_mid_evilsav[v];break;
				case 18:src_tx=&texpos_mountain_mid_good[v];break;
				case 19:src_tx=&texpos_mountain_mid_goodsav[v];break;
				case 20:src_tx=&texpos_mountain_low[v];break;
				case 21:src_tx=&texpos_mountain_low_evil[v];break;
				case 22:src_tx=&texpos_mountain_low_evilsav[v];break;
				case 23:src_tx=&texpos_mountain_low_good[v];break;
				case 24:src_tx=&texpos_mountain_low_goodsav[v];break;
				case 25:src_tx=&texpos_forest_broadleaf_trop_moist[v];break;
				case 26:src_tx=&texpos_forest_broadleaf_trop_moist_evil[v];break;
				case 27:src_tx=&texpos_forest_broadleaf_trop_moist_evilsav[v];break;
				case 28:src_tx=&texpos_forest_broadleaf_trop_moist_good[v];break;
				case 29:src_tx=&texpos_forest_broadleaf_trop_moist_goodsav[v];break;
				case 30:src_tx=&texpos_forest_broadleaf_trop_dry[v];break;
				case 31:src_tx=&texpos_forest_broadleaf_trop_dry_evil[v];break;
				case 32:src_tx=&texpos_forest_broadleaf_trop_dry_evilsav[v];break;
				case 33:src_tx=&texpos_forest_broadleaf_trop_dry_good[v];break;
				case 34:src_tx=&texpos_forest_broadleaf_trop_dry_goodsav[v];break;
				case 35:src_tx=&texpos_forest_broadleaf_temp[v];break;
				case 36:src_tx=&texpos_forest_broadleaf_temp_evil[v];break;
				case 37:src_tx=&texpos_forest_broadleaf_temp_evilsav[v];break;
				case 38:src_tx=&texpos_forest_broadleaf_temp_good[v];break;
				case 39:src_tx=&texpos_forest_broadleaf_temp_goodsav[v];break;
				case 40:src_tx=&texpos_forest_conifer_trop[v];break;
				case 41:src_tx=&texpos_forest_conifer_trop_evil[v];break;
				case 42:src_tx=&texpos_forest_conifer_trop_evilsav[v];break;
				case 43:src_tx=&texpos_forest_conifer_trop_good[v];break;
				case 44:src_tx=&texpos_forest_conifer_trop_goodsav[v];break;
				case 45:src_tx=&texpos_forest_conifer_temp[v];break;
				case 46:src_tx=&texpos_forest_conifer_temp_evil[v];break;
				case 47:src_tx=&texpos_forest_conifer_temp_evilsav[v];break;
				case 48:src_tx=&texpos_forest_conifer_temp_good[v];break;
				case 49:src_tx=&texpos_forest_conifer_temp_goodsav[v];break;
				case 50:src_tx=&texpos_forest_taiga[v];break;
				case 51:src_tx=&texpos_forest_taiga_evil[v];break;
				case 52:src_tx=&texpos_forest_taiga_evilsav[v];break;
				case 53:src_tx=&texpos_forest_taiga_good[v];break;
				case 54:src_tx=&texpos_forest_taiga_goodsav[v];break;
				case 55:src_tx=&texpos_site_map_crops[v];break;
				case 56:src_tx=&texpos_site_map_orchard[v];break;
				}
			if(src_tx==NULL)continue;
			if(*src_tx==0)continue;
			SDL_Surface *src=enabler.textures.get_texture_data(*src_tx);
			if(src==NULL)continue;

			SDL_LockSurface(src);

			for(c=0;c<9;++c)
				{
				int32_t *dst_tx=NULL;
				switch(i)
					{
					case 0:dst_tx=&texpos_mountain_peak_c[v][c];break;
					case 1:dst_tx=&texpos_mountain_peak_evil_c[v][c];break;
					case 2:dst_tx=&texpos_mountain_peak_evilsav_c[v][c];break;
					case 3:dst_tx=&texpos_mountain_peak_good_c[v][c];break;
					case 4:dst_tx=&texpos_mountain_peak_goodsav_c[v][c];break;
					case 5:dst_tx=&texpos_volcano_c[v][c];break;
					case 6:dst_tx=&texpos_volcano_evil_c[v][c];break;
					case 7:dst_tx=&texpos_volcano_evilsav_c[v][c];break;
					case 8:dst_tx=&texpos_volcano_good_c[v][c];break;
					case 9:dst_tx=&texpos_volcano_goodsav_c[v][c];break;
					case 10:dst_tx=&texpos_mountain_high_c[v][c];break;
					case 11:dst_tx=&texpos_mountain_high_evil_c[v][c];break;
					case 12:dst_tx=&texpos_mountain_high_evilsav_c[v][c];break;
					case 13:dst_tx=&texpos_mountain_high_good_c[v][c];break;
					case 14:dst_tx=&texpos_mountain_high_goodsav_c[v][c];break;
					case 15:dst_tx=&texpos_mountain_mid_c[v][c];break;
					case 16:dst_tx=&texpos_mountain_mid_evil_c[v][c];break;
					case 17:dst_tx=&texpos_mountain_mid_evilsav_c[v][c];break;
					case 18:dst_tx=&texpos_mountain_mid_good_c[v][c];break;
					case 19:dst_tx=&texpos_mountain_mid_goodsav_c[v][c];break;
					case 20:dst_tx=&texpos_mountain_low_c[v][c];break;
					case 21:dst_tx=&texpos_mountain_low_evil_c[v][c];break;
					case 22:dst_tx=&texpos_mountain_low_evilsav_c[v][c];break;
					case 23:dst_tx=&texpos_mountain_low_good_c[v][c];break;
					case 24:dst_tx=&texpos_mountain_low_goodsav_c[v][c];break;
					case 25:dst_tx=&texpos_forest_broadleaf_trop_moist_c[v][c];break;
					case 26:dst_tx=&texpos_forest_broadleaf_trop_moist_evil_c[v][c];break;
					case 27:dst_tx=&texpos_forest_broadleaf_trop_moist_evilsav_c[v][c];break;
					case 28:dst_tx=&texpos_forest_broadleaf_trop_moist_good_c[v][c];break;
					case 29:dst_tx=&texpos_forest_broadleaf_trop_moist_goodsav_c[v][c];break;
					case 30:dst_tx=&texpos_forest_broadleaf_trop_dry_c[v][c];break;
					case 31:dst_tx=&texpos_forest_broadleaf_trop_dry_evil_c[v][c];break;
					case 32:dst_tx=&texpos_forest_broadleaf_trop_dry_evilsav_c[v][c];break;
					case 33:dst_tx=&texpos_forest_broadleaf_trop_dry_good_c[v][c];break;
					case 34:dst_tx=&texpos_forest_broadleaf_trop_dry_goodsav_c[v][c];break;
					case 35:dst_tx=&texpos_forest_broadleaf_temp_c[v][c];break;
					case 36:dst_tx=&texpos_forest_broadleaf_temp_evil_c[v][c];break;
					case 37:dst_tx=&texpos_forest_broadleaf_temp_evilsav_c[v][c];break;
					case 38:dst_tx=&texpos_forest_broadleaf_temp_good_c[v][c];break;
					case 39:dst_tx=&texpos_forest_broadleaf_temp_goodsav_c[v][c];break;
					case 40:dst_tx=&texpos_forest_conifer_trop_c[v][c];break;
					case 41:dst_tx=&texpos_forest_conifer_trop_evil_c[v][c];break;
					case 42:dst_tx=&texpos_forest_conifer_trop_evilsav_c[v][c];break;
					case 43:dst_tx=&texpos_forest_conifer_trop_good_c[v][c];break;
					case 44:dst_tx=&texpos_forest_conifer_trop_goodsav_c[v][c];break;
					case 45:dst_tx=&texpos_forest_conifer_temp_c[v][c];break;
					case 46:dst_tx=&texpos_forest_conifer_temp_evil_c[v][c];break;
					case 47:dst_tx=&texpos_forest_conifer_temp_evilsav_c[v][c];break;
					case 48:dst_tx=&texpos_forest_conifer_temp_good_c[v][c];break;
					case 49:dst_tx=&texpos_forest_conifer_temp_goodsav_c[v][c];break;
					case 50:dst_tx=&texpos_forest_taiga_c[v][c];break;
					case 51:dst_tx=&texpos_forest_taiga_evil_c[v][c];break;
					case 52:dst_tx=&texpos_forest_taiga_evilsav_c[v][c];break;
					case 53:dst_tx=&texpos_forest_taiga_good_c[v][c];break;
					case 54:dst_tx=&texpos_forest_taiga_goodsav_c[v][c];break;
					case 55:dst_tx=&texpos_site_map_crops_c[v][c];break;
					case 56:dst_tx=&texpos_site_map_orchard_c[v][c];break;
					}
				if(dst_tx==NULL)continue;
				*dst_tx=enabler.textures.create_texture(src->w/2,src->h/2);
				SDL_Surface *dst;
				dst=enabler.textures.get_texture_data(*dst_tx);
				if(dst==NULL)continue;

				SDL_LockSurface(dst);

				memset(dst->pixels,0,sizeof(Uint8)*(dst->h)*dst->pitch);

				int32_t sx=0,sy=0,ex=31,ey=31;
				int32_t dst_off_x=0,dst_off_y=0;
				switch(c)
					{
					case 8:
						sx=24;sy=24;ex=31;ey=31;
						dst_off_x=24;dst_off_y=24;
						break;
					case 7:
						sx=8;sy=24;ex=23;ey=31;
						dst_off_x=8;dst_off_y=24;
						break;
					case 6:
						sx=0;sy=24;ex=7;ey=31;
						dst_off_x=-8;dst_off_y=24;
						break;
					case 5:
						sx=24;sy=8;ex=31;ey=23;
						dst_off_x=24;dst_off_y=8;
						break;
					case 4:
						sx=8;sy=8;ex=23;ey=23;
						dst_off_x=8;dst_off_y=8;
						break;
					case 3:
						sx=0;sy=8;ex=7;ey=23;
						dst_off_x=-8;dst_off_y=8;
						break;
					case 2:
						sx=24;sy=0;ex=31;ey=7;
						dst_off_x=24;dst_off_y=-8;
						break;
					case 1:
						sx=8;sy=0;ex=23;ey=7;
						dst_off_x=8;dst_off_y=-8;
						break;
					case 0:
						sx=0;sy=0;ex=7;ey=7;
						dst_off_x=-8;dst_off_y=-8;
						break;
					}

				Uint8 *pixel_src;
				int32_t x,y;
				for(y=sy;y<src->h&&y<=ey;y++)
					{
					pixel_src=((Uint8*)src->pixels)+sx*4+(y*src->pitch);
					for(x=sx;x<=ex;++x,pixel_src+=4)
						{
						if(pixel_src[3]!=0)
							{
							if(x>=sx&&x<=ex&&y>=sy&&y<=ey)
								{
								((Uint8*)dst->pixels)[(x-dst_off_x)*4+(y-dst_off_y)*dst->pitch]=pixel_src[0];
								((Uint8*)dst->pixels)[(x-dst_off_x)*4+(y-dst_off_y)*dst->pitch+1]=pixel_src[1];
								((Uint8*)dst->pixels)[(x-dst_off_x)*4+(y-dst_off_y)*dst->pitch+2]=pixel_src[2];
								((Uint8*)dst->pixels)[(x-dst_off_x)*4+(y-dst_off_y)*dst->pitch+3]=pixel_src[3];
								}
							}
						}
					}

				SDL_UnlockSurface(dst);
				}

			SDL_UnlockSurface(src);
			}
		}}

	int32_t siv,bm,sv;
	for(sv=0;sv<REGION_SQUARE_VARIANT_NUM;++sv)
		{
		for(bm=0;bm<35;++bm)
			{
			int32_t *tp;
			switch(bm)
				{
				case 0:tp=&texpos_grassland_temp[0];break;
				case 1:tp=&texpos_grassland_temp_evil[0];break;
				case 2:tp=&texpos_grassland_temp_good[0];break;
				case 3:tp=&texpos_grassland_trop[0];break;
				case 4:tp=&texpos_grassland_trop_evil[0];break;
				case 5:tp=&texpos_grassland_trop_good[0];break;
				case 6:tp=&texpos_marsh[0];break;
				case 7:tp=&texpos_rocky_plains[0];break;
				case 8:tp=&texpos_rocky_plains_evil[0];break;
				case 9:tp=&texpos_rocky_plains_good[0];break;
				case 10:tp=&texpos_tundra[0];break;
				case 11:tp=&texpos_tundra_evil[0];break;
				case 12:tp=&texpos_tundra_good[0];break;
				case 13:tp=&texpos_glacier[0];break;
				case 14:tp=&texpos_glacier_evil[0];break;
				case 15:tp=&texpos_glacier_good[0];break;
				case 16:tp=&texpos_ocean[0];break;
				case 17:tp=&texpos_ocean_evil[0];break;
				case 18:tp=&texpos_ocean_good[0];break;
				case 19:tp=&texpos_sand_desert[0];break;
				case 20:tp=&texpos_sand_desert_yellow[0];break;
				case 21:tp=&texpos_sand_desert_white[0];break;
				case 22:tp=&texpos_sand_desert_black[0];break;
				case 23:tp=&texpos_sand_desert_red[0];break;
				case 24:tp=&texpos_sand_desert_evil[0];break;
				case 25:tp=&texpos_sand_desert_good[0];break;
				case 26:tp=&texpos_marsh_evil[0];break;
				case 27:tp=&texpos_marsh_good[0];break;
				case 28:tp=&texpos_beach[0];break;
				case 29:tp=&texpos_beach_evil[0];break;
				case 30:tp=&texpos_beach_good[0];break;
				case 31:tp=&texpos_glacier_evilsav[0];break;
				case 32:tp=&texpos_glacier_goodsav[0];break;
				case 33:tp=&texpos_site_muddy[0];break;
				case 34:tp=&texpos_site_grassy[0];break;
				}
			if(*tp==0)continue;

			for(siv=0;siv<37;++siv)
				{
				int32_t *alpha_tp;
				switch(siv)
					{
					case 0:alpha_tp=&texpos_world_edge_shape_straight_n[sv];break;
					case 1:alpha_tp=&texpos_world_edge_shape_straight_s[sv];break;
					case 2:alpha_tp=&texpos_world_edge_shape_straight_w[sv];break;
					case 3:alpha_tp=&texpos_world_edge_shape_straight_e[sv];break;
					case 4:alpha_tp=&texpos_world_edge_shape_thick_corner_nw[sv];break;
					case 5:alpha_tp=&texpos_world_edge_shape_thick_corner_ne[sv];break;
					case 6:alpha_tp=&texpos_world_edge_shape_thick_corner_sw[sv];break;
					case 7:alpha_tp=&texpos_world_edge_shape_thick_corner_se[sv];break;
					case 8:alpha_tp=&texpos_world_edge_shape_thin_corner_nw[sv];break;
					case 9:alpha_tp=&texpos_world_edge_shape_thin_corner_ne[sv];break;
					case 10:alpha_tp=&texpos_world_edge_shape_thin_corner_sw[sv];break;
					case 11:alpha_tp=&texpos_world_edge_shape_thin_corner_se[sv];break;
					case 12:alpha_tp=&texpos_world_edge_shape_tiny_corner_nw[sv];break;
					case 13:alpha_tp=&texpos_world_edge_shape_tiny_corner_ne[sv];break;
					case 14:alpha_tp=&texpos_world_edge_shape_tiny_corner_sw[sv];break;
					case 15:alpha_tp=&texpos_world_edge_shape_tiny_corner_se[sv];break;
					case 16:alpha_tp=&texpos_world_edge_shape_inlet_nsw[sv];break;
					case 17:alpha_tp=&texpos_world_edge_shape_inlet_nse[sv];break;
					case 18:alpha_tp=&texpos_world_edge_shape_inlet_nwe[sv];break;
					case 19:alpha_tp=&texpos_world_edge_shape_inlet_swe[sv];break;
					case 20:alpha_tp=&texpos_world_edge_shape_surround[sv];break;
					case 21:alpha_tp=&texpos_world_edge_shape_thinning_n_w[sv];break;
					case 22:alpha_tp=&texpos_world_edge_shape_thinning_n_e[sv];break;
					case 23:alpha_tp=&texpos_world_edge_shape_thinning_s_w[sv];break;
					case 24:alpha_tp=&texpos_world_edge_shape_thinning_s_e[sv];break;
					case 25:alpha_tp=&texpos_world_edge_shape_thinning_w_n[sv];break;
					case 26:alpha_tp=&texpos_world_edge_shape_thinning_w_s[sv];break;
					case 27:alpha_tp=&texpos_world_edge_shape_thinning_e_n[sv];break;
					case 28:alpha_tp=&texpos_world_edge_shape_thinning_e_s[sv];break;
					case 29:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_nw_n[sv];break;
					case 30:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_nw_w[sv];break;
					case 31:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_ne_n[sv];break;
					case 32:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_ne_e[sv];break;
					case 33:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_sw_s[sv];break;
					case 34:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_sw_w[sv];break;
					case 35:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_se_s[sv];break;
					case 36:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_se_e[sv];break;
					}
				if(*alpha_tp==0)
					{
					switch(siv)
						{
						case 0:alpha_tp=&texpos_world_edge_shape_straight_n[0];break;
						case 1:alpha_tp=&texpos_world_edge_shape_straight_s[0];break;
						case 2:alpha_tp=&texpos_world_edge_shape_straight_w[0];break;
						case 3:alpha_tp=&texpos_world_edge_shape_straight_e[0];break;
						case 4:alpha_tp=&texpos_world_edge_shape_thick_corner_nw[0];break;
						case 5:alpha_tp=&texpos_world_edge_shape_thick_corner_ne[0];break;
						case 6:alpha_tp=&texpos_world_edge_shape_thick_corner_sw[0];break;
						case 7:alpha_tp=&texpos_world_edge_shape_thick_corner_se[0];break;
						case 8:alpha_tp=&texpos_world_edge_shape_thin_corner_nw[0];break;
						case 9:alpha_tp=&texpos_world_edge_shape_thin_corner_ne[0];break;
						case 10:alpha_tp=&texpos_world_edge_shape_thin_corner_sw[0];break;
						case 11:alpha_tp=&texpos_world_edge_shape_thin_corner_se[0];break;
						case 12:alpha_tp=&texpos_world_edge_shape_tiny_corner_nw[0];break;
						case 13:alpha_tp=&texpos_world_edge_shape_tiny_corner_ne[0];break;
						case 14:alpha_tp=&texpos_world_edge_shape_tiny_corner_sw[0];break;
						case 15:alpha_tp=&texpos_world_edge_shape_tiny_corner_se[0];break;
						case 16:alpha_tp=&texpos_world_edge_shape_inlet_nsw[0];break;
						case 17:alpha_tp=&texpos_world_edge_shape_inlet_nse[0];break;
						case 18:alpha_tp=&texpos_world_edge_shape_inlet_nwe[0];break;
						case 19:alpha_tp=&texpos_world_edge_shape_inlet_swe[0];break;
						case 20:alpha_tp=&texpos_world_edge_shape_surround[0];break;
						case 21:alpha_tp=&texpos_world_edge_shape_thinning_n_w[0];break;
						case 22:alpha_tp=&texpos_world_edge_shape_thinning_n_e[0];break;
						case 23:alpha_tp=&texpos_world_edge_shape_thinning_s_w[0];break;
						case 24:alpha_tp=&texpos_world_edge_shape_thinning_s_e[0];break;
						case 25:alpha_tp=&texpos_world_edge_shape_thinning_w_n[0];break;
						case 26:alpha_tp=&texpos_world_edge_shape_thinning_w_s[0];break;
						case 27:alpha_tp=&texpos_world_edge_shape_thinning_e_n[0];break;
						case 28:alpha_tp=&texpos_world_edge_shape_thinning_e_s[0];break;
						case 29:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_nw_n[0];break;
						case 30:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_nw_w[0];break;
						case 31:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_ne_n[0];break;
						case 32:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_ne_e[0];break;
						case 33:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_sw_s[0];break;
						case 34:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_sw_w[0];break;
						case 35:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_se_s[0];break;
						case 36:alpha_tp=&texpos_world_edge_shape_thin_corner_transition_se_e[0];break;
						}
					if(*alpha_tp==0)continue;
					}

				int32_t *tpp;
				switch(bm)
					{
					case 0:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_temp_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_temp_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_temp_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_temp_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_temp_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_temp_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_temp_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_temp_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_temp_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_temp_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_temp_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_temp_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_temp_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_temp_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_temp_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_temp_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_temp_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_temp_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_temp_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_temp_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_temp_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_temp_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_temp_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_temp_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_temp_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_temp_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_temp_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_temp_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_temp_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_temp_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_temp_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_temp_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_temp_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_temp_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_temp_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_temp_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_temp_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 1:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_temp_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_temp_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_temp_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_temp_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_temp_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_temp_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_temp_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_temp_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_temp_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_temp_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_temp_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_temp_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_temp_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_temp_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_temp_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_temp_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_temp_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_temp_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_temp_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_temp_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_temp_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_temp_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_temp_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_temp_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_temp_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_temp_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_temp_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_temp_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_temp_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_temp_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 2:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_temp_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_temp_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_temp_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_temp_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_temp_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_temp_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_temp_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_temp_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_temp_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_temp_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_temp_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_temp_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_temp_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_temp_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_temp_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_temp_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_temp_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_temp_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_temp_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_temp_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_temp_good_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_temp_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_temp_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_temp_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_temp_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_temp_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_temp_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_temp_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_temp_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_temp_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 3:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_trop_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_trop_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_trop_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_trop_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_trop_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_trop_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_trop_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_trop_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_trop_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_trop_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_trop_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_trop_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_trop_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_trop_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_trop_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_trop_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_trop_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_trop_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_trop_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_trop_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_trop_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_trop_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_trop_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_trop_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_trop_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_trop_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_trop_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_trop_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_trop_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_trop_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_trop_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_trop_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_trop_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_trop_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_trop_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_trop_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_trop_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 4:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_trop_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_trop_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_trop_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_trop_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_trop_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_trop_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_trop_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_trop_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_trop_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_trop_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_trop_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_trop_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_trop_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_trop_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_trop_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_trop_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_trop_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_trop_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_trop_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_trop_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_trop_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_trop_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_trop_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_trop_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_trop_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_trop_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_trop_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_trop_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_trop_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_trop_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 5:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_grass_trop_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_grass_trop_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_grass_trop_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_grass_trop_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_grass_trop_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_grass_trop_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_grass_trop_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_grass_trop_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_grass_trop_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_grass_trop_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_grass_trop_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_grass_trop_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_grass_trop_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_grass_trop_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_grass_trop_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_grass_trop_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_grass_trop_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_grass_trop_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_grass_trop_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_grass_trop_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_grass_trop_good_surround[sv];break;
							case 21:tpp=&texpos_edge_grass_trop_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_grass_trop_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_grass_trop_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_grass_trop_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_grass_trop_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_grass_trop_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_grass_trop_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_grass_trop_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_grass_trop_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 6:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_wetland_straight_n[sv];break;
							case 1:tpp=&texpos_edge_wetland_straight_s[sv];break;
							case 2:tpp=&texpos_edge_wetland_straight_w[sv];break;
							case 3:tpp=&texpos_edge_wetland_straight_e[sv];break;
							case 4:tpp=&texpos_edge_wetland_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_wetland_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_wetland_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_wetland_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_wetland_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_wetland_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_wetland_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_wetland_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_wetland_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_wetland_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_wetland_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_wetland_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_wetland_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_wetland_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_wetland_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_wetland_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_wetland_surround[sv];break;
							case 21:tpp=&texpos_edge_wetland_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_wetland_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_wetland_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_wetland_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_wetland_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_wetland_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_wetland_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_wetland_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_wetland_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_wetland_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_wetland_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_wetland_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_wetland_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_wetland_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_wetland_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_wetland_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 7:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_desert_straight_n[sv];break;
							case 1:tpp=&texpos_edge_desert_straight_s[sv];break;
							case 2:tpp=&texpos_edge_desert_straight_w[sv];break;
							case 3:tpp=&texpos_edge_desert_straight_e[sv];break;
							case 4:tpp=&texpos_edge_desert_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_desert_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_desert_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_desert_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_desert_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_desert_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_desert_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_desert_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_desert_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_desert_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_desert_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_desert_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_desert_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_desert_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_desert_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_desert_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_desert_surround[sv];break;
							case 21:tpp=&texpos_edge_desert_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_desert_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_desert_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_desert_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_desert_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_desert_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_desert_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_desert_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_desert_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_desert_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_desert_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_desert_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_desert_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_desert_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_desert_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_desert_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 8:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_desert_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_desert_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_desert_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_desert_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_desert_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_desert_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_desert_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_desert_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_desert_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_desert_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_desert_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_desert_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_desert_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_desert_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_desert_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_desert_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_desert_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_desert_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_desert_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_desert_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_desert_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_desert_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_desert_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_desert_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_desert_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_desert_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_desert_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_desert_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_desert_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_desert_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_desert_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_desert_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_desert_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_desert_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_desert_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_desert_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_desert_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 9:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_desert_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_desert_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_desert_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_desert_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_desert_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_desert_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_desert_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_desert_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_desert_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_desert_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_desert_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_desert_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_desert_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_desert_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_desert_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_desert_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_desert_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_desert_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_desert_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_desert_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_desert_good_surround[sv];break;
							case 21:tpp=&texpos_edge_desert_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_desert_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_desert_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_desert_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_desert_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_desert_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_desert_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_desert_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_desert_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_desert_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_desert_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_desert_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_desert_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_desert_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_desert_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_desert_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 10:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_tundra_straight_n[sv];break;
							case 1:tpp=&texpos_edge_tundra_straight_s[sv];break;
							case 2:tpp=&texpos_edge_tundra_straight_w[sv];break;
							case 3:tpp=&texpos_edge_tundra_straight_e[sv];break;
							case 4:tpp=&texpos_edge_tundra_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_tundra_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_tundra_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_tundra_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_tundra_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_tundra_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_tundra_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_tundra_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_tundra_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_tundra_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_tundra_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_tundra_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_tundra_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_tundra_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_tundra_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_tundra_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_tundra_surround[sv];break;
							case 21:tpp=&texpos_edge_tundra_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_tundra_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_tundra_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_tundra_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_tundra_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_tundra_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_tundra_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_tundra_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_tundra_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_tundra_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_tundra_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_tundra_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_tundra_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_tundra_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_tundra_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_tundra_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 11:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_tundra_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_tundra_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_tundra_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_tundra_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_tundra_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_tundra_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_tundra_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_tundra_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_tundra_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_tundra_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_tundra_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_tundra_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_tundra_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_tundra_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_tundra_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_tundra_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_tundra_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_tundra_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_tundra_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_tundra_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_tundra_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_tundra_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_tundra_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_tundra_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_tundra_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_tundra_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_tundra_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_tundra_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_tundra_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_tundra_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_tundra_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_tundra_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_tundra_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_tundra_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_tundra_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_tundra_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_tundra_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 12:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_tundra_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_tundra_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_tundra_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_tundra_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_tundra_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_tundra_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_tundra_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_tundra_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_tundra_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_tundra_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_tundra_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_tundra_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_tundra_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_tundra_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_tundra_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_tundra_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_tundra_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_tundra_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_tundra_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_tundra_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_tundra_good_surround[sv];break;
							case 21:tpp=&texpos_edge_tundra_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_tundra_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_tundra_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_tundra_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_tundra_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_tundra_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_tundra_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_tundra_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_tundra_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_tundra_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_tundra_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_tundra_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_tundra_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_tundra_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_tundra_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_tundra_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 13:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_glacier_straight_n[sv];break;
							case 1:tpp=&texpos_edge_glacier_straight_s[sv];break;
							case 2:tpp=&texpos_edge_glacier_straight_w[sv];break;
							case 3:tpp=&texpos_edge_glacier_straight_e[sv];break;
							case 4:tpp=&texpos_edge_glacier_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_glacier_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_glacier_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_glacier_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_glacier_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_glacier_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_glacier_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_glacier_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_glacier_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_glacier_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_glacier_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_glacier_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_glacier_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_glacier_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_glacier_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_glacier_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_glacier_surround[sv];break;
							case 21:tpp=&texpos_edge_glacier_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_glacier_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_glacier_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_glacier_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_glacier_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_glacier_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_glacier_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_glacier_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_glacier_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_glacier_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_glacier_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_glacier_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_glacier_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_glacier_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_glacier_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_glacier_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 14:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_glacier_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_glacier_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_glacier_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_glacier_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_glacier_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_glacier_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_glacier_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_glacier_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_glacier_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_glacier_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_glacier_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_glacier_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_glacier_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_glacier_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_glacier_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_glacier_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_glacier_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_glacier_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_glacier_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_glacier_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_glacier_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_glacier_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_glacier_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_glacier_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_glacier_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_glacier_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_glacier_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_glacier_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_glacier_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_glacier_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_glacier_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_glacier_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_glacier_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_glacier_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_glacier_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_glacier_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_glacier_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 15:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_glacier_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_glacier_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_glacier_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_glacier_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_glacier_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_glacier_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_glacier_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_glacier_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_glacier_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_glacier_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_glacier_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_glacier_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_glacier_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_glacier_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_glacier_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_glacier_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_glacier_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_glacier_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_glacier_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_glacier_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_glacier_good_surround[sv];break;
							case 21:tpp=&texpos_edge_glacier_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_glacier_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_glacier_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_glacier_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_glacier_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_glacier_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_glacier_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_glacier_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_glacier_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_glacier_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_glacier_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_glacier_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_glacier_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_glacier_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_glacier_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_glacier_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 16:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_water_straight_n[sv];break;
							case 1:tpp=&texpos_edge_water_straight_s[sv];break;
							case 2:tpp=&texpos_edge_water_straight_w[sv];break;
							case 3:tpp=&texpos_edge_water_straight_e[sv];break;
							case 4:tpp=&texpos_edge_water_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_water_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_water_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_water_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_water_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_water_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_water_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_water_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_water_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_water_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_water_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_water_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_water_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_water_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_water_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_water_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_water_surround[sv];break;
							case 21:tpp=&texpos_edge_water_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_water_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_water_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_water_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_water_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_water_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_water_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_water_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_water_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_water_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_water_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_water_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_water_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_water_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_water_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_water_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 17:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_water_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_water_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_water_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_water_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_water_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_water_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_water_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_water_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_water_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_water_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_water_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_water_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_water_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_water_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_water_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_water_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_water_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_water_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_water_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_water_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_water_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_water_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_water_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_water_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_water_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_water_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_water_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_water_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_water_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_water_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_water_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_water_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_water_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_water_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_water_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_water_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_water_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 18:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_water_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_water_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_water_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_water_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_water_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_water_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_water_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_water_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_water_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_water_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_water_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_water_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_water_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_water_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_water_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_water_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_water_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_water_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_water_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_water_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_water_good_surround[sv];break;
							case 21:tpp=&texpos_edge_water_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_water_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_water_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_water_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_water_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_water_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_water_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_water_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_water_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_water_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_water_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_water_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_water_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_water_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_water_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_water_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 19:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 20:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_yellow_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_yellow_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_yellow_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_yellow_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_yellow_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_yellow_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_yellow_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_yellow_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_yellow_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_yellow_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_yellow_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_yellow_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_yellow_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_yellow_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_yellow_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_yellow_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_yellow_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_yellow_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_yellow_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_yellow_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_yellow_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_yellow_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_yellow_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_yellow_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_yellow_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_yellow_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_yellow_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_yellow_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_yellow_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_yellow_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 21:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_white_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_white_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_white_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_white_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_white_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_white_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_white_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_white_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_white_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_white_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_white_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_white_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_white_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_white_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_white_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_white_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_white_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_white_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_white_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_white_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_white_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_white_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_white_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_white_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_white_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_white_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_white_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_white_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_white_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_white_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 22:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_black_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_black_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_black_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_black_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_black_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_black_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_black_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_black_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_black_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_black_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_black_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_black_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_black_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_black_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_black_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_black_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_black_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_black_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_black_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_black_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_black_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_black_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_black_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_black_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_black_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_black_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_black_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_black_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_black_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_black_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 23:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_red_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_red_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_red_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_red_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_red_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_red_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_red_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_red_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_red_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_red_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_red_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_red_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_red_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_red_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_red_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_red_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_red_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_red_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_red_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_red_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_red_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_red_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_red_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_red_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_red_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_red_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_red_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_red_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_red_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_red_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 24:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 25:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_sand_desert_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_sand_desert_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_sand_desert_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_sand_desert_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_sand_desert_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_sand_desert_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_sand_desert_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_sand_desert_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_sand_desert_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_sand_desert_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_sand_desert_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_sand_desert_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_sand_desert_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_sand_desert_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_sand_desert_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_sand_desert_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_sand_desert_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_sand_desert_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_sand_desert_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_sand_desert_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_sand_desert_good_surround[sv];break;
							case 21:tpp=&texpos_edge_sand_desert_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_sand_desert_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_sand_desert_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_sand_desert_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_sand_desert_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_sand_desert_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_sand_desert_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_sand_desert_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_sand_desert_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 26:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_wetland_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_wetland_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_wetland_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_wetland_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_wetland_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_wetland_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_wetland_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_wetland_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_wetland_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_wetland_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_wetland_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_wetland_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_wetland_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_wetland_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_wetland_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_wetland_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_wetland_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_wetland_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_wetland_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_wetland_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_wetland_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_wetland_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_wetland_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_wetland_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_wetland_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_wetland_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_wetland_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_wetland_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_wetland_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_wetland_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_wetland_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_wetland_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_wetland_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_wetland_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_wetland_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_wetland_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_wetland_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 27:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_wetland_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_wetland_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_wetland_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_wetland_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_wetland_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_wetland_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_wetland_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_wetland_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_wetland_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_wetland_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_wetland_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_wetland_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_wetland_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_wetland_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_wetland_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_wetland_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_wetland_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_wetland_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_wetland_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_wetland_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_wetland_good_surround[sv];break;
							case 21:tpp=&texpos_edge_wetland_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_wetland_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_wetland_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_wetland_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_wetland_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_wetland_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_wetland_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_wetland_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_wetland_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_wetland_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_wetland_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_wetland_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_wetland_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_wetland_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_wetland_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_wetland_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 28:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_beach_straight_n[sv];break;
							case 1:tpp=&texpos_edge_beach_straight_s[sv];break;
							case 2:tpp=&texpos_edge_beach_straight_w[sv];break;
							case 3:tpp=&texpos_edge_beach_straight_e[sv];break;
							case 4:tpp=&texpos_edge_beach_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_beach_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_beach_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_beach_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_beach_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_beach_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_beach_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_beach_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_beach_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_beach_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_beach_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_beach_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_beach_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_beach_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_beach_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_beach_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_beach_surround[sv];break;
							case 21:tpp=&texpos_edge_beach_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_beach_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_beach_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_beach_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_beach_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_beach_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_beach_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_beach_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_beach_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_beach_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_beach_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_beach_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_beach_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_beach_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_beach_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_beach_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 29:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_beach_evil_straight_n[sv];break;
							case 1:tpp=&texpos_edge_beach_evil_straight_s[sv];break;
							case 2:tpp=&texpos_edge_beach_evil_straight_w[sv];break;
							case 3:tpp=&texpos_edge_beach_evil_straight_e[sv];break;
							case 4:tpp=&texpos_edge_beach_evil_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_beach_evil_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_beach_evil_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_beach_evil_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_beach_evil_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_beach_evil_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_beach_evil_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_beach_evil_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_beach_evil_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_beach_evil_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_beach_evil_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_beach_evil_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_beach_evil_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_beach_evil_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_beach_evil_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_beach_evil_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_beach_evil_surround[sv];break;
							case 21:tpp=&texpos_edge_beach_evil_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_beach_evil_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_beach_evil_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_beach_evil_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_beach_evil_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_beach_evil_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_beach_evil_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_beach_evil_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_beach_evil_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_beach_evil_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_beach_evil_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_beach_evil_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_beach_evil_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_beach_evil_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_beach_evil_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_beach_evil_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 30:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_beach_good_straight_n[sv];break;
							case 1:tpp=&texpos_edge_beach_good_straight_s[sv];break;
							case 2:tpp=&texpos_edge_beach_good_straight_w[sv];break;
							case 3:tpp=&texpos_edge_beach_good_straight_e[sv];break;
							case 4:tpp=&texpos_edge_beach_good_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_beach_good_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_beach_good_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_beach_good_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_beach_good_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_beach_good_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_beach_good_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_beach_good_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_beach_good_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_beach_good_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_beach_good_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_beach_good_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_beach_good_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_beach_good_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_beach_good_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_beach_good_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_beach_good_surround[sv];break;
							case 21:tpp=&texpos_edge_beach_good_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_beach_good_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_beach_good_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_beach_good_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_beach_good_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_beach_good_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_beach_good_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_beach_good_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_beach_good_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_beach_good_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_beach_good_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_beach_good_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_beach_good_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_beach_good_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_beach_good_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_beach_good_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 31:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_glacier_evilsav_straight_n[sv];break;
							case 1:tpp=&texpos_edge_glacier_evilsav_straight_s[sv];break;
							case 2:tpp=&texpos_edge_glacier_evilsav_straight_w[sv];break;
							case 3:tpp=&texpos_edge_glacier_evilsav_straight_e[sv];break;
							case 4:tpp=&texpos_edge_glacier_evilsav_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_glacier_evilsav_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_glacier_evilsav_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_glacier_evilsav_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_glacier_evilsav_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_glacier_evilsav_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_glacier_evilsav_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_glacier_evilsav_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_glacier_evilsav_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_glacier_evilsav_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_glacier_evilsav_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_glacier_evilsav_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_glacier_evilsav_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_glacier_evilsav_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_glacier_evilsav_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_glacier_evilsav_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_glacier_evilsav_surround[sv];break;
							case 21:tpp=&texpos_edge_glacier_evilsav_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_glacier_evilsav_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_glacier_evilsav_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_glacier_evilsav_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_glacier_evilsav_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_glacier_evilsav_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_glacier_evilsav_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_glacier_evilsav_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_glacier_evilsav_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 32:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_glacier_goodsav_straight_n[sv];break;
							case 1:tpp=&texpos_edge_glacier_goodsav_straight_s[sv];break;
							case 2:tpp=&texpos_edge_glacier_goodsav_straight_w[sv];break;
							case 3:tpp=&texpos_edge_glacier_goodsav_straight_e[sv];break;
							case 4:tpp=&texpos_edge_glacier_goodsav_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_glacier_goodsav_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_glacier_goodsav_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_glacier_goodsav_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_glacier_goodsav_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_glacier_goodsav_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_glacier_goodsav_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_glacier_goodsav_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_glacier_goodsav_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_glacier_goodsav_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_glacier_goodsav_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_glacier_goodsav_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_glacier_goodsav_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_glacier_goodsav_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_glacier_goodsav_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_glacier_goodsav_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_glacier_goodsav_surround[sv];break;
							case 21:tpp=&texpos_edge_glacier_goodsav_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_glacier_goodsav_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_glacier_goodsav_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_glacier_goodsav_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_glacier_goodsav_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_glacier_goodsav_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_glacier_goodsav_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_glacier_goodsav_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_glacier_goodsav_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 33:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_site_muddy_straight_n[sv];break;
							case 1:tpp=&texpos_edge_site_muddy_straight_s[sv];break;
							case 2:tpp=&texpos_edge_site_muddy_straight_w[sv];break;
							case 3:tpp=&texpos_edge_site_muddy_straight_e[sv];break;
							case 4:tpp=&texpos_edge_site_muddy_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_site_muddy_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_site_muddy_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_site_muddy_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_site_muddy_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_site_muddy_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_site_muddy_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_site_muddy_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_site_muddy_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_site_muddy_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_site_muddy_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_site_muddy_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_site_muddy_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_site_muddy_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_site_muddy_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_site_muddy_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_site_muddy_surround[sv];break;
							case 21:tpp=&texpos_edge_site_muddy_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_site_muddy_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_site_muddy_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_site_muddy_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_site_muddy_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_site_muddy_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_site_muddy_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_site_muddy_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_site_muddy_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_site_muddy_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_site_muddy_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_site_muddy_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_site_muddy_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_site_muddy_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_site_muddy_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_site_muddy_thin_corner_transition_se_e[sv];break;
							}
						break;
					case 34:
						switch(siv)
							{
							case 0:tpp=&texpos_edge_site_grassy_straight_n[sv];break;
							case 1:tpp=&texpos_edge_site_grassy_straight_s[sv];break;
							case 2:tpp=&texpos_edge_site_grassy_straight_w[sv];break;
							case 3:tpp=&texpos_edge_site_grassy_straight_e[sv];break;
							case 4:tpp=&texpos_edge_site_grassy_thick_corner_nw[sv];break;
							case 5:tpp=&texpos_edge_site_grassy_thick_corner_ne[sv];break;
							case 6:tpp=&texpos_edge_site_grassy_thick_corner_sw[sv];break;
							case 7:tpp=&texpos_edge_site_grassy_thick_corner_se[sv];break;
							case 8:tpp=&texpos_edge_site_grassy_thin_corner_nw[sv];break;
							case 9:tpp=&texpos_edge_site_grassy_thin_corner_ne[sv];break;
							case 10:tpp=&texpos_edge_site_grassy_thin_corner_sw[sv];break;
							case 11:tpp=&texpos_edge_site_grassy_thin_corner_se[sv];break;
							case 12:tpp=&texpos_edge_site_grassy_tiny_corner_nw[sv];break;
							case 13:tpp=&texpos_edge_site_grassy_tiny_corner_ne[sv];break;
							case 14:tpp=&texpos_edge_site_grassy_tiny_corner_sw[sv];break;
							case 15:tpp=&texpos_edge_site_grassy_tiny_corner_se[sv];break;
							case 16:tpp=&texpos_edge_site_grassy_inlet_nsw[sv];break;
							case 17:tpp=&texpos_edge_site_grassy_inlet_nse[sv];break;
							case 18:tpp=&texpos_edge_site_grassy_inlet_nwe[sv];break;
							case 19:tpp=&texpos_edge_site_grassy_inlet_swe[sv];break;
							case 20:tpp=&texpos_edge_site_grassy_surround[sv];break;
							case 21:tpp=&texpos_edge_site_grassy_thinning_n_w[sv];break;
							case 22:tpp=&texpos_edge_site_grassy_thinning_n_e[sv];break;
							case 23:tpp=&texpos_edge_site_grassy_thinning_s_w[sv];break;
							case 24:tpp=&texpos_edge_site_grassy_thinning_s_e[sv];break;
							case 25:tpp=&texpos_edge_site_grassy_thinning_w_n[sv];break;
							case 26:tpp=&texpos_edge_site_grassy_thinning_w_s[sv];break;
							case 27:tpp=&texpos_edge_site_grassy_thinning_e_n[sv];break;
							case 28:tpp=&texpos_edge_site_grassy_thinning_e_s[sv];break;
							case 29:tpp=&texpos_edge_site_grassy_thin_corner_transition_nw_n[sv];break;
							case 30:tpp=&texpos_edge_site_grassy_thin_corner_transition_nw_w[sv];break;
							case 31:tpp=&texpos_edge_site_grassy_thin_corner_transition_ne_n[sv];break;
							case 32:tpp=&texpos_edge_site_grassy_thin_corner_transition_ne_e[sv];break;
							case 33:tpp=&texpos_edge_site_grassy_thin_corner_transition_sw_s[sv];break;
							case 34:tpp=&texpos_edge_site_grassy_thin_corner_transition_sw_w[sv];break;
							case 35:tpp=&texpos_edge_site_grassy_thin_corner_transition_se_s[sv];break;
							case 36:tpp=&texpos_edge_site_grassy_thin_corner_transition_se_e[sv];break;
							}
						break;
					}
				if(*tpp!=0)continue;

				SDL_Surface *tex=enabler.textures.get_texture_data(*tp);
				SDL_Surface *alpha_tex=enabler.textures.get_texture_data(*alpha_tp);
				if(tex==NULL||alpha_tex==NULL)continue;

				*tpp=enabler.textures.create_texture(tex->w,tex->h);
				SDL_Surface *color=enabler.textures.get_texture_data(*tpp);
				if(color==NULL)continue;

				SDL_LockSurface(tex);
				SDL_LockSurface(alpha_tex);
				SDL_LockSurface(color);

				copy_pixel_data_with_alpha_mask(tex,alpha_tex,color);

				SDL_UnlockSurface(color);
				SDL_UnlockSurface(alpha_tex);
				SDL_UnlockSurface(tex);
				}
			}
		}
	{int32_t i,v,f;
	for(i=0;i<11;++i)
	{
	for(v=0;v<4;++v)
	{
	for(f=0;f<BROOK_FLOW_ANIMATION_NUM;++f)
	{
	int32_t src[4]={0,0,0,0};
	int32_t *dst;
	switch(i)
		{
		case 0:
			dst=&gps.texpos_brook_bed_nw_ne_sw_se[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_ne[v][f];
			src[2]=gps.texpos_brook_bed_sw[v][f];
			src[3]=gps.texpos_brook_bed_se[v][f];
			break;
		case 1:
			dst=&gps.texpos_brook_bed_nw_ne_sw[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_ne[v][f];
			src[2]=gps.texpos_brook_bed_sw[v][f];
			break;
		case 2:
			dst=&gps.texpos_brook_bed_nw_ne_se[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_ne[v][f];
			src[2]=gps.texpos_brook_bed_se[v][f];
			break;
		case 3:
			dst=&gps.texpos_brook_bed_nw_sw_se[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_sw[v][f];
			src[2]=gps.texpos_brook_bed_se[v][f];
			break;
		case 4:
			dst=&gps.texpos_brook_bed_ne_sw_se[v][f];
			src[0]=gps.texpos_brook_bed_ne[v][f];
			src[1]=gps.texpos_brook_bed_sw[v][f];
			src[2]=gps.texpos_brook_bed_se[v][f];
			break;
		case 5:
			dst=&gps.texpos_brook_bed_nw_ne[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_ne[v][f];
			break;
		case 6:
			dst=&gps.texpos_brook_bed_nw_sw[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_sw[v][f];
			break;
		case 7:
			dst=&gps.texpos_brook_bed_nw_se[v][f];
			src[0]=gps.texpos_brook_bed_nw[v][f];
			src[1]=gps.texpos_brook_bed_se[v][f];
			break;
		case 8:
			dst=&gps.texpos_brook_bed_ne_sw[v][f];
			src[0]=gps.texpos_brook_bed_ne[v][f];
			src[1]=gps.texpos_brook_bed_sw[v][f];
			break;
		case 9:
			dst=&gps.texpos_brook_bed_ne_se[v][f];
			src[0]=gps.texpos_brook_bed_ne[v][f];
			src[1]=gps.texpos_brook_bed_se[v][f];
			break;
		case 10:
			dst=&gps.texpos_brook_bed_sw_se[v][f];
			src[0]=gps.texpos_brook_bed_sw[v][f];
			src[1]=gps.texpos_brook_bed_se[v][f];
			break;
		}
	if(src[0]==0)continue;
	SDL_Surface *tex=enabler.textures.get_texture_data(src[0]);
	if(tex==NULL)continue;
	*dst=enabler.textures.create_texture(tex->w,tex->h);
	SDL_Surface *dsts=enabler.textures.get_texture_data(*dst);
	if(dsts==NULL)continue;

	//copy the first one over
	SDL_LockSurface(tex);
	SDL_LockSurface(dsts);

	Uint8 *pixel_src,*pixel_dst;
	int x,y;
	for(y=0;y<tex->h;y++)
		{
		pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
		pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
		for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4)
			{
			pixel_dst[0]=pixel_src[0];
			pixel_dst[1]=pixel_src[1];
			pixel_dst[2]=pixel_src[2];
			pixel_dst[3]=pixel_src[3];
			}
		}

	SDL_UnlockSurface(dsts);
	SDL_UnlockSurface(tex);

	//blit the other corners on top
	int32_t si;
	for(si=1;si<4;++si)
		{
		if(src[si]==0)continue;

		SDL_Surface *tex=enabler.textures.get_texture_data(src[si]);
		if(tex==NULL)continue;

		//NOTE: a blit did not work here, it overwrote instead of doing alpha blending
		SDL_LockSurface(tex);
		SDL_LockSurface(dsts);

		Uint8 *pixel_src,*pixel_dst;
		int x,y;
		for(y=0;y<tex->h;y++)
			{
			pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
			pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
			for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4)
				{
				//******************************** BROOKBED
					//instead of using the most alpha pixel, can we get the blend formula?
				if(pixel_dst[3]<pixel_src[3])
					{
					pixel_dst[0]=pixel_src[0];
					pixel_dst[1]=pixel_src[1];
					pixel_dst[2]=pixel_src[2];
					pixel_dst[3]=pixel_src[3];
					}
				}
			}

		SDL_UnlockSurface(dsts);
		SDL_UnlockSurface(tex);
		}
	}}}}

	//grid alpha
	{
	SDL_Surface *tex=enabler.textures.get_texture_data(texpos_viewport_grid);
	if(tex!=NULL)
		{
		SDL_LockSurface(tex);
		Uint8 *pixel_src;
		int x,y;
		for(y=0;y<tex->h;y++)
			{
			pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
			for(x=0;x<tex->w;++x,pixel_src+=4)
				{
				if(pixel_src[3]!=0)pixel_src[3]=128;
				}
			}
		SDL_UnlockSurface(tex);
		}
	}

	{int32_t dig_type;
	for(dig_type=0;dig_type<7;++dig_type)
		{
		int32_t src_tp=0;
		switch(dig_type)
			{
			case 0:src_tp=texpos_designation_dig_stair_updown[0][0];break;
			case 1:src_tp=texpos_designation_dig_stair_up[0][0];break;
			case 2:src_tp=texpos_designation_dig_stair_down[0][0];break;
			case 3:src_tp=texpos_designation_dig_ramp[0][0];break;
			case 4:src_tp=texpos_designation_dig_channel[0][0];break;
			case 5:src_tp=texpos_designation_dig_remove_construction[0][0];break;
			case 6:src_tp=texpos_designation_dig_standard[0][0];break;
			}
		if(src_tp==0)continue;
		SDL_Surface *tex=enabler.textures.get_texture_data(src_tp);
		if(tex==NULL)continue;

		SDL_LockSurface(tex);

		int32_t p,m;
		for(p=0;p<8;++p)
			{
			SDL_Surface *ptex=NULL;
			int32_t psrc=0;
			if(p>=1)psrc=texpos_designation_priority[p-1];
			if(psrc!=0)
				{
				ptex=enabler.textures.get_texture_data(psrc);
				}

			for(m=0;m<3;++m)
				{
				if(p==0&&m==0)continue;
				int32_t *dst;
				switch(dig_type)
					{
					case 0:dst=&texpos_designation_dig_stair_updown[p][m];break;
					case 1:dst=&texpos_designation_dig_stair_up[p][m];break;
					case 2:dst=&texpos_designation_dig_stair_down[p][m];break;
					case 3:dst=&texpos_designation_dig_ramp[p][m];break;
					case 4:dst=&texpos_designation_dig_channel[p][m];break;
					case 5:dst=&texpos_designation_dig_remove_construction[p][m];break;
					case 6:dst=&texpos_designation_dig_standard[p][m];break;
					}
				*dst=enabler.textures.create_texture(tex->w,tex->h);
				SDL_Surface *dsts=enabler.textures.get_texture_data(*dst);
				if(dsts==NULL)continue;

				SDL_LockSurface(dsts);

				Uint8 *pixel_src,*pixel_dst,*pixel_psrc;
				int x,y;
				for(y=0;y<tex->h;y++)
					{
					pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
					pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
					if(ptex!=NULL)pixel_psrc=((Uint8*)ptex->pixels)+(y*ptex->pitch);
					else pixel_psrc=0;//doesn't matter
					for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4,pixel_psrc+=4)
						{
						pixel_dst[3]=pixel_src[3];

						if(m==0)
							{
							pixel_dst[0]=pixel_src[0];
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=pixel_src[2];
							}
						if(m==1)
							{
							pixel_dst[0]=0;
							pixel_dst[1]=pixel_src[1]/2;
							pixel_dst[2]=pixel_src[2];
							}
						if(m==2)
							{
							pixel_dst[0]=0;
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=0;
							}

						if(ptex!=NULL&&pixel_psrc[3]!=0)
							{
							pixel_dst[0]=pixel_psrc[0];
							pixel_dst[1]=pixel_psrc[1];
							pixel_dst[2]=pixel_psrc[2];
							}
						}
					}

				SDL_UnlockSurface(dsts);
				}
			}
		SDL_UnlockSurface(tex);
		}}
	{int32_t des_type;
	for(des_type=0;des_type<20;++des_type)
		{
		int32_t src_tp=0;
		switch(des_type)
			{
			case 0:src_tp=texpos_designation_chop[0][0];break;
			case 1:src_tp=texpos_designation_gather[0][0];break;
			case 2:src_tp=texpos_designation_smooth[0][0];break;
			case 3:src_tp=texpos_designation_engrave[0][0];break;
			case 4:src_tp=texpos_designation_fortify[0][0];break;
			case 5:src_tp=texpos_designation_track_n[0][0];break;
			case 6:src_tp=texpos_designation_track_s[0][0];break;
			case 7:src_tp=texpos_designation_track_w[0][0];break;
			case 8:src_tp=texpos_designation_track_e[0][0];break;
			case 9:src_tp=texpos_designation_track_ns[0][0];break;
			case 10:src_tp=texpos_designation_track_nw[0][0];break;
			case 11:src_tp=texpos_designation_track_ne[0][0];break;
			case 12:src_tp=texpos_designation_track_sw[0][0];break;
			case 13:src_tp=texpos_designation_track_se[0][0];break;
			case 14:src_tp=texpos_designation_track_we[0][0];break;
			case 15:src_tp=texpos_designation_track_nsw[0][0];break;
			case 16:src_tp=texpos_designation_track_nse[0][0];break;
			case 17:src_tp=texpos_designation_track_nwe[0][0];break;
			case 18:src_tp=texpos_designation_track_swe[0][0];break;
			case 19:src_tp=texpos_designation_track_nswe[0][0];break;
			}
		if(src_tp==0)continue;
		SDL_Surface *tex=enabler.textures.get_texture_data(src_tp);
		if(tex==NULL)continue;

		SDL_LockSurface(tex);

		int32_t p,m;
		for(p=0;p<8;++p)
			{
			SDL_Surface *ptex=NULL;
			int32_t psrc=0;
			if(p>=1)psrc=texpos_designation_priority[p-1];
			if(psrc!=0)
				{
				ptex=enabler.textures.get_texture_data(psrc);
				}

			for(m=0;m<2;++m)
				{
				if(p==0&&m==0)continue;
				int32_t *dst;
				switch(des_type)
					{
					case 0:dst=&texpos_designation_chop[p][m];break;
					case 1:dst=&texpos_designation_gather[p][m];break;
					case 2:dst=&texpos_designation_smooth[p][m];break;
					case 3:dst=&texpos_designation_engrave[p][m];break;
					case 4:dst=&texpos_designation_fortify[p][m];break;
					case 5:dst=&texpos_designation_track_n[p][m];break;
					case 6:dst=&texpos_designation_track_s[p][m];break;
					case 7:dst=&texpos_designation_track_w[p][m];break;
					case 8:dst=&texpos_designation_track_e[p][m];break;
					case 9:dst=&texpos_designation_track_ns[p][m];break;
					case 10:dst=&texpos_designation_track_nw[p][m];break;
					case 11:dst=&texpos_designation_track_ne[p][m];break;
					case 12:dst=&texpos_designation_track_sw[p][m];break;
					case 13:dst=&texpos_designation_track_se[p][m];break;
					case 14:dst=&texpos_designation_track_we[p][m];break;
					case 15:dst=&texpos_designation_track_nsw[p][m];break;
					case 16:dst=&texpos_designation_track_nse[p][m];break;
					case 17:dst=&texpos_designation_track_nwe[p][m];break;
					case 18:dst=&texpos_designation_track_swe[p][m];break;
					case 19:dst=&texpos_designation_track_nswe[p][m];break;
					}
				*dst=enabler.textures.create_texture(tex->w,tex->h);
				SDL_Surface *dsts=enabler.textures.get_texture_data(*dst);
				if(dsts==NULL)continue;

				SDL_LockSurface(dsts);

				Uint8 *pixel_src,*pixel_dst,*pixel_psrc;
				int x,y;
				for(y=0;y<tex->h;y++)
					{
					pixel_src=((Uint8*)tex->pixels)+(y*tex->pitch);
					pixel_dst=((Uint8*)dsts->pixels)+(y*dsts->pitch);
					if(ptex!=NULL)pixel_psrc=((Uint8*)ptex->pixels)+(y*ptex->pitch);
					else pixel_psrc=0;//doesn't matter
					for(x=0;x<tex->w;++x,pixel_src+=4,pixel_dst+=4,pixel_psrc+=4)
						{
						pixel_dst[3]=pixel_src[3];

						if(m==0)
							{
							pixel_dst[0]=pixel_src[0];
							pixel_dst[1]=pixel_src[1];
							pixel_dst[2]=pixel_src[2];
							}
						if(m==1)
							{
							pixel_dst[0]=0;
							pixel_dst[1]=pixel_src[1]/2;
							pixel_dst[2]=pixel_src[2];
							}

						if(ptex!=NULL&&pixel_psrc[3]!=0)
							{
							pixel_dst[0]=pixel_psrc[0];
							pixel_dst[1]=pixel_psrc[1];
							pixel_dst[2]=pixel_psrc[2];
							}
						}
					}

				SDL_UnlockSurface(dsts);
				}
			}
		SDL_UnlockSurface(tex);
		}}

	/*
	if(texpos_item_barrel_wood!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(texpos_item_barrel_wood);
		if(tex!=NULL)
			{
			texpos_item_barrel_wood_lockable=enabler.textures.create_texture(tex->w,tex->h);
			SDL_Surface *color=enabler.textures.get_texture_data(texpos_item_barrel_wood_lockable);

			if(color!=NULL)
				{
				SDL_LockSurface(tex);
				SDL_LockSurface(color);
      
				copy_transformed_pixel_data(tex,color,0,false);

				SDL_UnlockSurface(color);
				SDL_UnlockSurface(tex);
				}
			}
		}
		*/
	/*
	if(texpos_item_barrel_metal!=0)
		{
		SDL_Surface *tex=enabler.textures.get_texture_data(texpos_item_barrel_metal);
		if(tex!=NULL)
			{
			texpos_item_barrel_metal_lockable=enabler.textures.create_texture(tex->w,tex->h);
			SDL_Surface *color=enabler.textures.get_texture_data(texpos_item_barrel_metal_lockable);

			if(color!=NULL)
				{
				SDL_LockSurface(tex);
				SDL_LockSurface(color);
      
				copy_transformed_pixel_data(tex,color,0,false);

				SDL_UnlockSurface(color);
				SDL_UnlockSurface(tex);
				}
			}
		}
		*/

	int32_t sh;
	for(sh=0;sh<29;++sh)
		{
		int32_t *dst=NULL;
		int32_t src[4],src_num=0;
		switch(sh)
			{
			case 0:dst=&texpos_vision_shadow_n_sw;src_num=2;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_sw;break;
			case 1:dst=&texpos_vision_shadow_n_se;src_num=2;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_se;break;
			case 2:dst=&texpos_vision_shadow_s_nw;src_num=2;src[0]=texpos_vision_shadow_s;src[1]=texpos_vision_shadow_nw;break;
			case 3:dst=&texpos_vision_shadow_s_ne;src_num=2;src[0]=texpos_vision_shadow_s;src[1]=texpos_vision_shadow_ne;break;
			case 4:dst=&texpos_vision_shadow_w_ne;src_num=2;src[0]=texpos_vision_shadow_w;src[1]=texpos_vision_shadow_ne;break;
			case 5:dst=&texpos_vision_shadow_w_se;src_num=2;src[0]=texpos_vision_shadow_w;src[1]=texpos_vision_shadow_se;break;
			case 6:dst=&texpos_vision_shadow_e_nw;src_num=2;src[0]=texpos_vision_shadow_e;src[1]=texpos_vision_shadow_nw;break;
			case 7:dst=&texpos_vision_shadow_e_sw;src_num=2;src[0]=texpos_vision_shadow_e;src[1]=texpos_vision_shadow_sw;break;
			case 8:dst=&texpos_vision_shadow_n_w_se;src_num=3;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_w;src[2]=texpos_vision_shadow_se;break;
			case 9:dst=&texpos_vision_shadow_n_e_sw;src_num=3;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_e;src[2]=texpos_vision_shadow_sw;break;
			case 10:dst=&texpos_vision_shadow_s_w_ne;src_num=3;src[0]=texpos_vision_shadow_s;src[1]=texpos_vision_shadow_w;src[2]=texpos_vision_shadow_ne;break;
			case 11:dst=&texpos_vision_shadow_s_e_nw;src_num=3;src[0]=texpos_vision_shadow_s;src[1]=texpos_vision_shadow_e;src[2]=texpos_vision_shadow_nw;break;
			case 12:dst=&texpos_vision_shadow_nw_ne;src_num=2;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_ne;break;
			case 13:dst=&texpos_vision_shadow_nw_sw;src_num=2;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_sw;break;
			case 14:dst=&texpos_vision_shadow_nw_se;src_num=2;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_se;break;
			case 15:dst=&texpos_vision_shadow_ne_sw;src_num=2;src[0]=texpos_vision_shadow_ne;src[1]=texpos_vision_shadow_sw;break;
			case 16:dst=&texpos_vision_shadow_ne_se;src_num=2;src[0]=texpos_vision_shadow_ne;src[1]=texpos_vision_shadow_se;break;
			case 17:dst=&texpos_vision_shadow_sw_se;src_num=2;src[0]=texpos_vision_shadow_sw;src[1]=texpos_vision_shadow_se;break;
			case 18:dst=&texpos_vision_shadow_nw_ne_sw;src_num=3;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_ne;src[2]=texpos_vision_shadow_sw;break;
			case 19:dst=&texpos_vision_shadow_nw_ne_se;src_num=3;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_ne;src[2]=texpos_vision_shadow_se;break;
			case 20:dst=&texpos_vision_shadow_nw_sw_se;src_num=3;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_sw;src[2]=texpos_vision_shadow_se;break;
			case 21:dst=&texpos_vision_shadow_ne_sw_se;src_num=3;src[0]=texpos_vision_shadow_ne;src[1]=texpos_vision_shadow_sw;src[2]=texpos_vision_shadow_se;break;
			case 22:dst=&texpos_vision_shadow_nw_ne_sw_se;src_num=4;src[0]=texpos_vision_shadow_nw;src[1]=texpos_vision_shadow_ne;src[2]=texpos_vision_shadow_sw;src[3]=texpos_vision_shadow_se;break;
			case 23:dst=&texpos_vision_shadow_n_s;src_num=2;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_s;break;
			case 24:dst=&texpos_vision_shadow_w_e;src_num=2;src[0]=texpos_vision_shadow_w;src[1]=texpos_vision_shadow_e;break;
			case 25:dst=&texpos_vision_shadow_n_sw_se;src_num=3;src[0]=texpos_vision_shadow_n;src[1]=texpos_vision_shadow_sw;src[2]=texpos_vision_shadow_se;break;
			case 26:dst=&texpos_vision_shadow_s_nw_ne;src_num=3;src[0]=texpos_vision_shadow_s;src[1]=texpos_vision_shadow_nw;src[2]=texpos_vision_shadow_ne;break;
			case 27:dst=&texpos_vision_shadow_w_ne_se;src_num=3;src[0]=texpos_vision_shadow_w;src[1]=texpos_vision_shadow_ne;src[2]=texpos_vision_shadow_se;break;
			case 28:dst=&texpos_vision_shadow_e_nw_sw;src_num=3;src[0]=texpos_vision_shadow_e;src[1]=texpos_vision_shadow_nw;src[2]=texpos_vision_shadow_sw;break;
			}
		if(dst==NULL)continue;
		if(*dst==0&&src_num>0)
			{
			bool good_source=true;
			int32_t s;
			for(s=0;s<src_num;++s)
				{
				if(src[s]==0)
					{
					good_source=false;
					break;
					}
				}
			if(good_source)
				{
				for(s=0;s<src_num;++s)
					{
					SDL_Surface *tex=enabler.textures.get_texture_data(src[s]);
					SDL_Surface *color=NULL;
					if(tex!=NULL)
						{
						if(s==0)
							{
							*dst=enabler.textures.create_texture(tex->w,tex->h);
							color=enabler.textures.get_texture_data(*dst);
							SDL_LockSurface(color);
							}

						if(color!=NULL)
							{
							SDL_LockSurface(tex);

							if(s==0)copy_transformed_pixel_data(tex,color,0,false);
							else copy_pixel_data_nonzero_only_no_blend(tex,color);

							SDL_UnlockSurface(tex);
							}
						}
					if(color!=NULL)SDL_UnlockSurface(color);
					}
				}
			}
		}
}

void graphicst::set_force_full_display_count(short new_count)
{
	if(force_full_display_count<new_count)force_full_display_count=new_count;
}

void graphicst::zero_and_clear_all_texpos()
{
	texpos_boulder.clear();
	texpos_driftwood=0;

	texpos_campfire_top[0]=0;
	texpos_campfire_top[1]=0;
	texpos_campfire_top[2]=0;
	texpos_campfire_top[3]=0;
	texpos_campfire[0]=0;
	texpos_campfire[1]=0;
	texpos_campfire[2]=0;
	texpos_campfire[3]=0;
	texpos_campfire_dead=0;

	texpos_fire[0]=0;
	texpos_fire[1]=0;
	texpos_fire[2]=0;
	texpos_fire[3]=0;
	texpos_fireball[0]=0;
	texpos_fireball[1]=0;
	texpos_eerie_pit[0]=0;
	texpos_eerie_pit[1]=0;
	texpos_eerie_pit[2]=0;
	texpos_eerie_pit[3]=0;
	texpos_floor_ashes[0]=0;
	texpos_floor_ashes[1]=0;
	texpos_floor_ashes[2]=0;
	texpos_floor_ashes[3]=0;
	texpos_floor_stone_block=0;
	texpos_floor_stone_engraved_non_palette=0;
	texpos_floor_stone_engraved_palette=0;
	//********************************* BETTER ENGRAVINGS init

	texpos_floor_wood=0;
	texpos_floor_ice_rough=0;
	texpos_floor_ice_smooth=0;
	texpos_floor_glass_green=0;
	texpos_floor_glass_clear=0;
	texpos_floor_glass_crystal=0;
	memset(texpos_floor_sand,0,sizeof(int32_t)*4*9);
	memset(texpos_floor_sand_yellow,0,sizeof(int32_t)*4*9);
	memset(texpos_floor_sand_white,0,sizeof(int32_t)*4*9);
	memset(texpos_floor_sand_black,0,sizeof(int32_t)*4*9);
	memset(texpos_floor_sand_red,0,sizeof(int32_t)*4*9);
	texpos_floor_metal=0;
	texpos_floor_furrowed_soil[0]=0;
	texpos_floor_furrowed_soil[1]=0;
	texpos_floor_furrowed_soil[2]=0;
	texpos_floor_furrowed_soil[3]=0;

	int32_t dg;
	for(dg=0;dg<2;++dg)
		{
		texpos_guts_n[dg]=0;
		texpos_guts_s[dg]=0;
		texpos_guts_w[dg]=0;
		texpos_guts_e[dg]=0;
		texpos_guts_nw[dg]=0;
		texpos_guts_sw[dg]=0;
		texpos_guts_ne[dg]=0;
		texpos_guts_se[dg]=0;
		}

	texpos_custom_symbol.clear();

	grass_stair_updown_texpos=0;
	grass_stair_up_texpos=0;
	grass_stair_down_texpos=0;
	stone_stair_updown_texpos=0;
	stone_stair_up_texpos=0;
	stone_stair_down_texpos=0;
	dirt_stair_updown_texpos=0;
	dirt_stair_up_texpos=0;
	dirt_stair_down_texpos=0;
	sand_stair_updown_texpos=0;
	sand_stair_up_texpos=0;
	sand_stair_down_texpos=0;
	sand_y_stair_updown_texpos=0;
	sand_y_stair_up_texpos=0;
	sand_y_stair_down_texpos=0;
	sand_w_stair_updown_texpos=0;
	sand_w_stair_up_texpos=0;
	sand_w_stair_down_texpos=0;
	sand_r_stair_updown_texpos=0;
	sand_r_stair_up_texpos=0;
	sand_r_stair_down_texpos=0;
	sand_b_stair_updown_texpos=0;
	sand_b_stair_up_texpos=0;
	sand_b_stair_down_texpos=0;
	palette_stair_updown_texpos=0;
	palette_stair_up_texpos=0;
	palette_stair_down_texpos=0;
	smooth_floor_texpos=0;
	workshop_floor_texpos=0;
	
	memset(texpos_scaffold_floor_wood,0,sizeof(int32_t)*4);
	texpos_scaffold_stair_updown_wood=0;
	texpos_scaffold_stair_up_wood=0;
	texpos_scaffold_stair_down_wood=0;
	texpos_scaffold_underside_wood=0;
	memset(texpos_scaffold_floor_stone,0,sizeof(int32_t)*4);
	texpos_scaffold_stair_updown_stone=0;
	texpos_scaffold_stair_up_stone=0;
	texpos_scaffold_stair_down_stone=0;
	texpos_scaffold_underside_stone=0;

	texpos_fortification=0;
	texpos_fortification_open_nswe=0;
	texpos_fortification_open_nsw=0;
	texpos_fortification_open_nse=0;
	texpos_fortification_open_nwe=0;
	texpos_fortification_open_swe=0;
	texpos_fortification_open_ns=0;
	texpos_fortification_open_we=0;
	texpos_fortification_open_nw=0;
	texpos_fortification_open_ne=0;
	texpos_fortification_open_sw=0;
	texpos_fortification_open_se=0;

	texpos_fortification_ice=0;
	texpos_fortification_ice_open_nswe=0;
	texpos_fortification_ice_open_nsw=0;
	texpos_fortification_ice_open_nse=0;
	texpos_fortification_ice_open_nwe=0;
	texpos_fortification_ice_open_swe=0;
	texpos_fortification_ice_open_ns=0;
	texpos_fortification_ice_open_we=0;
	texpos_fortification_ice_open_nw=0;
	texpos_fortification_ice_open_ne=0;
	texpos_fortification_ice_open_sw=0;
	texpos_fortification_ice_open_se=0;

	texpos_fortification_wood=0;
	texpos_fortification_wood_open_nswe=0;
	texpos_fortification_wood_open_nsw=0;
	texpos_fortification_wood_open_nse=0;
	texpos_fortification_wood_open_nwe=0;
	texpos_fortification_wood_open_swe=0;
	texpos_fortification_wood_open_ns=0;
	texpos_fortification_wood_open_we=0;
	texpos_fortification_wood_open_nw=0;
	texpos_fortification_wood_open_ne=0;
	texpos_fortification_wood_open_sw=0;
	texpos_fortification_wood_open_se=0;

	texpos_unit_selector_pasture=0;
	texpos_unit_selector_pit=0;
	texpos_unit_selector_pond=0;
	texpos_creature_is_traveling=0;
	texpos_creature_is_dead=0;
	texpos_building_icon_workshops=0;
	texpos_building_icon_furniture=0;
	texpos_building_icon_doors_hatches=0;
	texpos_building_icon_walls_floors=0;
	texpos_building_icon_machines_fluids=0;
	texpos_building_icon_cages_restraints=0;
	texpos_building_icon_traps=0;
	texpos_building_icon_military=0;
	texpos_building_icon_trade_depot=0;
	texpos_building_icon_workshop_carpenter=0;
	texpos_building_icon_workshop_mason=0;
	texpos_building_icon_workshop_metalsmith=0;
	texpos_building_icon_workshops_furnaces=0;
	texpos_building_icon_workshop_crafts=0;
	texpos_building_icon_workshop_jeweler=0;
	texpos_building_icon_workshops_clothing=0;
	texpos_building_icon_workshops_farming=0;
	texpos_building_icon_workshop_bowyer=0;
	texpos_building_icon_workshop_mechanic=0;
	texpos_building_icon_workshop_siege=0;
	texpos_building_icon_workshop_ashery=0;
	texpos_building_icon_furnace_wood=0;
	texpos_building_icon_furnace_smelter=0;
	texpos_building_icon_furnace_glass=0;
	texpos_building_icon_furnace_kiln=0;
	texpos_building_icon_workshop_leather=0;
	texpos_building_icon_workshop_loom=0;
	texpos_building_icon_workshop_clothes=0;
	texpos_building_icon_workshop_dyer=0;
	texpos_building_icon_farm_plot=0;
	texpos_building_icon_workshop_still=0;
	texpos_building_icon_workshop_butcher=0;
	texpos_building_icon_workshop_tanner=0;
	texpos_building_icon_workshop_fishery=0;
	texpos_building_icon_workshop_kitchen=0;
	texpos_building_icon_workshop_farmer=0;
	texpos_building_icon_workshop_quern=0;
	texpos_building_icon_workshop_kennel=0;
	texpos_building_icon_nest_box=0;
	texpos_building_icon_hive=0;
	texpos_building_icon_bed=0;
	texpos_building_icon_chair=0;
	texpos_building_icon_table=0;
	texpos_building_icon_box=0;
	texpos_building_icon_cabinet=0;
	texpos_building_icon_coffin=0;
	texpos_building_icon_slab=0;
	texpos_building_icon_statue=0;
	texpos_building_icon_traction_bench=0;
	texpos_building_icon_bookcase=0;
	texpos_building_icon_display_furniture=0;
	texpos_building_icon_offering_place=0;
	texpos_building_icon_instrument=0;
	texpos_building_icon_door=0;
	texpos_building_icon_hatch=0;
	texpos_building_icon_wall=0;
	texpos_building_icon_reinforced_wall=0;
	texpos_building_icon_floor=0;
	texpos_building_icon_ramp=0;
	texpos_building_icon_stairs=0;
	texpos_building_icon_bridge=0;
	texpos_building_icon_road_paved=0;
	texpos_building_icon_road_dirt=0;
	texpos_building_icon_fortification=0;
	texpos_building_icon_grate_wall=0;
	texpos_building_icon_grate_floor=0;
	texpos_building_icon_bars_vertical=0;
	texpos_building_icon_bars_floors=0;
	texpos_building_icon_window_glass=0;
	texpos_building_icon_window_gem=0;
	texpos_building_icon_support=0;
	texpos_building_icon_track=0;
	texpos_building_icon_track_stop=0;
	texpos_building_icon_lever=0;
	texpos_building_icon_well=0;
	texpos_building_icon_floodgate=0;
	texpos_building_icon_screw_pump=0;
	texpos_building_icon_water_wheel=0;
	texpos_building_icon_windmill=0;
	texpos_building_icon_gear_assembly=0;
	texpos_building_icon_axle_horizontal=0;
	texpos_building_icon_axle_vertical=0;
	texpos_building_icon_workshop_millstone=0;
	texpos_building_icon_rollers=0;
	texpos_building_icon_restraint=0;
	texpos_building_icon_cage=0;
	texpos_building_icon_animal_trap=0;
	texpos_building_icon_pressure_plate=0;
	texpos_building_icon_trap_stone=0;
	texpos_building_icon_trap_weapon=0;
	texpos_building_icon_trap_cage=0;
	texpos_building_icon_weapon=0;
	texpos_building_icon_archery_target=0;
	texpos_building_icon_weapon_rack=0;
	texpos_building_icon_armor_stand=0;
	texpos_building_icon_ballista=0;
	texpos_building_icon_catapult=0;
	texpos_building_icon_bolt_thrower=0;
	texpos_building_icon_wagon=0;
	{int32_t c;
	for(c=0;c<38;++c)texpos_planned_construction[c]=0;}
	texpos_zone_inactive_n_s_w_e=0;
	texpos_zone_inactive_n_w=0;
	texpos_zone_inactive_n_e=0;
	texpos_zone_inactive_s_e=0;
	texpos_zone_inactive_s_w=0;
	texpos_zone_inactive_n=0;
	texpos_zone_inactive_e=0;
	texpos_zone_inactive_w=0;
	texpos_zone_inactive_s=0;
	texpos_zone_inactive_n_s=0;
	texpos_zone_inactive_w_e=0;
	texpos_zone_inactive_n_w_e=0;
	texpos_zone_inactive_s_w_e=0;
	texpos_zone_inactive_n_s_w=0;
	texpos_zone_inactive_n_s_e=0;
	texpos_zone_inactive=0;
	texpos_zone_inactive_selected_n_s_w_e=0;
	texpos_zone_inactive_selected_n_w=0;
	texpos_zone_inactive_selected_n_e=0;
	texpos_zone_inactive_selected_s_e=0;
	texpos_zone_inactive_selected_s_w=0;
	texpos_zone_inactive_selected_n=0;
	texpos_zone_inactive_selected_e=0;
	texpos_zone_inactive_selected_w=0;
	texpos_zone_inactive_selected_s=0;
	texpos_zone_inactive_selected_n_s=0;
	texpos_zone_inactive_selected_w_e=0;
	texpos_zone_inactive_selected_n_w_e=0;
	texpos_zone_inactive_selected_s_w_e=0;
	texpos_zone_inactive_selected_n_s_w=0;
	texpos_zone_inactive_selected_n_s_e=0;
	texpos_zone_inactive_selected=0;
	texpos_zone_active_n_s_w_e=0;
	texpos_zone_active_n_w=0;
	texpos_zone_active_n_e=0;
	texpos_zone_active_s_e=0;
	texpos_zone_active_s_w=0;
	texpos_zone_active_n=0;
	texpos_zone_active_e=0;
	texpos_zone_active_w=0;
	texpos_zone_active_s=0;
	texpos_zone_active_n_s=0;
	texpos_zone_active_w_e=0;
	texpos_zone_active_n_w_e=0;
	texpos_zone_active_s_w_e=0;
	texpos_zone_active_n_s_w=0;
	texpos_zone_active_n_s_e=0;
	texpos_zone_active=0;
	texpos_zone_selected_n_s_w_e=0;
	texpos_zone_selected_n_w=0;
	texpos_zone_selected_n_e=0;
	texpos_zone_selected_s_e=0;
	texpos_zone_selected_s_w=0;
	texpos_zone_selected_n=0;
	texpos_zone_selected_e=0;
	texpos_zone_selected_w=0;
	texpos_zone_selected_s=0;
	texpos_zone_selected_n_s=0;
	texpos_zone_selected_w_e=0;
	texpos_zone_selected_n_w_e=0;
	texpos_zone_selected_s_w_e=0;
	texpos_zone_selected_n_s_w=0;
	texpos_zone_selected_n_s_e=0;
	texpos_zone_selected=0;
	texpos_zone_general_n_s_w_e=0;
	texpos_zone_general_n_w=0;
	texpos_zone_general_n_e=0;
	texpos_zone_general_s_e=0;
	texpos_zone_general_s_w=0;
	texpos_zone_general_n=0;
	texpos_zone_general_e=0;
	texpos_zone_general_w=0;
	texpos_zone_general_s=0;
	texpos_zone_general_n_s=0;
	texpos_zone_general_w_e=0;
	texpos_zone_general_n_w_e=0;
	texpos_zone_general_s_w_e=0;
	texpos_zone_general_n_s_w=0;
	texpos_zone_general_n_s_e=0;
	texpos_zone_general=0;
	texpos_zone_bedroom=0;
	texpos_zone_dining_hall=0;
	texpos_zone_office=0;
	texpos_zone_archery_range=0;
	texpos_zone_barracks=0;
	texpos_zone_dormitory=0;
	texpos_zone_dungeon=0;
	texpos_zone_tomb=0;
	texpos_zone_water_source=0;
	texpos_zone_fishing=0;
	texpos_zone_gather=0;
	texpos_zone_dump=0;
	texpos_zone_pen=0;
	texpos_zone_pit=0;
	texpos_zone_sand=0;
	texpos_zone_clay=0;
	texpos_zone_meeting=0;
	texpos_zone_hospital=0;
	texpos_zone_animal_training=0;
	texpos_zone_multi=0;
	texpos_zone_tavern=0;
	texpos_zone_library=0;
	texpos_zone_guildhall=0;
	texpos_zone_grand_guildhall=0;
	texpos_zone_shrine=0;
	texpos_zone_temple=0;
	texpos_zone_temple_complex=0;
	stockpile_floor_texpos=0;
	stockpile_n_up_texpos=0;
	stockpile_n_texpos=0;
	stockpile_s_texpos=0;
	stockpile_w_up_texpos=0;
	stockpile_w_texpos=0;
	stockpile_e_up_texpos=0;
	stockpile_e_texpos=0;
	stockpile_nw_up_texpos=0;
	stockpile_nw_texpos=0;
	stockpile_ne_up_texpos=0;
	stockpile_ne_texpos=0;
	stockpile_sw_texpos=0;
	stockpile_se_texpos=0;
	stockpile_nw_up_texpos=0;
	stockpile_nw_texpos=0;
	stockpile_ns_up_texpos=0;
	stockpile_ns_texpos=0;
	stockpile_we_texpos=0;
	stockpile_nsw_up_texpos=0;
	stockpile_nsw_texpos=0;
	stockpile_nse_up_texpos=0;
	stockpile_nse_texpos=0;
	stockpile_nwe_up_texpos=0;
	stockpile_nwe_texpos=0;
	stockpile_swe_texpos=0;
	stockpile_nswe_up_texpos=0;
	stockpile_nswe_texpos=0;
	stockpile_blank_texpos=0;
	stockpile_ammo_texpos=0;
	stockpile_animals_texpos=0;
	stockpile_armor_texpos=0;
	stockpile_bars_texpos=0;
	stockpile_cloth_texpos=0;
	stockpile_coins_texpos=0;
	stockpile_corpses_texpos=0;
	stockpile_finished_goods_texpos=0;
	stockpile_food_texpos=0;
	stockpile_furniture_texpos=0;
	stockpile_gems_texpos=0;
	stockpile_leather_texpos=0;
	stockpile_refuse_texpos=0;
	stockpile_sheets_texpos=0;
	stockpile_stone_texpos=0;
	stockpile_weapons_texpos=0;
	stockpile_wood_texpos=0;
	stockpile_custom_texpos=0;
	stockpile_icon_blank_texpos=0;
	stockpile_icon_ammo_texpos=0;
	stockpile_icon_animals_texpos=0;
	stockpile_icon_armor_texpos=0;
	stockpile_icon_bars_texpos=0;
	stockpile_icon_cloth_texpos=0;
	stockpile_icon_coins_texpos=0;
	stockpile_icon_corpses_texpos=0;
	stockpile_icon_finished_goods_texpos=0;
	stockpile_icon_food_texpos=0;
	stockpile_icon_furniture_texpos=0;
	stockpile_icon_gems_texpos=0;
	stockpile_icon_leather_texpos=0;
	stockpile_icon_refuse_texpos=0;
	stockpile_icon_sheets_texpos=0;
	stockpile_icon_stone_texpos=0;
	stockpile_icon_weapons_texpos=0;
	stockpile_icon_wood_texpos=0;
	stockpile_icon_custom_texpos=0;
	stockpile_icon_all_texpos=0;
	stockpile_icon_signless_blank_texpos=0;
	stockpile_icon_signless_ammo_texpos=0;
	stockpile_icon_signless_animals_texpos=0;
	stockpile_icon_signless_armor_texpos=0;
	stockpile_icon_signless_bars_texpos=0;
	stockpile_icon_signless_cloth_texpos=0;
	stockpile_icon_signless_coins_texpos=0;
	stockpile_icon_signless_corpses_texpos=0;
	stockpile_icon_signless_finished_goods_texpos=0;
	stockpile_icon_signless_food_texpos=0;
	stockpile_icon_signless_furniture_texpos=0;
	stockpile_icon_signless_gems_texpos=0;
	stockpile_icon_signless_leather_texpos=0;
	stockpile_icon_signless_refuse_texpos=0;
	stockpile_icon_signless_sheets_texpos=0;
	stockpile_icon_signless_stone_texpos=0;
	stockpile_icon_signless_weapons_texpos=0;
	stockpile_icon_signless_wood_texpos=0;
	stockpile_icon_signless_custom_texpos=0;
	stockpile_icon_signless_all_texpos=0;
	memset(texpos_wagon_bld,0,sizeof(int32_t)*4*3);
	memset(texpos_wagon_n,0,sizeof(int32_t)*3*4);
	memset(texpos_wagon_s,0,sizeof(int32_t)*3*4);
	memset(texpos_wagon_w,0,sizeof(int32_t)*4*3);
	memset(texpos_wagon_e,0,sizeof(int32_t)*4*3);
	memset(texpos_wagon_goods_bld,0,sizeof(int32_t)*3*4*4);
	memset(texpos_wagon_goods_n,0,sizeof(int32_t)*3*4*4);
	memset(texpos_wagon_goods_s,0,sizeof(int32_t)*3*4*4);
	memset(texpos_wagon_goods_w,0,sizeof(int32_t)*4*3*4);
	memset(texpos_wagon_goods_e,0,sizeof(int32_t)*4*3*4);

	memset(texpos_ram_wood,0,sizeof(int32_t)*4*3);
	memset(texpos_ram_banded,0,sizeof(int32_t)*4*3);
	memset(texpos_ram_bands,0,sizeof(int32_t)*4*3);
	memset(texpos_ram_head,0,sizeof(int32_t)*RAM_HEADNUM*4);

	memset(texpos_border_top_left,0,sizeof(int32_t)*4*3);
	memset(texpos_border_top_right,0,sizeof(int32_t)*4*3);
	memset(texpos_border_bottom_left,0,sizeof(int32_t)*4*3);
	memset(texpos_border_bottom_right,0,sizeof(int32_t)*4*3);
	memset(texpos_border_top_intersection,0,sizeof(int32_t)*4*2);
	memset(texpos_border_bottom_intersection,0,sizeof(int32_t)*4*2);
	memset(texpos_border_middle_intersection,0,sizeof(int32_t)*4);
	memset(texpos_border_left_intersection,0,sizeof(int32_t)*3*3);
	memset(texpos_border_right_intersection,0,sizeof(int32_t)*3*3);
	memset(texpos_border_left,0,sizeof(int32_t)*4);
	memset(texpos_border_right,0,sizeof(int32_t)*4);
	memset(texpos_border_top,0,sizeof(int32_t)*3);
	memset(texpos_border_bottom,0,sizeof(int32_t)*3);

	memset(texpos_hover_rectangle,0,sizeof(int32_t)*3*3);
	texpos_hover_rectangle_join_w_sw=0;
	texpos_hover_rectangle_join_w_s=0;
	texpos_hover_rectangle_join_e_s=0;
	texpos_hover_rectangle_join_e_se=0;
	memset(texpos_hover_close,0,sizeof(int32_t)*3*2);
	memset(texpos_hover_tab,0,sizeof(int32_t)*3*3);
	memset(texpos_hover_tab_inactive,0,sizeof(int32_t)*3);
	texpos_hover_tab_inside_corner_top=0;
	texpos_hover_tab_inside_corner_bottom=0;
	memset(texpos_button_rectangle,0,sizeof(int32_t)*3*3);
	memset(texpos_button_rectangle_selected,0,sizeof(int32_t)*3*3);
	memset(texpos_button_rectangle_light,0,sizeof(int32_t)*3*3);
	memset(texpos_button_rectangle_dark,0,sizeof(int32_t)*3*3);
	memset(texpos_button_rectangle_divider,0,sizeof(int32_t)*3);
	memset(texpos_button_picture_box,0,sizeof(int32_t)*3*3);
	memset(texpos_button_picture_box_selected,0,sizeof(int32_t)*3*3);
	memset(texpos_button_picture_box_highlighted,0,sizeof(int32_t)*3*3);
	memset(texpos_button_picture_box_sel_highlighted,0,sizeof(int32_t)*3*3);
	memset(texpos_button_picture_box_light,0,sizeof(int32_t)*3*3);
	memset(texpos_button_picture_box_dark,0,sizeof(int32_t)*3*3);
	memset(texpos_button_add,0,sizeof(int32_t)*3*3);
	memset(texpos_button_add_hover,0,sizeof(int32_t)*3*3);
	memset(texpos_button_add_pressed,0,sizeof(int32_t)*3*3);
	memset(texpos_button_add_invalid,0,sizeof(int32_t)*3*3);
	memset(texpos_button_subtract,0,sizeof(int32_t)*3*3);
	memset(texpos_button_subtract_hover,0,sizeof(int32_t)*3*3);
	memset(texpos_button_subtract_pressed,0,sizeof(int32_t)*3*3);
	memset(texpos_button_subtract_invalid,0,sizeof(int32_t)*3*3);
	memset(texpos_button_expander_closed,0,sizeof(int32_t)*2*3);
	memset(texpos_button_expander_open,0,sizeof(int32_t)*2*3);
	memset(texpos_button_category_rectangle,0,sizeof(int32_t)*3*3);
	memset(texpos_button_category_rectangle_selected,0,sizeof(int32_t)*3*3);
	memset(texpos_button_category_rectangle_on,0,sizeof(int32_t)*3*3);
	memset(texpos_button_category_rectangle_on_selected,0,sizeof(int32_t)*3*3);
	memset(texpos_button_category_rectangle_off,0,sizeof(int32_t)*3*3);
	memset(texpos_button_category_rectangle_off_selected,0,sizeof(int32_t)*3*3);
	memset(texpos_button_filter,0,sizeof(int32_t)*6*3);
	memset(texpos_button_filter_no_mag_right,0,sizeof(int32_t)*3);
	memset(texpos_button_filter_name,0,sizeof(int32_t)*4*3);
	memset(texpos_button_main,0,sizeof(int32_t)*INTERFACE_BUTTON_MAINNUM*4*3);
	memset(texpos_button_small,0,sizeof(int32_t)*INTERFACE_BUTTON_SMALLNUM*2*2);

	memset(texpos_calendar_month,0,sizeof(int32_t)*12*3);
	memset(texpos_calendar_day_past,0,sizeof(int32_t)*3);
	memset(texpos_calendar_day_current,0,sizeof(int32_t)*3);
	memset(texpos_calendar_day_future,0,sizeof(int32_t)*3);

	memset(texpos_slider_background,0,sizeof(int32_t)*3*3);
	memset(texpos_slider,0,sizeof(int32_t)*2*3);
	memset(texpos_slider_hover,0,sizeof(int32_t)*2*3);

	memset(texpos_scrollbar,0,sizeof(int32_t)*2*3);
	memset(texpos_scrollbar_up_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_up_pressed,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_down_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_down_pressed,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_small_scroller,0,sizeof(int32_t)*2*2);
	memset(texpos_scrollbar_small_scroller_hover,0,sizeof(int32_t)*2*2);
	memset(texpos_scrollbar_top_scroller,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_top_scroller_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_bottom_scroller,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_bottom_scroller_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_blank_scroller,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_blank_scroller_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_center_scroller,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_center_scroller_hover,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_offcenter_scroller,0,sizeof(int32_t)*2*2);
	memset(texpos_scrollbar_offcenter_scroller_hover,0,sizeof(int32_t)*2*2);
	memset(texpos_scrollbar_sky,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_ground,0,sizeof(int32_t)*2);
	memset(texpos_scrollbar_underground,0,sizeof(int32_t)*2);

	memset(texpos_sort_ascending_active,0,sizeof(int32_t)*4);
	memset(texpos_sort_ascending_inactive,0,sizeof(int32_t)*4);
	memset(texpos_sort_descending_active,0,sizeof(int32_t)*4);
	memset(texpos_sort_descending_inactive,0,sizeof(int32_t)*4);
	memset(texpos_sort_text_active,0,sizeof(int32_t)*3);
	memset(texpos_sort_text_inactive,0,sizeof(int32_t)*3);
	texpos_interior_border_n_s_w_e=0;
	texpos_interior_border_n_w_e=0;
	texpos_interior_border_s_w_e=0;
	texpos_interior_border_w_e=0;
	texpos_interior_border_n_s=0;
	texpos_interface_background=0;
	memset(texpos_button_horizontal_option_left_ornament,0,sizeof(int32_t)*4*3);
	memset(texpos_button_horizontal_option_active,0,sizeof(int32_t)*3*3);
	memset(texpos_button_horizontal_option_inactive,0,sizeof(int32_t)*3*3);
	memset(texpos_button_horizontal_option_remove,0,sizeof(int32_t)*3*3);
	memset(texpos_button_horizontal_option_confirm,0,sizeof(int32_t)*3*3);
	memset(texpos_button_horizontal_option_right_ornament,0,sizeof(int32_t)*4*3);
	memset(texpos_siege_light,0,sizeof(int32_t)*3*3);
	memset(texpos_diplomacy_light,0,sizeof(int32_t)*3*3);
	memset(texpos_petitions_light,0,sizeof(int32_t)*3*3);
	memset(texpos_grid_cell_inactive,0,sizeof(int32_t)*3*3);
	memset(texpos_grid_cell_active,0,sizeof(int32_t)*3*3);
	memset(texpos_grid_cell_button,0,sizeof(int32_t)*3*3);
	memset(texpos_type_filter_left,0,sizeof(int32_t)*3);
	memset(texpos_type_filter_right,0,sizeof(int32_t)*3);
	memset(texpos_type_filter_text,0,sizeof(int32_t)*3);
	memset(texpos_pinned,0,sizeof(int32_t)*2);
	memset(texpos_not_pinned,0,sizeof(int32_t)*2);
	memset(texpos_button_wrestle_right,0,sizeof(int32_t)*3);
	memset(texpos_button_wrestle_equal,0,sizeof(int32_t)*3);
	memset(texpos_button_wrestle_left,0,sizeof(int32_t)*3);
	memset(texpos_button_adventure_tactical_mode_on,0,sizeof(int32_t)*4*2);
	memset(texpos_button_adventure_tactical_mode_off,0,sizeof(int32_t)*4*2);

	memset(texpos_adventure_log_pinned_active,0,sizeof(int32_t)*3*3);
	memset(texpos_adventure_log_pinned_inactive,0,sizeof(int32_t)*3*3);
	memset(texpos_adventure_log_item_active,0,sizeof(int32_t)*3*3);
	memset(texpos_adventure_log_item_inactive,0,sizeof(int32_t)*3*3);

	memset(texpos_button_announcement_open_all_announcements,0,sizeof(int32_t)*3*3);
	memset(texpos_button_announcement_not_pausing_on_new_report,0,sizeof(int32_t)*3*3);
	memset(texpos_button_announcement_pausing_on_new_report,0,sizeof(int32_t)*3*3);
	memset(texpos_button_announcement_open_from_main,0,sizeof(int32_t)*3*3);

	memset(texpos_button_quality_up,0,sizeof(int32_t)*3*3);
	memset(texpos_button_quality_down,0,sizeof(int32_t)*3*3);
	memset(texpos_button_quality_up_inactive,0,sizeof(int32_t)*3*3);
	memset(texpos_button_quality_down_inactive,0,sizeof(int32_t)*3*3);

	memset(texpos_button_stocks_recenter,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_view_item,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_forbid,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_forbid_active,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_dump,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_dump_active,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_melt,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_melt_active,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_hide,0,sizeof(int32_t)*3*3);
	memset(texpos_button_stocks_hide_active,0,sizeof(int32_t)*3*3);

	memset(texpos_button_short_forbid,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_forbid_active,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_dump,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_dump_active,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_melt,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_melt_active,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_hide,0,sizeof(int32_t)*3*2);
	memset(texpos_button_short_hide_active,0,sizeof(int32_t)*3*2);

	memset(texpos_building_short_item_task,0,sizeof(int32_t)*3*2);
	memset(texpos_building_item_task,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_incorporated,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_trade,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_animal,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_bait,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_loaded,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_dead,0,sizeof(int32_t)*3*3);
	memset(texpos_building_item_other,0,sizeof(int32_t)*3*3);

	memset(texpos_building_jobs_repeat,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_repeat_active,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_do_now,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_do_now_active,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_suspended,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_suspended_active,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_priority_up,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_remove,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_active,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_quota,0,sizeof(int32_t)*3*3);
	memset(texpos_building_jobs_remove_worker,0,sizeof(int32_t)*3*3);

	memset(texpos_button_assign_trade,0,sizeof(int32_t)*3*3*INTERFACE_BUTTON_ASSIGN_TRADENUM);
	memset(texpos_button_building_info,0,sizeof(int32_t)*3*3*INTERFACE_BUTTON_BUILDING_INFONUM);
	memset(texpos_button_building_sheet,0,sizeof(int32_t)*4*3*INTERFACE_BUTTON_BUILDING_SHEETNUM);
	memset(texpos_button_unit_sheet,0,sizeof(int32_t)*3*3*INTERFACE_BUTTON_UNIT_SHEETNUM);
	memset(texpos_button_large_unit_sheet,0,sizeof(int32_t)*4*3*INTERFACE_BUTTON_LARGE_UNIT_SHEETNUM);
	memset(texpos_button_pets_livestock,0,sizeof(int32_t)*3*3*INTERFACE_BUTTON_PETS_LIVESTOCKNUM);
	memset(texpos_button_inventory_item,0,sizeof(int32_t)*3*3*INTERFACE_BUTTON_INVENTORY_ITEMNUM);

	memset(texpos_adventure_travel_dir,0,sizeof(int32_t)*INTERFACE_ADVENTURE_TRAVEL_DIRNUM);

	texpos_skill_progress_bar_left_full=0;
	texpos_skill_progress_bar_mid_full=0;
	texpos_skill_progress_bar_right_full=0;
	texpos_skill_progress_bar_left_half=0;
	texpos_skill_progress_bar_mid_half=0;
	texpos_skill_progress_bar_right_half=0;
	texpos_skill_progress_bar_left_empty=0;
	texpos_skill_progress_bar_mid_empty=0;
	texpos_skill_progress_bar_right_empty=0;

	memset(texpos_moon_weather,0,sizeof(int32_t)*INTERFACE_MOON_WEATHERNUM);
	memset(texpos_announcement_alert,0,sizeof(int32_t)*ANNOUNCEMENT_ALERTNUM);
	memset(texpos_unit_status,0,sizeof(int32_t)*UNIT_STATUSNUM);
	memset(texpos_side_indicator,0,sizeof(int32_t)*SIDE_INDICATORNUM);

	texpos_default_portrait_background=0;
	texpos_chosen_portrait_frame=0;
	texpos_hero_portrait_frame=0;
	texpos_default_portrait_frame=0;
	texpos_portrait_chosen=0;
	texpos_portrait_hero=0;
	texpos_portrait_ordinary=0;
	texpos_portrait_chosen_selected=0;
	texpos_portrait_hero_selected=0;
	texpos_portrait_ordinary_selected=0;
	texpos_default_portrait_background_selected=0;
	texpos_portrait_left=0;
	texpos_portrait_right=0;
	texpos_portrait_add_active=0;
	texpos_portrait_add_inactive=0;
	texpos_portrait_frame_deity_bad=0;
	texpos_portrait_frame_deity_normal=0;
	texpos_portrait_frame_deity_good=0;

	texpos_vision_shadow_n=0;
	texpos_vision_shadow_s=0;
	texpos_vision_shadow_w=0;
	texpos_vision_shadow_e=0;
	texpos_vision_shadow_n_w=0;
	texpos_vision_shadow_n_e=0;
	texpos_vision_shadow_s_w=0;
	texpos_vision_shadow_s_e=0;
	texpos_vision_shadow_n_s_w=0;
	texpos_vision_shadow_n_s_e=0;
	texpos_vision_shadow_n_w_e=0;
	texpos_vision_shadow_s_w_e=0;
	texpos_vision_shadow_n_s_w_e=0;
	texpos_vision_shadow_nw=0;
	texpos_vision_shadow_ne=0;
	texpos_vision_shadow_sw=0;
	texpos_vision_shadow_se=0;
	texpos_vision_shadow_n_s=0;
	texpos_vision_shadow_w_e=0;
	texpos_vision_shadow_n_sw_se=0;
	texpos_vision_shadow_n_sw=0;
	texpos_vision_shadow_n_se=0;
	texpos_vision_shadow_s_nw_ne=0;
	texpos_vision_shadow_s_nw=0;
	texpos_vision_shadow_s_ne=0;
	texpos_vision_shadow_w_ne_se=0;
	texpos_vision_shadow_w_ne=0;
	texpos_vision_shadow_w_se=0;
	texpos_vision_shadow_e_nw_sw=0;
	texpos_vision_shadow_e_nw=0;
	texpos_vision_shadow_e_sw=0;
	texpos_vision_shadow_n_w_se=0;
	texpos_vision_shadow_n_e_sw=0;
	texpos_vision_shadow_s_w_ne=0;
	texpos_vision_shadow_s_e_nw=0;
	texpos_vision_shadow_nw_ne=0;
	texpos_vision_shadow_nw_sw=0;
	texpos_vision_shadow_nw_se=0;
	texpos_vision_shadow_ne_sw=0;
	texpos_vision_shadow_ne_se=0;
	texpos_vision_shadow_sw_se=0;
	texpos_vision_shadow_nw_ne_sw=0;
	texpos_vision_shadow_nw_ne_se=0;
	texpos_vision_shadow_nw_sw_se=0;
	texpos_vision_shadow_ne_sw_se=0;
	texpos_vision_shadow_nw_ne_sw_se=0;

	multilevel_ramp_with_wall_nsew=0;
	multilevel_ramp_with_wall_nse=0;
	multilevel_ramp_with_wall_nsw=0;
	multilevel_ramp_with_wall_nwe=0;
	multilevel_ramp_with_wall_swe=0;
	multilevel_ramp_with_wall_n_s=0;
	multilevel_ramp_with_wall_n_w=0;
	multilevel_ramp_with_wall_n_e=0;
	multilevel_ramp_with_wall_s_w=0;
	multilevel_ramp_with_wall_s_e=0;
	multilevel_ramp_with_wall_w_e=0;
	multilevel_ramp_with_wall_n=0;
	multilevel_ramp_with_wall_s=0;
	multilevel_ramp_with_wall_w=0;
	multilevel_ramp_with_wall_e=0;
	multilevel_ramp_with_wall_nw=0;
	multilevel_ramp_with_wall_sw=0;
	multilevel_ramp_with_wall_se=0;
	multilevel_ramp_with_wall_ne=0;
	multilevel_ramp_with_wall_nw_ne=0;
	multilevel_ramp_with_wall_sw_se=0;
	multilevel_ramp_with_wall_nw_sw=0;
	multilevel_ramp_with_wall_ne_se=0;
	multilevel_ramp_other=0;
	multilevel_ramp_with_wall_s_ne=0;
	multilevel_ramp_with_wall_w_se=0;
	multilevel_ramp_with_wall_n_sw=0;
	multilevel_ramp_with_wall_e_nw=0;
	multilevel_ramp_with_wall_e_sw=0;
	multilevel_ramp_with_wall_s_nw=0;
	multilevel_ramp_with_wall_w_ne=0;
	multilevel_ramp_with_wall_n_se=0;
	multilevel_ramp_with_wall_s_e_nw=0;
	multilevel_ramp_with_wall_s_w_ne=0;
	multilevel_ramp_with_wall_n_w_se=0;
	multilevel_ramp_with_wall_n_e_sw=0;
	multilevel_ramp_with_wall_n_sw_se=0;
	multilevel_ramp_with_wall_e_nw_sw=0;
	multilevel_ramp_with_wall_s_nw_ne=0;
	multilevel_ramp_with_wall_w_ne_se=0;
	multilevel_ramp_with_wall_nw_se=0;
	multilevel_ramp_with_wall_ne_sw=0;
	multilevel_ramp_with_wall_nw_ne_sw_se=0;
	multilevel_ramp_with_wall_nw_ne_sw=0;
	multilevel_ramp_with_wall_nw_ne_se=0;
	multilevel_ramp_with_wall_nw_sw_se=0;
	multilevel_ramp_with_wall_ne_sw_se=0;

	{int32_t a,b,c;
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_carpenter_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_farmer_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_mason_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_craftsman_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_jeweler_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_metalsmith_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_lavamill_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_bowyer_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_mechanic_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)workshop_siege_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_butcher_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_leather_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_tanner_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_clothes_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_fishery_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_still_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_loom_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)workshop_kennel_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_kitchen_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_ashery_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_dyer_texpos[a][b][c]=0;}}

	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_carpenter_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_farmer_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_mason_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_craftsman_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_jeweler_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_metalsmith_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_lavamill_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_bowyer_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_mechanic_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_butcher_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_leather_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_tanner_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_clothes_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_fishery_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_still_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_loom_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_kitchen_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_ashery_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)workshop_dyer_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)workshop_siege_texpos_overlay[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)workshop_kennel_texpos_overlay[a][b][c]=0;}}

	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)trade_depot_texpos[a][b][c]=0;}}
	for(a=0;a<4;++a){for(b=0;b<5;++b){for(c=0;c<6;++c)trade_depot_texpos_overlay[a][b][c]=0;}}
	int32_t find;
	for(find=0;find<7;++find){for(a=0;a<4;++a){for(b=0;b<3;++b){for(c=0;c<4;++c)furnace_texpos[find][a][b][c]=0;furnace_texpos_overlay[find][a][b][c]=0;}}}
	for(a=0;a<2;++a){for(b=0;b<2;++b)workshop_quern_texpos[a][b]=0;}
	for(a=0;a<2;++a){for(b=0;b<2;++b)workshop_millstone_texpos[a][b]=0;}
	for(a=0;a<2;++a)workshop_millstone_turning_texpos[a]=0;
	for(a=0;a<PCG_LAYERINGNUM;++a)
		{
		for(b=0;b<3;++b){for(c=0;c<2;++c)texpos_pcg[a][b][c]=0;}
		}
	}

	texpos_stone_track_n=0;
	texpos_stone_track_s=0;
	texpos_stone_track_w=0;
	texpos_stone_track_e=0;
	texpos_stone_track_ns=0;
	texpos_stone_track_nw=0;
	texpos_stone_track_ne=0;
	texpos_stone_track_sw=0;
	texpos_stone_track_se=0;
	texpos_stone_track_we=0;
	texpos_stone_track_nsw=0;
	texpos_stone_track_nse=0;
	texpos_stone_track_nwe=0;
	texpos_stone_track_swe=0;
	texpos_stone_track_nswe=0;
	texpos_constructed_stone_track_n=0;
	texpos_constructed_stone_track_s=0;
	texpos_constructed_stone_track_w=0;
	texpos_constructed_stone_track_e=0;
	texpos_constructed_stone_track_ns=0;
	texpos_constructed_stone_track_nw=0;
	texpos_constructed_stone_track_ne=0;
	texpos_constructed_stone_track_sw=0;
	texpos_constructed_stone_track_se=0;
	texpos_constructed_stone_track_we=0;
	texpos_constructed_stone_track_nsw=0;
	texpos_constructed_stone_track_nse=0;
	texpos_constructed_stone_track_nwe=0;
	texpos_constructed_stone_track_swe=0;
	texpos_constructed_stone_track_nswe=0;
	texpos_constructed_wood_track_n=0;
	texpos_constructed_wood_track_s=0;
	texpos_constructed_wood_track_w=0;
	texpos_constructed_wood_track_e=0;
	texpos_constructed_wood_track_ns=0;
	texpos_constructed_wood_track_nw=0;
	texpos_constructed_wood_track_ne=0;
	texpos_constructed_wood_track_sw=0;
	texpos_constructed_wood_track_se=0;
	texpos_constructed_wood_track_we=0;
	texpos_constructed_wood_track_nsw=0;
	texpos_constructed_wood_track_nse=0;
	texpos_constructed_wood_track_nwe=0;
	texpos_constructed_wood_track_swe=0;
	texpos_constructed_wood_track_nswe=0;

	texpos_track_stop_stone_n=0;
	texpos_track_stop_stone_s=0;
	texpos_track_stop_stone_w=0;
	texpos_track_stop_stone_e=0;
	texpos_track_stop_stone_ns=0;
	texpos_track_stop_stone_nw=0;
	texpos_track_stop_stone_ne=0;
	texpos_track_stop_stone_sw=0;
	texpos_track_stop_stone_se=0;
	texpos_track_stop_stone_we=0;
	texpos_track_stop_stone_nsw=0;
	texpos_track_stop_stone_nse=0;
	texpos_track_stop_stone_nwe=0;
	texpos_track_stop_stone_swe=0;
	texpos_track_stop_stone_nswe=0;
	texpos_track_stop_wood_n=0;
	texpos_track_stop_wood_s=0;
	texpos_track_stop_wood_w=0;
	texpos_track_stop_wood_e=0;
	texpos_track_stop_wood_ns=0;
	texpos_track_stop_wood_nw=0;
	texpos_track_stop_wood_ne=0;
	texpos_track_stop_wood_sw=0;
	texpos_track_stop_wood_se=0;
	texpos_track_stop_wood_we=0;
	texpos_track_stop_wood_nsw=0;
	texpos_track_stop_wood_nse=0;
	texpos_track_stop_wood_nwe=0;
	texpos_track_stop_wood_swe=0;
	texpos_track_stop_wood_nswe=0;

	int32_t m;
	for(m=0;m<2;++m)
		{
		texpos_rollers_stone_n[m]=0;
		texpos_rollers_stone_s[m]=0;
		texpos_rollers_stone_w[m]=0;
		texpos_rollers_stone_e[m]=0;
		texpos_rollers_stone_ns[m]=0;
		texpos_rollers_stone_nw[m]=0;
		texpos_rollers_stone_ne[m]=0;
		texpos_rollers_stone_sw[m]=0;
		texpos_rollers_stone_se[m]=0;
		texpos_rollers_stone_we[m]=0;
		texpos_rollers_stone_nsw[m]=0;
		texpos_rollers_stone_nse[m]=0;
		texpos_rollers_stone_nwe[m]=0;
		texpos_rollers_stone_swe[m]=0;
		texpos_rollers_stone_nswe[m]=0;
		texpos_rollers_wood_n[m]=0;
		texpos_rollers_wood_s[m]=0;
		texpos_rollers_wood_w[m]=0;
		texpos_rollers_wood_e[m]=0;
		texpos_rollers_wood_ns[m]=0;
		texpos_rollers_wood_nw[m]=0;
		texpos_rollers_wood_ne[m]=0;
		texpos_rollers_wood_sw[m]=0;
		texpos_rollers_wood_se[m]=0;
		texpos_rollers_wood_we[m]=0;
		texpos_rollers_wood_nsw[m]=0;
		texpos_rollers_wood_nse[m]=0;
		texpos_rollers_wood_nwe[m]=0;
		texpos_rollers_wood_swe[m]=0;
		texpos_rollers_wood_nswe[m]=0;
		}

	water_texpos=0;
	for(m=0;m<3;++m)magma_texpos[m]=0;
	for(m=0;m<4;++m)
		{
		int32_t mi;
		for(mi=0;mi<9;++mi)grass_texpos[m][mi]=0;
		}
	for(m=0;m<32;++m)
		{
		int32_t mi;
		for(mi=0;mi<9;++mi)custom_grass_edge_texpos[m][mi]=0;
		}
	for(m=0;m<8;++m)
		{
		underwater_texpos[m]=0;
		undermagma_texpos[m]=0;
		underwater_label_texpos[m]=0;
		undermagma_label_texpos[m]=0;
		}
	{int32_t mx,my;
	for(mx=0;mx<3;++mx)
		{
		for(my=0;my<3;++my)
			{
			underwater_edge_texpos[mx][my]=0;
			undermagma_edge_texpos[mx][my]=0;
			}
		}}
	texpos_shrub=0;
	texpos_sapling=0;
	for(m=0;m<TREE_WOOD_TILENUM;++m)texpos_tree_wood_tile[m]=0;
	texpos_overleaves_trunk_n_s_e=0;
	texpos_overleaves_trunk_n_s_w=0;
	texpos_overleaves_trunk_n_w_e=0;
	texpos_overleaves_trunk_s_w_e=0;
	texpos_overleaves_trunk_n_w=0;
	texpos_overleaves_trunk_n_e=0;
	texpos_overleaves_trunk_s_w=0;
	texpos_overleaves_trunk_s_e=0;
	texpos_overleaves_trunk_n_s=0;
	texpos_overleaves_trunk_e_w=0;
	texpos_overleaves_trunk_n=0;
	texpos_overleaves_trunk_s=0;
	texpos_overleaves_trunk_w=0;
	texpos_overleaves_trunk_e=0;
	texpos_overleaves_trunk_n_s_e_w=0;
	texpos_tree_overleaves_heavy_branch_n=0;
	texpos_tree_overleaves_heavy_branch_s=0;
	texpos_tree_overleaves_heavy_branch_w=0;
	texpos_tree_overleaves_heavy_branch_e=0;
	texpos_tree_overleaves_heavy_branch_ns=0;
	texpos_tree_overleaves_heavy_branch_ew=0;
	texpos_tree_overleaves_heavy_branch_nw=0;
	texpos_tree_overleaves_heavy_branch_ne=0;
	texpos_tree_overleaves_heavy_branch_sw=0;
	texpos_tree_overleaves_heavy_branch_se=0;
	texpos_tree_overleaves_heavy_branch_nse=0;
	texpos_tree_overleaves_heavy_branch_nsw=0;
	texpos_tree_overleaves_heavy_branch_new=0;
	texpos_tree_overleaves_heavy_branch_sew=0;
	texpos_tree_overleaves_heavy_branch_nsew=0;
	texpos_tree_twigs=0;
	{int32_t tx,ty;
	for(tx=0;tx<3;++tx)
		{
		for(ty=0;ty<3;++ty)
			{
			texpos_tree_core_trunk[tx][ty]=0;
			texpos_tree_core_shadow[tx][ty]=0;
			}
		}}
	{int32_t v;
	for(v=0;v<4;++v)texpos_tree_twigs_full[v]=0;}
	texpos_tree_twigs_full_n=0;
	texpos_tree_twigs_full_s=0;
	texpos_tree_twigs_full_w=0;
	texpos_tree_twigs_full_e=0;
	texpos_tree_twigs_full_nw=0;
	texpos_tree_twigs_full_ne=0;
	texpos_tree_twigs_full_sw=0;
	texpos_tree_twigs_full_se=0;
	texpos_tree_twigs_n_s_w_e=0;
	texpos_tree_twigs_s_w_e=0;
	texpos_tree_twigs_n_w_e=0;
	texpos_tree_twigs_n_s_e=0;
	texpos_tree_twigs_n_s_w=0;
	texpos_tree_twigs_n_s=0;
	texpos_tree_twigs_n_w=0;
	texpos_tree_twigs_n_e=0;
	texpos_tree_twigs_s_w=0;
	texpos_tree_twigs_s_e=0;
	texpos_tree_twigs_w_e=0;
	texpos_tree_twigs_n=0;
	texpos_tree_twigs_s=0;
	texpos_tree_twigs_w=0;
	texpos_tree_twigs_e=0;

	memset(texpos_tree_twigs_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_autumn,0,sizeof(int32_t)*4*3);
	memset(texpos_tree_twigs_full_n_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_nw_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_ne_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_sw_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_se_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_n_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_s_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_w_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_e_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ns_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ew_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nw_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ne_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sw_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_se_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nse_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsw_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_new_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sew_autumn,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsew_autumn,0,sizeof(int32_t)*3);

	memset(texpos_tree_twigs_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_fruit,0,sizeof(int32_t)*4*3);
	memset(texpos_tree_twigs_full_n_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_nw_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_ne_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_sw_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_se_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_n_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_s_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_w_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_e_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ns_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ew_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nw_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ne_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sw_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_se_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nse_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsw_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_new_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sew_fruit,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsew_fruit,0,sizeof(int32_t)*3);

	memset(texpos_tree_twigs_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_flower,0,sizeof(int32_t)*4*3);
	memset(texpos_tree_twigs_full_n_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_nw_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_ne_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_sw_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_full_se_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_n_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_twigs_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_overleaves_trunk_n_s_e_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_n_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_s_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_w_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_e_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ns_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ew_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nw_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_ne_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sw_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_se_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nse_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsw_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_new_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_sew_flower,0,sizeof(int32_t)*3);
	memset(texpos_tree_overleaves_heavy_branch_nsew_flower,0,sizeof(int32_t)*3);

	texpos_tree_cap_pillar=0;
	texpos_tree_cap_wall_thick_n=0;
	texpos_tree_cap_wall_thick_s=0;
	texpos_tree_cap_wall_thick_e=0;
	texpos_tree_cap_wall_thick_w=0;
	texpos_tree_cap_wall_thick_nw=0;
	texpos_tree_cap_wall_thick_ne=0;
	texpos_tree_cap_wall_thick_sw=0;
	texpos_tree_cap_wall_thick_se=0;
	texpos_tree_cap_wall_n_s_e=0;
	texpos_tree_cap_wall_n_s_w=0;
	texpos_tree_cap_wall_n_w_e=0;
	texpos_tree_cap_wall_s_w_e=0;
	texpos_tree_cap_wall_n_w=0;
	texpos_tree_cap_wall_n_e=0;
	texpos_tree_cap_wall_s_w=0;
	texpos_tree_cap_wall_s_e=0;
	texpos_tree_cap_wall_n_s=0;
	texpos_tree_cap_wall_w_e=0;
	texpos_tree_cap_wall_n=0;
	texpos_tree_cap_wall_s=0;
	texpos_tree_cap_wall_w=0;
	texpos_tree_cap_wall_e=0;
	texpos_tree_cap_wall_n_s_w_e=0;
	texpos_tree_cap_thick_interior=0;
	texpos_tree_cap_floor_1=0;
	texpos_tree_cap_floor_2=0;
	texpos_tree_cap_floor_3=0;
	texpos_tree_cap_floor_4=0;
	texpos_tree_cap_ramp=0;
	texpos_tree_cap_ramp_n_s_w_e=0;
	texpos_tree_cap_ramp_n_s_e=0;
	texpos_tree_cap_ramp_n_s_w=0;
	texpos_tree_cap_ramp_n_w_e=0;
	texpos_tree_cap_ramp_s_w_e=0;
	texpos_tree_cap_ramp_n_w=0;
	texpos_tree_cap_ramp_n_e=0;
	texpos_tree_cap_ramp_s_w=0;
	texpos_tree_cap_ramp_s_e=0;
	texpos_tree_cap_ramp_n_s=0;
	texpos_tree_cap_ramp_w_e=0;
	texpos_tree_cap_ramp_n=0;
	texpos_tree_cap_ramp_s=0;
	texpos_tree_cap_ramp_w=0;
	texpos_tree_cap_ramp_e=0;

	overlay_ramp_with_wall_nsew=0;
	overlay_ramp_with_wall_nse=0;
	overlay_ramp_with_wall_nsw=0;
	overlay_ramp_with_wall_nwe=0;
	overlay_ramp_with_wall_swe=0;
	overlay_ramp_with_wall_n_s=0;
	overlay_ramp_with_wall_n_w=0;
	overlay_ramp_with_wall_n_e=0;
	overlay_ramp_with_wall_s_w=0;
	overlay_ramp_with_wall_s_e=0;
	overlay_ramp_with_wall_w_e=0;
	overlay_ramp_with_wall_n=0;
	overlay_ramp_with_wall_s=0;
	overlay_ramp_with_wall_w=0;
	overlay_ramp_with_wall_e=0;
	overlay_ramp_with_wall_nw=0;
	overlay_ramp_with_wall_sw=0;
	overlay_ramp_with_wall_se=0;
	overlay_ramp_with_wall_ne=0;
	overlay_ramp_with_wall_nw_ne=0;
	overlay_ramp_with_wall_sw_se=0;
	overlay_ramp_with_wall_nw_sw=0;
	overlay_ramp_with_wall_ne_se=0;
	overlay_ramp_other=0;
	overlay_ramp_with_wall_s_ne=0;
	overlay_ramp_with_wall_w_se=0;
	overlay_ramp_with_wall_n_sw=0;
	overlay_ramp_with_wall_e_nw=0;
	overlay_ramp_with_wall_e_sw=0;
	overlay_ramp_with_wall_s_nw=0;
	overlay_ramp_with_wall_w_ne=0;
	overlay_ramp_with_wall_n_se=0;
	overlay_ramp_with_wall_s_e_nw=0;
	overlay_ramp_with_wall_s_w_ne=0;
	overlay_ramp_with_wall_n_w_se=0;
	overlay_ramp_with_wall_n_e_sw=0;
	overlay_ramp_with_wall_n_sw_se=0;
	overlay_ramp_with_wall_e_nw_sw=0;
	overlay_ramp_with_wall_s_nw_ne=0;
	overlay_ramp_with_wall_w_ne_se=0;
	overlay_ramp_with_wall_nw_se=0;
	overlay_ramp_with_wall_ne_sw=0;
	overlay_ramp_with_wall_nw_ne_sw_se=0;
	overlay_ramp_with_wall_nw_ne_sw=0;
	overlay_ramp_with_wall_nw_ne_se=0;
	overlay_ramp_with_wall_nw_sw_se=0;
	overlay_ramp_with_wall_ne_sw_se=0;

	grass_ramp_with_wall_nsew=0;
	grass_ramp_with_wall_nse=0;
	grass_ramp_with_wall_nsw=0;
	grass_ramp_with_wall_nwe=0;
	grass_ramp_with_wall_swe=0;
	grass_ramp_with_wall_n_s=0;
	grass_ramp_with_wall_n_w=0;
	grass_ramp_with_wall_n_e=0;
	grass_ramp_with_wall_s_w=0;
	grass_ramp_with_wall_s_e=0;
	grass_ramp_with_wall_w_e=0;
	grass_ramp_with_wall_n=0;
	grass_ramp_with_wall_s=0;
	grass_ramp_with_wall_w=0;
	grass_ramp_with_wall_e=0;
	grass_ramp_with_wall_nw=0;
	grass_ramp_with_wall_sw=0;
	grass_ramp_with_wall_se=0;
	grass_ramp_with_wall_ne=0;
	grass_ramp_with_wall_nw_ne=0;
	grass_ramp_with_wall_sw_se=0;
	grass_ramp_with_wall_nw_sw=0;
	grass_ramp_with_wall_ne_se=0;
	grass_ramp_other=0;
	grass_ramp_with_wall_s_ne=0;
	grass_ramp_with_wall_w_se=0;
	grass_ramp_with_wall_n_sw=0;
	grass_ramp_with_wall_e_nw=0;
	grass_ramp_with_wall_e_sw=0;
	grass_ramp_with_wall_s_nw=0;
	grass_ramp_with_wall_w_ne=0;
	grass_ramp_with_wall_n_se=0;
	grass_ramp_with_wall_s_e_nw=0;
	grass_ramp_with_wall_s_w_ne=0;
	grass_ramp_with_wall_n_w_se=0;
	grass_ramp_with_wall_n_e_sw=0;
	grass_ramp_with_wall_n_sw_se=0;
	grass_ramp_with_wall_e_nw_sw=0;
	grass_ramp_with_wall_s_nw_ne=0;
	grass_ramp_with_wall_w_ne_se=0;
	grass_ramp_with_wall_nw_se=0;
	grass_ramp_with_wall_ne_sw=0;
	grass_ramp_with_wall_nw_ne_sw_se=0;
	grass_ramp_with_wall_nw_ne_sw=0;
	grass_ramp_with_wall_nw_ne_se=0;
	grass_ramp_with_wall_nw_sw_se=0;
	grass_ramp_with_wall_ne_sw_se=0;

	int32_t cg;
	for(cg=0;cg<32;++cg)
		{
		custom_grass_ramp_with_wall_nsew[cg]=0;
		custom_grass_ramp_with_wall_nse[cg]=0;
		custom_grass_ramp_with_wall_nsw[cg]=0;
		custom_grass_ramp_with_wall_nwe[cg]=0;
		custom_grass_ramp_with_wall_swe[cg]=0;
		custom_grass_ramp_with_wall_n_s[cg]=0;
		custom_grass_ramp_with_wall_n_w[cg]=0;
		custom_grass_ramp_with_wall_n_e[cg]=0;
		custom_grass_ramp_with_wall_s_w[cg]=0;
		custom_grass_ramp_with_wall_s_e[cg]=0;
		custom_grass_ramp_with_wall_w_e[cg]=0;
		custom_grass_ramp_with_wall_n[cg]=0;
		custom_grass_ramp_with_wall_s[cg]=0;
		custom_grass_ramp_with_wall_w[cg]=0;
		custom_grass_ramp_with_wall_e[cg]=0;
		custom_grass_ramp_with_wall_nw[cg]=0;
		custom_grass_ramp_with_wall_sw[cg]=0;
		custom_grass_ramp_with_wall_se[cg]=0;
		custom_grass_ramp_with_wall_ne[cg]=0;
		custom_grass_ramp_with_wall_nw_ne[cg]=0;
		custom_grass_ramp_with_wall_sw_se[cg]=0;
		custom_grass_ramp_with_wall_nw_sw[cg]=0;
		custom_grass_ramp_with_wall_ne_se[cg]=0;
		custom_grass_ramp_other[cg]=0;
		custom_grass_ramp_with_wall_s_ne[cg]=0;
		custom_grass_ramp_with_wall_w_se[cg]=0;
		custom_grass_ramp_with_wall_n_sw[cg]=0;
		custom_grass_ramp_with_wall_e_nw[cg]=0;
		custom_grass_ramp_with_wall_e_sw[cg]=0;
		custom_grass_ramp_with_wall_s_nw[cg]=0;
		custom_grass_ramp_with_wall_w_ne[cg]=0;
		custom_grass_ramp_with_wall_n_se[cg]=0;
		custom_grass_ramp_with_wall_s_e_nw[cg]=0;
		custom_grass_ramp_with_wall_s_w_ne[cg]=0;
		custom_grass_ramp_with_wall_n_w_se[cg]=0;
		custom_grass_ramp_with_wall_n_e_sw[cg]=0;
		custom_grass_ramp_with_wall_n_sw_se[cg]=0;
		custom_grass_ramp_with_wall_e_nw_sw[cg]=0;
		custom_grass_ramp_with_wall_s_nw_ne[cg]=0;
		custom_grass_ramp_with_wall_w_ne_se[cg]=0;
		custom_grass_ramp_with_wall_nw_se[cg]=0;
		custom_grass_ramp_with_wall_ne_sw[cg]=0;
		custom_grass_ramp_with_wall_nw_ne_sw_se[cg]=0;
		custom_grass_ramp_with_wall_nw_ne_sw[cg]=0;
		custom_grass_ramp_with_wall_nw_ne_se[cg]=0;
		custom_grass_ramp_with_wall_nw_sw_se[cg]=0;
		custom_grass_ramp_with_wall_ne_sw_se[cg]=0;
		}

	soil_ramp_with_wall_nsew=0;
	soil_ramp_with_wall_nse=0;
	soil_ramp_with_wall_nsw=0;
	soil_ramp_with_wall_nwe=0;
	soil_ramp_with_wall_swe=0;
	soil_ramp_with_wall_n_s=0;
	soil_ramp_with_wall_n_w=0;
	soil_ramp_with_wall_n_e=0;
	soil_ramp_with_wall_s_w=0;
	soil_ramp_with_wall_s_e=0;
	soil_ramp_with_wall_w_e=0;
	soil_ramp_with_wall_n=0;
	soil_ramp_with_wall_s=0;
	soil_ramp_with_wall_w=0;
	soil_ramp_with_wall_e=0;
	soil_ramp_with_wall_nw=0;
	soil_ramp_with_wall_sw=0;
	soil_ramp_with_wall_se=0;
	soil_ramp_with_wall_ne=0;
	soil_ramp_with_wall_nw_ne=0;
	soil_ramp_with_wall_sw_se=0;
	soil_ramp_with_wall_nw_sw=0;
	soil_ramp_with_wall_ne_se=0;
	soil_ramp_other=0;
	soil_ramp_with_wall_s_ne=0;
	soil_ramp_with_wall_w_se=0;
	soil_ramp_with_wall_n_sw=0;
	soil_ramp_with_wall_e_nw=0;
	soil_ramp_with_wall_e_sw=0;
	soil_ramp_with_wall_s_nw=0;
	soil_ramp_with_wall_w_ne=0;
	soil_ramp_with_wall_n_se=0;
	soil_ramp_with_wall_s_e_nw=0;
	soil_ramp_with_wall_s_w_ne=0;
	soil_ramp_with_wall_n_w_se=0;
	soil_ramp_with_wall_n_e_sw=0;
	soil_ramp_with_wall_n_sw_se=0;
	soil_ramp_with_wall_e_nw_sw=0;
	soil_ramp_with_wall_s_nw_ne=0;
	soil_ramp_with_wall_w_ne_se=0;
	soil_ramp_with_wall_nw_se=0;
	soil_ramp_with_wall_ne_sw=0;
	soil_ramp_with_wall_nw_ne_sw_se=0;
	soil_ramp_with_wall_nw_ne_sw=0;
	soil_ramp_with_wall_nw_ne_se=0;
	soil_ramp_with_wall_nw_sw_se=0;
	soil_ramp_with_wall_ne_sw_se=0;

	sand_beige_ramp_with_wall_nsew=0;
	sand_beige_ramp_with_wall_nse=0;
	sand_beige_ramp_with_wall_nsw=0;
	sand_beige_ramp_with_wall_nwe=0;
	sand_beige_ramp_with_wall_swe=0;
	sand_beige_ramp_with_wall_n_s=0;
	sand_beige_ramp_with_wall_n_w=0;
	sand_beige_ramp_with_wall_n_e=0;
	sand_beige_ramp_with_wall_s_w=0;
	sand_beige_ramp_with_wall_s_e=0;
	sand_beige_ramp_with_wall_w_e=0;
	sand_beige_ramp_with_wall_n=0;
	sand_beige_ramp_with_wall_s=0;
	sand_beige_ramp_with_wall_w=0;
	sand_beige_ramp_with_wall_e=0;
	sand_beige_ramp_with_wall_nw=0;
	sand_beige_ramp_with_wall_sw=0;
	sand_beige_ramp_with_wall_se=0;
	sand_beige_ramp_with_wall_ne=0;
	sand_beige_ramp_with_wall_nw_ne=0;
	sand_beige_ramp_with_wall_sw_se=0;
	sand_beige_ramp_with_wall_nw_sw=0;
	sand_beige_ramp_with_wall_ne_se=0;
	sand_beige_ramp_other=0;
	sand_beige_ramp_with_wall_s_ne=0;
	sand_beige_ramp_with_wall_w_se=0;
	sand_beige_ramp_with_wall_n_sw=0;
	sand_beige_ramp_with_wall_e_nw=0;
	sand_beige_ramp_with_wall_e_sw=0;
	sand_beige_ramp_with_wall_s_nw=0;
	sand_beige_ramp_with_wall_w_ne=0;
	sand_beige_ramp_with_wall_n_se=0;
	sand_beige_ramp_with_wall_s_e_nw=0;
	sand_beige_ramp_with_wall_s_w_ne=0;
	sand_beige_ramp_with_wall_n_w_se=0;
	sand_beige_ramp_with_wall_n_e_sw=0;
	sand_beige_ramp_with_wall_n_sw_se=0;
	sand_beige_ramp_with_wall_e_nw_sw=0;
	sand_beige_ramp_with_wall_s_nw_ne=0;
	sand_beige_ramp_with_wall_w_ne_se=0;
	sand_beige_ramp_with_wall_nw_se=0;
	sand_beige_ramp_with_wall_ne_sw=0;
	sand_beige_ramp_with_wall_nw_ne_sw_se=0;
	sand_beige_ramp_with_wall_nw_ne_sw=0;
	sand_beige_ramp_with_wall_nw_ne_se=0;
	sand_beige_ramp_with_wall_nw_sw_se=0;
	sand_beige_ramp_with_wall_ne_sw_se=0;

	sand_black_ramp_with_wall_nsew=0;
	sand_black_ramp_with_wall_nse=0;
	sand_black_ramp_with_wall_nsw=0;
	sand_black_ramp_with_wall_nwe=0;
	sand_black_ramp_with_wall_swe=0;
	sand_black_ramp_with_wall_n_s=0;
	sand_black_ramp_with_wall_n_w=0;
	sand_black_ramp_with_wall_n_e=0;
	sand_black_ramp_with_wall_s_w=0;
	sand_black_ramp_with_wall_s_e=0;
	sand_black_ramp_with_wall_w_e=0;
	sand_black_ramp_with_wall_n=0;
	sand_black_ramp_with_wall_s=0;
	sand_black_ramp_with_wall_w=0;
	sand_black_ramp_with_wall_e=0;
	sand_black_ramp_with_wall_nw=0;
	sand_black_ramp_with_wall_sw=0;
	sand_black_ramp_with_wall_se=0;
	sand_black_ramp_with_wall_ne=0;
	sand_black_ramp_with_wall_nw_ne=0;
	sand_black_ramp_with_wall_sw_se=0;
	sand_black_ramp_with_wall_nw_sw=0;
	sand_black_ramp_with_wall_ne_se=0;
	sand_black_ramp_other=0;
	sand_black_ramp_with_wall_s_ne=0;
	sand_black_ramp_with_wall_w_se=0;
	sand_black_ramp_with_wall_n_sw=0;
	sand_black_ramp_with_wall_e_nw=0;
	sand_black_ramp_with_wall_e_sw=0;
	sand_black_ramp_with_wall_s_nw=0;
	sand_black_ramp_with_wall_w_ne=0;
	sand_black_ramp_with_wall_n_se=0;
	sand_black_ramp_with_wall_s_e_nw=0;
	sand_black_ramp_with_wall_s_w_ne=0;
	sand_black_ramp_with_wall_n_w_se=0;
	sand_black_ramp_with_wall_n_e_sw=0;
	sand_black_ramp_with_wall_n_sw_se=0;
	sand_black_ramp_with_wall_e_nw_sw=0;
	sand_black_ramp_with_wall_s_nw_ne=0;
	sand_black_ramp_with_wall_w_ne_se=0;
	sand_black_ramp_with_wall_nw_se=0;
	sand_black_ramp_with_wall_ne_sw=0;
	sand_black_ramp_with_wall_nw_ne_sw_se=0;
	sand_black_ramp_with_wall_nw_ne_sw=0;
	sand_black_ramp_with_wall_nw_ne_se=0;
	sand_black_ramp_with_wall_nw_sw_se=0;
	sand_black_ramp_with_wall_ne_sw_se=0;

	sand_red_ramp_with_wall_nsew=0;
	sand_red_ramp_with_wall_nse=0;
	sand_red_ramp_with_wall_nsw=0;
	sand_red_ramp_with_wall_nwe=0;
	sand_red_ramp_with_wall_swe=0;
	sand_red_ramp_with_wall_n_s=0;
	sand_red_ramp_with_wall_n_w=0;
	sand_red_ramp_with_wall_n_e=0;
	sand_red_ramp_with_wall_s_w=0;
	sand_red_ramp_with_wall_s_e=0;
	sand_red_ramp_with_wall_w_e=0;
	sand_red_ramp_with_wall_n=0;
	sand_red_ramp_with_wall_s=0;
	sand_red_ramp_with_wall_w=0;
	sand_red_ramp_with_wall_e=0;
	sand_red_ramp_with_wall_nw=0;
	sand_red_ramp_with_wall_sw=0;
	sand_red_ramp_with_wall_se=0;
	sand_red_ramp_with_wall_ne=0;
	sand_red_ramp_with_wall_nw_ne=0;
	sand_red_ramp_with_wall_sw_se=0;
	sand_red_ramp_with_wall_nw_sw=0;
	sand_red_ramp_with_wall_ne_se=0;
	sand_red_ramp_other=0;
	sand_red_ramp_with_wall_s_ne=0;
	sand_red_ramp_with_wall_w_se=0;
	sand_red_ramp_with_wall_n_sw=0;
	sand_red_ramp_with_wall_e_nw=0;
	sand_red_ramp_with_wall_e_sw=0;
	sand_red_ramp_with_wall_s_nw=0;
	sand_red_ramp_with_wall_w_ne=0;
	sand_red_ramp_with_wall_n_se=0;
	sand_red_ramp_with_wall_s_e_nw=0;
	sand_red_ramp_with_wall_s_w_ne=0;
	sand_red_ramp_with_wall_n_w_se=0;
	sand_red_ramp_with_wall_n_e_sw=0;
	sand_red_ramp_with_wall_n_sw_se=0;
	sand_red_ramp_with_wall_e_nw_sw=0;
	sand_red_ramp_with_wall_s_nw_ne=0;
	sand_red_ramp_with_wall_w_ne_se=0;
	sand_red_ramp_with_wall_nw_se=0;
	sand_red_ramp_with_wall_ne_sw=0;
	sand_red_ramp_with_wall_nw_ne_sw_se=0;
	sand_red_ramp_with_wall_nw_ne_sw=0;
	sand_red_ramp_with_wall_nw_ne_se=0;
	sand_red_ramp_with_wall_nw_sw_se=0;
	sand_red_ramp_with_wall_ne_sw_se=0;

	sand_tan_ramp_with_wall_nsew=0;
	sand_tan_ramp_with_wall_nse=0;
	sand_tan_ramp_with_wall_nsw=0;
	sand_tan_ramp_with_wall_nwe=0;
	sand_tan_ramp_with_wall_swe=0;
	sand_tan_ramp_with_wall_n_s=0;
	sand_tan_ramp_with_wall_n_w=0;
	sand_tan_ramp_with_wall_n_e=0;
	sand_tan_ramp_with_wall_s_w=0;
	sand_tan_ramp_with_wall_s_e=0;
	sand_tan_ramp_with_wall_w_e=0;
	sand_tan_ramp_with_wall_n=0;
	sand_tan_ramp_with_wall_s=0;
	sand_tan_ramp_with_wall_w=0;
	sand_tan_ramp_with_wall_e=0;
	sand_tan_ramp_with_wall_nw=0;
	sand_tan_ramp_with_wall_sw=0;
	sand_tan_ramp_with_wall_se=0;
	sand_tan_ramp_with_wall_ne=0;
	sand_tan_ramp_with_wall_nw_ne=0;
	sand_tan_ramp_with_wall_sw_se=0;
	sand_tan_ramp_with_wall_nw_sw=0;
	sand_tan_ramp_with_wall_ne_se=0;
	sand_tan_ramp_other=0;
	sand_tan_ramp_with_wall_s_ne=0;
	sand_tan_ramp_with_wall_w_se=0;
	sand_tan_ramp_with_wall_n_sw=0;
	sand_tan_ramp_with_wall_e_nw=0;
	sand_tan_ramp_with_wall_e_sw=0;
	sand_tan_ramp_with_wall_s_nw=0;
	sand_tan_ramp_with_wall_w_ne=0;
	sand_tan_ramp_with_wall_n_se=0;
	sand_tan_ramp_with_wall_s_e_nw=0;
	sand_tan_ramp_with_wall_s_w_ne=0;
	sand_tan_ramp_with_wall_n_w_se=0;
	sand_tan_ramp_with_wall_n_e_sw=0;
	sand_tan_ramp_with_wall_n_sw_se=0;
	sand_tan_ramp_with_wall_e_nw_sw=0;
	sand_tan_ramp_with_wall_s_nw_ne=0;
	sand_tan_ramp_with_wall_w_ne_se=0;
	sand_tan_ramp_with_wall_nw_se=0;
	sand_tan_ramp_with_wall_ne_sw=0;
	sand_tan_ramp_with_wall_nw_ne_sw_se=0;
	sand_tan_ramp_with_wall_nw_ne_sw=0;
	sand_tan_ramp_with_wall_nw_ne_se=0;
	sand_tan_ramp_with_wall_nw_sw_se=0;
	sand_tan_ramp_with_wall_ne_sw_se=0;

	sand_white_ramp_with_wall_nsew=0;
	sand_white_ramp_with_wall_nse=0;
	sand_white_ramp_with_wall_nsw=0;
	sand_white_ramp_with_wall_nwe=0;
	sand_white_ramp_with_wall_swe=0;
	sand_white_ramp_with_wall_n_s=0;
	sand_white_ramp_with_wall_n_w=0;
	sand_white_ramp_with_wall_n_e=0;
	sand_white_ramp_with_wall_s_w=0;
	sand_white_ramp_with_wall_s_e=0;
	sand_white_ramp_with_wall_w_e=0;
	sand_white_ramp_with_wall_n=0;
	sand_white_ramp_with_wall_s=0;
	sand_white_ramp_with_wall_w=0;
	sand_white_ramp_with_wall_e=0;
	sand_white_ramp_with_wall_nw=0;
	sand_white_ramp_with_wall_sw=0;
	sand_white_ramp_with_wall_se=0;
	sand_white_ramp_with_wall_ne=0;
	sand_white_ramp_with_wall_nw_ne=0;
	sand_white_ramp_with_wall_sw_se=0;
	sand_white_ramp_with_wall_nw_sw=0;
	sand_white_ramp_with_wall_ne_se=0;
	sand_white_ramp_other=0;
	sand_white_ramp_with_wall_s_ne=0;
	sand_white_ramp_with_wall_w_se=0;
	sand_white_ramp_with_wall_n_sw=0;
	sand_white_ramp_with_wall_e_nw=0;
	sand_white_ramp_with_wall_e_sw=0;
	sand_white_ramp_with_wall_s_nw=0;
	sand_white_ramp_with_wall_w_ne=0;
	sand_white_ramp_with_wall_n_se=0;
	sand_white_ramp_with_wall_s_e_nw=0;
	sand_white_ramp_with_wall_s_w_ne=0;
	sand_white_ramp_with_wall_n_w_se=0;
	sand_white_ramp_with_wall_n_e_sw=0;
	sand_white_ramp_with_wall_n_sw_se=0;
	sand_white_ramp_with_wall_e_nw_sw=0;
	sand_white_ramp_with_wall_s_nw_ne=0;
	sand_white_ramp_with_wall_w_ne_se=0;
	sand_white_ramp_with_wall_nw_se=0;
	sand_white_ramp_with_wall_ne_sw=0;
	sand_white_ramp_with_wall_nw_ne_sw_se=0;
	sand_white_ramp_with_wall_nw_ne_sw=0;
	sand_white_ramp_with_wall_nw_ne_se=0;
	sand_white_ramp_with_wall_nw_sw_se=0;
	sand_white_ramp_with_wall_ne_sw_se=0;

	sand_yellow_ramp_with_wall_nsew=0;
	sand_yellow_ramp_with_wall_nse=0;
	sand_yellow_ramp_with_wall_nsw=0;
	sand_yellow_ramp_with_wall_nwe=0;
	sand_yellow_ramp_with_wall_swe=0;
	sand_yellow_ramp_with_wall_n_s=0;
	sand_yellow_ramp_with_wall_n_w=0;
	sand_yellow_ramp_with_wall_n_e=0;
	sand_yellow_ramp_with_wall_s_w=0;
	sand_yellow_ramp_with_wall_s_e=0;
	sand_yellow_ramp_with_wall_w_e=0;
	sand_yellow_ramp_with_wall_n=0;
	sand_yellow_ramp_with_wall_s=0;
	sand_yellow_ramp_with_wall_w=0;
	sand_yellow_ramp_with_wall_e=0;
	sand_yellow_ramp_with_wall_nw=0;
	sand_yellow_ramp_with_wall_sw=0;
	sand_yellow_ramp_with_wall_se=0;
	sand_yellow_ramp_with_wall_ne=0;
	sand_yellow_ramp_with_wall_nw_ne=0;
	sand_yellow_ramp_with_wall_sw_se=0;
	sand_yellow_ramp_with_wall_nw_sw=0;
	sand_yellow_ramp_with_wall_ne_se=0;
	sand_yellow_ramp_other=0;
	sand_yellow_ramp_with_wall_s_ne=0;
	sand_yellow_ramp_with_wall_w_se=0;
	sand_yellow_ramp_with_wall_n_sw=0;
	sand_yellow_ramp_with_wall_e_nw=0;
	sand_yellow_ramp_with_wall_e_sw=0;
	sand_yellow_ramp_with_wall_s_nw=0;
	sand_yellow_ramp_with_wall_w_ne=0;
	sand_yellow_ramp_with_wall_n_se=0;
	sand_yellow_ramp_with_wall_s_e_nw=0;
	sand_yellow_ramp_with_wall_s_w_ne=0;
	sand_yellow_ramp_with_wall_n_w_se=0;
	sand_yellow_ramp_with_wall_n_e_sw=0;
	sand_yellow_ramp_with_wall_n_sw_se=0;
	sand_yellow_ramp_with_wall_e_nw_sw=0;
	sand_yellow_ramp_with_wall_s_nw_ne=0;
	sand_yellow_ramp_with_wall_w_ne_se=0;
	sand_yellow_ramp_with_wall_nw_se=0;
	sand_yellow_ramp_with_wall_ne_sw=0;
	sand_yellow_ramp_with_wall_nw_ne_sw_se=0;
	sand_yellow_ramp_with_wall_nw_ne_sw=0;
	sand_yellow_ramp_with_wall_nw_ne_se=0;
	sand_yellow_ramp_with_wall_nw_sw_se=0;
	sand_yellow_ramp_with_wall_ne_sw_se=0;

	stone_ramp_with_wall_nsew=0;
	stone_ramp_with_wall_nse=0;
	stone_ramp_with_wall_nsw=0;
	stone_ramp_with_wall_nwe=0;
	stone_ramp_with_wall_swe=0;
	stone_ramp_with_wall_n_s=0;
	stone_ramp_with_wall_n_w=0;
	stone_ramp_with_wall_n_e=0;
	stone_ramp_with_wall_s_w=0;
	stone_ramp_with_wall_s_e=0;
	stone_ramp_with_wall_w_e=0;
	stone_ramp_with_wall_n=0;
	stone_ramp_with_wall_s=0;
	stone_ramp_with_wall_w=0;
	stone_ramp_with_wall_e=0;
	stone_ramp_with_wall_nw=0;
	stone_ramp_with_wall_sw=0;
	stone_ramp_with_wall_se=0;
	stone_ramp_with_wall_ne=0;
	stone_ramp_with_wall_nw_ne=0;
	stone_ramp_with_wall_sw_se=0;
	stone_ramp_with_wall_nw_sw=0;
	stone_ramp_with_wall_ne_se=0;
	stone_ramp_other=0;
	stone_ramp_with_wall_s_ne=0;
	stone_ramp_with_wall_w_se=0;
	stone_ramp_with_wall_n_sw=0;
	stone_ramp_with_wall_e_nw=0;
	stone_ramp_with_wall_e_sw=0;
	stone_ramp_with_wall_s_nw=0;
	stone_ramp_with_wall_w_ne=0;
	stone_ramp_with_wall_n_se=0;
	stone_ramp_with_wall_s_e_nw=0;
	stone_ramp_with_wall_s_w_ne=0;
	stone_ramp_with_wall_n_w_se=0;
	stone_ramp_with_wall_n_e_sw=0;
	stone_ramp_with_wall_n_sw_se=0;
	stone_ramp_with_wall_e_nw_sw=0;
	stone_ramp_with_wall_s_nw_ne=0;
	stone_ramp_with_wall_w_ne_se=0;
	stone_ramp_with_wall_nw_se=0;
	stone_ramp_with_wall_ne_sw=0;
	stone_ramp_with_wall_nw_ne_sw_se=0;
	stone_ramp_with_wall_nw_ne_sw=0;
	stone_ramp_with_wall_nw_ne_se=0;
	stone_ramp_with_wall_nw_sw_se=0;
	stone_ramp_with_wall_ne_sw_se=0;

	wall_shadow_straight_n=0;
	wall_shadow_straight_s=0;
	wall_shadow_straight_w=0;
	wall_shadow_straight_e=0;
	wall_shadow_near_n_open_nw=0;
	wall_shadow_near_n_open_ne=0;
	wall_shadow_near_s_open_sw=0;
	wall_shadow_near_s_open_se=0;
	wall_shadow_near_w_open_nw=0;
	wall_shadow_near_w_open_sw=0;
	wall_shadow_near_e_open_ne=0;
	wall_shadow_near_e_open_se=0;
	wall_shadow_corner_nw=0;
	wall_shadow_corner_ne=0;
	wall_shadow_corner_sw=0;
	wall_shadow_corner_se=0;
	wall_shadow_end_wall_n=0;
	wall_shadow_end_wall_s=0;
	wall_shadow_end_wall_w=0;
	wall_shadow_end_wall_e=0;

	ramp_shadow_on_floor_nw_of_corner_se=0;
	ramp_shadow_on_floor_n_of_corner_se=0;
	ramp_shadow_on_floor_n_of_s=0;
	ramp_shadow_on_floor_n_of_corner_sw=0;
	ramp_shadow_on_floor_ne_of_corner_sw=0;
	ramp_shadow_on_ramp_s=0;
	ramp_shadow_on_floor_w_of_corner_se=0;
	ramp_shadow_on_ramp_inside_corner_se=0;
	ramp_shadow_on_ramp_inside_corner_sw=0;
	ramp_shadow_on_floor_e_of_corner_sw=0;
	ramp_shadow_on_floor_w_of_e=0;
	ramp_shadow_on_ramp_e=0;
	ramp_shadow_on_ramp_w=0;
	ramp_shadow_on_floor_e_of_w=0;
	ramp_shadow_on_floor_w_of_corner_ne=0;
	ramp_shadow_on_ramp_inside_corner_ne=0;
	ramp_shadow_on_ramp_inside_corner_nw=0;
	ramp_shadow_on_floor_e_of_corner_nw=0;
	ramp_shadow_on_ramp_n=0;
	ramp_shadow_on_floor_sw_of_corner_ne=0;
	ramp_shadow_on_floor_s_of_corner_ne=0;
	ramp_shadow_on_floor_s_of_n=0;
	ramp_shadow_on_floor_s_of_corner_nw=0;
	ramp_shadow_on_floor_se_of_corner_nw=0;
	ramp_shadow_on_ramp_corner_nw_w_tri_light=0;
	ramp_shadow_on_ramp_corner_nw_w_tri_heavy=0;
	ramp_shadow_on_ramp_corner_nw_n_tri_light=0;
	ramp_shadow_on_ramp_corner_nw_n_tri_heavy=0;
	ramp_shadow_on_ramp_corner_ne_e_tri_light=0;
	ramp_shadow_on_ramp_corner_ne_e_tri_heavy=0;
	ramp_shadow_on_ramp_corner_ne_n_tri_light=0;
	ramp_shadow_on_ramp_corner_ne_n_tri_heavy=0;
	ramp_shadow_on_ramp_corner_sw_w_tri_light=0;
	ramp_shadow_on_ramp_corner_sw_w_tri_heavy=0;
	ramp_shadow_on_ramp_corner_sw_s_tri_light=0;
	ramp_shadow_on_ramp_corner_sw_s_tri_heavy=0;
	ramp_shadow_on_ramp_corner_se_e_tri_light=0;
	ramp_shadow_on_ramp_corner_se_e_tri_heavy=0;
	ramp_shadow_on_ramp_corner_se_s_tri_light=0;
	ramp_shadow_on_ramp_corner_se_s_tri_heavy=0;

	for(m=0;m<4;++m)stone_wall_nswe_texpos[m]=0;
	stone_wall_swe_texpos=0;
	stone_wall_nwe_texpos=0;
	stone_wall_nse_texpos=0;
	stone_wall_nsw_texpos=0;
	stone_wall_n_w_texpos=0;
	stone_wall_n_e_texpos=0;
	stone_wall_s_w_texpos=0;
	stone_wall_s_e_texpos=0;
	for(m=0;m<4;++m)stone_wall_n_s_texpos[m]=0;
	for(m=0;m<4;++m)stone_wall_w_e_texpos[m]=0;
	for(m=0;m<4;++m)stone_wall_n_texpos[m]=0;
	for(m=0;m<4;++m)stone_wall_s_texpos[m]=0;
	for(m=0;m<4;++m)stone_wall_w_texpos[m]=0;
	for(m=0;m<4;++m)stone_wall_e_texpos[m]=0;
	stone_wall_nw_texpos=0;
	stone_wall_ne_texpos=0;
	stone_wall_sw_texpos=0;
	stone_wall_se_texpos=0;

	for(m=0;m<4;++m)soil_wall_nswe_texpos[m]=0;
	soil_wall_swe_texpos=0;
	soil_wall_nwe_texpos=0;
	soil_wall_nse_texpos=0;
	soil_wall_nsw_texpos=0;
	soil_wall_n_w_texpos=0;
	soil_wall_n_e_texpos=0;
	soil_wall_s_w_texpos=0;
	soil_wall_s_e_texpos=0;
	for(m=0;m<4;++m)soil_wall_n_s_texpos[m]=0;
	for(m=0;m<4;++m)soil_wall_w_e_texpos[m]=0;
	for(m=0;m<4;++m)soil_wall_n_texpos[m]=0;
	for(m=0;m<4;++m)soil_wall_s_texpos[m]=0;
	for(m=0;m<4;++m)soil_wall_w_texpos[m]=0;
	for(m=0;m<4;++m)soil_wall_e_texpos[m]=0;
	soil_wall_nw_texpos=0;
	soil_wall_ne_texpos=0;
	soil_wall_sw_texpos=0;
	soil_wall_se_texpos=0;

	for(m=0;m<4;++m)worn1_stone_wall_nswe_texpos[m]=0;
	worn1_stone_wall_swe_texpos=0;
	worn1_stone_wall_nwe_texpos=0;
	worn1_stone_wall_nse_texpos=0;
	worn1_stone_wall_nsw_texpos=0;
	worn1_stone_wall_n_w_texpos=0;
	worn1_stone_wall_n_e_texpos=0;
	worn1_stone_wall_s_w_texpos=0;
	worn1_stone_wall_s_e_texpos=0;
	for(m=0;m<4;++m)worn1_stone_wall_n_s_texpos[m]=0;
	for(m=0;m<4;++m)worn1_stone_wall_w_e_texpos[m]=0;
	for(m=0;m<4;++m)worn1_stone_wall_n_texpos[m]=0;
	for(m=0;m<4;++m)worn1_stone_wall_s_texpos[m]=0;
	for(m=0;m<4;++m)worn1_stone_wall_w_texpos[m]=0;
	for(m=0;m<4;++m)worn1_stone_wall_e_texpos[m]=0;
	worn1_stone_wall_nw_texpos=0;
	worn1_stone_wall_ne_texpos=0;
	worn1_stone_wall_sw_texpos=0;
	worn1_stone_wall_se_texpos=0;

	for(m=0;m<4;++m)worn2_stone_wall_nswe_texpos[m]=0;
	worn2_stone_wall_swe_texpos=0;
	worn2_stone_wall_nwe_texpos=0;
	worn2_stone_wall_nse_texpos=0;
	worn2_stone_wall_nsw_texpos=0;
	worn2_stone_wall_n_w_texpos=0;
	worn2_stone_wall_n_e_texpos=0;
	worn2_stone_wall_s_w_texpos=0;
	worn2_stone_wall_s_e_texpos=0;
	for(m=0;m<4;++m)worn2_stone_wall_n_s_texpos[m]=0;
	for(m=0;m<4;++m)worn2_stone_wall_w_e_texpos[m]=0;
	for(m=0;m<4;++m)worn2_stone_wall_n_texpos[m]=0;
	for(m=0;m<4;++m)worn2_stone_wall_s_texpos[m]=0;
	for(m=0;m<4;++m)worn2_stone_wall_w_texpos[m]=0;
	for(m=0;m<4;++m)worn2_stone_wall_e_texpos[m]=0;
	worn2_stone_wall_nw_texpos=0;
	worn2_stone_wall_ne_texpos=0;
	worn2_stone_wall_sw_texpos=0;
	worn2_stone_wall_se_texpos=0;

	for(m=0;m<4;++m)worn3_stone_wall_nswe_texpos[m]=0;
	worn3_stone_wall_swe_texpos=0;
	worn3_stone_wall_nwe_texpos=0;
	worn3_stone_wall_nse_texpos=0;
	worn3_stone_wall_nsw_texpos=0;
	worn3_stone_wall_n_w_texpos=0;
	worn3_stone_wall_n_e_texpos=0;
	worn3_stone_wall_s_w_texpos=0;
	worn3_stone_wall_s_e_texpos=0;
	for(m=0;m<4;++m)worn3_stone_wall_n_s_texpos[m]=0;
	for(m=0;m<4;++m)worn3_stone_wall_w_e_texpos[m]=0;
	for(m=0;m<4;++m)worn3_stone_wall_n_texpos[m]=0;
	for(m=0;m<4;++m)worn3_stone_wall_s_texpos[m]=0;
	for(m=0;m<4;++m)worn3_stone_wall_w_texpos[m]=0;
	for(m=0;m<4;++m)worn3_stone_wall_e_texpos[m]=0;
	worn3_stone_wall_nw_texpos=0;
	worn3_stone_wall_ne_texpos=0;
	worn3_stone_wall_sw_texpos=0;
	worn3_stone_wall_se_texpos=0;

	smoothed_stone_wall_nswe_texpos=0;
	smoothed_stone_wall_swe_texpos=0;
	smoothed_stone_wall_nwe_texpos=0;
	smoothed_stone_wall_nse_texpos=0;
	smoothed_stone_wall_nsw_texpos=0;
	smoothed_stone_wall_n_w_texpos=0;
	smoothed_stone_wall_n_e_texpos=0;
	smoothed_stone_wall_s_w_texpos=0;
	smoothed_stone_wall_s_e_texpos=0;
	smoothed_stone_wall_n_s_texpos=0;
	smoothed_stone_wall_w_e_texpos=0;
	smoothed_stone_wall_n_texpos=0;
	smoothed_stone_wall_s_texpos=0;
	smoothed_stone_wall_w_texpos=0;
	smoothed_stone_wall_e_texpos=0;
	smoothed_stone_wall_nw_texpos=0;
	smoothed_stone_wall_ne_texpos=0;
	smoothed_stone_wall_sw_texpos=0;
	smoothed_stone_wall_se_texpos=0;

	engraved_stone_wall_nswe_texpos=0;
	engraved_stone_wall_swe_texpos=0;
	engraved_stone_wall_nwe_texpos=0;
	engraved_stone_wall_nse_texpos=0;
	engraved_stone_wall_nsw_texpos=0;
	engraved_stone_wall_n_w_texpos=0;
	engraved_stone_wall_n_e_texpos=0;
	engraved_stone_wall_s_w_texpos=0;
	engraved_stone_wall_s_e_texpos=0;
	engraved_stone_wall_n_s_texpos=0;
	engraved_stone_wall_w_e_texpos=0;
	engraved_stone_wall_n_texpos=0;
	engraved_stone_wall_s_texpos=0;
	engraved_stone_wall_w_texpos=0;
	engraved_stone_wall_e_texpos=0;
	engraved_stone_wall_nw_texpos=0;
	engraved_stone_wall_ne_texpos=0;
	engraved_stone_wall_sw_texpos=0;
	engraved_stone_wall_se_texpos=0;

	moss_wall_nswe_texpos=0;
	moss_wall_swe_texpos=0;
	moss_wall_nwe_texpos=0;
	moss_wall_nse_texpos=0;
	moss_wall_nsw_texpos=0;
	moss_wall_n_w_texpos=0;
	moss_wall_n_e_texpos=0;
	moss_wall_s_w_texpos=0;
	moss_wall_s_e_texpos=0;
	moss_wall_n_s_texpos=0;
	moss_wall_w_e_texpos=0;
	moss_wall_n_texpos=0;
	moss_wall_s_texpos=0;
	moss_wall_w_texpos=0;
	moss_wall_e_texpos=0;
	moss_wall_nw_texpos=0;
	moss_wall_ne_texpos=0;
	moss_wall_sw_texpos=0;
	moss_wall_se_texpos=0;

	root_wall_nswe_texpos=0;
	root_wall_swe_texpos=0;
	root_wall_nwe_texpos=0;
	root_wall_nse_texpos=0;
	root_wall_nsw_texpos=0;
	root_wall_n_w_texpos=0;
	root_wall_n_e_texpos=0;
	root_wall_s_w_texpos=0;
	root_wall_s_e_texpos=0;
	root_wall_n_s_texpos=0;
	root_wall_w_e_texpos=0;
	root_wall_n_texpos=0;
	root_wall_s_texpos=0;
	root_wall_w_texpos=0;
	root_wall_e_texpos=0;
	root_wall_nw_texpos=0;
	root_wall_ne_texpos=0;
	root_wall_sw_texpos=0;
	root_wall_se_texpos=0;

	sand_wall_nswe_texpos=0;
	sand_wall_swe_texpos=0;
	sand_wall_nwe_texpos=0;
	sand_wall_nse_texpos=0;
	sand_wall_nsw_texpos=0;
	sand_wall_n_w_texpos=0;
	sand_wall_n_e_texpos=0;
	sand_wall_s_w_texpos=0;
	sand_wall_s_e_texpos=0;
	sand_wall_n_s_texpos=0;
	sand_wall_w_e_texpos=0;
	sand_wall_n_texpos=0;
	sand_wall_s_texpos=0;
	sand_wall_w_texpos=0;
	sand_wall_e_texpos=0;
	sand_wall_nw_texpos=0;
	sand_wall_ne_texpos=0;
	sand_wall_sw_texpos=0;
	sand_wall_se_texpos=0;

	sand_y_wall_nswe_texpos=0;
	sand_y_wall_swe_texpos=0;
	sand_y_wall_nwe_texpos=0;
	sand_y_wall_nse_texpos=0;
	sand_y_wall_nsw_texpos=0;
	sand_y_wall_n_w_texpos=0;
	sand_y_wall_n_e_texpos=0;
	sand_y_wall_s_w_texpos=0;
	sand_y_wall_s_e_texpos=0;
	sand_y_wall_n_s_texpos=0;
	sand_y_wall_w_e_texpos=0;
	sand_y_wall_n_texpos=0;
	sand_y_wall_s_texpos=0;
	sand_y_wall_w_texpos=0;
	sand_y_wall_e_texpos=0;
	sand_y_wall_nw_texpos=0;
	sand_y_wall_ne_texpos=0;
	sand_y_wall_sw_texpos=0;
	sand_y_wall_se_texpos=0;

	sand_w_wall_nswe_texpos=0;
	sand_w_wall_swe_texpos=0;
	sand_w_wall_nwe_texpos=0;
	sand_w_wall_nse_texpos=0;
	sand_w_wall_nsw_texpos=0;
	sand_w_wall_n_w_texpos=0;
	sand_w_wall_n_e_texpos=0;
	sand_w_wall_s_w_texpos=0;
	sand_w_wall_s_e_texpos=0;
	sand_w_wall_n_s_texpos=0;
	sand_w_wall_w_e_texpos=0;
	sand_w_wall_n_texpos=0;
	sand_w_wall_s_texpos=0;
	sand_w_wall_w_texpos=0;
	sand_w_wall_e_texpos=0;
	sand_w_wall_nw_texpos=0;
	sand_w_wall_ne_texpos=0;
	sand_w_wall_sw_texpos=0;
	sand_w_wall_se_texpos=0;

	sand_b_wall_nswe_texpos=0;
	sand_b_wall_swe_texpos=0;
	sand_b_wall_nwe_texpos=0;
	sand_b_wall_nse_texpos=0;
	sand_b_wall_nsw_texpos=0;
	sand_b_wall_n_w_texpos=0;
	sand_b_wall_n_e_texpos=0;
	sand_b_wall_s_w_texpos=0;
	sand_b_wall_s_e_texpos=0;
	sand_b_wall_n_s_texpos=0;
	sand_b_wall_w_e_texpos=0;
	sand_b_wall_n_texpos=0;
	sand_b_wall_s_texpos=0;
	sand_b_wall_w_texpos=0;
	sand_b_wall_e_texpos=0;
	sand_b_wall_nw_texpos=0;
	sand_b_wall_ne_texpos=0;
	sand_b_wall_sw_texpos=0;
	sand_b_wall_se_texpos=0;

	sand_r_wall_nswe_texpos=0;
	sand_r_wall_swe_texpos=0;
	sand_r_wall_nwe_texpos=0;
	sand_r_wall_nse_texpos=0;
	sand_r_wall_nsw_texpos=0;
	sand_r_wall_n_w_texpos=0;
	sand_r_wall_n_e_texpos=0;
	sand_r_wall_s_w_texpos=0;
	sand_r_wall_s_e_texpos=0;
	sand_r_wall_n_s_texpos=0;
	sand_r_wall_w_e_texpos=0;
	sand_r_wall_n_texpos=0;
	sand_r_wall_s_texpos=0;
	sand_r_wall_w_texpos=0;
	sand_r_wall_e_texpos=0;
	sand_r_wall_nw_texpos=0;
	sand_r_wall_ne_texpos=0;
	sand_r_wall_sw_texpos=0;
	sand_r_wall_se_texpos=0;

	ice_wall_nswe_texpos=0;
	ice_wall_swe_texpos=0;
	ice_wall_nwe_texpos=0;
	ice_wall_nse_texpos=0;
	ice_wall_nsw_texpos=0;
	ice_wall_n_w_texpos=0;
	ice_wall_n_e_texpos=0;
	ice_wall_s_w_texpos=0;
	ice_wall_s_e_texpos=0;
	ice_wall_n_s_texpos=0;
	ice_wall_w_e_texpos=0;
	ice_wall_n_texpos=0;
	ice_wall_s_texpos=0;
	ice_wall_w_texpos=0;
	ice_wall_e_texpos=0;
	ice_wall_nw_texpos=0;
	ice_wall_ne_texpos=0;
	ice_wall_sw_texpos=0;
	ice_wall_se_texpos=0;

	smoothed_ice_wall_nswe_texpos=0;
	smoothed_ice_wall_swe_texpos=0;
	smoothed_ice_wall_nwe_texpos=0;
	smoothed_ice_wall_nse_texpos=0;
	smoothed_ice_wall_nsw_texpos=0;
	smoothed_ice_wall_n_w_texpos=0;
	smoothed_ice_wall_n_e_texpos=0;
	smoothed_ice_wall_s_w_texpos=0;
	smoothed_ice_wall_s_e_texpos=0;
	smoothed_ice_wall_n_s_texpos=0;
	smoothed_ice_wall_w_e_texpos=0;
	smoothed_ice_wall_n_texpos=0;
	smoothed_ice_wall_s_texpos=0;
	smoothed_ice_wall_w_texpos=0;
	smoothed_ice_wall_e_texpos=0;
	smoothed_ice_wall_nw_texpos=0;
	smoothed_ice_wall_ne_texpos=0;
	smoothed_ice_wall_sw_texpos=0;
	smoothed_ice_wall_se_texpos=0;

	magma_wall_nswe_texpos=0;
	magma_wall_swe_texpos=0;
	magma_wall_nwe_texpos=0;
	magma_wall_nse_texpos=0;
	magma_wall_nsw_texpos=0;
	magma_wall_n_w_texpos=0;
	magma_wall_n_e_texpos=0;
	magma_wall_s_w_texpos=0;
	magma_wall_s_e_texpos=0;
	magma_wall_n_s_texpos=0;
	magma_wall_w_e_texpos=0;
	magma_wall_n_texpos=0;
	magma_wall_s_texpos=0;
	magma_wall_w_texpos=0;
	magma_wall_e_texpos=0;
	magma_wall_nw_texpos=0;
	magma_wall_ne_texpos=0;
	magma_wall_sw_texpos=0;
	magma_wall_se_texpos=0;

	gem_a_wall_nswe_texpos=0;
	gem_a_wall_swe_texpos=0;
	gem_a_wall_nwe_texpos=0;
	gem_a_wall_nse_texpos=0;
	gem_a_wall_nsw_texpos=0;
	gem_a_wall_n_w_texpos=0;
	gem_a_wall_n_e_texpos=0;
	gem_a_wall_s_w_texpos=0;
	gem_a_wall_s_e_texpos=0;
	gem_a_wall_n_s_texpos=0;
	gem_a_wall_w_e_texpos=0;
	gem_a_wall_n_texpos=0;
	gem_a_wall_s_texpos=0;
	gem_a_wall_w_texpos=0;
	gem_a_wall_e_texpos=0;
	gem_a_wall_nw_texpos=0;
	gem_a_wall_ne_texpos=0;
	gem_a_wall_sw_texpos=0;
	gem_a_wall_se_texpos=0;

	gem_b_wall_nswe_texpos=0;
	gem_b_wall_swe_texpos=0;
	gem_b_wall_nwe_texpos=0;
	gem_b_wall_nse_texpos=0;
	gem_b_wall_nsw_texpos=0;
	gem_b_wall_n_w_texpos=0;
	gem_b_wall_n_e_texpos=0;
	gem_b_wall_s_w_texpos=0;
	gem_b_wall_s_e_texpos=0;
	gem_b_wall_n_s_texpos=0;
	gem_b_wall_w_e_texpos=0;
	gem_b_wall_n_texpos=0;
	gem_b_wall_s_texpos=0;
	gem_b_wall_w_texpos=0;
	gem_b_wall_e_texpos=0;
	gem_b_wall_nw_texpos=0;
	gem_b_wall_ne_texpos=0;
	gem_b_wall_sw_texpos=0;
	gem_b_wall_se_texpos=0;

	gem_c_wall_nswe_texpos=0;
	gem_c_wall_swe_texpos=0;
	gem_c_wall_nwe_texpos=0;
	gem_c_wall_nse_texpos=0;
	gem_c_wall_nsw_texpos=0;
	gem_c_wall_n_w_texpos=0;
	gem_c_wall_n_e_texpos=0;
	gem_c_wall_s_w_texpos=0;
	gem_c_wall_s_e_texpos=0;
	gem_c_wall_n_s_texpos=0;
	gem_c_wall_w_e_texpos=0;
	gem_c_wall_n_texpos=0;
	gem_c_wall_s_texpos=0;
	gem_c_wall_w_texpos=0;
	gem_c_wall_e_texpos=0;
	gem_c_wall_nw_texpos=0;
	gem_c_wall_ne_texpos=0;
	gem_c_wall_sw_texpos=0;
	gem_c_wall_se_texpos=0;

	gem_d_wall_nswe_texpos=0;
	gem_d_wall_swe_texpos=0;
	gem_d_wall_nwe_texpos=0;
	gem_d_wall_nse_texpos=0;
	gem_d_wall_nsw_texpos=0;
	gem_d_wall_n_w_texpos=0;
	gem_d_wall_n_e_texpos=0;
	gem_d_wall_s_w_texpos=0;
	gem_d_wall_s_e_texpos=0;
	gem_d_wall_n_s_texpos=0;
	gem_d_wall_w_e_texpos=0;
	gem_d_wall_n_texpos=0;
	gem_d_wall_s_texpos=0;
	gem_d_wall_w_texpos=0;
	gem_d_wall_e_texpos=0;
	gem_d_wall_nw_texpos=0;
	gem_d_wall_ne_texpos=0;
	gem_d_wall_sw_texpos=0;
	gem_d_wall_se_texpos=0;

	metal_bars_wall_nswe_texpos=0;
	metal_bars_wall_swe_texpos=0;
	metal_bars_wall_nwe_texpos=0;
	metal_bars_wall_nse_texpos=0;
	metal_bars_wall_nsw_texpos=0;
	metal_bars_wall_n_w_texpos=0;
	metal_bars_wall_n_e_texpos=0;
	metal_bars_wall_s_w_texpos=0;
	metal_bars_wall_s_e_texpos=0;
	metal_bars_wall_n_s_texpos=0;
	metal_bars_wall_w_e_texpos=0;
	metal_bars_wall_n_texpos=0;
	metal_bars_wall_s_texpos=0;
	metal_bars_wall_w_texpos=0;
	metal_bars_wall_e_texpos=0;
	metal_bars_wall_nw_texpos=0;
	metal_bars_wall_ne_texpos=0;
	metal_bars_wall_sw_texpos=0;
	metal_bars_wall_se_texpos=0;

	ore_vein_wall_nswe_texpos=0;
	ore_vein_wall_swe_texpos=0;
	ore_vein_wall_nwe_texpos=0;
	ore_vein_wall_nse_texpos=0;
	ore_vein_wall_nsw_texpos=0;
	ore_vein_wall_n_w_texpos=0;
	ore_vein_wall_n_e_texpos=0;
	ore_vein_wall_s_w_texpos=0;
	ore_vein_wall_s_e_texpos=0;
	ore_vein_wall_n_s_texpos=0;
	ore_vein_wall_w_e_texpos=0;
	ore_vein_wall_n_texpos=0;
	ore_vein_wall_s_texpos=0;
	ore_vein_wall_w_texpos=0;
	ore_vein_wall_e_texpos=0;
	ore_vein_wall_nw_texpos=0;
	ore_vein_wall_ne_texpos=0;
	ore_vein_wall_sw_texpos=0;
	ore_vein_wall_se_texpos=0;

	rock_blocks_wall_nswe_texpos=0;
	rock_blocks_wall_swe_texpos=0;
	rock_blocks_wall_nwe_texpos=0;
	rock_blocks_wall_nse_texpos=0;
	rock_blocks_wall_nsw_texpos=0;
	rock_blocks_wall_n_w_texpos=0;
	rock_blocks_wall_n_e_texpos=0;
	rock_blocks_wall_s_w_texpos=0;
	rock_blocks_wall_s_e_texpos=0;
	rock_blocks_wall_n_s_texpos=0;
	rock_blocks_wall_w_e_texpos=0;
	rock_blocks_wall_n_texpos=0;
	rock_blocks_wall_s_texpos=0;
	rock_blocks_wall_w_texpos=0;
	rock_blocks_wall_e_texpos=0;
	rock_blocks_wall_nw_texpos=0;
	rock_blocks_wall_ne_texpos=0;
	rock_blocks_wall_sw_texpos=0;
	rock_blocks_wall_se_texpos=0;

	reinforced_metal_wall_nswe_texpos=0;
	reinforced_metal_wall_swe_texpos=0;
	reinforced_metal_wall_nwe_texpos=0;
	reinforced_metal_wall_nse_texpos=0;
	reinforced_metal_wall_nsw_texpos=0;
	reinforced_metal_wall_n_w_texpos=0;
	reinforced_metal_wall_n_e_texpos=0;
	reinforced_metal_wall_s_w_texpos=0;
	reinforced_metal_wall_s_e_texpos=0;
	reinforced_metal_wall_n_s_texpos=0;
	reinforced_metal_wall_w_e_texpos=0;
	reinforced_metal_wall_n_texpos=0;
	reinforced_metal_wall_s_texpos=0;
	reinforced_metal_wall_w_texpos=0;
	reinforced_metal_wall_e_texpos=0;
	reinforced_metal_wall_nw_texpos=0;
	reinforced_metal_wall_ne_texpos=0;
	reinforced_metal_wall_sw_texpos=0;
	reinforced_metal_wall_se_texpos=0;

	wooden_wall_nswe_texpos=0;
	wooden_wall_swe_texpos=0;
	wooden_wall_nwe_texpos=0;
	wooden_wall_nse_texpos=0;
	wooden_wall_nsw_texpos=0;
	wooden_wall_n_w_texpos=0;
	wooden_wall_n_e_texpos=0;
	wooden_wall_s_w_texpos=0;
	wooden_wall_s_e_texpos=0;
	wooden_wall_n_s_texpos=0;
	wooden_wall_w_e_texpos=0;
	wooden_wall_n_texpos=0;
	wooden_wall_s_texpos=0;
	wooden_wall_w_texpos=0;
	wooden_wall_e_texpos=0;
	wooden_wall_nw_texpos=0;
	wooden_wall_ne_texpos=0;
	wooden_wall_sw_texpos=0;
	wooden_wall_se_texpos=0;

	for(m=0;m<5;++m)texpos_hidden_rock[m]=0;

	{int32_t p;
	for(p=0;p<7;++p)texpos_designation_priority[p]=0;
	for(p=0;p<8;++p){
	for(m=0;m<3;++m)
		{
		texpos_designation_dig_stair_updown[p][m]=0;
		texpos_designation_dig_stair_up[p][m]=0;
		texpos_designation_dig_stair_down[p][m]=0;
		texpos_designation_dig_ramp[p][m]=0;
		texpos_designation_dig_channel[p][m]=0;
		texpos_designation_dig_remove_construction[p][m]=0;
		texpos_designation_dig_standard[p][m]=0;
		}}
	for(p=0;p<8;++p){
	for(m=0;m<2;++m)
		{
		texpos_designation_chop[p][m]=0;
		texpos_designation_gather[p][m]=0;
		texpos_designation_smooth[p][m]=0;
		texpos_designation_engrave[p][m]=0;
		texpos_designation_fortify[p][m]=0;
		texpos_designation_track_n[p][m]=0;
		texpos_designation_track_s[p][m]=0;
		texpos_designation_track_w[p][m]=0;
		texpos_designation_track_e[p][m]=0;
		texpos_designation_track_ns[p][m]=0;
		texpos_designation_track_nw[p][m]=0;
		texpos_designation_track_ne[p][m]=0;
		texpos_designation_track_sw[p][m]=0;
		texpos_designation_track_se[p][m]=0;
		texpos_designation_track_we[p][m]=0;
		texpos_designation_track_nsw[p][m]=0;
		texpos_designation_track_nse[p][m]=0;
		texpos_designation_track_nwe[p][m]=0;
		texpos_designation_track_swe[p][m]=0;
		texpos_designation_track_nswe[p][m]=0;
		}}
		}
	texpos_designation_item_melt=0;
	texpos_designation_item_dump=0;
	texpos_designation_item_forbidden=0;
	texpos_designation_item_hidden=0;
	texpos_designation_item_forbidden_melt=0;
	texpos_designation_item_forbidden_dump=0;
	texpos_designation_traffic_high=0;
	texpos_designation_traffic_low=0;
	texpos_designation_traffic_restricted=0;

	texpos_damp_stone_warning=0;
	texpos_warm_stone_warning=0;

	memset(texpos_classic_moon_weather,0,sizeof(int32_t)*INTERFACE_MOON_WEATHERNUM*4*3);
	memset(texpos_classic_announcement_alert,0,sizeof(int32_t)*ANNOUNCEMENT_ALERTNUM*4*3);

	texpos_bottom_button_border_nw=0;
	texpos_bottom_button_border_w=0;
	texpos_bottom_button_border_n=0;
	texpos_bottom_button_border_interior=0;
	texpos_bottom_button_border_ne=0;
	texpos_bottom_button_border_e=0;

	missing_creature=0;
	missing_item=0;
	missing_building=0;

	memset(texpos_legends_tab_page_left,0,sizeof(int32_t)*3*2);
	memset(texpos_legends_tab_page_right,0,sizeof(int32_t)*3*2);
	memset(texpos_legends_tab_close_inactive,0,sizeof(int32_t)*2);
	memset(texpos_legends_tab_close_active,0,sizeof(int32_t)*2);

	memset(texpos_embark_selected,0,sizeof(int32_t)*4*3);
	memset(texpos_embark_not_selected,0,sizeof(int32_t)*4*3);
	memset(texpos_embark_expand_y_active,0,sizeof(int32_t)*2*3);
	memset(texpos_embark_expand_y_inactive,0,sizeof(int32_t)*2*3);
	memset(texpos_embark_contract_y_active,0,sizeof(int32_t)*2*3);
	memset(texpos_embark_contract_y_inactive,0,sizeof(int32_t)*2*3);
	memset(texpos_embark_expand_x_active,0,sizeof(int32_t)*4);
	memset(texpos_embark_expand_x_inactive,0,sizeof(int32_t)*4);
	memset(texpos_embark_contract_x_active,0,sizeof(int32_t)*4);
	memset(texpos_embark_contract_x_inactive,0,sizeof(int32_t)*4);

	memset(texpos_adventure_burden_light,0,sizeof(int32_t)*4);
	memset(texpos_adventure_burden_heavy,0,sizeof(int32_t)*4);

	memset(texpos_help_border,0,sizeof(int32_t)*3*3);
	memset(texpos_help_corner,0,sizeof(int32_t)*8*6);
	memset(texpos_help_close,0,sizeof(int32_t)*3*2);
	memset(texpos_help_hide,0,sizeof(int32_t)*3*2);
	memset(texpos_help_reveal,0,sizeof(int32_t)*3*2);

	texpos_embark_overlay_restricted=0;
	texpos_embark_overlay_good=0;
	texpos_embark_overlay_find_result=0;
	texpos_embark_overlay_find_result_partial=0;

	texpos_world_el_ocean_base=0;
	texpos_world_el_mid_base=0;
	texpos_world_el_mountains_base=0;
	memset(texpos_world_el_water,0,sizeof(int32_t)*280);
	memset(texpos_world_el,0,sizeof(int32_t)*280);
	memset(texpos_world_el_mountain,0,sizeof(int32_t)*280);
	memset(texpos_world_cliff,0,sizeof(int32_t)*11);

	int32_t s;
	for(s=0;s<REGION_SQUARE_VARIANT_NUM;++s)
		{
		texpos_grassland_temp[s]=0;
		texpos_grassland_temp_evilsav[s]=0;
		texpos_grassland_temp_evil[s]=0;
		texpos_grassland_temp_goodsav[s]=0;
		texpos_grassland_temp_good[s]=0;
		texpos_grassland_trop[s]=0;
		texpos_grassland_trop_evilsav[s]=0;
		texpos_grassland_trop_evil[s]=0;
		texpos_grassland_trop_goodsav[s]=0;
		texpos_grassland_trop_good[s]=0;
		texpos_hills[s]=0;
		texpos_hills_evilsav[s]=0;
		texpos_hills_evil[s]=0;
		texpos_hills_goodsav[s]=0;
		texpos_hills_good[s]=0;
		texpos_shrubland[s]=0;
		texpos_shrubland_evilsav[s]=0;
		texpos_shrubland_evil[s]=0;
		texpos_shrubland_goodsav[s]=0;
		texpos_shrubland_good[s]=0;
		texpos_savanna_temp[s]=0;
		texpos_savanna_temp_evilsav[s]=0;
		texpos_savanna_temp_evil[s]=0;
		texpos_savanna_temp_goodsav[s]=0;
		texpos_savanna_temp_good[s]=0;
		texpos_savanna_trop[s]=0;
		texpos_savanna_trop_evilsav[s]=0;
		texpos_savanna_trop_evil[s]=0;
		texpos_savanna_trop_goodsav[s]=0;
		texpos_savanna_trop_good[s]=0;
		texpos_tundra[s]=0;
		texpos_tundra_evilsav[s]=0;
		texpos_tundra_evil[s]=0;
		texpos_tundra_goodsav[s]=0;
		texpos_tundra_good[s]=0;
		texpos_marsh[s]=0;
		texpos_marsh_evilsav[s]=0;
		texpos_marsh_evil[s]=0;
		texpos_marsh_goodsav[s]=0;
		texpos_marsh_good[s]=0;
		texpos_swamp[s]=0;
		texpos_swamp_evilsav[s]=0;
		texpos_swamp_evil[s]=0;
		texpos_swamp_goodsav[s]=0;
		texpos_swamp_good[s]=0;
		texpos_badlands[s]=0;
		texpos_badlands_evilsav[s]=0;
		texpos_badlands_evil[s]=0;
		texpos_badlands_goodsav[s]=0;
		texpos_badlands_good[s]=0;
		texpos_rocky_hills[s]=0;
		texpos_rocky_hills_evilsav[s]=0;
		texpos_rocky_hills_evil[s]=0;
		texpos_rocky_hills_goodsav[s]=0;
		texpos_rocky_hills_good[s]=0;
		texpos_rocky_plains[s]=0;
		texpos_rocky_plains_evilsav[s]=0;
		texpos_rocky_plains_evil[s]=0;
		texpos_rocky_plains_goodsav[s]=0;
		texpos_rocky_plains_good[s]=0;
		texpos_sand_desert[s]=0;
		texpos_sand_desert_yellow[s]=0;
		texpos_sand_desert_white[s]=0;
		texpos_sand_desert_black[s]=0;
		texpos_sand_desert_red[s]=0;
		texpos_sand_desert_evilsav[s]=0;
		texpos_sand_desert_evil[s]=0;
		texpos_sand_desert_goodsav[s]=0;
		texpos_sand_desert_good[s]=0;
		texpos_beach[s]=0;
		texpos_beach_evilsav[s]=0;
		texpos_beach_evil[s]=0;
		texpos_beach_goodsav[s]=0;
		texpos_beach_good[s]=0;
		texpos_glacier[s]=0;
		texpos_glacier_evilsav[s]=0;
		texpos_glacier_evil[s]=0;
		texpos_glacier_goodsav[s]=0;
		texpos_glacier_good[s]=0;
		texpos_lake[s]=0;
		texpos_lake_evilsav[s]=0;
		texpos_lake_evil[s]=0;
		texpos_lake_goodsav[s]=0;
		texpos_lake_good[s]=0;
		texpos_ocean[s]=0;
		texpos_ocean_evilsav[s]=0;
		texpos_ocean_evil[s]=0;
		texpos_ocean_goodsav[s]=0;
		texpos_ocean_good[s]=0;
		texpos_ocean_deep[s]=0;
		texpos_ocean_deep_evilsav[s]=0;
		texpos_ocean_deep_evil[s]=0;
		texpos_ocean_deep_goodsav[s]=0;
		texpos_ocean_deep_good[s]=0;
		texpos_frozen_ocean[s]=0;
		texpos_frozen_ocean_evilsav[s]=0;
		texpos_frozen_ocean_evil[s]=0;
		texpos_frozen_ocean_goodsav[s]=0;
		texpos_frozen_ocean_good[s]=0;
		texpos_site_grassy[s]=0;
		texpos_site_muddy[s]=0;
		texpos_world_edge_shape_straight_n[s]=0;
		texpos_world_edge_shape_straight_s[s]=0;
		texpos_world_edge_shape_straight_w[s]=0;
		texpos_world_edge_shape_straight_e[s]=0;
		texpos_world_edge_shape_thick_corner_nw[s]=0;
		texpos_world_edge_shape_thick_corner_ne[s]=0;
		texpos_world_edge_shape_thick_corner_sw[s]=0;
		texpos_world_edge_shape_thick_corner_se[s]=0;
		texpos_world_edge_shape_thin_corner_nw[s]=0;
		texpos_world_edge_shape_thin_corner_ne[s]=0;
		texpos_world_edge_shape_thin_corner_sw[s]=0;
		texpos_world_edge_shape_thin_corner_se[s]=0;
		texpos_world_edge_shape_tiny_corner_nw[s]=0;
		texpos_world_edge_shape_tiny_corner_ne[s]=0;
		texpos_world_edge_shape_tiny_corner_sw[s]=0;
		texpos_world_edge_shape_tiny_corner_se[s]=0;
		texpos_world_edge_shape_inlet_nsw[s]=0;
		texpos_world_edge_shape_inlet_nse[s]=0;
		texpos_world_edge_shape_inlet_nwe[s]=0;
		texpos_world_edge_shape_inlet_swe[s]=0;
		texpos_world_edge_shape_surround[s]=0;
		texpos_world_edge_shape_thinning_n_w[s]=0;
		texpos_world_edge_shape_thinning_n_e[s]=0;
		texpos_world_edge_shape_thinning_s_w[s]=0;
		texpos_world_edge_shape_thinning_s_e[s]=0;
		texpos_world_edge_shape_thinning_w_n[s]=0;
		texpos_world_edge_shape_thinning_w_s[s]=0;
		texpos_world_edge_shape_thinning_e_n[s]=0;
		texpos_world_edge_shape_thinning_e_s[s]=0;
		texpos_world_edge_shape_thin_corner_transition_nw_n[s]=0;
		texpos_world_edge_shape_thin_corner_transition_nw_w[s]=0;
		texpos_world_edge_shape_thin_corner_transition_ne_n[s]=0;
		texpos_world_edge_shape_thin_corner_transition_ne_e[s]=0;
		texpos_world_edge_shape_thin_corner_transition_sw_s[s]=0;
		texpos_world_edge_shape_thin_corner_transition_sw_w[s]=0;
		texpos_world_edge_shape_thin_corner_transition_se_s[s]=0;
		texpos_world_edge_shape_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_temp_straight_n[s]=0;
		texpos_edge_grass_temp_straight_s[s]=0;
		texpos_edge_grass_temp_straight_w[s]=0;
		texpos_edge_grass_temp_straight_e[s]=0;
		texpos_edge_grass_temp_thick_corner_nw[s]=0;
		texpos_edge_grass_temp_thick_corner_ne[s]=0;
		texpos_edge_grass_temp_thick_corner_sw[s]=0;
		texpos_edge_grass_temp_thick_corner_se[s]=0;
		texpos_edge_grass_temp_thin_corner_nw[s]=0;
		texpos_edge_grass_temp_thin_corner_ne[s]=0;
		texpos_edge_grass_temp_thin_corner_sw[s]=0;
		texpos_edge_grass_temp_thin_corner_se[s]=0;
		texpos_edge_grass_temp_inlet_nsw[s]=0;
		texpos_edge_grass_temp_inlet_nse[s]=0;
		texpos_edge_grass_temp_inlet_nwe[s]=0;
		texpos_edge_grass_temp_inlet_swe[s]=0;
		texpos_edge_grass_temp_surround[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_temp_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_temp_tiny_corner_nw[s]=0;
		texpos_edge_grass_temp_tiny_corner_ne[s]=0;
		texpos_edge_grass_temp_tiny_corner_sw[s]=0;
		texpos_edge_grass_temp_tiny_corner_se[s]=0;
		texpos_edge_grass_temp_thinning_n_w[s]=0;
		texpos_edge_grass_temp_thinning_n_e[s]=0;
		texpos_edge_grass_temp_thinning_s_w[s]=0;
		texpos_edge_grass_temp_thinning_s_e[s]=0;
		texpos_edge_grass_temp_thinning_w_n[s]=0;
		texpos_edge_grass_temp_thinning_w_s[s]=0;
		texpos_edge_grass_temp_thinning_e_n[s]=0;
		texpos_edge_grass_temp_thinning_e_s[s]=0;
		texpos_edge_grass_temp_evil_straight_n[s]=0;
		texpos_edge_grass_temp_evil_straight_s[s]=0;
		texpos_edge_grass_temp_evil_straight_w[s]=0;
		texpos_edge_grass_temp_evil_straight_e[s]=0;
		texpos_edge_grass_temp_evil_thick_corner_nw[s]=0;
		texpos_edge_grass_temp_evil_thick_corner_ne[s]=0;
		texpos_edge_grass_temp_evil_thick_corner_sw[s]=0;
		texpos_edge_grass_temp_evil_thick_corner_se[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_nw[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_ne[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_sw[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_se[s]=0;
		texpos_edge_grass_temp_evil_inlet_nsw[s]=0;
		texpos_edge_grass_temp_evil_inlet_nse[s]=0;
		texpos_edge_grass_temp_evil_inlet_nwe[s]=0;
		texpos_edge_grass_temp_evil_inlet_swe[s]=0;
		texpos_edge_grass_temp_evil_surround[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_temp_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_temp_evil_tiny_corner_nw[s]=0;
		texpos_edge_grass_temp_evil_tiny_corner_ne[s]=0;
		texpos_edge_grass_temp_evil_tiny_corner_sw[s]=0;
		texpos_edge_grass_temp_evil_tiny_corner_se[s]=0;
		texpos_edge_grass_temp_evil_thinning_n_w[s]=0;
		texpos_edge_grass_temp_evil_thinning_n_e[s]=0;
		texpos_edge_grass_temp_evil_thinning_s_w[s]=0;
		texpos_edge_grass_temp_evil_thinning_s_e[s]=0;
		texpos_edge_grass_temp_evil_thinning_w_n[s]=0;
		texpos_edge_grass_temp_evil_thinning_w_s[s]=0;
		texpos_edge_grass_temp_evil_thinning_e_n[s]=0;
		texpos_edge_grass_temp_evil_thinning_e_s[s]=0;
		texpos_edge_grass_temp_good_straight_n[s]=0;
		texpos_edge_grass_temp_good_straight_s[s]=0;
		texpos_edge_grass_temp_good_straight_w[s]=0;
		texpos_edge_grass_temp_good_straight_e[s]=0;
		texpos_edge_grass_temp_good_thick_corner_nw[s]=0;
		texpos_edge_grass_temp_good_thick_corner_ne[s]=0;
		texpos_edge_grass_temp_good_thick_corner_sw[s]=0;
		texpos_edge_grass_temp_good_thick_corner_se[s]=0;
		texpos_edge_grass_temp_good_thin_corner_nw[s]=0;
		texpos_edge_grass_temp_good_thin_corner_ne[s]=0;
		texpos_edge_grass_temp_good_thin_corner_sw[s]=0;
		texpos_edge_grass_temp_good_thin_corner_se[s]=0;
		texpos_edge_grass_temp_good_inlet_nsw[s]=0;
		texpos_edge_grass_temp_good_inlet_nse[s]=0;
		texpos_edge_grass_temp_good_inlet_nwe[s]=0;
		texpos_edge_grass_temp_good_inlet_swe[s]=0;
		texpos_edge_grass_temp_good_surround[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_temp_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_temp_good_tiny_corner_nw[s]=0;
		texpos_edge_grass_temp_good_tiny_corner_ne[s]=0;
		texpos_edge_grass_temp_good_tiny_corner_sw[s]=0;
		texpos_edge_grass_temp_good_tiny_corner_se[s]=0;
		texpos_edge_grass_temp_good_thinning_n_w[s]=0;
		texpos_edge_grass_temp_good_thinning_n_e[s]=0;
		texpos_edge_grass_temp_good_thinning_s_w[s]=0;
		texpos_edge_grass_temp_good_thinning_s_e[s]=0;
		texpos_edge_grass_temp_good_thinning_w_n[s]=0;
		texpos_edge_grass_temp_good_thinning_w_s[s]=0;
		texpos_edge_grass_temp_good_thinning_e_n[s]=0;
		texpos_edge_grass_temp_good_thinning_e_s[s]=0;
		texpos_edge_grass_trop_straight_n[s]=0;
		texpos_edge_grass_trop_straight_s[s]=0;
		texpos_edge_grass_trop_straight_w[s]=0;
		texpos_edge_grass_trop_straight_e[s]=0;
		texpos_edge_grass_trop_thick_corner_nw[s]=0;
		texpos_edge_grass_trop_thick_corner_ne[s]=0;
		texpos_edge_grass_trop_thick_corner_sw[s]=0;
		texpos_edge_grass_trop_thick_corner_se[s]=0;
		texpos_edge_grass_trop_thin_corner_nw[s]=0;
		texpos_edge_grass_trop_thin_corner_ne[s]=0;
		texpos_edge_grass_trop_thin_corner_sw[s]=0;
		texpos_edge_grass_trop_thin_corner_se[s]=0;
		texpos_edge_grass_trop_inlet_nsw[s]=0;
		texpos_edge_grass_trop_inlet_nse[s]=0;
		texpos_edge_grass_trop_inlet_nwe[s]=0;
		texpos_edge_grass_trop_inlet_swe[s]=0;
		texpos_edge_grass_trop_surround[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_trop_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_trop_tiny_corner_nw[s]=0;
		texpos_edge_grass_trop_tiny_corner_ne[s]=0;
		texpos_edge_grass_trop_tiny_corner_sw[s]=0;
		texpos_edge_grass_trop_tiny_corner_se[s]=0;
		texpos_edge_grass_trop_thinning_n_w[s]=0;
		texpos_edge_grass_trop_thinning_n_e[s]=0;
		texpos_edge_grass_trop_thinning_s_w[s]=0;
		texpos_edge_grass_trop_thinning_s_e[s]=0;
		texpos_edge_grass_trop_thinning_w_n[s]=0;
		texpos_edge_grass_trop_thinning_w_s[s]=0;
		texpos_edge_grass_trop_thinning_e_n[s]=0;
		texpos_edge_grass_trop_thinning_e_s[s]=0;
		texpos_edge_grass_trop_evil_straight_n[s]=0;
		texpos_edge_grass_trop_evil_straight_s[s]=0;
		texpos_edge_grass_trop_evil_straight_w[s]=0;
		texpos_edge_grass_trop_evil_straight_e[s]=0;
		texpos_edge_grass_trop_evil_thick_corner_nw[s]=0;
		texpos_edge_grass_trop_evil_thick_corner_ne[s]=0;
		texpos_edge_grass_trop_evil_thick_corner_sw[s]=0;
		texpos_edge_grass_trop_evil_thick_corner_se[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_nw[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_ne[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_sw[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_se[s]=0;
		texpos_edge_grass_trop_evil_inlet_nsw[s]=0;
		texpos_edge_grass_trop_evil_inlet_nse[s]=0;
		texpos_edge_grass_trop_evil_inlet_nwe[s]=0;
		texpos_edge_grass_trop_evil_inlet_swe[s]=0;
		texpos_edge_grass_trop_evil_surround[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_trop_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_trop_evil_tiny_corner_nw[s]=0;
		texpos_edge_grass_trop_evil_tiny_corner_ne[s]=0;
		texpos_edge_grass_trop_evil_tiny_corner_sw[s]=0;
		texpos_edge_grass_trop_evil_tiny_corner_se[s]=0;
		texpos_edge_grass_trop_evil_thinning_n_w[s]=0;
		texpos_edge_grass_trop_evil_thinning_n_e[s]=0;
		texpos_edge_grass_trop_evil_thinning_s_w[s]=0;
		texpos_edge_grass_trop_evil_thinning_s_e[s]=0;
		texpos_edge_grass_trop_evil_thinning_w_n[s]=0;
		texpos_edge_grass_trop_evil_thinning_w_s[s]=0;
		texpos_edge_grass_trop_evil_thinning_e_n[s]=0;
		texpos_edge_grass_trop_evil_thinning_e_s[s]=0;
		texpos_edge_grass_trop_good_straight_n[s]=0;
		texpos_edge_grass_trop_good_straight_s[s]=0;
		texpos_edge_grass_trop_good_straight_w[s]=0;
		texpos_edge_grass_trop_good_straight_e[s]=0;
		texpos_edge_grass_trop_good_thick_corner_nw[s]=0;
		texpos_edge_grass_trop_good_thick_corner_ne[s]=0;
		texpos_edge_grass_trop_good_thick_corner_sw[s]=0;
		texpos_edge_grass_trop_good_thick_corner_se[s]=0;
		texpos_edge_grass_trop_good_thin_corner_nw[s]=0;
		texpos_edge_grass_trop_good_thin_corner_ne[s]=0;
		texpos_edge_grass_trop_good_thin_corner_sw[s]=0;
		texpos_edge_grass_trop_good_thin_corner_se[s]=0;
		texpos_edge_grass_trop_good_inlet_nsw[s]=0;
		texpos_edge_grass_trop_good_inlet_nse[s]=0;
		texpos_edge_grass_trop_good_inlet_nwe[s]=0;
		texpos_edge_grass_trop_good_inlet_swe[s]=0;
		texpos_edge_grass_trop_good_surround[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_grass_trop_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_grass_trop_good_tiny_corner_nw[s]=0;
		texpos_edge_grass_trop_good_tiny_corner_ne[s]=0;
		texpos_edge_grass_trop_good_tiny_corner_sw[s]=0;
		texpos_edge_grass_trop_good_tiny_corner_se[s]=0;
		texpos_edge_grass_trop_good_thinning_n_w[s]=0;
		texpos_edge_grass_trop_good_thinning_n_e[s]=0;
		texpos_edge_grass_trop_good_thinning_s_w[s]=0;
		texpos_edge_grass_trop_good_thinning_s_e[s]=0;
		texpos_edge_grass_trop_good_thinning_w_n[s]=0;
		texpos_edge_grass_trop_good_thinning_w_s[s]=0;
		texpos_edge_grass_trop_good_thinning_e_n[s]=0;
		texpos_edge_grass_trop_good_thinning_e_s[s]=0;
		texpos_edge_wetland_straight_n[s]=0;
		texpos_edge_wetland_straight_s[s]=0;
		texpos_edge_wetland_straight_w[s]=0;
		texpos_edge_wetland_straight_e[s]=0;
		texpos_edge_wetland_thick_corner_nw[s]=0;
		texpos_edge_wetland_thick_corner_ne[s]=0;
		texpos_edge_wetland_thick_corner_sw[s]=0;
		texpos_edge_wetland_thick_corner_se[s]=0;
		texpos_edge_wetland_thin_corner_nw[s]=0;
		texpos_edge_wetland_thin_corner_ne[s]=0;
		texpos_edge_wetland_thin_corner_sw[s]=0;
		texpos_edge_wetland_thin_corner_se[s]=0;
		texpos_edge_wetland_inlet_nsw[s]=0;
		texpos_edge_wetland_inlet_nse[s]=0;
		texpos_edge_wetland_inlet_nwe[s]=0;
		texpos_edge_wetland_inlet_swe[s]=0;
		texpos_edge_wetland_surround[s]=0;
		texpos_edge_wetland_thin_corner_transition_nw_n[s]=0;
		texpos_edge_wetland_thin_corner_transition_nw_w[s]=0;
		texpos_edge_wetland_thin_corner_transition_ne_n[s]=0;
		texpos_edge_wetland_thin_corner_transition_ne_e[s]=0;
		texpos_edge_wetland_thin_corner_transition_sw_s[s]=0;
		texpos_edge_wetland_thin_corner_transition_sw_w[s]=0;
		texpos_edge_wetland_thin_corner_transition_se_s[s]=0;
		texpos_edge_wetland_thin_corner_transition_se_e[s]=0;
		texpos_edge_wetland_tiny_corner_nw[s]=0;
		texpos_edge_wetland_tiny_corner_ne[s]=0;
		texpos_edge_wetland_tiny_corner_sw[s]=0;
		texpos_edge_wetland_tiny_corner_se[s]=0;
		texpos_edge_wetland_thinning_n_w[s]=0;
		texpos_edge_wetland_thinning_n_e[s]=0;
		texpos_edge_wetland_thinning_s_w[s]=0;
		texpos_edge_wetland_thinning_s_e[s]=0;
		texpos_edge_wetland_thinning_w_n[s]=0;
		texpos_edge_wetland_thinning_w_s[s]=0;
		texpos_edge_wetland_thinning_e_n[s]=0;
		texpos_edge_wetland_thinning_e_s[s]=0;
		texpos_edge_wetland_evil_straight_n[s]=0;
		texpos_edge_wetland_evil_straight_s[s]=0;
		texpos_edge_wetland_evil_straight_w[s]=0;
		texpos_edge_wetland_evil_straight_e[s]=0;
		texpos_edge_wetland_evil_thick_corner_nw[s]=0;
		texpos_edge_wetland_evil_thick_corner_ne[s]=0;
		texpos_edge_wetland_evil_thick_corner_sw[s]=0;
		texpos_edge_wetland_evil_thick_corner_se[s]=0;
		texpos_edge_wetland_evil_thin_corner_nw[s]=0;
		texpos_edge_wetland_evil_thin_corner_ne[s]=0;
		texpos_edge_wetland_evil_thin_corner_sw[s]=0;
		texpos_edge_wetland_evil_thin_corner_se[s]=0;
		texpos_edge_wetland_evil_inlet_nsw[s]=0;
		texpos_edge_wetland_evil_inlet_nse[s]=0;
		texpos_edge_wetland_evil_inlet_nwe[s]=0;
		texpos_edge_wetland_evil_inlet_swe[s]=0;
		texpos_edge_wetland_evil_surround[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_wetland_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_wetland_evil_tiny_corner_nw[s]=0;
		texpos_edge_wetland_evil_tiny_corner_ne[s]=0;
		texpos_edge_wetland_evil_tiny_corner_sw[s]=0;
		texpos_edge_wetland_evil_tiny_corner_se[s]=0;
		texpos_edge_wetland_evil_thinning_n_w[s]=0;
		texpos_edge_wetland_evil_thinning_n_e[s]=0;
		texpos_edge_wetland_evil_thinning_s_w[s]=0;
		texpos_edge_wetland_evil_thinning_s_e[s]=0;
		texpos_edge_wetland_evil_thinning_w_n[s]=0;
		texpos_edge_wetland_evil_thinning_w_s[s]=0;
		texpos_edge_wetland_evil_thinning_e_n[s]=0;
		texpos_edge_wetland_evil_thinning_e_s[s]=0;
		texpos_edge_wetland_good_straight_n[s]=0;
		texpos_edge_wetland_good_straight_s[s]=0;
		texpos_edge_wetland_good_straight_w[s]=0;
		texpos_edge_wetland_good_straight_e[s]=0;
		texpos_edge_wetland_good_thick_corner_nw[s]=0;
		texpos_edge_wetland_good_thick_corner_ne[s]=0;
		texpos_edge_wetland_good_thick_corner_sw[s]=0;
		texpos_edge_wetland_good_thick_corner_se[s]=0;
		texpos_edge_wetland_good_thin_corner_nw[s]=0;
		texpos_edge_wetland_good_thin_corner_ne[s]=0;
		texpos_edge_wetland_good_thin_corner_sw[s]=0;
		texpos_edge_wetland_good_thin_corner_se[s]=0;
		texpos_edge_wetland_good_inlet_nsw[s]=0;
		texpos_edge_wetland_good_inlet_nse[s]=0;
		texpos_edge_wetland_good_inlet_nwe[s]=0;
		texpos_edge_wetland_good_inlet_swe[s]=0;
		texpos_edge_wetland_good_surround[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_wetland_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_wetland_good_tiny_corner_nw[s]=0;
		texpos_edge_wetland_good_tiny_corner_ne[s]=0;
		texpos_edge_wetland_good_tiny_corner_sw[s]=0;
		texpos_edge_wetland_good_tiny_corner_se[s]=0;
		texpos_edge_wetland_good_thinning_n_w[s]=0;
		texpos_edge_wetland_good_thinning_n_e[s]=0;
		texpos_edge_wetland_good_thinning_s_w[s]=0;
		texpos_edge_wetland_good_thinning_s_e[s]=0;
		texpos_edge_wetland_good_thinning_w_n[s]=0;
		texpos_edge_wetland_good_thinning_w_s[s]=0;
		texpos_edge_wetland_good_thinning_e_n[s]=0;
		texpos_edge_wetland_good_thinning_e_s[s]=0;
		texpos_edge_beach_straight_n[s]=0;
		texpos_edge_beach_straight_s[s]=0;
		texpos_edge_beach_straight_w[s]=0;
		texpos_edge_beach_straight_e[s]=0;
		texpos_edge_beach_thick_corner_nw[s]=0;
		texpos_edge_beach_thick_corner_ne[s]=0;
		texpos_edge_beach_thick_corner_sw[s]=0;
		texpos_edge_beach_thick_corner_se[s]=0;
		texpos_edge_beach_thin_corner_nw[s]=0;
		texpos_edge_beach_thin_corner_ne[s]=0;
		texpos_edge_beach_thin_corner_sw[s]=0;
		texpos_edge_beach_thin_corner_se[s]=0;
		texpos_edge_beach_inlet_nsw[s]=0;
		texpos_edge_beach_inlet_nse[s]=0;
		texpos_edge_beach_inlet_nwe[s]=0;
		texpos_edge_beach_inlet_swe[s]=0;
		texpos_edge_beach_surround[s]=0;
		texpos_edge_beach_thin_corner_transition_nw_n[s]=0;
		texpos_edge_beach_thin_corner_transition_nw_w[s]=0;
		texpos_edge_beach_thin_corner_transition_ne_n[s]=0;
		texpos_edge_beach_thin_corner_transition_ne_e[s]=0;
		texpos_edge_beach_thin_corner_transition_sw_s[s]=0;
		texpos_edge_beach_thin_corner_transition_sw_w[s]=0;
		texpos_edge_beach_thin_corner_transition_se_s[s]=0;
		texpos_edge_beach_thin_corner_transition_se_e[s]=0;
		texpos_edge_beach_tiny_corner_nw[s]=0;
		texpos_edge_beach_tiny_corner_ne[s]=0;
		texpos_edge_beach_tiny_corner_sw[s]=0;
		texpos_edge_beach_tiny_corner_se[s]=0;
		texpos_edge_beach_thinning_n_w[s]=0;
		texpos_edge_beach_thinning_n_e[s]=0;
		texpos_edge_beach_thinning_s_w[s]=0;
		texpos_edge_beach_thinning_s_e[s]=0;
		texpos_edge_beach_thinning_w_n[s]=0;
		texpos_edge_beach_thinning_w_s[s]=0;
		texpos_edge_beach_thinning_e_n[s]=0;
		texpos_edge_beach_thinning_e_s[s]=0;
		texpos_edge_beach_evil_straight_n[s]=0;
		texpos_edge_beach_evil_straight_s[s]=0;
		texpos_edge_beach_evil_straight_w[s]=0;
		texpos_edge_beach_evil_straight_e[s]=0;
		texpos_edge_beach_evil_thick_corner_nw[s]=0;
		texpos_edge_beach_evil_thick_corner_ne[s]=0;
		texpos_edge_beach_evil_thick_corner_sw[s]=0;
		texpos_edge_beach_evil_thick_corner_se[s]=0;
		texpos_edge_beach_evil_thin_corner_nw[s]=0;
		texpos_edge_beach_evil_thin_corner_ne[s]=0;
		texpos_edge_beach_evil_thin_corner_sw[s]=0;
		texpos_edge_beach_evil_thin_corner_se[s]=0;
		texpos_edge_beach_evil_inlet_nsw[s]=0;
		texpos_edge_beach_evil_inlet_nse[s]=0;
		texpos_edge_beach_evil_inlet_nwe[s]=0;
		texpos_edge_beach_evil_inlet_swe[s]=0;
		texpos_edge_beach_evil_surround[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_beach_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_beach_evil_tiny_corner_nw[s]=0;
		texpos_edge_beach_evil_tiny_corner_ne[s]=0;
		texpos_edge_beach_evil_tiny_corner_sw[s]=0;
		texpos_edge_beach_evil_tiny_corner_se[s]=0;
		texpos_edge_beach_evil_thinning_n_w[s]=0;
		texpos_edge_beach_evil_thinning_n_e[s]=0;
		texpos_edge_beach_evil_thinning_s_w[s]=0;
		texpos_edge_beach_evil_thinning_s_e[s]=0;
		texpos_edge_beach_evil_thinning_w_n[s]=0;
		texpos_edge_beach_evil_thinning_w_s[s]=0;
		texpos_edge_beach_evil_thinning_e_n[s]=0;
		texpos_edge_beach_evil_thinning_e_s[s]=0;
		texpos_edge_beach_good_straight_n[s]=0;
		texpos_edge_beach_good_straight_s[s]=0;
		texpos_edge_beach_good_straight_w[s]=0;
		texpos_edge_beach_good_straight_e[s]=0;
		texpos_edge_beach_good_thick_corner_nw[s]=0;
		texpos_edge_beach_good_thick_corner_ne[s]=0;
		texpos_edge_beach_good_thick_corner_sw[s]=0;
		texpos_edge_beach_good_thick_corner_se[s]=0;
		texpos_edge_beach_good_thin_corner_nw[s]=0;
		texpos_edge_beach_good_thin_corner_ne[s]=0;
		texpos_edge_beach_good_thin_corner_sw[s]=0;
		texpos_edge_beach_good_thin_corner_se[s]=0;
		texpos_edge_beach_good_inlet_nsw[s]=0;
		texpos_edge_beach_good_inlet_nse[s]=0;
		texpos_edge_beach_good_inlet_nwe[s]=0;
		texpos_edge_beach_good_inlet_swe[s]=0;
		texpos_edge_beach_good_surround[s]=0;
		texpos_edge_beach_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_beach_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_beach_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_beach_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_beach_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_beach_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_beach_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_beach_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_beach_good_tiny_corner_nw[s]=0;
		texpos_edge_beach_good_tiny_corner_ne[s]=0;
		texpos_edge_beach_good_tiny_corner_sw[s]=0;
		texpos_edge_beach_good_tiny_corner_se[s]=0;
		texpos_edge_beach_good_thinning_n_w[s]=0;
		texpos_edge_beach_good_thinning_n_e[s]=0;
		texpos_edge_beach_good_thinning_s_w[s]=0;
		texpos_edge_beach_good_thinning_s_e[s]=0;
		texpos_edge_beach_good_thinning_w_n[s]=0;
		texpos_edge_beach_good_thinning_w_s[s]=0;
		texpos_edge_beach_good_thinning_e_n[s]=0;
		texpos_edge_beach_good_thinning_e_s[s]=0;
		texpos_edge_desert_straight_n[s]=0;
		texpos_edge_desert_straight_s[s]=0;
		texpos_edge_desert_straight_w[s]=0;
		texpos_edge_desert_straight_e[s]=0;
		texpos_edge_desert_thick_corner_nw[s]=0;
		texpos_edge_desert_thick_corner_ne[s]=0;
		texpos_edge_desert_thick_corner_sw[s]=0;
		texpos_edge_desert_thick_corner_se[s]=0;
		texpos_edge_desert_thin_corner_nw[s]=0;
		texpos_edge_desert_thin_corner_ne[s]=0;
		texpos_edge_desert_thin_corner_sw[s]=0;
		texpos_edge_desert_thin_corner_se[s]=0;
		texpos_edge_desert_inlet_nsw[s]=0;
		texpos_edge_desert_inlet_nse[s]=0;
		texpos_edge_desert_inlet_nwe[s]=0;
		texpos_edge_desert_inlet_swe[s]=0;
		texpos_edge_desert_surround[s]=0;
		texpos_edge_desert_thin_corner_transition_nw_n[s]=0;
		texpos_edge_desert_thin_corner_transition_nw_w[s]=0;
		texpos_edge_desert_thin_corner_transition_ne_n[s]=0;
		texpos_edge_desert_thin_corner_transition_ne_e[s]=0;
		texpos_edge_desert_thin_corner_transition_sw_s[s]=0;
		texpos_edge_desert_thin_corner_transition_sw_w[s]=0;
		texpos_edge_desert_thin_corner_transition_se_s[s]=0;
		texpos_edge_desert_thin_corner_transition_se_e[s]=0;
		texpos_edge_desert_tiny_corner_nw[s]=0;
		texpos_edge_desert_tiny_corner_ne[s]=0;
		texpos_edge_desert_tiny_corner_sw[s]=0;
		texpos_edge_desert_tiny_corner_se[s]=0;
		texpos_edge_desert_thinning_n_w[s]=0;
		texpos_edge_desert_thinning_n_e[s]=0;
		texpos_edge_desert_thinning_s_w[s]=0;
		texpos_edge_desert_thinning_s_e[s]=0;
		texpos_edge_desert_thinning_w_n[s]=0;
		texpos_edge_desert_thinning_w_s[s]=0;
		texpos_edge_desert_thinning_e_n[s]=0;
		texpos_edge_desert_thinning_e_s[s]=0;
		texpos_edge_desert_evil_straight_n[s]=0;
		texpos_edge_desert_evil_straight_s[s]=0;
		texpos_edge_desert_evil_straight_w[s]=0;
		texpos_edge_desert_evil_straight_e[s]=0;
		texpos_edge_desert_evil_thick_corner_nw[s]=0;
		texpos_edge_desert_evil_thick_corner_ne[s]=0;
		texpos_edge_desert_evil_thick_corner_sw[s]=0;
		texpos_edge_desert_evil_thick_corner_se[s]=0;
		texpos_edge_desert_evil_thin_corner_nw[s]=0;
		texpos_edge_desert_evil_thin_corner_ne[s]=0;
		texpos_edge_desert_evil_thin_corner_sw[s]=0;
		texpos_edge_desert_evil_thin_corner_se[s]=0;
		texpos_edge_desert_evil_inlet_nsw[s]=0;
		texpos_edge_desert_evil_inlet_nse[s]=0;
		texpos_edge_desert_evil_inlet_nwe[s]=0;
		texpos_edge_desert_evil_inlet_swe[s]=0;
		texpos_edge_desert_evil_surround[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_desert_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_desert_evil_tiny_corner_nw[s]=0;
		texpos_edge_desert_evil_tiny_corner_ne[s]=0;
		texpos_edge_desert_evil_tiny_corner_sw[s]=0;
		texpos_edge_desert_evil_tiny_corner_se[s]=0;
		texpos_edge_desert_evil_thinning_n_w[s]=0;
		texpos_edge_desert_evil_thinning_n_e[s]=0;
		texpos_edge_desert_evil_thinning_s_w[s]=0;
		texpos_edge_desert_evil_thinning_s_e[s]=0;
		texpos_edge_desert_evil_thinning_w_n[s]=0;
		texpos_edge_desert_evil_thinning_w_s[s]=0;
		texpos_edge_desert_evil_thinning_e_n[s]=0;
		texpos_edge_desert_evil_thinning_e_s[s]=0;
		texpos_edge_desert_good_straight_n[s]=0;
		texpos_edge_desert_good_straight_s[s]=0;
		texpos_edge_desert_good_straight_w[s]=0;
		texpos_edge_desert_good_straight_e[s]=0;
		texpos_edge_desert_good_thick_corner_nw[s]=0;
		texpos_edge_desert_good_thick_corner_ne[s]=0;
		texpos_edge_desert_good_thick_corner_sw[s]=0;
		texpos_edge_desert_good_thick_corner_se[s]=0;
		texpos_edge_desert_good_thin_corner_nw[s]=0;
		texpos_edge_desert_good_thin_corner_ne[s]=0;
		texpos_edge_desert_good_thin_corner_sw[s]=0;
		texpos_edge_desert_good_thin_corner_se[s]=0;
		texpos_edge_desert_good_inlet_nsw[s]=0;
		texpos_edge_desert_good_inlet_nse[s]=0;
		texpos_edge_desert_good_inlet_nwe[s]=0;
		texpos_edge_desert_good_inlet_swe[s]=0;
		texpos_edge_desert_good_surround[s]=0;
		texpos_edge_desert_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_desert_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_desert_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_desert_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_desert_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_desert_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_desert_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_desert_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_desert_good_tiny_corner_nw[s]=0;
		texpos_edge_desert_good_tiny_corner_ne[s]=0;
		texpos_edge_desert_good_tiny_corner_sw[s]=0;
		texpos_edge_desert_good_tiny_corner_se[s]=0;
		texpos_edge_desert_good_thinning_n_w[s]=0;
		texpos_edge_desert_good_thinning_n_e[s]=0;
		texpos_edge_desert_good_thinning_s_w[s]=0;
		texpos_edge_desert_good_thinning_s_e[s]=0;
		texpos_edge_desert_good_thinning_w_n[s]=0;
		texpos_edge_desert_good_thinning_w_s[s]=0;
		texpos_edge_desert_good_thinning_e_n[s]=0;
		texpos_edge_desert_good_thinning_e_s[s]=0;

		texpos_edge_sand_desert_straight_n[s]=0;
		texpos_edge_sand_desert_straight_s[s]=0;
		texpos_edge_sand_desert_straight_w[s]=0;
		texpos_edge_sand_desert_straight_e[s]=0;
		texpos_edge_sand_desert_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_thick_corner_se[s]=0;
		texpos_edge_sand_desert_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_thin_corner_se[s]=0;
		texpos_edge_sand_desert_inlet_nsw[s]=0;
		texpos_edge_sand_desert_inlet_nse[s]=0;
		texpos_edge_sand_desert_inlet_nwe[s]=0;
		texpos_edge_sand_desert_inlet_swe[s]=0;
		texpos_edge_sand_desert_surround[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_thinning_n_w[s]=0;
		texpos_edge_sand_desert_thinning_n_e[s]=0;
		texpos_edge_sand_desert_thinning_s_w[s]=0;
		texpos_edge_sand_desert_thinning_s_e[s]=0;
		texpos_edge_sand_desert_thinning_w_n[s]=0;
		texpos_edge_sand_desert_thinning_w_s[s]=0;
		texpos_edge_sand_desert_thinning_e_n[s]=0;
		texpos_edge_sand_desert_thinning_e_s[s]=0;
		texpos_edge_sand_desert_evil_straight_n[s]=0;
		texpos_edge_sand_desert_evil_straight_s[s]=0;
		texpos_edge_sand_desert_evil_straight_w[s]=0;
		texpos_edge_sand_desert_evil_straight_e[s]=0;
		texpos_edge_sand_desert_evil_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_evil_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_evil_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_evil_thick_corner_se[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_se[s]=0;
		texpos_edge_sand_desert_evil_inlet_nsw[s]=0;
		texpos_edge_sand_desert_evil_inlet_nse[s]=0;
		texpos_edge_sand_desert_evil_inlet_nwe[s]=0;
		texpos_edge_sand_desert_evil_inlet_swe[s]=0;
		texpos_edge_sand_desert_evil_surround[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_evil_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_evil_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_evil_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_evil_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_evil_thinning_n_w[s]=0;
		texpos_edge_sand_desert_evil_thinning_n_e[s]=0;
		texpos_edge_sand_desert_evil_thinning_s_w[s]=0;
		texpos_edge_sand_desert_evil_thinning_s_e[s]=0;
		texpos_edge_sand_desert_evil_thinning_w_n[s]=0;
		texpos_edge_sand_desert_evil_thinning_w_s[s]=0;
		texpos_edge_sand_desert_evil_thinning_e_n[s]=0;
		texpos_edge_sand_desert_evil_thinning_e_s[s]=0;
		texpos_edge_sand_desert_good_straight_n[s]=0;
		texpos_edge_sand_desert_good_straight_s[s]=0;
		texpos_edge_sand_desert_good_straight_w[s]=0;
		texpos_edge_sand_desert_good_straight_e[s]=0;
		texpos_edge_sand_desert_good_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_good_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_good_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_good_thick_corner_se[s]=0;
		texpos_edge_sand_desert_good_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_good_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_good_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_good_thin_corner_se[s]=0;
		texpos_edge_sand_desert_good_inlet_nsw[s]=0;
		texpos_edge_sand_desert_good_inlet_nse[s]=0;
		texpos_edge_sand_desert_good_inlet_nwe[s]=0;
		texpos_edge_sand_desert_good_inlet_swe[s]=0;
		texpos_edge_sand_desert_good_surround[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_good_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_good_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_good_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_good_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_good_thinning_n_w[s]=0;
		texpos_edge_sand_desert_good_thinning_n_e[s]=0;
		texpos_edge_sand_desert_good_thinning_s_w[s]=0;
		texpos_edge_sand_desert_good_thinning_s_e[s]=0;
		texpos_edge_sand_desert_good_thinning_w_n[s]=0;
		texpos_edge_sand_desert_good_thinning_w_s[s]=0;
		texpos_edge_sand_desert_good_thinning_e_n[s]=0;
		texpos_edge_sand_desert_good_thinning_e_s[s]=0;

		texpos_edge_sand_desert_yellow_straight_n[s]=0;
		texpos_edge_sand_desert_yellow_straight_s[s]=0;
		texpos_edge_sand_desert_yellow_straight_w[s]=0;
		texpos_edge_sand_desert_yellow_straight_e[s]=0;
		texpos_edge_sand_desert_yellow_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_yellow_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_yellow_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_yellow_thick_corner_se[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_se[s]=0;
		texpos_edge_sand_desert_yellow_inlet_nsw[s]=0;
		texpos_edge_sand_desert_yellow_inlet_nse[s]=0;
		texpos_edge_sand_desert_yellow_inlet_nwe[s]=0;
		texpos_edge_sand_desert_yellow_inlet_swe[s]=0;
		texpos_edge_sand_desert_yellow_surround[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_yellow_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_yellow_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_yellow_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_yellow_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_yellow_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_yellow_thinning_n_w[s]=0;
		texpos_edge_sand_desert_yellow_thinning_n_e[s]=0;
		texpos_edge_sand_desert_yellow_thinning_s_w[s]=0;
		texpos_edge_sand_desert_yellow_thinning_s_e[s]=0;
		texpos_edge_sand_desert_yellow_thinning_w_n[s]=0;
		texpos_edge_sand_desert_yellow_thinning_w_s[s]=0;
		texpos_edge_sand_desert_yellow_thinning_e_n[s]=0;
		texpos_edge_sand_desert_yellow_thinning_e_s[s]=0;
		texpos_edge_sand_desert_white_straight_n[s]=0;
		texpos_edge_sand_desert_white_straight_s[s]=0;
		texpos_edge_sand_desert_white_straight_w[s]=0;
		texpos_edge_sand_desert_white_straight_e[s]=0;
		texpos_edge_sand_desert_white_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_white_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_white_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_white_thick_corner_se[s]=0;
		texpos_edge_sand_desert_white_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_white_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_white_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_white_thin_corner_se[s]=0;
		texpos_edge_sand_desert_white_inlet_nsw[s]=0;
		texpos_edge_sand_desert_white_inlet_nse[s]=0;
		texpos_edge_sand_desert_white_inlet_nwe[s]=0;
		texpos_edge_sand_desert_white_inlet_swe[s]=0;
		texpos_edge_sand_desert_white_surround[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_white_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_white_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_white_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_white_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_white_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_white_thinning_n_w[s]=0;
		texpos_edge_sand_desert_white_thinning_n_e[s]=0;
		texpos_edge_sand_desert_white_thinning_s_w[s]=0;
		texpos_edge_sand_desert_white_thinning_s_e[s]=0;
		texpos_edge_sand_desert_white_thinning_w_n[s]=0;
		texpos_edge_sand_desert_white_thinning_w_s[s]=0;
		texpos_edge_sand_desert_white_thinning_e_n[s]=0;
		texpos_edge_sand_desert_white_thinning_e_s[s]=0;
		texpos_edge_sand_desert_black_straight_n[s]=0;
		texpos_edge_sand_desert_black_straight_s[s]=0;
		texpos_edge_sand_desert_black_straight_w[s]=0;
		texpos_edge_sand_desert_black_straight_e[s]=0;
		texpos_edge_sand_desert_black_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_black_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_black_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_black_thick_corner_se[s]=0;
		texpos_edge_sand_desert_black_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_black_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_black_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_black_thin_corner_se[s]=0;
		texpos_edge_sand_desert_black_inlet_nsw[s]=0;
		texpos_edge_sand_desert_black_inlet_nse[s]=0;
		texpos_edge_sand_desert_black_inlet_nwe[s]=0;
		texpos_edge_sand_desert_black_inlet_swe[s]=0;
		texpos_edge_sand_desert_black_surround[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_black_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_black_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_black_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_black_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_black_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_black_thinning_n_w[s]=0;
		texpos_edge_sand_desert_black_thinning_n_e[s]=0;
		texpos_edge_sand_desert_black_thinning_s_w[s]=0;
		texpos_edge_sand_desert_black_thinning_s_e[s]=0;
		texpos_edge_sand_desert_black_thinning_w_n[s]=0;
		texpos_edge_sand_desert_black_thinning_w_s[s]=0;
		texpos_edge_sand_desert_black_thinning_e_n[s]=0;
		texpos_edge_sand_desert_black_thinning_e_s[s]=0;
		texpos_edge_sand_desert_red_straight_n[s]=0;
		texpos_edge_sand_desert_red_straight_s[s]=0;
		texpos_edge_sand_desert_red_straight_w[s]=0;
		texpos_edge_sand_desert_red_straight_e[s]=0;
		texpos_edge_sand_desert_red_thick_corner_nw[s]=0;
		texpos_edge_sand_desert_red_thick_corner_ne[s]=0;
		texpos_edge_sand_desert_red_thick_corner_sw[s]=0;
		texpos_edge_sand_desert_red_thick_corner_se[s]=0;
		texpos_edge_sand_desert_red_thin_corner_nw[s]=0;
		texpos_edge_sand_desert_red_thin_corner_ne[s]=0;
		texpos_edge_sand_desert_red_thin_corner_sw[s]=0;
		texpos_edge_sand_desert_red_thin_corner_se[s]=0;
		texpos_edge_sand_desert_red_inlet_nsw[s]=0;
		texpos_edge_sand_desert_red_inlet_nse[s]=0;
		texpos_edge_sand_desert_red_inlet_nwe[s]=0;
		texpos_edge_sand_desert_red_inlet_swe[s]=0;
		texpos_edge_sand_desert_red_surround[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_nw_n[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_nw_w[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_ne_n[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_ne_e[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_sw_s[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_sw_w[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_se_s[s]=0;
		texpos_edge_sand_desert_red_thin_corner_transition_se_e[s]=0;
		texpos_edge_sand_desert_red_tiny_corner_nw[s]=0;
		texpos_edge_sand_desert_red_tiny_corner_ne[s]=0;
		texpos_edge_sand_desert_red_tiny_corner_sw[s]=0;
		texpos_edge_sand_desert_red_tiny_corner_se[s]=0;
		texpos_edge_sand_desert_red_thinning_n_w[s]=0;
		texpos_edge_sand_desert_red_thinning_n_e[s]=0;
		texpos_edge_sand_desert_red_thinning_s_w[s]=0;
		texpos_edge_sand_desert_red_thinning_s_e[s]=0;
		texpos_edge_sand_desert_red_thinning_w_n[s]=0;
		texpos_edge_sand_desert_red_thinning_w_s[s]=0;
		texpos_edge_sand_desert_red_thinning_e_n[s]=0;
		texpos_edge_sand_desert_red_thinning_e_s[s]=0;

		texpos_edge_tundra_straight_n[s]=0;
		texpos_edge_tundra_straight_s[s]=0;
		texpos_edge_tundra_straight_w[s]=0;
		texpos_edge_tundra_straight_e[s]=0;
		texpos_edge_tundra_thick_corner_nw[s]=0;
		texpos_edge_tundra_thick_corner_ne[s]=0;
		texpos_edge_tundra_thick_corner_sw[s]=0;
		texpos_edge_tundra_thick_corner_se[s]=0;
		texpos_edge_tundra_thin_corner_nw[s]=0;
		texpos_edge_tundra_thin_corner_ne[s]=0;
		texpos_edge_tundra_thin_corner_sw[s]=0;
		texpos_edge_tundra_thin_corner_se[s]=0;
		texpos_edge_tundra_inlet_nsw[s]=0;
		texpos_edge_tundra_inlet_nse[s]=0;
		texpos_edge_tundra_inlet_nwe[s]=0;
		texpos_edge_tundra_inlet_swe[s]=0;
		texpos_edge_tundra_surround[s]=0;
		texpos_edge_tundra_thin_corner_transition_nw_n[s]=0;
		texpos_edge_tundra_thin_corner_transition_nw_w[s]=0;
		texpos_edge_tundra_thin_corner_transition_ne_n[s]=0;
		texpos_edge_tundra_thin_corner_transition_ne_e[s]=0;
		texpos_edge_tundra_thin_corner_transition_sw_s[s]=0;
		texpos_edge_tundra_thin_corner_transition_sw_w[s]=0;
		texpos_edge_tundra_thin_corner_transition_se_s[s]=0;
		texpos_edge_tundra_thin_corner_transition_se_e[s]=0;
		texpos_edge_tundra_tiny_corner_nw[s]=0;
		texpos_edge_tundra_tiny_corner_ne[s]=0;
		texpos_edge_tundra_tiny_corner_sw[s]=0;
		texpos_edge_tundra_tiny_corner_se[s]=0;
		texpos_edge_tundra_thinning_n_w[s]=0;
		texpos_edge_tundra_thinning_n_e[s]=0;
		texpos_edge_tundra_thinning_s_w[s]=0;
		texpos_edge_tundra_thinning_s_e[s]=0;
		texpos_edge_tundra_thinning_w_n[s]=0;
		texpos_edge_tundra_thinning_w_s[s]=0;
		texpos_edge_tundra_thinning_e_n[s]=0;
		texpos_edge_tundra_thinning_e_s[s]=0;
		texpos_edge_tundra_evil_straight_n[s]=0;
		texpos_edge_tundra_evil_straight_s[s]=0;
		texpos_edge_tundra_evil_straight_w[s]=0;
		texpos_edge_tundra_evil_straight_e[s]=0;
		texpos_edge_tundra_evil_thick_corner_nw[s]=0;
		texpos_edge_tundra_evil_thick_corner_ne[s]=0;
		texpos_edge_tundra_evil_thick_corner_sw[s]=0;
		texpos_edge_tundra_evil_thick_corner_se[s]=0;
		texpos_edge_tundra_evil_thin_corner_nw[s]=0;
		texpos_edge_tundra_evil_thin_corner_ne[s]=0;
		texpos_edge_tundra_evil_thin_corner_sw[s]=0;
		texpos_edge_tundra_evil_thin_corner_se[s]=0;
		texpos_edge_tundra_evil_inlet_nsw[s]=0;
		texpos_edge_tundra_evil_inlet_nse[s]=0;
		texpos_edge_tundra_evil_inlet_nwe[s]=0;
		texpos_edge_tundra_evil_inlet_swe[s]=0;
		texpos_edge_tundra_evil_surround[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_tundra_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_tundra_evil_tiny_corner_nw[s]=0;
		texpos_edge_tundra_evil_tiny_corner_ne[s]=0;
		texpos_edge_tundra_evil_tiny_corner_sw[s]=0;
		texpos_edge_tundra_evil_tiny_corner_se[s]=0;
		texpos_edge_tundra_evil_thinning_n_w[s]=0;
		texpos_edge_tundra_evil_thinning_n_e[s]=0;
		texpos_edge_tundra_evil_thinning_s_w[s]=0;
		texpos_edge_tundra_evil_thinning_s_e[s]=0;
		texpos_edge_tundra_evil_thinning_w_n[s]=0;
		texpos_edge_tundra_evil_thinning_w_s[s]=0;
		texpos_edge_tundra_evil_thinning_e_n[s]=0;
		texpos_edge_tundra_evil_thinning_e_s[s]=0;
		texpos_edge_tundra_good_straight_n[s]=0;
		texpos_edge_tundra_good_straight_s[s]=0;
		texpos_edge_tundra_good_straight_w[s]=0;
		texpos_edge_tundra_good_straight_e[s]=0;
		texpos_edge_tundra_good_thick_corner_nw[s]=0;
		texpos_edge_tundra_good_thick_corner_ne[s]=0;
		texpos_edge_tundra_good_thick_corner_sw[s]=0;
		texpos_edge_tundra_good_thick_corner_se[s]=0;
		texpos_edge_tundra_good_thin_corner_nw[s]=0;
		texpos_edge_tundra_good_thin_corner_ne[s]=0;
		texpos_edge_tundra_good_thin_corner_sw[s]=0;
		texpos_edge_tundra_good_thin_corner_se[s]=0;
		texpos_edge_tundra_good_inlet_nsw[s]=0;
		texpos_edge_tundra_good_inlet_nse[s]=0;
		texpos_edge_tundra_good_inlet_nwe[s]=0;
		texpos_edge_tundra_good_inlet_swe[s]=0;
		texpos_edge_tundra_good_surround[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_tundra_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_tundra_good_tiny_corner_nw[s]=0;
		texpos_edge_tundra_good_tiny_corner_ne[s]=0;
		texpos_edge_tundra_good_tiny_corner_sw[s]=0;
		texpos_edge_tundra_good_tiny_corner_se[s]=0;
		texpos_edge_tundra_good_thinning_n_w[s]=0;
		texpos_edge_tundra_good_thinning_n_e[s]=0;
		texpos_edge_tundra_good_thinning_s_w[s]=0;
		texpos_edge_tundra_good_thinning_s_e[s]=0;
		texpos_edge_tundra_good_thinning_w_n[s]=0;
		texpos_edge_tundra_good_thinning_w_s[s]=0;
		texpos_edge_tundra_good_thinning_e_n[s]=0;
		texpos_edge_tundra_good_thinning_e_s[s]=0;

		texpos_edge_glacier_straight_n[s]=0;
		texpos_edge_glacier_straight_s[s]=0;
		texpos_edge_glacier_straight_w[s]=0;
		texpos_edge_glacier_straight_e[s]=0;
		texpos_edge_glacier_thick_corner_nw[s]=0;
		texpos_edge_glacier_thick_corner_ne[s]=0;
		texpos_edge_glacier_thick_corner_sw[s]=0;
		texpos_edge_glacier_thick_corner_se[s]=0;
		texpos_edge_glacier_thin_corner_nw[s]=0;
		texpos_edge_glacier_thin_corner_ne[s]=0;
		texpos_edge_glacier_thin_corner_sw[s]=0;
		texpos_edge_glacier_thin_corner_se[s]=0;
		texpos_edge_glacier_inlet_nsw[s]=0;
		texpos_edge_glacier_inlet_nse[s]=0;
		texpos_edge_glacier_inlet_nwe[s]=0;
		texpos_edge_glacier_inlet_swe[s]=0;
		texpos_edge_glacier_surround[s]=0;
		texpos_edge_glacier_thin_corner_transition_nw_n[s]=0;
		texpos_edge_glacier_thin_corner_transition_nw_w[s]=0;
		texpos_edge_glacier_thin_corner_transition_ne_n[s]=0;
		texpos_edge_glacier_thin_corner_transition_ne_e[s]=0;
		texpos_edge_glacier_thin_corner_transition_sw_s[s]=0;
		texpos_edge_glacier_thin_corner_transition_sw_w[s]=0;
		texpos_edge_glacier_thin_corner_transition_se_s[s]=0;
		texpos_edge_glacier_thin_corner_transition_se_e[s]=0;
		texpos_edge_glacier_tiny_corner_nw[s]=0;
		texpos_edge_glacier_tiny_corner_ne[s]=0;
		texpos_edge_glacier_tiny_corner_sw[s]=0;
		texpos_edge_glacier_tiny_corner_se[s]=0;
		texpos_edge_glacier_thinning_n_w[s]=0;
		texpos_edge_glacier_thinning_n_e[s]=0;
		texpos_edge_glacier_thinning_s_w[s]=0;
		texpos_edge_glacier_thinning_s_e[s]=0;
		texpos_edge_glacier_thinning_w_n[s]=0;
		texpos_edge_glacier_thinning_w_s[s]=0;
		texpos_edge_glacier_thinning_e_n[s]=0;
		texpos_edge_glacier_thinning_e_s[s]=0;
		texpos_edge_glacier_evil_straight_n[s]=0;
		texpos_edge_glacier_evil_straight_s[s]=0;
		texpos_edge_glacier_evil_straight_w[s]=0;
		texpos_edge_glacier_evil_straight_e[s]=0;
		texpos_edge_glacier_evil_thick_corner_nw[s]=0;
		texpos_edge_glacier_evil_thick_corner_ne[s]=0;
		texpos_edge_glacier_evil_thick_corner_sw[s]=0;
		texpos_edge_glacier_evil_thick_corner_se[s]=0;
		texpos_edge_glacier_evil_thin_corner_nw[s]=0;
		texpos_edge_glacier_evil_thin_corner_ne[s]=0;
		texpos_edge_glacier_evil_thin_corner_sw[s]=0;
		texpos_edge_glacier_evil_thin_corner_se[s]=0;
		texpos_edge_glacier_evil_inlet_nsw[s]=0;
		texpos_edge_glacier_evil_inlet_nse[s]=0;
		texpos_edge_glacier_evil_inlet_nwe[s]=0;
		texpos_edge_glacier_evil_inlet_swe[s]=0;
		texpos_edge_glacier_evil_surround[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_glacier_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_glacier_evil_tiny_corner_nw[s]=0;
		texpos_edge_glacier_evil_tiny_corner_ne[s]=0;
		texpos_edge_glacier_evil_tiny_corner_sw[s]=0;
		texpos_edge_glacier_evil_tiny_corner_se[s]=0;
		texpos_edge_glacier_evil_thinning_n_w[s]=0;
		texpos_edge_glacier_evil_thinning_n_e[s]=0;
		texpos_edge_glacier_evil_thinning_s_w[s]=0;
		texpos_edge_glacier_evil_thinning_s_e[s]=0;
		texpos_edge_glacier_evil_thinning_w_n[s]=0;
		texpos_edge_glacier_evil_thinning_w_s[s]=0;
		texpos_edge_glacier_evil_thinning_e_n[s]=0;
		texpos_edge_glacier_evil_thinning_e_s[s]=0;
		texpos_edge_glacier_good_straight_n[s]=0;
		texpos_edge_glacier_good_straight_s[s]=0;
		texpos_edge_glacier_good_straight_w[s]=0;
		texpos_edge_glacier_good_straight_e[s]=0;
		texpos_edge_glacier_good_thick_corner_nw[s]=0;
		texpos_edge_glacier_good_thick_corner_ne[s]=0;
		texpos_edge_glacier_good_thick_corner_sw[s]=0;
		texpos_edge_glacier_good_thick_corner_se[s]=0;
		texpos_edge_glacier_good_thin_corner_nw[s]=0;
		texpos_edge_glacier_good_thin_corner_ne[s]=0;
		texpos_edge_glacier_good_thin_corner_sw[s]=0;
		texpos_edge_glacier_good_thin_corner_se[s]=0;
		texpos_edge_glacier_good_inlet_nsw[s]=0;
		texpos_edge_glacier_good_inlet_nse[s]=0;
		texpos_edge_glacier_good_inlet_nwe[s]=0;
		texpos_edge_glacier_good_inlet_swe[s]=0;
		texpos_edge_glacier_good_surround[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_glacier_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_glacier_good_tiny_corner_nw[s]=0;
		texpos_edge_glacier_good_tiny_corner_ne[s]=0;
		texpos_edge_glacier_good_tiny_corner_sw[s]=0;
		texpos_edge_glacier_good_tiny_corner_se[s]=0;
		texpos_edge_glacier_good_thinning_n_w[s]=0;
		texpos_edge_glacier_good_thinning_n_e[s]=0;
		texpos_edge_glacier_good_thinning_s_w[s]=0;
		texpos_edge_glacier_good_thinning_s_e[s]=0;
		texpos_edge_glacier_good_thinning_w_n[s]=0;
		texpos_edge_glacier_good_thinning_w_s[s]=0;
		texpos_edge_glacier_good_thinning_e_n[s]=0;
		texpos_edge_glacier_good_thinning_e_s[s]=0;
		texpos_edge_glacier_evilsav_straight_n[s]=0;
		texpos_edge_glacier_evilsav_straight_s[s]=0;
		texpos_edge_glacier_evilsav_straight_w[s]=0;
		texpos_edge_glacier_evilsav_straight_e[s]=0;
		texpos_edge_glacier_evilsav_thick_corner_nw[s]=0;
		texpos_edge_glacier_evilsav_thick_corner_ne[s]=0;
		texpos_edge_glacier_evilsav_thick_corner_sw[s]=0;
		texpos_edge_glacier_evilsav_thick_corner_se[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_nw[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_ne[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_sw[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_se[s]=0;
		texpos_edge_glacier_evilsav_inlet_nsw[s]=0;
		texpos_edge_glacier_evilsav_inlet_nse[s]=0;
		texpos_edge_glacier_evilsav_inlet_nwe[s]=0;
		texpos_edge_glacier_evilsav_inlet_swe[s]=0;
		texpos_edge_glacier_evilsav_surround[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_nw_n[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_nw_w[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_ne_n[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_ne_e[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_sw_s[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_sw_w[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_se_s[s]=0;
		texpos_edge_glacier_evilsav_thin_corner_transition_se_e[s]=0;
		texpos_edge_glacier_evilsav_tiny_corner_nw[s]=0;
		texpos_edge_glacier_evilsav_tiny_corner_ne[s]=0;
		texpos_edge_glacier_evilsav_tiny_corner_sw[s]=0;
		texpos_edge_glacier_evilsav_tiny_corner_se[s]=0;
		texpos_edge_glacier_evilsav_thinning_n_w[s]=0;
		texpos_edge_glacier_evilsav_thinning_n_e[s]=0;
		texpos_edge_glacier_evilsav_thinning_s_w[s]=0;
		texpos_edge_glacier_evilsav_thinning_s_e[s]=0;
		texpos_edge_glacier_evilsav_thinning_w_n[s]=0;
		texpos_edge_glacier_evilsav_thinning_w_s[s]=0;
		texpos_edge_glacier_evilsav_thinning_e_n[s]=0;
		texpos_edge_glacier_evilsav_thinning_e_s[s]=0;
		texpos_edge_glacier_goodsav_straight_n[s]=0;
		texpos_edge_glacier_goodsav_straight_s[s]=0;
		texpos_edge_glacier_goodsav_straight_w[s]=0;
		texpos_edge_glacier_goodsav_straight_e[s]=0;
		texpos_edge_glacier_goodsav_thick_corner_nw[s]=0;
		texpos_edge_glacier_goodsav_thick_corner_ne[s]=0;
		texpos_edge_glacier_goodsav_thick_corner_sw[s]=0;
		texpos_edge_glacier_goodsav_thick_corner_se[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_nw[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_ne[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_sw[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_se[s]=0;
		texpos_edge_glacier_goodsav_inlet_nsw[s]=0;
		texpos_edge_glacier_goodsav_inlet_nse[s]=0;
		texpos_edge_glacier_goodsav_inlet_nwe[s]=0;
		texpos_edge_glacier_goodsav_inlet_swe[s]=0;
		texpos_edge_glacier_goodsav_surround[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_nw_n[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_nw_w[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_ne_n[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_ne_e[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_sw_s[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_sw_w[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_se_s[s]=0;
		texpos_edge_glacier_goodsav_thin_corner_transition_se_e[s]=0;
		texpos_edge_glacier_goodsav_tiny_corner_nw[s]=0;
		texpos_edge_glacier_goodsav_tiny_corner_ne[s]=0;
		texpos_edge_glacier_goodsav_tiny_corner_sw[s]=0;
		texpos_edge_glacier_goodsav_tiny_corner_se[s]=0;
		texpos_edge_glacier_goodsav_thinning_n_w[s]=0;
		texpos_edge_glacier_goodsav_thinning_n_e[s]=0;
		texpos_edge_glacier_goodsav_thinning_s_w[s]=0;
		texpos_edge_glacier_goodsav_thinning_s_e[s]=0;
		texpos_edge_glacier_goodsav_thinning_w_n[s]=0;
		texpos_edge_glacier_goodsav_thinning_w_s[s]=0;
		texpos_edge_glacier_goodsav_thinning_e_n[s]=0;
		texpos_edge_glacier_goodsav_thinning_e_s[s]=0;

		texpos_edge_water_straight_n[s]=0;
		texpos_edge_water_straight_s[s]=0;
		texpos_edge_water_straight_w[s]=0;
		texpos_edge_water_straight_e[s]=0;
		texpos_edge_water_thick_corner_nw[s]=0;
		texpos_edge_water_thick_corner_ne[s]=0;
		texpos_edge_water_thick_corner_sw[s]=0;
		texpos_edge_water_thick_corner_se[s]=0;
		texpos_edge_water_thin_corner_nw[s]=0;
		texpos_edge_water_thin_corner_ne[s]=0;
		texpos_edge_water_thin_corner_sw[s]=0;
		texpos_edge_water_thin_corner_se[s]=0;
		texpos_edge_water_inlet_nsw[s]=0;
		texpos_edge_water_inlet_nse[s]=0;
		texpos_edge_water_inlet_nwe[s]=0;
		texpos_edge_water_inlet_swe[s]=0;
		texpos_edge_water_surround[s]=0;
		texpos_edge_water_thin_corner_transition_nw_n[s]=0;
		texpos_edge_water_thin_corner_transition_nw_w[s]=0;
		texpos_edge_water_thin_corner_transition_ne_n[s]=0;
		texpos_edge_water_thin_corner_transition_ne_e[s]=0;
		texpos_edge_water_thin_corner_transition_sw_s[s]=0;
		texpos_edge_water_thin_corner_transition_sw_w[s]=0;
		texpos_edge_water_thin_corner_transition_se_s[s]=0;
		texpos_edge_water_thin_corner_transition_se_e[s]=0;
		texpos_edge_water_tiny_corner_nw[s]=0;
		texpos_edge_water_tiny_corner_ne[s]=0;
		texpos_edge_water_tiny_corner_sw[s]=0;
		texpos_edge_water_tiny_corner_se[s]=0;
		texpos_edge_water_thinning_n_w[s]=0;
		texpos_edge_water_thinning_n_e[s]=0;
		texpos_edge_water_thinning_s_w[s]=0;
		texpos_edge_water_thinning_s_e[s]=0;
		texpos_edge_water_thinning_w_n[s]=0;
		texpos_edge_water_thinning_w_s[s]=0;
		texpos_edge_water_thinning_e_n[s]=0;
		texpos_edge_water_thinning_e_s[s]=0;
		texpos_edge_water_evil_straight_n[s]=0;
		texpos_edge_water_evil_straight_s[s]=0;
		texpos_edge_water_evil_straight_w[s]=0;
		texpos_edge_water_evil_straight_e[s]=0;
		texpos_edge_water_evil_thick_corner_nw[s]=0;
		texpos_edge_water_evil_thick_corner_ne[s]=0;
		texpos_edge_water_evil_thick_corner_sw[s]=0;
		texpos_edge_water_evil_thick_corner_se[s]=0;
		texpos_edge_water_evil_thin_corner_nw[s]=0;
		texpos_edge_water_evil_thin_corner_ne[s]=0;
		texpos_edge_water_evil_thin_corner_sw[s]=0;
		texpos_edge_water_evil_thin_corner_se[s]=0;
		texpos_edge_water_evil_inlet_nsw[s]=0;
		texpos_edge_water_evil_inlet_nse[s]=0;
		texpos_edge_water_evil_inlet_nwe[s]=0;
		texpos_edge_water_evil_inlet_swe[s]=0;
		texpos_edge_water_evil_surround[s]=0;
		texpos_edge_water_evil_thin_corner_transition_nw_n[s]=0;
		texpos_edge_water_evil_thin_corner_transition_nw_w[s]=0;
		texpos_edge_water_evil_thin_corner_transition_ne_n[s]=0;
		texpos_edge_water_evil_thin_corner_transition_ne_e[s]=0;
		texpos_edge_water_evil_thin_corner_transition_sw_s[s]=0;
		texpos_edge_water_evil_thin_corner_transition_sw_w[s]=0;
		texpos_edge_water_evil_thin_corner_transition_se_s[s]=0;
		texpos_edge_water_evil_thin_corner_transition_se_e[s]=0;
		texpos_edge_water_evil_tiny_corner_nw[s]=0;
		texpos_edge_water_evil_tiny_corner_ne[s]=0;
		texpos_edge_water_evil_tiny_corner_sw[s]=0;
		texpos_edge_water_evil_tiny_corner_se[s]=0;
		texpos_edge_water_evil_thinning_n_w[s]=0;
		texpos_edge_water_evil_thinning_n_e[s]=0;
		texpos_edge_water_evil_thinning_s_w[s]=0;
		texpos_edge_water_evil_thinning_s_e[s]=0;
		texpos_edge_water_evil_thinning_w_n[s]=0;
		texpos_edge_water_evil_thinning_w_s[s]=0;
		texpos_edge_water_evil_thinning_e_n[s]=0;
		texpos_edge_water_evil_thinning_e_s[s]=0;
		texpos_edge_water_good_straight_n[s]=0;
		texpos_edge_water_good_straight_s[s]=0;
		texpos_edge_water_good_straight_w[s]=0;
		texpos_edge_water_good_straight_e[s]=0;
		texpos_edge_water_good_thick_corner_nw[s]=0;
		texpos_edge_water_good_thick_corner_ne[s]=0;
		texpos_edge_water_good_thick_corner_sw[s]=0;
		texpos_edge_water_good_thick_corner_se[s]=0;
		texpos_edge_water_good_thin_corner_nw[s]=0;
		texpos_edge_water_good_thin_corner_ne[s]=0;
		texpos_edge_water_good_thin_corner_sw[s]=0;
		texpos_edge_water_good_thin_corner_se[s]=0;
		texpos_edge_water_good_inlet_nsw[s]=0;
		texpos_edge_water_good_inlet_nse[s]=0;
		texpos_edge_water_good_inlet_nwe[s]=0;
		texpos_edge_water_good_inlet_swe[s]=0;
		texpos_edge_water_good_surround[s]=0;
		texpos_edge_water_good_thin_corner_transition_nw_n[s]=0;
		texpos_edge_water_good_thin_corner_transition_nw_w[s]=0;
		texpos_edge_water_good_thin_corner_transition_ne_n[s]=0;
		texpos_edge_water_good_thin_corner_transition_ne_e[s]=0;
		texpos_edge_water_good_thin_corner_transition_sw_s[s]=0;
		texpos_edge_water_good_thin_corner_transition_sw_w[s]=0;
		texpos_edge_water_good_thin_corner_transition_se_s[s]=0;
		texpos_edge_water_good_thin_corner_transition_se_e[s]=0;
		texpos_edge_water_good_tiny_corner_nw[s]=0;
		texpos_edge_water_good_tiny_corner_ne[s]=0;
		texpos_edge_water_good_tiny_corner_sw[s]=0;
		texpos_edge_water_good_tiny_corner_se[s]=0;
		texpos_edge_water_good_thinning_n_w[s]=0;
		texpos_edge_water_good_thinning_n_e[s]=0;
		texpos_edge_water_good_thinning_s_w[s]=0;
		texpos_edge_water_good_thinning_s_e[s]=0;
		texpos_edge_water_good_thinning_w_n[s]=0;
		texpos_edge_water_good_thinning_w_s[s]=0;
		texpos_edge_water_good_thinning_e_n[s]=0;
		texpos_edge_water_good_thinning_e_s[s]=0;

		texpos_edge_beach_line_straight_n[s]=0;
		texpos_edge_beach_line_straight_s[s]=0;
		texpos_edge_beach_line_straight_w[s]=0;
		texpos_edge_beach_line_straight_e[s]=0;
		texpos_edge_beach_line_thick_corner_nw[s]=0;
		texpos_edge_beach_line_thick_corner_ne[s]=0;
		texpos_edge_beach_line_thick_corner_sw[s]=0;
		texpos_edge_beach_line_thick_corner_se[s]=0;
		texpos_edge_beach_line_thin_corner_nw[s]=0;
		texpos_edge_beach_line_thin_corner_ne[s]=0;
		texpos_edge_beach_line_thin_corner_sw[s]=0;
		texpos_edge_beach_line_thin_corner_se[s]=0;
		texpos_edge_beach_line_inlet_nsw[s]=0;
		texpos_edge_beach_line_inlet_nse[s]=0;
		texpos_edge_beach_line_inlet_nwe[s]=0;
		texpos_edge_beach_line_inlet_swe[s]=0;
		texpos_edge_beach_line_surround[s]=0;
		texpos_edge_beach_line_thin_corner_transition_nw_n[s]=0;
		texpos_edge_beach_line_thin_corner_transition_nw_w[s]=0;
		texpos_edge_beach_line_thin_corner_transition_ne_n[s]=0;
		texpos_edge_beach_line_thin_corner_transition_ne_e[s]=0;
		texpos_edge_beach_line_thin_corner_transition_sw_s[s]=0;
		texpos_edge_beach_line_thin_corner_transition_sw_w[s]=0;
		texpos_edge_beach_line_thin_corner_transition_se_s[s]=0;
		texpos_edge_beach_line_thin_corner_transition_se_e[s]=0;
		texpos_edge_beach_line_tiny_corner_nw[s]=0;
		texpos_edge_beach_line_tiny_corner_ne[s]=0;
		texpos_edge_beach_line_tiny_corner_sw[s]=0;
		texpos_edge_beach_line_tiny_corner_se[s]=0;
		texpos_edge_beach_line_thinning_n_w[s]=0;
		texpos_edge_beach_line_thinning_n_e[s]=0;
		texpos_edge_beach_line_thinning_s_w[s]=0;
		texpos_edge_beach_line_thinning_s_e[s]=0;
		texpos_edge_beach_line_thinning_w_n[s]=0;
		texpos_edge_beach_line_thinning_w_s[s]=0;
		texpos_edge_beach_line_thinning_e_n[s]=0;
		texpos_edge_beach_line_thinning_e_s[s]=0;

		texpos_edge_site_muddy_straight_n[s]=0;
		texpos_edge_site_muddy_straight_s[s]=0;
		texpos_edge_site_muddy_straight_w[s]=0;
		texpos_edge_site_muddy_straight_e[s]=0;
		texpos_edge_site_muddy_thick_corner_nw[s]=0;
		texpos_edge_site_muddy_thick_corner_ne[s]=0;
		texpos_edge_site_muddy_thick_corner_sw[s]=0;
		texpos_edge_site_muddy_thick_corner_se[s]=0;
		texpos_edge_site_muddy_thin_corner_nw[s]=0;
		texpos_edge_site_muddy_thin_corner_ne[s]=0;
		texpos_edge_site_muddy_thin_corner_sw[s]=0;
		texpos_edge_site_muddy_thin_corner_se[s]=0;
		texpos_edge_site_muddy_inlet_nsw[s]=0;
		texpos_edge_site_muddy_inlet_nse[s]=0;
		texpos_edge_site_muddy_inlet_nwe[s]=0;
		texpos_edge_site_muddy_inlet_swe[s]=0;
		texpos_edge_site_muddy_surround[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_nw_n[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_nw_w[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_ne_n[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_ne_e[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_sw_s[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_sw_w[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_se_s[s]=0;
		texpos_edge_site_muddy_thin_corner_transition_se_e[s]=0;
		texpos_edge_site_muddy_tiny_corner_nw[s]=0;
		texpos_edge_site_muddy_tiny_corner_ne[s]=0;
		texpos_edge_site_muddy_tiny_corner_sw[s]=0;
		texpos_edge_site_muddy_tiny_corner_se[s]=0;
		texpos_edge_site_muddy_thinning_n_w[s]=0;
		texpos_edge_site_muddy_thinning_n_e[s]=0;
		texpos_edge_site_muddy_thinning_s_w[s]=0;
		texpos_edge_site_muddy_thinning_s_e[s]=0;
		texpos_edge_site_muddy_thinning_w_n[s]=0;
		texpos_edge_site_muddy_thinning_w_s[s]=0;
		texpos_edge_site_muddy_thinning_e_n[s]=0;
		texpos_edge_site_muddy_thinning_e_s[s]=0;
		texpos_edge_site_grassy_straight_n[s]=0;
		texpos_edge_site_grassy_straight_s[s]=0;
		texpos_edge_site_grassy_straight_w[s]=0;
		texpos_edge_site_grassy_straight_e[s]=0;
		texpos_edge_site_grassy_thick_corner_nw[s]=0;
		texpos_edge_site_grassy_thick_corner_ne[s]=0;
		texpos_edge_site_grassy_thick_corner_sw[s]=0;
		texpos_edge_site_grassy_thick_corner_se[s]=0;
		texpos_edge_site_grassy_thin_corner_nw[s]=0;
		texpos_edge_site_grassy_thin_corner_ne[s]=0;
		texpos_edge_site_grassy_thin_corner_sw[s]=0;
		texpos_edge_site_grassy_thin_corner_se[s]=0;
		texpos_edge_site_grassy_inlet_nsw[s]=0;
		texpos_edge_site_grassy_inlet_nse[s]=0;
		texpos_edge_site_grassy_inlet_nwe[s]=0;
		texpos_edge_site_grassy_inlet_swe[s]=0;
		texpos_edge_site_grassy_surround[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_nw_n[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_nw_w[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_ne_n[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_ne_e[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_sw_s[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_sw_w[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_se_s[s]=0;
		texpos_edge_site_grassy_thin_corner_transition_se_e[s]=0;
		texpos_edge_site_grassy_tiny_corner_nw[s]=0;
		texpos_edge_site_grassy_tiny_corner_ne[s]=0;
		texpos_edge_site_grassy_tiny_corner_sw[s]=0;
		texpos_edge_site_grassy_tiny_corner_se[s]=0;
		texpos_edge_site_grassy_thinning_n_w[s]=0;
		texpos_edge_site_grassy_thinning_n_e[s]=0;
		texpos_edge_site_grassy_thinning_s_w[s]=0;
		texpos_edge_site_grassy_thinning_s_e[s]=0;
		texpos_edge_site_grassy_thinning_w_n[s]=0;
		texpos_edge_site_grassy_thinning_w_s[s]=0;
		texpos_edge_site_grassy_thinning_e_n[s]=0;
		texpos_edge_site_grassy_thinning_e_s[s]=0;

		texpos_site_map_crops_fallow[s]=0;
		texpos_site_map_inner_yard[s]=0;
		texpos_site_map_pasture[s]=0;
		texpos_site_map_meadow[s]=0;
		}
	for(s=0;s<REGION_SQUARE_MOUNTAIN_NUM;++s)
		{
		texpos_mountain_peak[s]=0;
		texpos_volcano[s]=0;
		texpos_mountain_high[s]=0;
		texpos_mountain_mid[s]=0;
		texpos_mountain_low[s]=0;
		texpos_mountain_peak_evilsav[s]=0;
		texpos_volcano_evilsav[s]=0;
		texpos_mountain_high_evilsav[s]=0;
		texpos_mountain_mid_evilsav[s]=0;
		texpos_mountain_low_evilsav[s]=0;
		texpos_mountain_peak_evil[s]=0;
		texpos_volcano_evil[s]=0;
		texpos_mountain_high_evil[s]=0;
		texpos_mountain_mid_evil[s]=0;
		texpos_mountain_low_evil[s]=0;
		texpos_mountain_peak_goodsav[s]=0;
		texpos_volcano_goodsav[s]=0;
		texpos_mountain_high_goodsav[s]=0;
		texpos_mountain_mid_goodsav[s]=0;
		texpos_mountain_low_goodsav[s]=0;
		texpos_mountain_peak_good[s]=0;
		texpos_volcano_good[s]=0;
		texpos_mountain_high_good[s]=0;
		texpos_mountain_mid_good[s]=0;
		texpos_mountain_low_good[s]=0;
		int32_t c;
		for(c=0;c<9;++c)
			{
			texpos_mountain_peak_c[s][c]=0;
			texpos_volcano_c[s][c]=0;
			texpos_mountain_high_c[s][c]=0;
			texpos_mountain_mid_c[s][c]=0;
			texpos_mountain_low_c[s][c]=0;
			texpos_mountain_peak_evilsav_c[s][c]=0;
			texpos_volcano_evilsav_c[s][c]=0;
			texpos_mountain_high_evilsav_c[s][c]=0;
			texpos_mountain_mid_evilsav_c[s][c]=0;
			texpos_mountain_low_evilsav_c[s][c]=0;
			texpos_mountain_peak_evil_c[s][c]=0;
			texpos_volcano_evil_c[s][c]=0;
			texpos_mountain_high_evil_c[s][c]=0;
			texpos_mountain_mid_evil_c[s][c]=0;
			texpos_mountain_low_evil_c[s][c]=0;
			texpos_mountain_peak_goodsav_c[s][c]=0;
			texpos_volcano_goodsav_c[s][c]=0;
			texpos_mountain_high_goodsav_c[s][c]=0;
			texpos_mountain_mid_goodsav_c[s][c]=0;
			texpos_mountain_low_goodsav_c[s][c]=0;
			texpos_mountain_peak_good_c[s][c]=0;
			texpos_volcano_good_c[s][c]=0;
			texpos_mountain_high_good_c[s][c]=0;
			texpos_mountain_mid_good_c[s][c]=0;
			texpos_mountain_low_good_c[s][c]=0;
			}
		}
	for(s=0;s<REGION_SQUARE_FOREST_NUM;++s)
		{
		texpos_forest_broadleaf_trop_moist[s]=0;
		texpos_forest_broadleaf_trop_moist_evilsav[s]=0;
		texpos_forest_broadleaf_trop_moist_evil[s]=0;
		texpos_forest_broadleaf_trop_moist_goodsav[s]=0;
		texpos_forest_broadleaf_trop_moist_good[s]=0;
		texpos_forest_broadleaf_trop_dry[s]=0;
		texpos_forest_broadleaf_trop_dry_evilsav[s]=0;
		texpos_forest_broadleaf_trop_dry_evil[s]=0;
		texpos_forest_broadleaf_trop_dry_goodsav[s]=0;
		texpos_forest_broadleaf_trop_dry_good[s]=0;
		texpos_forest_broadleaf_temp[s]=0;
		texpos_forest_broadleaf_temp_evilsav[s]=0;
		texpos_forest_broadleaf_temp_evil[s]=0;
		texpos_forest_broadleaf_temp_goodsav[s]=0;
		texpos_forest_broadleaf_temp_good[s]=0;
		texpos_forest_conifer_trop[s]=0;
		texpos_forest_conifer_trop_evilsav[s]=0;
		texpos_forest_conifer_trop_evil[s]=0;
		texpos_forest_conifer_trop_goodsav[s]=0;
		texpos_forest_conifer_trop_good[s]=0;
		texpos_forest_conifer_temp[s]=0;
		texpos_forest_conifer_temp_evilsav[s]=0;
		texpos_forest_conifer_temp_evil[s]=0;
		texpos_forest_conifer_temp_goodsav[s]=0;
		texpos_forest_conifer_temp_good[s]=0;
		texpos_forest_taiga[s]=0;
		texpos_forest_taiga_evilsav[s]=0;
		texpos_forest_taiga_evil[s]=0;
		texpos_forest_taiga_goodsav[s]=0;
		texpos_forest_taiga_good[s]=0;
		texpos_site_map_crops[s]=0;
		texpos_site_map_orchard[s]=0;
		int32_t c;
		for(c=0;c<9;++c)
			{
			texpos_forest_broadleaf_trop_moist_c[s][c]=0;
			texpos_forest_broadleaf_trop_moist_evilsav_c[s][c]=0;
			texpos_forest_broadleaf_trop_moist_evil_c[s][c]=0;
			texpos_forest_broadleaf_trop_moist_goodsav_c[s][c]=0;
			texpos_forest_broadleaf_trop_moist_good_c[s][c]=0;
			texpos_forest_broadleaf_trop_dry_c[s][c]=0;
			texpos_forest_broadleaf_trop_dry_evilsav_c[s][c]=0;
			texpos_forest_broadleaf_trop_dry_evil_c[s][c]=0;
			texpos_forest_broadleaf_trop_dry_goodsav_c[s][c]=0;
			texpos_forest_broadleaf_trop_dry_good_c[s][c]=0;
			texpos_forest_broadleaf_temp_c[s][c]=0;
			texpos_forest_broadleaf_temp_evilsav_c[s][c]=0;
			texpos_forest_broadleaf_temp_evil_c[s][c]=0;
			texpos_forest_broadleaf_temp_goodsav_c[s][c]=0;
			texpos_forest_broadleaf_temp_good_c[s][c]=0;
			texpos_forest_taiga_c[s][c]=0;
			texpos_forest_taiga_evilsav_c[s][c]=0;
			texpos_forest_taiga_evil_c[s][c]=0;
			texpos_forest_taiga_goodsav_c[s][c]=0;
			texpos_forest_taiga_good_c[s][c]=0;
			texpos_forest_conifer_temp_c[s][c]=0;
			texpos_forest_conifer_temp_evilsav_c[s][c]=0;
			texpos_forest_conifer_temp_evil_c[s][c]=0;
			texpos_forest_conifer_temp_goodsav_c[s][c]=0;
			texpos_forest_conifer_temp_good_c[s][c]=0;
			texpos_forest_conifer_trop_c[s][c]=0;
			texpos_forest_conifer_trop_evilsav_c[s][c]=0;
			texpos_forest_conifer_trop_evil_c[s][c]=0;
			texpos_forest_conifer_trop_goodsav_c[s][c]=0;
			texpos_forest_conifer_trop_good_c[s][c]=0;
			texpos_site_map_crops_c[s][c]=0;
			texpos_site_map_orchard_c[s][c]=0;
			}
		}

	for(s=0;s<12;++s)
		{
		texpos_site_map_solid_buildings[s]=0;
		texpos_site_map_solid_buildings_top[s]=0;
		}
	texpos_site_map_buildings_road_nswe=0;
	texpos_site_map_buildings_road_swe=0;
	texpos_site_map_buildings_road_nwe=0;
	texpos_site_map_buildings_road_nse=0;
	texpos_site_map_buildings_road_nsw=0;
	texpos_site_map_buildings_road_ns=0;
	texpos_site_map_buildings_road_nw=0;
	texpos_site_map_buildings_road_ne=0;
	texpos_site_map_buildings_road_sw=0;
	texpos_site_map_buildings_road_se=0;
	texpos_site_map_buildings_road_we=0;
	texpos_site_map_buildings_road_n=0;
	texpos_site_map_buildings_road_s=0;
	texpos_site_map_buildings_road_w=0;
	texpos_site_map_buildings_road_e=0;
	texpos_site_map_buildings_road_nswe_top=0;
	texpos_site_map_buildings_road_swe_top=0;
	texpos_site_map_buildings_road_nwe_top=0;
	texpos_site_map_buildings_road_nse_top=0;
	texpos_site_map_buildings_road_nsw_top=0;
	texpos_site_map_buildings_road_ns_top=0;
	texpos_site_map_buildings_road_nw_top=0;
	texpos_site_map_buildings_road_ne_top=0;
	texpos_site_map_buildings_road_sw_top=0;
	texpos_site_map_buildings_road_se_top=0;
	texpos_site_map_buildings_road_we_top=0;
	texpos_site_map_buildings_road_n_top=0;
	texpos_site_map_buildings_road_s_top=0;
	texpos_site_map_buildings_road_w_top=0;
	texpos_site_map_buildings_road_e_top=0;
	texpos_site_map_isolated_building=0;

	texpos_site_map_mead_hall_11.clear();
	texpos_site_map_mead_hall_11_top.clear();
	texpos_site_map_temple_11.clear();
	texpos_site_map_temple_11_top.clear();
	texpos_site_map_tavern_11.clear();
	texpos_site_map_tavern_11_top.clear();
	texpos_site_map_library_11.clear();
	texpos_site_map_library_11_top.clear();
	texpos_site_map_counting_house_11.clear();
	texpos_site_map_counting_house_11_top.clear();
	texpos_site_map_bld_tower.clear();
	texpos_site_map_bld_tower_top.clear();
	texpos_site_map_garden.clear();
	texpos_site_map_garden_top.clear();
	texpos_site_map_guildhall_11.clear();
	texpos_site_map_guildhall_11_top.clear();
	for(s=0;s<2;++s)
		{
		texpos_site_map_temple_21[s].clear();
		texpos_site_map_temple_21_top[s].clear();
		texpos_site_map_temple_12[s].clear();
		texpos_site_map_tavern_21[s].clear();
		texpos_site_map_tavern_21_top[s].clear();
		texpos_site_map_tavern_12[s].clear();
		texpos_site_map_library_21[s].clear();
		texpos_site_map_library_21_top[s].clear();
		texpos_site_map_library_12[s].clear();
		texpos_site_map_counting_house_21[s].clear();
		texpos_site_map_counting_house_21_top[s].clear();
		texpos_site_map_counting_house_12[s].clear();
		texpos_site_map_guildhall_21[s].clear();
		texpos_site_map_guildhall_21_top[s].clear();
		texpos_site_map_guildhall_12[s].clear();
		int32_t s2;
		for(s2=0;s2<2;++s2)
			{
			texpos_site_map_temple_22[s][s2].clear();
			texpos_site_map_tavern_22[s][s2].clear();
			texpos_site_map_library_22[s][s2].clear();
			texpos_site_map_counting_house_22[s][s2].clear();
			texpos_site_map_guildhall_22[s][s2].clear();
			}
		texpos_site_map_temple_22_top[s].clear();
		texpos_site_map_tavern_22_top[s].clear();
		texpos_site_map_library_22_top[s].clear();
		texpos_site_map_counting_house_22_top[s].clear();
		texpos_site_map_guildhall_22_top[s].clear();
		}

	texpos_site_map_plaza=0;
	texpos_site_map_dirt_field=0;
	texpos_site_map_road_paved_nswe=0;
	texpos_site_map_road_paved_swe=0;
	texpos_site_map_road_paved_nwe=0;
	texpos_site_map_road_paved_nse=0;
	texpos_site_map_road_paved_nsw=0;
	texpos_site_map_road_paved_ns=0;
	texpos_site_map_road_paved_nw=0;
	texpos_site_map_road_paved_ne=0;
	texpos_site_map_road_paved_sw=0;
	texpos_site_map_road_paved_se=0;
	texpos_site_map_road_paved_we=0;
	texpos_site_map_road_paved_n=0;
	texpos_site_map_road_paved_s=0;
	texpos_site_map_road_paved_w=0;
	texpos_site_map_road_paved_e=0;
	texpos_site_map_road_dirt_nswe=0;
	texpos_site_map_road_dirt_swe=0;
	texpos_site_map_road_dirt_nwe=0;
	texpos_site_map_road_dirt_nse=0;
	texpos_site_map_road_dirt_nsw=0;
	texpos_site_map_road_dirt_ns=0;
	texpos_site_map_road_dirt_nw=0;
	texpos_site_map_road_dirt_ne=0;
	texpos_site_map_road_dirt_sw=0;
	texpos_site_map_road_dirt_se=0;
	texpos_site_map_road_dirt_we=0;
	texpos_site_map_road_dirt_n=0;
	texpos_site_map_road_dirt_s=0;
	texpos_site_map_road_dirt_w=0;
	texpos_site_map_road_dirt_e=0;
	texpos_site_map_castle=0;
	texpos_site_map_castle_wall_nswe=0;
	texpos_site_map_castle_wall_swe=0;
	texpos_site_map_castle_wall_nwe=0;
	texpos_site_map_castle_wall_nse=0;
	texpos_site_map_castle_wall_nsw=0;
	texpos_site_map_castle_wall_ns=0;
	texpos_site_map_castle_wall_nw=0;
	texpos_site_map_castle_wall_ne=0;
	texpos_site_map_castle_wall_sw=0;
	texpos_site_map_castle_wall_se=0;
	texpos_site_map_castle_wall_we=0;
	texpos_site_map_castle_keep=0;
	texpos_site_map_castle_keep_top=0;
	texpos_site_map_wooden_wall_nswe=0;
	texpos_site_map_wooden_wall_swe=0;
	texpos_site_map_wooden_wall_nwe=0;
	texpos_site_map_wooden_wall_nse=0;
	texpos_site_map_wooden_wall_nsw=0;
	texpos_site_map_wooden_wall_ns=0;
	texpos_site_map_wooden_wall_nw=0;
	texpos_site_map_wooden_wall_ne=0;
	texpos_site_map_wooden_wall_sw=0;
	texpos_site_map_wooden_wall_se=0;
	texpos_site_map_wooden_wall_we=0;
	texpos_site_map_wooden_keep=0;
	texpos_site_map_wooden_keep_top=0;
	texpos_site_map_bridge_brook_we=0;
	texpos_site_map_bridge_brook_ns=0;
	texpos_site_map_bridge_river_we=0;
	texpos_site_map_bridge_river_ns=0;
	texpos_site_map_gate_ns=0;
	texpos_site_map_gate_we=0;
	texpos_site_map_wooden_gate_tower=0;
	texpos_site_map_wooden_gate_tower_top=0;
	texpos_site_map_stone_gate_tower=0;
	texpos_site_map_stone_gate_tower_top=0;
	texpos_site_map_great_dark_tower=0;
	texpos_site_map_great_dark_tower_top=0;
	for(s=0;s<3;++s)
		{
		texpos_site_map_great_dark_tower_z_top[s]=0;
		int32_t s2;
		for(s2=0;s2<3;++s2)
			{
			texpos_site_map_great_dark_tower_z[s][s2]=0;
			}
		}
	texpos_site_map_dark_tower=0;
	texpos_site_map_dark_tower_top=0;
	texpos_site_map_trench_nswe=0;
	texpos_site_map_trench_swe=0;
	texpos_site_map_trench_nwe=0;
	texpos_site_map_trench_nse=0;
	texpos_site_map_trench_nsw=0;
	texpos_site_map_trench_ns=0;
	texpos_site_map_trench_nw=0;
	texpos_site_map_trench_ne=0;
	texpos_site_map_trench_sw=0;
	texpos_site_map_trench_se=0;
	texpos_site_map_trench_we=0;
	texpos_site_map_trench_n=0;
	texpos_site_map_trench_s=0;
	texpos_site_map_trench_w=0;
	texpos_site_map_trench_e=0;
	texpos_site_map_trench=0;
	texpos_site_map_hillock_house.clear();
	texpos_site_map_hillock_hall.clear();
	texpos_site_map_hillock_tavern.clear();
	texpos_site_map_hillock_civic.clear();
	texpos_site_map_tree_residential=0;
	texpos_site_map_tree_residential_top=0;
	texpos_site_map_tree_home=0;
	texpos_site_map_tree_home_top=0;
	texpos_site_map_tree_industry=0;
	texpos_site_map_tree_industry_top=0;
	texpos_site_map_tree_market=0;
	texpos_site_map_tree_market_top=0;
	texpos_site_map_tree_inn=0;
	texpos_site_map_tree_inn_top=0;
	texpos_site_map_tree_library=0;
	texpos_site_map_tree_library_top=0;
	texpos_site_map_tree_building=0;
	texpos_site_map_tree_building_top=0;
	texpos_site_map_tower_undead=0;
	texpos_site_map_tower_undead_top=0;
	texpos_site_map_building=0;
	texpos_site_map_vault=0;
	texpos_site_map_vault_top=0;
	for(s=0;s<3;++s)
		{
		texpos_site_map_vault_z_top[s]=0;

		int32_t s2;
		for(s2=0;s2<3;++s2)
			{
			texpos_site_map_vault_z[s][s2]=0;
			}
		}
	texpos_site_map_mythical_palace=0;
	texpos_site_map_mythical_dungeon=0;
	texpos_site_map_mythical_lair=0;

	texpos_tunnel_ns=0;
	texpos_tunnel_we=0;
	texpos_tunnel_se=0;
	texpos_tunnel_sw=0;
	texpos_tunnel_ne=0;
	texpos_tunnel_nw=0;
	texpos_tunnel_nse=0;
	texpos_tunnel_swe=0;
	texpos_tunnel_nwe=0;
	texpos_tunnel_nsw=0;
	texpos_tunnel_nswe=0;
	texpos_tunnel_0=0;
	texpos_tunnel_s=0;
	texpos_tunnel_n=0;
	texpos_tunnel_w=0;
	texpos_tunnel_e=0;
	texpos_road_paved_ns=0;
	texpos_road_paved_we=0;
	texpos_road_paved_se=0;
	texpos_road_paved_sw=0;
	texpos_road_paved_ne=0;
	texpos_road_paved_nw=0;
	texpos_road_paved_nse=0;
	texpos_road_paved_swe=0;
	texpos_road_paved_nwe=0;
	texpos_road_paved_nsw=0;
	texpos_road_paved_nswe=0;
	texpos_road_paved_0=0;
	texpos_road_paved_s=0;
	texpos_road_paved_n=0;
	texpos_road_paved_w=0;
	texpos_road_paved_e=0;
	texpos_road_dirt_ns=0;
	texpos_road_dirt_we=0;
	texpos_road_dirt_se=0;
	texpos_road_dirt_sw=0;
	texpos_road_dirt_ne=0;
	texpos_road_dirt_nw=0;
	texpos_road_dirt_nse=0;
	texpos_road_dirt_swe=0;
	texpos_road_dirt_nwe=0;
	texpos_road_dirt_nsw=0;
	texpos_road_dirt_nswe=0;
	texpos_road_dirt_0=0;
	texpos_road_dirt_s=0;
	texpos_road_dirt_n=0;
	texpos_road_dirt_w=0;
	texpos_road_dirt_e=0;
	{int32_t r;
	for(r=0;r<REGION_SQUARE_RIVER_NUM;++r)
		{
		texpos_brook_ns[r]=0;
		texpos_brook_we[r]=0;
		texpos_river_ns[r]=0;
		texpos_river_we[r]=0;
		texpos_river_major_ns[r]=0;
		texpos_river_major_we[r]=0;
		}}
	texpos_brook_se=0;
	texpos_brook_sw=0;
	texpos_brook_ne=0;
	texpos_brook_nw=0;
	texpos_brook_nse=0;
	texpos_brook_swe=0;
	texpos_brook_nwe=0;
	texpos_brook_nsw=0;
	texpos_brook_nswe=0;
	texpos_brook_0=0;
	texpos_brook_s=0;
	texpos_brook_n=0;
	texpos_brook_w=0;
	texpos_brook_e=0;
	texpos_brook_mouth_narrow_s=0;
	texpos_brook_mouth_narrow_n=0;
	texpos_brook_mouth_narrow_w=0;
	texpos_brook_mouth_narrow_e=0;
	texpos_river_se=0;
	texpos_river_sw=0;
	texpos_river_ne=0;
	texpos_river_nw=0;
	texpos_river_nse=0;
	texpos_river_swe=0;
	texpos_river_nwe=0;
	texpos_river_nsw=0;
	texpos_river_nswe=0;
	texpos_river_0=0;
	texpos_river_s=0;
	texpos_river_n=0;
	texpos_river_w=0;
	texpos_river_e=0;
	texpos_river_mouth_narrow_s=0;
	texpos_river_mouth_narrow_n=0;
	texpos_river_mouth_narrow_w=0;
	texpos_river_mouth_narrow_e=0;
	texpos_river_major_se=0;
	texpos_river_major_sw=0;
	texpos_river_major_ne=0;
	texpos_river_major_nw=0;
	texpos_river_major_nse=0;
	texpos_river_major_swe=0;
	texpos_river_major_nwe=0;
	texpos_river_major_nsw=0;
	texpos_river_major_nswe=0;
	texpos_river_major_0=0;
	texpos_river_major_s=0;
	texpos_river_major_n=0;
	texpos_river_major_w=0;
	texpos_river_major_e=0;
	texpos_river_major_mouth_narrow_s=0;
	texpos_river_major_mouth_narrow_n=0;
	texpos_river_major_mouth_narrow_w=0;
	texpos_river_major_mouth_narrow_e=0;
	texpos_site_cave=0;
	texpos_site_labyrinth=0;
	texpos_site_shrine_titan=0;
	texpos_site_lair_burrow=0;
	texpos_map_cursor=0;
	texpos_map_your_fort=0;
	texpos_map_your_civ=0;
	texpos_map_neutral=0;
	texpos_map_hostile=0;
	texpos_map_occupied_holding=0;
	texpos_map_holding=0;
	texpos_map_vassal=0;
	texpos_map_occupied_tributary=0;
	texpos_map_tributary=0;
	texpos_map_market_dependence=0;
	texpos_map_ally=0;
	texpos_map_attack=0;
	texpos_map_mission_path=0;
	texpos_map_mission_marker=0;
	texpos_site_camp=0;
	texpos_site_fortress=0;
	texpos_site_hillocks=0;
	texpos_site_mountain_home=0;
	texpos_site_village=0;
	texpos_site_city_1=0;
	texpos_site_city_2=0;
	texpos_site_city_3=0;
	texpos_site_city_4=0;
	texpos_site_forest_retreat_2=0;
	texpos_site_forest_retreat_1=0;
	texpos_site_necrotower=0;
	texpos_site_castle=0;
	texpos_site_fort=0;
	texpos_site_monastery=0;
	texpos_site_vault=0;
	texpos_site_mythical_palace=0;
	texpos_site_mythical_dungeon=0;
	texpos_site_mythical_lair=0;
	texpos_site_tomb=0;
	texpos_site_dark_fortress_2=0;
	texpos_site_dark_fortress_1=0;
	texpos_site_ruin_dwarf=0;
	texpos_site_ruin_village=0;
	texpos_site_ruin_city=0;
	texpos_site_ruin_forest_retreat_2=0;
	texpos_site_ruin_forest_retreat_1=0;
	texpos_site_ruin_castle=0;
	texpos_site_ruin_goblin=0;
	texpos_site_ruin=0;
	texpos_elevation_indicator=0;
	texpos_army_group_small=0;
	texpos_army_group_medium=0;
	texpos_army_group_large=0;
	texpos_army_soldiers_small=0;
	texpos_army_soldiers_medium=0;
	texpos_army_soldiers_large=0;
	texpos_army_monsters_small=0;
	texpos_army_monsters_medium=0;
	texpos_army_monsters_large=0;
	texpos_map_adv_you_travel=0;

	{int32_t r;
	for(r=0;r<3*3;++r)
		{
		texpos_world_stratus_nimbus[r]=0;
		texpos_world_stratus_proper[r]=0;
		texpos_world_stratus_alto[r]=0;
		texpos_world_cumulus_nimbus[r]=0;
		texpos_world_cirrus[r]=0;
		}}
	texpos_world_cumulus_multi=0;
	texpos_world_cumulus_med=0;
	texpos_world_fog_thick=0;
	texpos_world_fog_normal=0;
	texpos_world_fog_mist=0;

	{int32_t r;
	for(r=0;r<TEXTURE_MAP_DRAWNNUM;++r)texpos_map_drawn[r].clear();}

	texpos_forgotten_beast_icon=0;
	texpos_titan_icon=0;
	texpos_demon_icon=0;
	texpos_night_creature_icon=0;
	texpos_angel_icon=0;

	texpos_ramp_up_arrow=0;
	texpos_ramp_down_arrow=0;
	memset(texpos_liquid_numbers_on,0,sizeof(int32_t)*2*2);
	memset(texpos_liquid_numbers_off,0,sizeof(int32_t)*2*2);
	memset(texpos_ramp_arrows_on,0,sizeof(int32_t)*2*2);
	memset(texpos_ramp_arrows_off,0,sizeof(int32_t)*2*2);

	memset(texpos_zoom_in_on,0,sizeof(int32_t)*2*2);
	memset(texpos_zoom_in_off,0,sizeof(int32_t)*2*2);
	memset(texpos_zoom_out_on,0,sizeof(int32_t)*2*2);
	memset(texpos_zoom_out_off,0,sizeof(int32_t)*2*2);

	memset(texpos_adv_liquid_numbers_on,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_liquid_numbers_off,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_ramp_arrows_on,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_ramp_arrows_off,0,sizeof(int32_t)*2*2);

	memset(texpos_adv_zoom_in_on,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_zoom_in_off,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_zoom_out_on,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_zoom_out_off,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_tracks_on,0,sizeof(int32_t)*2*2);
	memset(texpos_adv_tracks_off,0,sizeof(int32_t)*2*2);

	memset(texpos_adv_env,0,sizeof(int32_t)*INTERFACE_ADV_ENV_TEXTURENUM*4*3);

	look_cursor_texpos=0;
	talk_cursor_texpos=0;
	legal_jump_texpos=0;
	illegal_jump_texpos=0;
	error_cursor_texpos=0;
	out_of_bounds_cursor_texpos=0;

	{int32_t r;
	for(r=0;r<RECTANGLE_CURSORNUM;++r)
		{
		texpos_rectangle_cursor_n_w[r]=0;
		texpos_rectangle_cursor_n[r]=0;
		texpos_rectangle_cursor_n_e[r]=0;
		texpos_rectangle_cursor_n_s_w[r]=0;
		texpos_rectangle_cursor_n_s[r]=0;
		texpos_rectangle_cursor_n_s_e[r]=0;
		texpos_rectangle_cursor_w[r]=0;
		texpos_rectangle_cursor_interior[r]=0;
		texpos_rectangle_cursor_e[r]=0;
		texpos_rectangle_cursor_n_w_e[r]=0;
		texpos_rectangle_cursor_s_w_e[r]=0;
		texpos_rectangle_cursor_s_w[r]=0;
		texpos_rectangle_cursor_s[r]=0;
		texpos_rectangle_cursor_s_e[r]=0;
		texpos_rectangle_cursor_w_e[r]=0;
		texpos_rectangle_cursor_n_s_w_e[r]=0;
		}}

	texpos_keyboard_cursor=0;

	texpos_recenter_indicator[0]=0;
	texpos_recenter_indicator[1]=0;
	texpos_recenter_indicator[2]=0;
	texpos_target_indicator[0]=0;
	texpos_target_indicator[1]=0;
	texpos_target_indicator[2]=0;

	texpos_patrol_route_point=0;
	texpos_patrol_route_line=0;

	texpos_viewport_grid=0;

	{int32_t i,s;
	for(i=0;i<4;++i){
	for(s=0;s<RIVER_FLOW_ANIMATION_NUM;++s)
		{
		texpos_river_to_nw[i][s]=0;
		texpos_river_to_ne[i][s]=0;
		texpos_river_to_sw[i][s]=0;
		texpos_river_to_se[i][s]=0;
		texpos_river_to_n[i][s]=0;
		texpos_river_to_s[i][s]=0;
		texpos_river_to_w[i][s]=0;
		texpos_river_to_e[i][s]=0;
		texpos_river_bed[i][s]=0;
		texpos_river_bed_n[i][s]=0;
		texpos_river_bed_s[i][s]=0;
		texpos_river_bed_w[i][s]=0;
		texpos_river_bed_e[i][s]=0;
		texpos_river_bed_n_w[i][s]=0;
		texpos_river_bed_n_e[i][s]=0;
		texpos_river_bed_s_w[i][s]=0;
		texpos_river_bed_s_e[i][s]=0;
		texpos_river_bed_source[i][s]=0;
		texpos_river_bed_nw[i][s]=0;
		texpos_river_bed_ne[i][s]=0;
		texpos_river_bed_sw[i][s]=0;
		texpos_river_bed_se[i][s]=0;
		texpos_river_bed_nw_se[i][s]=0;
		texpos_river_bed_ne_sw[i][s]=0;
		}
	for(s=0;s<BROOK_FLOW_ANIMATION_NUM;++s)
		{
		texpos_brook_to_nw[i][s]=0;
		texpos_brook_to_ne[i][s]=0;
		texpos_brook_to_sw[i][s]=0;
		texpos_brook_to_se[i][s]=0;
		texpos_brook_to_n[i][s]=0;
		texpos_brook_to_s[i][s]=0;
		texpos_brook_to_w[i][s]=0;
		texpos_brook_to_e[i][s]=0;
		texpos_brook_bed[i][s]=0;
		texpos_brook_bed_n[i][s]=0;
		texpos_brook_bed_s[i][s]=0;
		texpos_brook_bed_w[i][s]=0;
		texpos_brook_bed_e[i][s]=0;
		texpos_brook_bed_n_w[i][s]=0;
		texpos_brook_bed_n_e[i][s]=0;
		texpos_brook_bed_s_w[i][s]=0;
		texpos_brook_bed_s_e[i][s]=0;
		texpos_brook_bed_nw[i][s]=0;
		texpos_brook_bed_ne[i][s]=0;
		texpos_brook_bed_sw[i][s]=0;
		texpos_brook_bed_se[i][s]=0;
		texpos_brook_bed_nw_ne[i][s]=0;
		texpos_brook_bed_nw_sw[i][s]=0;
		texpos_brook_bed_nw_se[i][s]=0;
		texpos_brook_bed_ne_sw[i][s]=0;
		texpos_brook_bed_ne_se[i][s]=0;
		texpos_brook_bed_sw_se[i][s]=0;
		texpos_brook_bed_nw_ne_sw[i][s]=0;
		texpos_brook_bed_nw_ne_se[i][s]=0;
		texpos_brook_bed_nw_sw_se[i][s]=0;
		texpos_brook_bed_ne_sw_se[i][s]=0;
		texpos_brook_bed_nw_ne_sw_se[i][s]=0;
		}}}

	{int32_t i,s,s2;
	for(i=0;i<COMBAT_ANIMATION_SWISHNUM;++i)
		{
		for(s=0;s<COMBAT_ANIMATION_SWISH_DIRECTIONNUM;++s)
			{
			for(s2=0;s2<COMBAT_ANIMATION_SWISH_FRAME_COUNT;++s2)
				{
				texpos_combat_animation_swish[i][s][s2]=0;
				texpos_combat_animation_sparks[i][s][s2]=0;
				}}}}

	{int32_t s,s2;
	for(s=0;s<COMBAT_ANIMATION_RAM_DIRECTIONNUM;++s)
		{
		for(s2=0;s2<COMBAT_ANIMATION_RAM_FRAME_COUNT;++s2)
			{
			texpos_combat_animation_ram[s][s2]=0;
			}}}

	{int32_t i,s,s2;
	for(i=0;i<GRAPHICS_TRACKING_SYMBOL_WEIGHTNUM;++i)
		{
		for(s=0;s<GRAPHICS_TRACKING_SYMBOL_DIRNUM;++s)
			{
			for(s2=0;s2<2;++s2)
				{
				texpos_tracking_symbol_broken_vegetation[i][s][s2]=0;
				texpos_tracking_symbol_humanoid_liquid_print[i][s][s2]=0;
				texpos_tracking_symbol_beast_liquid_print[i][s][s2]=0;
				texpos_tracking_symbol_footwear_liquid_print[i][s][s2]=0;
				texpos_tracking_symbol_humanoid_imprint[i][s][s2]=0;
				texpos_tracking_symbol_beast_imprint[i][s][s2]=0;
				texpos_tracking_symbol_footwear_imprint[i][s][s2]=0;
				}}}}
	texpos_tracking_symbol_unintelligible_mess[0]=0;
	texpos_tracking_symbol_unintelligible_mess[1]=0;

	{int32_t i,s,s2;
	for(i=0;i<MOVE_INDICATORNUM;++i)
		{
		for(s=0;s<MOVE_INDICATOR_DIRECTIONNUM;++s)
			{
			for(s2=0;s2<MOVE_INDICATOR_FRAME_COUNT;++s2)
				{
				texpos_move_indicator[i][s][s2]=0;
				}}}}
	{int32_t i;
	for(i=0;i<GRAPHICS_SOUND_INDICATORNUM;++i)
		{
		texpos_sound_indicator[i]=0;
		}}
	{int32_t i,s;
	for(i=0;i<3;++i)
		{
		for(s=0;s<2;++s)
			{
			texpos_adv_speed_box[i][s]=0;
			}}}
	texpos_vampire_sense_marker=0;
	texpos_stealth_vision_direct=0;
	texpos_stealth_vision_direct_down=0;
	texpos_stealth_vision_direct_up=0;
	texpos_stealth_vision_peripheral=0;
	texpos_stealth_vision_peripheral_down=0;
	texpos_stealth_vision_peripheral_up=0;
	texpos_memory_map_monster=0;
	texpos_memory_map_item=0;
	texpos_memory_map_building=0;
	texpos_memory_map_wall=0;
	texpos_memory_map_stair_up=0;
	texpos_memory_map_stair_down=0;
	texpos_memory_map_stair_updown=0;
	texpos_memory_map_ramp_up=0;
	texpos_memory_map_ramp_down=0;
	texpos_memory_map_floor=0;
	texpos_memory_map_air=0;

	int32_t smt;
	for(smt=0;smt<SPATTER_MATERIALNUM;++smt)
		{
		texpos_full_nswe_a_spatter[smt]=0;
		texpos_full_nswe_b_spatter[smt]=0;
		texpos_full_nswe_c_spatter[smt]=0;
		texpos_full_nswe_d_spatter[smt]=0;
		texpos_full_nswe_e_spatter[smt]=0;
		texpos_full_nsw_spatter[smt]=0;
		texpos_full_nse_spatter[smt]=0;
		texpos_full_nwe_spatter[smt]=0;
		texpos_full_swe_spatter[smt]=0;
		texpos_full_ns_spatter[smt]=0;
		texpos_full_we_spatter[smt]=0;
		texpos_full_nw_spatter[smt]=0;
		texpos_full_ne_spatter[smt]=0;
		texpos_full_sw_spatter[smt]=0;
		texpos_full_se_spatter[smt]=0;
		texpos_full_n_spatter[smt]=0;
		texpos_full_s_spatter[smt]=0;
		texpos_full_w_spatter[smt]=0;
		texpos_full_e_spatter[smt]=0;
		texpos_full_isolated_spatter[smt]=0;
		/*
		texpos_spatter_ramp_with_wall_nsew[smt]=0;
		texpos_spatter_ramp_with_wall_nse[smt]=0;
		texpos_spatter_ramp_with_wall_nsw[smt]=0;
		texpos_spatter_ramp_with_wall_nwe[smt]=0;
		texpos_spatter_ramp_with_wall_swe[smt]=0;
		texpos_spatter_ramp_with_wall_n_s[smt]=0;
		texpos_spatter_ramp_with_wall_n_w[smt]=0;
		texpos_spatter_ramp_with_wall_n_e[smt]=0;
		texpos_spatter_ramp_with_wall_s_w[smt]=0;
		texpos_spatter_ramp_with_wall_s_e[smt]=0;
		texpos_spatter_ramp_with_wall_w_e[smt]=0;
		texpos_spatter_ramp_with_wall_n[smt]=0;
		texpos_spatter_ramp_with_wall_s[smt]=0;
		texpos_spatter_ramp_with_wall_w[smt]=0;
		texpos_spatter_ramp_with_wall_e[smt]=0;
		texpos_spatter_ramp_with_wall_nw[smt]=0;
		texpos_spatter_ramp_with_wall_sw[smt]=0;
		texpos_spatter_ramp_with_wall_se[smt]=0;
		texpos_spatter_ramp_with_wall_ne[smt]=0;
		texpos_spatter_ramp_with_wall_nw_ne[smt]=0;
		texpos_spatter_ramp_with_wall_sw_se[smt]=0;
		texpos_spatter_ramp_with_wall_nw_sw[smt]=0;
		texpos_spatter_ramp_with_wall_ne_se[smt]=0;
		texpos_spatter_ramp_other[smt]=0;
		texpos_spatter_ramp_with_wall_s_ne[smt]=0;
		texpos_spatter_ramp_with_wall_w_se[smt]=0;
		texpos_spatter_ramp_with_wall_n_sw[smt]=0;
		texpos_spatter_ramp_with_wall_e_nw[smt]=0;
		texpos_spatter_ramp_with_wall_e_sw[smt]=0;
		texpos_spatter_ramp_with_wall_s_nw[smt]=0;
		texpos_spatter_ramp_with_wall_w_ne[smt]=0;
		texpos_spatter_ramp_with_wall_n_se[smt]=0;
		texpos_spatter_ramp_with_wall_s_e_nw[smt]=0;
		texpos_spatter_ramp_with_wall_s_w_ne[smt]=0;
		texpos_spatter_ramp_with_wall_n_w_se[smt]=0;
		texpos_spatter_ramp_with_wall_n_e_sw[smt]=0;
		texpos_spatter_ramp_with_wall_n_sw_se[smt]=0;
		texpos_spatter_ramp_with_wall_e_nw_sw[smt]=0;
		texpos_spatter_ramp_with_wall_s_nw_ne[smt]=0;
		texpos_spatter_ramp_with_wall_w_ne_se[smt]=0;
		texpos_spatter_ramp_with_wall_nw_se[smt]=0;
		texpos_spatter_ramp_with_wall_ne_sw[smt]=0;
		texpos_spatter_ramp_with_wall_nw_ne_sw_se[smt]=0;
		texpos_spatter_ramp_with_wall_nw_ne_sw[smt]=0;
		texpos_spatter_ramp_with_wall_nw_ne_se[smt]=0;
		texpos_spatter_ramp_with_wall_nw_sw_se[smt]=0;
		texpos_spatter_ramp_with_wall_ne_sw_se[smt]=0;
		*/
		int32_t x,y;
		for(x=0;x<4;++x)
			{
			for(y=0;y<4;++y)texpos_partial_spatter[smt][x][y]=0;
			}
		}

	texpos_item_branch=0;
	texpos_item_wood=0;
	texpos_item_rough_gem=0;
	texpos_item_rough_gem_value_2=0;
	texpos_item_rough_gem_value_3=0;
	texpos_item_rough_gem_value_5=0;
	texpos_item_rough_gem_value_10=0;
	texpos_item_rough_gem_value_15=0;
	texpos_item_rough_gem_value_20=0;
	texpos_item_rough_gem_value_25=0;
	texpos_item_rough_gem_value_30=0;
	texpos_item_rough_gem_value_40=0;
	texpos_item_rough_gem_value_60=0;
	texpos_item_boulder=0;
	texpos_item_rock=0;
	texpos_item_bars=0;
		texpos_item_bars_soap=0;
	texpos_item_gems=0;
	texpos_item_gem=0;
	texpos_item_blocks=0;
	texpos_item_hatch_cover_stone=0;
	texpos_item_hatch_cover_stone_open=0;
	texpos_item_hatch_cover_wooden=0;
	texpos_item_hatch_cover_wooden_open=0;
	texpos_item_hatch_cover_metal=0;
	texpos_item_hatch_cover_metal_open=0;
	texpos_item_hatch_cover_glass=0;
	texpos_item_hatch_cover_glass_open=0;
		for(s=0;s<7;++s)
			{
			texpos_item_hatch_cover_wood_closed[s]=0;
			texpos_item_hatch_cover_stone_closed[s]=0;
			texpos_item_hatch_cover_metal_closed[s]=0;
			texpos_item_hatch_cover_glass_closed[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_hatch_cover_variant[s]=0;
		texpos_item_hatch_cover_spikes=0;
		texpos_item_hatch_cover_rings=0;
		texpos_item_hatch_cover_studs=0;
		texpos_item_hatch_cover_engraving=0;
		for(s=0;s<3;++s)texpos_item_hatch_cover_damage[s]=0;
		texpos_item_hatch_cover_bands=0;
		texpos_item_hatch_cover_blood=0;
		texpos_item_hatch_cover_vomit=0;
		texpos_item_hatch_cover_water=0;
		texpos_item_hatch_cover_mud=0;
		texpos_item_hatch_cover_forbidden=0;
	texpos_item_grate=0;
	texpos_item_floodgate_stone=0;
	texpos_item_floodgate_stone_open=0;
	texpos_item_floodgate_wooden=0;
	texpos_item_floodgate_wooden_open=0;
	texpos_item_floodgate_metal=0;
	texpos_item_floodgate_metal_open=0;
	texpos_item_floodgate_glass=0;
	texpos_item_floodgate_glass_open=0;
		for(s=0;s<7;++s)
			{
			texpos_item_floodgate_wood_closed[s]=0;
			texpos_item_floodgate_stone_closed[s]=0;
			texpos_item_floodgate_metal_closed[s]=0;
			texpos_item_floodgate_glass_closed[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_floodgate_variant[s]=0;
		texpos_item_floodgate_spikes=0;
		texpos_item_floodgate_rings=0;
		texpos_item_floodgate_studs=0;
		texpos_item_floodgate_engraving=0;
		for(s=0;s<3;++s)texpos_item_floodgate_damage[s]=0;
		texpos_item_floodgate_bands=0;
		texpos_item_floodgate_blood=0;
		texpos_item_floodgate_vomit=0;
		texpos_item_floodgate_water=0;
		texpos_item_floodgate_mud=0;
	texpos_item_door_stone=0;
	texpos_item_door_stone_open=0;
	texpos_item_door_wooden=0;
	texpos_item_door_wooden_open=0;
	texpos_item_door_metal=0;
	texpos_item_door_metal_open=0;
	texpos_item_door_glass=0;
	texpos_item_door_glass_open=0;
	{int32_t d;
	for(d=0;d<3;++d)
		{
		texpos_item_door_stone_open_damaged[d]=0;
		texpos_item_door_wooden_open_damaged[d]=0;
		texpos_item_door_metal_open_damaged[d]=0;
		texpos_item_door_glass_open_damaged[d]=0;
		}}
		for(s=0;s<7;++s)
			{
			texpos_item_door_wood_closed[s]=0;
			texpos_item_door_stone_closed[s]=0;
			texpos_item_door_metal_closed[s]=0;
			texpos_item_door_glass_closed[s]=0;
			{int32_t d;
			for(d=0;d<3;++d)
				{
				texpos_item_door_wood_closed_damaged[s][d]=0;
				texpos_item_door_stone_closed_damaged[s][d]=0;
				texpos_item_door_metal_closed_damaged[s][d]=0;
				texpos_item_door_glass_closed_damaged[s][d]=0;
				}}
			}
		for(s=0;s<4;++s)
			{
			texpos_item_door_variant[s]=0;
			{int32_t d;
			for(d=0;d<3;++d)
				{
				texpos_item_door_variant_damaged[s][d]=0;
				}}
			}
		texpos_item_door_spikes=0;
		texpos_item_door_rings=0;
		texpos_item_door_studs=0;
		texpos_item_door_engraving=0;
		texpos_item_door_bands=0;
		texpos_item_door_blood=0;
		texpos_item_door_vomit=0;
		texpos_item_door_water=0;
		texpos_item_door_mud=0;
		texpos_item_door_forbidden=0;
	texpos_item_chain_metal=0;
	texpos_item_chain_rope=0;
	texpos_item_flask_metal=0;
	texpos_item_flask_leather=0;
	texpos_item_flask_glass=0;
	texpos_item_goblet_metal=0;
	texpos_item_goblet_stone=0;
	texpos_item_goblet_wood=0;
	texpos_item_instrument_keyboard_building=0;
	texpos_item_instrument_stringed_building=0;
	texpos_item_instrument_wind_building=0;
	texpos_item_instrument_percussion_building=0;
	texpos_item_instrument_keyboard_handheld=0;
	texpos_item_instrument_stringed_handheld=0;
	texpos_item_instrument_wind_handheld=0;
	texpos_item_instrument_percussion_handheld=0;
	texpos_item_toy=0;
	texpos_item_tool=0;
	texpos_item_generated_tool=0;
	texpos_item_window_glass=0;
	texpos_item_window_gem=0;
	texpos_item_cage_wood=0;
	texpos_item_cage_metal=0;
	texpos_item_cage_glass=0;
	texpos_item_cage_wood_occupied=0;
	texpos_item_cage_metal_occupied=0;
	texpos_item_cage_glass_occupied=0;

	texpos_item_barrel_wood=0;
	texpos_item_barrel_metal=0;
	texpos_item_barrel_wood_empty=0;
	texpos_item_barrel_metal_empty=0;
	texpos_item_barrel_wood_liquid=0;
	texpos_liquid_for_barrel_wood=0;
	texpos_item_barrel_metal_liquid=0;
	texpos_liquid_for_barrel_metal=0;
	texpos_item_barrel_wood_item=0;
	texpos_item_barrel_metal_item=0;
	texpos_item_barrel_top_plant.clear();
	texpos_item_barrel_top_plant_subterranean.clear();
	texpos_item_barrel_top_meal.clear();
	texpos_item_barrel_top_cheese.clear();
	texpos_item_barrel_top_meat.clear();
	texpos_item_barrel_top_fish.clear();
	texpos_item_barrel_top_bag.clear();

	texpos_food_container_top_liquid=0;
	texpos_food_container_top_bag=0;
	for(s=0;s<4;++s)texpos_food_container_top_fish[s]=0;
	for(s=0;s<4;++s)texpos_food_container_top_meat[s]=0;

	texpos_item_bucket=0;
	texpos_item_bucket_wood=0;
	texpos_liquid_for_bucket_wood=0;
	texpos_item_bucket_metal=0;
	texpos_liquid_for_bucket_metal=0;

	texpos_item_animal_trap_wood=0;
	texpos_item_animal_trap_metal=0;
	texpos_item_animal_trap_wood_occupied=0;
	texpos_item_animal_trap_metal_occupied=0;
	texpos_item_chair=0;
		for(s=0;s<7;++s)
			{
			texpos_item_chair_wood[s]=0;
			texpos_item_chair_stone[s]=0;
			texpos_item_chair_metal[s]=0;
			texpos_item_chair_glass[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_chair_leg[s]=0;
		texpos_item_chair_spikes=0;
		texpos_item_chair_rings=0;
		texpos_item_chair_studs=0;
		texpos_item_chair_engraving=0;
		for(s=0;s<3;++s)texpos_item_chair_damage[s]=0;
		texpos_item_chair_bands=0;
		texpos_item_chair_blood=0;
		texpos_item_chair_vomit=0;
		texpos_item_chair_water=0;
		texpos_item_chair_mud=0;
	texpos_item_coffin=0;
		texpos_item_coffin_wood=0;
		texpos_item_coffin_stone=0;
		texpos_item_coffin_metal=0;
		texpos_item_coffin_glass=0;
		texpos_item_coffin_wood_unused=0;
		texpos_item_coffin_stone_unused=0;
		texpos_item_coffin_metal_unused=0;
		texpos_item_coffin_glass_unused=0;
	texpos_item_table=0;
		for(s=0;s<7;++s)
			{
			texpos_item_table_wood[s]=0;
			texpos_item_table_stone[s]=0;
			texpos_item_table_metal[s]=0;
			texpos_item_table_glass[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_table_leg[s]=0;
		texpos_item_table_spikes=0;
		texpos_item_table_rings=0;
		texpos_item_table_studs=0;
		texpos_item_table_engraving=0;
		for(s=0;s<3;++s)texpos_item_table_damage[s]=0;
		texpos_item_table_bands=0;
		texpos_item_table_blood=0;
		texpos_item_table_vomit=0;
		texpos_item_table_water=0;
		texpos_item_table_mud=0;
	texpos_item_backpack=0;
	texpos_item_quiver=0;
	texpos_item_splint=0;
	texpos_item_orthopedic_cast=0;
	texpos_item_crutch=0;
	texpos_item_bin=0;
		texpos_item_bin_wood_empty=0;
		texpos_item_bin_metal_empty=0;
		texpos_item_bin_wood_in_use=0;
		texpos_item_bin_metal_in_use=0;
		texpos_item_bin_top_ammo.clear();
		texpos_item_bin_top_cloth.clear();
		texpos_item_bin_top_clothing.clear();
		texpos_item_bin_top_armor_leather.clear();
		texpos_item_bin_top_armor_metal.clear();
		texpos_item_bin_top_bars.clear();
		texpos_item_bin_top_blocks.clear();
		texpos_item_bin_top_coal.clear();
		texpos_item_bin_top_powders.clear();
		texpos_item_bin_top_coins.clear();
		texpos_item_bin_top_gems.clear();
		texpos_item_bin_top_mechanisms.clear();
		texpos_item_bin_top_bags.clear();
		texpos_item_bin_top_books.clear();
		texpos_item_bin_top_sheets.clear();
		texpos_item_bin_top_chains.clear();
		texpos_item_bin_top_ropes.clear();
		texpos_item_bin_top_leather.clear();
		texpos_item_bin_top_weapons.clear();
		texpos_item_bin_top_trap_comps.clear();
		texpos_item_bin_top_crafts.clear();
	texpos_item_bag=0;
		texpos_item_bag_empty=0;
		texpos_item_bag_in_use=0;
		texpos_powder_for_bag=0;
		texpos_leaves_for_bag=0;
		texpos_seeds_for_bag=0;
	texpos_item_box=0;
		for(s=0;s<7;++s)
			{
			texpos_item_box_wood[s]=0;
			texpos_item_box_stone[s]=0;
			texpos_item_box_metal[s]=0;
			texpos_item_box_glass[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_box_variant[s]=0;
		texpos_item_box_spikes=0;
		texpos_item_box_rings=0;
		texpos_item_box_studs=0;
		texpos_item_box_engraving=0;
		for(s=0;s<3;++s)texpos_item_box_damage[s]=0;
		texpos_item_box_bands=0;
		texpos_item_box_blood=0;
		texpos_item_box_vomit=0;
		texpos_item_box_water=0;
		texpos_item_box_mud=0;

	texpos_item_weapon_rack_wood_empty=0;
	texpos_item_weapon_rack_stone_empty=0;
	texpos_item_weapon_rack_metal_empty=0;
	texpos_item_weapon_rack_wood_full=0;
	texpos_item_weapon_rack_stone_full=0;
	texpos_item_weapon_rack_metal_full=0;
	texpos_item_armor_stand_wood_empty=0;
	texpos_item_armor_stand_stone_empty=0;
	texpos_item_armor_stand_metal_empty=0;
	texpos_item_armor_stand_wood_full=0;
	texpos_item_armor_stand_stone_full=0;
	texpos_item_armor_stand_metal_full=0;

	texpos_item_cabinet=0;
		for(s=0;s<7;++s)
			{
			texpos_item_cabinet_wood[s]=0;
			texpos_item_cabinet_stone[s]=0;
			texpos_item_cabinet_metal[s]=0;
			texpos_item_cabinet_glass[s]=0;
			}
		for(s=0;s<4;++s)texpos_item_cabinet_variant[s]=0;
		texpos_item_cabinet_spikes=0;
		texpos_item_cabinet_rings=0;
		texpos_item_cabinet_studs=0;
		texpos_item_cabinet_engraving=0;
		for(s=0;s<3;++s)texpos_item_cabinet_damage[s]=0;
		texpos_item_cabinet_bands=0;
		texpos_item_cabinet_blood=0;
		texpos_item_cabinet_vomit=0;
		texpos_item_cabinet_water=0;
		texpos_item_cabinet_mud=0;
	texpos_item_weapon=0;
	texpos_item_pants=0;
	texpos_item_helm=0;
	texpos_item_gloves=0;
	texpos_item_shoes=0;
	texpos_item_shield=0;
	texpos_item_armor=0;
	for(s=0;s<4;++s)texpos_item_web_harmless[s]=0;
	for(s=0;s<4;++s)texpos_item_web_thick[s]=0;
	texpos_item_thread=0;
	texpos_item_cloth=0;
	texpos_item_sheet=0;
	texpos_item_tanned_skin=0;
	texpos_item_totem=0;
	texpos_item_book_wood=0;
	texpos_item_book_metal=0;
	texpos_item_liquid=0;
	texpos_item_powder=0;
	texpos_item_mechanisms=0;
	texpos_item_pipe_section=0;
	texpos_item_trap_component=0;
	texpos_item_catapult_parts=0;
	texpos_item_ballista_parts=0;
	texpos_item_bolt_thrower_parts=0;
	texpos_item_anvil=0;
	texpos_item_ammo=0;
	texpos_item_ballista_arrowhead=0;
	texpos_item_ballista_arrow=0;
	texpos_item_coins_single=0;
	texpos_item_coins_pile_1=0;
	texpos_item_coins_pile_2=0;
	texpos_item_coins_pile_3=0;
	texpos_item_coins_pile_4=0;

	texpos_item_figurine_metal=0;
	texpos_item_amulet_metal=0;
	texpos_item_scepter_metal=0;
	texpos_item_crown_metal=0;
	texpos_item_ring_metal=0;
	texpos_item_bracelet_metal=0;
	texpos_item_earring_metal=0;
	texpos_item_figurine_wood=0;
	texpos_item_amulet_wood=0;
	texpos_item_scepter_wood=0;
	texpos_item_crown_wood=0;
	texpos_item_ring_wood=0;
	texpos_item_bracelet_wood=0;
	texpos_item_earring_wood=0;

	texpos_item_bed=0;
		for(s=0;s<7;++s)
			{
			texpos_item_bed_wood[s]=0;
			texpos_item_bed_stone[s]=0;
			texpos_item_bed_metal[s]=0;
			texpos_item_bed_glass[s]=0;
			}
		for(s=0;s<2;++s)texpos_item_bed_sheet_variant[s]=0;
		for(s=0;s<3;++s)texpos_item_bed_pillow_variant[s]=0;
		texpos_item_bed_spikes=0;
		texpos_item_bed_rings=0;
		texpos_item_bed_studs=0;
		texpos_item_bed_engraving=0;
		for(s=0;s<3;++s)texpos_item_bed_damage[s]=0;
		texpos_item_bed_bands=0;
		texpos_item_bed_blood=0;
		texpos_item_bed_vomit=0;
		texpos_item_bed_water=0;
		texpos_item_bed_mud=0;
	texpos_item_traction_bench_stone_chain=0;
	texpos_item_traction_bench_stone_rope=0;
	texpos_item_traction_bench_wooden_chain=0;
	texpos_item_traction_bench_wooden_rope=0;
	texpos_item_traction_bench_metal_chain=0;
	texpos_item_traction_bench_metal_rope=0;
	texpos_item_traction_bench_glass_chain=0;
	texpos_item_traction_bench_glass_rope=0;
	texpos_item_remains=0;
	texpos_item_seeds=0;
	texpos_item_plant_growth=0;
	texpos_item_rotten_plant=0;
	texpos_item_plant=0;
	texpos_item_vermin=0;
	texpos_item_fish=0;
	texpos_item_egg_size1=0;
	texpos_item_egg_size2=0;
	texpos_item_egg_size3=0;
	texpos_item_egg_size4=0;
	texpos_item_meat=0;
	texpos_item_prepared_meal=0;
	texpos_item_cheese=0;
	texpos_item_corpse_chunk=0;
	texpos_item_corpse=0;
	texpos_item_statue=0;
	texpos_item_default_statue_top=0;
	texpos_item_default_statue_bottom=0;
	texpos_item_slab_blank=0;
	memset(texpos_item_slab_engraved,0,sizeof(int32_t)*ENGRAVING_INTENTNUM);
	texpos_item_quern=0;
	texpos_item_millstone=0;
	texpos_item_mythical_remnant_default=0;
	memset(texpos_item_mythical_remnant,0,sizeof(int32_t)*SPHERENUM);

	texpos_skeleton=0;

	texpos_item_statue_item=0;
	for(s=0;s<7;++s)
		{
		texpos_item_statue_wood[s]=0;
		texpos_item_statue_stone[s]=0;
		texpos_item_statue_metal[s]=0;
		texpos_item_statue_glass[s]=0;
		}
	texpos_item_statue_spikes=0;
	texpos_item_statue_rings=0;
	texpos_item_statue_studs=0;
	texpos_item_statue_engraving=0;
	for(s=0;s<3;++s)texpos_item_statue_damage[s]=0;
	texpos_item_statue_bands=0;
	texpos_item_statue_blood=0;
	texpos_item_statue_vomit=0;
	texpos_item_statue_water=0;
	texpos_item_statue_mud=0;
	texpos_item_statue_artifact.clear();
	for(s=0;s<2;++s)
		{
		texpos_item_statue_generic_event[s]=0;
		texpos_item_statue_generic_shape[s]=0;
		texpos_item_statue_generic_duel[s]=0;
		texpos_item_statue_generic_triumph[s]=0;
		texpos_item_statue_generic_civilized[s]=0;
		texpos_item_statue_generic_strike_down[s]=0;
		texpos_item_statue_generic_shot[s]=0;
		texpos_item_statue_generic_item_creation[s]=0;
		texpos_item_statue_generic_tree[s]=0;
		texpos_item_statue_generic_shrub[s]=0;
		texpos_item_statue_generic_battle[s]=0;
		texpos_item_statue_generic_site[s]=0;
		}

	texpos_item_bodypart_fat=0;
	texpos_item_bodypart_eye=0;
	texpos_item_bodypart_cartilage=0;
	texpos_item_bodypart_lung=0;
	texpos_item_bodypart_heart=0;
	texpos_item_bodypart_intestines=0;
	texpos_item_bodypart_liver=0;
	texpos_item_bodypart_tripe=0;
	texpos_item_bodypart_sweetbread=0;
	texpos_item_bodypart_spleen=0;
	texpos_item_bodypart_kidney=0;
	texpos_item_bodypart_hair=0;
	texpos_item_bodypart_nervous_tissue=0;
	texpos_item_bodypart_brain=0;
	texpos_item_bodypart_skull_1=0;
	texpos_item_bodypart_skull_2=0;
	texpos_item_bodypart_skin=0;
	texpos_item_bodypart_meat=0;
	texpos_item_bodypart_horn=0;
	texpos_item_bodypart_hoof=0;
	texpos_item_bodypart_bone=0;
	texpos_item_bodypart_teeth=0;
	texpos_item_bodypart_large_1=0;
	texpos_item_bodypart_large_2=0;
	texpos_item_bodypart_large_3=0;
	texpos_item_bodypart_small_1=0;
	texpos_item_bodypart_small_2=0;
	texpos_item_bodypart_small_3=0;
	texpos_item_bodypart_shell=0;
	texpos_item_bodypart_scale=0;
	texpos_item_bodypart_chitin_1=0;
	texpos_item_bodypart_chitin_2=0;
	texpos_item_bodypart_wool=0;
	texpos_item_bodypart_antler=0;
	texpos_item_bodypart_gizzard=0;
	texpos_item_bodypart_gizzard_stone=0;

	texpos_natural_nest=0;

	{int32_t d;
	for(d=0;d<9;++d)
		{
		texpos_bld_chain_metal[d][0]=0;
		texpos_bld_chain_rope[d][0]=0;
		texpos_bld_chain_metal[d][1]=0;
		texpos_bld_chain_rope[d][1]=0;
		}}
	texpos_planned_bld_chain_metal=0;
	texpos_planned_bld_chain_rope=0;
	texpos_bld_support_wood=0;
	texpos_bld_support_stone=0;
	texpos_bld_support_metal=0;
	texpos_bld_archery_target_stone=0;
	texpos_bld_archery_target_metal=0;
	texpos_bld_archery_target_wood=0;
	texpos_bld_well=0;
	texpos_bld_well_with_rope=0;
	texpos_bld_well_with_chain=0;
	texpos_bld_well_rope=0;
	texpos_bld_well_chain=0;
	texpos_bld_well_bucket_empty_chain=0;
	texpos_bld_well_bucket_empty_rope=0;
	texpos_bld_well_bucket_full_chain=0;
	texpos_bld_well_bucket_full_rope=0;
	texpos_bld_paved_road=0;
	texpos_bld_dirt_road=0;
	texpos_planned_bld_archery_target_stone=0;
	texpos_planned_bld_archery_target_metal=0;
	texpos_planned_bld_archery_target_wood=0;
	texpos_planned_bld_well=0;
	texpos_planned_bld_floodgate_stone=0;
	texpos_planned_bld_floodgate_wooden=0;
	texpos_planned_bld_floodgate_metal=0;
	texpos_planned_bld_floodgate_glass=0;
	texpos_planned_bld_paved_road=0;
	texpos_planned_bld_dirt_road=0;
	texpos_bld_floor_bars=0;
	texpos_planned_bld_floor_grate=0;
	texpos_planned_bld_wall_grate=0;

	texpos_item_grate_stone=0;
	texpos_item_grate_wooden=0;
	texpos_item_grate_metal=0;
	texpos_item_grate_glass=0;
	texpos_item_grate_stone_wall_closed=0;
	texpos_item_grate_wooden_wall_closed=0;
	texpos_item_grate_metal_wall_closed=0;
	texpos_item_grate_glass_wall_closed=0;
	texpos_item_grate_stone_wall_open=0;
	texpos_item_grate_wooden_wall_open=0;
	texpos_item_grate_metal_wall_open=0;
	texpos_item_grate_glass_wall_open=0;
	texpos_item_grate_stone_floor_closed=0;
	texpos_item_grate_wooden_floor_closed=0;
	texpos_item_grate_metal_floor_closed=0;
	texpos_item_grate_glass_floor_closed=0;
	texpos_item_grate_stone_floor_open=0;
	texpos_item_grate_wooden_floor_open=0;
	texpos_item_grate_metal_floor_open=0;
	texpos_item_grate_glass_floor_open=0;

	texpos_bld_vertical_bars_w=0;
	texpos_bld_vertical_bars_e=0;
	texpos_bld_vertical_bars_n_signpost=0;
	texpos_bld_vertical_bars_n_back=0;
	texpos_bld_vertical_bars_s=0;
	texpos_bld_vertical_bars_we=0;
	texpos_bld_vertical_bars_nwe=0;
	texpos_bld_vertical_bars_nw=0;
	texpos_bld_vertical_bars_ne=0;
	texpos_bld_vertical_bars_sw=0;
	texpos_bld_vertical_bars_se=0;
	texpos_bld_vertical_bars_nsw=0;
	texpos_bld_vertical_bars_nse=0;
	texpos_bld_vertical_bars_swe=0;
	texpos_bld_vertical_bars_ns=0;

	int32_t t;
	for(t=0;t<TEXTURE_BRIDGENUM;++t)
		{
		gps.texpos_bld_bridge_wood[t]=0;
		gps.texpos_bld_bridge_stone[t]=0;
		gps.texpos_bld_bridge_metal[t]=0;
		gps.texpos_bld_bridge_glass[t]=0;
		gps.texpos_planned_bld_bridge_wood[t]=0;
		gps.texpos_planned_bld_bridge_stone[t]=0;
		gps.texpos_planned_bld_bridge_metal[t]=0;
		gps.texpos_planned_bld_bridge_glass[t]=0;
		}

	texpos_vermin_swarm_large=0;
	texpos_vermin_swarm_medium=0;
	texpos_vermin_swarm_small=0;
	texpos_vermin_light_swarm_large=0;
	texpos_vermin_light_swarm_medium=0;
	texpos_vermin_light_swarm_small=0;
	texpos_vermin_light=0;

	texpos_planned_bld_cage=0;
	texpos_planned_bld_bag=0;
	texpos_planned_bld_box=0;
	texpos_planned_bld_cabinet=0;
	texpos_planned_bld_table=0;
	texpos_planned_bld_chair=0;
	texpos_planned_bld_bed=0;
	texpos_planned_bld_statue=0;
	texpos_planned_bld_slab=0;
	texpos_planned_bld_door_stone=0;
	texpos_planned_bld_door_wooden=0;
	texpos_planned_bld_door_metal=0;
	texpos_planned_bld_door_glass=0;
	texpos_planned_bld_hatch_cover=0;
	texpos_planned_bld_coffin_wooden=0;
	texpos_planned_bld_coffin_stone=0;
	texpos_planned_bld_coffin_metal=0;
	texpos_planned_bld_coffin_glass=0;
	texpos_planned_bld_animal_trap=0;
	texpos_planned_bld_traction_bench_stone_chain=0;
	texpos_planned_bld_traction_bench_stone_rope=0;
	texpos_planned_bld_traction_bench_wooden_chain=0;
	texpos_planned_bld_traction_bench_wooden_rope=0;
	texpos_planned_bld_traction_bench_metal_chain=0;
	texpos_planned_bld_traction_bench_metal_rope=0;
	texpos_planned_bld_traction_bench_glass_chain=0;
	texpos_planned_bld_traction_bench_glass_rope=0;
	{int32_t tx,ty,s;
	for(tx=0;tx<3;++tx)
		{
		for(ty=0;ty<4;++ty)
			{
			for(s=0;s<2;++s)
				{
				texpos_windmill_n[tx][ty][s]=0;
				texpos_windmill_s[tx][ty][s]=0;
				texpos_windmill_w[tx][ty][s]=0;
				texpos_windmill_e[tx][ty][s]=0;
				texpos_windmill_nw[tx][ty][s]=0;
				texpos_windmill_ne[tx][ty][s]=0;
				texpos_windmill_sw[tx][ty][s]=0;
				texpos_windmill_se[tx][ty][s]=0;
				}
			}
		}}
	{int32_t x,y,z;
	for(y=0;y<4;++y)
		{
		for(z=0;z<2;++z)texpos_water_wheel_ns[y][z]=0;
		}
	for(x=0;x<3;++x)
		{
		for(y=0;y<2;++y)
			{
			for(z=0;z<2;++z)texpos_water_wheel_we[x][y][z]=0;
			}
		}}
	{int32_t tx,s;
	for(tx=0;tx<2;++tx)
		{
		for(s=0;s<2;++s)
			{
			texpos_screwpump_n[tx][s]=0;
			texpos_screwpump_s[tx][s]=0;
			texpos_screwpump_w[tx][s]=0;
			texpos_screwpump_e[tx][s]=0;
			}
		texpos_screwpump_axle[tx]=0;
		}}
	texpos_axle_horizontal_ns[0]=0;
	texpos_axle_horizontal_ns[1]=0;
	texpos_axle_horizontal_we[0]=0;
	texpos_axle_horizontal_we[1]=0;
	texpos_axle_horizontal_ns_fittings[0]=0;
	texpos_axle_horizontal_ns_fittings[1]=0;
	texpos_axle_horizontal_we_fittings[0]=0;
	texpos_axle_horizontal_we_fittings[1]=0;
	texpos_axle_vertical[0]=0;
	texpos_axle_vertical[1]=0;
	texpos_axle_vertical_fittings[0]=0;
	texpos_axle_vertical_fittings[1]=0;
	texpos_gear_assembly[0]=0;
	texpos_gear_assembly[1]=0;
	texpos_gear_assembly_axle_s[0]=0;
	texpos_gear_assembly_axle_s[1]=0;
	texpos_planned_gear_assembly=0;
	texpos_farmplot=0;
	texpos_planned_farmplot=0;
	texpos_farmplot_planted=0;
	memset(texpos_debris,0,sizeof(int32_t)*16*2);
	memset(texpos_ballista_n,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_s,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_w,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_e,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_nw,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_ne,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_sw,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_se,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_n_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_s_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_w_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_e_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_nw_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_ne_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_sw_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_se_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_ballista_const,0,sizeof(int32_t)*4*3*3);
	memset(texpos_catapult_n,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_s,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_w,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_e,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_nw,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_ne,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_sw,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_se,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_n_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_s_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_w_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_e_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_nw_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_ne_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_sw_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_se_firing,0,sizeof(int32_t)*3*3);
	memset(texpos_catapult_const,0,sizeof(int32_t)*4*3*3);
	texpos_bolt_thrower_ready_n=0;
	texpos_bolt_thrower_ready_ne=0;
	texpos_bolt_thrower_ready_e=0;
	texpos_bolt_thrower_ready_se=0;
	texpos_bolt_thrower_ready_s=0;
	texpos_bolt_thrower_ready_sw=0;
	texpos_bolt_thrower_ready_w=0;
	texpos_bolt_thrower_ready_nw=0;
	texpos_bolt_thrower_firing_n=0;
	texpos_bolt_thrower_firing_ne=0;
	texpos_bolt_thrower_firing_e=0;
	texpos_bolt_thrower_firing_se=0;
	texpos_bolt_thrower_firing_s=0;
	texpos_bolt_thrower_firing_sw=0;
	texpos_bolt_thrower_firing_w=0;
	texpos_bolt_thrower_firing_nw=0;
	memset(texpos_bolt_thrower_ammo_n,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_ne,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_e,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_se,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_s,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_sw,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_w,0,sizeof(int32_t)*5);
	memset(texpos_bolt_thrower_ammo_nw,0,sizeof(int32_t)*5);

	int32_t fl;
	for(fl=0;fl<4;++fl)
		{
		texpos_flow_miasma[fl]=0;
		texpos_flow_dust[fl]=0;
		texpos_flow_water_mist[fl]=0;
		texpos_flow_lava_mist[fl]=0;
		texpos_flow_boiling[fl]=0;
		texpos_flow_vapor[fl]=0;
		texpos_flow_dragonfire[fl]=0;
		texpos_flow_fire[fl]=0;
		texpos_flow_smoke[fl]=0;
		texpos_flow_item[fl]=0;
		}

	texpos_lever_1=0;
	texpos_lever_2=0;
	texpos_planned_lever=0;

	texpos_trap_cage=0;
	texpos_trap_cage_top=0;
	texpos_trap_cage_delayed=0;
	texpos_trap_stone=0;
	texpos_trap_stone_top=0;
	texpos_trap_stone_delayed=0;
	texpos_trap_weapon=0;
	texpos_trap_weapon_delayed=0;
	texpos_trap_weapon_generic_pike=0;
	texpos_trap_weapon_generic_spear=0;
	texpos_trap_weapon_generic_whip=0;
	texpos_trap_weapon_generic_sword=0;
	texpos_trap_weapon_generic_axe=0;
	texpos_trap_weapon_generic_mace=0;
	texpos_trap_weapon_generic_hammer=0;
	texpos_trap_weapon_generic_dagger=0;
	texpos_trap_weapon_generic_crossbow=0;
	texpos_trap_weapon_generic_bow=0;
	texpos_trap_weapon_generic_blowgun=0;
	texpos_trap_plate=0;
	texpos_trap_plate_delayed=0;
	texpos_trap_plate_water=0;
	texpos_trap_plate_delayed_water=0;
	texpos_trap_plate_magma=0;
	texpos_trap_plate_delayed_magma=0;
	texpos_trap_plate_minecart=0;
	texpos_trap_plate_delayed_minecart=0;
	texpos_planned_trap_cage=0;
	texpos_planned_trap_stone=0;
	texpos_planned_trap_weapon=0;
	texpos_planned_trap_plate=0;

	memset(texpos_upright_weapon_retracted,0,sizeof(int32_t)*10);
	texpos_planned_upright_weapon=0;
}

void graphicst::get_play_area_tile_size(int32_t &limx,int32_t &limy)
{
	limx=(::init.display.grid_x);
	limy=(::init.display.grid_y);
	if(init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
		{
		if(gps.main_viewport!=NULL)
			{
			limx=gps.main_viewport->dim_x;
			limy=gps.main_viewport->dim_y;
			}
		}
}

// This should probably end up being moved to raws.
// Eventually. Hopefully.

const unordered_bimap<string,PCGLayering> PCG_LAYERINGSTRINGS={
	{"BEAST_AMORPHOUS",PCG_LAYERING_BEAST_AMORPHOUS},
	{"BEAST_AMORPHOUS_SHELL_BACK",PCG_LAYERING_BEAST_AMORPHOUS_SHELL_BACK},

	{"BEAST_SNAKE",PCG_LAYERING_BEAST_SNAKE},
	{"BEAST_SNAKE_MANDIBLES",PCG_LAYERING_BEAST_SNAKE_MANDIBLES},
	{"BEAST_SNAKE_HORNS",PCG_LAYERING_BEAST_SNAKE_HORNS},
	{"BEAST_SNAKE_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SNAKE_WINGS_LACY_BACK},
	{"BEAST_SNAKE_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SNAKE_WINGS_LACY_FRONT},
	{"BEAST_SNAKE_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SNAKE_WINGS_FEATHERED_BACK},
	{"BEAST_SNAKE_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SNAKE_WINGS_FEATHERED_FRONT},
	{"BEAST_SNAKE_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SNAKE_WINGS_BAT_BACK},
	{"BEAST_SNAKE_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SNAKE_WINGS_BAT_FRONT},
	{"BEAST_SNAKE_EYE_ONE",PCG_LAYERING_BEAST_SNAKE_EYE_ONE},
	{"BEAST_SNAKE_EYE_TWO",PCG_LAYERING_BEAST_SNAKE_EYE_TWO},
	{"BEAST_SNAKE_EYE_THREE",PCG_LAYERING_BEAST_SNAKE_EYE_THREE},
	{"BEAST_SNAKE_SHELL_BACK",PCG_LAYERING_BEAST_SNAKE_SHELL_BACK},
	{"BEAST_SNAKE_TRUNK",PCG_LAYERING_BEAST_SNAKE_TRUNK},
	{"BEAST_SNAKE_ANTENNAE",PCG_LAYERING_BEAST_SNAKE_ANTENNAE},

	{"BEAST_WORM_LONG",PCG_LAYERING_BEAST_WORM_LONG},
	{"BEAST_WORM_LONG_MANDIBLES",PCG_LAYERING_BEAST_WORM_LONG_MANDIBLES},
	{"BEAST_WORM_LONG_HORNS",PCG_LAYERING_BEAST_WORM_LONG_HORNS},
	{"BEAST_WORM_LONG_WINGS_LACY_BACK",PCG_LAYERING_BEAST_WORM_LONG_WINGS_LACY_BACK},
	{"BEAST_WORM_LONG_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_WORM_LONG_WINGS_LACY_FRONT},
	{"BEAST_WORM_LONG_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_WORM_LONG_WINGS_FEATHERED_BACK},
	{"BEAST_WORM_LONG_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_WORM_LONG_WINGS_FEATHERED_FRONT},
	{"BEAST_WORM_LONG_WINGS_BAT_BACK",PCG_LAYERING_BEAST_WORM_LONG_WINGS_BAT_BACK},
	{"BEAST_WORM_LONG_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_WORM_LONG_WINGS_BAT_FRONT},
	{"BEAST_WORM_LONG_EYE_ONE",PCG_LAYERING_BEAST_WORM_LONG_EYE_ONE},
	{"BEAST_WORM_LONG_EYE_TWO",PCG_LAYERING_BEAST_WORM_LONG_EYE_TWO},
	{"BEAST_WORM_LONG_EYE_THREE",PCG_LAYERING_BEAST_WORM_LONG_EYE_THREE},
	{"BEAST_WORM_LONG_SHELL_BACK",PCG_LAYERING_BEAST_WORM_LONG_SHELL_BACK},
	{"BEAST_WORM_LONG_TRUNK",PCG_LAYERING_BEAST_WORM_LONG_TRUNK},
	{"BEAST_WORM_LONG_ANTENNAE",PCG_LAYERING_BEAST_WORM_LONG_ANTENNAE},

	{"BEAST_WORM_SHORT",PCG_LAYERING_BEAST_WORM_SHORT},
	{"BEAST_WORM_SHORT_MANDIBLES",PCG_LAYERING_BEAST_WORM_SHORT_MANDIBLES},
	{"BEAST_WORM_SHORT_HORNS",PCG_LAYERING_BEAST_WORM_SHORT_HORNS},
	{"BEAST_WORM_SHORT_WINGS_LACY_BACK",PCG_LAYERING_BEAST_WORM_SHORT_WINGS_LACY_BACK},
	{"BEAST_WORM_SHORT_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_WORM_SHORT_WINGS_LACY_FRONT},
	{"BEAST_WORM_SHORT_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_WORM_SHORT_WINGS_FEATHERED_BACK},
	{"BEAST_WORM_SHORT_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_WORM_SHORT_WINGS_FEATHERED_FRONT},
	{"BEAST_WORM_SHORT_WINGS_BAT_BACK",PCG_LAYERING_BEAST_WORM_SHORT_WINGS_BAT_BACK},
	{"BEAST_WORM_SHORT_EYE_ONE",PCG_LAYERING_BEAST_WORM_SHORT_EYE_ONE},
	{"BEAST_WORM_SHORT_EYE_TWO",PCG_LAYERING_BEAST_WORM_SHORT_EYE_TWO},
	{"BEAST_WORM_SHORT_EYE_THREE",PCG_LAYERING_BEAST_WORM_SHORT_EYE_THREE},
	{"BEAST_WORM_SHORT_SHELL_BACK",PCG_LAYERING_BEAST_WORM_SHORT_SHELL_BACK},
	{"BEAST_WORM_SHORT_TRUNK",PCG_LAYERING_BEAST_WORM_SHORT_TRUNK},
	{"BEAST_WORM_SHORT_ANTENNAE",PCG_LAYERING_BEAST_WORM_SHORT_ANTENNAE},

	{"BEAST_INSECT",PCG_LAYERING_BEAST_INSECT},
	{"BEAST_INSECT_MANDIBLES",PCG_LAYERING_BEAST_INSECT_MANDIBLES},
	{"BEAST_INSECT_HORNS",PCG_LAYERING_BEAST_INSECT_HORNS},
	{"BEAST_INSECT_WINGS_LACY_BACK",PCG_LAYERING_BEAST_INSECT_WINGS_LACY_BACK},
	{"BEAST_INSECT_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_INSECT_WINGS_FEATHERED_FRONT},
	{"BEAST_INSECT_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_INSECT_WINGS_BAT_FRONT},
	{"BEAST_INSECT_EYE_ONE",PCG_LAYERING_BEAST_INSECT_EYE_ONE},
	{"BEAST_INSECT_EYE_TWO",PCG_LAYERING_BEAST_INSECT_EYE_TWO},
	{"BEAST_INSECT_EYE_THREE",PCG_LAYERING_BEAST_INSECT_EYE_THREE},
	{"BEAST_INSECT_PROBOSCIS",PCG_LAYERING_BEAST_INSECT_PROBOSCIS},
	{"BEAST_INSECT_SHELL_BACK",PCG_LAYERING_BEAST_INSECT_SHELL_BACK},
	{"BEAST_INSECT_TRUNK",PCG_LAYERING_BEAST_INSECT_TRUNK},
	{"BEAST_INSECT_ANTENNAE",PCG_LAYERING_BEAST_INSECT_ANTENNAE},

	{"BEAST_SPIDER",PCG_LAYERING_BEAST_SPIDER},
	{"BEAST_SPIDER_MANDIBLES",PCG_LAYERING_BEAST_SPIDER_MANDIBLES},
	{"BEAST_SPIDER_HORNS",PCG_LAYERING_BEAST_SPIDER_HORNS},
	{"BEAST_SPIDER_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SPIDER_WINGS_LACY_FRONT},
	{"BEAST_SPIDER_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SPIDER_WINGS_FEATHERED_FRONT},
	{"BEAST_SPIDER_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SPIDER_WINGS_BAT_FRONT},
	{"BEAST_SPIDER_EYE_ONE",PCG_LAYERING_BEAST_SPIDER_EYE_ONE},
	{"BEAST_SPIDER_EYE_TWO",PCG_LAYERING_BEAST_SPIDER_EYE_TWO},
	{"BEAST_SPIDER_EYE_THREE",PCG_LAYERING_BEAST_SPIDER_EYE_THREE},
	{"BEAST_SPIDER_SHELL_FRONT",PCG_LAYERING_BEAST_SPIDER_SHELL_FRONT},
	{"BEAST_SPIDER_TRUNK",PCG_LAYERING_BEAST_SPIDER_TRUNK},
	{"BEAST_SPIDER_ANTENNAE",PCG_LAYERING_BEAST_SPIDER_ANTENNAE},

	{"BEAST_SCORPION",PCG_LAYERING_BEAST_SCORPION},
	{"BEAST_SCORPION_MANDIBLES",PCG_LAYERING_BEAST_SCORPION_MANDIBLES},
	{"BEAST_SCORPION_HORNS",PCG_LAYERING_BEAST_SCORPION_HORNS},
	{"BEAST_SCORPION_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SCORPION_WINGS_LACY_BACK},
	{"BEAST_SCORPION_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SCORPION_WINGS_LACY_FRONT},
	{"BEAST_SCORPION_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SCORPION_WINGS_FEATHERED_BACK},
	{"BEAST_SCORPION_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SCORPION_WINGS_FEATHERED_FRONT},
	{"BEAST_SCORPION_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SCORPION_WINGS_BAT_BACK},
	{"BEAST_SCORPION_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SCORPION_WINGS_BAT_FRONT},
	{"BEAST_SCORPION_EYE_ONE",PCG_LAYERING_BEAST_SCORPION_EYE_ONE},
	{"BEAST_SCORPION_EYE_TWO",PCG_LAYERING_BEAST_SCORPION_EYE_TWO},
	{"BEAST_SCORPION_EYE_THREE",PCG_LAYERING_BEAST_SCORPION_EYE_THREE},
	{"BEAST_SCORPION_TAIL_ONE",PCG_LAYERING_BEAST_SCORPION_TAIL_ONE},
	{"BEAST_SCORPION_TAIL_TWO",PCG_LAYERING_BEAST_SCORPION_TAIL_TWO},
	{"BEAST_SCORPION_TAIL_THREE",PCG_LAYERING_BEAST_SCORPION_TAIL_THREE},
	{"BEAST_SCORPION_SHELL_FRONT",PCG_LAYERING_BEAST_SCORPION_SHELL_FRONT},
	{"BEAST_SCORPION_TRUNK",PCG_LAYERING_BEAST_SCORPION_TRUNK},
	{"BEAST_SCORPION_ANTENNAE",PCG_LAYERING_BEAST_SCORPION_ANTENNAE},

	{"BEAST_BIPEDAL_DINOSAUR",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR},
	{"BEAST_BIPEDAL_DINOSAUR_MANDIBLES",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_MANDIBLES},
	{"BEAST_BIPEDAL_DINOSAUR_HORNS",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_HORNS},
	{"BEAST_BIPEDAL_DINOSAUR_WINGS_LACY_BACK",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_WINGS_LACY_BACK},
	{"BEAST_BIPEDAL_DINOSAUR_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_WINGS_LACY_FRONT},
	{"BEAST_BIPEDAL_DINOSAUR_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_WINGS_FEATHERED_BACK},
	{"BEAST_BIPEDAL_DINOSAUR_WINGS_BAT_BACK",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_WINGS_BAT_BACK},
	{"BEAST_BIPEDAL_DINOSAUR_EYE_ONE",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_EYE_ONE},
	{"BEAST_BIPEDAL_DINOSAUR_EYE_TWO",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_EYE_TWO},
	{"BEAST_BIPEDAL_DINOSAUR_EYE_THREE",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_EYE_THREE},
	{"BEAST_BIPEDAL_DINOSAUR_TAIL_ONE",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_TAIL_ONE},
	{"BEAST_BIPEDAL_DINOSAUR_TAIL_TWO",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_TAIL_TWO},
	{"BEAST_BIPEDAL_DINOSAUR_TAIL_THREE",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_TAIL_THREE},
	{"BEAST_BIPEDAL_DINOSAUR_SHELL_BACK",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_SHELL_BACK},
	{"BEAST_BIPEDAL_DINOSAUR_TRUNK",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_TRUNK},
	{"BEAST_BIPEDAL_DINOSAUR_ANTENNAE",PCG_LAYERING_BEAST_BIPEDAL_DINOSAUR_ANTENNAE},

	{"BEAST_HUMANOID",PCG_LAYERING_BEAST_HUMANOID},
	{"BEAST_HUMANOID_MANDIBLES",PCG_LAYERING_BEAST_HUMANOID_MANDIBLES},
	{"BEAST_HUMANOID_HORNS",PCG_LAYERING_BEAST_HUMANOID_HORNS},
	{"BEAST_HUMANOID_WINGS_LACY_BACK",PCG_LAYERING_BEAST_HUMANOID_WINGS_LACY_BACK},
	{"BEAST_HUMANOID_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_HUMANOID_WINGS_LACY_FRONT},
	{"BEAST_HUMANOID_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_HUMANOID_WINGS_FEATHERED_BACK},
	{"BEAST_HUMANOID_WINGS_BAT_BACK",PCG_LAYERING_BEAST_HUMANOID_WINGS_BAT_BACK},
	{"BEAST_HUMANOID_EYE_ONE",PCG_LAYERING_BEAST_HUMANOID_EYE_ONE},
	{"BEAST_HUMANOID_EYE_TWO",PCG_LAYERING_BEAST_HUMANOID_EYE_TWO},
	{"BEAST_HUMANOID_EYE_THREE",PCG_LAYERING_BEAST_HUMANOID_EYE_THREE},
	{"BEAST_HUMANOID_TAIL_ONE",PCG_LAYERING_BEAST_HUMANOID_TAIL_ONE},
	{"BEAST_HUMANOID_TAIL_TWO",PCG_LAYERING_BEAST_HUMANOID_TAIL_TWO},
	{"BEAST_HUMANOID_TAIL_THREE",PCG_LAYERING_BEAST_HUMANOID_TAIL_THREE},
	{"BEAST_HUMANOID_SHELL_BACK",PCG_LAYERING_BEAST_HUMANOID_SHELL_BACK},
	{"BEAST_HUMANOID_TRUNK",PCG_LAYERING_BEAST_HUMANOID_TRUNK},
	{"BEAST_HUMANOID_ANTENNAE",PCG_LAYERING_BEAST_HUMANOID_ANTENNAE},

	{"BEAST_FRONT_GRASP",PCG_LAYERING_BEAST_FRONT_GRASP},
	{"BEAST_FRONT_GRASP_HEX",PCG_LAYERING_BEAST_FRONT_GRASP_HEX},
	{"BEAST_FRONT_GRASP_OCT",PCG_LAYERING_BEAST_FRONT_GRASP_OCT},
	{"BEAST_FRONT_GRASP_MANDIBLES",PCG_LAYERING_BEAST_FRONT_GRASP_MANDIBLES},
	{"BEAST_FRONT_GRASP_HORNS",PCG_LAYERING_BEAST_FRONT_GRASP_HORNS},
	{"BEAST_FRONT_GRASP_WINGS_LACY_BACK",PCG_LAYERING_BEAST_FRONT_GRASP_WINGS_LACY_BACK},
	{"BEAST_FRONT_GRASP_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_FRONT_GRASP_WINGS_LACY_FRONT},
	{"BEAST_FRONT_GRASP_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_FRONT_GRASP_WINGS_FEATHERED_BACK},
	{"BEAST_FRONT_GRASP_WINGS_BAT_BACK",PCG_LAYERING_BEAST_FRONT_GRASP_WINGS_BAT_BACK},
	{"BEAST_FRONT_GRASP_EYE_ONE",PCG_LAYERING_BEAST_FRONT_GRASP_EYE_ONE},
	{"BEAST_FRONT_GRASP_EYE_TWO",PCG_LAYERING_BEAST_FRONT_GRASP_EYE_TWO},
	{"BEAST_FRONT_GRASP_EYE_THREE",PCG_LAYERING_BEAST_FRONT_GRASP_EYE_THREE},
	{"BEAST_FRONT_GRASP_TAIL_ONE",PCG_LAYERING_BEAST_FRONT_GRASP_TAIL_ONE},
	{"BEAST_FRONT_GRASP_TAIL_TWO",PCG_LAYERING_BEAST_FRONT_GRASP_TAIL_TWO},
	{"BEAST_FRONT_GRASP_TAIL_THREE",PCG_LAYERING_BEAST_FRONT_GRASP_TAIL_THREE},
	{"BEAST_FRONT_GRASP_SHELL_BACK",PCG_LAYERING_BEAST_FRONT_GRASP_SHELL_BACK},
	{"BEAST_FRONT_GRASP_TRUNK",PCG_LAYERING_BEAST_FRONT_GRASP_TRUNK},
	{"BEAST_FRONT_GRASP_ANTENNAE",PCG_LAYERING_BEAST_FRONT_GRASP_ANTENNAE},

	{"BEAST_QUADRUPED_BULKY",PCG_LAYERING_BEAST_QUADRUPED_BULKY},
	{"BEAST_QUADRUPED_BULKY_HEX",PCG_LAYERING_BEAST_QUADRUPED_BULKY_HEX},
	{"BEAST_QUADRUPED_BULKY_OCT",PCG_LAYERING_BEAST_QUADRUPED_BULKY_OCT},
	{"BEAST_QUADRUPED_BULKY_MANDIBLES",PCG_LAYERING_BEAST_QUADRUPED_BULKY_MANDIBLES},
	{"BEAST_QUADRUPED_BULKY_HORNS",PCG_LAYERING_BEAST_QUADRUPED_BULKY_HORNS},
	{"BEAST_QUADRUPED_BULKY_WINGS_LACY_BACK",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_LACY_BACK},
	{"BEAST_QUADRUPED_BULKY_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_LACY_FRONT},
	{"BEAST_QUADRUPED_BULKY_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_FEATHERED_BACK},
	{"BEAST_QUADRUPED_BULKY_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_FEATHERED_FRONT},
	{"BEAST_QUADRUPED_BULKY_WINGS_BAT_BACK",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_BAT_BACK},
	{"BEAST_QUADRUPED_BULKY_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_QUADRUPED_BULKY_WINGS_BAT_FRONT},
	{"BEAST_QUADRUPED_BULKY_EYE_ONE",PCG_LAYERING_BEAST_QUADRUPED_BULKY_EYE_ONE},
	{"BEAST_QUADRUPED_BULKY_EYE_TWO",PCG_LAYERING_BEAST_QUADRUPED_BULKY_EYE_TWO},
	{"BEAST_QUADRUPED_BULKY_EYE_THREE",PCG_LAYERING_BEAST_QUADRUPED_BULKY_EYE_THREE},
	{"BEAST_QUADRUPED_BULKY_TAIL_ONE",PCG_LAYERING_BEAST_QUADRUPED_BULKY_TAIL_ONE},
	{"BEAST_QUADRUPED_BULKY_TAIL_TWO",PCG_LAYERING_BEAST_QUADRUPED_BULKY_TAIL_TWO},
	{"BEAST_QUADRUPED_BULKY_TAIL_THREE",PCG_LAYERING_BEAST_QUADRUPED_BULKY_TAIL_THREE},
	{"BEAST_QUADRUPED_BULKY_SHELL_FRONT",PCG_LAYERING_BEAST_QUADRUPED_BULKY_SHELL_FRONT},
	{"BEAST_QUADRUPED_BULKY_TRUNK",PCG_LAYERING_BEAST_QUADRUPED_BULKY_TRUNK},
	{"BEAST_QUADRUPED_BULKY_ANTENNAE",PCG_LAYERING_BEAST_QUADRUPED_BULKY_ANTENNAE},

	{"BEAST_QUADRUPED_SLINKY",PCG_LAYERING_BEAST_QUADRUPED_SLINKY},
	{"BEAST_QUADRUPED_SLINKY_HEX",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_HEX},
	{"BEAST_QUADRUPED_SLINKY_OCT",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_OCT},
	{"BEAST_QUADRUPED_SLINKY_MANDIBLES",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_MANDIBLES},
	{"BEAST_QUADRUPED_SLINKY_HORNS",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_HORNS},
	{"BEAST_QUADRUPED_SLINKY_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_WINGS_LACY_FRONT},
	{"BEAST_QUADRUPED_SLINKY_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_WINGS_FEATHERED_BACK},
	{"BEAST_QUADRUPED_SLINKY_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_WINGS_FEATHERED_FRONT},
	{"BEAST_QUADRUPED_SLINKY_WINGS_BAT_BACK",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_WINGS_BAT_BACK},
	{"BEAST_QUADRUPED_SLINKY_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_WINGS_BAT_FRONT},
	{"BEAST_QUADRUPED_SLINKY_EYE_ONE",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_EYE_ONE},
	{"BEAST_QUADRUPED_SLINKY_EYE_TWO",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_EYE_TWO},
	{"BEAST_QUADRUPED_SLINKY_EYE_THREE",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_EYE_THREE},
	{"BEAST_QUADRUPED_SLINKY_TAIL_ONE",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_TAIL_ONE},
	{"BEAST_QUADRUPED_SLINKY_TAIL_TWO",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_TAIL_TWO},
	{"BEAST_QUADRUPED_SLINKY_TAIL_THREE",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_TAIL_THREE},
	{"BEAST_QUADRUPED_SLINKY_SHELL_FRONT",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_SHELL_FRONT},
	{"BEAST_QUADRUPED_SLINKY_TRUNK",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_TRUNK},
	{"BEAST_QUADRUPED_SLINKY_ANTENNAE",PCG_LAYERING_BEAST_QUADRUPED_SLINKY_ANTENNAE},

	{"BEAST_WALRUS",PCG_LAYERING_BEAST_WALRUS},
	{"BEAST_WALRUS_MANDIBLES",PCG_LAYERING_BEAST_WALRUS_MANDIBLES},
	{"BEAST_WALRUS_WINGS_LACY_BACK",PCG_LAYERING_BEAST_WALRUS_WINGS_LACY_BACK},
	{"BEAST_WALRUS_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_WALRUS_WINGS_LACY_FRONT},
	{"BEAST_WALRUS_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_WALRUS_WINGS_FEATHERED_BACK},
	{"BEAST_WALRUS_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_WALRUS_WINGS_FEATHERED_FRONT},
	{"BEAST_WALRUS_WINGS_BAT_BACK",PCG_LAYERING_BEAST_WALRUS_WINGS_BAT_BACK},
	{"BEAST_WALRUS_EYE_ONE",PCG_LAYERING_BEAST_WALRUS_EYE_ONE},
	{"BEAST_WALRUS_EYE_TWO",PCG_LAYERING_BEAST_WALRUS_EYE_TWO},
	{"BEAST_WALRUS_EYE_THREE",PCG_LAYERING_BEAST_WALRUS_EYE_THREE},
	{"BEAST_WALRUS_TAIL_ONE",PCG_LAYERING_BEAST_WALRUS_TAIL_ONE},
	{"BEAST_WALRUS_TAIL_TWO",PCG_LAYERING_BEAST_WALRUS_TAIL_TWO},
	{"BEAST_WALRUS_TAIL_THREE",PCG_LAYERING_BEAST_WALRUS_TAIL_THREE},
	{"BEAST_WALRUS_SHELL_BACK",PCG_LAYERING_BEAST_WALRUS_SHELL_BACK},
	{"BEAST_WALRUS_TRUNK",PCG_LAYERING_BEAST_WALRUS_TRUNK},
	{"BEAST_WALRUS_ANTENNAE",PCG_LAYERING_BEAST_WALRUS_ANTENNAE},

	{"BEAST_SMALL_AMORPHOUS",PCG_LAYERING_BEAST_SMALL_AMORPHOUS},
	{"BEAST_SMALL_AMORPHOUS_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_AMORPHOUS_SHELL_BACK},

	{"BEAST_SMALL_SNAKE",PCG_LAYERING_BEAST_SMALL_SNAKE},
	{"BEAST_SMALL_SNAKE_MANDIBLES",PCG_LAYERING_BEAST_SMALL_SNAKE_MANDIBLES},
	{"BEAST_SMALL_SNAKE_HORNS",PCG_LAYERING_BEAST_SMALL_SNAKE_HORNS},
	{"BEAST_SMALL_SNAKE_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_LACY_BACK},
	{"BEAST_SMALL_SNAKE_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_LACY_FRONT},
	{"BEAST_SMALL_SNAKE_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_SNAKE_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_SNAKE_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_BAT_BACK},
	{"BEAST_SMALL_SNAKE_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_SNAKE_WINGS_BAT_FRONT},
	{"BEAST_SMALL_SNAKE_EYE_ONE",PCG_LAYERING_BEAST_SMALL_SNAKE_EYE_ONE},
	{"BEAST_SMALL_SNAKE_EYE_TWO",PCG_LAYERING_BEAST_SMALL_SNAKE_EYE_TWO},
	{"BEAST_SMALL_SNAKE_EYE_THREE",PCG_LAYERING_BEAST_SMALL_SNAKE_EYE_THREE},
	{"BEAST_SMALL_SNAKE_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_SNAKE_SHELL_BACK},
	{"BEAST_SMALL_SNAKE_TRUNK",PCG_LAYERING_BEAST_SMALL_SNAKE_TRUNK},
	{"BEAST_SMALL_SNAKE_ANTENNAE",PCG_LAYERING_BEAST_SMALL_SNAKE_ANTENNAE},

	{"BEAST_SMALL_WORM_LONG",PCG_LAYERING_BEAST_SMALL_WORM_LONG},
	{"BEAST_SMALL_WORM_LONG_MANDIBLES",PCG_LAYERING_BEAST_SMALL_WORM_LONG_MANDIBLES},
	{"BEAST_SMALL_WORM_LONG_HORNS",PCG_LAYERING_BEAST_SMALL_WORM_LONG_HORNS},
	{"BEAST_SMALL_WORM_LONG_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_LACY_BACK},
	{"BEAST_SMALL_WORM_LONG_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_LACY_FRONT},
	{"BEAST_SMALL_WORM_LONG_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_WORM_LONG_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_WORM_LONG_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_BAT_BACK},
	{"BEAST_SMALL_WORM_LONG_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_WORM_LONG_WINGS_BAT_FRONT},
	{"BEAST_SMALL_WORM_LONG_EYE_ONE",PCG_LAYERING_BEAST_SMALL_WORM_LONG_EYE_ONE},
	{"BEAST_SMALL_WORM_LONG_EYE_TWO",PCG_LAYERING_BEAST_SMALL_WORM_LONG_EYE_TWO},
	{"BEAST_SMALL_WORM_LONG_EYE_THREE",PCG_LAYERING_BEAST_SMALL_WORM_LONG_EYE_THREE},
	{"BEAST_SMALL_WORM_LONG_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_WORM_LONG_SHELL_BACK},
	{"BEAST_SMALL_WORM_LONG_TRUNK",PCG_LAYERING_BEAST_SMALL_WORM_LONG_TRUNK},
	{"BEAST_SMALL_WORM_LONG_ANTENNAE",PCG_LAYERING_BEAST_SMALL_WORM_LONG_ANTENNAE},

	{"BEAST_SMALL_WORM_SHORT",PCG_LAYERING_BEAST_SMALL_WORM_SHORT},
	{"BEAST_SMALL_WORM_SHORT_MANDIBLES",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_MANDIBLES},
	{"BEAST_SMALL_WORM_SHORT_HORNS",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_HORNS},
	{"BEAST_SMALL_WORM_SHORT_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_WINGS_LACY_BACK},
	{"BEAST_SMALL_WORM_SHORT_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_WINGS_LACY_FRONT},
	{"BEAST_SMALL_WORM_SHORT_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_WORM_SHORT_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_WORM_SHORT_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_WINGS_BAT_BACK},
	{"BEAST_SMALL_WORM_SHORT_EYE_ONE",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_EYE_ONE},
	{"BEAST_SMALL_WORM_SHORT_EYE_TWO",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_EYE_TWO},
	{"BEAST_SMALL_WORM_SHORT_EYE_THREE",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_EYE_THREE},
	{"BEAST_SMALL_WORM_SHORT_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_SHELL_BACK},
	{"BEAST_SMALL_WORM_SHORT_TRUNK",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_TRUNK},
	{"BEAST_SMALL_WORM_SHORT_ANTENNAE",PCG_LAYERING_BEAST_SMALL_WORM_SHORT_ANTENNAE},

	{"BEAST_SMALL_INSECT",PCG_LAYERING_BEAST_SMALL_INSECT},
	{"BEAST_SMALL_INSECT_MANDIBLES",PCG_LAYERING_BEAST_SMALL_INSECT_MANDIBLES},
	{"BEAST_SMALL_INSECT_HORNS",PCG_LAYERING_BEAST_SMALL_INSECT_HORNS},
	{"BEAST_SMALL_INSECT_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_INSECT_WINGS_LACY_BACK},
	{"BEAST_SMALL_INSECT_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_INSECT_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_INSECT_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_INSECT_WINGS_BAT_FRONT},
	{"BEAST_SMALL_INSECT_EYE_ONE",PCG_LAYERING_BEAST_SMALL_INSECT_EYE_ONE},
	{"BEAST_SMALL_INSECT_EYE_TWO",PCG_LAYERING_BEAST_SMALL_INSECT_EYE_TWO},
	{"BEAST_SMALL_INSECT_EYE_THREE",PCG_LAYERING_BEAST_SMALL_INSECT_EYE_THREE},
	{"BEAST_SMALL_INSECT_PROBOSCIS",PCG_LAYERING_BEAST_SMALL_INSECT_PROBOSCIS},
	{"BEAST_SMALL_INSECT_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_INSECT_SHELL_BACK},
	{"BEAST_SMALL_INSECT_TRUNK",PCG_LAYERING_BEAST_SMALL_INSECT_TRUNK},
	{"BEAST_SMALL_INSECT_ANTENNAE",PCG_LAYERING_BEAST_SMALL_INSECT_ANTENNAE},

	{"BEAST_SMALL_SPIDER",PCG_LAYERING_BEAST_SMALL_SPIDER},
	{"BEAST_SMALL_SPIDER_MANDIBLES",PCG_LAYERING_BEAST_SMALL_SPIDER_MANDIBLES},
	{"BEAST_SMALL_SPIDER_HORNS",PCG_LAYERING_BEAST_SMALL_SPIDER_HORNS},
	{"BEAST_SMALL_SPIDER_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_SPIDER_WINGS_LACY_FRONT},
	{"BEAST_SMALL_SPIDER_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_SPIDER_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_SPIDER_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_SPIDER_WINGS_BAT_FRONT},
	{"BEAST_SMALL_SPIDER_EYE_ONE",PCG_LAYERING_BEAST_SMALL_SPIDER_EYE_ONE},
	{"BEAST_SMALL_SPIDER_EYE_TWO",PCG_LAYERING_BEAST_SMALL_SPIDER_EYE_TWO},
	{"BEAST_SMALL_SPIDER_EYE_THREE",PCG_LAYERING_BEAST_SMALL_SPIDER_EYE_THREE},
	{"BEAST_SMALL_SPIDER_SHELL_FRONT",PCG_LAYERING_BEAST_SMALL_SPIDER_SHELL_FRONT},
	{"BEAST_SMALL_SPIDER_TRUNK",PCG_LAYERING_BEAST_SMALL_SPIDER_TRUNK},
	{"BEAST_SMALL_SPIDER_ANTENNAE",PCG_LAYERING_BEAST_SMALL_SPIDER_ANTENNAE},

	{"BEAST_SMALL_SCORPION",PCG_LAYERING_BEAST_SMALL_SCORPION},
	{"BEAST_SMALL_SCORPION_MANDIBLES",PCG_LAYERING_BEAST_SMALL_SCORPION_MANDIBLES},
	{"BEAST_SMALL_SCORPION_HORNS",PCG_LAYERING_BEAST_SMALL_SCORPION_HORNS},
	{"BEAST_SMALL_SCORPION_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_LACY_BACK},
	{"BEAST_SMALL_SCORPION_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_LACY_FRONT},
	{"BEAST_SMALL_SCORPION_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_SCORPION_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_SCORPION_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_BAT_BACK},
	{"BEAST_SMALL_SCORPION_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_SCORPION_WINGS_BAT_FRONT},
	{"BEAST_SMALL_SCORPION_EYE_ONE",PCG_LAYERING_BEAST_SMALL_SCORPION_EYE_ONE},
	{"BEAST_SMALL_SCORPION_EYE_TWO",PCG_LAYERING_BEAST_SMALL_SCORPION_EYE_TWO},
	{"BEAST_SMALL_SCORPION_EYE_THREE",PCG_LAYERING_BEAST_SMALL_SCORPION_EYE_THREE},
	{"BEAST_SMALL_SCORPION_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_SCORPION_TAIL_ONE},
	{"BEAST_SMALL_SCORPION_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_SCORPION_TAIL_TWO},
	{"BEAST_SMALL_SCORPION_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_SCORPION_TAIL_THREE},
	{"BEAST_SMALL_SCORPION_SHELL_FRONT",PCG_LAYERING_BEAST_SMALL_SCORPION_SHELL_FRONT},
	{"BEAST_SMALL_SCORPION_TRUNK",PCG_LAYERING_BEAST_SMALL_SCORPION_TRUNK},
	{"BEAST_SMALL_SCORPION_ANTENNAE",PCG_LAYERING_BEAST_SMALL_SCORPION_ANTENNAE},

	{"BEAST_SMALL_BIPEDAL_DINOSAUR",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_MANDIBLES",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_MANDIBLES},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_HORNS",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_HORNS},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_LACY_BACK},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_LACY_FRONT},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_WINGS_BAT_BACK},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_ONE",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_ONE},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_TWO",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_TWO},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_THREE",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_EYE_THREE},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_ONE},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_TWO},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_TAIL_THREE},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_SHELL_BACK},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_TRUNK",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_TRUNK},
	{"BEAST_SMALL_BIPEDAL_DINOSAUR_ANTENNAE",PCG_LAYERING_BEAST_SMALL_BIPEDAL_DINOSAUR_ANTENNAE},

	{"BEAST_SMALL_HUMANOID",PCG_LAYERING_BEAST_SMALL_HUMANOID},
	{"BEAST_SMALL_HUMANOID_MANDIBLES",PCG_LAYERING_BEAST_SMALL_HUMANOID_MANDIBLES},
	{"BEAST_SMALL_HUMANOID_HORNS",PCG_LAYERING_BEAST_SMALL_HUMANOID_HORNS},
	{"BEAST_SMALL_HUMANOID_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_HUMANOID_WINGS_LACY_BACK},
	{"BEAST_SMALL_HUMANOID_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_HUMANOID_WINGS_LACY_FRONT},
	{"BEAST_SMALL_HUMANOID_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_HUMANOID_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_HUMANOID_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_HUMANOID_WINGS_BAT_BACK},
	{"BEAST_SMALL_HUMANOID_EYE_ONE",PCG_LAYERING_BEAST_SMALL_HUMANOID_EYE_ONE},
	{"BEAST_SMALL_HUMANOID_EYE_TWO",PCG_LAYERING_BEAST_SMALL_HUMANOID_EYE_TWO},
	{"BEAST_SMALL_HUMANOID_EYE_THREE",PCG_LAYERING_BEAST_SMALL_HUMANOID_EYE_THREE},
	{"BEAST_SMALL_HUMANOID_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_HUMANOID_TAIL_ONE},
	{"BEAST_SMALL_HUMANOID_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_HUMANOID_TAIL_TWO},
	{"BEAST_SMALL_HUMANOID_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_HUMANOID_TAIL_THREE},
	{"BEAST_SMALL_HUMANOID_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_HUMANOID_SHELL_BACK},
	{"BEAST_SMALL_HUMANOID_TRUNK",PCG_LAYERING_BEAST_SMALL_HUMANOID_TRUNK},
	{"BEAST_SMALL_HUMANOID_ANTENNAE",PCG_LAYERING_BEAST_SMALL_HUMANOID_ANTENNAE},

	{"BEAST_SMALL_FRONT_GRASP",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP},
	{"BEAST_SMALL_FRONT_GRASP_HEX",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_HEX},
	{"BEAST_SMALL_FRONT_GRASP_OCT",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_OCT},
	{"BEAST_SMALL_FRONT_GRASP_MANDIBLES",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_MANDIBLES},
	{"BEAST_SMALL_FRONT_GRASP_HORNS",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_HORNS},
	{"BEAST_SMALL_FRONT_GRASP_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_WINGS_LACY_BACK},
	{"BEAST_SMALL_FRONT_GRASP_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_WINGS_LACY_FRONT},
	{"BEAST_SMALL_FRONT_GRASP_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_FRONT_GRASP_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_WINGS_BAT_BACK},
	{"BEAST_SMALL_FRONT_GRASP_EYE_ONE",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_EYE_ONE},
	{"BEAST_SMALL_FRONT_GRASP_EYE_TWO",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_EYE_TWO},
	{"BEAST_SMALL_FRONT_GRASP_EYE_THREE",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_EYE_THREE},
	{"BEAST_SMALL_FRONT_GRASP_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_TAIL_ONE},
	{"BEAST_SMALL_FRONT_GRASP_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_TAIL_TWO},
	{"BEAST_SMALL_FRONT_GRASP_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_TAIL_THREE},
	{"BEAST_SMALL_FRONT_GRASP_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_SHELL_BACK},
	{"BEAST_SMALL_FRONT_GRASP_TRUNK",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_TRUNK},
	{"BEAST_SMALL_FRONT_GRASP_ANTENNAE",PCG_LAYERING_BEAST_SMALL_FRONT_GRASP_ANTENNAE},

	{"BEAST_SMALL_QUADRUPED_BULKY",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY},
	{"BEAST_SMALL_QUADRUPED_BULKY_HEX",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_HEX},
	{"BEAST_SMALL_QUADRUPED_BULKY_OCT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_OCT},
	{"BEAST_SMALL_QUADRUPED_BULKY_MANDIBLES",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_MANDIBLES},
	{"BEAST_SMALL_QUADRUPED_BULKY_HORNS",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_HORNS},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_LACY_BACK},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_LACY_FRONT},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_BAT_BACK},
	{"BEAST_SMALL_QUADRUPED_BULKY_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_WINGS_BAT_FRONT},
	{"BEAST_SMALL_QUADRUPED_BULKY_EYE_ONE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_EYE_ONE},
	{"BEAST_SMALL_QUADRUPED_BULKY_EYE_TWO",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_EYE_TWO},
	{"BEAST_SMALL_QUADRUPED_BULKY_EYE_THREE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_EYE_THREE},
	{"BEAST_SMALL_QUADRUPED_BULKY_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_TAIL_ONE},
	{"BEAST_SMALL_QUADRUPED_BULKY_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_TAIL_TWO},
	{"BEAST_SMALL_QUADRUPED_BULKY_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_TAIL_THREE},
	{"BEAST_SMALL_QUADRUPED_BULKY_SHELL_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_SHELL_FRONT},
	{"BEAST_SMALL_QUADRUPED_BULKY_TRUNK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_TRUNK},
	{"BEAST_SMALL_QUADRUPED_BULKY_ANTENNAE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_BULKY_ANTENNAE},

	{"BEAST_SMALL_QUADRUPED_SLINKY",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY},
	{"BEAST_SMALL_QUADRUPED_SLINKY_HEX",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_HEX},
	{"BEAST_SMALL_QUADRUPED_SLINKY_OCT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_OCT},
	{"BEAST_SMALL_QUADRUPED_SLINKY_MANDIBLES",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_MANDIBLES},
	{"BEAST_SMALL_QUADRUPED_SLINKY_HORNS",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_HORNS},
	{"BEAST_SMALL_QUADRUPED_SLINKY_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_WINGS_LACY_FRONT},
	{"BEAST_SMALL_QUADRUPED_SLINKY_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_QUADRUPED_SLINKY_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_QUADRUPED_SLINKY_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_WINGS_BAT_BACK},
	{"BEAST_SMALL_QUADRUPED_SLINKY_WINGS_BAT_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_WINGS_BAT_FRONT},
	{"BEAST_SMALL_QUADRUPED_SLINKY_EYE_ONE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_EYE_ONE},
	{"BEAST_SMALL_QUADRUPED_SLINKY_EYE_TWO",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_EYE_TWO},
	{"BEAST_SMALL_QUADRUPED_SLINKY_EYE_THREE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_EYE_THREE},
	{"BEAST_SMALL_QUADRUPED_SLINKY_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_TAIL_ONE},
	{"BEAST_SMALL_QUADRUPED_SLINKY_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_TAIL_TWO},
	{"BEAST_SMALL_QUADRUPED_SLINKY_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_TAIL_THREE},
	{"BEAST_SMALL_QUADRUPED_SLINKY_SHELL_FRONT",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_SHELL_FRONT},
	{"BEAST_SMALL_QUADRUPED_SLINKY_TRUNK",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_TRUNK},
	{"BEAST_SMALL_QUADRUPED_SLINKY_ANTENNAE",PCG_LAYERING_BEAST_SMALL_QUADRUPED_SLINKY_ANTENNAE},

	{"BEAST_SMALL_WALRUS",PCG_LAYERING_BEAST_SMALL_WALRUS},
	{"BEAST_SMALL_WALRUS_MANDIBLES",PCG_LAYERING_BEAST_SMALL_WALRUS_MANDIBLES},
	{"BEAST_SMALL_WALRUS_WINGS_LACY_BACK",PCG_LAYERING_BEAST_SMALL_WALRUS_WINGS_LACY_BACK},
	{"BEAST_SMALL_WALRUS_WINGS_LACY_FRONT",PCG_LAYERING_BEAST_SMALL_WALRUS_WINGS_LACY_FRONT},
	{"BEAST_SMALL_WALRUS_WINGS_FEATHERED_BACK",PCG_LAYERING_BEAST_SMALL_WALRUS_WINGS_FEATHERED_BACK},
	{"BEAST_SMALL_WALRUS_WINGS_FEATHERED_FRONT",PCG_LAYERING_BEAST_SMALL_WALRUS_WINGS_FEATHERED_FRONT},
	{"BEAST_SMALL_WALRUS_WINGS_BAT_BACK",PCG_LAYERING_BEAST_SMALL_WALRUS_WINGS_BAT_BACK},
	{"BEAST_SMALL_WALRUS_EYE_ONE",PCG_LAYERING_BEAST_SMALL_WALRUS_EYE_ONE},
	{"BEAST_SMALL_WALRUS_EYE_TWO",PCG_LAYERING_BEAST_SMALL_WALRUS_EYE_TWO},
	{"BEAST_SMALL_WALRUS_EYE_THREE",PCG_LAYERING_BEAST_SMALL_WALRUS_EYE_THREE},
	{"BEAST_SMALL_WALRUS_TAIL_ONE",PCG_LAYERING_BEAST_SMALL_WALRUS_TAIL_ONE},
	{"BEAST_SMALL_WALRUS_TAIL_TWO",PCG_LAYERING_BEAST_SMALL_WALRUS_TAIL_TWO},
	{"BEAST_SMALL_WALRUS_TAIL_THREE",PCG_LAYERING_BEAST_SMALL_WALRUS_TAIL_THREE},
	{"BEAST_SMALL_WALRUS_SHELL_BACK",PCG_LAYERING_BEAST_SMALL_WALRUS_SHELL_BACK},
	{"BEAST_SMALL_WALRUS_TRUNK",PCG_LAYERING_BEAST_SMALL_WALRUS_TRUNK},
	{"BEAST_SMALL_WALRUS_ANTENNAE",PCG_LAYERING_BEAST_SMALL_WALRUS_ANTENNAE},

	/*
	{"NIGHT_TROLL_BODY_EYELESS",PCG_LAYERING_NIGHT_TROLL_BODY_EYELESS},
	{"NIGHT_TROLL_BODY_ONE_EYE",PCG_LAYERING_NIGHT_TROLL_BODY_ONE_EYE},
	{"NIGHT_TROLL_BODY_TWO_EYES",PCG_LAYERING_NIGHT_TROLL_BODY_TWO_EYES},
	{"NIGHT_TROLL_BODY_THREE_EYES",PCG_LAYERING_NIGHT_TROLL_BODY_THREE_EYES},
	{"NIGHT_TROLL_BODY_HAIRY",PCG_LAYERING_NIGHT_TROLL_BODY_HAIRY},
	{"NIGHT_TROLL_BODY_SCALY",PCG_LAYERING_NIGHT_TROLL_BODY_SCALY},
	{"NIGHT_TROLL_BODY_SKINLESS",PCG_LAYERING_NIGHT_TROLL_BODY_SKINLESS},
	{"NIGHT_TROLL_BODY_FEATHERED",PCG_LAYERING_NIGHT_TROLL_BODY_FEATHERED},
	{"NIGHT_TROLL_TAIL_THIN_1",PCG_LAYERING_NIGHT_TROLL_TAIL_THIN_1},
	{"NIGHT_TROLL_TAIL_THIN_2",PCG_LAYERING_NIGHT_TROLL_TAIL_THIN_2},
	{"NIGHT_TROLL_TAIL_THIN_3",PCG_LAYERING_NIGHT_TROLL_TAIL_THIN_3},
	{"NIGHT_TROLL_TAIL_THICK_1",PCG_LAYERING_NIGHT_TROLL_TAIL_THICK_1},
	{"NIGHT_TROLL_TAIL_THICK_2",PCG_LAYERING_NIGHT_TROLL_TAIL_THICK_2},
	{"NIGHT_TROLL_TAIL_THICK_3",PCG_LAYERING_NIGHT_TROLL_TAIL_THICK_3},
	{"NIGHT_TROLL_WINGS_FEATHERED",PCG_LAYERING_NIGHT_TROLL_WINGS_FEATHERED},
	{"NIGHT_TROLL_WINGS_SKIN",PCG_LAYERING_NIGHT_TROLL_WINGS_SKIN},
	{"NIGHT_TROLL_HORN_SHORT_1",PCG_LAYERING_NIGHT_TROLL_HORN_SHORT_1},
	{"NIGHT_TROLL_HORN_SHORT_2",PCG_LAYERING_NIGHT_TROLL_HORN_SHORT_2},
	{"NIGHT_TROLL_HORN_SHORT_3",PCG_LAYERING_NIGHT_TROLL_HORN_SHORT_3},
	{"NIGHT_TROLL_HORN_SHORT_4",PCG_LAYERING_NIGHT_TROLL_HORN_SHORT_4},
	{"NIGHT_TROLL_HORN_SPIRAL_1",PCG_LAYERING_NIGHT_TROLL_HORN_SPIRAL_1},
	{"NIGHT_TROLL_HORN_SPIRAL_2",PCG_LAYERING_NIGHT_TROLL_HORN_SPIRAL_2},
	{"NIGHT_TROLL_HORN_SPIRAL_3",PCG_LAYERING_NIGHT_TROLL_HORN_SPIRAL_3},
	{"NIGHT_TROLL_HORN_SPIRAL_4",PCG_LAYERING_NIGHT_TROLL_HORN_SPIRAL_4},
	{"NIGHT_TROLL_HORN_LONG_1",PCG_LAYERING_NIGHT_TROLL_HORN_LONG_1},
	{"NIGHT_TROLL_HORN_LONG_2",PCG_LAYERING_NIGHT_TROLL_HORN_LONG_2},
	{"NIGHT_TROLL_HORN_LONG_3",PCG_LAYERING_NIGHT_TROLL_HORN_LONG_3},
	{"NIGHT_TROLL_HORN_LONG_4",PCG_LAYERING_NIGHT_TROLL_HORN_LONG_4},
	{"NIGHT_TROLL_HORN_CURVED_1",PCG_LAYERING_NIGHT_TROLL_HORN_CURVED_1},
	{"NIGHT_TROLL_HORN_CURVED_2",PCG_LAYERING_NIGHT_TROLL_HORN_CURVED_2},
	{"NIGHT_TROLL_HORN_CURVED_3",PCG_LAYERING_NIGHT_TROLL_HORN_CURVED_3},
	{"NIGHT_TROLL_HORN_CURVED_4",PCG_LAYERING_NIGHT_TROLL_HORN_CURVED_4},
	{"NIGHT_TROLL_TRUNK_LONG",PCG_LAYERING_NIGHT_TROLL_TRUNK_LONG},
	{"NIGHT_TROLL_TRUNK_SHORT",PCG_LAYERING_NIGHT_TROLL_TRUNK_SHORT},
	*/

	{"WEREBEAST_ANTEATER",PCG_LAYERING_WEREBEAST_ANTEATER},
	{"WEREBEAST_ANTELOPE",PCG_LAYERING_WEREBEAST_ANTELOPE},
	{"WEREBEAST_APE",PCG_LAYERING_WEREBEAST_APE},
	{"WEREBEAST_ARMADILLO",PCG_LAYERING_WEREBEAST_ARMADILLO},
	{"WEREBEAST_BADGER",PCG_LAYERING_WEREBEAST_BADGER},
	{"WEREBEAST_BEAR",PCG_LAYERING_WEREBEAST_BEAR},
	{"WEREBEAST_BEAVER",PCG_LAYERING_WEREBEAST_BEAVER},
	{"WEREBEAST_BISON",PCG_LAYERING_WEREBEAST_BISON},
	{"WEREBEAST_BUFFALO",PCG_LAYERING_WEREBEAST_BUFFALO},
	{"WEREBEAST_BULL",PCG_LAYERING_WEREBEAST_BULL},
	{"WEREBEAST_CAPYBARA",PCG_LAYERING_WEREBEAST_CAPYBARA},
	{"WEREBEAST_CAMEL",PCG_LAYERING_WEREBEAST_CAMEL},
	{"WEREBEAST_CAT",PCG_LAYERING_WEREBEAST_CAT},
	{"WEREBEAST_CAVY",PCG_LAYERING_WEREBEAST_CAVY},
	{"WEREBEAST_CHAMELEON",PCG_LAYERING_WEREBEAST_CHAMELEON},
	{"WEREBEAST_CHINCHILLA",PCG_LAYERING_WEREBEAST_CHINCHILLA},
	{"WEREBEAST_CIVET",PCG_LAYERING_WEREBEAST_CIVET},
	{"WEREBEAST_COATI",PCG_LAYERING_WEREBEAST_COATI},
	{"WEREBEAST_COYOTE",PCG_LAYERING_WEREBEAST_COYOTE},
	{"WEREBEAST_DEER",PCG_LAYERING_WEREBEAST_DEER},
	{"WEREBEAST_DONKEY",PCG_LAYERING_WEREBEAST_DONKEY},
	{"WEREBEAST_ELEPHANT",PCG_LAYERING_WEREBEAST_ELEPHANT},
	{"WEREBEAST_ELK",PCG_LAYERING_WEREBEAST_ELK},
	{"WEREBEAST_FOX",PCG_LAYERING_WEREBEAST_FOX},
	{"WEREBEAST_GECKO",PCG_LAYERING_WEREBEAST_GECKO},
	{"WEREBEAST_GILA_MONSTER",PCG_LAYERING_WEREBEAST_GILA_MONSTER},
	{"WEREBEAST_GIRAFFE",PCG_LAYERING_WEREBEAST_GIRAFFE},
	{"WEREBEAST_GOAT",PCG_LAYERING_WEREBEAST_GOAT},
	{"WEREBEAST_GOPHER",PCG_LAYERING_WEREBEAST_GOPHER},
	{"WEREBEAST_HARE",PCG_LAYERING_WEREBEAST_HARE},
	{"WEREBEAST_HEDGEHOG",PCG_LAYERING_WEREBEAST_HEDGEHOG},
	{"WEREBEAST_HORSE",PCG_LAYERING_WEREBEAST_HORSE},
	{"WEREBEAST_HYENA",PCG_LAYERING_WEREBEAST_HYENA},
	{"WEREBEAST_IGUANA",PCG_LAYERING_WEREBEAST_IGUANA},
	{"WEREBEAST_JACKAL",PCG_LAYERING_WEREBEAST_JACKAL},
	{"WEREBEAST_KANGAROO",PCG_LAYERING_WEREBEAST_KANGAROO},
	{"WEREBEAST_KOALA",PCG_LAYERING_WEREBEAST_KOALA},
	{"WEREBEAST_LEMUR",PCG_LAYERING_WEREBEAST_LEMUR},
	{"WEREBEAST_LIZARD",PCG_LAYERING_WEREBEAST_LIZARD},
	{"WEREBEAST_LLAMA",PCG_LAYERING_WEREBEAST_LLAMA},
	{"WEREBEAST_LORIS",PCG_LAYERING_WEREBEAST_LORIS},
	{"WEREBEAST_MAMMOTH",PCG_LAYERING_WEREBEAST_MAMMOTH},
	{"WEREBEAST_MARMOT",PCG_LAYERING_WEREBEAST_MARMOT},
	{"WEREBEAST_MOLE",PCG_LAYERING_WEREBEAST_MOLE},
	{"WEREBEAST_MONGOOSE",PCG_LAYERING_WEREBEAST_MONGOOSE},
	{"WEREBEAST_MONITOR_LIZARD",PCG_LAYERING_WEREBEAST_MONITOR_LIZARD},
	{"WEREBEAST_MOOSE",PCG_LAYERING_WEREBEAST_MOOSE},
	{"WEREBEAST_MONKEY",PCG_LAYERING_WEREBEAST_MONKEY},
	{"WEREBEAST_MOUSE",PCG_LAYERING_WEREBEAST_MOUSE},
	{"WEREBEAST_OPOSSUM",PCG_LAYERING_WEREBEAST_OPOSSUM},
	{"WEREBEAST_PANDA",PCG_LAYERING_WEREBEAST_PANDA},
	{"WEREBEAST_PANGOLIN",PCG_LAYERING_WEREBEAST_PANGOLIN},
	{"WEREBEAST_PANTHER",PCG_LAYERING_WEREBEAST_PANTHER},
	{"WEREBEAST_PIG",PCG_LAYERING_WEREBEAST_PIG},
	{"WEREBEAST_PORCUPINE",PCG_LAYERING_WEREBEAST_PORCUPINE},
	{"WEREBEAST_RABBIT",PCG_LAYERING_WEREBEAST_RABBIT},
	{"WEREBEAST_RACCOON",PCG_LAYERING_WEREBEAST_RACCOON},
	{"WEREBEAST_RAT",PCG_LAYERING_WEREBEAST_RAT},
	{"WEREBEAST_RHINOCEROS",PCG_LAYERING_WEREBEAST_RHINOCEROS},
	{"WEREBEAST_SHEEP",PCG_LAYERING_WEREBEAST_SHEEP},
	{"WEREBEAST_SHREW",PCG_LAYERING_WEREBEAST_SHREW},
	{"WEREBEAST_SKINK",PCG_LAYERING_WEREBEAST_SKINK},
	{"WEREBEAST_SKUNK",PCG_LAYERING_WEREBEAST_SKUNK},
	{"WEREBEAST_SLOTH",PCG_LAYERING_WEREBEAST_SLOTH},
	{"WEREBEAST_SQUIRREL",PCG_LAYERING_WEREBEAST_SQUIRREL},
	{"WEREBEAST_TAPIR",PCG_LAYERING_WEREBEAST_TAPIR},
	{"WEREBEAST_TORTOISE",PCG_LAYERING_WEREBEAST_TORTOISE},
	{"WEREBEAST_WARTHOG",PCG_LAYERING_WEREBEAST_WARTHOG},
	{"WEREBEAST_WEASEL",PCG_LAYERING_WEREBEAST_WEASEL},
	{"WEREBEAST_WOLF",PCG_LAYERING_WEREBEAST_WOLF},
	{"WEREBEAST_WOMBAT",PCG_LAYERING_WEREBEAST_WOMBAT},
	{"WEREBEAST_ZEBRA",PCG_LAYERING_WEREBEAST_ZEBRA},

	{"EXPERIMENT_HUMANOID_SHADOW",PCG_LAYERING_EXPERIMENT_HUMANOID_SHADOW},
	{"EXPERIMENT_HUMANOID_HEAD_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_HEAD_SKIN},
	{"EXPERIMENT_HUMANOID_HEAD_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_HEAD_SCALES},
	{"EXPERIMENT_HUMANOID_HEAD_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_HEAD_FEATHERS},
	{"EXPERIMENT_HUMANOID_HEAD_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_HEAD_FUR},
	{"EXPERIMENT_HUMANOID_MOUTH",PCG_LAYERING_EXPERIMENT_HUMANOID_MOUTH},
	{"EXPERIMENT_HUMANOID_MANDIBLES",PCG_LAYERING_EXPERIMENT_HUMANOID_MANDIBLES},
	{"EXPERIMENT_HUMANOID_TRUNK",PCG_LAYERING_EXPERIMENT_HUMANOID_TRUNK},
	{"EXPERIMENT_HUMANOID_ANTENNAE",PCG_LAYERING_EXPERIMENT_HUMANOID_ANTENNAE},
	{"EXPERIMENT_HUMANOID_EYE_ONE",PCG_LAYERING_EXPERIMENT_HUMANOID_EYE_ONE},
	{"EXPERIMENT_HUMANOID_EYE_TWO",PCG_LAYERING_EXPERIMENT_HUMANOID_EYE_TWO},
	{"EXPERIMENT_HUMANOID_EYE_THREE",PCG_LAYERING_EXPERIMENT_HUMANOID_EYE_THREE},
	{"EXPERIMENT_HUMANOID_TORSO_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_TORSO_SKINLESS},
	{"EXPERIMENT_HUMANOID_TORSO_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_TORSO_SKIN},
	{"EXPERIMENT_HUMANOID_TORSO_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_TORSO_SCALES},
	{"EXPERIMENT_HUMANOID_TORSO_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_TORSO_FEATHERS},
	{"EXPERIMENT_HUMANOID_TORSO_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_TORSO_FUR},
	{"EXPERIMENT_HUMANOID_EXTERNAL_RIBS",PCG_LAYERING_EXPERIMENT_HUMANOID_EXTERNAL_RIBS},
	{"EXPERIMENT_HUMANOID_FOOT_LEFT",PCG_LAYERING_EXPERIMENT_HUMANOID_FOOT_LEFT},
	{"EXPERIMENT_HUMANOID_FOOT_RIGHT",PCG_LAYERING_EXPERIMENT_HUMANOID_FOOT_RIGHT},
	{"EXPERIMENT_HUMANOID_HAND_LEFT",PCG_LAYERING_EXPERIMENT_HUMANOID_HAND_LEFT},
	{"EXPERIMENT_HUMANOID_HAND_RIGHT",PCG_LAYERING_EXPERIMENT_HUMANOID_HAND_RIGHT},
	{"EXPERIMENT_HUMANOID_LEG_LEFT",PCG_LAYERING_EXPERIMENT_HUMANOID_LEG_LEFT},
	{"EXPERIMENT_HUMANOID_LEG_RIGHT",PCG_LAYERING_EXPERIMENT_HUMANOID_LEG_RIGHT},
	{"EXPERIMENT_HUMANOID_ARM_LEFT",PCG_LAYERING_EXPERIMENT_HUMANOID_ARM_LEFT},
	{"EXPERIMENT_HUMANOID_ARM_RIGHT",PCG_LAYERING_EXPERIMENT_HUMANOID_ARM_RIGHT},
	{"EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_1},
	{"EXPERIMENT_HUMANOID_HORN_LONG_CURVING_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_CURVING_1},
	{"EXPERIMENT_HUMANOID_HORN_SHORT_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_SHORT_1},
	{"EXPERIMENT_HUMANOID_HORN_STUBBY_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_STUBBY_1},
	{"EXPERIMENT_HUMANOID_HORN_BROAD_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_BROAD_1},
	{"EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_1",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_1},
	{"EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_2},
	{"EXPERIMENT_HUMANOID_HORN_LONG_CURVING_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_CURVING_2},
	{"EXPERIMENT_HUMANOID_HORN_SHORT_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_SHORT_2},
	{"EXPERIMENT_HUMANOID_HORN_STUBBY_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_STUBBY_2},
	{"EXPERIMENT_HUMANOID_HORN_BROAD_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_BROAD_2},
	{"EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_2",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_2},
	{"EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_3},
	{"EXPERIMENT_HUMANOID_HORN_LONG_CURVING_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_CURVING_3},
	{"EXPERIMENT_HUMANOID_HORN_SHORT_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_SHORT_3},
	{"EXPERIMENT_HUMANOID_HORN_STUBBY_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_STUBBY_3},
	{"EXPERIMENT_HUMANOID_HORN_BROAD_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_BROAD_3},
	{"EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_3",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_3},
	{"EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_4},
	{"EXPERIMENT_HUMANOID_HORN_LONG_CURVING_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_CURVING_4},
	{"EXPERIMENT_HUMANOID_HORN_SHORT_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_SHORT_4},
	{"EXPERIMENT_HUMANOID_HORN_STUBBY_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_STUBBY_4},
	{"EXPERIMENT_HUMANOID_HORN_BROAD_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_BROAD_4},
	{"EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_4",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_4},
	{"EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_SPIRAL_5},
	{"EXPERIMENT_HUMANOID_HORN_LONG_CURVING_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_CURVING_5},
	{"EXPERIMENT_HUMANOID_HORN_SHORT_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_SHORT_5},
	{"EXPERIMENT_HUMANOID_HORN_STUBBY_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_STUBBY_5},
	{"EXPERIMENT_HUMANOID_HORN_BROAD_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_BROAD_5},
	{"EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_5",PCG_LAYERING_EXPERIMENT_HUMANOID_HORN_LONG_STRAIGHT_5},
	{"EXPERIMENT_HUMANOID_WINGS_LACY",PCG_LAYERING_EXPERIMENT_HUMANOID_WINGS_LACY},
	{"EXPERIMENT_HUMANOID_WINGS_FEATHERED",PCG_LAYERING_EXPERIMENT_HUMANOID_WINGS_FEATHERED},
	{"EXPERIMENT_HUMANOID_WINGS_BAT",PCG_LAYERING_EXPERIMENT_HUMANOID_WINGS_BAT},

	{"EXPERIMENT_HUMANOID_PORTRAIT_TORSO_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TORSO_FUR},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TORSO_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TORSO_FEATHERS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SCALES},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SKIN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TORSO_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_EXTERNAL_RIBS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_EXTERNAL_RIBS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_FUR},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_FEATHERS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SCALES},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SKIN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_LEFT_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_FUR},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_FEATHERS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SCALES},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SKIN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ARM_RIGHT_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_FUR",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_FUR},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_FEATHERS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_FEATHERS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SCALES",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SCALES},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SKIN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SKIN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MOUTH",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MOUTH},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MOUTH_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MOUTH_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MANDIBLES",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MANDIBLES},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MANDIBLES_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_MANDIBLES_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_ONE",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_ONE},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_ONE_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_ONE_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_TWO",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_TWO},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_TWO_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_TWO_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_THREE",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_THREE},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_THREE_SKINLESS",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HEAD_EYE_THREE_SKINLESS},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_LONG",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_LONG},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_SHORT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_SHORT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_FAT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_FAT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_TWISTING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_TWISTING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_CURLING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_CURLING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_KNOBBY",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_TRUNK_KNOBBY},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_LONG",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_LONG},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_FAN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_FAN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_SPINDLY",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_SPINDLY},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_SQUAT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_SQUAT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_BRANCHING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_BRANCHING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_KNOBBING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_LEFT_KNOBBING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_LONG",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_LONG},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_FAN",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_FAN},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_SPINDLY",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_SPINDLY},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_SQUAT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_SQUAT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_BRANCHING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_BRANCHING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_KNOBBING",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_ANTENNA_RIGHT_KNOBBING},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_LACY",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_LACY},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_FEATHERED",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_FEATHERED},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_BAT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_LEFT_BAT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_LACY",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_LACY},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_FEATHERED",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_FEATHERED},
	{"EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_BAT",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_WING_RIGHT_BAT},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_1",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_1},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_2",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_2},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_3",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_3},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_4",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_4},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_SPIRAL_5},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_CURVING_5},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_SHORT_5},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_STUBBY_5},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_BROAD_5},
	{"EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_5",PCG_LAYERING_EXPERIMENT_HUMANOID_PORTRAIT_HORN_LONG_STRAIGHT_5},
	};

	const unordered_bimap<string,PCGLayeringModifier> PCG_LAYERING_MODIFIERSTRINGS={
	{"HORN_COUNT_1",PCG_LAYERING_MODIFIER_HORN_COUNT_1},
	{"HORN_COUNT_2",PCG_LAYERING_MODIFIER_HORN_COUNT_2},
	{"HORN_COUNT_3",PCG_LAYERING_MODIFIER_HORN_COUNT_3},
	{"HORN_COUNT_4",PCG_LAYERING_MODIFIER_HORN_COUNT_4},

	{"HORN_LONG_SPIRAL",PCG_LAYERING_MODIFIER_HORN_LONG_SPIRAL},
	{"HORN_LONG_CURVING",PCG_LAYERING_MODIFIER_HORN_LONG_CURVING},
	{"HORN_SHORT",PCG_LAYERING_MODIFIER_HORN_SHORT},
	{"HORN_STUBBY",PCG_LAYERING_MODIFIER_HORN_STUBBY},
	{"HORN_BROAD",PCG_LAYERING_MODIFIER_HORN_BROAD},
	{"HORN_LONG_STRAIGHT",PCG_LAYERING_MODIFIER_HORN_LONG_STRAIGHT},

	{"ANTENNA_LONG",PCG_LAYERING_MODIFIER_ANTENNA_LONG},
	{"ANTENNA_FAN",PCG_LAYERING_MODIFIER_ANTENNA_FAN},
	{"ANTENNA_SPINDLY",PCG_LAYERING_MODIFIER_ANTENNA_SPINDLY},
	{"ANTENNA_SQUAT",PCG_LAYERING_MODIFIER_ANTENNA_SQUAT},
	{"ANTENNA_BRANCHING",PCG_LAYERING_MODIFIER_ANTENNA_BRANCHING},
	{"ANTENNA_KNOBBING",PCG_LAYERING_MODIFIER_ANTENNA_KNOBBING},

	{"TRUNK_LONG",PCG_LAYERING_MODIFIER_TRUNK_LONG},
	{"TRUNK_SHORT",PCG_LAYERING_MODIFIER_TRUNK_SHORT},
	{"TRUNK_FAT",PCG_LAYERING_MODIFIER_TRUNK_FAT},
	{"TRUNK_TWISTING",PCG_LAYERING_MODIFIER_TRUNK_TWISTING},
	{"TRUNK_CURLING",PCG_LAYERING_MODIFIER_TRUNK_CURLING},
	{"TRUNK_KNOBBY",PCG_LAYERING_MODIFIER_TRUNK_KNOBBY},

	{"SURFACE_FUR",PCG_LAYERING_MODIFIER_SURFACE_FUR},
	{"SURFACE_FEATHERS",PCG_LAYERING_MODIFIER_SURFACE_FEATHERS},
	{"SURFACE_SCALES",PCG_LAYERING_MODIFIER_SURFACE_SCALES},
	{"SURFACE_SKIN",PCG_LAYERING_MODIFIER_SURFACE_SKIN},
	{"SURFACE_SKINLESS",PCG_LAYERING_MODIFIER_SURFACE_SKINLESS},

	{"EXTERNAL_RIBS",PCG_LAYERING_MODIFIER_EXTERNAL_RIBS},
};