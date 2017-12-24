/*
Task 2 of C Programming assignment 
Passowrd recovery tool by comparing hashes of users with the help of a wordlist containing hashes for commmon words
Author: Javier Yong 
Last Updated: 23/12/2017
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

struct shadow{
  //data for each node containing the uesr and hash in shadow file e.g usr=py1, hsh=to the hash that represent the user pyc1
  char *usr;
   char *hsh;
   struct shadow * nextnode; //pointer to the next node, in this case to the next user
};
typedef struct shadow sha;

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

  /*As fopen() function returns a FILE pointer, we declare and initialize two file pointers that stores the pointers returned by fopen
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
    while(EOF != fscanf(fp,"%[^\n]\n", str)){ //fscanf function reads the file until it reaches the '\n' character, this is string is stored in str variable 
        user = strtok(str,s); //strtok is used separete into a series of tokens/sub-strings using the delimeter s 
        hash = strtok(NULL,s); //separate hash form the string and stored it 
        appendlinkedlist(&headoflinkedlist,user,hash); //append user and hash to tail of linked list
    }
    
    fclose(fp); //close shadow.txt file as we do not need it 
    //end of shadow file linked list management

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
    fclose(fp2);  //close mytab2411.txt file as we do not need it

    freeLinkedList(headoflinkedlist);
    freeWordList(headofwordlist);
    endtime();

    return 0;
  }
} //end of main function

void starttime(){

  time_t rawtime; //declare a time_t variable called rawtime which stores system time values
  char buffer[26]; //store system time into a char array 
  struct tm* tm_info; //structure used to hold time and date


  time(&rawtime); 
  tm_info = localtime(&rawtime); // uses the time pointed by rawtime to fill a tm structure with the values that represent the corresponding local time

  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); /*formats the time represented in the structure tm_info
  according to the formatting rules defined in format and stored into buffer.*/

  printf("Program started at ");
  puts(buffer);
} //end of starttime function

void endtime(){
  //code similar to startime function
  time_t rawtime;
  char buffer[26];
  struct tm* tm_info;

  time(&rawtime);
  tm_info = localtime(&rawtime);

  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  printf("Program ended at ");
  puts(buffer);

} //end of endtime function 

//comparehash function used to compare the hash of a user to all the hashes found in mytab2411.txt
void comparehash(sha *linkedlist,word *wordlist){
  int returnvalue; //used later in strcmp function
  int hashlen; //represent the length of the hash that is stored in the shadow file which correspond to eaach user

  word *tmp = wordlist; //create a tmp variable that contains all passwords and hashes in mytab2411.txt, as we do not want to modify the wordlist variable itself

  while(linkedlist != NULL){ //while loop used to iterate linkedlist until there are no more users in shadow.txt file
    hashlen = strlen(linkedlist->hsh); //calculate length of hash using strlen
    returnvalue = strcmp(linkedlist->hsh,tmp->hashofpw); //strcmp compares the two strings to see if they match and returns an integer value
    //in this case we are comparing the hash of the user in shadow.txt and the hash of the corresponding password in mytab2411.txt

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
        }
      } 
    } //end of outer-else statement
  } //end of while-loop
} //end of comparehash function 

/*this function is used to append a node containing the user and password in shadow.txt, 
and also the pointer to the next node/user to tail of linked list called sha*/

void appendlinkedlist(sha** head_ref, char *user, char *hash)
{
    /* 1. allocate node */
    sha* new_node = (sha*)malloc(sizeof(sha));
 
    sha *last = *head_ref;  // used in step 5*/
 
    /* 2. put in the data  */
    new_node->usr  = strdup(user);
    new_node->hsh = strdup(hash);
 
    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
    new_node->nextnode = NULL;
 
    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->nextnode != NULL)
        last = last->nextnode;
      
    /* 6. Change the next of last node */
    last->nextnode = new_node;
    return;
} //end of appendlinkedlist function

/*this function is used to append a node containing the password and hash in mytab2411.txt, 
and also the pointer to the next node/password to tail of linked list called word*/
void appendwordlist(word** head_ref,char *password,char *hash)
{
    /* 1. allocate node */
    word* new_node = (word*)malloc(sizeof(word));
 
    word *last = *head_ref;  // used in step 5*/
 
    /* 2. put in the data  */
    new_node->password  = strdup(password);
    new_node->hashofpw = strdup(hash);
 
    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
    new_node->nextword = NULL;
 
    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->nextword != NULL)
        last = last->nextword;
      
    /* 6. Change the next of last node */
    last->nextword = new_node;
    return;
} //emd of appendwordlist function

void freeLinkedList(sha* head){
   sha* tmp;

   while (head != NULL){
       tmp = head; 
       head = head->nextnode; //re-initialize head to be equal to next node until nextnode == NULL
       free(tmp); //realease node 
    }

}

void freeWordList(word* head){
   word* tmp;
   while (head != NULL){
       tmp = head;
       head = head->nextword;
       free(tmp);
    }

}
