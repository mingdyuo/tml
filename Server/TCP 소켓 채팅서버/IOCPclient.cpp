#pragma comment(lib, "ws2_32.lib")
#include <cstdio>
#include <stdlib.h>
#include <winsock2.h>
#include <string>
#include <iostream>
#define MAX_SOCKBUF 1024

void ErrorHandling(char *message);
static const int SERVER_PORT = 9898;

enum IOOperation
{
	RECV,
	SEND
};

class IOCPClient{
private:
    SOCKET mSocket;
    bool mbIsWorkerRun;
    char mNickname[32];

public:
    IOCPClient():mSocket(INVALID_SOCKET), mbIsWorkerRun(true){
    }

    bool OnConnect(){return mbIsWorkerRun;}

    char* getNickname(){return mNickname;}

    bool InitSocket(){
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
            printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
            return false;
        }

        mSocket = socket(PF_INET, SOCK_STREAM, 0); 

        if (mSocket == INVALID_SOCKET){
            printf("[에러] socket()함수 실패 : %d\n", WSAGetLastError());
            return false;
        }
            
        printf("[성공] 소켓 초기화 완료\n");
        return true;
    }

    bool ConnectServer(int bBindPort){
        SOCKADDR_IN serverAddress;

        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        serverAddress.sin_port = htons(bBindPort); 

        if (SOCKET_ERROR == connect(mSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress))){
            printf("[에러] connect()함수 실패 : %d\n", WSAGetLastError());
            return false;
        }

        printf("[성공] connect 완료\n");
        return true;
    }

    bool SetNickname(){
        do {
            printf("[연결됨] 대화에 사용할 닉네임을 입력하세요(최대 31글자)\n");
            scanf("%s", mNickname);
        } while(strlen(mNickname)==0);

        send(mSocket, mNickname, strlen(mNickname), 0);
        return true;
    }

    bool CreateThreads(HANDLE* sender, HANDLE* recver){
        *recver = CreateThread(NULL, 0, StaticRecvThread, this, 0, 0);
        *sender = CreateThread(NULL, 0, StaticSendThread, this, 0, 0);
        printf("[성공] 스레드 생성 완료\n");

        Sleep(700);
        return true;
    }

    bool close(){
        mbIsWorkerRun = false;
        closesocket(mSocket);
        WSACleanup(); 
        return true;
    }

    static DWORD WINAPI StaticRecvThread(LPVOID arg){
        IOCPClient* This = (IOCPClient*)arg;
        return This->RecvThread();
    }

    static DWORD WINAPI StaticSendThread(LPVOID arg){
        IOCPClient* This = (IOCPClient*)arg;
        return This->SendThread();
    }

    DWORD RecvThread(){
        char received[1024];
        while (mbIsWorkerRun) { 
            memset(&received, 0, sizeof(received));
            int length = recv(mSocket, received, sizeof(received), 0);
            received[length] = '\0';
            if(strcmp(received, "quit")==0) {
                mbIsWorkerRun = false;
                break;
            }
        }
        return 0;
    }

    DWORD SendThread(){
        std::string msg;
        while (mbIsWorkerRun) { 
            printf("[%s님]: ", mNickname);
            do{
                getline(std::cin, msg);
            } while(msg.empty());

            send(mSocket, msg.c_str(), msg.length(), 0);
            msg.clear();
            if (msg == "quit") {
                mbIsWorkerRun = false;
                printf("대화를 종료합니다.\n");
                break;
            }
        }
        return 0;
    }
   
};
 
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
    
    WaitForSingleObject (sender, INFINITE);
    WaitForSingleObject (recver, INFINITE);
    iocpClient.close();
    printf("[알림] 클라이언트가 종료되었습니다. 아무키나 누르면 창을 종료합니다.\n");
    getchar();
    return 0;
}
 
