edit: main.o epoll_server.o mailbox.o net_util.o
	g++ -o edit main.o epoll_server.o mailbox.o net_util.o
main.o :main.cpp epoll_server.h
	g++ -c main.cpp
epoll_server.o :epoll_server.cpp epoll_server.h mailbox.h
	g++ -c epoll_server.cpp
mailbox.o :mailbox.cpp mailbox.h
	g++ -c mailbox.cpp
net_util.o :net_util.cpp net_util.h
	g++ -c net_util.cpp
clean:
	rm edit main.o epoll_server.o mailbox.o net_util.o
