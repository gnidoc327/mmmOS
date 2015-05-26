#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define DELIMS " \t"

struct childList{
	int pid;
	char cmd[1024];
};

pid_t child=-1;                                            
struct childList list[100];
int count=0;
bool cmd_cd(int argc, char *argv[]);          
bool cmd_exit(int argc, char *argv[]);        
bool cmd_help(int argc, char *argv[]); 
bool cmd_jobs(int argc, char *argv[]);
bool pipes(char *line);

struct command{ 
	char *name;
	char *desc;
	bool (*func)(int argc, char *argv[]); 
};

struct command  commandList[] = {
	{ "cd",		"change directory",		cmd_cd   },
	{ "jobs",	"show process list",	cmd_jobs},
	{ "exit",   "exit this shell",      cmd_exit  },
	{ "help",	 "show this help",      cmd_help }};

bool cmd_cd(int argc, char* argv[]){
	if(argc == 1)
		chdir(getenv("HOME"));
	else if( argc == 2 )
		if(chdir(argv[1]))
			printf( "No directory\n" );
	else
		printf( "USAGE: cd [dir]\n" );
}

bool cmd_exit(int argc, char* argv[]){
	printf("###\tExit Shell Good Bye~!\t###\n");
	exit(0);
	return true;
}

bool cmd_help(int argc, char* argv[]){ 
	int i;
	for(i = 0;i < sizeof(commandList)/sizeof(struct command);i++)
		if( (argc == 1) || !strcmp(commandList[i].name, argv[1]))
			printf("%s\t\t: %s\n", commandList[i].name, commandList[i].desc );
	return true;
}

bool cmd_jobs(int argc, char *argv[]){
	int i=0, j=0;
	int status;
	struct childList tempChildList[100];

	for(i=0, j=0;i<count;i++){
		waitpid(list[i].pid, &status, WNOHANG);
		if(waitpid(list[i].pid, &status, WNOHANG)){
			printf("[%d]\t%s\t%s\n", i, list[i].cmd, "Done");
			continue;
		} else if(WIFSTOPPED(status)){
			printf("[%d]\t%s\t%s\n", i, list[i].cmd, "Stop");
		} else {
			printf("[%d]\t%s\t%s\n", i, list[i].cmd, "Running");
		}
		tempChildList[j].pid = list[i].pid;
		strcpy(tempChildList[j++].cmd, list[i].cmd);
	}
	list[i].pid = 0;
	for(i=0;i<j;i++){
		list[i].pid = tempChildList[i].pid;
		strcpy(list[i].cmd, tempChildList[i].cmd);
	}
	count = j;
	
	return true;
}

int lineSplit(char *line, char *token[], int maxTokens){
	int token_count = 0;
	char *temp; 
	char *tempBuf;


	temp = strtok_r(line, DELIMS, &tempBuf);
	while( temp != NULL && token_count < maxTokens ){
		token[token_count++] = strdup(temp);
		temp = strtok_r(NULL, DELIMS, &tempBuf);
	}
	token[token_count] = NULL;

	return token_count;
}

bool run(char *line){
	char *token[100];
	char *temp;
	char buf[1024];
	int token_count;
	int i;
	bool back=false;
	int status;
	bool pipeCheck=false;
	int fd[2];


	if(line == NULL) false;

	for(i=0;i<strlen(line);i++){ 
		if(line[i] == '&'){
			back=true;
			line[i]='\0';
			break;
		} else if(line[i] == '|'){
			line = strdup(strtok_r(line, "|", &temp)); 
			pipe(fd);
			pipeCheck = true;;
			break;
		}
	}

	token_count = lineSplit(line, token, sizeof(token)/sizeof(char*));
	
	if(token_count == 0)	return true;
	for(i = 0;i < sizeof(commandList)/sizeof(struct command);i++ ){
		if(!strcmp(commandList[i].name, token[0])) 
			return commandList[i].func( token_count, token );
	}
	
	child = fork();
	
	list[count].pid = child;
	strcpy(list[count++].cmd, line);

	if(child == 0){ // child
		if(pipeCheck){
			return true;
		}
		execvp(token[0], token);
	} else if(child < 0){ //error
		printf("Failed to fork()!");
		exit(0);
		return false;
	} else if(back == false){ // parent
		if(pipeCheck){
			run(temp);
		}
		waitpid(child, &status, WUNTRACED);
	} 
	return true;
}

int main(){
	char line[1024];
	
	
	while(1){
		printf("OS/mmmOS%s $ ", (char*)get_current_dir_name());
		fgets( line, sizeof( line ) - 1, stdin );
		line[strlen(line)-1] = '\0';
		if(run(line) == false)
			break;
	}
	return 0;
}






