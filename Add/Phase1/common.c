#include "common_impl.h"

int creer_socket(int *port_num)
{
  /* fonction de creation et d'attachement */
  /* d'une nouvelle socket */
  /* renvoie le numero de descripteur */
  /* et modifie le parametre port_num */

   struct sockaddr_in sin;
   int sock ;
   int bind_err;

   //create the socket
   sock = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   //Check for the socket validity
   if (sock == SOCKET_ERROR){
     error("socket");
   }

   //init the sin structure


   //clean the sin structure
   memset(&sin, 0, sizeof(sin));
   //Bind to any IP from the host
   sin.sin_addr.s_addr = INADDR_ANY;
   //Internet family protocol
   sin.sin_family = AF_INET;
   //Bind on the TCP port
   sin.sin_port = htons(port_num);

   //perform the binding
   //we bind on the tcp port specified
   bind_err=bind(sock, (struct sockaddr*)&sin, sizeof(sin));
   if ( bind_err == BIND_ERROR)
     error("bind");


   return sock;
}

int do_socket(int domain, int type, int protocol) {
    int sockfd;
    int yes = 1;
    //create the socket
    sockfd = socket(domain,type,protocol);

    //check for socket validity

    // set socket option, to prevent "already in use" issue when rebooting the server right on

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        error("ERROR setting socket options");

    return sockfd;
}

int nb_lines(int fd,int mxlen, char *buffer){

  int i;
  int count=0;

  //Reading the buffer caracter by caracter
  for (i=0; i<mxlen; i++){

        //Each time there is a new line we increment count
        if (buffer[i] == '\n'){
          count++;
        }
  }
  return count;

}

void read_line(int fd, int nb_lines, int mxlen, char *stock,char *buffer[]){


  char *machine;
  machine = malloc(sizeof(char)*36);
  int i;
  int j=0;
  int l=0;

  //We read the file caracter by caractere
  for (i=0; i<mxlen; i++){
    //Writting computer name into machine
    machine[j++]=stock[i];

    if (stock[i] == '\n'){ //new line
      machine[j-1]='\0'; //delete the \n
      buffer[l]=malloc(sizeof(char)*36);
      strcpy(buffer[l], machine); //Adding The machine name to the machine buffer
      l++;
      memset(machine, '\0', j);
      j=0;
    }
  }

}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}




/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */
