#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXSIZE 1024
struct shadow{
   char *usr;
   char *hsh;
   struct shadow * nextnode;
};
typedef struct shadow sha;

struct wordlist
{
   char *password;
   char *hashofpw;
   struct wordlist * nextword;
};
typedef struct wordlist word;

void starttime();
void endtime();

void appendlinkedlist(sha** head_ref, char *user, char *hash);
void appendwordlist(word** head_ref, char *password, char *hash);
void printLinkedlist(sha *linkedlist);
void comparehash(sha *linkedlist,word *wordlist);
void printWordlist(word *linkedlist);
void freeLinkedList(sha* head);
void freeWordList(word* head);

int main(int argc, char const *argv[])
{
  starttime();

  char shadowfilename[MAXSIZE];
  strcpy(shadowfilename,argv[1]);
  char wordlistfilename[MAXSIZE];
  strcpy(wordlistfilename,argv[2]);

  FILE *fp=fopen(shadowfilename,"r");
  FILE *fp2=fopen(wordlistfilename,"r");

  if(fp == NULL && fp2 == NULL){
    printf("Both files %s and %s cannot be found.\n",argv[1], argv[2]);
    printf("Please verify file name and try again\n");
  
  }
  else if(fp == NULL || fp2 == NULL){
    if(fp == NULL){
    printf("Fatal error! %s not found\n ", argv[1]);
   printf("Please verify file name and try again\n");
    }
    else if(fp2 == NULL){
      printf("Fatal error! %s not found\n", argv[2]);
      printf("Please verify file name and try again\n");
    }
  }

  else{
   char *user;
   char *hash;
   
   const char s[2] = ":";
   char str[256];

   sha* headoflinkedlist = NULL;

   while(EOF != fscanf(fp,"%[^\n]\n", str)){
      user = strtok(str,s);
      hash = strtok(NULL,s);
      appendlinkedlist(&headoflinkedlist,user,hash);
      }
     
   fclose(fp);
  
  char *pw;
  char *hash2;  
   word* headofwordlist = NULL;
   char str2[MAXSIZE];

   while(EOF != fscanf(fp2,"%[^\n]\n", str2)){
      pw = strtok(str2,s);
      hash2 = strtok(NULL,s);
      appendwordlist(&headofwordlist,pw,hash2);
   }  
   comparehash(headoflinkedlist,headofwordlist);
   fclose(fp2);
   freeLinkedList(headoflinkedlist);
   freeWordList(headofwordlist);
   endtime();

  return 0;
  }
}

void starttime(){
  time_t rawtime;
  char buffer[26];
  struct tm* tm_info;

    time(&rawtime);
    tm_info = localtime(&rawtime);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Program started at ");
    puts(buffer);

}

void endtime(){
  time_t rawtime;
    char buffer[26];
  struct tm* tm_info;

    time(&rawtime);
    tm_info = localtime(&rawtime);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Program ended at ");
    puts(buffer);

}

void comparehash(sha *linkedlist,word *wordlist){
  int returnvalue;
  int hashlen;
  word *tmp = wordlist;
  while(linkedlist != NULL){
    hashlen = strlen(linkedlist->hsh);
    returnvalue = strcmp(linkedlist->hsh,tmp->hashofpw);
    if(returnvalue == 0){
      printf("user id : %s - password found => %s\n", linkedlist->usr,tmp->password);
      linkedlist = linkedlist->nextnode;
      tmp = wordlist;
    }
    else{
      tmp = tmp->nextword;
      if(tmp == NULL){
        if(hashlen != 26 && hashlen != 90){
         printf("Invalid entry found in shadow file.(skipped)\n");
          tmp = wordlist;
         linkedlist = linkedlist->nextnode;
       } 
        else{
        printf("user id : %s - password <NOT FOUND>\n", linkedlist->usr);
        tmp = wordlist;
        linkedlist = linkedlist->nextnode;
        }
      }
    }
  }
}

void printLinkedlist(sha *linkedlist){
      while(linkedlist!=NULL){
         printf("User:%s\t\t Hash:%s\n", linkedlist->usr, linkedlist->hsh);
         linkedlist = linkedlist->nextnode;
      }   
   
   printf("\n");
}
 
 void printWordlist(word *linkedlist){
      while(linkedlist!=NULL){
         printf("Password:%s\t\t Hash:%s\n", linkedlist->password,linkedlist->hashofpw);
         linkedlist = linkedlist->nextword;
      }   
   
   printf("\n");
}

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
}

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
}

void freeLinkedList(sha* head)
{
   sha* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->nextnode;
       free(tmp);
    }

}

void freeWordList(word* head)
{
   word* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->nextword;
       free(tmp);
    }

}
