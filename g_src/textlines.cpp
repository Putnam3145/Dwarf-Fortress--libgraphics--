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

#include "basics.h"

extern char filecomp_buffer[20000];
extern char filecomp_buffer2[80000];
extern char filecomp_buffer_aux[20000];
extern char filecomp_buffer2_aux[80000];


void textlinesst::load_raw_to_lines(const char *filename)
{
	clean_lines();

	//LOAD THE FILE
	std::ifstream fseed(filename);
	if(fseed.is_open())
		{
		string str;

		while(std::getline(fseed,str))
			{
			long end=(long)str.length();

			while(end>0)
				{
				if(isspace(str[end-1]))end--;
				else break;
				}

			str.resize(end);

			if(str.length()>0)text.add_string(str);
			}
		}
	fseed.close();
}
void textlinesst::save_lines_to_raw(const char *filename) 
{
	std::ofstream fseed(filename);
	if (fseed.is_open())
		{
		for (auto &line : text.str)
			{
			fseed<<line->dat<<std::endl;
			}
		}
	fseed.close();
}