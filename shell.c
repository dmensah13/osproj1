#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/limits.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128
//Worked with Bria Fassler and Nia Anderson

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;
pid_t pid;
int *status;


int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
      
        do{ 
            // Print the shell prompt.
          
            char cwd[PATH_MAX];
            if(getcwd(cwd,sizeof(cwd)) != NULL){
              printf("%s>",cwd);
            }
            //printf("%s", prompt);
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
        
        // 1. Tokenize the command line input (split it on whitespace)
        int n = 0;
        arguments[n] = strtok(command_line,delimiters);
        
        while(arguments[n] != NULL){
          char *str = arguments[n];
          if(strchr(str,'$')!=NULL){
            arguments[n]=getenv(str+1);
          }
          
          arguments[n+1] = strtok(NULL,delimiters);
        }
      
        // 2. Implement Built-In Commands
        if(strcmp(arguments[0],"pwd")==0){
          char cwd[PATH_MAX];
          if(getcwd(cwd,sizeof(cwd))!=NULL){
            printf("%s\n",cwd);
          }
        }
      
        else if(strcmp(arguments[0],"cd")==0){
          printf("Directory Chaneed to: %s\n", arguments[1]);
          chdir(arguments[1]);
        }
        
        else if(strcmp(arguments[0],"echo")==0){
          int iter = 1;
          while(arguments[iter]!= NULL){
            printf("%s",arguments[iter]);
            iter += 1;
          }
          printf("\n");
        }
        
        else if(strcmp(arguments[0],"setenv")==0){
          setenv(arguments[1], arguments[2],1);
        }
        
        else if(strcmp(arguments[0],"exit")==0){
          return 0;
        }
      
        else if(strcmp(arguments[0],"env")==0){
          int iter2 = 1;
          char *e = *environ;
          for(;e;iter2++){
            printf("%s\n",e);
            e = *(environ + iter2);
          }
        }
        // 3. Create a child process which will execute the command line input
        else{
          pid = fork();
          if(pid < 0){
            printf("ERROR IN FORK\n");
            exit(1);
          }
          else if(pid == 0){
            char *args[] = {arguments[0],arguments[1],arguments[2],NULL};
            execvp(args[0],args);
            exit(0);
            
            if(strcmp(arguments[1],"&") == 0){
              char *args[] = {arguments[0],NULL};
              execvp(args[0],args);
            }
         
          }
        }
  
        // 4. The parent process should wait for the child to complete unless its a background process
        pid = wait(status);
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}