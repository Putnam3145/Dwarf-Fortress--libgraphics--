#ifdef DF_GLUE_CPP
// Note: Provide compile-time flag to glue

/**
 * glue.hpp
 *
 * glue methods for making the code compilable,
 * providing external symbols and (stub) function
 * definitions
*/

// random.h glue

#include "../g_src/util/random.hpp"
int mt_index[MT_BUFFER_NUM];
short mt_cur_buffer;
short mt_virtual_buffer;
uint32_t mt_buffer[MT_BUFFER_NUM][MT_LEN];

// enabler.h glue

char beginroutine(){
    return 0;
}
char mainloop(){
    return 0;
}
void endroutine(){

}

// interface.cpp glue

#include "../g_src/render/interface.hpp"
interfacest gview;

void dwarf_end_announcements(){}
void dwarf_remove_screen(){}
void drawborder(const char *str,char style,const char *colorstr){}

// music_and_sound.cpp glue

thread_local string errorlog_prefix;

// initst init glue

#include "../g_src/render/init.hpp"
initst init;

// graphics.cpp glue

#include "../g_src/text/textlines.hpp"
void process_object_lines(textlinesst &lines,const std::string &chktype, const std::string &graphics_dir){
    return;
}

// init.cpp glue

int32_t convert_raw_to_ascii_texpos(uint8_t tile,uint8_t color_f,uint8_t color_b,uint8_t color_br){
    return 0;
}

// ViewBase.cpp glue

#include "../g_src/render/KeybindingScreen.hpp"
viewscreenst::viewscreenst(){}
void viewscreenst::set_port_flags() {
}

// other stuff

#include "../g_src/util/basics.hpp"
GameMode gamemode;
GameType gametype;
int32_t movie_version;

#endif // DF_GLUE_CPP
