#include "common_impl.h"

/* variables globales */

/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL;

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;

void usage(void)
{
fprintf(stdout,"Usage : dsmexec machine_file executable arg1 arg2 ...\n");
fflush(stdout);
exit(EXIT_FAILURE);
}

void sigchld_handler(int sig)
{
printf("Processus zombie tué\n");
wait(NULL);
}


int main(int argc, char *argv[])
{
if (argc < 3){
  usage();
}
else {
  struct sigaction tuer;
  pid_t pid;
  int num_procs = 0;
  int i;
  int port;

   /* Mise en place d'un traitant pour recuperer les fils zombies*/
   tuer.sa_handler = sigchld_handler;
   sigaction(SIGCHLD, &tuer, NULL);

   /* lecture du fichier de machines */
   int fd = open("machine_file", O_RDONLY);
   if (fd ==-1){
     error("open");
   }


   struct stat infos;

   //size of the file
   fstat(fd, &infos);
   int sizefile = infos.st_size;


   char *stock;
   stock = malloc(sizeof(char)*36);
   read(fd, stock, sizefile);

   /* 1- on recupere le nombre de processus a lancer */
   num_procs = nb_lines(fd, sizefile, stock);
   printf("num_procs : %d\n", num_procs);

   /* 2- on recupere les noms des machines : le nom de */
   /* la machine est un des elements d'identification */
   char *tab[num_procs];
   read_line(fd, num_procs, sizefile, stock, tab);





   /* creation de la socket d'ecoute */
   int sock = creer_socket(&port);

    /* + ecoute effective */
   if (listen(sock, num_procs) == LISTEN_ERROR)
      error("listen");

   printf("Connection on the port %d\n", port);
   int (*fd1)[2];
   fd1=malloc(2*num_procs*sizeof(int));
   int (*fd2)[2];
   fd2=malloc(2*num_procs*sizeof(int));

        /* creation des fils */
        for(i = 0; i < num_procs ; i++) {

          /* creation du tube pour rediriger stdout */

          pipe(fd1[i]);

          /* creation du tube pour rediriger stderr */

          pipe(fd2[i]);

          printf("test\n");

          pid = fork();

          printf("test2\n");

          if(pid == -1) ERROR_EXIT("fork");

          if (pid == 0) { /* fils */

            close(fd1[i][0]);

            //redirection de stdout
            close(STDOUT_FILENO);
            close(fd2[i][1]);
            dup(fd1[i][1]);
            close(fd1[i][1]);
            printf("test3\n");
            //redirection de stderr
            close(STDERR_FILENO);
            dup(fd2[i][0]);
            close(fd2[i][0]);

            /* Creation du tableau d'arguments pour le ssh */
            char **newargv;
            newargv = malloc((argc+4)*sizeof(char*));
            newargv[0] = "ssh";
            newargv[1] = tab[i];
            newargv[2] = "~/Sys_res/Prog_sys/Add/Phase1/bin/dsmwrap";

            int j;
            for (j=3; j<argc+1; j++)
              newargv[j]= argv[j-1];

            char *port_name;
            port_name = malloc(sizeof(char)*36);
            sprintf(port_name, "%d", port);
            newargv[argc+1]=port_name;

            char hostname[256];
            gethostname(hostname, 256);
            newargv[argc+2]=hostname;

            newargv[argc+3]=NULL;

            /* jump to new prog : */
            execvp("ssh",newargv);
            printf("test4\n");

} else  if(pid > 0) { /* pere */
  printf("test5");
   /* fermeture des extremites des tubes non utiles */
   close(fd1[i][1]);
   close(fd2[i][1]);
   num_procs_creat++;
}
   }

   printf("Connexion\n");
   int csock[num_procs];

   for(i = 0; i < num_procs ; i++){

      /* on accepte les connexions des processus dsm */
      struct sockaddr_in csin;
      socklen_t taille = sizeof(csin);
      csock[i] = accept(sock, (struct sockaddr*)&csin, &taille);
      if (csock[i] == BIND_ERROR){
        error("accept");
      }

      /*  On recupere le nom de la machine distante */
      /* 1- d'abord la taille de la chaine */
      int size_name = sizeof(tab[i]);
      /* 2- puis la chaine elle-meme */
      char *nom_machine = tab[i];
      printf("Machine %s\n",nom_machine);

/* On recupere le pid du processus distant  */

/* On recupere le numero de port de la socket */
/* d'ecoute des processus distants */
   }

   /* envoi du nombre de processus aux processus dsm*/

   /* envoi des rangs aux processus dsm */

   /* envoi des infos de connexion aux processus */

   /* gestion des E/S : on recupere les caracteres */
   /* sur les tubes de redirection de stdout/stderr */
   /* while(1)
       {
          je recupere les infos sur les tubes de redirection
          jusqu'à ce qu'ils soient inactifs (ie fermes par les
          processus dsm ecrivains de l'autre cote ...)

       };
    */

   /* on attend les processus fils */

   /* on ferme les descripteurs proprement */

   /* on ferme la socket d'ecoute */
   close(sock);
   free(fd1);
   free(fd2);
}
 exit(EXIT_SUCCESS);
}
