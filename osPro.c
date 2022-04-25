#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>

#define LENGTH 1200 
#define SIZE 100 

//Global Declarations
int Welcome();						
int getInput(char* val); 				
void CurrDirec();		  				
int execArgs(char** path);			
int execArgsPiped(char** parsed, char** parsedpipe);
void Assist();						
int Commandhandler(char** parsed);		
int parsePipe(char* str, char** piped_string);
void parseSpace(char* str, char** parsed); 				
int Getstr(char* str, char** parsed, char** parsed_pipe);	


//main function
void main()
{
	char get_string[LENGTH];
	char* parsed_Arguments[SIZE];
	char* parsed_Arguments_Piped[SIZE];
	int temp = 0;
	
	
	Welcome();
	while(1) 
	{
		CurrDirec();
		if (getInput(get_string))
			continue;
		temp = Getstr(get_string,parsed_Arguments, parsed_Arguments_Piped);
		if (temp==1)
			execArgs(parsed_Arguments);
		if (temp==2)
			execArgsPiped(parsed_Arguments, parsed_Arguments_Piped);
	}
}

int Welcome()
{
	char* username = getenv("USER");
	printf("\n\n\n\n\t\t***************""""""""""""""************************");
	printf("\n\n\n\n\t\t*****Welcome %s to Linux SHELL*****",username);
	printf("\n\n\n\n\t\t***************""""""""""""""************************");
	
	printf("\t\t\t \n");
    printf("\t\t\t \n");
    printf("\t\t\t                             .::!!!!!!!:.\n");
  	printf("\t\t\t.!!!!!:.                 .:!!!!!!!!!!!!\n");
  	printf("\t\t\t~~~~!!!!!!.          .:!!!!!!!!!UWWWSSS\n"); 
    printf("\t\t\t:SSNWX!!:           .:!!!!!!XUWWSSSSSSP\n"); 
    printf("\t\t\tSSSSS##WX!:      .<!!!!UWSSSS  SSSSSSSS# \n");
    printf("\t\t\tSSSSS  SSSUX   :!!UWSSSSSSSSS   4SSSSS* \n");
    printf("\t\t\t^SSSB  SSSS     SSSSSSSSSSSS   dSSR \n");
    printf("\t\t\t *SbdSSSS      '*SSSSSSSSSSSo+#\n" );
    printf("\t\t\t""""""""          """"""""\n"); 

	
	
	printf("\n\n\n Current USER is: %s", username);
	printf("\n");
	
	sleep(5);
	return 0;
}


int getInput(char* val)
{

	char* ptr=readline("\n ---- ))  ");
	int check=0;	
	
	if(strlen(ptr) !=check)
	{
		add_history(ptr);
		strcpy(val, ptr);
		return 0;	
	}
	else
	{
		return -1;
	}
}

void CurrDirec()
{
	char curr_work_dirc[256];
	
	getcwd(curr_work_dirc, sizeof(curr_work_dirc));
	printf("\nDirectory: %s", curr_work_dirc);
}

int execArgs(char** path)
{ 
	pid_t P = fork();
	
	if(P==-1){
		printf("\nThere is an Error while calling Fork()..");	
		return 0;
	}
	 
	else if (P==0)
	{
		if (execvp(path[0], path) < 0)
			printf("\n");
			
		exit(0);
	}

	else{
		wait(NULL);
		return 0;
	}
}

int execArgsPiped(char** path, char** pathpipe)
{
	int RW [ 2 ] ;
	pid_t processID_1;
	pid_t processID_2;

	if (pipe(RW)<0) 
	{
		printf("\nSorry, Pipe can't created");
		return 0;
	}
	
    processID_1=fork();
	if (processID_1<0) 
	{
		printf("\nSorry, the System can't Forked");
		return 0;
	}

	if (processID_1==0) 
	{
		close (RW[0]);
		dup2 (RW[1],STDOUT_FILENO);
		close (RW[1]);
		if (execvp(path[0],path)<0){
			printf("\nSorry instruction_one does not Executed");
			exit(0);
		}
	} 
	else 
	{
		processID_2=fork();
		if (processID_2<0) 
		{
			printf("\nSorry, the Sytem can't Forked");
			return 0;
		}
		if (processID_2==0) 
		{
			close (RW[1]);
			dup2 (RW[0],STDIN_FILENO);
			close (RW[0]);	
			if (execvp(pathpipe[0],pathpipe)<0) 
			{
				printf("\nSorry, instruction_two does not Executed");
				exit (0);
			}
		} 
		else{
		wait(NULL);
 	}
}
}

int Commandhandler(char** parsed)
{
	int num = 4;
	int i;
	int check=0;
	char* cmd_list[num];
	char* credentials;

	cmd_list[0] = "exit";
	cmd_list[1] = "cd";
	cmd_list[2] = "help";
	cmd_list[3] = "hello";
	
	for (i=0;i<num;i++) 
	{
		if (strcmp(parsed[0],cmd_list[i])==0) 
		{
			check=i+1;
			break;
		}
	}
	
	switch (check)
	{
	case 1:
		printf("\nTATA, Bye Bye\n");
		exit (0);
	case 2:
		chdir(parsed[1]);
		return 0;
	case 3:
		Assist();
		return 0;
	case 4:
		credentials = getenv("USER");
		printf("\nHey %s, how are you ?.\n"
			"Please Be Prudent while using this Server."
			"\nFor any Query use help ;)\n",
			credentials);
		return 0;
		
	default:
		break;	
	}
	return 0;
}
int Getstr(char* str, char** path, char** pathpipe)
{

	char* piped_string[2];
	int concat=0;

	concat=parsePipe(str,piped_string);
	
	if (concat) 
	{
		parseSpace (piped_string[0],path);
		parseSpace (piped_string[1],pathpipe);
	} 
	
	else 
	{
		parseSpace(str,path);
	}

	if (Commandhandler(path))
		return 0;
	else
		return 1+concat;
}

int parsePipe(char* str, char** piped_string)
{
        int i;
	for(i=0;i<2;i++){
		piped_string[i]=strsep(&str,"|");
		if (piped_string[i]==0)
			break;
	}
	if (piped_string[1]==0)
		return 0;
	else
               return 1;
}

void parseSpace(char* str, char** path)
{
	int i;
	for (i=0;i<SIZE;i++)
	{
		path[i]=strsep(&str," ");
		if (path[i]==0)
			break;
		if (strlen(path[i])==0)
			i=i-1;
	}
}

void Assist()
{
	     printf("\n*WELCOME TO Our Linux SHELL Assist*"
		"\nSupported Command in Shell:"
		"\n>For Changing Directory= cd"
		"\n>For Files= ls"
		"\n>For Exit = exit"
		"\n>Other Basic Commands are  "
		"\n>Piping"
		"\n>Incorrect method for spc_handling ");
}
