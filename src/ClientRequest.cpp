#include "ClientRequest.hpp"
#include "ServerConfig.hpp"

// constr
ClientRequest::ClientRequest () : _is_cgi(false)
{
	// need to initialize firstly

	// from req
	_method = "POST";
	_contentLength = "40";
	_file_absolute_path = "./test/cgi_tester";
	_file_ext = ".html";
	_request_message_body = "aaa";
	_path = "a";

	// from conf
	string m[] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	for (int i = 0; i < 7; i++){
		_allowedMethod.push_back(m[i]);
	}
	_maxBodySize = 50;
}

ClientRequest::ClientRequest(string requestMessage, ServerConfig Server) : _is_cgi(false)
{
	_server = Server;
	readClientRequest(requestMessage);
}

void ClientRequest::readClientRequest(std::string requestMessage)
{
	std::istringstream iss(requestMessage);

	string line;
	string tmp;
	string value;

	// get first line
	getline(iss, line);
	std::istringstream iss_first_line(line);
	// get method
	iss_first_line >> _method;
	// get path_info
	iss_first_line >> _path;
	// get version
	iss_first_line >> _version;

	// get second line
	getline(iss, line);
	std::istringstream iss_second_line(line);
	iss_second_line >> tmp >> value;
	_server_name = value.substr(0, value.find(":"));
	_port = value.substr(value.find(":") + 1);
	while (1)
	{
		string new_line;
		string key;
		getline(iss, new_line);
		std::istringstream iss_line(new_line);
		iss_line >> key >> value;
		if (key.empty())
			break;
		if (key == "Content-Length:")
			_contentLength = value;
		if (key == "Content-Type:")
			_content_type = value;
	}
	// get message body
	for (std::string line; getline(iss, line);)
		_request_message_body += line + "\n";
	
	setPath();
}

void ClientRequest::setPath()
{
	std::map<std::string, LocationConfig> locations = _server.getLocationConfigs();
	size_t LongestMatch = 0;
	std::string LongestMatchPath = "";
	for (std::map<std::string, LocationConfig>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::string location_path = it->first;
		if (_path.find(location_path) == 0 && location_path.length() > LongestMatch)
		{
			LongestMatch = location_path.length();
			LongestMatchPath = location_path;
		}
	}
	if (LongestMatchPath != "")
	{
		LocationConfig location = locations[LongestMatchPath];
		std::string sub_path = _path.substr(LongestMatchPath.length());
		if (sub_path.find(".") != std::string::npos)
		{
			_is_cgi = true;
			_file_absolute_path = location.getAlias() + sub_path.substr(0, sub_path.find("/"));
			_file_ext = (sub_path.substr(sub_path.find("."))).substr(0, sub_path.find("/") - 1);
			if (sub_path.find("/") != std::string::npos)
				_cgi_path_info = sub_path.substr(sub_path.find("/"));
		}
		else
			_file_absolute_path = location.getAlias() + sub_path;
	}
}

void ClientRequest::PrintRequest()
{
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "method: " << _method << std::endl;
	std::cout << "path: " << _path << std::endl;
	std::cout << "version: " << _version << std::endl;
	std::cout << "server_name: " << _server_name << std::endl;
	std::cout << "port: " << _port << std::endl;
	std::cout << "contentLength: " << _contentLength << std::endl;
	std::cout << "content_type: " << _content_type << std::endl;
	std::cout << "request_message_body: " << std::endl << _request_message_body;
	std::cout << "file_absolute_path: " << _file_absolute_path << std::endl;
	std::cout << "is_cgi: " << _is_cgi << std::endl;
	std::cout << "file_ext: " << _file_ext << std::endl;
	std::cout << "cgi_path_info: " << _cgi_path_info << std::endl;
	std::cout << "-------------------------------------" << std::endl;
}

// destr
ClientRequest::~ClientRequest () {}

// getter
string ClientRequest::getMethod () const { return (_method); }

string ClientRequest::getContentLength () const { return (_contentLength); }

string ClientRequest::getFileAbsolutePath() const { return (_file_absolute_path); }

string ClientRequest::getFileExt() const { return (_file_ext); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }

string ClientRequest::getRequestMessageBody() const { return (_request_message_body); }

string ClientRequest::getPathInfo() const { return (_path); }

bool ClientRequest::getIsCgi() const { return (_is_cgi); }
