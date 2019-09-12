#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void child(int);


void serviceClient(char* inputstring) 
{ 
while(1)
{
    int counter=0;
	int flag=0;
	char commandList[200][100];
	char IdNum[200];
	int i=0;
	char reg=';';
    while(counter <= strlen(inputstring)) 
    {		
		if(inputstring[counter]==';'|| inputstring[counter]=='&' || inputstring[counter]=='|')
		{	
			if(inputstring[counter]=='&')
			{
				reg='&';
			}
			else if(inputstring[counter]=='|')
			{
				reg='|';
			}
			else{
				reg=';';
			}
			if(inputstring[counter]=='&' && inputstring[counter+1]=='&' )
			{
				
			}
			else if(inputstring[counter]=='|' && inputstring[counter+1]=='|' )
			{
				
			}
			else{
				commandList[flag][i]='\0';
				IdNum[flag]=reg;			
			    flag++;
			 i=0;
			}
		}
		else{
			commandList[flag][i]=inputstring[counter];
			i++;
			
		}
		counter++;      
    }
	IdNum[flag]=';';
			

for(i=0;i<=flag;i++)
{
      printf("\nCommand: %s\n", commandList[i]);
      char *args[10] = {}; 
      args[0] = strtok(commandList[i], " ");
      int tokenCounter = 0;
      
      while (args[tokenCounter] != NULL){
        tokenCounter++;
        args[tokenCounter] = strtok(NULL, " ");
      }

      int processid = fork();

      if (processid == 0){
        if ((execvp(args[0], args)) < 0){
          printf("Please Enter valid command\n");
		  exit(-1);
        }
        exit(0);
      }
	  else if (processid > 0){  	  
        int status;       
        wait(&status);
        if(WIFEXITED(status))
		{
          int exitstat = WEXITSTATUS(status);
		  if(exitstat ==0)
		  {
          		   if(IdNum[i]=='|')
			  {
				  i=flag+1;
			  }
		  }		  
			else
			{
			 printf("Error with command");
			  if(IdNum[i]=='&')
			  {
				  i=flag+1;
			  }
			}
        }
		else
		{
			printf("signaled by =%d\n",WTERMSIG(status));
		}
      }else{
        printf("some unwanted error\n");
        exit(1);
      }
      
    } 
 break;
}	
  
}
int main(int argc, char *argv[]){  
  int sd, client, portNumber;
  struct sockaddr_in servAdd;    
  
 if(argc != 2){
    printf("Please Give port Number with %s\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    printf("Client Request Accepted\n");

    if(!fork())
      child(client);

    close(client);
  }
}
void child(int sd){
	char message[8000];


	while(1){
		fprintf(stderr, "waiting to get command\n");
		

		if(!read(sd, message, 255)){
			close(sd);
			fprintf(stderr, "no client command to handle waiting for new client\n");
			exit(0);
		}
		
		fprintf(stderr, "Client requeted command %s", message);
		dup2(sd,1); //control of the server screen is forwarded to descripter
		 printf("Running commandList\n");
		 int len=strlen(message);
		 message[len-1]='\0';
		 fprintf(stderr, "Running command: (%s)\n", message);
		 
		serviceClient(message);
		printf("\n");
		 printf("Done\n");
		
	}
}
