
#include <iostream>
#include <winsock2.h>

int main()
{
	WSADATA WSA;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in Server, Client;
	int ClientSize;

	// Winsock 초기화
	WSAStartup(MAKEWORD(2, 2), &WSA);

	// 소켓 생성
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Could not Create Socket" << std::endl;
		return 1;
	}

	// 서버 주소 구조체 설정
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = INADDR_ANY;	// 모든 IP로부터 연결 허용
	Server.sin_port = htons(8080);			// 포트 번호 8080

	// 소켓에 ip, port 바인딩
	if (bind(ServerSocket, (struct sockaddr*)&Server, sizeof(Server)) 
		== SOCKET_ERROR)
	{
		std::cout << "Bind failed" << std::endl;
		return 1;
	}

	// 연결 대기
	listen(ServerSocket, 3);

	std::cout << "Waiting for Connection..." << std::endl;
	ClientSize = sizeof(struct sockaddr_in);

	// 클라 연결 수락
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&Client, &ClientSize);
	if (ClientSocket != INVALID_SOCKET)
	{
		std::cout << "Client Connected" << std::endl;
		const char* Message = "Hello \n- from Server -";
		send(ClientSocket, Message, (int)strlen(Message), 0);

		// 클라이언트 소켓 닫기
		//closesocket(ClientSocket);
	}

	closesocket(ServerSocket);
	WSACleanup();
	return 0;
}
