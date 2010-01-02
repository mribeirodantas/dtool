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
#include <string.h>
#include "config.h"

void run(const char* fmt, ...);
void dictionary(const char*, const char*, const char*);
void pronunciation(char* );
void translation(const char*, const char*, const char*);
void usage(void);
void filecheck(const char*);

char buffer[256];	/* run line buffer 		*/
char dir[64];		/* Working directory 	*/
char home_dir[64];	/* Home directory		*/

int main (int argc, char* argv[]) {
	char *from_lang = NULL, *to_lang = NULL;
    int index, c, option_index=0; /* variables to getopt_long */
    if (argc == 1) { 
		printf("Usage: dtool [from-language] [to-language] [option] \"word or phrase\"\n");
		printf("For help try: dtool --help\n");
		exit(EXIT_SUCCESS);
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
		if (c == -1) { break; } /* end of arguments reading */
	    switch (c) {
			case 'd':
				if ((from_lang == NULL) && (to_lang == NULL)) 
					dictionary(optarg, "en", "en");
				else if ((from_lang == NULL) && (to_lang != NULL))
					dictionary(optarg, "en", to_lang);
				else if ((from_lang != NULL) && (to_lang == NULL))
					dictionary(optarg, from_lang, "en");
				else	
					dictionary(optarg, from_lang, to_lang);
				break;
			case 'p':
				pronunciation(optarg);
				break;
			case 'T':
				if ((from_lang == NULL) && (to_lang == NULL)) {
					fprintf(stderr, "Missing argument.\n");
					usage();
					exit(EXIT_FAILURE);
				}
				else if (((strcmp(from_lang,"en")) != 0) && (to_lang == NULL))
					translation(optarg, from_lang, "en");
				else
					translation(optarg, from_lang, to_lang);
				break;
			case 'f':
				from_lang = optarg;
				break;
			case 't':
				to_lang = optarg;
				break;
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
				break;
			case '?':
				/* getopt_long, different from getopt, already printed an error message. */
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

	flag = system(buffer); /* It catchs int returned by the bin execd */
	if (flag) { 
		fprintf(stderr, "The word you prompted is not in our data base, or\n");
		fprintf(stderr, "you should check for your internet connection.\n");
		exit(EXIT_FAILURE);
	}
}

void dictionary(const char* word, const char* source, const char* destiny) {
	filecheck(".dtool");
	run("xdg-open \"http://www.google.com/dictionary?aq=f&langpair=%s%%7C%s&q=%s&hl=%s\"",
	source, destiny, word, destiny);
	run("`echo %s - %s/%s >> %s/.dic` &> /dev/null", word, source, destiny, home_dir);
		/* `` avoid run error msg */
}

void pronunciation(char* word) {
	filecheck(word);
	run("wget -nc -qc http://www.google.com/dictionary/sounds/%s.mp3", word);
	run("mv %s.mp3 %s &> /dev/null", word, dir);
	run("ffplay %s%s.mp3 &> /dev/null", dir, word);
	run("`echo %s >> %s/.pron` &> /dev/null", word, dir);
		/* `` show the mv one */
#ifndef KEEP_FILES
	run("rm -f %s/%s.mp3", dir, word);
#endif
}

void translation(const char* phrase, const char* source, const char* destiny) {
	run("xdg-open \"http://translate.google.com/translate_t#%s|%s|%s\"",
	source, destiny, phrase);
}

void usage(void) {
	printf("Usage: dtool [from-language] [to-language] [option] \"word to find or");
	printf(" translate\"\n");
	printf("--dictionary\t-d\tword\n");
	printf("--translation\t-T\tword or phrase\n");
	printf("--pronunciation\t-p\tpronunciation option\n");
	printf("--from\t\t-f\tfrom what language you want to translate\n");
	printf("--to\t\t-t\tto what language you want to translate\n");
	printf("--help\t\t-h\thelp option.\n");
	printf("Example:\n");
	printf("user@domn:~$ dtool --from en --to de --translation \"This is an");
	printf(" useful tool.\"\n");
	printf("Or even:\n");
	printf("user@domn:~$ dtool -f en -t de -T \"This is an useful tool.\"\n");
}

void filecheck(const char* word) {
	FILE *stream;
	static int x=1;

	if (x!=0) { /* Is it the first time it runs filecheck? */
		snprintf(home_dir, sizeof(home_dir), "%s/.dtool", getenv("HOME"));
	}
	if (!(stream = fopen(home_dir,"r"))) { /* Is there the .dtool directory ? */
		fprintf(stderr, "The %s/ directory is nonexistent.\n", home_dir);
		fclose(stream);
		exit(EXIT_FAILURE);
	}
	snprintf(dir, sizeof(dir), "%s/%s.mp3", home_dir, word); 
	if (((strcmp(word,".dtool")) != 0) && (stream = fopen(dir,"r"))) {
					/* You've already downloaded it */
		run("ffplay %s &> /dev/null", dir);
		fclose(stream);
		exit(EXIT_SUCCESS);
	}
	x=0; /* Making sure snprintf won't overwrite home_dir repeatedly */
}
