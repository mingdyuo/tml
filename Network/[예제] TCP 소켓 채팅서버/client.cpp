#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include<winsock2.h>
#include <thread>

using namespace std;
void ShowErrorMessage(string message)
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

void sendChat(SOCKET& clientSocket, string& nickname, bool& connected){
	string sent;

	while (1) { 
		cout << "["<<nickname<<"님]: ";
		getline(cin, sent);
		if (sent == ""){
			cout<<"공백쓰\n";
			 continue;
		}
		send(clientSocket, sent.c_str(), sent.length(), 0);
		if (sent == "[exit]") {
			connected = false;
			cout << "[대화 종료]\n";
			break;
		}
	}

}

void recvChat(SOCKET& clientSocket, bool& connected){
	char received[256];
	while (1) { 
		memset(&received, 0, sizeof(received));
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if(strcmp(received, "exit")==0) break;
	}
}


int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string nickname;



	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); 

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열IP를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");

	while(1){
		cout << "[연결됨] 대화에 사용할 닉네임을 입력하세요\n";
		getline(cin, nickname);
		if(nickname.empty())
			continue;
		send(clientSocket, nickname.c_str(), nickname.length(), 0);
		break;
	}

	cout<<"=======["<<nickname<<"]님이 입장하셨습니다.=======\n";

	bool connected = true;
	thread sendThread = thread(sendChat, ref(clientSocket), ref(nickname), ref(connected));
	thread recvThread = thread(recvChat, ref(clientSocket), ref(connected));

	sendThread.join();
	recvThread.join();

	if(connected)
		ShowErrorMessage("Shoud be unconnected!");
	
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}
