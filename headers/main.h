#ifndef STDLIBS_H
#define STDLIBS_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <conio.h>
#include <string.h>
#include <windows.h>

#ifndef WD_DASK_H
#define WD_DASK_H
#include "WD-dask64.h"
#endif

#include "config.h"
#include "card.h"

struct callback_data{
	struct config *config;
	I16 cardnumber;
	I16 *channel0_buffer;
	I16 *channel1_buffer;
};

void callback();