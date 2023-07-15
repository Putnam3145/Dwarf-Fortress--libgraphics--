#ifndef DF_GSRC_PLATFORM_TYPES_HPP
#define DF_GSRC_PLATFORM_TYPES_HPP

extern "C" {

#include <zlib.h>

#include "../_external/zlib/contrib/minizip/unzip.h"
#ifndef WIN32
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#endif
}

#ifndef WIN32
#include <stdint.h>
#endif

// Integer Types Base-Definition

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

#endif // DF_GSRC_PLATFORM_TYPES_HPP