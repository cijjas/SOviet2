#include <irqDispatcher.h>
#include <syscalls.h>
#include <sound_driver.h>
#include <video_driver.h>
#include <tron.h>
#include <interrupts.h>
#include <naiveConsole.h>
extern uint64_t getkey();

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3


static void int_20();
static void int_21();
static void int_80();

static void write();
static void tron();

static char buffer[500] = {0};
static int lastChar = 0;

char * getBuffer(){
	return buffer;
}
//static void int_22(uint64_t rdi, uint64_t rsi, uint64_t rdx ,uint64_t rcx, uint64_t r8, uint64_t r9);
void (*fun_inter[256])(uint64_t,uint64_t,uint64_t,uint64_t);
uint64_t (*fun_sys[256])(uint64_t,uint64_t,uint64_t);

void clearBuffer ( void ){
	for ( int i = 0; i < 80; i++ )
		buffer[i] = (char)0;
	lastChar = 0;
}

char strcmp( const char* stringA,const char* stringB)  
{  
    char flag = 0;
	int i = 0;
    while(stringA[i]!='\0' && stringB[i]!='\0')  {  
       if(stringA[i]!=stringB[i])  
       {  
           flag=1;  
           break;  
       }  
       i++;  
    }  
    if(flag==0)  
    	return 1;  
 	return 0;  
}  

void commandHelp(){
	clearScreen();
	videoPrintWord("BIENVENIDO AL MENU HELP");
	videoNewLine();
	videoPrintWord("EL SISTEMA CUENTA CON LOS SIGUIENTES COMANDOS:");
	videoNewLine();
	videoPrintWord("- HELP");
	videoNewLine();
	videoPrintWord("- TRON");
	videoNewLine();
	videoPrintWord("- SNAPSHOT");
	videoNewLine();
	videoPrintWord("- MEMACCESS ( MEM ADDRESS )");
	videoNewLine();
	videoPrintWord("- CLEAR");
	videoNewLine();
	videoPrintWord("PRESIONE ESC PARA VOLVER AL MENU PRINCIPAL");
}

void commandTron(){
	clearScreen();
	initialize_players();
	tronMotherfucker(1);
	
}
void commandClear(){
	clearScreen();
	restartCursor();
}
void checkCommand( char * command ){
	if(strcmp(command, "HELP") || strcmp(command, "- HELP") ){
		commandHelp();
	}
	else if(strcmp(command, "TRON") || strcmp(command, "- TRON") ){
		commandTron();
	}
	else if(strcmp(command, "CLEAR") || strcmp(command, "- CLEAR") ){
		commandClear();
	}
	else if(strcmp(command, "BEEP") || strcmp(command, "- BEEP") ){
		//beep();
	}
}

void initialize(){
	(fun_inter[0])=int_20;
	(fun_inter[1])=int_21;
	(fun_inter[0x60])=int_80; // pasasr 60 en el asm

	//agreago syscalls
	(fun_sys[0])=sys_read;
	(fun_sys[1])=sys_write;
	(fun_sys[2])=sys_time;
}

void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, char *  rdx ,uint64_t rcx) {
	(*fun_inter[irq])(rdi,rsi,rdx,rcx);
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	uint8_t teclahex=getkey();
	static const uint8_t charHexMap[256] = 
    {       
          0,    27,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '-',  '=',    8,    '    ',
        'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',   'O',  'P',  '[',   ']',  '\n',    0,     'A',       'S',
        'D',  'F',  'G',  'H',  'J',  'K',  'L',  ';',  '\'',    0,    0,  '\\',   'Z',  'X',     'C',       'V',
        'B',  'N',  'M',  ',',  '.',  '/',    0,  '*',     0,  ' ',    0,     0,     0,    0,       0,         0,
    };
	uint8_t aux = charHexMap[teclahex];
	soviet_anthem();
	if(tronOn()){
		tron(aux);

	} //chequeo en que modo estoy y envio el char obtenido a la funcion apropiada
	else{
		write(aux);
	}
	
}

void int_80(uint64_t rdi, uint64_t rsi, char *  rdx ,uint64_t rcx){
	(*fun_sys[rdi])(rsi,rdx,rcx);
	return;
}

void write(int aux){ // escritura usando funciones de video
	if(aux!=0){
		if(aux == 8){
			videoBackSpace();
			buffer[lastChar--] = 0;
		}
		else if(aux == '\n'){
			if ( buffer != 0 && buffer[0] != 0 )
				checkCommand(buffer);
			clearBuffer();
			videoNewLine();

		}
		else if(aux == 27){ //escape
			clearScreen();
			restartCursor();
		}
		else{
			videoPrintChar(aux);
			buffer[lastChar] = aux;
			lastChar+=1;
		}
	}
	
	//int teclahex=getkey();
	//	if(charHexMap[ teclahex]=='\n')
	//			ncNewline();
	//	else if(charHexMap[ teclahex]!=0)
	//		ncPrintChar (charHexMap[ teclahex]);
	//
}



void tron(int aux){
	// asigno WASD para el jugador 1 y IJKL para el jugador 2
	switch(aux){
		case 27:
			//gameSwitch(0);
			tronMotherfucker(0);
			clearScreen();
			restartCursor();

		/*
		case ' ':
			gameSwitch(1); // al apretar la barra espaciadora se inicia el juego
			break;
		*/
		case 'W':
			changePlayerDirection(1,UP);
			break;
		case 'A':
			changePlayerDirection(1,LEFT);
			break;
		case 'S':
			changePlayerDirection(1,DOWN);
			break;
		case 'D':
			changePlayerDirection(1,RIGHT);
			break;
		
		case 'I':
			changePlayerDirection(2,UP);
			break;
		case 'J':
			changePlayerDirection(2,LEFT);
			break;
		case 'K':
			changePlayerDirection(2,DOWN);
			break;
		case 'L':
			changePlayerDirection(2,RIGHT);
			break;
		default:
			break;
	}
}