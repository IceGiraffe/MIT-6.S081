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

    // learned lesson
    while (1)
    {
        if (fork() == 0)
        {
            int leftPipe[2];
            leftPipe[0] = rightPipe[0];
            leftPipe[1] = rightPipe[1];
            // 左边不写
            close(leftPipe[1]);
            pipe(rightPipe);

            // 新建管道
            printf("%d %d %d %d\n", leftPipe[0], leftPipe[1], rightPipe[0], rightPipe[1]);
            int thisPrime;
            int res = read(leftPipe[0], &thisPrime, sizeof(thisPrime));
            // printf("%d\n", res);
            if (res == 0)
            {
                // close(leftPipe[0]);
                exit(0);
            }
            else{
                printf("prime %d\n", thisPrime);
                int num;
                while ((res = read(leftPipe[0], &num, sizeof(num))) != 0)
                {
                    // printf("RES: %d\n", res);
                    // printf("%d: read %d\n", getpid(), num);
                    if (num % thisPrime == 0)
                    {
                        ;
                    }
                    else
                    {
                        // printf("%d: write %d\n", getpid(), num);
                        write(rightPipe[1], &num, sizeof(num));
                    }
                }
                // close(leftPipe[0]);
                close(rightPipe[1]);
            }
        }
        else{   
            // close(rightPipe[0]);
            close(rightPipe[1]);
            wait(0);
            break;
        }
    }
    exit(0);
}