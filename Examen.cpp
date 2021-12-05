#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
/*Primero que nada tenemos que definir 2 constantes */
#define READ_END    0    
#define WRITE_END   1    
/* tenemos la funcion main que nos devolvera un entero*/
int main(int argc, char* argv[])   
{
    /*el primer fd1 es para el indicer 1 de escritura */
    /* y el segundo para correr el programa*/
    int fd1[2],fd2[2];
    /* tenemos 2 enteros el status para la funcion que hace cuando acaba la funcion de los hijos  */
    /* y el pid que es el identificador del proceso de los hijos */
    int status, pid;
	/*llamamos la funcion pipe y nos regresara el fd1 relleno*/
    pipe(fd1);                 /* comunica ls y grep */
	/*creamos un hijo con fork */
    pid = fork();     
/* el hijo uno tendra que ejecutarlo,¿Como? con el ls- l*/
    if(pid == 0)              
    {      		
        close(fd1[READ_END]);   /* cerrar extremo */
		
        dup2(fd1[WRITE_END], STDOUT_FILENO); 
	close(fd1[WRITE_END]);
		/*llamamos a execlp con estos elementos */
        execlp("/bin/ls", "ls", "-l", NULL);
    }
    else                          /* padre */
    { 
	close(fd1[WRITE_END]);    /* extremo no necesario ya */
		
	pipe(fd2);		  /* comunica grep y wc */
    	/*creamos el segundo hijo y va a heredar la lectura del pipe */
        pid = fork();
        
       if(pid == 0)              
       {
	  close(fd2[READ_END]);  
									
          dup2(fd1[READ_END], STDIN_FILENO);
	  close(fd1[READ_END]);
			
	  dup2(fd2[WRITE_END], STDOUT_FILENO);			
	  close(fd2[WRITE_END]);
			
          execlp("/bin/grep","grep", "u",NULL);
	}
	else 
	{
           close(fd1[READ_END]);      /* cerrar extremo no necesario */

        }		        
      }
    wait(&status);   
    wait(&status);	
    
    return 0;

}   

    