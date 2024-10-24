
#include <iostream>
#include <winsock2.h>

void sendMessage(SOCKET clientSocket, const char* message);

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

	// �޼��� ���� (Server -> Client)
	const char* Message = "Hello";
	sendMessage(ClientSocket, Message);
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

void sendMessage(SOCKET ClientSocket, const char* Message) 
{
	int MessageLength = static_cast<int>(strlen(Message));
	int Result = send(ClientSocket, Message, MessageLength, 0);
	if (SOCKET_ERROR == Result) 
	{
		std::cerr << "Failed Send Message: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "Send Message Successfully." << std::endl;
	}
}