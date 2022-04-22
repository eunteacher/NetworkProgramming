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

	// ��Ʈ��ũ �ʱ�ȭ
	WSAStartup(0x202, &wsaData);

	// ���� ����
	socketClient = INVALID_SOCKET;
	socketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP �ɼ�
	if (socketListen == INVALID_SOCKET)
	{
		printf("Socket Create Error !! \n");
		return 0;
	}

	// �ʱ�ȭ
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8600);

	// ���ε�
	if (bind(socketListen, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		printf("Bind Failed !! : %d\n", WSAGetLastError());
		return 0;
	}
	// listen : ������ ���� ��û�� �޾Ƶ��� �� �ִ� ���·� �����.  Ŭ���̾�Ʈ�κ��� ���� ��û�� ���� �� ����Ѵ�.
	if (listen(socketListen, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen Failed !! : %d\n", WSAGetLastError());
		return 0;
	}

	while (1)
	{
		// ���� ó��, Ŭ���̾�Ʈ�� �ϳ��� �޴´�.
		if (socketClient == INVALID_SOCKET)
		{
			fd_set fds;
			struct timeval tv = { 0,100 }; // 0.1��

			FD_ZERO(&fds);
			FD_SET(socketListen, &fds);

			select(0, &fds, 0, 0, &tv);
			if (FD_ISSET(socketListen, &fds))
			{
				struct sockaddr_in fromAddr;
				int size = sizeof(fromAddr);
				// ���� ��û�� Accept�� �޾� Ŭ���̾�Ʈ�� ����� �� �ִ� ������ �����Ѵ�.
				socketClient = accept(socketListen, (struct sockaddr*)&fromAddr, &size);

				printf("Accepted A Client : %s\n", inet_ntoa(fromAddr.sin_addr));
			}
			else
			{
				// ������ �̷���� �Ŀ��� Echo ����� �Ѵ�.
				char recvBuffer[127];
				int recvBytes;

				recvBytes = recv(socketClient, recvBuffer, 127, 0);

				printf("%d Bytes Received : %s\n", recvBytes, recvBuffer);

				send(socketClient, recvBuffer, recvBytes, 0);

				// ����� ���� �Ŀ��� Ŭ���̾�Ʈ�� ������ �����Ѵ�.
				shutdown(socketClient, SD_BOTH);
				closesocket(socketClient);
				socketClient = INVALID_SOCKET;
			}

		}
	}

	WSACleanup();
	return 0;
}
