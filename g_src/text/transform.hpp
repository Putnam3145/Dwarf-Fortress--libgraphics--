#ifndef DF_GSRC_TEXT_TRANSFORM_HPP
#define DF_GSRC_TEXT_TRANSFORM_HPP

#include <string>

#include "../platform/types.hpp"

bool grab_token_string(std::string& dest, std::string& source,
                       char compc = ':');
bool grab_token_string(std::string& dest, std::string& source, int32_t& pos,
                       char compc = ':');
bool grab_token_string_pos(std::string& dest, std::string& source, int32_t pos,
                           char compc = ':');
void replace_token_string(std::string& token, std::string& str, int32_t pos,
                          char compc, std::string& nw, char repc);
bool grab_token_list_as_string(std::string& dest, std::string& source,
                               int32_t& pos, char compc = ':');
bool grab_token_expression(std::string& dest, std::string& source, int32_t& pos,
                           char compc = ':');
char grab_variable_token(std::string& str, std::string& token, char sec_comp,
                         int32_t& pos, int32_t i_pos);

int32_t convert_string_to_long(const std::string& str);
uint32_t convert_string_to_ulong(const std::string& str);
uint64_t convert_string_to_ulong64(const std::string& str);
void add_long_to_string(int32_t n, std::string& str);
void add_ulong64_to_string(uint64_t n, std::string& str);
void convert_long_to_string(int32_t n, std::string& str);
void convert_ulong_to_string(uint32_t n, std::string& str);
void convert_ulong64_to_string(uint64_t n, std::string& str);

void lower_case_string(std::string& str);
void upper_case_string(std::string& str);
void simplify_string(std::string& str);
void capitalize_string_words(std::string& str);
void capitalize_string_first_word(std::string& str);
void abbreviate_string(std::string& str, int32_t len);

void get_number(int32_t number, std::string& str);
void get_ordinal(int32_t number, std::string& str, bool shorten);

#endif