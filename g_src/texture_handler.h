#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "textlines.h"

struct palette_pagest {

	string token;

	string graphics_dir;

	string filename;

	int32_t default_row = -1;
	stringvectst color_token;
	svector<int32_t> color_row;//linked
};

struct tile_pagest {

	string token;

	string graphics_dir;

	string filename;
	short tile_dim_x;
	short tile_dim_y;
	short page_dim_x;
	short page_dim_y;

	svector<long> texpos;
	svector<long> datapos;
	svector<long> texpos_gs;
	svector<long> datapos_gs;

	char loaded = 0;

	void load_graphics();
	void refresh_graphics();
};

struct texture_handlerst {
	// are you sure the svector is the right type here? not a map?
	svector<tile_pagest *> page;
	svector<palette_pagest *> palette;

	void clean();
	void adopt_new_lines(textlinesst &lines,const string &graphics_dir);

	~texture_handlerst()
		{
		clean();
		}

	tile_pagest *get_tile_page_by_token(string &tk)
		{
		int32_t t;
		for(t=0;t<page.size();t++)
			{
			if(page[t]->token==tk)return page[t];
			}
		return NULL;
		}
	palette_pagest *get_palette_page_by_token(string &tk)
		{
		int32_t t;
		for(t=0;t<palette.size();t++)
			{
			if(palette[t]->token==tk)return palette[t];
			}
		return NULL;
		}
};

#endif
