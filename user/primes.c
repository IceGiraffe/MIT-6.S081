#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int rightPipe[2];
    pipe(rightPipe);
    // feed numbers in the beginning
    // no need to read
    for (int i = 2; i <= 35; i++)
    {
        write(rightPipe[1], &i, sizeof(i));
    }
    // 主进程的父进程，一定要记得关
    close(rightPipe[1]);


    // 一直fork，直到read不出来新的素数
    while (1)
    {
        if (fork() == 0)
        {
            // 父进程的right，对于子进程而言是left
            int leftPipe[2];
            leftPipe[0] = rightPipe[0];
            leftPipe[1] = rightPipe[1];
            // 关闭left的写端
            close(leftPipe[1]);

            // 创建新的pipe
            pipe(rightPipe);
            // 此时不能基于关闭right的读端，必须先等子进程fork到这个读
            // close(rightPipe[0]);
            
            // 代码第60行，如果取消注释，下一行的输出会产生变化，体现了关不关读端的影响
            // printf("%d %d %d %d\n", leftPipe[0], leftPipe[1], rightPipe[0], rightPipe[1]);
            int thisPrime;
            int res = read(leftPipe[0], &thisPrime, sizeof(thisPrime));
            // printf("%d\n", res);
            if (res == 0)
            {
                // 直接exit的话，其实无所谓close不close
                exit(0);
            }
            else{
                printf("prime %d\n", thisPrime);
                int num;
                while ((res = read(leftPipe[0], &num, sizeof(num))) != 0)
                {
                    // printf("RES: %d\n", res);
                    // printf("%d: read %d\n", getpid(), num);
                    if (num % thisPrime != 0)
                    {
                        // printf("%d: write %d\n", getpid(), num);
                        write(rightPipe[1], &num, sizeof(num));
                    }
                }
                // 关闭左边的读，其实不关闭不影响正确性
                close(leftPipe[0]);
                // 关闭右边的写，不需要写了
                close(rightPipe[1]);
            }
        }
        else{   
            // 关闭右边的读端
            close(rightPipe[0]);
            // 等待子进程结束
            wait(0);
            break;
        }
    }
    exit(0);
}