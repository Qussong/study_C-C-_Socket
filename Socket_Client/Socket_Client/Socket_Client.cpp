
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // inet_pton 사용을 위한 헤더 추가

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA WSA;
	SOCKET Socket;
	struct sockaddr_in Server;
	char Buffer[2000];

	// Winsock 초기화
	WSAStartup(MAKEWORD(2, 2), &WSA);

	// 소켓 생성
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Could not Create Socket" << std::endl;
		return 1;
	}

	// 서버 주소 구조체 설정
	Server.sin_family = AF_INET;
	// IP 주소 설정
	//Server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	if (inet_pton(AF_INET, "127.0.0.1", &Server.sin_addr) <= 0) // inet_pton 사용
	{
		std::cout << "Invalid address" << std::endl;
		return 1;
	}
	// 포트 번호 8080
	Server.sin_port = htons(8080);			

	// 서버에 연결
	if (connect(Socket, (struct sockaddr*)&Server, sizeof(Server)) < 0)
	{
		std::cout << "Connection Failed" << std::endl;
		return 1;
	}

	// 서버로부터 메세지 받기
	int RecvSize = recv(Socket, Buffer, 2000, 0);
	Buffer[RecvSize] = '\0';
	std::cout << "Server Reply : " << Buffer << std::endl;

	// 소켓닫기
	closesocket(Socket);
	WSACleanup();
	return 0;
}