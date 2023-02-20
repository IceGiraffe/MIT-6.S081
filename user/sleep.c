#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        char *msg = "Too fewer argument\n";
        write(1, msg, strlen(msg));
        exit(1);
    }
    else
    {
        int tm = atoi(argv[1]);
        if (tm < 0)
        {
            char *msg = "Sleep time should >= 0\n";
            write(1, msg, strlen(msg));
            exit(1);
        }
        sleep(tm);
        exit(0);
    }
}