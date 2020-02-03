/* Group Members
      Charanpreet Kaur Dhir - ckd22@njit.edu (Group Leader)
      Soumyadeep Basu - sb2356@njit.edu
      Sayantan Maity - sm2734@njit.edu
  Akhil Mangalamadaka - am2825@njit.edu
 Pruthvi Chintakayala - pc422@njit.edu
 Rohini Puvvula - rp787@njit.edu */


/* header files */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define Listenpower 99999
#define RCVBUFSIZE 1024

int main(int argc,char* argv[])
{

/*creating socket */
   int socketid;
   socketid=socket(AF_INET,SOCK_STREAM,0);
   if(socketid<0)
   printf("Socket error \n ");
   //else printf("socket working \n");

   struct in_addr                                                       /* hold 32 bit host id */
        {
                unsigned long s_addr;
        };


   struct sockaddr_in                                                   /* holds socket information */
        {
                unsigned short sin_family;
                unsigned short sin_port;
                struct in_addr sin_addr;
        };

 /* for client addr */
   struct sockaddr_in clntaddr;
   struct sockaddr_in addrport;

   addrport.sin_family=AF_INET;
   addrport.sin_port=htons(8294);
   addrport.sin_addr.s_addr=htonl(INADDR_ANY);

   int binding;                                                 /* creating bind for server */
   binding=bind(socketid,(struct sockaddr *) &addrport,sizeof(addrport));
   if(binding<0)
    printf("Bind error \n ");
   //else printf("Bind working \n");

   int listn;                                                   /*creating listen for server */
   listn=listen(socketid,Listenpower);
   if(listn<0)
      printf("Listen error \n ");
   else
      printf("DNS Server Listening on socket %s \n",argv[1]);
 for(;;)                                                       /*infinite for loop */
    {
      int s;                                                    /*Accepting request from client */                                                 $
      int i;
      int clntlen;
      clntlen=sizeof(clntaddr);
      s=accept(socketid,( struct sockaddr *)&clntaddr,&clntlen);
      if(s<0)
        printf("error in accept");
      else
        {
          //printf(" accept working number \n");
          i++;
         }

      // printf("ip is %c ",inet_ntoa(clntaddr.sin_addr));

       char buffer[RCVBUFSIZE];
       long int r;
      char  echobuffer[70000];
       bzero(echobuffer,70000);
       r=recv(s,echobuffer,RCVBUFSIZE,0);               /* receiving from socket */
       if(r<0)
         {
           printf("error in recieve");
           exit(0);
          }
       else
          {
           //printf(" recieve  working\n");
           printf("%d] Incoming client connection from [1.2.3.4:nnnnn] to me [1.2.3.4:nnnnn] \n REQ : %s \n",i,echobuffer);
           }

        char ss[70000];
       bzero(echobuffer,70000);
       char s1[900];
       strcpy(s1,"IP=2.3.4.5 ");
       char s2[900];
       strcpy(s2,"IP=1.2.3.4 ");
       char s3[900];
       strcpy(s3,"IP=1.2.3.4 ");
       char s4[900];
       strcpy(s4,"Prefered IP = 2.3.4.5 ");
       strcat(s3,s4);
       strcat(s2,s3);
       strcat(s1,s2);
       strcpy(echobuffer,s1);

       if(r > 0)
          {
            if (send(s,echobuffer,r,0)!= r)
              {
              printf("send failed");
               exit(0);
              }
            else
              {
              //printf("send successful");
              close(s);
              }
            }
         else
        {
               printf("problem with send");
             }

     }

}
