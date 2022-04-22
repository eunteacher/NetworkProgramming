
// inet_ntoa() ���� ������ ���� �ڵ�, Ȥ�� SDL �˻縦 �ƴϿ��� ���� 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> // ���Ͽ� ���õ� ��� 
#include <iostream>

#pragma comment(lib,"ws2_32") // ws2_32 ���̺귯�� ��ũ 

#define ECHOMAX 512 // ������ ũ��

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET listenSocket;
	struct sockaddr_in enhoServerAddr;;
	struct sockaddr_in enhoClientAddr;
	char echoBuffer[ECHOMAX];
	int receiverSize, clinetAddrLen;

	WSAStartup(0x202, &wsaData);

	// UDP ���� ����
	listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Socket create error !!";
		return 0;
	}

	// sockaddr_in ����ü�� ������ �����Ѵ�
	memset(&enhoServerAddr, 0, sizeof(enhoServerAddr));
	enhoServerAddr.sin_family = AF_INET;
	enhoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	enhoServerAddr.sin_port = htons(8599);
	
	// ���� ���ε�
	if (bind(listenSocket, (struct sockaddr*)&enhoServerAddr, sizeof(enhoServerAddr)) != 0)
	{
		cout << "bind() error occured!";
		return 0;
	}

	while (1)
	{
		clinetAddrLen = sizeof(enhoClientAddr);

		// ��Ŷ�� ���ŵ� �� ���� ����Ѵ�.
		receiverSize = recvfrom(listenSocket, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&enhoClientAddr, &clinetAddrLen);

		if (receiverSize < 0)
		{
			cout << "recvfrom() < 0";
		}

		cout << "Handling client - " << inet_ntoa(enhoClientAddr.sin_addr) << '\n' << receiverSize << " Bytes : " << echoBuffer;

		// ���� ���� �����͸� �ٽ� Ŭ���̾�Ʈ�� �����Ѵ�.
		if (sendto(listenSocket, echoBuffer, receiverSize, 0, (struct sockaddr*)&enhoClientAddr, sizeof(enhoClientAddr)) != receiverSize)
		{
			__asm int 3;
			break;
		}
	}

	shutdown(listenSocket, SD_BOTH);
	
	closesocket(listenSocket);

	return 0;
}