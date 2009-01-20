/*
###################################################################
#                                                                 #
#  Author: Marcel Ribeiro Dantas <vuln at slackware-rn.com.br>    #
#  Website: http://www.slackware-rn.com.br/~vuln                  #
#  This code is licensed under GNU GPLv3 and any other            #
#  version of this license.                                       #
#                                                                 #
#  A copy of this license can be read in:                         #
#  http://www.gnu.org/licenses/gpl-3.0.txt                        #
#                                                                 #
#  dtool - a dictionary tool with many features.                  #
#  A tool for help you translating                                #
#  and learning more about a word in                              #
#  a lot of different languages                                   #
#                                                                 #
################################################################### */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	char command[256];
	int i;

	if (argc == 2 && (!(strcmp(argv[1],"-h")))) {
		
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("-d        dictionary option.\n");
		printf("-h        help option.\n");
		printf("-t        translation option.\n");
		printf("-p        pronunciation option\n");
		printf("Special parameter: \n");
		printf("dtool -p \"word/phrase to listen\"\n");
		
		return 0;
	
	}
	
	else if (argc == 3 && (!(strcmp(argv[1],"-p")))) {
	/*	snprintf(command, sizeof(command), "wget -qc http://www.google.com/dictionary/sounds/%s.mp3", argv[2]);
		system(command);
		command[0] = '\0';
		snprintf(command, sizeof(command), "xdg-open %s.mp3 >> /dev/null", argv[2]);
		system(command);
		command[0] = '\0';
		snprintf(command, sizeof(command), "rm -i %s.mp3", argv[2]);

		snprintf(command, sizeof(command), "xdg-open \"http://www.howjsay.com/index.php?word=%s&submit=Submit\"", argv[2]); */
		
		snprintf(command, sizeof(command), "xdg-open \"http://www.google.com/dictionary/sounds/%s.mp3\"", argv[2]);

	}

	else if (argc != 5 ) {
		
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("For help try: dtool -h\n");

		return 0;
	
	}
		
	else if (!(strcmp(argv[1],"-d"))) {
	
		snprintf(command, sizeof(command), "xdg-open \"http://www.google.com/dictionary?aq=f&langpair=%s%%7C%s&q=%s&hl=en\"", argv[2], argv[3], argv[4]);
	}

	else	if (!(strcmp(argv[1],"-t"))) {

		snprintf(command, sizeof(command), "xdg-open \"http://translate.google.com/translate_t#%s|%s|%s\"", argv[2], argv[3], argv[4]);
	}

	else if (!(strcmp(argv[1],"-p"))) {
	
		snprintf(command, sizeof(command), "xdg-open \"http://www.google.com/dictionary/sounds/%s\"", argv[2],".mp3");

	}

	else {
		printf("Usage: dtool [option] [from-language] [to-language] \"word to find or translate\"\n");
		printf("For help try: dtool -h\n");
		
		return 0;
		
	}

      system(command);
	
	return 0;

}
