#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]){
  char message[8001];
  int server, portNumber;
  struct sockaddr_in servAdd;    
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber); 
  servAdd.sin_port = htons((uint16_t)portNumber); //multibyte conversion from host to network

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n"); //presentation to network conversion
  exit(2);
}

  
 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }

 while(1){
  
  fprintf(stderr, "Enter Command or type quit to exit from the Server\n");
  fgets(message, 8000, stdin);
  

  write(server, message, strlen(message)+1);
  
   if(strcmp("quit\n",message)==0)
   {
	    close(server);
    exit(0);
   }
  
  char jp;
  char buff[1024];
  int i=0;
  while(1)
  {
  if (!read(server, &jp, 1)){
	   close(server);
    fprintf(stderr, "Error in Reading the response\n");
    exit(0); 
 }
		buff[i]=jp;
		i++;
		if(jp=='\n')
		{
			buff[i-1]='\0';
			i=0;
		}
   fprintf(stderr, "%c", jp);
   if(strcmp("Done",buff)==0)
   {
	    fprintf(stderr, "\n");
	   break;
   }
  }
}
}


