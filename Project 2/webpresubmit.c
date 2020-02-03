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
#define RCVMSGSIZE 65535


int doparse(char functnmsg[],char *host,char *request)
{

 
        char text[5000];
        char copyl[5000];
        bzero(text,5000);
        bzero(copyl,5000);
	strcpy(text,functnmsg);
        bzero(functnmsg,5000);
  
	int getlength = strcspn(text,"\r\n");
	request[getlength];
	strncpy(request,text,getlength);                     
        //strncpy(functnmsg,request,getlength);
         
        char *cpy;
        char *hostp = strstr(text,"Host: ");
        int hostlen = strcspn(hostp,"\r\n");                
	host[hostlen];
        bzero(host,hostlen);
	strncpy(host,hostp,hostlen);                        
      
       
        char *UserA = strstr(text,"User");
        int userlen = strcspn(UserA,"\r\n");
	char User[userlen];
        bzero(User,userlen);
	strncpy(User,UserA,userlen); 
        strncpy(functnmsg,User,userlen);                    
      
   
        char *Accept1 = strstr(text,"Accept: ");
        int Acceptlen = strcspn(Accept1,"\n");
	char Accept2[Acceptlen];
        bzero(Accept2,Acceptlen);
	strncpy(Accept2,Accept1,Acceptlen);               
        //strncpy(functnmsg,Accept2,Acceptlen);
  
        char *Connect1 = strstr(text,"Connection: ");
        int Connectlen = strcspn(Connect1,"\n");
	char Connect2[Connectlen];
        bzero(Connect2,Connectlen);
	strncpy(Connect2,Connect1,Connectlen);            
       // strncpy(functnmsg,Connect2,Connectlen);
            
           char pa[10]="<br>";
           
           strcpy(copyl,request);   
         
           strcat(copyl,pa);
           strcat(copyl,User);
           strcat(copyl,".");   
           strcat(copyl,pa);
           strcat(copyl,host); 
         
           strcat(copyl,pa);
           strcat(copyl,Accept2);
         
           strcat(copyl,pa);
           strcat(copyl,Connect2);
         
           bzero(functnmsg,5000);
           strcpy(functnmsg,copyl);
         
           
        //return 0;

}



int dns(char functnmsg1[])
    {       
        char text[5000];
        char copyl[5000];
        bzero(text,5000);
	strcpy(text,functnmsg1);
        bzero(functnmsg1,5000);

        //char *hostp = strstr(text,"Host: ");
        //int hostlen = strcspn(hostp,"\r");
	//char host[hostlen];
        //bzero(host,hostlen);
      	//strncpy(host,hostp,hostlen);
        char *hostpointer = strchr(text, ' ');
        hostpointer++;
        int cutlen;
        cutlen = strlen(hostpointer);
	hostpointer[cutlen] = '\0';
        char hostval[200];
        strcpy(hostval,hostpointer);

       // printf("\n 1 2 3 4  %s \n\n",hostval);
        int x=1;
        int result;
        char cpyhstnme[5000],temp[5000],name[5000],host2[5000],prefername[5000]; /*creating various character arrays to store message*/
        bzero(temp, 500);	
        struct addrinfo *addrpt , *addrvalue ,addrfam;           /*declaring variable and pointer pointing to address of addrinfo*/
        addrfam.ai_family = AF_INET;                             /*IPV4 address family*/
        addrfam.ai_socktype = SOCK_STREAM;                       /*TCP*/
        
        result = getaddrinfo(hostval, NULL, &addrfam, &addrpt);   /* getting address from ANSI host name*/
            if (result != 0)                                 /*if getaddrinfo success provides 0 so if no IP address found for host name then non zero returned*/
                {       
	            char *pa;
                    bzero(functnmsg1,5000);
                    pa = "( NO IP Addresses Found \n)";
                    //printf("%s",hostval);
                    strcpy(functnmsg1,pa);                    /*sending msg back to send on failure and exiting dns function */
                    bzero(temp,5000);
                    return 0;
                }
        bzero(cpyhstnme,5000);
        bzero(prefername,5000);
		for (addrvalue = addrpt; addrvalue != NULL; addrvalue = addrvalue->ai_next)  /*loop to read ip address stored 4 hostname one by one,ai_next defined in in_addr*/
                {                        
                    void *ptr;
                    ptr = &((struct sockaddr_in *) (*addrvalue).ai_addr)->sin_addr;      /*pointer that is storing string represented of ip address*/
                    inet_ntop((*addrvalue).ai_family, ptr, host2, 10000);                       /*converting IP  numeric address to string address*/
                    //sprintf(name,"IP = %s. \n",host2);                                            /*printing IP address */
                    if((*addrvalue).ai_protocol==6 && (*addrvalue).ai_family ==AF_INET && x==1) /*for preferred id checking for prototcol,IPv4 address andfirst value */
                        {
                           sprintf(prefername," (%s)\n",host2);      /*storing preferred ip in prefername*/
                           x++;
                        }
                    //strcat(cpyhstnme,name);                                    /*concatenate ip address to cpyhstname */
                }
        //strcat(cpyhstnme,prefername);  /*from all ip address in cpyhstname concatenating with functnmsg which is to be send to client */
        char hostIP[20]="HostIP = ";
        strcat(hostIP,hostval);
        strcat(hostIP,prefername);
        strcat(functnmsg1,hostIP); 
        freeaddrinfo(addrpt);          /* to free the address information that getaddrinfo dynamically allocates in addrinfo structures */      
        return 0;
    }

int dohttp(char functnmsg2[],char *host1,char *request2)
{
    char pa[5]="<br>";
     char port[12]="PORT = 80";
    char path[10]="PATH =  ";
    //printf("\n%s\n",functnmsg2);
    strcat(functnmsg2,pa);
    strcat(functnmsg2,pa);
    strcat(functnmsg2,host1);
    strcat(functnmsg2,pa);
    strcat(functnmsg2,port);
    strcat(functnmsg2,pa);
    strcat(functnmsg2,path);
    strcat(functnmsg2,request2);
    //strcat(functnmsg2,'\0');
   //int lenn=strlen(functnmsg2);
     //functnmsg2[lenn+1]='\0';
    		
 
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
		printf("Proxy Server Listening on socket %s \n", argv[1]);
	for (;;)                                                             /*loop so that it can run forever*/
	{
		for (;;)                                                     /*infinite for loop*/
		{
			int s;                                                   
			long int r;
			char message [5000];
                        char messagecpy[5000];
                        char getcpy[5000];
			bzero(message,5000);
                        bzero(messagecpy,5000);
                        bzero(getcpy,5000);
                        char valhost[200];
                        char gethost[300];                   /*places null bytes in message string helping clear previous buffer*/
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
		
                        
                        doparse(message,valhost,gethost);
                        //printf("\n 3 %s \n",message);
                        memcpy(messagecpy,message,strlen(message));
                        bzero(message,5000);
                        memcpy(getcpy,gethost,strlen(gethost));
                   
			dns(valhost);
                   
                        //printf("\n 4  %s \n",messagecpy);
                        //printf("\n %s \n",valhost);
                        //printf("\n %s \n",getcpy);
                   
                        printf(" (%d) REQ : %s \n",i,getcpy);
                        i++;
                        dohttp(messagecpy,valhost,getcpy);          
			
                        
                                                 						   /*calling DNS function*/
			if (r > 0)
			{
				if (send(s,messagecpy, RCVMSGSIZE, 0) != RCVMSGSIZE) /*sending reply back to client received from dns function*/
				{
					printf("send failed");
					exit(0);
				}
				else
				{
					bzero(message,5000);
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

