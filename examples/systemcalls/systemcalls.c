#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int ret = system(cmd); 
    if (ret == 0) return true;
    else return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t pid; 
    fflush(stdout); 
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        //exit(EXIT_FAILURE); 
        return false;    
    }
    else if (pid == 0) { // fork() returns 0 in child process       
        printf("child process (PID : %d) executing command... \n", getpid());
        printf("Executing command: %s\n", command[0]);
        for (int i = 0; command[i] != NULL; i++) {
            printf("Arg[%d]: %s\n", i, command[i]);
        }

        if (execv( command[0], command) == -1){
            perror("execv failed with error");
            //exit(EXIT_FAILURE);
            return false;
        }
    }
    else {
        int status; 
        printf("Parent process (pid : %d) waiting for child to complete\n", getpid());
        if (waitpid(pid, &status, 0) == -1){
            perror("waitpid failed");
            //exit(EXIT_FAILURE);
            return false;
        }
        if (WIFEXITED(status)){
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
        else {
            printf("Child process did not exit successfully\n");
        }
    }
    va_end(args);

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    va_end(args);

    pid_t pid;
    int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    //int fd = creat(output, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); 
    if (fd < 0) { 
        perror("Open"); 
        //abort();
        return false;
    }
    if(dup2(fd, STDOUT_FILENO) == -1){
        perror("dup2, redirect failed");
        //abort();
        close(fd);
        return false;
    }
    close(fd);

    fflush(stdout);
    pid = fork();

    if(pid < 0){
        perror("fork");
        //abort();
        return false;
    }
    else if(pid == 0){
        if(execv(command[0], command) == -1){
            perror("execv failed");
            return false; 
        }
    } else {
        int status; 
        if(waitpid(pid, &status, 0) == -1){
            perror("waitpid failed");
            return false;
        }
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
    return true; 
}
