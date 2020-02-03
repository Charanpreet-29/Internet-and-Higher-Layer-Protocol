#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 
#include<netdb.h> 
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#define _POSIX_C_SOURCE 200809L
int main(int argc,char* argv[])   
{       
       if (argc!=5)   
             {
                printf("Arguments Missing: Please enter all the arguments and execute the program again\n");
                return 0;  
              }
        int portnumber, delay, buffersize=1024, n=0;
        char buffer[buffersize]; 
        long ms;
        struct timespec t;
        portnumber = atoi(argv[2]);    
        delay=atoi(argv[4]);
        ms = (1000000*delay);
        t.tv_nsec = ms;
        FILE *filename = NULL;
        filename = fopen(argv[3], "rb");
        struct sockaddr_in socpart;
        socpart.sin_port = htons(portnumber); 
        socpart.sin_family = AF_INET;  
        inet_pton(AF_INET,argv[1],&(socpart.sin_addr.s_addr));
        int dnserverid = socket(AF_INET, SOCK_DGRAM, 0);
         if (dnserverid<0)
           {           	
               printf("Socket error \n ");
	  return 0;  
           }
        if (filename != NULL) 
                {   
                     for(;;)
		    { 
                                         do 
                                              {
			          n = fread(buffer, 1, buffersize, filename); 
			          sendto(dnserverid, buffer, n, 0, (struct sockaddr *) &socpart, sizeof(socpart));
		 	           nanosleep(&t,&t);  
                                               }
		    while(n>0);
		    if(n<=0)
			{
                                                        printf("No more data to send. Closing program.");
				break;	
                                            }
                    fclose(filename);    
                    close(dnserverid); 
                                      }
                         }
            return 0;
    }
