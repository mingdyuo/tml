#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <list>
#include <errno.h>
#include <cstring>

#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

struct clientInfo{
	SOCKET socket;
	SOCKADDR_IN clientAddr;
	int id;
	bool connected;
	clientInfo(SOCKET& s, SOCKADDR_IN addr, int id):
		socket(s), clientAddr(addr), id(id), connected(true) {}
};


vector<clientInfo> clientInfos;

void broadCastMsg(int senderId, const char* msg){
	cout<<msg;
	for(int clientCount = 0 ;clientCount < clientInfos.size(); clientCount++){
		if(clientInfos[clientCount].id==senderId || !clientInfos[clientCount].connected) continue;
		int sendByte = send(clientInfos[clientCount].socket, msg, strlen(msg), 0);
	}
	return;
}

void clientChat(int clientId){
	char nickname[256];
	char received[256];

	clientInfo& client = clientInfos[clientId];

	int nickLength = recv(client.socket, nickname, sizeof(nickname), 0);
	nickname[nickLength] = '\0';
	string nicknameMsg = "["+string(nickname)+"]님: ";
	string entranceMsg = "["+string(nickname)+"]님 입장\n";
	broadCastMsg(clientId, entranceMsg.c_str());

	while (1) { 
	
		int length = recv(client.socket, received, sizeof(received), 0);
		received[length] = '\0';

		string msg = nicknameMsg+string(received)+"\n";

		if(length==0) cout<<"length 0\n";
		if(length==-1){
			cout<<"length -1\n";
			cout<<"errorno is "<<errno<<'\n';
		}
		if(length < 0){
			int errorNo = errno;
			strerror(errorNo);
		}
		


		if (strcmp(received, "[exit]") == 0) {
			msg = "["+string(nickname)+"]님 퇴장\n";
			broadCastMsg(clientId, msg.c_str());
			send(clientInfos[clientId].socket, "exit", sizeof("exit") , 0);
			cout<<msg;
			break;
		}
		broadCastMsg(clientId, msg.c_str());
	}

	clientInfos[clientId].connected = false;
	closesocket(client.socket);

}




int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	vector<thread> clientThreads;
	
	int serverPort = 9876;
	int clientId = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을초기화합니다.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을생성합니다.

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트정수를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재상태] listen()\n";


	while(1){
		int sizeClientAddress = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);	

		if (clientSocket == INVALID_SOCKET)
			ShowErrorMessage("accept()");

		printf("[클라이언트 접속] accept(): IP주소 = %s, 포트 번호 = %d\n", 
			inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

		clientInfos.push_back(clientInfo(clientSocket, clientAddress, clientId));
		clientThreads.push_back(thread(clientChat, clientId++));

	}

	for(int i=0;i<clientThreads.size();i++)
		clientThreads[i].join();
		
	closesocket(serverSocket);
	WSACleanup();
	system("pause");

	return 0;
}
