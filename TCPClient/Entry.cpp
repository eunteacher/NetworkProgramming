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

	// 네트워크 초기화
	WSAStartup(0x202, &wsaData);

	// 소켓 옵션으로 SOCK_STREAM을 지정하고, TCP소켓을 생성
	socketConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketConnect == INVALID_SOCKET)
	{
		cout << "Cannot Create Socket !!" << '\n';
		return 0;
	}

	// 접속할 서버의 정보를 설정한다.
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8600);

	if(connect(socketConnect,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cout << "Cannot Connect To Server !!" << '\n';
		return 0;
	}

	// 서버와 통신한다.
	{
		char sendBuffer[127] = "Test Client Message...";
		char recvBuffer[127];
		int sentBytes, recvBytes;

		sentBytes = send(socketConnect, sendBuffer, strlen(sendBuffer) + 1, 0); // 전송한 데이터의 크기를 반환
		cout << sentBytes << " bytes sent" << '\n';
		recvBytes = recv(socketConnect, recvBuffer, 127, 0);// 실제로 수신할 데이터의 크기를 반환
		cout << recvBytes << " bytes Received" << '\n' << recvBuffer << '\n';
	}

	shutdown(socketConnect, SD_BOTH);
	closesocket(socketConnect);
	WSACleanup();

	return 0;
}