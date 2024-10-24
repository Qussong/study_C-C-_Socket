
#include <iostream>
#include <winsock2.h>

void receiveMessage(SOCKET ClientSocket);

int main()
{
	WSADATA WSA;	// Window Socket API
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in Server, Client;
	int ClientSize = sizeof(Client);
	int Result = 0;

	// Winsock 초기화
	Result = WSAStartup(MAKEWORD(2, 2), &WSA);
	if (NO_ERROR != Result)
	{
		std::cerr << "Init Winsock Failed " << std::endl;
		return 1;
	}
	std::cout << "Init Winsock Successful" << std::endl;


	// 소켓 생성
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cerr << "Could not Create Socket" << std::endl;
		return 1;
	}
	std::cout << "Create Socket Successful" << std::endl;

	// 서버 주소 구조체 설정
	Server.sin_family = AF_INET;
	Server.sin_port = htons(8080);			// 포트 번호 8080
	Server.sin_addr.s_addr = INADDR_ANY;	// 모든 IP로부터 연결 허용

	// 소켓에 ip, port 바인딩
	Result = bind(ServerSocket, (struct sockaddr*)&Server, sizeof(Server));
	if (Result == SOCKET_ERROR)
	{
		std::cerr << "Bind Failed: " << WSAGetLastError() << std::endl;
		return 1;
	}
	std::cout << "Bind Successful" << std::endl;

	// 연결 대기
	Result = listen(ServerSocket, 3);
	if (Result == SOCKET_ERROR)
	{
		std::cerr << "Waiting Failed: " << WSAGetLastError() << std::endl;
		return 1;
	}
	std::cout << "Waiting for Connection..." << std::endl;

	// 클라 연결 수락
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&Client, &ClientSize);
	if (INVALID_SOCKET == ClientSocket)
	{
		std::cerr << "Accept Failed: " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Client Connected" << std::endl;

	//  지속적인 대화 루프
	while (1)
	{
		// 클라로부터 메세지 수신
		receiveMessage(ClientSocket);
	}


	// 소켓 닫기
	Result = closesocket(ClientSocket);
	if (SOCKET_ERROR == Result)
	{
		std::cerr << "Failed Close Client Socket: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "Client Socket Closed Successfully." << std::endl;
	}
	Result = closesocket(ServerSocket);
	if (SOCKET_ERROR == Result)
	{
		std::cerr << "Failed Close Server Socket: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "Server Socket Closed Successfully." << std::endl;
	}

	WSACleanup();
	return 0;
}

void receiveMessage(SOCKET ClientSocket)
{
	char Buffer[1024] = {};
	int RecvSize = recv(ClientSocket, Buffer, sizeof(Buffer) - 1, 0); // -1 to leave space for null-terminator
	if (SOCKET_ERROR == RecvSize)
	{
		std::cerr << "Receive Failed with Error: " << WSAGetLastError() << std::endl;
	}
	// 결과 출력
	if (0 == RecvSize)
	{
		std::cout << "Connection closed by Client." << std::endl;
	}
	else
	{
		Buffer[RecvSize] = '\0'; // Add null - terminator, 오버 플로우 방지
		std::cout << "Client Reply (" << RecvSize << " bytes): " << Buffer << std::endl;
	}
}
