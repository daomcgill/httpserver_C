/**
 * NAME : Dao McGill
 *
 * HOMEWORK : 4
 *
 * CLASS : ICS 451
 *
 * DATE : October 18, 2023
 *
 * FILE : client.c
 *
 **/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

FILE *file;

/**
 * FUNCTION: printBoth
 * 
 * DESCRIPTION: Prints to both stdout and file.
 *
 * @param fp: file to print to
 * @param string: string to print
 * 
 * @return: void
 *
 **/
/* I took this entire function from https://stackoverflow.com/questions/6420194/how-to-print-both-to-stdout-and-file-in-c */
void printBoth(FILE *fp, char *string) 
{
    printf("%s", string);
    fprintf(fp, "%s", string);
    return;
}

/**
 * FUNCTION: createSocket
 * 
 * DESCRIPTION: Creates a socket using the given parameters.
 *
 * @param ipv4: family of ip address (AF_INET)
 * @param tcp: tcp socket type (SOCK_STREAM)
 * 
 * @return: socket or -1 if error
 *
 **/
int createSocket(int ipv4, int tcp)
{
    int sock = 0;
    sock = socket(ipv4, tcp, 0);
    if (sock < 0)
    {
        printBoth(file, "ERR: socket could not be created\n");
        sock = -1;
    }
    return sock;
}

/**
 * FUNCTION: connectServer
 * 
 * DESCRIPTION: Connects to server using the given parameters.
 * 
 * @param sock: socket to bind
 * @param ipv4: family of ip address (AF_INET)
 * @param port: port number
 * @param ip_address: IP address of the server
 * 
 * @return: 0 or -1 if error
 **/
int connectServer(int sock, int ipv4, int port, const char *ip_address)
{
    int rv = 0;
    struct sockaddr_in servaddr;
    servaddr.sin_family = ipv4;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printBoth(file, "ERR: connection to server failed\n");
        rv = -1;
    }
    return rv;
}

/**
 * FUNCTION: main
 * 
 * DESCRIPTION: Main function of server.
 * 
 * @param argc: number of arguments
 * @param argv: arg[0]: ./client, arg[1]: port number
*/
int main(int argc, char *argv[])
{
   int rv = 0;
   if (argc < 2) 
   {
    printf("ERR: port number not provided\n");
    return -1;
   }
   int port = atoi(argv[1]);
   const char *ip_address = "127.0.0.1";
   int ipv4 = AF_INET;
   int tcp = SOCK_STREAM;
   int sock = 0;

   file = fopen("output_client.txt", "w");
   if (file == NULL)
   {
      printBoth(file, "ERR: could not open file\n");
      rv = -1;
   }

   sock = createSocket(ipv4, tcp);
   if (sock >= 0 && rv == 0)
   {
      printBoth(file, "socket created\n");
      
      if (connectServer(sock, ipv4, port, ip_address) >= 0 && rv == 0)
      {
         printBoth(file, "Connected to server\n");

         FILE *f = fopen("file_client.jpeg", "wb");
         if (f != NULL)
         {
               char buff[1000];
               int received = 0;
               printBoth(file, "receiving file...\n");
               while ((received = recv(sock, buff, sizeof(buff), 0)) > 0)
               {
                  fwrite(buff, 1, received, f);
               }
               printBoth(file, "file received\n");
               fclose(f);
         }
         else
         {
               printBoth(file, "ERR: could not open file\n");
               rv = -1;
         }

         close(sock);
         printBoth(file, "connection to server closed\n");
      }
      else
      {
         printBoth(file, "ERR: connection to server failed\n");
         rv = -1;
      }
   }
   else
   {
      printBoth(file, "ERR: socket could not be created\n");
      rv = -1;
   }

   fclose(file);
   return rv;
}
