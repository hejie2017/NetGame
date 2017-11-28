#include <winsock2.h> 
#include <stdio.h> 
#pragma comment(lib, "ws2_32.lib") 
int main()
{
	// ����win socket 
	WSADATA ws;
	int ret;
	ret = WSAStartup(MAKEWORD(2, 2), &ws);
	if (ret != 0)
	{
		printf("WSAStartup() ʧ��!\n");
		return -1;
	}
	// ��������SOCKET 
	int sListen;
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (sListen == INVALID_SOCKET)
	{
		printf("socket() ʧ��!\n");
		return -1;
	}
	// ����������ַ�ṹ 
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;
	servAddr.sin_port = htons(8888);

	// �󶨷������׽��� 
	ret = bind(sListen, (sockaddr*)&servAddr, sizeof(servAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() ʧ��!\n");
		return -1;
	}

	// ��ʼ���� 
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() ʧ��!\n");
		return -1;
	}
	printf("�����������ɹ����ڶ˿�%d������\n", ntohs(servAddr.sin_port));
	//ʹ��selectģ�� 
	// �����׽��ּ��� 
	fd_set allSockSet; // �ܵ��׽��ּ��� 
	fd_set readSet; // �ɶ��׽��ּ��� 
	fd_set writeSet; // ��д�׽��ּ���

	FD_ZERO(&allSockSet); // ����׽��ּ��� 
	FD_SET(sListen, &allSockSet); // ��sListen�׽��ּ����׽��ּ����� 
	char bufRecv[100]; // ���ջ�����
					   // �����������ѭ�� 
	while (1)
	{
		FD_ZERO(&readSet); // ��տɶ��׽��� 
		FD_ZERO(&writeSet); // ��տ�д�׽��� 
		readSet = allSockSet; // ��ֵ 
		writeSet = allSockSet; // ��ֵ 
							   // ����select������timeout����ΪNULL 
		ret = select(0, &readSet, 0, NULL, NULL);
		//
		if (ret == SOCKET_ERROR)
		{
			printf("select() ʧ��!\n");
			return -1;
		}
		// �����׽��ֵ�I/O�Ѿ�׼���� 
		if (ret > 0)
		{
			// ���������׽��� 
			for (int i = 0; i < allSockSet.fd_count; ++i)
			{
				int s = allSockSet.fd_array[i];
				// ���ڿɶ����׽��� 
				if (FD_ISSET(s, &readSet))
				{
					// �ɶ��׽���ΪsListen 
					if (s == sListen)
					{
						// �����µ����� 
						sockaddr_in clientAddr;
						int len = sizeof(clientAddr);
						int sClient = accept(s, (sockaddr*)&clientAddr, &len);
						// ���´������׽��ּ��뵽������ 
						FD_SET(sClient, &allSockSet);
						printf(">>>>>���µ����ӵ�������\n");
						printf("Ŀǰ�ͻ�����ĿΪ��%d\n", allSockSet.fd_count - 1);
					}
					else // ���տͻ�����Ϣ 
					{
						ret = recv(s, bufRecv, 100, 0);
						// ���մ��� 
						if (ret == SOCKET_ERROR)
						{
							DWORD err = WSAGetLastError();
							if (err == WSAECONNRESET)
								printf("�ͻ��˱�ǿ�йر�\n");
							else
								printf("recv() ʧ��!");
							// ɾ���׽��� 
							FD_CLR(s, &allSockSet);
							printf("Ŀǰ�ͻ�����ĿΪ��%d\n", allSockSet.fd_count - 1);
							break;
						}
						if (ret == 0)
						{
							printf("�ͻ����Ѿ��˳�!\n");
							// ɾ���׽��� 
							FD_CLR(s, &allSockSet);
							printf("Ŀǰ�ͻ�����ĿΪ��%d\n", allSockSet.fd_count - 1);
							break;
						}
						bufRecv[ret] = '\0';
						printf("�յ�����Ϣ��%s\n", bufRecv);
					} // end else

				}// end if

			}// end for 
		} // end if 
	}//end while 
	return 0;
}