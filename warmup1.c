#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cs402.h"
#include "my402list.h"
#include "myfunction.h"

int main(int argc, char *argv[])
{
	if(argc<2 || argc>3)
	{
		fprintf(stderr,"Invalid commandline due to inappropriate number of arguments!\n");
		fprintf(stderr,"Please type the valid commandline syntax: ./warmup1 sort [tfile]!\n");
		exit(EXIT_FAILURE);
	}	
	if(strcmp(argv[1], "sort")!=0)
	{
		fprintf(stderr,"Invalid commandline option!\n");
		fprintf(stderr,"Please type the valid commandline syntax: ./warmup1 sort [tfile]!\n");
		exit(EXIT_FAILURE);
	}
	
	FILE *file;
	if(argc==3)
	{
		file = fopen(argv[2],"rt");
		if(file==NULL)
		{
			perror("Failed to open file! Reason is");
			fprintf(stderr,"Please check if filename is valid or file exists or access is permitted!\n");
       		exit(EXIT_FAILURE);
		}
	}
	else file=stdin;

	char buffer[1030];
	My402List list;
	memset(&list, 0, sizeof(My402List));
	(void)My402ListInit(&list);
	int line=0;
	int is_valid=0;
	while(fgets(buffer,sizeof(buffer),file) != NULL)
	{
		line++;
		if(strlen(buffer)>1024)
		{
			fprintf(stderr,"Invalid tfile as line %d is too long!\n",line);
			is_valid=0;
			break;
		}
		if(buffer[strlen(buffer)-1]!='\n')
		{
			fprintf(stderr,"Invalid tfile as line %d is not ended with Enter!\n",line);
			is_valid=0;
			break;
		}
		buffer[strlen(buffer)-1]='\0';
		transaction* t=parse_line(buffer);
		if(t==NULL)
		{
			fprintf(stderr,"Failed to parse line %d as mentioned above!\n", line);
			is_valid=0;
			break;
		}
		else
		{
			is_valid=1;
			if(My402ListAppend(&list, t)==0)
			{
				fprintf(stderr,"Failed to append list!\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	if(is_valid==0)
	{
		if(My402ListLength(&list)==0)
		{
			fprintf(stderr,"File may not be in the right format!\n");
			fprintf(stderr,"Please check if file is a valid transaction file!\n");
		}
		exit(EXIT_FAILURE);
	}
	if(BubbleSortList(&list, My402ListLength(&list))==0) exit(EXIT_FAILURE);
	PrintList(&list);
	fclose(file);
	return 0;
}

