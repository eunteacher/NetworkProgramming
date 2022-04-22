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

	echoStringLen = strlen(echoString) + 1; // 문자열의 '\0' 까지의 길이 

	// 네트워크 초기화
	WSAStartup(0x202, &wsaData);

	// UDP 소켓 생성
	socketValue = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketValue == INVALID_SOCKET)
	{
		cout << "Socket create error !!";
		return 0;
	}
	// sockaddr_in 구조체의 정보를 설정한다
	memset(&enhoServerAddr, 0, sizeof(enhoServerAddr));
	enhoServerAddr.sin_family = AF_INET;
	enhoServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	enhoServerAddr.sin_port = htons(8599);
	// 에코 서버에 메시지를 전송
	if (sendto(socketValue, echoString, echoStringLen, 0, (struct sockaddr*)&enhoServerAddr, sizeof(enhoServerAddr)) != echoStringLen)
	{
		__asm int 3;
		return 0;
	}

	// 에코 서버로부터 메시지가 도착하기를 기다린다.
	fromSize = sizeof(fromAddr);
	if ((recSTringLen = recvfrom(socketValue, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) != echoStringLen)
	{
		// echoStringLen 보낸 메시지의 길이만큼만 수신
		__asm int 3;
		return 0;
	}

	// 수신된 패킷을 보낸 서버가 에코 서버가 아닐 경우 에러 처리
	if (enhoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
	{
		cout << "Received a packet from unknown source";
		return 0;
	}

	// 에코 서버로부터 받은 메시지를 출력
	echoBuffer[recSTringLen] = '\0';
	cout << "Received " << recSTringLen << " " << "bytes : " << echoBuffer;

	// 소켓의 입/출력 버퍼를 비우고 비활성화 시킨다
	shutdown(socketValue, SD_BOTH);

	closesocket(socketValue);

	return 0;
}