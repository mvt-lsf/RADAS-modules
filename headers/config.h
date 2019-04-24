#ifndef STDLIBS_H
#define STDLIBS_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#endif

#include <string.h>

struct config{
	int nch;
	int qfreq;
	int bins;
	int nshotsperchunk;
	int nsubchunk;
};

void read_config(char * filename, struct config *config);