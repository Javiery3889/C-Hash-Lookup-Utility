/*
Task 1 of C Programming assignment 
Cryptographic hash lookup generator (Simplified Version)
Author: Javier Yong 1726682 DISM/1A/21
        Ng Jun Xiang 1703151 DISM/1A/21
Last Updated: 23/12/2017
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <ctype.h>
#include <time.h>

void starttime();
void endtime();
void words(FILE * filename,int maxsize,int minsize);
int main(int argc, char const *argv[]){
	
	starttime(); // calling starttime function

	char filename[256]; //declare a char array variable to store the value of the wordlist file name in the command line argument
	strcpy(filename,argv[1]);
	int minsize = atoi(argv[2]);
	int maxsize = atoi(argv[3]);

	FILE *file = fopen(filename,"r");

	if(file == NULL){
		printf("Fatal error! %s not found\n ", argv[1]);
	}
	else if (minsize > maxsize){
		printf("Usage : ./task1 <wordlist> <min> <max>\n\t<wordlist> : A file path/name in which contains the password dictionary\n\t<min> : An integer value greater than 1.\n\t\tThis value represents the minimum length of the password\n\t<max> : An integer value greater than or equal to <min>.\n\t\t<max> represents the maximum length of the password.\n");
	}
	else{ //if file is valid
	words(file,maxsize,minsize); 
	}

	endtime(); // calling endtime function
	return 0; // end of the program
}

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
word function takes in a file pointer of the wordlist file in the function parameters, this function hashes each word in the wordlist file using 
the crypt library, each word is hash to MD5 and SHA-512 until it reaches to the end of the file
*/
void words(FILE * filename,int maxsize,int minsize){  
	char * hash_type_1 = "$1$";	 // type 1 implies md5 (number of iteration is only 1000 rounds)
    char * hash_type_2 = "$6$";	 // type 2 implies sha-512 (default value as in yr 2017, number of iteration is minimum 10,000 rounds )
	char * salt_1 ="$";			  // a simplified 0 length salt.
	char * result;
	char encyption_scheme[20];	// declare an array of size 20 to store the encryption type for md5 later
	char encyption_scheme2[20];	// declare an array of size 20 to store the encryption type for sha-512 later
	int count=0;
	char sizeofline[50];
	strcpy(encyption_scheme,hash_type_1); // copy value of hashtype into encyptionscheme
	strcpy(encyption_scheme2,hash_type_2);
	strcat(encyption_scheme,salt_1);	// add "$" into value of encryptionscheme
	strcat(encyption_scheme2,salt_1);
	FILE *fp =fopen("mytab2411.txt","w"); // Open mytab2411.txt and allow writing permission, able to change the content of file
	while((fgets(sizeofline,sizeof(sizeofline),filename)) != NULL){
	// To read the file line by line from "smallwordlist until the last line. 
	//A null pointer is returned when EOF is encountered and no more characters can be read
		int len = strlen(sizeofline); 
		if(len > minsize && len < maxsize){ //if the length of word is in the range of the min and max value specified in command line
		//finding the last character 
		while( len>0 && isspace(sizeofline[len-1])){ // Searches for "\n" (last character in the password)
			// printf("%d\n",len); This would print the length of the first password .. Examaple C\n   (two character)
			len--; 
			// printf("%d\n",len); This would print the new length of the first password , which is now C "one character"
			// It then returns to the while loop
			// Since len-1 is now just C , it does not fulfil the isspace() requirement and therefore exits the while loop
		}
		// Since the program nows know where the last character is, it thus makes the last character a '\0' value

		sizeofline[len]='\0';

		result = crypt(sizeofline,encyption_scheme); // store the encrypted value into result using md5 encryption
		fprintf(fp,"%s:%s\n",sizeofline,result);// print the result into mytab2411.txt file
		
		result = crypt(sizeofline,encyption_scheme2);// store the encrypted value into result using sha-512 encryption
		fprintf(fp,"%s:%s\n",sizeofline,result);// print the result into mytab2411.txt file
		count++; // count the numbers of line in small_wordlist.txt
		}
		else{
			continue;
		}
	}	
	printf("Total number of words processed\t=>%d\n",count ); // number of line in small wordlist
	printf("Total number of generated entries =>%d\n",count*2); // number of encrypted passwords in both sha-512 and md5
	
}

/*
References:
 starttime and endtime function adapted from: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
 strftime function: https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
 isspace function: https://www.tutorialspoint.com/c_standard_library/c_function_isspace.htm
 crypt library and crypt function adapted from mkpassword.c
 fgets function used for reading file: https://stackoverflow.com/questions/9434207/reading-from-file-using-fgets
*/
