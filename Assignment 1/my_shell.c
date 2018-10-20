#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

extern char **getln();
void argumentHandler (char**args, int counter, char* fileOut, char* fileIn, int out, int inp);
void addFunction (char** args, int counter);
void argFunction (char** args, int counter);
void prmeFunction (char** args);


int main() {
	int i;
	int counter = 0;
	int out = 0;
	int inp = 0;
	int x = 0;
	char **args;
	char* fileOut;
	char* fileIn;

	while(1) {

		printf(">>");
		args = getln();
		out = 0;
		inp = 0;
		counter = 0;

		//Loops the shell until arguements are null.
		for(i = 0; args[i] != NULL; i++)
		{
			x = i + 1;
			printf("Argument %d: %s\n", i, args[i]);

			//Used for redirecting the file, delete from arguments and save make input 1
			if(args[i][0] == '<')
			{
				fileIn = args[x];
				free(args[i]);
				args[i] = args[x];
				args[x] = NULL;
				inp = 1;
			}

			//Used for redirecting the file, delete from arguments and save make output 1
			if(args[i][0] == '>')
			{
				fileOut = args[x];
				free(args[i]);
				args[i] = args[x];
				args[x] = NULL;
				out = 1;
			}

			//Counter for all the arguments
			counter++;

		}

		//Exit the shell if only the command "exit" is entered.
		if((strncmp(args[0], "exit", 4) == 0) && (args[1] == NULL)) exit(0);

		//Prime function within shell.
		else if(strncmp(args[0], "prme", 4) == 0) prmeFunction(args);

		//Add function within shell.
		else if(strncmp(args[0], "add", 4) == 0) addFunction(args, counter);

		//Args function within shell.
		else if(strncmp(args[0], "args", 4) == 0) argFunction(args, counter);

		//Function to call the argument handler.
		else argumentHandler(args, counter, fileOut, fileIn, out, inp);

	}
}

//Function for calculating the average of the arguments
void prmeFunction(char **args){

	int check = 0, x;
	int stat = 0;
	check = atof(args[1]);
	int final = 0;

	//Checks to see if a positive number was entered or not.
	if (check <= 0) printf("Please enter a numerical value and non negative\n");

	else
	{
		//Used in for loop, divisible by 2
		final = check / 2;
		//Loop to check if it is not a prime number
		//for (x = 2; x<=check/2; x++)
		for (x = 2; x<=final; x++)
		{

			//Criteria to check if number is not a prime number
			if (check%x==0)
			{
				printf("%d is not a prime number.", check);
				stat = 1;
				break;
			}
		}
		// If for loop does not catch anything, it is a prime number.
		if (stat == 0) printf("%d is a prime number", check);
	}

}

//Function for listing all the arguments in the shell
void argFunction (char **args, int counter){

	int x;
	int start = 0;
	start = counter-1;

	printf("argc = %d, args = ", start);

	//Loop through the list of arguments and print each one out.
	for (x=1; x < counter; x++)
	{

			if (x == start) printf("%s\n", args[x]);

			else printf("%s, ", args[x]);

	}

}

//Function for adding all the arguments in the shell
void addFunction (char** args, int counter){

	int x;
	int start = 0;
	int answer = 0;
	int temp = 0;

	for (x=1; x<counter; x++)
		{
			start = counter-1;
			temp = atoi(args[x]);
			//Do the add function via using hexadecimal characters if 0x is found.
			if (args[x][0] == '0')
			{
				if (args[x][1] == 'x')
				{
					//Converting Hex values and adding to normal.

					if (args[x][2] == 'A' || args[x][2] == 'a')
					{
						answer = 10 + answer;
					}

					else if (args[x][2] == 'B' || args[x][2] == 'b')
					{
						answer = 11 + answer;
					}

					else if (args[x][2] == 'C' || args[x][2] == 'c')
					{
						answer = 12 + answer;
					}

					else if (args[x][2] == 'D' || args[x][2] == 'd')
					{
						answer = 13 + answer;
					}

					else if (args[x][2] == 'E' || args[x][2] == 'e')
					{
						answer = 14 + answer;
					}

					else if (args[x][2] == 'F' || args[x][2] == 'f')
					{
						answer = 15 + answer;
					}

				}
			}
				//Do normal calculation without
			else answer =  temp + answer;

			//Print statements for single input vs multiple numbers.
			if (x != start) printf("%s+", args[x]);

			else printf("%s =", args[x]);

		}
		printf( "%d \n", answer);
}


//Function that involves forking and argument handling.
void argumentHandler (char**args, int counter, char* fileOut, char* fileIn, int out, int inp){

	int stat;
	int resEx;
	int ampCheck = 0;
	int starter = 0;
	pid_t cProc;

	//forking.
	cProc = fork();

	starter = counter -1;

	//Ampersand Checker
	if (args[starter][0] == '&')
	{
		if (strlen(args[starter]) ==1)
		{
			ampCheck = 1;
			args[starter] = NULL;
		}

	}

	//Checking if Fork has error or not, if cProc is less than 0 it has an error.
	if (cProc == 0 || cProc > 0 )
	{
			if (cProc == 0) // Forking - Child
			{
				//Redirection commences if input was set to one.
				if (inp == 1) freopen(fileIn, "r", stdin);

				//Redirection commences it output was set to one
				if (out == 1) freopen(fileOut, "w+", stdout);


				//Execute the first argument
				resEx = execvp(args[0], args);

				//If Execvp is less than 0 there was an error.
				if (resEx != 0)
				{
					printf("Unknown Command - Error Executing \n");
					exit(-1);

				}
				exit(1);
			}

			else  // Forking - Parent
			{
				if (ampCheck == 0)
				{
					//If no ampersand, Parent needs to wait for child
					resEx = waitpid(cProc, &stat, 0);
				}
			}
	}

	//cProc is less than 0 and forking has an error.
	else
	{
		printf("Error in forking");
	}

}
