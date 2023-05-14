#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP


# include <iostream>
# include <string>
# include <unistd.h>
// # include <ctype.h>
// # include <errno.h>
// # include <signal.h>
// # include <stdlib.h>
// # include <sysexits.h>

# include <sys/param.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/wait.h>

# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>

# include <fcntl.h>

using namespace std;

class ServerSocket
{
	public:
		// Constructors
		ServerSocket(string port);
		
		// Destructor
		~ServerSocket();
		
		// Member Functions
		int makeSocket(string port);
		int getFd() const;
	private:
		int _socketFd;
		int result;
		int makeAddressInfo(string port, struct addrinfo **res);
};

#endif