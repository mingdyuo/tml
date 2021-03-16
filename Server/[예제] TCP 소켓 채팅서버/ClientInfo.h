#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <cstdio>
#define MAX_SOCKBUF 1024	//패킷 크기

enum IOOperation
{
	RECV,
	SEND
};

//WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx
{
	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O구조체
	SOCKET		m_socketClient;			//클라이언트 소켓
	WSABUF		m_wsaBuf;				//Overlapped I/O작업 버퍼
	IOOperation m_eOperation;			//작업 동작 종류
};

//클라이언트 정보를 담기위한 구조체
struct stClientInfo
{
	INT32 mIndex;
	SOCKET			m_socketClient;			//Cliet와 연결되는 소켓
	stOverlappedEx	m_stRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O작업을 위한 변수
	
	char			mRecvBuf[MAX_SOCKBUF]; //데이터 버퍼
	char			mSendBuf[MAX_SOCKBUF]; //데이터 버퍼

	stClientInfo(): mIndex(0), m_socketClient(INVALID_SOCKET)
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	}

	stClientInfo(UINT32 index): mIndex(mIndex), m_socketClient(INVALID_SOCKET)
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	}

public:
	INT32 GetIndex(){return mIndex;}
	SOCKET GetSock() {return m_socketClient; }
	bool IsConnected() {return m_socketClient != INVALID_SOCKET;}

	void SendCompleted(const UINT32 dataSize_){
		printf("[송신 완료] bytes : %d\n", dataSize_);
	}

	void Close(bool bIsForce = false){
		struct linger stLinger = {0, 0};
		if(bIsForce)
			stLinger.l_onoff = 1;
		
		shutdown(m_socketClient, SD_BOTH);

		setsockopt(m_socketClient, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		closesocket(m_socketClient);
		m_socketClient = INVALID_SOCKET;
	}

	bool BindIOCompletionPort(HANDLE iocpHandle_){
		HANDLE hIOCP = CreateIoCompletionPort(
			(HANDLE)GetSock(),
			iocpHandle_,
			(ULONG_PTR)(this),
			0
		);
		if(hIOCP == INVALID_HANDLE_VALUE){
			printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
			return false;
		}

		return true;
	}

	bool OnConnect(HANDLE iocpHandle_, SOCKET socket_){
		m_socketClient = socket_;
		if(BindIOCompletionPort(iocpHandle_) == false)
			return false;

		return BindRecv(); 
	}

	bool BindRecv(){
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		m_stRecvOverlappedEx.m_wsaBuf.len = MAX_SOCKBUF;
		m_stRecvOverlappedEx.m_wsaBuf.buf = mRecvBuf;
		m_stRecvOverlappedEx.m_eOperation = RECV;

		int nRet = WSARecv(
			m_socketClient,
			&(m_stRecvOverlappedEx.m_wsaBuf),
			1,
			&dwRecvNumBytes,
			&dwFlag,
			(LPWSAOVERLAPPED) & (m_stRecvOverlappedEx),
			NULL
		);

		if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)){
			printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}
		return true;
	}

	// 1개의 스레드 에서만 호출해야함
	bool SendMsg(const UINT32 dataSize_, char* pMsg_){
		stOverlappedEx* sendOverlappedEx = new stOverlappedEx;

	}



};
