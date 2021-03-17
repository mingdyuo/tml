#include "IOCPClient.h"
#define MAX_SOCKBUF 1024

void ErrorHandling(char *message);
static const int SERVER_PORT = 9898;

 
int main()
{
    IOCPClient iocpClient;
    HANDLE sender, recver;
    iocpClient.InitSocket();
    iocpClient.ConnectServer(SERVER_PORT);
    iocpClient.SetNickname();
    iocpClient.CreateThreads(&sender, &recver);

    system("cls");
    printf("[%s님] 대화방에 입장하셨습니다.\n", iocpClient.getNickname());
    printf("[알림] quit을 입력시 클라이언트 종료합니다.\n");
    printf("인사말을 건네보세요 !! :");


    WaitForSingleObject (sender, INFINITE);
    WaitForSingleObject (recver, INFINITE);
    
    iocpClient.close();

    printf("[알림] 클라이언트가 종료되었습니다. 아무키나 누르면 창을 종료합니다.\n");
    getchar();
    return 0;
}
 
