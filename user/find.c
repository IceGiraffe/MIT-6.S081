#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    // memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *schObj)
{
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
    {
        char *name = fmtname(path);
        if (strcmp(schObj, name) == 0)
        {
            printf("%s\n", path);
        }
        break;
    }
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > 512)
        {
            printf("ls: path too long\n");
            break;
        }
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            char buf[512], *p;
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            if (stat(buf, &st) < 0)
            {
                printf("open: cannot stat %s\n", buf);
                continue;
            }
            find(buf, schObj);
        }
        break;
    }
    close(fd);
    return;
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Too fewer arguments\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
