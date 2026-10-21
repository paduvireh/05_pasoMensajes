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
    int msgflg = IPC_CREAT | 0666;
    key_t key = 5678;
    message_buf rbuf, sbuf;
    size_t buf_length;

    msqid = msgget(key, msgflg);

    if (msqid < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Servidor esperando clientes...\n");

    // Recibe solicitudes de los dos clientes
    for (int i = 0; i < 2; i++) {

        // Recibe mensajes de tipo 1
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Servidor recibe: %s\n", rbuf.mtext);

        // Identifica a que cliente responder
        if (strcmp(rbuf.mtext, "Buenos dias, servidor. Soy el Cliente 1") == 0) {
            sbuf.mtype = 101;
            strcpy(sbuf.mtext, "Buenos dias, estimado Cliente 1. Es un gusto atenderlo.");
        }
        else if (strcmp(rbuf.mtext, "Hola servidor. Soy el Cliente 2") == 0) {
            sbuf.mtype = 102;
            strcpy(sbuf.mtext, "Hola Cliente 2. ¿Que tal, como esta?");
        }
        else {
            printf("Cliente no reconocido\n");
            continue;
        }

        // Envia respuesta al cliente correspondiente
        buf_length = strlen(sbuf.mtext) + 1;

        if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Servidor envia: %s\n", sbuf.mtext);
    }

    return 0;
}
