#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(char *path, char *filename) { // arg: directory path, file name
    char buf[256], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) { // read-only mode
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) { // get status of opened file
        fprintf(2, "find: cannot stat %s\n", path); 
        close(fd);
        return;
    }

    if (st.type == T_DIR) {
        // Directory handling
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) { // check length
            printf("find: path too long\n");
            close(fd);
            return;
        }

        strcpy(buf, path); // copy path to buf
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) // skip '.' and '..'
                continue;

            memmove(p, de.name, DIRSIZ); // copy directory entry name
            p[DIRSIZ] = 0;

            if(stat(buf, &st) < 0) { // check status full path
                printf("ls: cannot stat %s\n", buf);
                continue;
            }

            if (strcmp(de.name, filename) == 0) { // check directory entry name and filename the same?
                printf("%s\n", buf);
            }

            find(buf, filename); // recurse for subdirectory
        }
    }
    else if (st.type == T_FILE) {
        // File handling
        if (strcmp(fmtname(path), filename) == 0) {
            printf("%s\n", path);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <name>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

// hello
int x;

