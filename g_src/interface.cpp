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

#include "svector.h"
using std::string;

#include "endian.h"

#include "files.h"

#include "enabler.h"

#include "textlines.h"

#include "find_files.h"

#include "basics.h"

#include "g_basics.h"

#include "music_and_sound_g.h"

#include "graphics.h"

#include "init.h"

#include "keybindings.h"
#include "interface.h"
#include "KeybindingScreen.h"
//#include "ttf_manager.hpp"

#include <list>
#include <set>

void dwarf_end_announcements();
void dwarf_remove_screen();
void drawborder(const char *str,char style,const char *colorstr);


inline void CHECK_ERR(int err, const char* msg)
{
	if (err != Z_OK)
		{
		MessageBox(NULL, "One of the compressed files on disk has errors in it.  Restore from backup if you are able.", 0, 0);
		exit(1);
		}
}

using std::fstream;
using std::ios;
using std::list;
using std::set;

extern interfacest gview;
extern enablerst enabler;
extern graphicst gps;
extern initst init;
#ifndef NO_FMOD
extern musicsoundst musicsound;
#endif

extern GameMode gamemode;
extern GameType gametype;

extern int32_t movie_version;

#ifdef CURSES_MOVIES
void interfacest::finish_movie()
{
	supermovie_on=0;
	currentblocksize=0;
	nextfilepos=0;
	supermovie_pos=0;
	viewscreen_movieplayerst::create(INTERFACE_PUSH_AT_BACK);
}

void interfacest::use_movie_input()
{
	if(supermovie_on)handlemovie(1);
	finish_movie();
}

viewscreen_movieplayerst *viewscreen_movieplayerst::create(char pushtype,viewscreenst *scr)
{
	viewscreen_movieplayerst *newv=new viewscreen_movieplayerst();
	gview.addscreen(newv,pushtype,scr);

	return newv;
}

void viewscreen_movieplayerst::force_play(const string &file)
{
	force_file=file;
	is_forced_play=1;
}

void viewscreen_movieplayerst::logic()
{
	enabler.flag&=~ENABLERFLAG_MAXFPS;

	enabler.flag|=ENABLERFLAG_RENDER;

	if(!force_file.empty()&&!is_playing&&!quit_if_no_play&&is_forced_play)
		{
		is_playing=1;
		quit_if_no_play=1;
		gview.movie_file=force_file;
		gview.supermovie_on=0;
		gview.currentblocksize=0;
		gview.nextfilepos=0;
		gview.supermovie_pos=0;
		maxmoviepos=0;
		}

	if(!is_playing&&quit_if_no_play)
		{
		breakdownlevel=INTERFACE_BREAKDOWN_STOPSCREEN;
		return;
		}

	//LOAD A MOVIE BUFFER BY BUFFER
	if(is_playing)
		{
		if(gview.supermovie_pos>=MOVIEBUFFSIZE||gview.currentblocksize==0)
			{
			gview.read_movie_chunk(maxmoviepos,is_playing);
			}

		if(is_playing)
			{
			int half_frame_size=init.display.grid_x*init.display.grid_y;

#ifndef NO_FMOD
			//PLAY ANY RELEVANT SOUNDS
			if(gview.supermovie_delaystep==gview.supermovie_delayrate)
				{
				int fr=gview.supermovie_pos/(half_frame_size*2);
				if(fr>=0&&fr<200)
					{
					int c,sd;
					for(c=0;c<16;c++)
						{
						sd=gview.supermovie_sound_time[fr][c];
						if(sd>=0&&sd<gview.supermovie_sound.str.size())
							{
							musicsound.playsound(sd);
							}
						}
					}
				}
#endif

			//PRINT THE NEXT FRAME AND ADVANCE POSITION
			short x2,y2;
			for(x2=0;x2<init.display.grid_x;x2++)
				{
				for(y2=0;y2<init.display.grid_y;y2++)
					{
					gview.supermovie_pos++;
					}
				}
			if(gview.supermovie_delaystep==0)
				{
				gview.supermovie_pos+=half_frame_size;
				gview.supermovie_delaystep=gview.supermovie_delayrate;
				}
			else
				{
				gview.supermovie_pos-=half_frame_size;//RETURN TO LAST FRAME
				gview.supermovie_delaystep--;
				}

			//DONE
			if(gview.supermovie_pos>=maxmoviepos&&
				maxmoviepos+half_frame_size*2<MOVIEBUFFSIZE)
				{
				is_playing=0;
				//NOTE: THIS CAUSES IT TO LOSE THE LAST FRAME DUE TO CLEARING
				}
			}
		}
}

void viewscreen_movieplayerst::render(uint32_t curtick)
{
	if(breakdownlevel!=INTERFACE_BREAKDOWN_NONE)return;
	
	if(!is_playing&&is_forced_play)return;

	if(!quit_if_no_play)
		{
		if(editing)drawborder(NULL);
		else drawborder("  Moving Records  ");
		}

	//LOAD A MOVIE BUFFER BY BUFFER
	if(is_playing)
		{
		if(gview.currentblocksize>0)
			{
			int32_t half_frame_size=init.display.grid_x*init.display.grid_y;

			//PRINT THE NEXT FRAME AND ADVANCE POSITION
			drawborder(NULL,-1);
		
			int32_t curp=gview.supermovie_pos;
				//HANG ON THE LAST FRAME TO AVOID POSSIBLE OVERRUNS
			if(gview.supermovie_pos>=MOVIEBUFFSIZE-half_frame_size*2)
				{
				curp=MOVIEBUFFSIZE-half_frame_size*4;
				}
			short x2,y2;
			for(x2=0;x2<init.display.grid_x;x2++)
				{
				for(y2=0;y2<init.display.grid_y;y2++,++curp)
					{
					gps.locate(y2,x2);

					gps.changecolor((gview.supermoviebuffer[curp+half_frame_size] & 7),
						(gview.supermoviebuffer[curp+half_frame_size] & 56)>>3,
						(gview.supermoviebuffer[curp+half_frame_size] & 64));

					gps.addchar(gview.supermoviebuffer[curp]);
					}
				}
			}
		}
	else if(loading)
		{
		int scroll=selfile/21*21;
		int l;
		for(l=scroll;l<filelist.size() && l<scroll+21;l++)
			{
			if(l==selfile)gps.changecolor(7,0,1);
			else gps.changecolor(7,0,0);
			gps.locate(l-scroll+2,2);
			gps.addst(filelist[l]);
			}
		}
#ifdef DEBUG_MOVIE_EDIT
	else if(editing)
		{
		if(editing_menu)
			{
			int tx,ty;
			unsigned char c=0;
			for(ty=0;ty<16;ty++)
				{
				for(tx=0;tx<16;tx++)
					{
					gps.locate(ty,tx);
					gps.changecolor(editing_screenf,editing_screenb,editing_screenbright);
					gps.addchar(c);
					c++;
					}
				}
			gps.locate(18,0);
			gps.changecolor(editing_screenf,editing_screenb,editing_screenbright);
			gps.addchar(editing_char);
			for(ty=0;ty<16;ty++)
				{
				for(tx=0;tx<8;tx++)
					{
					gps.locate(ty,tx+16);
					gps.changecolor(ty%8,tx,ty/8);
					gps.addchar('A');
					}
				}

			gps.changecolor(7,0,1);
			gps.locate(20,0);
			string str;
			gps.addst("1/100 sec per frame: ");convert_long_to_string(gview.supermovie_delayrate,str);
			gps.addst(str);

			int scroll=(editing_selected_sound/25)*25;
			int e;
			for(e=scroll;e<scroll+25&&e<gview.supermovie_sound.str.size();e++)
				{
				if(e==editing_selected_sound)gps.changecolor(7,0,1);
				else gps.changecolor(7,0,0);
				gps.locate(e-scroll,26);
				gps.addst(gview.supermovie_sound.str[e]->dat);
				}

			int frame=gview.supermovie_pos/4000,sd;
			for(e=0;e<SOUND_CHANNELNUM;e++)
				{
				gps.changecolor(2,0,1);
				gps.locate(e-scroll,52);
				sd=gview.supermovie_sound_time[frame][e];
				if(sd>=0&&sd<gview.supermovie_sound.str.size())gps.addst(gview.supermovie_sound.str[sd]->dat);
				else
					{
					gps.addst("-----------------");
					}
				gps.changecolor(4,0,1);
				gps.locate(e-scroll,(init.display.grid_x-1));
				gps.addst("X");
				}
			}
		else
			{
			drawborder(NULL,-1);

			int curp=gview.supermovie_pos;
			int x2,y2;
			for(x2=0;x2<80;x2++)
				{
				for(y2=0;y2<25;y2++)
					{
					gps.locate(y2,x2);

					gps.changecolor((gview.supermoviebuffer[curp+2000] & 7),
						(gview.supermoviebuffer[curp+2000] & 56)>>3,
						(gview.supermoviebuffer[curp+2000] & 64));

					gps.addchar(gview.supermoviebuffer[curp]);

					curp++;
					}
				}

			if(enabler.mouse_y<150)gps.locate(24,0);
			else gps.locate(0,0);
			gps.changecolor(2,0,1);
			gps.addst("Frame: ");
			string num;
			convert_long_to_string(gview.supermovie_pos/4000+1,num);
			gps.addst(num);

			if(enabler.mouse_y<150)gps.locate(24,20);
			else gps.locate(0,20);
			gps.changecolor(3,0,1);
			gps.addst("Copy From: ");
			convert_long_to_string(editing_copy_from/4000+1,num);
			gps.addst(num);

			if(enabler.mouse_y<150)gps.locate(24,40);
			else gps.locate(0,40);
			gps.changecolor(4,0,1);
			gps.addst("Ends At: ");
			convert_long_to_string(end_frame_pos/4000+1,num);
			gps.addst(num);

			if(enabler.mouse_y<150)gps.locate(24,60);
			else gps.locate(0,60);
			int sx,sy;
			gps.get_mouse_text_coords(sx,sy);
			gps.changecolor(7,0,1);
			gps.addst("(");
			convert_long_to_string(sx,num);
			gps.addst(num);
			gps.addst(",");
			convert_long_to_string(sy,num);
			gps.addst(num);
			gps.addst(")");
			}
		}
#endif
	else
		{
		gps.changecolor(7,0,1);
		gps.locate(2,2);
		gview.print_interface_token(INTERFACEKEY_MOVIE_RECORD);
		gps.addst(": Start recording (active record is erased, stops when you return here)");
		gps.locate(3,2);
		gview.print_interface_token(INTERFACEKEY_MOVIE_PLAY);
		gps.addst(": Play the active moving record");
		gps.locate(4,2);
		gview.print_interface_token(INTERFACEKEY_MOVIE_SAVE);
		gps.addst(": Save the active moving record (you will be prompted for a name)");
		gps.locate(5,2);
		gview.print_interface_token(INTERFACEKEY_MOVIE_LOAD);
		gps.addst(": Load a moving record");

#ifdef DEBUG_MOVIE_EDIT
		gps.locate(7,2);
		gps.addst("E: Edit");
#endif

		if(saving)
			{
			gps.locate(10,2);
			gps.addst("Name: ");
			gps.addst(savename);
			}
		}
}

void viewscreen_movieplayerst::feed(std::set<InterfaceKey> &events)
{
	if(events.count(INTERFACEKEY_LEAVESCREEN))
		{
		events.clear();

		if(is_playing)
			{
			is_playing=0;
                        enabler.release_grid_size();
                        if (gview.original_fps)
                          enabler.set_fps(gview.original_fps);
			gview.supermovie_on=0;
			gview.currentblocksize=0;
			gview.nextfilepos=0;
			gview.supermovie_pos=0;
			maxmoviepos=0;

#ifndef NO_FMOD
			musicsound.stop_sound();
#endif
			}
		else if(saving)saving=0;
		else if(loading)loading=0;
#ifdef DEBUG_MOVIE_EDIT
		else if(editing)editing=0;
#endif
		else
			{
			is_playing=0;
                        enabler.release_grid_size();
                        if (gview.original_fps)
                          enabler.set_fps(gview.original_fps);
			gview.supermovie_on=0;
			gview.currentblocksize=0;
			gview.nextfilepos=0;
			gview.supermovie_pos=0;
			maxmoviepos=0;

			breakdownlevel=INTERFACE_BREAKDOWN_STOPSCREEN;
			return;
			}
		}
	else if(saving)
		{
		standardstringentry(savename,39,STRINGENTRY_LETTERS|STRINGENTRY_SPACE|STRINGENTRY_NUMBERS|STRINGENTRY_SYMBOLS,events);

		if(events.count(INTERFACEKEY_SELECT))
			{
			string filename;
			filename="data/movies/";
			filename+=savename;
			filename+=".cmv";

			copy_file(gview.movie_file,filename);
			saving=0;
			}
		}
	else if(loading)
		{
		if(events.count(INTERFACEKEY_SELECT))
			{
			string filename;
			filename="data/movies/";
			filename+=filelist[selfile];

			if(filename!=gview.movie_file)
				{
				copy_file(filename,gview.movie_file);
				}
			loading=0;
			}

		standardscrolling(events,selfile,0,filelist.size()-1,21);
		}
#ifdef DEBUG_MOVIE_EDIT
	else if(editing)
		{
		char entering=0;

		if(editing_menu)
			{
			if(enabler.mouse_lbut&&enabler.tracking_on)
				{
				int sx,sy;
				gps.get_mouse_text_coords(sx,sy);

				if(sx>=0&&sx<80&&sy>=0&&sy<25)
					{
					if(sx>=0&&sx<16&&sy>=0&&sy<16)
						{
						editing_char=sx+sy*16;
						}
					if(sx>=16&&sx<24&&sy>=0&&sy<16)
						{
						editing_screenf=sy%8;
						editing_screenb=sx-16;
						editing_screenbright=sy/8;
						}
					if(sx>=26&&sx<=51&&sy>=0&&sy<SOUND_CHANNELNUM)
						{
						editing_selected_sound=sy;
						}
					int frame=gview.supermovie_pos/4000;
					if(sx>=52&&sx<=78&&sy>=0&&sy<SOUND_CHANNELNUM)
						{
						gview.supermovie_sound_time[frame][sy]=editing_selected_sound;
						}
					if(sx==(init.display.grid_x-1)&&sy>=0&&sy<SOUND_CHANNELNUM)
						{
						gview.supermovie_sound_time[frame][sy]=-1;
						}
					}

				enabler.mouse_lbut=0;
				}

			if(enabler.mouse_rbut&&enabler.tracking_on)
				{
				editing_menu=0;
				enabler.mouse_rbut=0;
				}

			if(editing_selected_sound>=0&&editing_selected_sound<gview.supermovie_sound.str.size())
				{
				if(gview.c== '%')
					{
					delete gview.supermovie_sound.str[editing_selected_sound];
					gview.supermovie_sound.str.erase(editing_selected_sound);
					}
				else
					{
					standardstringentry(events,gview.supermovie_sound.str[editing_selected_sound]->dat,26,STRINGENTRY_LETTERS|STRINGENTRY_SPACE|STRINGENTRY_NUMBERS|STRINGENTRY_SYMBOLS);
					entering=1;
					}
				}
			else
				{
				if(gview.c== '#')gview.supermovie_sound.add_string("new_sound");
				if(gview.c== '+')gview.supermovie_delayrate++;
				if(gview.c== '-')gview.supermovie_delayrate--;
				if(gview.c== 'T')text_mode=1-text_mode;
				}
			if(gview.supermovie_delayrate<0)gview.supermovie_delayrate=0;
			if(gview.supermovie_delayrate>10)gview.supermovie_delayrate=10;
			}
		else
			{
			if(text_mode)
				{
				if(gview.c!=0)
					{
					int sx,sy;
					gps.get_mouse_text_coords(sx,sy);

					if(sx>=0&&sx<80&&sy>=0&&sy<25)
						{
						int curpos=gview.supermovie_pos+sy+sx*25;
						gview.supermoviebuffer[curpos]=gview.c;
						gview.supermoviebuffer[curpos+2000]=(editing_screenf&7)+((editing_screenb&7)<<3);
						if(editing_screenbright)gview.supermoviebuffer[curpos+2000]+=64;
						}
					}
				}
			else
				{
				if(gview.c== 'a')
					{
					int x2,y2;
					for(x2=0;x2<80;x2++)
						{
						for(y2=0;y2<25;y2++)
							{
							if(x2>0)
								{
								gview.supermoviebuffer[gview.supermovie_pos+y2+(x2-1)*25]=gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25];
								gview.supermoviebuffer[gview.supermovie_pos+y2+(x2-1)*25+2000]=gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25+2000];
								}
							if(x2==(init.display.grid_x-1))gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25]=0;
							}
						}
					}
				if(gview.c== 'd')
					{
					int x2,y2;
					for(x2=(init.display.grid_x-1);x2>=0;x2--)
						{
						for(y2=0;y2<(init.display.grid_y-1);y2++)
							{
							if(x2<(init.display.grid_x-1))
								{
								gview.supermoviebuffer[gview.supermovie_pos+y2+(x2+1)*25]=gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25];
								gview.supermoviebuffer[gview.supermovie_pos+y2+(x2+1)*25+2000]=gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25+2000];
								}
							if(x2==0)gview.supermoviebuffer[gview.supermovie_pos+y2+x2*25]=0;
							}
						}
					}
				if(gview.c== 'E')end_frame_pos=gview.supermovie_pos;
				if(gview.c== 'c')editing_copy_from=gview.supermovie_pos;
				if(gview.c== 'p')
					{
					int i;
					for(i=0;i<4000;i++)
						{
						gview.supermoviebuffer[gview.supermovie_pos+i]=gview.supermoviebuffer[editing_copy_from+i];
						}
					}
				if(gview.c== '+')gview.supermovie_pos+=4000;
				if(gview.c== '-')gview.supermovie_pos-=4000;
				if(gview.c== '/')gview.supermovie_pos-=40000;
				if(gview.c== '*')gview.supermovie_pos+=40000;
				if(gview.supermovie_pos<0)gview.supermovie_pos=0;
				if(gview.supermovie_pos>=MOVIEBUFFSIZE)gview.supermovie_pos=MOVIEBUFFSIZE-4000;
				}

			if(enabler.mouse_lbut&&enabler.tracking_on)
				{
				int sx,sy;
				gps.get_mouse_text_coords(sx,sy);

				if(sx>=0&&sx<80&&sy>=0&&sy<25)
					{
					int curpos=gview.supermovie_pos+sy+sx*25;
					gview.supermoviebuffer[curpos]=editing_char;
					gview.supermoviebuffer[curpos+2000]=(editing_screenf&7)+((editing_screenb&7)<<3);
					if(editing_screenbright)gview.supermoviebuffer[curpos+2000]+=64;
					}
				}
			if(enabler.mouse_rbut&&enabler.tracking_on)
				{
				editing_menu=1;
				enabler.mouse_rbut=0;
				}
			}

		if(!entering&&gview.c== 'S')
			{
			int opos=gview.supermovie_pos;
			gview.first_movie_write=1;
			gview.supermovie_pos=end_frame_pos+4000;

			gview.write_movie_chunk();

			gview.supermovie_pos=opos;
			}
		}
#endif
	else
		{
		if(is_playing)
			{
			}
		else
			{
#ifdef DEBUG_MOVIE_EDIT
			if(gview.c== 'E')
				{
				editing=1;
				gview.supermovie_pos=0;
				}
#endif

			if(events.count(INTERFACEKEY_MOVIE_RECORD))
				{
				//TURN ON THE MOVIE RECORDER
				is_playing=0;
                                enabler.release_grid_size();
                                if (gview.original_fps)
                                  enabler.set_fps(gview.original_fps);
				gview.supermovie_on=1;
				gview.currentblocksize=0;
				gview.nextfilepos=0;
				gview.supermovie_pos=0;
				gview.supermovie_delayrate=0;
				gview.first_movie_write=1;
				maxmoviepos=0;

				breakdownlevel=INTERFACE_BREAKDOWN_STOPSCREEN;
				}
			if(events.count(INTERFACEKEY_MOVIE_PLAY))
				{
				is_playing=1;
				gview.supermovie_on=0;
				gview.currentblocksize=0;
				gview.nextfilepos=0;
				gview.supermovie_pos=0;
				maxmoviepos=0;
				}
			if(events.count(INTERFACEKEY_MOVIE_SAVE))
				{
				savename.erase();
				saving=1;
				}
			if(events.count(INTERFACEKEY_MOVIE_LOAD))
				{
				selfile=0;

				clearfilelist();

				find_files_by_pattern("data/movies/*.cmv",filelist);

				if(filelist.size()>0)loading=1;
				}
			}
		}
}

void viewscreen_movieplayerst::clearfilelist()
{
	int f;
	for(f=0;f<filelist.size();f++)delete[] filelist[f];
	filelist.clear();
}

viewscreen_movieplayerst::viewscreen_movieplayerst()
{
	force_file.erase();
	gview.movie_file="data/movies/last_record.cmv";
	is_playing=0;
        enabler.release_grid_size();
        if (gview.original_fps)
          enabler.set_fps(gview.original_fps);
	is_forced_play=0;
	quit_if_no_play=0;
	gview.supermovie_on=0;
	gview.currentblocksize=0;
	gview.nextfilepos=0;
	gview.supermovie_pos=0;
	maxmoviepos=0;
	saving=0;
	loading=0;
	editing=0;
	text_mode=0;
	editing_copy_from=0;
	editing_char=219;
	editing_screenf=7;
	editing_screenb=0;
	editing_screenbright=0;
	editing_menu=0;
	editing_selected_sound=0;
	end_frame_pos=0;
	gview.supermovie_sound.clean();
#ifndef NO_FMOD
	int i,c;
	for(i=0;i<200;i++)
		{
		for(c=0;c<SOUND_CHANNELNUM;c++)gview.supermovie_sound_time[i][c]=-1;
		}
#endif
}
#endif

interfacest::interfacest()
{
        original_fps = 0;
	shutdown_interface_for_ms=0;
	shutdown_interface_tickcount=0;
	flag=0;
	cur_textbox=NULL;
#ifdef CURSES_MOVIES
	supermovie_on=0;
	supermovie_pos=0;
	supermovie_delayrate=0;
#endif
}

interfacest::~interfacest()
{
	//GO AHEAD
	while(view.child!=NULL)
		{
		removescreen(view.child);
		}
}

void interfacest::addscreen(viewscreenst *scr,char pushtype,viewscreenst *relate)
{
	gps.force_full_display_count+=2;

	switch(pushtype)
		{
		case INTERFACE_PUSH_AS_PARENT:insertscreen_as_parent(scr,relate);break;
		case INTERFACE_PUSH_AS_CHILD:insertscreen_as_child(scr,relate);break;
		case INTERFACE_PUSH_AT_FRONT:insertscreen_at_front(scr);break;
		default:insertscreen_at_back(scr);break;
		}

	//WHENEVER A SCREEN IS ADDED, END ANNOUNCEMENTS
	if(gamemode==GAMEMODE_DWARF)dwarf_end_announcements();
}

void interfacest::insertscreen_as_parent(viewscreenst *scr,viewscreenst *child)
{
	if(child==NULL)
		{
		insertscreen_at_back(scr);
		return;
		}

	scr->child=child;
	scr->parent=child->parent;

	if(scr->parent!=NULL)scr->parent->child=scr;
	child->parent=scr;
}

void interfacest::insertscreen_as_child(viewscreenst *scr,viewscreenst *parent)
{
	if(parent==NULL)
		{
		insertscreen_at_back(scr);
		return;
		}

	scr->child=parent->child;
	scr->parent=parent;

	if(scr->child!=NULL)scr->child->parent=scr;
	parent->child=scr;
}

void interfacest::insertscreen_at_back(viewscreenst *scr)
{
	//GRAB CURRENT SCREEN AT THE END OF THE LIST
	viewscreenst *currentscreen=&view;
	while(currentscreen->child!=NULL)currentscreen=currentscreen->child;

	//PUT IT ON TO THE BACK SCREEN
	insertscreen_as_child(scr,currentscreen);
}

void interfacest::insertscreen_at_front(viewscreenst *scr)
{
	//PUT IT ON TO THE BASE
	insertscreen_as_child(scr,&view);
}

viewscreenst *interfacest::grab_lastscreen() {
  viewscreenst *currentscreen = &view;
  while (currentscreen->child) currentscreen = currentscreen->child;
  return currentscreen;
}

char interfacest::loop() {
  //NO INTERFACE LEFT, QUIT
  if(view.child==0)return 1;

  //GRAB CURRENT SCREEN AT THE END OF THE LIST
  viewscreenst *currentscreen = grab_lastscreen();
  //MOVE SCREENS BACK
  switch(currentscreen->breakdownlevel) {
  case INTERFACE_BREAKDOWN_NONE: {
    
    currentscreen->logic();
	currentscreen->widgets.logic();

#ifdef CURSES_MOVIES
	if(currentscreen->movies_okay())
		{
		//HANDLE MOVIES
		handlemovie(0);
		}
#endif

    const Time now = SDL_GetTicks();
    // Process as much input as possible. Some screens can't handle multiple input events
    // per logic call (retain_nonzero_input, and any alteration to the window setup
    // requires us to stop until the next logic call.
    for (;;) {
      if (currentscreen->child || currentscreen->breakdownlevel != INTERFACE_BREAKDOWN_NONE)
        break; // Some previous input or logic had the effect of switching screens

      if (flag & INTERFACEFLAG_RETAIN_NONZERO_INPUT) {
        flag&=~INTERFACEFLAG_RETAIN_NONZERO_INPUT;
        break;
      } else {
        set<InterfaceKey> era = enabler.get_input(now);
		if (cur_textbox != NULL)
			{
			if(!cur_textbox->is_visible() || !cur_textbox->do_input(era) && !enabler.listening_to_text)
				{
				cur_textbox=NULL;
				}
			}
        if (era.size() == 0) {
          if(enabler.mouse_lbut || enabler.mouse_rbut || enabler.mouse_mbut || enabler.get_text_input()[0] != '\0') currentscreen->feed(era);
          break;
        }
        
        //DO MOVIE COMMANDS
#ifdef CURSES_MOVIES
        if (era.count(INTERFACEKEY_MOVIES)&&!currentscreen->key conflict(INTERFACEKEY_MOVIES))
          if(currentscreen->movies_okay()) use_movie_input();
#endif
        // Prefix commands
        // Most prefix commands we don't want to touch game management commands,
        // i.e. what's in here. Macro playback is a notable exception.
        if (era.count(INTERFACEKEY_PREFIX))
          enabler.prefix_toggle();
        int repeats = 1;  // If this input ends a prefix command, we'll want to repeat it.
        if (enabler.prefix_building()) {
          // TODO: OMGWTFBBQ
          char c = 0;
          if (era.count(INTERFACEKEY_STRING_A048)) c = '0';
          else if (era.count(INTERFACEKEY_STRING_A049)) c = '1';
          else if (era.count(INTERFACEKEY_STRING_A050)) c = '2';
          else if (era.count(INTERFACEKEY_STRING_A051)) c = '3';
          else if (era.count(INTERFACEKEY_STRING_A052)) c = '4';
          else if (era.count(INTERFACEKEY_STRING_A053)) c = '5';
          else if (era.count(INTERFACEKEY_STRING_A054)) c = '6';
          else if (era.count(INTERFACEKEY_STRING_A055)) c = '7';
          else if (era.count(INTERFACEKEY_STRING_A056)) c = '8';
          else if (era.count(INTERFACEKEY_STRING_A057)) c = '9';

          if (c) {
            enabler.prefix_add_digit(c);
            era.clear();
          } else {
            repeats = enabler.prefix_end();
          }
        }
        // TTF toggle
		/*
        if (era.count(INTERFACEKEY_TOGGLE_TTF)) {
          if (init.font.use_ttf == ttf_auto) {
            // Do whatever produces a visible result.
            if (ttf_manager.ttf_active())
              init.font.use_ttf = ttf_off;
            else
              init.font.use_ttf = ttf_on;
          } else if (init.font.use_ttf == ttf_on) {
            init.font.use_ttf = ttf_off;
          } else {
            init.font.use_ttf = ttf_on;
          }
          gps.force_full_display_count++;
        }
		*/
        // Macro commands
        if (era.count(INTERFACEKEY_RECORD_MACRO)) {
          if (enabler.is_recording())
            enabler.record_stop();
          else
            enabler.record_input();
        }
        if (era.count(INTERFACEKEY_PLAY_MACRO)) {
          for (int i = 0; i < repeats; i++)
            enabler.play_macro();
        }
        if (era.count(INTERFACEKEY_SAVE_MACRO))
          gview.addscreen(new MacroScreenSave(), INTERFACE_PUSH_AT_BACK, NULL);
        if (era.count(INTERFACEKEY_LOAD_MACRO))
          gview.addscreen(new MacroScreenLoad(), INTERFACE_PUSH_AT_BACK, NULL);
        // Feed input
        for (int i = 0; i < repeats; i++)
          currentscreen->feed(era);
        if (era.count(INTERFACEKEY_TOGGLE_FULLSCREEN)) {
          enabler.toggle_fullscreen();
        }
        if (era.count(INTERFACEKEY_FPS_UP)) {
          int fps = enabler.get_fps();
          enabler.set_fps(fps + (fps+9)/10);
          enabler.clear_fps();
        }
        if (era.count(INTERFACEKEY_FPS_DOWN)) {
          int fps = enabler.get_fps();
          enabler.set_fps(fps - (fps+8)/10);
          enabler.clear_fps();
        }
      }
    }
    break;
  } // case INTERFACE_BREAKDOWN_NONE
    
  case INTERFACE_BREAKDOWN_QUIT:
    {
#ifdef CURSES_MOVIES
      handlemovie(1);
#endif
      return 1;
    }
  case INTERFACE_BREAKDOWN_STOPSCREEN:
	{
#ifdef CURSES_MOVIES
    if(currentscreen->movies_okay())
      {
        //HANDLE MOVIES
        handlemovie(0);
      }
#endif
    
    removescreen(currentscreen);
    
	currentscreen = grab_lastscreen();
	currentscreen->set_port_flags();
	break;
	}
  case INTERFACE_BREAKDOWN_TOFIRST:
#ifdef CURSES_MOVIES
    if(currentscreen->movies_okay())
      {
        //HANDLE MOVIES
        handlemovie(0);
      }
#endif
    
    remove_to_first();
    break;
  }
  
  return 0;
}

void interfacest::remove_to_first()
{
	//GRAB LAST SCREEN AT THE END OF THE LIST
	viewscreenst *lastscreen=&view;
	while(lastscreen->child!=NULL)lastscreen=lastscreen->child;

	//NO INTERFACE LEFT
	if(lastscreen==&view)return;

	//GO AHEAD
	while(lastscreen->parent!=&view)
		{
		viewscreenst *par=lastscreen->parent;
		removescreen(lastscreen);
		lastscreen=par;
		}

	viewscreenst *currentscreen = grab_lastscreen();
	currentscreen->set_port_flags();
}

void interfacest::removescreen(viewscreenst *scr)
{
	//THE MINIMAP IS EXPENSIVE, SO WE REFRESH IT WHENEVER INTERFACE GETS IN THE WAY
	if(gamemode==GAMEMODE_DWARF)dwarf_remove_screen();

	//FIX LINKS
	if(scr->parent!=NULL)scr->parent->child=scr->child;
	if(scr->child!=NULL)scr->child->parent=scr->parent;

	//WASTE SCREEN
	delete scr;
}

#ifdef CURSES_MOVIES
int interfacest::write_movie_chunk()
{
	int inputsize=supermovie_pos;
	if(inputsize>MOVIEBUFFSIZE)inputsize=MOVIEBUFFSIZE;

	//DUMP CURRENT BUFFER INTO A COMPRESSION STREAM
	z_stream c_stream;
	int err;

	c_stream.zalloc = (alloc_func)0;
	c_stream.zfree = (free_func)0;
	c_stream.opaque = (voidpf)0;

	err = deflateInit(&c_stream, 9);
	CHECK_ERR(err, "deflateInit");

	c_stream.next_out = (Bytef*)supermoviebuffer_comp;
	c_stream.avail_out = COMPMOVIEBUFFSIZE;

	c_stream.next_in  = (Bytef*)supermoviebuffer;
	c_stream.avail_in  = inputsize;

	while (c_stream.total_in != inputsize && c_stream.total_out < COMPMOVIEBUFFSIZE) {
		//c_stream.avail_in = c_stream.avail_out = 1; // force small buffers
		err = deflate(&c_stream, Z_NO_FLUSH);
		CHECK_ERR(err, "deflate");
		}

	// Finish the stream, still forcing small buffers:
	for (;;) {
		err = deflate(&c_stream, Z_FINISH);
		if (err == Z_STREAM_END) break;
		CHECK_ERR(err, "deflate");
		}

	err = deflateEnd(&c_stream);
	CHECK_ERR(err, "deflateEnd");

	int length=0;

	if(c_stream.total_out>0)
		{
		if(first_movie_write)
			{
			//GET RID OF ANY EXISTING MOVIES IF THIS IS THE FIRST TIME THROUGH
			unlink(movie_file.c_str());
			}

		//OPEN UP THE MOVIE FILE AND APPEND
		std::fstream f;
		f.open(movie_file.c_str(), fstream::out | fstream::binary | fstream::app);

		if(f.is_open())
			{
			//WRITE A HEADER
			if(first_movie_write)
				{
				int swp_l=byteswap(movie_version);
				f.write((const char *)&swp_l,sizeof(int));


				cursesmovie_headerst cmh;
					cmh.dimx=init.display.grid_x;
					cmh.dimy=init.display.grid_y;
					cmh.delayrate=supermovie_delayrate;
					cmh.dimx=byteswap(cmh.dimx);
					cmh.dimy=byteswap(cmh.dimy);
					cmh.delayrate=byteswap(cmh.delayrate);
				f.write((const char *)&cmh,sizeof(cursesmovie_headerst));

				int32_t s=gview.supermovie_sound.str.size();
				s=byteswap(s);
				f.write((const char *)&s,sizeof(int32_t));
				char buf[50];
				for(s=0;s<gview.supermovie_sound.str.size();s++)
					{
					strcpy(buf,gview.supermovie_sound.str[s]->dat.c_str());
					f.write(buf,sizeof(char)*50);
					}

				int i1,i2;
				for(i1=0;i1<200;i1++)
					{
					for(i2=0;i2<SOUND_CHANNELNUM;i2++)
						{
#ifndef NO_FMOD
						swp_l=byteswap(gview.supermovie_sound_time[i1][i2]);
#else
                        swp_l=-1;
#endif
						f.write((const char *)&swp_l,sizeof(int));
						}
					}
				}

			//WRITE IT
			int compsize=byteswap(c_stream.total_out);
			f.write((const char *)&compsize,sizeof(int));
			f.write((const char *)supermoviebuffer_comp,c_stream.total_out);

			f.seekg(0,ios::beg);
			int beg=f.tellg();
			f.seekg(0,ios::end);
			int end=f.tellg();
			length=end-beg;

			f.close();
			}
		else supermovie_on=0;

		first_movie_write=0;
		}

	return length;
}

void interfacest::read_movie_chunk(int &maxmoviepos,char &is_playing)
{
	//OPEN UP THE MOVIE FILE AND MOVE TO CORRECT POSITION
	std::fstream f;
	f.open(movie_file.c_str(), fstream::in | fstream::binary);

	if(f.is_open())
		{
		f.seekg(0,ios::beg);
		int beg=f.tellg();
		f.seekg(0,ios::end);
		int end=f.tellg();
		int file_size=end-beg;

		if(gview.nextfilepos<file_size)
			{
			f.seekg(gview.nextfilepos,ios::beg);

			//LOAD THE HEADER
			char fail=0;
			if(gview.nextfilepos==0)
				{
				int loadversion;
				f.read((char *)&loadversion,sizeof(int));
				loadversion=byteswap(loadversion);

				if(loadversion>movie_version)fail=1;

				cursesmovie_headerst cmh;
				f.read((char *)&cmh,sizeof(cursesmovie_headerst));
                                cmh.dimx=byteswap(cmh.dimx);
                                cmh.dimy=byteswap(cmh.dimy);
                                cmh.delayrate=byteswap(cmh.delayrate);

                                enabler.override_grid_size(cmh.dimx, cmh.dimy);
                                if (!gview.original_fps)
                                  gview.original_fps = enabler.get_fps();
                                enabler.set_fps(100);

				gview.supermovie_delayrate=cmh.delayrate;
				gview.supermovie_delaystep=cmh.delayrate;

				gview.supermovie_sound.clean();
				if(loadversion>=10001)
					{
					int num;
					f.read((char *)&num,sizeof(int));
					num=byteswap(num);
					gview.nextfilepos+=sizeof(int);
					char buf[50];
					int s;
					for(s=0;s<num;s++)
						{
						f.read(buf,sizeof(char)*50);
						string str=buf;
						gview.supermovie_sound.add_string(str);
						gview.nextfilepos+=sizeof(char)*50;
						}

					int i1,i2,swp_l;
					for(i1=0;i1<200;i1++)
						{
						for(i2=0;i2<SOUND_CHANNELNUM;i2++)
							{
							f.read((char *)&swp_l,sizeof(int));
#ifndef NO_FMOD
							gview.supermovie_sound_time[i1][i2]=byteswap(swp_l);
#endif
							}
						}

					gview.nextfilepos+=sizeof(int)*200*SOUND_CHANNELNUM;
					}
				else
					{
#ifndef NO_FMOD
					int i,c;
					for(i=0;i<200;i++)
						{
						for(c=0;c<SOUND_CHANNELNUM;c++)gview.supermovie_sound_time[i][c]=-1;
						}
#endif
					}

				gview.nextfilepos+=sizeof(int)+sizeof(cursesmovie_headerst);

#ifndef NO_FMOD
				//HANDLE SOUND LOADING
				int s;
				for(s=0;s<gview.supermovie_sound.str.size();s++)
					{
					string filename="data/sound/";
					filename+=gview.supermovie_sound.str[s]->dat;
					filename+=".ogg";
					
					musicsound.set_sound(filename,s);
					}
#endif
				}

			if(!fail)
				{
				//READ IT
				f.read((char *)&gview.currentblocksize,sizeof(int));
				gview.currentblocksize=byteswap(gview.currentblocksize);
				f.read((char *)gview.supermoviebuffer_comp,gview.currentblocksize);

				gview.nextfilepos+=gview.currentblocksize+sizeof(int);

				//UNCOMPRESS IT
				z_stream d_stream; // decompression stream

				d_stream.zalloc = (alloc_func)0;
				d_stream.zfree = (free_func)0;
				d_stream.opaque = (voidpf)0;

				d_stream.next_in  = (Bytef*)gview.supermoviebuffer_comp;
				d_stream.avail_in = gview.currentblocksize;

				int err = inflateInit(&d_stream);
				CHECK_ERR(err, "inflateInit");

				d_stream.next_out = gview.supermoviebuffer;
				d_stream.avail_out = MOVIEBUFFSIZE;

				while (d_stream.total_out < MOVIEBUFFSIZE && d_stream.total_in < gview.currentblocksize) {
					//d_stream.avail_in = d_stream.avail_out = 1; // force small buffers
					err = inflate(&d_stream, Z_NO_FLUSH);
					if (err == Z_STREAM_END) break;
					CHECK_ERR(err, "inflate");
					}

				err = inflateEnd(&d_stream);
				CHECK_ERR(err, "inflateEnd");

				gview.supermovie_pos=0;
				maxmoviepos=d_stream.total_out;
				}
			else
				{
				is_playing=0;
                                enabler.release_grid_size();
                                if (gview.original_fps)
                                  enabler.set_fps(gview.original_fps);
				}
			}
		else
			{
			is_playing=0;
                        enabler.release_grid_size();
                        if (gview.original_fps)
                          enabler.set_fps(gview.original_fps);
			}

		f.close();
		}
	else
		{
		is_playing=0;
                enabler.release_grid_size();
                if (gview.original_fps)
                  enabler.set_fps(gview.original_fps);
		}
}

void interfacest::handlemovie(char flushall)
{
	//SAVE A MOVIE FRAME INTO THE CURRENT MOVIE BUFFER
	if(supermovie_on==1)
		{
		if(supermovie_delaystep>0&&!flushall)supermovie_delaystep--;
		else
			{
			if(!flushall)supermovie_delaystep=supermovie_delayrate;

			if(!flushall||supermovie_delaystep==0)
				{
							//************************ DIM COLORS
						//this all supposes the old way of storing colors
															supermovie_pos=0;
							/*
				//SAVING CHARACTERS, THEN COLORS
				short x2,y2;
				for(x2=0;x2<init.display.grid_x;x2++)
					{
					for(y2=0;y2<init.display.grid_y;y2++)
						{
						supermoviebuffer[supermovie_pos]=gps.screen[x2*gps.dimy*8 + y2*8 + 0];

						supermovie_pos++;
						}
					}
				char frame_col;
				for(x2=0;x2<init.display.grid_x;x2++)
					{
					for(y2=0;y2<init.display.grid_y;y2++)
						{
						frame_col=gps.screen[x2*gps.dimy*8 + y2*8 + 1];
						frame_col|=(gps.screen[x2*gps.dimy*8 + y2*8 + 2]<<3);
						if(gps.screen[x2*gps.dimy*8 + y2*8 + 3])frame_col|=64;
						supermoviebuffer[supermovie_pos]=frame_col;

						supermovie_pos++;
						}
					}
				}

			int frame_size=init.display.grid_x*init.display.grid_y*2;
			if(supermovie_pos+frame_size>=MOVIEBUFFSIZE||flushall)
				{
				int length=write_movie_chunk();

				if(length>5000000)
					{
					finish_movie();
					}
				else supermovie_pos=0;
				*/
				}
			}
		}
}
#endif

void interfacest::print_interface_token(InterfaceKey key,justification just)
{
	short o_screenf=gps.screenf,o_screenb=gps.screenb,o_screenbright=gps.screenbright;
	gps.changecolor(2,0,1);
        string tok = enabler.GetKeyDisplay(key);
	gps.addst(tok,just);
	gps.changecolor(o_screenf,o_screenb,o_screenbright);
}

char standardstringentry(char *str,int maxlen,unsigned int flag,std::set<InterfaceKey> &events)
{
	string str2;
	str2=str;
	char ret=standardstringentry(str2,maxlen,flag,events);
	strcpy(str,str2.c_str());
	return ret;
}

std::set<char> invalid_filename_chars={ '<', '>', ':', '"', '/', '\\', '|', '?', '*' };

char standardstringentry(string& str, int maxlen, unsigned int flag, std::set<InterfaceKey>& events) {
	char r = standardstringentry(str, maxlen, flag, events, enabler.get_text_input());
	if(r) enabler.clear_text_input();
	return r;
}

char standardstringentry(string &str,int maxlen,unsigned int flag,std::set<InterfaceKey> &events, const char* text_input)
{
	enabler.set_listen_to_text(true); // cheap cheap cheap
	if (events.count(INTERFACEKEY_STRING_A000) && str.length()>0) {
		str.pop_back();
		events.clear();
		return 1;
	}
	else {
		if(events.count(INTERFACEKEY_SELECT)||events.count(INTERFACEKEY_LEAVESCREEN)||enabler.mouse_rbut) 
			{
			enabler.set_listen_to_text(false);
			if ((flag & STRINGENTRY_REMOVEKEYS))
				{
				events.erase(INTERFACEKEY_SELECT);
				events.erase(INTERFACEKEY_LEAVESCREEN);
				enabler.mouse_rbut=false;
				}
			return 0;
			}
		events.clear();
		if (str.length()>=maxlen)
			{
			return 0;
			}
		bool any_valid=false;
		for (size_t i = 0; i < 32; i++) {
			char entry = text_input[i];
			if (entry == '\0') break;
			if (str.length() < maxlen &&
				(entry == 0xA) ||
				(!(flag & STRINGENTRY_FILENAME) || invalid_filename_chars.count(entry) == 0) ||
				(flag & STRINGENTRY_SYMBOLS) ||
				((flag & STRINGENTRY_LETTERS) && (entry >= 'a' && entry <= 'z') || (entry >= 'A' && entry <= 'Z')) ||
				((flag & STRINGENTRY_SPACE) && entry == ' ') ||
				((flag & STRINGENTRY_NUMBERS) && (entry >= '0' && entry <= '9'))
				)
			{
				if (entry >= 'a' && entry <= 'z' && (flag & STRINGENTRY_CAPS)) {
					entry += 'A' - 'a';
				}
				any_valid=true;
				str.push_back(entry);
				if (str.length() >= maxlen || entry == '\0' || entry == 0xA) break;
			}
		}
		return any_valid;
	}
}

//To Do
//get the gview.c references inside the DEBUG_MOVIE defines
//make scrolling and stringentry use newer pressed functions for better speed
