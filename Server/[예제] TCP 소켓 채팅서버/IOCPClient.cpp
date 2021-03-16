#pragma comment(lib, "ws2_32.lib")
#include <cstdio>
#include <stdlib.h>
#include <winsock2.h>

#define MAX_SOCKBUF 1024
void ErrorHandling(char *message);

enum IOOperation
{
	RECV,
	SEND
};

struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O구조체
	WSABUF		m_wsaBuf;				//Overlapped I/O작업 버퍼
	IOOperation m_eOperation;			//작업 동작 종류
};


class IOCPClient{
private:
    const int SERVER_PORT = 9898;
    const int MAX_WORKERTHREAD = 8;
    SOCKET mSocket;
    HANDLE mIOCPHandle;

	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O작업을 위한 변수

    char			mRecvBuf[MAX_SOCKBUF]; //데이터 버퍼
	char			mSendBuf[MAX_SOCKBUF]; //데이터 버퍼

public:
    IOCPClient():mSocket(INVALID_SOCKET){
        ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
    }

    bool InitSocket(){
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
            printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
            return false;
        }

        mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); 
        if (mSocket == INVALID_SOCKET){
            printf("[에러] WSASocket()함수 실패 : %d\n", WSAGetLastError());
            return false;
        }
            
        printf("[성공] 소켓 초기화 완료\n");
        return true;
    }

    bool CreateIocpAndThreads(){
        mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, MAX_WORKERTHREAD); 
        if (!mIOCPHandle){
            printf("[에러] CreateIoCompletionPort()함수 iocp 생성 실패 : %d\n", WSAGetLastError());
            return false;
        }
        
        for(int i=0;i<MAX_WORKERTHREAD;i++){
            HANDLE hThread = CreateThread(NULL, 0, StaticWorkerThread, (void*)this, 0, NULL); 
            CloseHandle(hThread); 
        }

        printf("[성공] iocp, thread 생성 완료\n");
        return true;
    }

    bool ConnectIOCP(){
        SOCKADDR_IN SockAddr; 
        SockAddr.sin_family = AF_INET; 
        SockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        SockAddr.sin_port = htons(SERVER_PORT); 

        CreateIoCompletionPort((HANDLE)mSocket, mIOCPHandle, 0, 0); 

        if (SOCKET_ERROR == WSAConnect(mSocket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr), NULL, NULL, NULL, NULL)){
            printf("[에러] WSAConnect()함수 iocp 생성 실패 : %d\n", WSAGetLastError());
            return false;
        }

        printf("[성공] iocp connect 완료\n");
        return true;
    }

    static DWORD WINAPI StaticWorkerThread(LPVOID arg){
        IOCPClient* This = (IOCPClient*)arg;
        return This->WorkerThread();
    }

    DWORD WorkerThread(){
        DWORD dwResult = 0;


        return dwResult;
    }

   

};
 
int main()
{
    IOCPClient iocpClient;

    iocpClient.InitSocket();
    iocpClient.CreateIocpAndThreads();
    iocpClient.ConnectIOCP();



    PER_IO_DATA *pPerIoData = new PER_IO_DATA;
    ZeroMemory(pPerIoData, sizeof(PER_IO_DATA)); 

    pPerIoData->Socket = Socket; 
    pPerIoData->Overlapped.hEvent = WSACreateEvent(); 
    pPerIoData->wsaBuf.buf = pPerIoData->Buffer; 
    pPerIoData->wsaBuf.len = sizeof(pPerIoData->Buffer); 

    DWORD dwNumRecv;
    if (WSARecv(Socket, &(pPerIoData->wsaBuf), 1, &dwNumRecv, &(pPerIoData->Flags), &(pPerIoData->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            delete pPerIoData;
            return 0;
        }
    } 

    while (TRUE) 
        Sleep(1000); 

    shutdown(Socket, SD_BOTH); 
    closesocket(Socket); 

    WSACleanup(); 
 
    return 0;
}
 
