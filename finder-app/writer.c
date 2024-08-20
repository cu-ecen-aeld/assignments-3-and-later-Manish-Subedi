#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
    openlog("writer_a2", 0, LOG_USER);
    if(argc != 3){
        syslog(LOG_ERR, "Invalid argument counts: %d", argc-1);
        return 1; 
    }
    const char *filename = argv[1];
    const char *string = argv[2];

    FILE *fp; 
    fp = fopen(filename, "w");
    if (fp == NULL){
        syslog(LOG_ERR, "Error opening the file: %s", filename);
        return 1; 
    }
    
    if (fprintf(fp, "%s", string) < 0){
        syslog(LOG_ERR, "Error writing to file: %s", filename);
        fclose(fp);
        return 1;
    }
    syslog(LOG_DEBUG, "Writing %s to %s", string, filename);

    if(fclose(fp) != 0){
        syslog(LOG_ERR, "Error closing the file: %s", filename);
        return 1;
    }
    closelog();
    return 0;
}