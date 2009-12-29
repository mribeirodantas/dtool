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
void dictionary(char*, char*, char*);
void pronunciation(char* );
void translation(char*, char*, char*);
void usage(void);
void dircheck(void);

char buffer[256] = "0";

int main (int argc, char* argv[]) {
	char *argument2 = NULL, *argument3 = NULL;
    int index;
    int c;
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
		/* getopt_long stores the option index here. */
		int option_index = 0;
		c = getopt_long(argc, argv, "d:p:T:f:t:h", long_options, &option_index);
		/* : to required argument. :: to optional */
		if (c == -1) { break; }
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
				/* getopt_long already printed an error message. */
				break;
			default:
				abort ();
		}
    }
    for (index = optind; index < argc; index++)
	printf ("Non-option argument %s\n", argv[index]);
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

void dictionary(char* word, char* source, char* destiny) {
	dircheck();
	run("xdg-open \"http://www.google.com/dictionary?aq=f&langpair=%s%%7C%s&q=%s&hl=%s\"",
	source, destiny, word, destiny); /* ou s s w d? */
	run("`echo %s >> ~/.dtool/.dic` &> /dev/null", word); /* avoid run error msg */
}

void pronunciation(char* word) {
	dircheck();
	run("wget -nc -qc http://www.google.com/dictionary/sounds/%s.mp3", word);
	run("mv %s.mp3 ~/.dtool/ &> /dev/null", word);
	run("ffplay -vn -nodisp ~/.dtool/%s.mp3 &> /dev/null", word);
	run("`echo %s >> ~/.dtool/.pron` &> /dev/null", word); /* and show the mv one */
#ifndef KEEP_FILES
	run("rm -f ~/.dtool/%s.mp3", word);
#endif
}

void translation(char* phrase, char* source, char* destiny) {
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
	char home_dir[64];
	snprintf(home_dir, sizeof(home_dir), "%s/.dtool", getenv("HOME"));
	if (!(stream = fopen(home_dir,"r"))) {
		fprintf(stderr, "The ~/.dtool directory is nonexistent.\n");
		exit(1);
	}
	fclose(stream);
}
