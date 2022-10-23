#include "Server.h"

void recvData(SOCKET s, int num) {
	char buffer[PACKET_SIZE] = { 0 };
	recv(s, buffer, sizeof buffer, 0); //User name �ޱ�
	Client[num].second = buffer;

	while (TRUE) {
		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0); //Message �ޱ�
		std::cout << Client[num].second << " : " << buffer << std::endl;
	}
}

void ACCEPT(SOCKET& s) { //Ŭ���̾�Ʈ���� ������ �����ϴ� �Լ�
	int cnt = 0;
	while (TRUE) {
		Client.push_back(pii(CLIENT(), ""));
		//client �� ���� ���� �� ������ ���� ����ü ���� �� �� �Ҵ�. Client�� ������ ��û�ϸ� �������ִ� ����
		//accept(����, ���� ������� ����ü�� �ּ�, ����ü�� ũ�⸦ ����ִ� ���� �ּ�)
		Client[cnt].first.client = accept(s, (SOCKADDR*)&Client[cnt].first.clientaddr, &Client[cnt].first.clientsize);
		Client[cnt].first.number = cnt;
		std::thread(recvData, Client[cnt].first.client, cnt).detach();	//recvData �Լ� ������ ����
		cnt += 1;
	}
}

int main() {
	WSADATA wsa;	//Windows�� ���� �ʱ�ȭ ������ �����ϱ� ���� ����ü
	WSAStartup(MAKEWORD(2, 2), &wsa);	//Windows�� ��� ������ Ȱ���� ������ �˷���.WSAStartup(���Ϲ���, WSADATA ����ü �ּ�);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	//AF_INET(IPv4), SOCK_STREAM(����������), IPPROTO_TCP(TCP ��������)���ڸ� ���� ���� ����

	SOCKADDR_IN addr = { 0 };	//sa_family�� AF_INET(IPv4)�� ��쿡 ����ϴ� ����ü
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	bind(server, (SOCKADDR*)&addr, sizeof addr);	//���Ͽ� ������ �ּ������� ����. bind(����, ���� ������� ����ü�� �ּ�, ����ü ũ��)
	listen(server, SOMAXCONN);	//������ �����ϴ� ���·� ���� ���� ����(���� ���� ��� ����). SOMAXCONN�� �ѹ��� ��û ������ �ִ� ���ӽ��� ��

	std::thread(ACCEPT, std::ref(server)).detach();	//ACCEPT �Լ� ���� �����带 ������ ��ü�� �и��Ͽ� ���������� ���

	char name[PACKET_SIZE],
		message[PACKET_SIZE];

	while (TRUE) {
		ZeroMemory(name, sizeof name);	//�޸� ���� �ּҺ��� �޸� ũ�⸸ŭ 0���� �ʱ�ȭ
		ZeroMemory(message, sizeof message);
		std::cin >> name >> message;
		for (int i = 0; i < Client.size(); i++)
			if (!strcmp(Client[i].second.c_str(), name))
				send(Client[i].first.client, message, sizeof message, 0);	//Client�� �޽��� ����
	}
}