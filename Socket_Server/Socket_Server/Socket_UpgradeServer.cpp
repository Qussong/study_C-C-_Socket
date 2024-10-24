
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

	// Winsock �ʱ�ȭ
	Result = WSAStartup(MAKEWORD(2, 2), &WSA);
	if (NO_ERROR != Result)
	{
		std::cerr << "Init Winsock Failed " << std::endl;
		return 1;
	}
	std::cout << "Init Winsock Successful" << std::endl;


	// ���� ����
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cerr << "Could not Create Socket" << std::endl;
		return 1;
	}
	std::cout << "Create Socket Successful" << std::endl;

	// ���� �ּ� ����ü ����
	Server.sin_family = AF_INET;
	Server.sin_port = htons(8080);			// ��Ʈ ��ȣ 8080
	Server.sin_addr.s_addr = INADDR_ANY;	// ��� IP�κ��� ���� ���

	// ���Ͽ� ip, port ���ε�
	Result = bind(ServerSocket, (struct sockaddr*)&Server, sizeof(Server));
	if (Result == SOCKET_ERROR)
	{
		std::cerr << "Bind Failed: " << WSAGetLastError() << std::endl;
		return 1;
	}
	std::cout << "Bind Successful" << std::endl;

	// ���� ���
	Result = listen(ServerSocket, 3);
	if (Result == SOCKET_ERROR)
	{
		std::cerr << "Waiting Failed: " << WSAGetLastError() << std::endl;
		return 1;
	}
	std::cout << "Waiting for Connection..." << std::endl;

	// Ŭ�� ���� ����
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&Client, &ClientSize);
	if (INVALID_SOCKET == ClientSocket)
	{
		std::cerr << "Accept Failed: " << WSAGetLastError() << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Client Connected" << std::endl;

	//  �������� ��ȭ ����
	while (1)
	{
		// Ŭ��κ��� �޼��� ����
		receiveMessage(ClientSocket);
	}


	// ���� �ݱ�
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
	// ��� ���
	if (0 == RecvSize)
	{
		std::cout << "Connection closed by Client." << std::endl;
	}
	else
	{
		Buffer[RecvSize] = '\0'; // Add null - terminator, ���� �÷ο� ����
		std::cout << "Client Reply (" << RecvSize << " bytes): " << Buffer << std::endl;
	}
}
