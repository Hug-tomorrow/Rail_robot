#ifndef USER_LIB_H
#define USER_LIB_H
#include "struct_typedef.h"

int abs(int value);
int map(int value, int in_min, int in_max, int out_min, int out_max);
int16_t saturate(int16_t value, int16_t min, int16_t max);
#endif
