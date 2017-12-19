
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
using namespace std;
#pragma  comment(lib,"ws2_32.lib")


int main(int argc, char* argv[])
{
	int pf = 0;
	char* ipstr = "";
	cout << "1:windows 2:linux" << endl;
	
	cin >> pf;
	if (pf == 1) {
		ipstr = "202.168.133.150";
	}
	else if (pf == 2) {
		ipstr = "202.168.133.195";
	}
	
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr(ipstr);
	//serAddr.sin_addr.S_un.S_addr = inet_addr("202.168.133.150");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	//char * sendData = "0016hello this is client! \n";
	char sendBuff[50];
	//char * sendData1 = "This is your order! \n";
	//send(sclient, sendData, strlen(sendData), 0);

	while (1) {
		cin >> sendBuff;
		send(sclient, sendBuff, strlen(sendBuff), 0);
		if (strcmp(sendBuff, "end")==0) {
			break;
		}
	}
	//send(sclient, sendData1, strlen(sendData1), 0);

	//int ret = recv(sclient, recData, 255, 0);
	//if (ret > 0)
	//{
	//	//recData[ret] = 0x00;
	//	printf(recData);
	//}

	closesocket(sclient);
	WSACleanup();

	return 0;
}