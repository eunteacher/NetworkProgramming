#include <iostream>
#include <WinSock2.h>
#include <sys/types.h>
#pragma comment(lib,"ws2_32")
using namespace std;

#define ECHOMAX 1024

int main()
{
	WSADATA wsaData;
	SOCKET socketListen, socketClient;
	struct sockaddr_in serverAddr;

	// 네트워크 초기화
	WSAStartup(0x202, &wsaData);

	// 소켓 생성
	socketClient = INVALID_SOCKET;
	socketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP 옵션
	if (socketListen == INVALID_SOCKET)
	{
		printf("Socket Create Error !! \n");
		return 0;
	}

	// 초기화
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8600);

	// 바인드
	if (bind(socketListen, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		printf("Bind Failed !! : %d\n", WSAGetLastError());
		return 0;
	}
	// listen : 소켓을 접속 요청을 받아들일 수 있는 상태로 만든다.  클라이언트로부터 접속 요청이 있을 때 사용한다.
	if (listen(socketListen, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen Failed !! : %d\n", WSAGetLastError());
		return 0;
	}

	while (1)
	{
		// 접속 처리, 클라이언트는 하나만 받는다.
		if (socketClient == INVALID_SOCKET)
		{
			fd_set fds;
			struct timeval tv = { 0,100 }; // 0.1초

			FD_ZERO(&fds);
			FD_SET(socketListen, &fds);

			select(0, &fds, 0, 0, &tv);
			if (FD_ISSET(socketListen, &fds))
			{
				struct sockaddr_in fromAddr;
				int size = sizeof(fromAddr);
				// 접속 요청을 Accept로 받아 클라이언트와 통신할 수 있는 소켓을 생성한다.
				socketClient = accept(socketListen, (struct sockaddr*)&fromAddr, &size);

				printf("Accepted A Client : %s\n", inet_ntoa(fromAddr.sin_addr));
			}
			else
			{
				// 접속이 이루어진 후에는 Echo 통신을 한다.
				char recvBuffer[127];
				int recvBytes;

				recvBytes = recv(socketClient, recvBuffer, 127, 0);

				printf("%d Bytes Received : %s\n", recvBytes, recvBuffer);

				send(socketClient, recvBuffer, recvBytes, 0);

				// 통신이 끝난 후에는 클라이언트의 접속을 해제한다.
				shutdown(socketClient, SD_BOTH);
				closesocket(socketClient);
				socketClient = INVALID_SOCKET;
			}

		}
	}

	WSACleanup();
	return 0;
}
