#include <iostream>
#include <WinSock2.h>
#include <sys/types.h>
#pragma comment(lib,"ws2_32")
using namespace std;

#define ECHOMAX 1024

int main()
{
	WSADATA wsaData;
	SOCKET socketConnect;
	struct sockaddr_in serverAddr;

	// ��Ʈ��ũ �ʱ�ȭ
	WSAStartup(0x202, &wsaData);

	// ���� �ɼ����� SOCK_STREAM�� �����ϰ�, TCP������ ����
	socketConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketConnect == INVALID_SOCKET)
	{
		cout << "Cannot Create Socket !!" << '\n';
		return 0;
	}

	// ������ ������ ������ �����Ѵ�.
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8600);

	if(connect(socketConnect,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cout << "Cannot Connect To Server !!" << '\n';
		return 0;
	}

	// ������ ����Ѵ�.
	{
		char sendBuffer[127] = "Test Client Message...";
		char recvBuffer[127];
		int sentBytes, recvBytes;

		sentBytes = send(socketConnect, sendBuffer, strlen(sendBuffer) + 1, 0); // ������ �������� ũ�⸦ ��ȯ
		cout << sentBytes << " bytes sent" << '\n';
		recvBytes = recv(socketConnect, recvBuffer, 127, 0);// ������ ������ �������� ũ�⸦ ��ȯ
		cout << recvBytes << " bytes Received" << '\n' << recvBuffer << '\n';
	}

	shutdown(socketConnect, SD_BOTH);
	closesocket(socketConnect);
	WSACleanup();

	return 0;
}