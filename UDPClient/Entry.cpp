#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32")
using namespace std;

#define ECHOMAX 512

int main()
{
	WSADATA wsaData;
	SOCKET socketValue;
	struct sockaddr_in enhoServerAddr;;
	struct sockaddr_in fromAddr;
	char echoString[127] = "Echo test message\n", echoBuffer[ECHOMAX + 1];
	int echoStringLen, recSTringLen;
	int fromSize;

	echoStringLen = strlen(echoString) + 1; // ���ڿ��� '\0' ������ ���� 

	// ��Ʈ��ũ �ʱ�ȭ
	WSAStartup(0x202, &wsaData);

	// UDP ���� ����
	socketValue = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketValue == INVALID_SOCKET)
	{
		cout << "Socket create error !!";
		return 0;
	}
	// sockaddr_in ����ü�� ������ �����Ѵ�
	memset(&enhoServerAddr, 0, sizeof(enhoServerAddr));
	enhoServerAddr.sin_family = AF_INET;
	enhoServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	enhoServerAddr.sin_port = htons(8599);
	// ���� ������ �޽����� ����
	if (sendto(socketValue, echoString, echoStringLen, 0, (struct sockaddr*)&enhoServerAddr, sizeof(enhoServerAddr)) != echoStringLen)
	{
		__asm int 3;
		return 0;
	}

	// ���� �����κ��� �޽����� �����ϱ⸦ ��ٸ���.
	fromSize = sizeof(fromAddr);
	if ((recSTringLen = recvfrom(socketValue, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) != echoStringLen)
	{
		// echoStringLen ���� �޽����� ���̸�ŭ�� ����
		__asm int 3;
		return 0;
	}

	// ���ŵ� ��Ŷ�� ���� ������ ���� ������ �ƴ� ��� ���� ó��
	if (enhoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
	{
		cout << "Received a packet from unknown source";
		return 0;
	}

	// ���� �����κ��� ���� �޽����� ���
	echoBuffer[recSTringLen] = '\0';
	cout << "Received " << recSTringLen << " " << "bytes : " << echoBuffer;

	// ������ ��/��� ���۸� ���� ��Ȱ��ȭ ��Ų��
	shutdown(socketValue, SD_BOTH);

	closesocket(socketValue);

	return 0;
}