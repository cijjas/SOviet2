#include <shell.h>

void *writer(int argc, char **argv)
{
	print("Writing in a Buffer!\n");
	exit();
	return NULL;
}

void *reader(int argc, char **argv)
{
	print("What i read from the buffer : ");
	while (1)
	{
		char c = getchar(); // lee del pipe
		print("%c", c);		// escribe en consola
	}
	exit();
	return NULL;
}

void *sleepyJoe(int argc, char **argv){
	print("\nIm going to sleep 5 seconds, like Timmy...\n");
	sleep(5);
	print("Im waking up, I FEEL IT IN MY BONES\n");
	exit();
	return NULL;
}


void *initialize_shell(int argc, char **argv)
{
	putSquare(0, 0, 1024, BACKGROUND_COLOR);
	printColor("user@Qemu:", USER_TEXT_COLOR, 0);
	printColor("> $ ", TERMINAL_BLUE, 0);


//	createFGProcess("sleepyJoe", sleepyJoe, 0, NULL );

	/* --------------------------PIPE TEST---------------------------- */

	// int pipeFD = openPipe("superpipe");
	// print("PIPE FD = %d\n", pipeFD);
	// int writeFD[] = {0, pipeFD};
	// int readFD[] = {pipeFD, 1};

	// int writerPID = createProcess("writer", writer, 0, NULL, writeFD);
	// waitPid(writerPID);
	// int readerPID = createProcess("reader", reader, 0, NULL, readFD);
	// waitPid(readerPID);

	// closePipe(pipeFD);

	/* ---------------------------------------------------------------- */

	while (1)
	{
		char c = getchar();
		if (c != 0)
		{
			handleKey(c);
		}
	}
	return NULL;
}
