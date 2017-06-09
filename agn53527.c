#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	5 /* size of history */

/** 
 * The setup function below will read in the next command line;
 * separate it into distinct arguments (using blanks as delimiters), 
 * and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */

int setup(char inputBuffer[], char *args[])
{
    int length,		/* # of characters in the command line */
	i,				/* loop index for accessing inputBuffer array */
	start,			/* index where beginning of next command parameter is */
	ct;			/* index of where to place the next parameter into args[] */
    char *token;
    ct = 0;
	
	  	
    /* read what the user enters on the command line */
   

	fgets(inputBuffer,MAX_LINE,stdin);   //get the line entered by the user

	printf("You entered : %s",inputBuffer);   //return the line for debuggging

	i = strlen(inputBuffer)-1;   //check if we have to fix the read in line
  	if( inputBuffer[i] == '\n') 
    	inputBuffer[i] = '\0';
	
    i=0;
	
   	token = strtok(inputBuffer, " "); 		//get the first token

   	args[i] = token;	//put the first token into the array

	//printf( "Args %i,  %s\n", i,args[i] ); //read back the command from the array

	while( token != NULL ) //loop for the rest of the tokens
   	{
   		i++;
     // printf( "Token:  %s\n", token );  //debugging print token statement
     
      	token = strtok(NULL, " "); 

      	args[i] = token;		//keep putting tokens into args array

      //  printf( "Args %i,  %s\n", i,args[i] );
     

   	}	
		//length = read(0,args,MAX_LINE); 
	
	return 1;
	
} /* end of setup routine */


int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	int status;           		/* result from execvp system call*/
	int shouldrun = 1;
	int i;
 
   while (shouldrun){            		/* Program terminates normally inside setup */

		printf("comp322>"); //print out the command prompt label

		shouldrun = setup(inputBuffer,args);       /* get next command */

		 /**
         * Deal with user's command from the new shell: 
         */

		child = fork(); // (1) fork a child process

		if(child == 0){    //check if we're the child

			register struct passwd *pw;   //variables to print userID and time
  			register uid_t uid;
  			uid = geteuid ();
  			pw = getpwuid (uid);
			time_t mytime;
			mytime = time(NULL);
		
			printf("Child Process: Command: %s Args: %s\n",args[0],args[1] );  //debugging print out the command and arguments

			printf("Program run by %s at %s\n",pw->pw_name , ctime(&mytime));   //print out the run by userID at time statement
			execvp(args[0],args);  //  * (3) the child process will invoke execvp()
			
		} else {  //we are the parent and must wait
			wait(NULL);
			
		}
    }
	return 0;
}
