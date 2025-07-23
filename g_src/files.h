//Copyright (c) 2006, Tarn Adams
//All rights reserved.  See game.cpp or license.txt for more information.
#ifndef FILES_H
#define FILES_H

#define FILE_IN_BUFF 1000000
#define FILE_OUT_BUFF 2000000

#include <string>
#include <string.h>
using std::string;

#include <fstream>
#include <string_view>
#include <filesystem>
#include <optional>
#include "endian.h"
#include "svector.h"

inline constexpr const char *PORTABLE_FILENAME="prefs/portable.txt";

inline constexpr std::filesystem::copy_options D_COPY_OPTIONS=std::filesystem::copy_options::overwrite_existing|std::filesystem::copy_options::recursive;

/// <summary>
/// A wrapper around a "local" path.
/// Essentially, all previously raw-accessed local paths should be grabbed through filest instead;
/// just wrap the path, e.g. any previous reference to "prefs/init.txt" should instead use
/// filest("prefs/init.txt"), at which point one can get the file proper via the various methods on filest.
/// </summary>
struct filest {
	/// <summary>
	/// The contained path object. This is generally not to be used raw, but is convenient to access,
	/// for example so that you can construct a new filest using an old one.
	/// </summary>
	std::filesystem::path path;
	/// <summary>
	/// Gets the absolute path of the location of this file in the base folder.
	/// So, on the Steam version on Windows, `filest("prefs/init.txt").base_location().string() will return
	/// "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Dwarf Fortress\\prefs\\init.txt".
	/// It also converts it to the preferred format for the platform, thus all the escaped backslashes.
	/// </summary>
	/// <returns>The absolute path for this file in the base folder.</returns>
	std::filesystem::path base_location() const;
	/// <summary>
	/// Gets the absolute path of the location of this file in the prefs folder.
	/// On Windows, `filest("prefs/init.txt").pref_location().string() will return
	/// "C:\\Users\\{username}\\AppData\\Roaming\\Bay 12 Game\\Dwarf Fortress\\prefs\\init.txt".
	/// It also converts it to the preferred format for the platform, thus all the escaped backslashes.
	/// </summary>
	/// <returns>The absolute path for this file in the prefs folder.</returns>
	std::filesystem::path pref_location() const;
	/// <summary>
	/// Gets the absolute path of the canon location of this file.
	/// The canon location depends on if the game is currently in portable mode or not;
	/// if it is, the canon location is the base folder, otherwise, it's the prefs folder.
	/// This should be used exclusively for all writes.
	/// </summary>
	/// <returns>The absolute path for the canon location of this file.</returns>
	std::filesystem::path canon_location() const;
	/// <summary>
	/// Gets the absolute path of the non-canon location of this file.
	/// This is the inverse of the canon location. Rarely useful; included for completeness' sake,
	/// although it is, in fact, used in remove_file, which helpfully deletes the file from both locations.
	/// </summary>
	/// <returns>The absolute path for the non-canon location of this file.</returns>
	std::filesystem::path non_canon_location() const;
	/// <summary>
	/// Gets the absolute path of any location for this file.
	/// If the file exists at the canon location, it'll return that path; otherwise it'll fall back on the non-canon location.
	/// If the file exists at neither, it'll return std::nullopt. This must be accounted for in all cases, lest we get crashes.
	/// </summary>
	/// <returns>The absolute path of any location for this file, if it exists, otherwise std::nullopt.</returns>
	std::optional<std::filesystem::path> any_location() const;
	/// <summary>
	/// Gets the absolute path of any location for this file.
	/// If the file exists at the canon location, it'll return that path; otherwise it'll fall back on the non-canon location.
	/// If the file exists at neither, it'll return the canon location anyway. The caller must account for this.
	/// </summary>
	/// <returns>The absolute path of any location for this file, if it exists, otherwise the canon location.</returns>
	std::filesystem::path any_location_unchecked() const;
	/// <summary>
	/// An array of both locations, for iterating over.
	/// The canon location is guaranteed to be first, followed by the non-canon one.
	/// This is if you want to check both for whatever reason, and not just whichever is there first.
	/// </summary>
	/// <returns>A 2-member array, containing both the canon and non-canon locations, in that order.</returns>
	std::array<std::filesystem::path,2> both_locations() const;
	/// <summary>
	/// An array of two items, each containing a bool saying if it's portable or not, then the path itself.
	/// This is mostly just to label things portable in the UI, where appropriate.
	/// The intended use uses pattern-matching, i.e.
	/// `for (auto &[is_portable,save_dir] : filest("save").both_locations_tagged()` is the only place it's used in the initial version.
	/// </summary>
	/// <returns>A 2-member array, containing canon and non-canon locations, in that order, tagged with a bool that is true if it's the portable location.</returns>
	std::array<std::pair<bool,std::filesystem::path>,2> both_locations_tagged() const;
	/// <summary>
	/// Returns an std::ofstream to the canon location of the file.
	/// Where before one would do `std::ofstream("prefs/init.txt")`, for example, now one should do `filest("prefs/init.txt").to_ofstream()`.
	/// This should be used instead of raw ofstreams wherever possible. Saving into Program Files might make the OS a bit irate, so, 
	/// ultimately, using this particular method is the entire point of this struct. The ofstream is just a normal ofstream.
	/// This will save to the canon location.
	/// </summary>
	/// <returns>An ofstream writing to this file's canon location.</returns>
	std::ofstream to_ofstream(std::ios_base::openmode mode=std::ios::out) const;
	/// <summary>
	/// Returns an std::ifstream to any location of the file.
	/// Where before one would do `std::ifstream("prefs/init.txt")`, for example, now one should do `filest("prefs/init.txt").to_ofstream()`.
	/// If no file exists, it will be an empty ifstream, and `is_open()` will return `false`, as expected.
	/// </summary>
	/// <returns>An ifstream reading from this file, if it exists anywhere.</returns>
	std::ifstream to_ifstream(std::ios_base::openmode mode=std::ios::in) const;
	/// <summary>
	/// Checks if the file exists anywhere, and if it does, copies it to the canon location.
	/// Doing this is implicit in most of the game's general file operations,
	/// but having an explicit function for it is nice.
	/// </summary>
	void canonize(std::filesystem::copy_options copy_options=D_COPY_OPTIONS) const;
	/// <summary>
	/// Constructs a filest using the given `std::filesystem::path`.
	/// Notably, `const char*` (and thus string literals) are implicitly convertable to `std::filesystem::path`, so this constructor just works for those,
	/// and it works for `std::string` too.
	/// </summary>
	/// <param name="p">The relative std::filesystem::path this filest refers to</param>
	/// <returns>This filest</returns>
	filest(const std::filesystem::path p) : path(p) {};
	/// <summary>
	/// Constructs a filest with an empty path.
	/// The path can be filled in later.
	/// Not too much reason to use this, but since the data of a filest is just a wrapper around a path, it'd be a little silly not to have this.
	/// </summary>
	filest() {
		path=std::filesystem::path();
		};
	};

/// <summary>
/// Copies a file from `src`, if it exists anywhere, to `dst`, at its canon location.
/// </summary>
/// <param name="src">The file to copy from.</param>
/// <param name="dst">The file to copy to.</param>
/// <returns>Whether there was a successful copy.</returns>
bool copy_file(const filest &src,const filest &dst,std::filesystem::copy_options copy_options=D_COPY_OPTIONS);

/// <summary>
/// Copies a file or directory from `src`, if it exists anywhere, to `dst`, at its canon location.
/// There are subtle differences between copy_file and copy in std::filesystem, so better to have a wrapper around both.
/// </summary>
/// <param name="src">The file to copy from.</param>
/// <param name="dst">The file to copy to.</param>
/// <returns>Whether there was a successful copy.</returns>
bool copy_file_or_dir(const filest &src,const filest &dst,std::filesystem::copy_options copy_options=D_COPY_OPTIONS);

/// <summary>
/// Copies a file or directory from `src`, which is assumed to be an absolute path, to `dst`, at its canon location.
/// </summary>
/// <param name="src">The file to copy from.</param>
/// <param name="dst">The file to copy to.</param>
/// <returns>Whether there was a successful copy.</returns>
bool copy_file_or_dir(const std::filesystem::path &src,const filest &dst,std::filesystem::copy_options copy_options=D_COPY_OPTIONS);

/// <summary>
/// Deletes and replaces the file at the canon location of `dst` with `src`, if `src` exists.
/// </summary>
/// <param name="src">The replacement file.</param>
/// <param name="dst">The file to replace.</param>
/// <returns>Whether there was a successful copy.</returns>
void replace_file(const filest &src,const filest &dst);

/// <summary>
/// Creates the directory represented by this filest at the canon location.
/// This works recursively, so you can e.g. do `create_directory(filest("foo/bar/baz"))` and it'll create all of foo, bar and baz if those didn't exist.
/// </summary>
/// <param name="src">The director(y|ies) to be created</param>
void create_directory(const filest &src);

/// <summary>
/// Removes the file at all locations.
/// </summary>
/// <param name="src">The file to be removed.</param>
/// <returns>`true` if any file was deleted, otherwise `false`.
bool remove_file(const filest &src);

/// <summary>
/// Displays an error code, if there is one; returns `true` if there was an error, otherwise false.
/// </summary>
/// <param name="ec">The errorcode in question.</param>
/// <param name="message">The message to be displayed before the errorcode's own.</param>
/// <returns>`true` if there was an error.</returns>
bool display_file_error(const std::error_code &ec,const std::string &message);

/// <summary>
/// Prints an error to the errorlog if there is one.
/// </summary>
/// <param name="ec">The errorcode in question.</param>
/// <param name="message">The message to be displayed in the errorlog before the errorcode's own.</param>
#define log_file_error(ec, m) if(ec) errorlog_string(std::string(m)+ec.message()); 

class file_compressorst
{
	public:
		bool compressed;
		std::fstream f;


		char open_file(const string &filename,char existing_only);
		char open_file(const filest &file,char existing_only, bool read_only=false);
		void close_file();
		char write_file(string &str);
		char read_file(string &str);

		char load_posnull_pointer();
		char save_posnull_pointer(void *ptr);

		void write_file_fixedlength(char *var)
			{
			int16_t len=(int16_t)strlen(var);
			write_file(len);
			if(len>0)write_file(var,len*sizeof(char));
			}
		void read_file_fixedlength(char *var)
			{
			int16_t len;
			read_file(len);//DO NOT NEED TO ALLOCATE SPACE
			if(len>0)read_file(var,len*sizeof(char));
			var[len]='\x0';
			}
		char write_file(int32_t var)
			{
			var=byteswap(var);
			return write_file(&var,sizeof(int32_t));
			}
		char read_file(int32_t &var)
			{
			char ret = read_file(&var,sizeof(int32_t));
			var = byteswap(var);
			return ret;
			}
		char write_file(int16_t var)
			{
			var=byteswap(var);
			return write_file(&var,sizeof(int16_t));
			}
		char read_file(int16_t &var)
			{
			char ret = read_file(&var,sizeof(int16_t));
			var = byteswap(var);
			return ret;
			}
		char write_file(int8_t var)
			{
			return write_file(&var,sizeof(int8_t));
			}
		char read_file(int8_t &var)
			{
			return read_file(&var,sizeof(int8_t));
			}
		char write_file(bool var)
			{
			int8_t temp;
			if(var)temp=1;
			else temp=0;
			return write_file(&temp,sizeof(int8_t));
			}
		char read_file(bool &var)
			{
			int8_t temp;
			if(!read_file(&temp,sizeof(int8_t)))return 0;
			var=(temp!=0);
			return 1;
			}
		char write_file(uint32_t var)
			{
			var=byteswap(var);
			return write_file(&var,sizeof(uint32_t));
			}
		char read_file(uint32_t &var)
			{
			char ret = read_file(&var,sizeof(uint32_t));
			var = byteswap(var);
			return ret;
			}
		char write_file(uint16_t var)
			{
			var=byteswap(var);
			return write_file(&var,sizeof(uint16_t));
			}
		char read_file(uint16_t &var)
			{
			char ret = read_file(&var,sizeof(uint16_t));
			var = byteswap(var);
			return ret;
			}
		char write_file(uint8_t var)
			{
			return write_file(&var,sizeof(uint8_t));
			}
		char read_file(uint8_t &var)
			{
			return read_file(&var,sizeof(uint8_t));
			}
		void write_file(svector<bool> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			bool bl;//JUST FOR PARITY WITH read BELOW
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				bl=(*i_b);
				write_file(bl);
				}
			}
		void read_file(svector<bool> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			bool bl;//NO IDEA WHY IT CAN'T JUST TAKE vect[i]
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file(bl);
				(*i_b)=bl;
				}
			}
		void write_file(svector<int16_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<int16_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}
		void write_file(svector<uint16_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<uint16_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}
		void write_file(svector<uint8_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<uint8_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}
		void write_file(svector<int8_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<int8_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}
		void write_file(svector<int32_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<int32_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}
		void write_file(svector<uint32_t> &vect)
			{
			int32_t s=(int32_t)vect.size();
			write_file(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				write_file((*i_b));
				}
			}
		void read_file(svector<uint32_t> &vect)
			{
			int32_t s;
			read_file(s);
			vect.resize(s);
			auto i_b=vect.begin(),i_e=vect.end();
			for(;i_b<i_e;++i_b)
				{
				read_file((*i_b));
				}
			}


		char load_new_in_buffer();
		char flush_in_buffer();

		file_compressorst();
		file_compressorst(char *new_in_buffer,long new_in_buffersize,
						char *new_out_buffer,long new_out_buffersize);
		~file_compressorst()
			{
			close_file();
			}
		void set_buffer_info(char *new_in_buffer,long new_in_buffersize,
						 char *new_out_buffer,long new_out_buffersize);

	private:
		char write_file(void *write_var,long write_size);
		char read_file(void *read_var,long read_size);

		char *in_buffer;
		long in_buffersize;
		long in_buffer_amount_loaded;
		long in_buffer_position;

		char *out_buffer;
		long out_buffersize;
		int32_t out_buffer_amount_written;

		static char def_ibuff[FILE_IN_BUFF];
		static char def_obuff[FILE_OUT_BUFF];
};

const char8_t *get_base_path_str();

std::filesystem::path get_base_path();

const char8_t *get_pref_path_str();

std::filesystem::path get_pref_path();

bool df_copy_file(const std::string &src,const std::string &dst);
// Replaces dst with src, removing src in the process. Atomic if possible.
void replace_file(const string &src, const string &dst);

#endif
