#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);

	WSAStartup(wVersionRequested, &wsaData);

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);

	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));


	while (1)
	{
		char szSendBuf[100] =  "123awer2342swwwee";
		scanf("%s", szSendBuf);
		send(sockClient, szSendBuf, strlen(szSendBuf) + 1, 0);
	}

	closesocket(sockClient);
	WSACleanup();

	return 0;
}