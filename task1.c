#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <ctype.h>
#include <time.h>


void starttime();
void endtime();
void words(FILE * filename);
int main(int argc, char const *argv[]){
	
	starttime();
	char filename[256];
	int returnvalue;
	strcpy(filename,argv[1]);
	int num2 = atoi(argv[2]);
	int num3 = atoi(argv[3]);

	FILE *file = fopen(filename,"r");

	if(file == NULL){
		printf("Fatal error! %s not found\n ", argv[1]);
	}
	else{
		words(file);
	}

	endtime();
	return 0;
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

void words(FILE * filename){
	char * hash_type_1 = "$1$";	 // type 1 implies md5 (number of iteration is only 1000 rounds)
    char * hash_type_2 = "$6$";	 // type 2 implies sha-512 (default value as in yr 2017, number of iteration is minimum 10,000 rounds )
	char * salt_1 ="$";			  // a simplified 0 length salt.
	char * result;
	char encyption_scheme[20];
	char encyption_scheme2[20];
	int count=0;
	char sizeofline[50];
	strcpy(encyption_scheme,hash_type_1);
	strcat(encyption_scheme,salt_1);
	strcpy(encyption_scheme2,hash_type_2);
	strcat(encyption_scheme2,salt_1);
	FILE *fp =fopen("mytab2411.txt","w");
	while((fgets(sizeofline,sizeof(sizeofline),filename)) != NULL){
		int len = strlen(sizeofline);
		//finding the last character 
		while( len>0 && isspace(sizeofline[len-1])){
			len--;
		}

		//making the last character '\0'
		sizeofline[len]='\0';

		result = crypt(sizeofline,encyption_scheme);
		fprintf(fp,"%s:%s\n",sizeofline,result);
		// md5
		// prepare the second call using sha-512 and a 8-char salt
		
		result = crypt(sizeofline,encyption_scheme2);
		//sha-512
	
		fprintf(fp,"%s:%s\n",sizeofline,result);
		count ++;
	}	
	printf("Total number of words processed\t=>%d\n",count );
	printf("Total number of generated entries =>%d\n",count*2);
	
}
