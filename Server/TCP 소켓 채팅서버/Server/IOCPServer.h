#pragma once
#pragma comment(lib, "ws2_32")
// #include <winsock2.h>
// #include <Ws2tcpip.h>
#include "ClientInfo.h"
#include <vector>
#include <cstring>
#include "Parser.h"

static char* notFoundUser = "31|해당 유저가 존재하지 않습니다.|시스템";

class IOCPServer
{
private:
    std::vector<stClientInfo> mClientInfos;
    SOCKET mListenSocket;
    size_t mClientCnt;
    static const int WAIT_QUEUE_CNT = 5;
    static const int MAX_WORKERTHREAD = 16;
    size_t mMaxClientCount;

    std::vector<HANDLE> mIOWorkerThreads;

    HANDLE mAccepterThread;
    HANDLE mIOCPHandle;

    bool mbIsWorkerRun;
    bool mbIsAccepterRun;

    char mSocketBuf[1024];
    
public:
    IOCPServer(void):
        mListenSocket(INVALID_SOCKET), mClientCnt(0), mIOCPHandle(INVALID_HANDLE_VALUE),
        mbIsWorkerRun(true), mbIsAccepterRun(true), mMaxClientCount(0){
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
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_) {
        printf("[OnReceive] 클라이언트: Index(%d), dataSize(%d)\n", clientIndex_, size_);

    }

private:
    void createClient(const UINT32 maxClientCount);
    bool createWorkerThread();
    bool createAccepterThread();
    size_t getEmptyClientInfo();
    stClientInfo* getClientInfo(const UINT32 sessionIndex);
  
    void CloseSocket(stClientInfo* pClientInfo, bool bIsForce = false);
    DWORD AccepterThread();
    DWORD WorkerThread();

    bool ActionProcess(DWORD dwIoSize, stClientInfo* senderClient){
        
        Parser parser = Parser(senderClient->mRecvBuf);

        switch(parser.mAction){
            case CHAT_MULTICAST:
                MultiCast(dwIoSize, senderClient);
                break;
            case CHAT_UNICAST:
                UniCast(&parser, senderClient);
                break;
            case CHAT_BROADCAST:
            case ROOM_ENTER:
            case ROOM_EXIT: 
                BroadCast(dwIoSize, senderClient);
                break;
            case SERVER_ENTER:
                if(senderClient->mbHasNick) {
                    printf("[에러] 닉네임이 이미 세팅되어 있음\n");
                    return false;
                }
                senderClient->setNickname(dwIoSize);
                BroadCast(dwIoSize, senderClient);
                break;
            case SERVER_EXIT:
                MultiCast(dwIoSize, senderClient);
                senderClient->Close();
                break;
            default:
                return false;
        }
        return true;
    }

    size_t GetNicknameIndex(char* nickname){
        for(size_t i = 0;i<mClientCnt;i++){
            if(0 == strcmp(nickname, mClientInfos[i].mNickname))
                return i;
        }
        return mMaxClientCount;
    }

    void BroadCast(DWORD dwIoSize, stClientInfo* senderClient){
        size_t senderIndex = senderClient->GetIndex();
        std::vector<size_t> closedClient;
        for(size_t i = 0;i<mClientCnt;i++){
            if(i == senderIndex || mClientInfos[i].IsConnected() == false) continue;
            bool bSuccess = mClientInfos[i].SendMsg(dwIoSize, senderClient->mRecvBuf);
            if(!bSuccess) closedClient.push_back(i);
        }

        for(size_t i=0;i<closedClient.size();++i){
            mClientInfos[closedClient[i]].Close();
            --mClientCnt;
        }
    };

    void UniCast(Parser* parser, stClientInfo* senderClient){
        char nickname[32];
        parser->GetClient(nickname);
    
        size_t recvIndex = GetNicknameIndex(nickname);
        if(recvIndex == mMaxClientCount){
            // 그런 닉네임 없다~
            bool bSuccess = senderClient->SendMsg(sizeof(notFoundUser), notFoundUser);
            if(!bSuccess) mClientInfos[recvIndex].Close();
            return;
        }
        int msgLen = parser->SetSender(senderClient->mNickname);

        bool bSuccess = mClientInfos[recvIndex].SendMsg(msgLen, senderClient->mRecvBuf);
        if(!bSuccess) mClientInfos[recvIndex].Close();
    }

    void MultiCast(DWORD dwIoSize, stClientInfo* senderClient){
        size_t senderIndex = senderClient->GetIndex();
        std::vector<size_t> closedClient;
        for(size_t i = 0;i<mMaxClientCount;i++){
            if(i == senderIndex || mClientInfos[i].IsConnected() == false || 
                mClientInfos[i].mRoom != senderClient->mRoom) continue;

            bool bSuccess = mClientInfos[i].SendMsg(dwIoSize, senderClient->mRecvBuf);
            if(!bSuccess) closedClient.push_back(i);
        }

        for(size_t i=0;i<closedClient.size();++i){
            mClientInfos[closedClient[i]].Close();
            --mClientCnt;
        }
    }

    stClientInfo* GetClientInfo(const UINT32 sessionIndex)
	{
		return &mClientInfos[sessionIndex];		
	}

    static DWORD WINAPI StaticAccepterThread(void* arg){
        IOCPServer* This = (IOCPServer*) arg;
        return This->AccepterThread();
    };

    static DWORD WINAPI StaticWorkerThread(LPVOID arg){
        IOCPServer* This = (IOCPServer*) arg;
        return This->WorkerThread();
    }
};
