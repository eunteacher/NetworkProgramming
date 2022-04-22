
// inet_ntoa() 에러 방지를 위한 코드, 혹은 SDL 검사를 아니오로 변경 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> // 소켓에 관련된 헤더 
#include <iostream>

#pragma comment(lib,"ws2_32") // ws2_32 라이브러리 링크 

#define ECHOMAX 512 // 버퍼의 크기

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

	// UDP 소켓 생성
	listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Socket create error !!";
		return 0;
	}

	// sockaddr_in 구조체의 정보를 설정한다
	memset(&enhoServerAddr, 0, sizeof(enhoServerAddr));
	enhoServerAddr.sin_family = AF_INET;
	enhoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	enhoServerAddr.sin_port = htons(8599);
	
	// 소켓 바인딩
	if (bind(listenSocket, (struct sockaddr*)&enhoServerAddr, sizeof(enhoServerAddr)) != 0)
	{
		cout << "bind() error occured!";
		return 0;
	}

	while (1)
	{
		clinetAddrLen = sizeof(enhoClientAddr);

		// 패킷이 수신될 때 까지 대기한다.
		receiverSize = recvfrom(listenSocket, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&enhoClientAddr, &clinetAddrLen);

		if (receiverSize < 0)
		{
			cout << "recvfrom() < 0";
		}

		cout << "Handling client - " << inet_ntoa(enhoClientAddr.sin_addr) << '\n' << receiverSize << " Bytes : " << echoBuffer;

		// 수신 받은 데이터를 다시 클라이언트로 전송한다.
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