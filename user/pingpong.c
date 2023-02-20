#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int myPipe[2];
    pipe(myPipe);

    int pid = fork();
    // child
    if(pid == 0){
        pid = getpid();
        char *buf = malloc(1);
        read(myPipe[1], buf, 1);
        printf("%d: received ping\n", pid);
        write(myPipe[0], buf, 1);
    }
    // parent
    else{
        pid = getpid();
        char *buf = malloc(1);
        *buf = 'b';
        write(myPipe[0], buf, 1);
        read(myPipe[1], buf, 1);
        printf("%d: received pong\n", pid);
    }
    close(myPipe[0]);
    close(myPipe[1]);
    wait(0);
    exit(0);
}