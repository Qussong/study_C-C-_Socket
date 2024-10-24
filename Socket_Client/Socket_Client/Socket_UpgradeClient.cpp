
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // inet_pton ����� ���� ��� �߰�

#pragma comment(lib, "Ws2_32.lib")

void sendMessage(SOCKET ClientSocket);

int main()
{
	WSADATA WSA;
	SOCKET ClientSocket;
	struct sockaddr_in Server;
	int Result;

	// Winsock �ʱ�ȭ
	Result = WSAStartup(MAKEWORD(2, 2), &WSA);
	if (NO_ERROR != Result)
	{
		std::cerr << "Init Winsock Failed" << std::endl;
		return 1;
	}
	std::cout << "Init Winsock Successful" << std::endl;

	// ���� ����
	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == ClientSocket)
	{
		std::cerr << "Could not Create Socket" << std::endl;
		return 1;
	}
	std::cout << "Create Socket Successful" << std::endl;

	// ���� �ּ� ����ü ����
	Server.sin_family = AF_INET;	// IPv4
	Server.sin_port = htons(8080);	// ��Ʈ ��ȣ 8080
	Result = inet_pton(AF_INET, "127.0.0.1", &Server.sin_addr); // IP �ּ� ����
	if (Result == 0)
	{
		std::cerr << "Invalid IPv4 address format." << std::endl;
		return 1;
	}
	else if (Result < 0)
	{
		std::cerr << "Error occurred during conversion." << std::endl;
		return 1;
	}
	std::cout << "IPv4 address successfully converted." << std::endl;

	// ������ ����
	Result = connect(ClientSocket, (struct sockaddr*)&Server, sizeof(Server));
	if (SOCKET_ERROR == Result)
	{
		std::cerr << "Connection Failed" << std::endl;
		return 1;
	}
	std::cout << "Connection Successful" << std::endl;

	// ���������� ������ �޼��� ������
	while(true)
	{ 
		sendMessage(ClientSocket);
	}

	WSACleanup();
	return 0;
}

void sendMessage(SOCKET ClientSocket)
{
	char Message[1024] = {};
	std::cout << "Input Message : ";
	std::cin.getline(Message, sizeof(Message)/sizeof(char));

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
