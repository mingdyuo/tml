#pragma once
#pragma comment(lib, "ws2_32")
// #include <winsock2.h>
// #include <Ws2tcpip.h>
#include "ClientInfo.h"
#include <vector>
#include <cstring>


class IOCPServer
{
private:
    std::vector<stClientInfo> mClientInfos;
    SOCKET  mListenSocket;
    int     mClientCnt;
    static const int WAIT_QUEUE_CNT = 5;
    static const int MAX_WORKERTHREAD = 16;

    std::vector<unsigned long> mIOWorkerThreads;

    HANDLE mAccepterThread;
    HANDLE mIOCPHandle;

    bool mbIsWorkerRun;
    bool mbIsAccepterRun;

    char mSocketBuf[1024];

public:
    IOCPServer(void):
        mListenSocket(INVALID_SOCKET), mClientCnt(0), mIOCPHandle(INVALID_HANDLE_VALUE),
        mbIsWorkerRun(true), mbIsAccepterRun(true){
            memset(mSocketBuf, 0, sizeof(mSocketBuf));
        }

    virtual ~IOCPServer(void){ WSACleanup(); }

    bool InitSocket();
    bool BindAndListen(int nBindPort);
    bool StartServer(const UINT32 maxClientCount);
    void DestroyThread();

    virtual void OnConnect(const UINT32 clientIndex_) {
        printf("[OnConnect] 클라이언트: Index(%d)\n", clientIndex_);
    }
	virtual void OnClose(const UINT32 clientIndex_) {
        printf("[OnClose] 클라이언트: Index(%d)\n", clientIndex_);
    }
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) {
        printf("[OnReceive] 클라이언트: Index(%d), dataSize(%d)\n", clientIndex_, size_);

		// PacketData packet;
		// packet.Set(clientIndex_, size_, pData_);

		// std::lock_guard<std::mutex> guard(mLock);
		// mPacketDataQueue.push_back(packet);
    }

private:
    void createClient(const UINT32 maxClientCount);
    bool createWorkerThread();
    bool createAccepterThread();
    stClientInfo* getEmptyClientInfo();
    stClientInfo* getClientInfo(const UINT32 sessionIndex);
    //bool bindIOCompletionPort(stClientInfo* pClientInfo);
    // bool bindRecv(stClientInfo* pClientInfo);
    // bool SendMsg();
  
    void CloseSocket(stClientInfo* pClientInfo, bool bIsForce = false);
    DWORD AccepterThread();
    DWORD WorkerThread();

    static DWORD WINAPI StaticAccepterThread(void* arg){
        IOCPServer* This = (IOCPServer*) arg;
        return This->AccepterThread();
    };

    static DWORD WINAPI StaticWorkerThread(LPVOID arg){
        IOCPServer* This = (IOCPServer*) arg;
        return This->WorkerThread();
    }
};
