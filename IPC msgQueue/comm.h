//comm.h
#ifndef _COMM_H_
#define _COMM_H_
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define PATHNAME "."
#define PROJ_ID 0x6666
#define SERVER_TYPE 1
#define CLIENT_TYPE 2
struct msgbuf
{
    long mtype;
    char mtext[1024]; 
};
int createMsgQueue(); //������Ϣ����
int getMsgQueue();    //������Ϣ����
int destroyMstQueue(int msgid);  //������Ϣ����
int senMsg(int msgid,int who,char* msg); //������Ϣ
int recvMsg(int msgid,int recvType,char out[]);//������Ϣ
#endif