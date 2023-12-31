#include "Servers.hpp"
#include "Webserv.hpp"
#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

int main(int argc, char **argv)
{
	Servers servers;

	if (argc > 2)
	{
		std::cout << "usage: ./webserv [config_file]" << std::endl;
		return (1);
	}
	try
	{
		if (argc == 1)
			servers.ReadConfig("conf/default.conf");
		else
			servers.ReadConfig(argv[1]);
		servers.setServersConfig();
		servers.PrintServersConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	Webserv webserv(servers);
	signal(SIGPIPE, SIG_IGN);
	webserv.makeServerSocket();
	webserv.run();


	// std::ifstream inputFile("ClientRequestSample.txt");
	// std::stringstream buffer;
	// buffer << inputFile.rdbuf();

	// std::string requestMessage = buffer.str();
	// //for debug
	// std::cout << requestMessage << std::endl;
	// std::cout << "-------------------------------------" << std::endl;


	// ServerConfig server = servers.getDefaultServer();
	// ClientRequest clientrequest(requestMessage, server);

	// //for debug
	// clientrequest.PrintRequest();

	// ServerResponse res = ServerResponse(clientrequest);
	// cout << "--- response from server (START) ---" << endl;
	// cout << res.getResponse();
	// cout << "--- response from server (END)   ---" << endl;
	
	return (0);
}