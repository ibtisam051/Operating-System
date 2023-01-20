#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    //ls -R | grep "max.c" | cat "max.c" | more
    int fd1[2];
    int fd2[2];
    int fd3[2];

    pipe(fd1);
    pipe(fd2);
    pipe(fd3);

    if(fork()==0) //child1 for ls-R
    {
        close(fd1[0]);
        close(fd2[1]);
        close(fd2[0]);
        close(fd3[1]);
        close(fd3[1]);

        dup2(fd1[1],1);
        execlp("ls","ls","-R",NULL);

    }
    else         //parent
    {
        if(fork()==0) //child2 for grep
        {
            close(fd1[1]);
            close(fd2[0]);
            close(fd3[1]);
            close(fd3[0]);

            dup2(fd1[0],0);
            dup2(fd2[1],1);

            execlp("grep","grep","max.c",NULL);
        }
        else //parent
        {
            if(fork()==0) //child3 for "cat"
            {
               close(fd1[1]);
               close(fd1[0]);
               close(fd2[1]);
               close(fd3[0]);

               dup2(fd2[0],0);
               dup2(fd3[1],1);
               execlp("cat","cat","max.c",NULL);
            }
            else  //parent
            {
               if(fork()==0) //child4 for "more"
               {
                  close(fd1[1]);
                  close(fd1[0]);
                  close(fd2[1]);
                  close(fd2[0]);
                  close(fd3[1]);

                  dup2(fd3[0],0);
                  execlp("more","more",NULL);
               }
               else
               {
                   //wait(NULL);
               }
            }

        }
    }
    return 0;
}