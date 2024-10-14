#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    char* argv_1[MAXARG];

    int idx = 1;
    int temp = 1;

    if (strcmp(argv[1], "-n") == 0) idx = 3;
        
    argv_1[0] = malloc(strlen(argv[idx])+1);
    strcpy(argv_1[0], argv[idx]);

    for (int i = idx + 1 ; i < argc ; i++) {
        argv_1[temp] = malloc(strlen(argv[i])+1);
        strcpy(argv_1[temp], argv[i]);
        temp++;
    }

    argv_1[temp] = malloc(256);

    // for (int i = 0 ; i < temp ; i++) {
    //    printf("%s\n", argv_1[i]);
    // }


    char ch;
    int i = 0;
    while(read(0, &ch, 1)){
        // printf("%d\n", buf);
        if (ch == '\n') {
            // printf("yes\n");
            argv_1[temp][i++]='\0';
            if (fork() ==0 ) {
                // printf("index: %d\n", index);
                exec(argv[idx], argv_1);
            } else {
                i = 0;
                wait(0);
            }
        } else {
            argv_1[temp][i++] = ch;
        }
    }
    exit(0);

}

