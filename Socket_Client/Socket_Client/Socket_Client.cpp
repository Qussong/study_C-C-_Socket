
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // inet_pton ����� ���� ��� �߰�

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA WSA;
	SOCKET Socket;
	struct sockaddr_in Server;
	char Buffer[2000];

	// Winsock �ʱ�ȭ
	WSAStartup(MAKEWORD(2, 2), &WSA);

	// ���� ����
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Could not Create Socket" << std::endl;
		return 1;
	}

	// ���� �ּ� ����ü ����
	Server.sin_family = AF_INET;
	// IP �ּ� ����
	//Server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	if (inet_pton(AF_INET, "127.0.0.1", &Server.sin_addr) <= 0) // inet_pton ���
	{
		std::cout << "Invalid address" << std::endl;
		return 1;
	}
	// ��Ʈ ��ȣ 8080
	Server.sin_port = htons(8080);			

	// ������ ����
	if (connect(Socket, (struct sockaddr*)&Server, sizeof(Server)) < 0)
	{
		std::cout << "Connection Failed" << std::endl;
		return 1;
	}

	// �����κ��� �޼��� �ޱ�
	int RecvSize = recv(Socket, Buffer, 2000, 0);
	Buffer[RecvSize] = '\0';
	std::cout << "Server Reply : " << Buffer << std::endl;

	// ���ϴݱ�
	closesocket(Socket);
	WSACleanup();
	return 0;
}