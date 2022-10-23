#include "Client.h"

void recvData(SOCKET& s) {
	char buffer[PACKET_SIZE];
	while (TRUE) {
		ZeroMemory(buffer, sizeof buffer);	//���۸� 0���� �ʱ�ȭ
		recv(s, buffer, sizeof buffer, 0);	//recv �Լ��� ��� �������κ��� ������ ������ �޾��ִ� ����
		std::cout << "Server : " << buffer << std::endl;
	}
}

int main() {
	char IP[PACKET_SIZE] = { 0 },
		name[PACKET_SIZE] = { 0 };

	std::cout << "User name : ";
	std::cin >> name;
	std::cout << "IP : ";
	std::cin >> IP;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	while (connect(server, (SOCKADDR*)&addr, sizeof addr));	//connect �Լ��� ������ ���Ͽ� ������ ����
	send(server, name, sizeof name, 0); //�̸�������

	std::thread(recvData, std::ref(server)).detach();	//recvData �Լ� ������ ����

	while (TRUE) {
		ZeroMemory(name, sizeof name);
		std::cout << "Message : ";
		std::cin >> name;
		send(server, name, sizeof name, 0);	//������ �޽��� ����
	}
}