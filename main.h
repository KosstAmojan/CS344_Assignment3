// Name: Matthew Norwood
// OSU Email: norwooma@oregonstate.edu
// Course: CS344 - Operating Systems
// Assignment: 3
// Due Date: 2/8/22
// Description: This is the header file to be used by main.c


#ifndef ASSIGNMENT3_MAIN_H
#define ASSIGNMENT3_MAIN_H
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>
#include <unistd.h>

// Macro definitions here //
#define MAX_LINE 2048

// Struct definitions here //

ssize_t pidDigitCount(pid_t process_id){
    ssize_t pidDigitCnt = 0;

    // count the number of digits in the shell_process_id
    while (process_id > 1) {
        pidDigitCnt++;
        process_id /= 10;
    }

    return pidDigitCnt;
}

char* pidConvert(pid_t process_id){
    char *pidString = NULL;

    ssize_t pidDigitCnt = 0;
    pidDigitCnt = pidDigitCount(process_id);

    // convert pid to string
    pidString = malloc(sizeof(size_t) * pidDigitCnt + 1);
    if (sprintf(pidString, "%d",process_id) < 0) {
        perror("sprintf pidString error: ");
    }

    return pidString;

}

void processUserInput(const char *inputString, size_t bytes_read){

    if (strncmp("exit", inputString, bytes_read) == 0) {
        // run the exit command.
    }


}

void pidExpand(char* inputString, size_t pidDigitCnt, const char *pidString){

    char *inputStringCpy = malloc(sizeof(*inputString));
    int n = 0, i = 0, j = 0, expand_done = 0;

    // copy contents of old string;
    strcpy(inputStringCpy, inputString);

    // copy character by character from the old string until we get to the symbols $$, then copy from pidString
    while (inputStringCpy[j] != '\0'){
        if (inputStringCpy[j] == '$' && expand_done == 0){
            if (inputStringCpy[n + 1] == '$') {
                for (i = 0; i < pidDigitCnt; i++) {
                    inputString[n] = pidString[i];
                    n++;
                }
                j += 2;
                expand_done++;
                continue;
            }
        }
        inputString[n] = inputStringCpy[n];
        n++;
        j++;
    }

}

void shellUserInput(void) {
    // This function, called by main.c, handles basic user input/option selection. It then calls the appropriate
    // helper functions as necessary to complete file selection and processing for the user.

    while (1) {

        char **usr_input = malloc(sizeof(char *) * MAX_LINE);
        char *usr_input_str = malloc(sizeof(char) * MAX_LINE);
        char *pidString = NULL;
        int n = 0, i = 0;
        size_t max_line1 = MAX_LINE, *max_line = &max_line1, bytes_read = 0, alloc_mem = sizeof(char) * MAX_LINE,
                            pidDigitCnt = 0;
        pid_t shell_process_id = getpid();
        printf(":");

        if ((bytes_read = getline(usr_input, max_line, stdin)) < 0) {
            perror("Error reading from getline");
            }

        // Clear stdin after each read from it.
        fflush(stdin);

        if (bytes_read == 0) {
            continue;
        }

        if ((*usr_input)[bytes_read - 1] == '\n') {
            (*usr_input)[bytes_read -1] = '\0';
        }

        strcpy(usr_input_str, *usr_input);

        // Strip leading spaces
        while (usr_input_str[0] == ' '){
            usr_input_str++;
            bytes_read--;
        }

        // Comment line, ignore
        if (usr_input_str[0] == '#') {
            continue;
        }

       while (usr_input_str[n] != '\0'){
           if (usr_input_str[n] == '$') {
               if (usr_input_str[n+1] == '$'){
                   pidDigitCnt = pidDigitCount(shell_process_id);
                   pidString = pidConvert(shell_process_id);

                   // determine if appending expanded pid to usr_input_str will result in an overflow, if it does, realloc inputString.
                   if ((strlen(pidString) + strlen(usr_input_str) - 1) > alloc_mem) {
                       char *tempString = realloc(usr_input_str, (strlen(pidString)
                                                    + strlen(usr_input_str) - 1));
                       usr_input_str = tempString;
                    }
                   pidExpand(usr_input_str, pidDigitCnt, pidString);
                   bytes_read = strlen(usr_input_str);
               }
           }
           n++;
       }

        processUserInput(usr_input_str, bytes_read);
        printf("The string the user entered was: '%s' and it was %d bytes long.\n", usr_input_str, (int)bytes_read);

        i++;
        if (i == 10) {

            free(pidString);
            free(*usr_input);
            free(usr_input_str);
            free(max_line);
            break;
        }

    }
}



#endif //ASSIGNMENT3_MAIN_H