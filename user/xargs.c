#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int parse_line(char *argv[], int argIdx)
{
    char buf[100];
    int n = 0;
    while (1)
    {
        int a = read(0, buf + n, 1);
        if (a == 0)
        {
            break;
        }
        if (buf[n] == '\n')
        {
            buf[n] = 0;
            n++;
            break;
        }
        ++n;
    }
    if (n == 0)
    {
        return 0;
    }
    int idx = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        // printf("%c", buf[i]);
        if (buf[i] == ' ' || buf[i] == 0)
        {
            argv[argIdx] = malloc(i - idx + 1);
            memmove(argv[argIdx], buf + idx, i - idx);
            argv[argIdx][i - idx] = 0;
            ++argIdx;
            idx = i + 1;
        }
    }
    // argv[argIdx] = malloc(i - idx + 1);
    // memmove(argv[argIdx], buf + idx, i - idx);
    // argv[argIdx][i - idx] = 0;
    // ++argIdx;
    return argIdx;
}

int main(int argc, char *argv[])
{
    // first cmd
    char *cmd = malloc(strlen(argv[1]) + 1);
    // strcpy 会复制'\0'
    strcpy(cmd, argv[1]);

    char *new_argv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }

    // 去掉 xargs 本身
    int argIdx = argc - 1;
    int newIdx;
    while ((newIdx = parse_line(new_argv, argIdx)) != 0)
    {
        new_argv[newIdx] = 0; // nullptr
        if (fork() == 0)
        {
            // for (int i = 0; i < MAXARG; i++)
            // {
            //     if (new_argv[i] != 0)
            //     {
            //         printf("%d ", strlen(new_argv[i]));
            //         printf("%s ", new_argv[i]);
            //     }
            //     else
            //     {
            //         printf("\n");
            //         break;
            //     }
            // }
            exec(cmd, new_argv);
            fprintf(2, "Exec failed\n");
            exit(1);
        }
        wait(0);
    }
    exit(0);
}