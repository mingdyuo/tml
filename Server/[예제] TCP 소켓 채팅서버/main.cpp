#include "IOCPServer.h"
#include <string>
#include <iostream>

const UINT16 SERVER_PORT = 9898;
const UINT16 MAX_CLIENT = 100;		//총 접속할수 있는 클라이언트 수

int main()
{
	IOCPServer iocpServer;

	iocpServer.InitSocket();
	iocpServer.BindAndListen(SERVER_PORT);
	iocpServer.StartServer(MAX_CLIENT);

	printf("아무 키나 누를 때까지 대기합니다\n");
	while (true)
	{
		std::string inputCmd;
		std::getline(std::cin, inputCmd);

		if (inputCmd == "quit")
		{
			break;
		}
	}

	iocpServer.DestroyThread();
	return 0;
}

