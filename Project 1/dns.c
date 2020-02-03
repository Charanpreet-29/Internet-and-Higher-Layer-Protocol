                     /* Comment Block
              CS656 Group W2- ProjectNW1            TEAM MEMBERS: 
			CHARANPREET KAUR DHIR(ckd22) (TEAM LEADER), SOUMYADEEP BASU (sb2356) ,SAYANTAN MAITY (sm2734)
			AKHIL MANGALAMADAKA(am2825) PRUTHVI CHINTAKAYALA(pc422)	ROHINI PUVVULA(rp787)   */   
			
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define Listenpower 999999
#define RCVMSGSIZE 1024

int dns(char functnmsg[])
    {
        int x=1,result;
        char cpyhstnme[70000],temp[70000],name[70000],host[70000],prefername[70000]; /*creating various character arrays to store message*/
        bzero(temp, 500);	
        struct addrinfo *addrpt , *addrvalue ,addrfam;     /*declaring variable and pointer pointing to address of addrinfo*/
        addrfam.ai_family = AF_INET;              /*IPV4 address family*/
        addrfam.ai_socktype = SOCK_STREAM;                       /*TCP*/
        memset(&addrfam, 0, sizeof (struct addrinfo));          /*memset used to zero out the structure*/
        memcpy(temp,functnmsg, strlen(functnmsg) - 2);         /*copying functn msg to temp with stringlength -2 for removing null value*/
        bzero(functnmsg, 70000);                               /*clearing the value of functnmsg to zero*/
        result = getaddrinfo(temp, NULL, &addrfam, &addrpt);   /* getting address from ANSI host name*/
            if (result != 0)                                 /*if getaddrinfo success provides 0 so if no IP address found for host name then non zero returned*/
                {       
	            char *pa;
                    bzero(functnmsg, 70000);
                    pa = "NO IP Addresses Found \n";
                    strcpy(functnmsg,pa);                    /*sending msg back to send on failure and exiting dns function */
                    bzero(temp, 500);
                    return 0;
                }
        bzero(cpyhstnme,70000);
        bzero(prefername,70000);
		for (addrvalue = addrpt; addrvalue != NULL; addrvalue = addrvalue->ai_next)  /*loop to read ip address stored 4 hostname one by one,ai_next defined in in_addr*/
                {                        
                    void *ptr;
                    ptr = &((struct sockaddr_in *) (*addrvalue).ai_addr)->sin_addr;      /*pointer that is storing string represented of ip address*/
                    inet_ntop((*addrvalue).ai_family, ptr, host, 10000);                       /*converting IP  numeric address to string address*/
                    sprintf(name,"IP = %s. \n",host);                                            /*printing IP address */
                    if((*addrvalue).ai_protocol==6 && (*addrvalue).ai_family ==AF_INET && x==1) /*for preferred id checking for prototcol,IPv4 address andfirst value */
                        {
                           sprintf(prefername,"Preferred IP = %s. \n",host);      /*storing preferred ip in prefername*/
                           x++;
                        }
                    strcat(cpyhstnme,name);                                    /*concatenate ip address to cpyhstname */
                }
        strcat(cpyhstnme,prefername);  /*from all ip address in cpyhstname concatenating with functnmsg which is to be send to client */
        strcat(functnmsg,cpyhstnme); 
        freeaddrinfo(addrpt);          /* to free the address information that getaddrinfo dynamically allocates in addrinfo structures */      
        return 0;
    }
	
int main(int argc, char* argv[])
    {      
        int binding,portnumber,listening,i = 1;
        portnumber = atoi(argv[1]);        /*atoi for convert string to integer*/
        struct sockaddr_in clntaddr;       /*declaring variable for client of structure type sockaddr*/  
        struct sockaddr_in socpart;                  /*declaring variable for server of structure type sockaddr*/
        socpart.sin_port = htons( portnumber);       /*convert IP port number of host byte order to IP port number of network byte order*/
        socpart.sin_family = AF_INET;                /* IPV4 address family */
        socpart.sin_addr.s_addr = htonl(INADDR_ANY);  /*allows any incoming interface */
          if (argc == 1)   /* if no port number supplied dynamically gives error */
            {
                printf("PORT NUMBER NOT GIVEN, PLEASE GIVE THE PORT NUMBER");
                exit(1);
            }
        int dnserverid = socket(AF_INET, SOCK_STREAM, 0);  /* creating server socket*/ 
        if (dnserverid<0)                              /*checking for socket*/
        printf("Socket error \n ");                
        binding = bind(dnserverid, (struct sockaddr *) &socpart, sizeof(socpart));  /*creating bind server to local address */
        if (binding<0)
            {
                printf("Bind error \n ");
                exit(0);
            }                                         
        listening = listen(dnserverid, Listenpower); /*server listening to any incoming connection from client */
        if (listening<0)
               printf("Listen error \n ");
        else
               printf("DNS Server Listening on socket %s \n", argv[1]);
	    for (;;)                                                             /*loop so that it can run forever*/
            {
                for (;;)                                                     /*infinite for loop*/
                    {
                        int s;                                                   
                        long int r;
                        char message [70000];
                        bzero(message, 70000);                   /*places null bytes in message string helping clear previous buffer*/
						socklen_t clntlen;                       /*clntlen of type socklen_t as defined in structure*/
                        clntlen = sizeof(clntaddr);              //size of client
                        close(s);                                                                /*closing previos open socket*/
                        s = accept(dnserverid, (struct sockaddr *)&clntaddr, &clntlen);          /*Accepting request from client*/
                        if (s<0)
                            printf("error in accept");
                       
                        r = recv(s, message, RCVMSGSIZE, 0);        /*receiving message from client */ 
                        if (r<0)
                          {
                             printf("error in recieve");
                             exit(0);
                          }
                        else
                          {
                            printf("%d] Incomming client connection from ",i);			            /*printing message at client side*/
                            printf(" [%s : %d] ",inet_ntoa(clntaddr.sin_addr),ntohs(clntaddr.sin_port));  /*getting client ip address and port number*/
                            printf("to me [%s:%d] \n",inet_ntoa(socpart.sin_addr),atoi(argv[1]));  /*getting server ip address and port number*/
                            printf("REQ : %s \n",message);
                            i++; 
                          }
						dns(message);									   /*calling DNS function*/
                        if (r > 0)
                          {
                            if (send(s, message, RCVMSGSIZE, 0) != RCVMSGSIZE) /*sending reply back to client received from dns function*/
                                {
                                   printf("send failed");
                                   exit(0);
                                }
                             else
                                {
                                   continue;
                                }
                          }
                        else
                          {
                                printf("problem with send");
                          }
					}
			}
			close(dnserverid);
}
