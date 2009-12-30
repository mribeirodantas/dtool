/*
####################################################################
#                                                                  #
#  Author:  Marcel Ribeiro Dantas <marcel at ribeirodantas.com.br> #
#  Website: http://www.ribeirodantas.com.br/blog                   #
#  This code is licensed under GNU GPLv3 or any other newer        #
#  version of this license released of your choice (GPLv3+).       #
#                                                                  #
#  A copy of the license can be read in:                           #
#  http://www.gnu.org/licenses/gpl-3.0.txt                         #
#  or in the COPYING file which should follow with this			   #
#  source file one.												   #	
#                                                                  #  
#  dtool - a dictionary tool with many features.                   # 
#  A tool for help you translating                                 # 
#  and learning more about a word in                               #  
#  a lot of different languages                                    #
#                                                                  #
####################################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>
#include "config.h"

void run(const char* fmt, ...);
void dictionary(const char*, const char*, const char*);
void pronunciation(const char* );
void translation(const char*, const char*, const char*);
void usage(void);
void dircheck(void);

char buffer[256] = "0";
char home_dir[64] = "0";

int main (int argc, char* argv[]) {
	char *argument2 = NULL, *argument3 = NULL;
    int index, c, option_index=0; /* variables to getopt_long */
    if (argc == 1) { 
		printf("Usage: dtool [from-language] [to-language] [option] \"word or phrase\"\n");
		printf("For help try: dtool --help\n");
		exit(0);
	}
    while (1) {
		static struct option long_options[] = {
			{"dictionary",		required_argument,	0,	'd'},
			{"pronunciation",	required_argument,	0,	'p'},
			{"translation",		required_argument,	0,	'T'},
			{"from",			required_argument, 	0,	'f'},
			{"to",				required_argument,	0,	't'},
			{"help",			no_argument,		0,	'h'},
			{0,					0,					0,	0}
		};
		c = getopt_long(argc, argv, "d:p:T:f:t:h", long_options, &option_index);
		if (c == -1) { break; } /* end of argument reading */
	    switch (c) {
			case 'd':
				if ((argument2 == NULL) || (argument3 == NULL)) 
					dictionary(optarg, "en", "en");
				else	
					dictionary(optarg, argument2, argument3);
				break;
			case 'p':
				pronunciation(optarg);
				break;
			case 'T':
				translation(optarg, argument2, argument3);
				break;
			case 'f':
				argument2 = optarg;
				break;
			case 't':
				argument3 = optarg;
				break;
			case 'h':
				usage();
				break;
			case '?':
				/* getopt_long, different from getopt already printed an error message. */
				break;
			default:
				abort ();
		}
  	}
    for (index = optind; index < argc; index++)
	fprintf(stderr, "Non-option argument %s\n", argv[index]);
	return EXIT_SUCCESS;
}

void run(const char* fmt, ...) {
	va_list l;
	int flag=0;

	va_start(l, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, l); 
	va_end(l);

	flag = system(buffer);
	if (flag) { 
		fprintf(stderr, "The word you prompted is not in our data base, or\n");
		fprintf(stderr, "you should check for your internet connection.\n");
		exit(1);
	}
}

void dictionary(const char* word, const char* source, const char* destiny) {
	dircheck();
	run("xdg-open \"http://www.google.com/dictionary?aq=f&langpair=%s%%7C%s&q=%s&hl=%s\"",
	source, destiny, word, destiny); /* ou s s w d? */
	run("`echo %s - %s/%s >> %s/.dic` &> /dev/null", word, source, destiny, home_dir);
		/* `` avoid run error msg */
}

void pronunciation(const char* word) {
	dircheck();
	run("wget -nc -qc http://www.google.com/dictionary/sounds/%s.mp3", word);
	run("mv %s.mp3 %s &> /dev/null", word, home_dir);
	run("ffplay -vn -nodisp %s/%s.mp3 &> /dev/null", home_dir, word);
	run("`echo %s >> %s/.pron` &> /dev/null", word, home_dir);
		/* `` show the mv one */
#ifndef KEEP_FILES
	run("rm -f %s/%s.mp3", home_dir, word);
#endif
}

void translation(const char* phrase, const char* source, const char* destiny) {
	run("xdg-open \"http://translate.google.com/translate_t#%s|%s|%s\"", source, destiny, phrase);
}

void usage(void) {
	printf("Usage: dtool [from-language] [to-language] [option] \"word to find or translate\"\n");
	printf("--dictionary\t-d\tword\n");
	printf("--translation\t-T\tword or phrase\n");
	printf("--pronuncation\t-p\tpronunciation option\n");
	printf("--from\t\t-f\tfrom what language you want to translate\n");
	printf("--to\t\t-t\tto what language you want to translate\n");
	printf("--help\t\t-h\thelp option.\n");
	exit(0);
}

void dircheck() {
	FILE *stream;
	static int x=1;

	if (x!=0) {
		snprintf(home_dir, sizeof(home_dir), "%s/.dtool", getenv("HOME"));
	}
	if (!(stream = fopen(home_dir,"r"))) {
		fprintf(stderr, "The %s/.dtool directory is nonexistent.\n", home_dir);
		exit(1);
	}
	fclose(stream);
	x=0;
}
