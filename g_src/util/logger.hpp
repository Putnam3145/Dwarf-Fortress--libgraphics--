#ifndef DF_GSRC_UTIL_LOGGER_HPP
#define DF_GSRC_UTIL_LOGGER_HPP

#include <string>

void emit_logs();
void gamelog_string(const std::string& str);
void errorlog_string(const std::string& str);
void errorlog_string(const char* ptr);

#endif // DF_GSRC_UTIL_LOGGER_HPP