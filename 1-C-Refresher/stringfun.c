#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void  reverse_string(char *);
void  word_print(char *);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *ptr = user_str;
    int str_len = 0;
    int buff_len;
    int has_space = 1;
    
    ptr = user_str;

    while (*ptr != '\0') {
        if (str_len > len) {
            return -1;
        }

        // Filter extra white spaces by checking if we already encounted a white space prior to a new white space.
        if (*ptr == ' ' || *ptr == '\t') {
            if (!has_space) {
                has_space = 1;
                *buff = ' ';
                buff++;
                str_len++;
            }
        } else {
            has_space = 0;
            *buff = *ptr;
            buff++;
            str_len++;
        }
        ptr++;
    }

    // Remove ending white space in case there are trailing white spaces
    if (*(ptr - 1) == ' ') {
        str_len--;
        buff--;
    }
    
    buff_len = str_len;

    // Fill rest of buffer with '.'
    while(buff_len < len) {
        *buff = '.';
        buff++;
        buff_len++;
    }

    return str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int word_start = 0;
    char* ptr = buff;
    int wc = 0;

    // Counts word by keeping track of the white spaces and if we are in the middle of a word.
    for (int i = 0; i < len && i < str_len; i++) {
        char currChar = *ptr;
        if (!word_start) {
            if (currChar == ' ' || currChar == '\t') {
                continue;
            } else {
                wc++;
                word_start = 1;
            }
        } else {
            if (currChar == ' ' || currChar == '\t') {
                word_start = 0;
            }
        }
        ptr++;
    }

    return wc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *str){
    int end_idx = 0; 
    int start_idx = 0;
    char tmp_char;
    char *ptr = str;

    while (*ptr != '.' && *ptr != '\0') {
        ptr++;
        end_idx++;
    }

    end_idx--;

    // Swaps letter from both ends of string as move to the middle by moving the two pointers closer to eachother until they intersect.
    while (end_idx > start_idx) {
        tmp_char = *(str + start_idx);
        *(str + start_idx) = *(str + end_idx);
        *(str + end_idx) = tmp_char;
        start_idx++;
        end_idx--;
    }

    return;
}

void word_print(char *str){
    int wc = 0;       
    int wlen = 0;      
    int word_start = 0;    
    char* ptr = str;

    // As we count each word similar to count_words, we keep track of the number of letters in said word and print the information as we move to a new word.
    while (*ptr != '\0' && *ptr != '.') {
        char currChar = *ptr;
        if (!word_start) {
            if (currChar == ' ' || currChar == '\t') {
                continue;
            } 
            // This else block prints the necessary information for starting a new word.
            else {
                wc++;
                word_start = 1;
                wlen = 0;
                printf("%d. ", wc);
            }
        } else {
            // We print the number of letters in the current word we are parsing in this if block because we are moving to a new word.
            if (currChar == ' ' || currChar == '\t') {
                printf("(%d)\n", wlen);
                word_start = 0;
                wlen = 0;
            } 
            // This else block prints the character one by one of a word while keeping track of the number of letters.
            else {
                printf("%c", currChar);
                wlen++;
            }
            ptr++;
        }
    }

    // Prints out count for the last word in the string
    if (word_start) {
        printf("(%d)\n", wlen);
    }
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      We put a guard here to make sure that this program is taking at least 2 arguments with the second one being '-'. 
    //      This prevents our program from having any indexing error or issue finding the flag argument.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      We put another guard here to make sure that the arguments being passed is 3 or more so that the program can run as expected. 
    //      This also prevents an indexing error below where we want to access the 3rd argument as the input_string. 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error allocating memory");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff);
            break;
        
        case 'w':
            printf("Word Print\n----------\n");
            word_print(buff);
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("\nNumber of words returned: %d\n", rc);
            break;

        case 'x':
            if (argc < 5) {
                usage(argv[0]);
                exit(0);
            }
            printf("Not Implemented!\n");
            exit(3);
            break;
            
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          Passing in the length of the buffer is good practice, because without it, there is no way
//          for the function to know how many byte is allocated in the buffer. In this assignment, we know that we assigned
//          50 bytes to the buffer, but this will not always be case. It is true we can access the global variable to see what the size of the buffer is,
//          but in the case where we are manipulating the buffer size, the size can be hard to track. Adding the size of the buffer as we pass it to functions prevents any confusion.