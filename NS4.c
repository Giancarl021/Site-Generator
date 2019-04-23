#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <windows.h>
#include <locale.h>

typedef int bool;
enum {false, true};

char date[12];
char add[256][256]; // the args with frameworks to add in the HTML
char *presetFolder = "presets"; // the folder with the base files
bool sub = false; // if the args have any framework to add in the HTML

void setDate(void); // get Date with String format YYYY-MM-DD
char* formatDate(int x); // put a zero before a DD or MM if the number is less than 10
void createFolder(char *mainfolder, char *name); // create folder using mkdir (cmd) with master folder afress (fd) + path or only master folder adress (to create it)
void writeFile(char *mainfolder, char *destination, char *origin, bool interrupt); // create and write a file, in the master folder adress (fd) + path (destination) from another file (origin), if the file needs a edit based on args, the interrupt needs to be true

void set_window_visibility(char visibility[]); // A windows function imported (copied) from another library (my lib)
void die(const char message[]); // A exit function imported (copied) and with little modifications from another library (my lib too)

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	
	// get args from console
	
	int n = 0, pn = -1, i = 1, posHTML = 0;
	bool tmp = sub, intHTML = true;
	char *presetHTML[] = {
			"presets\\preset.html",
			"presets\\preset.php"
		 },
		 *presetCSS = "presets\\preset.css",
		 *presetJS = "presets\\preset.js",
		 *destHTML[] = {
		 	"\\index.html",
			"\\index.php"
		 };
	
	// testing if some file has been dropped in the exe file
	
	FILE *tmpFILE = fopen(argv[1], "r");
	
	if(tmpFILE != NULL) {
		if(argv[1] != NULL) {
			if(strstr(argv[1], ".html") != NULL) {
				presetHTML[0] = argv[1]; // changing adress to the preset file
				i = 2;
				intHTML = false;
			}
			else if(strstr(argv[1], ".php") != NULL) {
				presetHTML[1] = argv[1];
				i = 2;
				posHTML = 1;
				intHTML = false;
			}
			else if(strstr(argv[1], ".css") != NULL) {
				presetCSS = argv[1];
				i = 2;
			}
			else if(strstr(argv[1], ".js") != NULL) {
				presetJS = argv[1];
				i = 2;	
			}
		}	
	}
	fclose(tmpFILE);
	
	// get other args
	
	for(; i < argc; i++) { // search for commands in all the args passed
		if(strstr(argv[i], "#") != NULL && strlen(argv[i]) > 1) { // if the arg in index i hava a "#" and after "#" have some text, then stop picking "-add" references and save the position of the arg with the folder name
			tmp = false;
			pn = i;
		}
		else if(strstr(argv[i], "-") != NULL) { // if the arg in index i have a "-" then serch what is the command
			if(strcmp(argv[i], "-add") == 0) { // if the arg in index i its a "-add" command, turn to true the framework interrumption in HTML
				tmp = sub = true; // the tmp its to stop the references name catch if another command its readed
			}
			else if(strcmp(argv[i], "-presets") == 0 || strcmp(argv[i], "-config") == 0) { // if the arg in index i its a "-preset" or "-config" command, then close the program and open the "presets" folder
				system("start presets");
				printf("Abrindo pasta de configurações e presets...\n");
				return 0;
			}
			else if(strcmp(argv[i], "-html") == 0) {
				posHTML = 0;
			}
			else if(strcmp(argv[i], "-php") == 0) {
				posHTML = 1;
			}
		}
		else if(tmp) { // if "-add" command its active, pick the args to import the frameworks later on HTML file
			strcpy(add[n++], argv[i]);
		}
	}
	
	// set folder name
	
	setDate();
	
	char *fd = (pn == -1) ? date : (++argv[pn]); // the folder name recieves the actual date (if none arg have "#" with the folder name) or the folder name recieved in args (the "++" before the name of folder its to remove the "#" to the first pointer of the argv[pn] and puts on the next letter) 
	char bf[256];
	
	// verification
	
	DIR *def = opendir(presetFolder); // presets folder exists
	closedir(def);
	if(!def) {	
		die("Pasta de configuração corrompida ou inexistente...\n");
	} else { // defect if all the preset files exists
		FILE *fBf = fopen(presetHTML[0], "r");
		fclose(fBf);
		if(fBf == NULL) die("Arquivos de configuração corrompidos ou inexistentes...\n");
		fBf = fopen(presetHTML[1], "r");
		fclose(fBf);
		if(fBf == NULL) die("Arquivos de configuração corrompidos ou inexistentes...\n");
		fBf = fopen(presetCSS, "r");
		fclose(fBf);
		if(fBf == NULL) die("Arquivos de configuração corrompidos ou inexistentes...\n");
		fBf = fopen(presetJS, "r");
		fclose(fBf);
		if(fBf == NULL) die("Arquivos de configuração corrompidos ou inexistentes...\n");
	}
	
	DIR *dir = opendir(fd); // destination folder already exists
	closedir(dir);
	if(dir) die("Pasta base já existe...\n");
	
	// create folders
	
	strcpy(bf, fd);
	
	createFolder(bf, "");
	Sleep(100); // a delay to give Windows time to create the folder and the other commands works
	createFolder(bf, "\\css");
	createFolder(bf, "\\js");
	createFolder(bf, "\\img");
		
	// create and write files
	
	writeFile(bf, destHTML[posHTML], presetHTML[posHTML], intHTML); // the HTML its the unique file with a break into to put frameworks based on args
	writeFile(bf, "\\css\\style.css", presetCSS, false);
	writeFile(bf, "\\js\\script.js", presetJS, false);
	
 	
	printf("Arquivos gerados...\n");
	return 0;
}

// exclusive functions

void setDate(void) {
	time_t t = time(NULL);	 // create struct time_t to get current time
	struct tm tm = *localtime(&t); // create tm struct to organize the time_t struct
	char c[12];
	snprintf(c, 12, "%d-%s-", tm.tm_year + 1900, formatDate(tm.tm_mon + 1)); // prints on "c" string year and month in format YYYY-MM- using "formatDate" function in month ints to put a zero before the number if its less than 10
	strcat(c, formatDate(tm.tm_mday)); // contat only the day (its separated because the three data in "snprintf" will print ")" (no idea why)
	strcpy(date, c);
}

char* formatDate(int x) {
	char c[3];
	if(x < 10) snprintf(c, 3, "0%d", x); // prints 0 + x (int) in the "c" string
	else snprintf(c, 3, "%d", x); //printf only x (int) in the "c" string
	char *r = c;
	return r;
}

void createFolder(char *mainfolder, char *name) {
	char bf[256] = "mkdir ";
	strcat(bf, mainfolder);
	if(strcmp(name, "")) strcat(bf, name); // if name == "" so name isn't added to bf (memory junk protection)
	system(bf);
}

void writeFile(char *mainfolder, char *destination, char *origin, bool interrupt) {
	bool lock = false; // a boolean to avoid the program find another "@" and put the frameworks there too
	char bf[256];
	strcpy(bf, mainfolder); // concat the destination adress
	strcat(bf, destination);
	FILE *dst = fopen(bf, "w"); // create and open the file in write mode
	FILE *org = fopen(origin, "r"); // open the preset file in read mode
	char ch, chx;
	
	while((ch = fgetc(org)) != EOF) { // read the preset file until the file ends
		if(ch == '@' && interrupt && sub && !lock) { // if the preset file have an "@" in this index, the interrupt is true (only in HTML file) and its the first time finding a "@", put the frameworks 
			lock = true; // lock this if to dont duplicate the inserts
			for(int i = 0; i < 256; i++) { // search in all the add commands array
				if(strlen(add[i]) > 0)	{ // detect if has some string in this index
					strcpy(bf, presetFolder); // put the preset base adress
					strcat(bf, "\\_ref\\");
					strcat(bf, add[i]); // put the string from arg
					strcat(bf, ".ref"); // put the extension
					FILE *fr = fopen(bf, "r"); // open the file in read mode
					if(fr != NULL) { // if the file doesnt exists, the fr will be null
						while((chx = fgetc(fr)) != EOF) { // for every character of the preset file, print in the destination file
							if(chx == '\n') { // if have a newline in the preset file, the indentation will be maked with these four spaces
								fprintf(dst, "\n    ");
							} else {
								fprintf(dst, "%c", chx);
							}
						}
					}
					fclose(fr); // close the preset file
				}
			}
			fprintf(dst, "<!-- PRESET BY GIANCARL021 -->"); // prints the "watermark" (just for dont leave a clean line in the miidle of the <head> tag)
		}
		else if(ch == '@' && !lock && interrupt) { // if doesnt have founded a "@" yet, this cahracter as a "@" and this file have a interrupt order (only HTML), put the watermark
			lock = true;
			fprintf(dst, "<!-- PRESET BY GIANCARL021 -->");
		}
		else fprintf(dst, "%c", ch); // any other character will be printed in the destination file
	}
	fclose(dst); // close the preset and the destination files
	fclose(org);
}

// sgd.h working functions

void set_window_visibility(char visibility[]) {
	HWND window = GetConsoleWindow();
	if(strcmp(visibility, "minimize") == 0 || strcmp(visibility, "minimized") == 0) {
		ShowWindow(window, SW_MINIMIZE);
	} else if (strcmp(visibility, "hide") == 0 || strcmp(visibility, "hidden") == 0) {
		ShowWindow(window, SW_MINIMIZE);
		ShowWindow(window, SW_HIDE);
	} else if(strcmp(visibility, "show") == 0 || strcmp(visibility, "visible") == 0){
		ShowWindow(window, SW_RESTORE);
	}
}

void die(const char message[]) {
	system("title Alerta de Erro");
	char *c;
	printf("%s", message);
	fflush(stdin);
	scanf("%c", &c);
	exit(0);
}
