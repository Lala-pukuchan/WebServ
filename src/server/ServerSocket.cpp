#include "ServerSocket.hpp"

// Constructors
ServerSocket::ServerSocket(string port)
{
	result = makeSocket(port);
}

// Destructor
ServerSocket::~ServerSocket() { }

// Member Functions
int ServerSocket::makeSocket(string port) {
	
	struct addrinfo *res = NULL;
	// int				errcode;

	if (makeAddressInfo(port, &res) == -1)
		return (-1);
	
	_socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_socketFd == -1) {
		perror("socket");
		freeaddrinfo(res);
		return (-1);
	}
	/* ソケットオプション（再利用フラグ）設定 */
	int opt = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		perror("setsockopt");
		close(_socketFd);
		freeaddrinfo(res);
		return (-1);
	}
	/* ソケットにアドレスを指定 */
	if (bind(_socketFd, res->ai_addr, res->ai_addrlen) == -1) {
		perror("bind");
		close(_socketFd);
		freeaddrinfo(res);
		return (-1);
	}
	/* アクセスバックログの指定 */
	if (listen(_socketFd, SOMAXCONN) == -1) {
		perror("listen");
		close(_socketFd);
		freeaddrinfo(res);
		return (-1);
	}
	fcntl(_socketFd, F_SETFL, O_NONBLOCK);
	freeaddrinfo(res);
	return (0);
}

// getter
int ServerSocket::getFd() const
{
	return _socketFd;
}


//private
int ServerSocket::makeAddressInfo(string port, struct addrinfo **res)
{
	struct addrinfo	hints;
	int				errcode;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((errcode = getaddrinfo(NULL, port.c_str(), &hints, res)) != 0) {
		perror("getaddrinfo");
		return (-1);
	}
	return (0);
}