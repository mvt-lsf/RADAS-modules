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

/*
 * read config: lee la configuracion especificada por filename
 * filename: el nombre del archivo a leer
 * struct *config: el struct config sobre el cual escribir
 * CUIDADO! read config no checkea por errores!
 */
void read_config(char * filename, struct config *config);
