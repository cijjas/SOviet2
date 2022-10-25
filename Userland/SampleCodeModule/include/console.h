#ifndef CONSOLE_H_
#define CONSOLE_H_

#define BACKSPACE 8
#define ESC 27
#define ENTER 10

extern char* snapshot();

static char consoleBuffer[256] = {0};
static char snapshotBuffer[128] = {0};
static int lastChar = 0;

void memMoveChar( char* array1, char* array2, int charsToMove );
char strcmp2( const char* stringA,const char* stringB);
void checkCommand(char * command);
void commandHelp();
void commandSnapshot();
void commandClear();
void printRegisters();
char toHex( char character );
char * toUPPER(char * string);
void clearScreen();
void checkKey( char c );

#endif