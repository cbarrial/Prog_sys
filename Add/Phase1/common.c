#include "common_impl.h"

int creer_socket(int prop, int *port_num)
{
   int fd = 0;

   /* fonction de creation et d'attachement */
   /* d'une nouvelle socket */
   /* renvoie le numero de descripteur */
   /* et modifie le parametre port_num */

   return fd;
}

int nb_lines(int fd,int mxlen){

  int i;
  int count=0;
  char *buffer;
  buffer = malloc(sizeof(char)*36);
  read(fd, buffer, mxlen);

  for (i=0; i<mxlen; i++){

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



  for (i=0; i<mxlen; i++){

    machine[j++]=stock[i];
    if (stock[i] == '\n'){
      machine[j-1]='\0';
      buffer[l]=malloc(sizeof(char)*36);
      strcpy(buffer[l], machine);
      l++;
      memset(machine, '\0', j);
      j=0;
    }
  }

}

/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */
