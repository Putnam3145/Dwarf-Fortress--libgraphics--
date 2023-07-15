#ifndef DF_GSRC_UTIL_RANDOM_HPP
#define DF_GSRC_UTIL_RANDOM_HPP

#include "../platform/platform.hpp"
#include "../platform/types.hpp"

#define MT_BUFFER_NUM 10
#define MT_LEN 624

void mt_init();
void trandom_twist();

void push_trandom_uniform_seed(uint32_t newseed);
void push_trandom_double_seed(uint32_t newseed1, uint32_t newseed2);
void push_trandom_triple_seed(uint32_t newseed1, uint32_t newseed2,
                              uint32_t newseed3);
void pop_trandom_uniform_seed();

uint32_t mt_trandom();

#endif // DF_GSRC_UTIL_RANDOM_HPP
