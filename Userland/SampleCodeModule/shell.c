
#include <library.h>
#include <system_calls.h>
#include <tron.h>
#include <console.h>

void initialize_shell(){
	putSquare(0,0,1024,BACKGROUND_COLOR);
	appendstring("> $ ");
	while(1){
		refreshCursor();
		//char c = getchar();
		//checkKey(c);
	}
}






//int a = 21;
	//float b = 3.6;
	//void* parameters[] = {"Joaquin", &a, &b};
	//char * foundation = "Hola me llamo %s, mi edad es %d y mi ojo ( version atp ) mide %f cm" ;
	//printf("Hola me llamo %s, mi edad es %d y mi ojo ( version atp ) mide %f cm", parameters);
	//char c;
	//while ( ( c = getchar()) ){
	//	if(c == 8){
	//		// videoBackSpace();
	//		// tiene que ser reemplazada
	//		buffer[lastChar--] = 0;
	//	}
	//	else if(c == '\n'){
	//		if ( buffer != 0 && buffer[0] != 0 )
	//			checkCommand(buffer);
	//		// clearBuffer();
	//		putnewline();
	//		/	appendstring(1,"enter");
//
//
	//	}
	//	else if(c == 27){ //escape
	//		//clearScreen();
	//		//restartCursor();
	//		/	appendstring(1,"escape");
	//	}
	//	else if ( c == '=' ){ // tecla para hacer un snapshot en un momento arbitrario
	//		//char* aux = snapshot(); 
	//		//memMoveChar(snapshotBuffer, aux, 128); 
	//		/	appendstring(1,"snapshot");
//
	//	}
	//	else{
	//		putchar(1,c);
	//		buffer[lastChar++] = c;
	//	}	
	//}