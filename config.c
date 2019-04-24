#include <config.h>

void read_config(char * filename, struct config *config){
    char *line = malloc(sizeof(char) * 200);
    char *value = malloc(sizeof(char) * 200);
    FILE *file_config = fopen(filename, "r");

    while(fscanf(file_config, "%200s %200s", line, value) != EOF){
        if (strcmp(line, "NCh") == 0)
            config->nch = atoi(value);
        if (strcmp(line, "qFreq") == 0)
            config->qfreq = atoi(value);
        if (strcmp(line, "Bins") == 0)
            config->bins = atoi(value);
        if (strcmp(line, "NShotsChk") == 0)
            config->nshotsperchunk = atoi(value);
        if (strcmp(line, "NSubChk") == 0)
            config->nsubchunk = atoi(value);
    }
}