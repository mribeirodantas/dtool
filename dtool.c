/*
####################################################################
#                                                                  #
#  Author:  Marcel Ribeiro Dantas <vuln at slackware-rn.com.br>    #
#  Website: http://www.slackware-rn.com.br/~vuln                   #
#  This code is licensed under GNU GPLv3 and any other             #
#  version of this license.                                        #
#                                                                  #
#  A copy of this license can be read in:                          #
#  http://www.gnu.org/licenses/gpl-3.0.txt                         #
#                                                                  #  
#  dtool - a dictionary tool with many features.                   # 
#  A tool for help you translating                                 # 
#  and learning more about a word in                               #  
#  a lot of different languages                                    #
#                                                                  #
#################################################################### */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#ifdef __linux__
#include <getopt.h>
#endif
#include "config.h"

void exec(const char *fmt, ...)
{
    char cmd[8192] = { 0 };
    va_list l;

    va_start(l, fmt);
    /* XXX: I'd use vasprintf, but... gah... */
    vsnprintf(cmd, 8192, fmt, l);
    va_end(l);
    /* XXX: I'll implement something with vfork + execve later */
    system(cmd);
    return ;
}

int main(int argc, char **argv, char **envp) {
	if (argc == 2 && (!(strcmp(argv[1],"-h")))) {
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("-d        dictionary option.\n");
		printf("-h        help option.\n");
		printf("-t        translation option.\n");
		printf("-p        pronunciation option\n");
		printf("Special parameter: \n");
		printf("dtool -p \"word/phrase to listen\"\n");
	    exit(0);
    }
	
	else if (argc == 3 && (!(strcmp(argv[1],"-p")))) {
		exec("wget -qc http://www.google.com/dictionary/sounds/%s.mp3", argv[2]);
		exec("xdg-open %s.mp3 &> /dev/null", argv[2]);
#ifndef KEEP_FILES
		exec("rm -f %s.mp3", argv[2]);
#endif
    }

	else if (argc != 5 ) {
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("For help try: dtool -h\n");
	    exit(1);
    }
		
	else if (!(strcmp(argv[1],"-d")))
        exec("xdg-open \"http://www.google.com/dictionary?aq=f&langpair=%s%%7C%s&q=%s&hl=en\"", argv[2], argv[3], argv[4]);

	else if (!(strcmp(argv[1],"-t")))
		exec("xdg-open \"http://translate.google.com/translate_t#%s|%s|%s\"", argv[2], argv[3], argv[4]);

	else if (!(strcmp(argv[1],"-p")))
		exec("xdg-open \"http://www.google.com/dictionary/sounds/%s\"", argv[2],".mp3");

	else {
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("For help try: dtool -h\n");
	    exit(0);	
	}

	return 0;
}
