
#include <iostream>
#include <winsock2.h>

int main()
{
	WSADATA WSA;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in Server, Client;
	int ClientSize;

	// Winsock �ʱ�ȭ
	WSAStartup(MAKEWORD(2, 2), &WSA);

	// ���� ����
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Could not Create Socket" << std::endl;
		return 1;
	}

	// ���� �ּ� ����ü ����
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = INADDR_ANY;	// ��� IP�κ��� ���� ���
	Server.sin_port = htons(8080);			// ��Ʈ ��ȣ 8080

	// ���Ͽ� ip, port ���ε�
	if (bind(ServerSocket, (struct sockaddr*)&Server, sizeof(Server)) 
		== SOCKET_ERROR)
	{
		std::cout << "Bind failed" << std::endl;
		return 1;
	}

	// ���� ���
	listen(ServerSocket, 3);

	std::cout << "Waiting for Connection..." << std::endl;
	ClientSize = sizeof(struct sockaddr_in);

	// Ŭ�� ���� ����
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&Client, &ClientSize);
	if (ClientSocket != INVALID_SOCKET)
	{
		std::cout << "Client Connected" << std::endl;
		const char* Message = "Hello \n- from Server -";
		send(ClientSocket, Message, (int)strlen(Message), 0);

		// Ŭ���̾�Ʈ ���� �ݱ�
		//closesocket(ClientSocket);
	}

	closesocket(ServerSocket);
	WSACleanup();
	return 0;
}
