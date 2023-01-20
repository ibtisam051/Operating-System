#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd1[2]; // for pipe1
    int fd2[2]; // for pipe2

    pipe(fd1);
    pipe(fd2);


    if(fork()==0) //child 
    {
       close (fd1[1]);
       close (fd2[0]);

       int temp_array[2];
       int add =0;

       read(fd1[0],temp_array,8);
       add = temp_array[0]+temp_array[1];
       write(fd2[1],&add,4);
    }
    else          //parent
    {
        close(fd1[0]);
        close(fd2[1]);

       int array[2]= {10,20};
       int sum =0;

       write(fd1[1],array,8);
       read(fd2[0],&sum,4);

       printf("The sum of two numbers is: %d\n",sum);
       wait(NULL);
    }
    return 0;
}