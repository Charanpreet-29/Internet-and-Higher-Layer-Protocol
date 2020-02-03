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
#include<time.h>
#include <sys/time.h>
#include<arpa/inet.h>
#define Listenpower 999999
#define RCVMSGSIZE 1024
#define Port1 80
int net; /*Global varible to check Block websites*/
char str[50];/*Global declaration to compare the host for website block*/
int doparse(char functnmsg[],char *host,char *request)
{
        char text[5000];
        char host2[500];
        bzero(text,5000);
	strcpy(text,functnmsg);  /* copying function into text */
        char *req1 = strstr(text," "); /* reading string from first space */
	int reqlen = strcspn(req1,"H"); /* trying to read it till first H and then substracting length by -1  to get the REQUEST */
	bzero(request,reqlen-1);
	strncpy(request,req1,reqlen-1);  /* copying in request in the function  */
        char *host1 = strstr(text,"Host: ");/* reading string from the Host:  */
        int hostlen = strcspn(host1,"\r\n");/* reading till /r/n in the http request */              
        bzero(host2,hostlen);
	strncpy(host2,host1,hostlen);                        
	char *hst = strchr(host2, ' '); /*  to read teh hostname from the string */ 
        hst++; /* to read from the next point  */
       	int len;
        len = strlen(hst);
        hst[len] = '\0';
        char hostval[500];
        bzero(hostval,500);
        strcpy(hostval,hst);
        strcpy(host,hostval);
	return 0;
}


int dns(char functnmsg1[], char addr[])
    {       
        char copymsg[5000];
        char copyl[5000];
	struct timeval timv1, timv2;
	double tm,tp=9;
	bzero(copymsg,5000);
	strcpy(copymsg,functnmsg1);
        bzero(copyl,5000);
        int x=1;
        int result;
        char cpyhstnme[5000],temp[5000],ipstore[50],host2[5000],prefername[5000]; /*creating various character arrays to store message*/
        bzero(temp, 500);
	bzero(ipstore,50);	
        struct addrinfo *addrpt , *addrvalue ,addrfam;           /*declaring variable and pointer pointing to address of addrinfo*/
        addrfam.ai_family = AF_INET;                             /*IPV4 address family*/
        addrfam.ai_socktype = SOCK_STREAM;                       /*TCP*/
        bzero(&addrfam,sizeof(addrfam));                          
        result = getaddrinfo(functnmsg1, NULL, &addrfam, &addrpt);   /* getting address from ANSI host name*/
             if (result != 0)                            /*if getaddrinfo success provides 0 so if no IP address found for host name then non zero returned*/
                {       
	            char *pa;
                    bzero(functnmsg1,5000);
                    pa = "( NO IP Addresses Found \n)";
                    strcpy(functnmsg1,pa);                    /*sending msg back to send on failure and exiting dns function */
                    strcpy(addr,pa);
                    bzero(temp,5000);
                    return 0;
                }
        bzero(cpyhstnme,5000);
        bzero(prefername,5000);
	int socksm = socket(AF_INET, SOCK_STREAM, 0);
		for (addrvalue = addrpt; addrvalue != NULL; addrvalue = addrvalue->ai_next)  /*loop to read ip address stored 4 hostname one by one,ai_next defined in in_addr*/
                {                        
                    void *ptr;
			gettimeofday(&timv1, NULL);
                    	int con = connect(socksm, (struct sockaddr*) &addrfam, sizeof(addrfam));
			gettimeofday(&timv2, NULL);
			if(con==0) {
			tm=(double) (timv2.tv_usec - timv1.tv_usec) / 1000000 + (double) (timv2.tv_sec - timv1.tv_sec);
                    ptr = &((struct sockaddr_in *) (*addrvalue).ai_addr)->sin_addr;      /*pointer that is storing string represented of ip address*/
                    inet_ntop((*addrvalue).ai_family, ptr, host2, 10000);                       /*converting IP  numeric address to string address*/
  if(tm<tp)
	{	
   if((*addrvalue).ai_protocol==6 && (*addrvalue).ai_family ==AF_INET && x==1) /*for preferred id checking for prototcol,IPv4 address andfirst value */
            {
                sprintf(addr,"%s",host2);   
		bzero(ipstore,50);
		strcpy(ipstore,host2);
		continue;                     
            }                     
                }
	}
}
	x++;        
        freeaddrinfo(addrpt);          /* to free the address information that getaddrinfo dynamically allocates in addrinfo structures */      
        bzero(host2,5000);
	strcat(host2,ipstore);
	return 0;
}


int dohttp(const char *blocksite, const char *str)
{
	net=1;/*initializing the value to compare if no blocked sites are found*/
	FILE *blockfile; /*variabe in which the file is opened that contains the blocked sites*/
	char store[200]; /*storing the contents of the file to compare with the host*/
	bzero(store,200);
	if((blockfile = fopen(blocksite, "r")) == NULL)/*Checking the file */
	 {
	return(-1);
	 }
	while(fgets(store, 200, blockfile) != NULL)/*reading the file till the end*/
	 {
		if((strstr(store, str)) != NULL)/*Comparing the value of the string with the file*/
	 {
		net=0;/*changing the variable value to block the website*/
	 }
	 }
		if(blockfile)/*closing the file if it is open*/
		 {
			fclose(blockfile);
	 	 }
   			return(0);
}



int main(int argc, char* argv[])
{      
	int binding,portnumber,listening,i = 1;
	portnumber = atoi(argv[1]);        /*atoi for convert string to integer*/
	struct sockaddr_in clntaddr;       /*declaring variable for client of structure type sockaddr*/  
	struct sockaddr_in socpart;                  /*declaring variable for server of structure type sockaddr*/
    struct sockaddr_in clientaddr;
	socpart.sin_port = htons( portnumber);       /*convert IP port number of host byte order to IP port number of network byte order*/
	socpart.sin_family = AF_INET;                /* IPV4 address family */
	socpart.sin_addr.s_addr = htonl(INADDR_ANY);  /*allows any incoming interface */
	int rval=0;
	if (argc == 1)   /* if no port number supplied dynamically gives error */
	{
		printf("\nPORT NUMBER NOT GIVEN, PLEASE GIVE THE PORT NUMBER");
		exit(1);
	}
	int dnserverid = socket(AF_INET, SOCK_STREAM, 0);  /* creating server socket*/ 
	if (dnserverid<0)                              /*checking for socket*/
		{	
			printf("\nSocket error \n ");
		}                
	binding = bind(dnserverid, (struct sockaddr *) &socpart, sizeof(socpart));  /*creating bind server to local address */
	if (binding<0)
	{
		printf("\nBind error \n ");
		exit(0);
	}                 
	listening = listen(dnserverid, Listenpower); /*server listening to any incoming connection from client */
	if (listening<0)
		printf("\nListen error \n ");
	else
		printf("\nProxy Server by Group W2 (ckd22@njit.edu) listening on Port Number ( %s ) \n", argv[1]);
	for (;;)                                                             /*loop so that it can run forever*/
	{
		for (;;)                                                     /*infinite for loop*/
		{
			int s,re,wri;                                                   
			long int r;
			char message [65535];
                        char address[500];
                        char message1 [5000];
                        char messageget[5000];
			bzero(messageget,5000);
                        char message2[5000];
                        memset(message2,0,sizeof(message2));
			bzero(message,65535);
                        bzero(message1,5000);
                        char valhost[500];
                        char gethost[500];                  
			bzero(valhost,500);
                        bzero(gethost,500);
                        socklen_t clntlen;                       /*clntlen of type socklen_t as defined in structure*/
			clntlen = sizeof(clntaddr);              //size of client
		        close(s);                                                                /*closing previos open socket*/
			s = accept(dnserverid, (struct sockaddr *)&clntaddr, &clntlen);  /*Accepting request from client*/
			if (s<0){
				printf("\nError in accept");}
                         
			r = read(s, message, RCVMSGSIZE);        /*receiving message from client */ 
			if (r<0)
			{   
				printf("\nError in recieve");
				break;
			}
if (r==0)/* checking to see if no bytes are read in the receive function*/
{
wri=write(s,"<html>\n<body>\n<strong><h1>404 NOT FOUND</h1></strong><br>\n<code><h2>Website not found</h2></code><br>\n</body>\n</html>",100);	
break;
}
                        rval=rval+r;
			if(rval>65535)/*checking to see if the request is greater than 65535 bytes*/
			{
			printf("\nMessage request exceeds 65535 bytes");
			close(s);
			rval=0;
			}
                        strcpy(message2,message);
                        doparse(message2,valhost,gethost);
                        printf("\n(%d) REQUEST: %s \n",i,gethost);
                        i++;
                        bzero(address,500);
                        dns(valhost,address);
			            dohttp(argv[2],valhost);
                   	    clientaddr.sin_port = htons(Port1);       /*convert IP port number of host byte order to IP port number of network byte order*/
                        clientaddr.sin_family = AF_INET;                /* IPV4 address family */
                        inet_pton(AF_INET,address,&(clientaddr.sin_addr.s_addr));
                        int clientid = socket(AF_INET, SOCK_STREAM, 0);  /* creating  socket*/
			            if (clientid<0)                              /*checking for socket*/
	                      {
                             	printf("\nSocket error \n ");
                              }
                        int status=connect(clientid, (struct sockaddr*) &clientaddr, sizeof(clientaddr));/*connecting to the web server */
                            if(status!=0)
                                {
                                  printf("\nConnect Error");
                                }
                        int w= write(clientid,message2,sizeof(r)); /* writing the HTTP request to the web server receieved from the browser*/                                                     
                             if(w<0) 
                                 {
                                   printf("\nError in read");
                                 }
if(net==0) /* code for blocked websites */
{
wri=write(s,"<html>\n<body>\n<strong><h1>403 Forbidden</h1></strong><br>\n<code><h2>This website is blocked</h2></code><br>\n</body>\n</html>",100);
}
	if(net==1) /* code for non blocked websites */
		{
                    do
                         { 
				re=read(clientid,messageget,sizeof(messageget));  /* to reading the http response for the given request */
				wri= write(s,messageget,re); /* writing the HTTP response for re bytes read from the web server*/
                         }
				 while(re!=0 && wri!=0);    /* to read and write till the re and wri values turn to be 0 */
		}
		}
	}
		close(dnserverid);
}
