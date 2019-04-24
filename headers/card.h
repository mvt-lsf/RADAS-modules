#ifndef STDLIBS_H
#define STDLIBS_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <conio.h>
#include <windows.h>

#ifndef WD_DASK_H
#define WD_DASK_H
#include "WD-dask64.h"
#endif

void setup_buffers(int cardnumber, int pointsperchunk, I16 *ch0_buff, I16 *ch1_buff, I16 Id);
void allocate_buffers(int cardnumber, int buffersize, I16 **ch0_buff, I16 **ch1_buff);
void configure_card(I16 cardnumber);
I16 register_card();