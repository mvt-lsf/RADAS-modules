#include <stdio.h>
#include <stdlib.h>

int main(){
    char *line = malloc(sizeof(char) * 200);
    char *value = malloc(sizeof(char) * 200);
    FILE *file_config = fopen("config.ini", "r");

    while(fscanf(file_config, "%200s %200s", line, value) != EOF){
        if (strcmp(line,"NCh") == 0)
            printf("\n%s %d",line, atoi(value));
        if (strcmp(line,"qFreq") == 0)
            printf("\n%s %d",line, atoi(value));
        if (strcmp(line,"Bins") == 0)
            printf("\n%s %d",line, atoi(value));
        if (strcmp(line,"NShotsChk") == 0)
            printf("\n%s %d",line, atoi(value));
        if (strcmp(line,"NSubChk") == 0)
            printf("\n%s %d",line, atoi(value));

        printf("\nline: %s",line);
        printf("\nvalue: %s", value);
    }

    getc(stdin);
}