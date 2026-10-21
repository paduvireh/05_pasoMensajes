#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 5678;
    message_buf sbuf, rbuf;
    size_t buf_length;

    msqid = msgget(key, 0666);

    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    // Envia solicitud al servidor
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Buenos dias, servidor. Soy el Cliente 1");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Cliente 1 envia solicitud\n");

    // Recibe solamente respuesta tipo 101
    if (msgrcv(msqid, &rbuf, MSGSZ, 101, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Cliente 1 recibe: %s\n", rbuf.mtext);

    return 0;
}
