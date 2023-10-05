#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

//accept port number as argument
int main(int argc, char *argv[])
{
   //arg to int
   int port = atoi(argv[1]);

   //write server output to file
   FILE *file = freopen("output_server.txt", "w", stdout);

   //create socket
   int soc = socket(AF_INET, SOCK_STREAM, 0);
   if (soc < 0)
   {
      printf("ERR: socket could not be created\n");
      return 1;
   }
   //bind socket to port
   


   //close file
   fclose(file);

}
