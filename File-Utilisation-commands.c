#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h> 
#include <unistd.h> /*              */
#include <string.h> /* String Library*/
#include <stdlib.h> /* Standard Library */


/* Andrew Lee 30864941, Start date of modification: 24th of August 2022.
                        End date of modification: 27th of August 2022.

30864941_fileutil.c is a simplistic substitute of the following commands: 
head,tail,cat and cp. the 30864941_fileutil.c program allows user to perform numerous operations such as Append and Viewing Source/Destination files 
*/

/* 
This specific program does not utilise <stdio.h>, system() and any form of process spawning such as spawn.h and is purely a program 
that interacts with the low level OS directly. Furthermore, any form of input/output is directly being called to the terminal through 
Standard or Error Stream. Therefore, there will not be any prompt/scan/print user input. It is assumed that the user will directly 
provide numerous arguments and custom commands according to the specification of the program. 
*/

/*
Limitations: 
There are certain limitations in the program, such as displaying the amount of words which is purely based on the delimiter space. 
The first argument will always be compilation gcc equivalent of ./a.out, therefore the first argument will always compile the program 
instead of running custom functions that is user input. 
Every filepath name would be considered in absolute path as stated in the assumptions of the given assignment. 
*/


/* Function List   */
void CustomViewAndAppend(int infile,char *buffer,int argc,char *argv[]); 
void CustomViewOrAppend(int infile,char *buffer,int argc, char *argv[]);  
void SampleDisplay(int infile,char *buffer,int argc,char *argv[]); 
void SourceDisplay(int infile,char *buffer,int argc,char *argv[]);  
void DisplayOrAppendSample(int infile,char *buffer,int argc,char *argv[]); 

/* Main Function */
int main(int argc,char *argv[]){
        int infile,outfile,count=0,num,def;
        char buffer[1000],first,second;
	
	if(argc==1){	//Functionality based on the amount of arguments. 
	SampleDisplay(infile,buffer,argc,argv);  //Display Sample Text
	}
	if(argc==2){   
	SourceDisplay(infile,buffer,argc,argv);  //Display Text based on user input sourcefile
	}

	if (argc==3){
	DisplayOrAppendSample(infile,buffer,argc,argv); //Display or Append Sample.txt based on destination
	}

	if ((infile = open(argv[1],O_RDONLY))<0){ //Used for CustomViewAndAppend and CustomViewOrAppend Function, Setting the Argument on User input at index 1
                exit(0);
        };

	if(argc==6){
		CustomViewAndAppend(infile,buffer,argc,argv); //CustomViewAndAppend will append the sourcefile to the destination file
	}//and display the appending content on terminal based specific limitation on word input. 
                
	if(argc==4){
		CustomViewOrAppend(infile,buffer,argc,argv); //CustomViewOrAppend based on user input
	}//will either Append or give a Custom View of the content specified in sourcefile.
	
        close(infile); //closing all the files that has been used
        close(outfile); 
        exit(0); //exit 0 if there is no erroneous error from the system. 

}

void CustomViewAndAppend(int infile,char *buffer,int argc,char *argv[]){  
	int num; //required condition
	int count = 0; //incrementation of word count
	char first,second; //comparison of buffer and delimiter, more specifically space. 
	int outfile; //
	if(strcmp(argv[2],"-a")==0){ 	//String comparison of user input content argument 2
                        if(*argv[3]== '/'){   //pointer of content argument 3 to absolute path 
                            outfile = open(argv[3],O_RDWR|O_APPEND|O_CREAT,0777); 	//variable outfile with the list of conditions:ReadOnly,Append,Create with r-w-x priviledges
 							num = atoi(argv[5]); 	//conversion of string to integer function atoi();
                                while((read(infile,buffer,1))&&count<num){ 	//reading 1 byte at a time of which the user input is always greater than count
                                    write(outfile,buffer,1); 	//write to variable outfile, which is the destination file from the user input
                                    write(1,buffer,1); 	//write to terminal to give an overview of content being appended
                                    first = *buffer; //points to buffer
                                    second = ' '; 	//delimiter space, this is a limitation as the program will only look for space. 
                                        if(first == second){ 	//if there is a match, it is considered as one word. 
                                        	count++; 	//increment count every time there is a match. 
                                                 }

                               		 }
					                        
						
                        }else if(*argv[3] != '/'){
							write(2,"invalid path",26);	//if the condition of which the following has not been satisfied, it is considered invalid
						}
	}else if(strcmp(argv[2],"-n")==0){ //the logic of which the first explanation follows exactly except for displaying
                    	num = atoi(argv[3]); //user input of content argument 3 will be converted to an integer		
							outfile = open(argv[5],O_RDWR|O_APPEND|O_CREAT,0777); //this will open the final argument of user input
                            while((read(infile,buffer,1))&&count<num){   //read 1 byte that will be stored into the buffer from infile, and count condition remains the same
                                write(outfile,buffer,1); //write the following to outfile with the same condition of string comparison.
                                first = *buffer; //this will allow the user to specify how many words of which will be appended to the destination file
                                second = ' '; 
                                    if(first == second){
                                    count++;
                                        }
                                        
                            }
                        
                }
		
}

void CustomViewOrAppend(int infile,char *buffer,int argc,char *argv[]){ 
	int num; 
	int count = 0;
	char first,second; 
	int outfile;

	if(strcmp(argv[2],"-n")==0){ //if the position of -n is right after the source file
		num = atoi(argv[3]); //the input following -n will have to be a number
            while((read(infile,buffer,1))&&count<num){ //same condition from line 101 to 113.
                write(1,buffer,1); //the logic for CustomViewOrAppend does not require 6 arguments, but only 4, of which -n is purely displaying.
                    first = *buffer; 
                    second = ' ';
                        if(first == second){
                            count++;
                        }
                                        
             }
						
	}if(strcmp(argv[2],"-a")==0){ //the position of -a has to be the second argument 
				if(*argv[3]== '/'){  //if the following argument is an absolute path as previously discussed 
                    outfile = open(argv[3],O_RDWR|O_APPEND|O_CREAT,0777); //this will open textfile if it exist, or create it if it does not and append
                    while((read(infile,buffer,1))&&count<10) { //this case is only for 10 words 
                            write(outfile,buffer,1); //same logic 
                                first = *buffer;
                                second = ' ';
                                    if(first == second){
                                        count++;
                                    }

                    }
                }else{
					write(2,"Argument invalid",26);	//if the condition does not apply, return an argument invalid standard error stream.
					}

		}

}
void SampleDisplay(int infile,char *buffer,int argc,char *argv[]){ //this function does not require multiple inputs, one user input is for compilation.
    int count = 0;
    char first,second;

	infile = open("sample.txt",O_RDONLY); //source file has already been hard coded

	while((read(infile,buffer,1))&&count<10){ //the only requirement is to run the source code using ./fileutil command
		write(1,buffer,1);
		first = *buffer;
		second = ' ';
			if(first == second){
				count++;
			}
	}
}

void SourceDisplay(int infile,char *buffer,int argc,char *argv[]){ //this function will specify a specific location in order to open and read.
	char first,second;
	int count = 0;	
	if((infile = open(argv[1],O_RDONLY))<0){  //before writing it to standard terminal, this requires the argument right after ./fileutil
        	exit(0);	           
    }
        while((read(infile,buffer,1))&&count<10){
                write(1,buffer,1);
                first = *buffer;
                second = ' ';
                    if (first == second){
                        count++;	
			}
	}
}

void DisplayOrAppendSample(int infile,char *buffer,int argc,char *argv[]){ //DisplayOrAppendSample 
	char first,second; 
	int count = 0;
	int num,outfile;
	infile = open("sample.txt",O_RDONLY); //Functionality of DisplayOrAppend Function is to either display through comparing user input to -n or -a.
	if(strcmp(argv[1],"-n")==0){ //Therefore, the sourcefile will alawys be sample.txt 
		num = atoi(argv[2]); //if it is -n, same logic as previously discussed will apply here 
            while((read(infile,buffer,1))&&count<num){
                write(1,buffer,1);
                first = *buffer;
                second = ' ';
                    if(first == second){
                        count++;
                    }
                                        
            }
						
	}if(strcmp(argv[1],"-a")==0){ //if it is -a, functionality of appending logic will be applied as well. 
		if(*argv[2]== '/'){ 
        	outfile = open(argv[2],O_RDWR|O_APPEND|O_CREAT,0777);
            while((read(infile,buffer,1))&&count<10){
                write(outfile,buffer,1);
            	first = *buffer;
                second = ' ';
                	if(first == second){
                        count++;
                    }
            }

        }
	}
  
} 
//END