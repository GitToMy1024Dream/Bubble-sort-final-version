#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
int main()
{
	struct pollfd evs;
	evs.fd = 0;
	evs.events = POLLIN;
	evs.revents = 0;
	int timeout = -1;//�����ȴ��¼�����
	while(1){
		switch(poll(&evs,1,timeout)){
                //��ʱ
		        case 0:
				printf("timeout...\n");
				break;
                //ʧ��
			case -1:
				perror("poll");
				break;
			default:
				{
                  //�鿴�ض��¼��Ƿ���
					if(evs.revents & POLLIN){
						char buf[1024];
                       //read
						ssize_t s = read(evs.fd,buf,sizeof(buf)-1);
                       //��ȡ�ɹ�����
						if(s > 0){
							buf[s] = 0;
							printf("echo# %s\n",buf);
						}
						else{
							perror("read");
						}
					}
				}
				break;
		}
	}
}