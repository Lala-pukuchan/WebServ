#include "ClientRequest.hpp"
#include "ServerConfig.hpp"

# define RE "\033[31m"
# define GR "\033[32m"
# define YE "\033[33m"
# define CY "\033[36m"
# define RC "\033[0m"
# define BOLD "\033[1m"

ClientRequest::ClientRequest () : _is_cgi(false), _is_redirect(false)
{
}

ClientRequest::ClientRequest(string requestMessage) : _is_cgi(false), _is_redirect(false)
{
	readClientRequest(requestMessage);
}

ClientRequest::ClientRequest(string requestMessage, ServerConfig Server) : _is_cgi(false), _is_redirect(false)
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
	_query_string = _path.substr(_path.find("?") + 1);
	_path = _path.substr(0, _path.find("?"));
	if (_path[_path.size() - 1] == '/' && _path.size() > 1)
		_path = _path.substr(0, _path.size() - 1);
	// get version
	iss_first_line >> _version;

	// get second line
	getline(iss, line);
	std::istringstream iss_second_line(line);
	iss_second_line >> tmp >> value;
	_server_name = value.substr(0, value.find(":"));
	_port = value.substr(value.find(":") + 1);
	_is_content = false;
	_transfer_encoding = "";
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
		{
			_contentLength = value;
			_is_content = true;
		}
		if (key == "Content-Type:")
			_content_type = value;
		if (key == "Transfer-Encoding:")
			_transfer_encoding = value;
		if (key == "Authorization:")
			_authorization = value;
	}
	// get message body
	for (std::string line; getline(iss, line);)
		_request_message_body += line + "\n";

	setPath();
	if (_transfer_encoding == "chunked")
		mergeChunkedBody();
}

void ClientRequest::setPath()
{
	std::map<std::string, LocationConfig> locations = _server.getLocationConfigs();
	size_t LongestMatch = 0;
	std::string LongestMatchPath = "";
	for (std::map<std::string, LocationConfig>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::string location_path = it->first;
		if (location_path[0] == '*')
			continue ;
		if (_path.compare(0, location_path.length(), location_path) == 0 && location_path.length() > LongestMatch)
		{
			LongestMatch = location_path.length();
			LongestMatchPath = location_path;
		}
	}
	if (LongestMatchPath != "")
	{
		_location = locations[LongestMatchPath];
		std::vector<std::string> allowedmethods = _location.getAllowedMethods();
		// redirectが存在したら
		if (!(_location.getReturnRedirect().empty()))
		{
			_is_redirect = true;
			_file_absolute_path = _location.getReturnRedirect().begin()->second;
		}
		else
		{
			std::string sub_path = _path.substr(LongestMatchPath.length());
			if (sub_path[0] == '/')
				sub_path = sub_path.substr(1);
			if (sub_path.find(".") != std::string::npos)
			{
				std::string after_dot = sub_path.substr(sub_path.find("."));
				_file_ext = after_dot.substr(0, after_dot.find("/"));
				std::vector<std::string> cgi_extensions = _location.getCgiExtension();
				//print cgi_extensions
				for (std::vector<std::string>::iterator it = cgi_extensions.begin(); it != cgi_extensions.end(); it++)
				{
					if (*it == _file_ext)
						_is_cgi = true;
				}
				if (_is_cgi)
					_file_absolute_path = _location.getAlias() + sub_path;
				else
				{
					_file_absolute_path = _location.getAlias() + sub_path.substr(0, sub_path.find("/"));
					if (sub_path.find("/") != std::string::npos)
						_cgi_path_info = sub_path.substr(sub_path.find("/"));
				}
			}
			else
				_file_absolute_path = _location.getAlias() + sub_path;
		}
		if (_file_ext == "")
			return ;
		for (std::map<std::string, LocationConfig>::iterator it = locations.begin(); it != locations.end(); it++)
		{
			std::string location_path = it->first;
			if (location_path[0] != '*')
				continue ;
			if (_file_ext == location_path.substr(1))
			{
				_location = locations[location_path];
				return ;
			}
		}
	}
}

void ClientRequest::PrintRequest()
{
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "method: " << _method << std::endl;
	std::cout << "path: " << _path << std::endl;
	std::cout << "query_string: " << _query_string << std::endl;
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
	std::cout << "is_redirect: " << _is_redirect << std::endl;
	std::cout << "-------------------------------------" << std::endl;
}

// destr
ClientRequest::~ClientRequest () {}

// getter
string ClientRequest::getMethod () const { return (_method); }

string ClientRequest::getContentLength () const { return (_contentLength); }

bool ClientRequest::getIsContent() const { return (_is_content); }

string ClientRequest::getTransferEncoding() const { return (_transfer_encoding); }

string ClientRequest::getFileAbsolutePath() const { return (_file_absolute_path); }

string ClientRequest::getFileExt() const { return (_file_ext); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }

string ClientRequest::getRequestMessageBody() const { return (_request_message_body); }

string ClientRequest::getPathInfo() const { return (_path); }

bool ClientRequest::getIsCgi() const { return (_is_cgi); }

ServerConfig ClientRequest::getServerConfig() const { return (_server); }

LocationConfig ClientRequest::getLocationConfig() const { return (_location); }

string ClientRequest::getHost() const { return (getLocationConfig().getServerName()); }

string ClientRequest::getPort() const { return (getLocationConfig().getPortString()); }

string ClientRequest::getContentType() const { return (_content_type); }

string ClientRequest::getQueryString() const { return (_query_string); }

string ClientRequest::getAuthorization() const { return (_authorization); }

void ClientRequest::mergeChunkedBody() {
    std::string line;
    std::string result;

	// Read the _request_message_body if chunked
	std::istringstream iss(_request_message_body);

    while (std::getline(iss, line)) {
		try {
			if (line == "\r")
				continue; // Skip the CR character

			// Get the size of the chunk
			unsigned int size = std::strtoul(line.c_str(), NULL, 16);

			// Break if we reached the last chunk
			if (size == 0)
				break;

			char *buffer = new char[size+1];

			// Read the chunk data
			iss.read(buffer, size);

			// Ensure the data is null terminated
			buffer[size] = '\0';

			// Append the chunk data to the result
			result.append(buffer);

			// Clean up
			delete [] buffer;

			// Read the trailing CR/LF
			std::getline(iss, line);
		} catch (std::exception &e) {
			std::cout << "Error: " << e.what() << std::endl;
			return ;
		}
    }

    _request_message_body = result;
}