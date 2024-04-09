#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

int main()
{
    /* The command buffer */
    string cmdBuff;
    
    /* The ID of the child process */
    pid_t pid;
    
    /* Keep running until the user has typed "exit" */
    do 
    {
        /* Prompt the user to enter the command */
        cerr << "cmd>";
        cin >> cmdBuff;
        
        /* If the user wants to exit */
        if(cmdBuff != "exit")
        {
            /* Create a child */
            pid = fork();
            
            /* Error check to make sure the child was successfully created */
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
                
            /* If I am child, I will do this: */
            if (pid == 0) {
                /* Call execlp() to replace my program with that specified at the command line */
                execlp(cmdBuff.c_str(), cmdBuff.c_str(), NULL);
                
                /* Error check for execlp */
                perror("execlp");
                exit(EXIT_FAILURE);
            }
            
            /* If I am a parent, I will do the following */
            /* Wait for the child process to terminate */
            if (wait(NULL) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
        }
    }
    while(cmdBuff != "exit");
    
    return 0;    
}
