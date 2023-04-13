// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#define MAX 10
  
// structure for message queue
struct mesg_buffer {
    long msg_type;
    int msg_number;
} message;
  
int main(void)
{
    int total = 128/3;
    pid_t pid1;
    if ((pid1 = fork()) == 0)
    {
        printf("***Processo PRODUTOR criado***\n");
        int key = 500;
        int msgid;
  
        msgid = msgget((key_t)key, 0666 | IPC_CREAT);
        message.msg_type = 1;

        int buffer = 0;
        
        int n = 0;
        while(n < total)
        {

            buffer++;
            message.msg_number = buffer;
            sleep(1); 
            msgsnd(msgid, &message, 32, 0);
            printf("-Mensagem do PRODUTOR enviada: %d\n", message.msg_number);
            n++;
        }
    }
    pid_t pid2;
    if ((pid2 = fork()) == 0)
    {
        printf("***Processo CONSUMIDOR 1 criado***\n");
        int msgid;

        int key = 500;
    
        msgid = msgget((key_t)key, 0666 | IPC_CREAT);
        
        int n = 0;
        while(n < total)
        {
            sleep(1);
            msgrcv(msgid, &message, 32, 1, 0);
    
            printf("-Mensagem do CONSUMIDOR 1 recebida : %d \n",message.msg_number);
            n++;
            //sleep(1);
        }
        msgctl(msgid, IPC_RMID, NULL);
    }
    pid_t pid3;
    if ((pid3 = fork()) == 0)
    {
        printf("***Processo CONSUMIDOR 2 criado***\n");
        int msgid;
        int key = 500;
        msgid = msgget((key_t)key, 0666 | IPC_CREAT);

        int n = 0;
        while(n < total)
        {
            sleep(3);
            msgrcv(msgid, &message, 32, 1, 0);

            printf("-Mensagem do CONSUMIDOR 2 recebida: %d\n", message.msg_number);
            n++;
            //sleep(1);
        
        }
        msgctl(msgid, IPC_RMID, NULL);

    }
  
    return 0;
}     
