/*
Task 2 of C Programming assignment 
Passowrd recovery tool by comparing hashes of users with the help of a wordlist containing hashes for commmon words
Author: Javier Yong 1726682 DISM/1A/21
        Ng Jun Xiang 1703151 DISM/1A/21
Last Updated: 23/12/2017
Estimated runtime: 4-10mins
*/
#include <stdio.h>
#include <stdlib.h> //malloc, free
#include <string.h> //for strcmp, strlen, strtok functions 
#include <time.h> //time.h header so that we can use the functions in the library to manipulate date and time  

#define MAXSIZE 1024 
/*
This program uses two linked lists, sha and word to store the data of shadow.txt and mytab2411.txt respectively.
Linked lists allows the program to insert and remove elements without reallocation of the entire structure compared to 
arrays
*/

//linked list used for shadow.txt file
struct shadow{
  //data for each node containing the uesr and hash in shadow file e.g usr=py1, hsh=to the hash that represent the user pyc1
  char *usr;
   char *hsh;
   struct shadow * nextnode; //pointer to the next node, in this case to the next user
};
typedef struct shadow sha;

//linked list used for mytab2411.txt file
struct wordlist{
  // data for each node containing the password and hash of the corresponding password in mytab21411.txt
  char *password;
  char *hashofpw;
  struct wordlist * nextword; //pointer to the next node, in this case to the next password
};
typedef struct wordlist word;

// display date and time to console 
void starttime();
void endtime();

//linked list management  
void appendlinkedlist(sha** head_ref, char *user, char *hash);
void appendwordlist(word** head_ref, char *password, char *hash);
void freeLinkedList(sha* head);
void freeWordList(word* head);

// compare hashes in linked list
void comparehash(sha *linkedlist,word *wordlist);

int main(int argc, char const *argv[]){
  
  starttime(); //call startime function to main, to display current time when user runs program.

  //declare two char array variables, shadowfilename and wordlistfilename, to store input from the two arguments in the command line. 
  char shadowfilename[MAXSIZE];
  char wordlistfilename[MAXSIZE];
  strcpy(shadowfilename,argv[1]); //strcpy function copies the string from argument 1 in command line to char array varaible shadowfilename
  strcpy(wordlistfilename,argv[2]);

  /*Declare and initialize two file pointers that stores the pointers returned by fopen
  fp pointing to the shadow.txt file and fp2 pointing to mytab2411.txt file 
  */
  FILE *fp=fopen(shadowfilename,"r"); 
  FILE *fp2=fopen(wordlistfilename,"r");

  // file validation, check if file exists on system 
  if(fp == NULL && fp2 == NULL){ //check if both files do not exist
    printf("Both files %s and %s cannot be found.\n",argv[1], argv[2]);
    printf("Please verify file name and try again\n");
    endtime();
  }

  else if(fp == NULL || fp2 == NULL){ //else if either files do not exist
    if(fp == NULL){ //if shadow file does not exist
      printf("Fatal error! shadow file:%s not found\n ", argv[1]);
      printf("Please verify file name and try again\n");
    }
    
    else if(fp2 == NULL){ //if wordlist file does not exist
      printf("Fatal error! wordlist file:%s not found\n", argv[2]);
      printf("Please verify file name and try again\n");
    }
    endtime();
  }

  else{ //else if both files are not NULL
    // declare two char pointers instead of char arrays as the length of string might vary
    char *user;
    char *hash;

    const char s[2] = ":"; //used later in strtok
    char str[MAXSIZE]; //str array declared to store each line of data in the file 
    sha* headoflinkedlist = NULL; //initializing an empty list to store contents of shadow file 

    //while loop used to traverse until end of shadow.txt file 
    while(EOF != fscanf(fp,"%[^\n]\n", str)){ //fscanf function reads the file until it reaches the '\n' character, this string is stored in str variable 
        user = strtok(str,s); //strtok is used separete into a series of tokens/sub-strings using the delimeter s 
        hash = strtok(NULL,s); //separate hash form the string and stored it in hash variable
        appendlinkedlist(&headoflinkedlist,user,hash); //append user and hash to linked list
    }
    
    fclose(fp); 
    //end of shadow file linked list management

    //linked list managemant for mytab2411.txt
    //these two char pointers will store the password and corresponding hash of password from str2
    char *pw;
    char *hash2;  
    word* headofwordlist = NULL; //initialize an empty list to store contents of wordlist file 
    char str2[MAXSIZE];

    while(EOF != fscanf(fp2,"%[^\n]\n", str2)){ 
        pw = strtok(str2,s); //store password string into pw variable by separating str2 using strtok 
        hash2 = strtok(NULL,s); //separate hash of corresponding password and store it in hash2 
        appendwordlist(&headofwordlist,pw,hash2);
    }  

    comparehash(headoflinkedlist,headofwordlist); 
    fclose(fp2);  

    freeLinkedList(headoflinkedlist);
    freeWordList(headofwordlist);
    endtime();

    return 0;
  }
} //end of main function

//this function calculates the system time when the program is executed and prints on the terminal
void starttime(){

    time_t timer; // declare the variable timer as a arithmetic time type
    char buffer[26]; // declare an array buffer of size 26
    struct tm* tm_info; // delcare tm_info as a simplified calender time type,year,month,day,hour,seconds,minute

    time(&timer); // returns current time of ubuntu system as a time_t value and store it into address of timer
    tm_info = localtime(&timer); // converts time_t value (&timer) and express as local time

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); // converts struct_tm object to year-month-date and hour-minutes-seconds and store it in the buffer array
    printf("Program started at %s\n",buffer); // print out the value of the buffer array
}

//this function calculate the systen time before the program exits and prints the time on terminal
void endtime(){
  
    time_t timer; // declare the variable timer as a arithmetic time type
    char buffer[26]; // declare an array buffer of size 26
    struct tm* tm_info; // delcare tm_info as a simplified calender time type,year,month,day,hour,seconds,minute

    time(&timer); // returns current time of ubuntu system as a time_t value and store it into address of timer
    tm_info = localtime(&timer); // converts time_t value (&timer) and express as local time

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); // converts struct_tm object to year-month-date and hour-minutes-seconds and store it in the buffer array
    printf("Program ended at %s\n",buffer); // print out the value of the buffer array
}

/*
appendlinkedlist function creates and append the node to the tail of the linked list called sha. 
if the sha is empty, it will set the head of the linked list to the first node that is created.
This function takes in a reference (pointer to pointer) to the head
of the linked list, sha, and the char pointer of both user and hash in shadow file 
*/
void appendlinkedlist(sha** head_ref, char *user, char *hash){
    //1. allocate memory to new_node
    sha* new_node = (sha*)malloc(sizeof(sha));
 
    sha *lastnode = *head_ref;  // creates a node that will be used to point to the last node in linked list in step 5

    //2. put in the data in new_node
    new_node->usr  = strdup(user);
    new_node->hsh = strdup(hash);
 
    /* 3. This new node is going to be the last node, so set nextnode of
    it as NULL. This is because we are always adding the new_node to the tail of the linked list*/
    new_node->nextnode = NULL;
 
    /* 4. If the Linked List is empty, then make the new node as head. This for the first node of the linked list */
    if (*head_ref == NULL){
       *head_ref = new_node;
       return;
    }
 
    /* 5. else traverse till the last node in linked list*/
   else{
       while (lastnode->nextnode != NULL){
        lastnode = lastnode->nextnode;
      }
    /* 6. Change the nextnode of last node */
      lastnode->nextnode = new_node;
      return;
  }
} //end of appendlinkedlist function

/*
appendwordlist function creates and append the node to the tail of the linked list called word. 
if the word is empty, it will set the head of the linked list to the first node that is created.
This function takes in a reference (pointer to pointer) to the head
of the linked list, sha, and the char pointer of both password and hash in shadow file 
*/
void appendwordlist(word** head_ref,char *password,char *hash){
    /* 1. allocate memory to new_node */
    word* new_node = (word*)malloc(sizeof(word));

    word *lastword = *head_ref;  // creates a node that will be used to point to the last node in linked list in step 5
 
    /* 2. put in the data into new_node*/
    new_node->password  = strdup(password);
    new_node->hashofpw = strdup(hash);
 
    /* 3. This new node is going to be the last node, so make nextword of
          it as NULL. This is because we are always adding the new_node to the tail of the linked list*/
    new_node->nextword = NULL;
 
    /* 4. If the Linked List is empty, then make the new node as head. This for the first node of the linked list*/
    if (*head_ref == NULL){
       *head_ref = new_node;
       return;
    }
 
    /* 5. Else traverse till the last node */
    else{
      while (lastword->nextword != NULL){
          lastword = lastword->nextword;
       }
    /* 6. Change the nextword of last node */
       lastword->nextword = new_node;
       return;
   } 
} //end of appendwordlist function

/*
comparehash function takes in the pointer of the head of both linked lists created earlier in the program in the function parameters
This function is used to check if the hash of each user in linkedlist is valid, and compare the hash of each user in linkedlist 
and the hash of each password in wordlist if the two values match.  
*/
void comparehash(sha *linkedlist,word *wordlist){

  int returnvalue; //used later in strcmp function
  int hashlen; //represent the length of the hash that is stored in the shadow file which correspond to eaach user

  word *tmp = wordlist; //create a tmp variable that contains all passwords and hashes in mytab2411.txt, as we do not want to modify the wordlist variable itself

  while(linkedlist != NULL){ //while loop used to iterate linkedlist until the last user of linkedlist
    hashlen = strlen(linkedlist->hsh); //calculate length of hash using strlen
    returnvalue = strcmp(linkedlist->hsh,tmp->hashofpw); //we are comparing the hash of the user in linkedlist and the hash of the corresponding password in wordlist

    if(returnvalue == 0){ //strcmp returns 0 only if both strings match, in this case if hash of user matches hash of password
      printf("user id : %s - password found => %s\n", linkedlist->usr,tmp->password); //prints the user and password of that user
      linkedlist = linkedlist->nextnode; //goes to next user 
      tmp = wordlist; //resets tmp pointer back to the head of wordlist
    }
    else{
      tmp = tmp->nextword; //traverse to next password
      if(tmp == NULL){ //after traversing to the next password, check if password exist or not
        if(hashlen != 26 && hashlen != 90){ //if hash is of invalid length
          printf("Invalid entry found in shadow file.(skipped)\n");
          tmp = wordlist; //resets tmp pointer back to the head of wordlist
          linkedlist = linkedlist->nextnode; //goes to next user
         } 
        else{
          printf("user id : %s - password <NOT FOUND>\n", linkedlist->usr);
          tmp = wordlist; //resets tmp pointer back to the head of wordlist
          linkedlist = linkedlist->nextnode; //goes to next user
        }//end of inner-else statement
      } 
    } //end of outer-else statement
  } //end of while-loop
} //end of comparehash function 

/* freeLinkedList deallocates memory that was previously allocated by the malloc call in 
appendlinkedlist. It takes it the head pointer of the linked list that is called sha.
*/

void freeLinkedList(sha* head){
   sha* tmp;

   while (head != NULL){
       tmp = head; 
       head = head->nextnode; //re-initialize head to be equal to next node until nextnode == NULL
       free(tmp); //realease node 
    }

}

/* freeWordList deallocates memory that was previously allocated by the malloc call in 
appendwordlist. It takes it the head pointer of the linked list that is called word.
*/

void freeWordList(word* head){
   word* tmp;
   while (head != NULL){
       tmp = head;
       head = head->nextword;
       free(tmp);
    }

}

/*
References:
 strtok function: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
 fscanf function to read file line by line: https://stackoverflow.com/questions/20108334/traverse-file-line-by-line-using-fscanf/20108623
 appendwordlist/appendlinkedlist function adapted from: https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/
 starttime and endtime function adapted from: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
 strftime function: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
*/
