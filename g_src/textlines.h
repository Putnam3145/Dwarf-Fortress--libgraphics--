#ifndef TEXTLINES_H
#define TEXTLINES_H

#include <filesystem>

struct textlinesst
{
	stringvectst text;



	void load_raw_to_lines(const std::filesystem::path &file);

	void save_lines_to_raw(const std::filesystem::path &file);

	void clean_lines()
		{
		text.clean();
		}
	void copy_from(textlinesst &ot)
		{
		text.copy_from(ot.text);
		}
};

#endif