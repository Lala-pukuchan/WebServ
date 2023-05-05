#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

std::map<std::string, std::string> mime_mapper (mime, mime + mime_size);

bool ServerResponse::methodCheck() {
    for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); ++i) {
		if (_req.getMethod() == _req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::contentLengthCheck(){ return (stoi(_req.getContentLength()) > _req.getMaxBodySize()); }

void ServerResponse::setRes(
	string status_code, string status, string response_message_body){

	// update res contents
	if (!status_code.empty())
		_status_code = status_code;
	if (!status.empty())
		_status = status;
	if (!response_message_body.empty())
		_response_message_body = response_message_body;
	
	// create res
	ostringstream os;
	os << 
		"HTTP/1.1 " << _status_code << " " << _status << "\r\n"
		<< "Content-Length: " << _response_message_body.size() << "\r\n"
		<< "Connection:close\r\n"
		<< "\r\n"
		<< _response_message_body;
	_res = os.str();
}

//void ServerResponse::Cgi(){}

void ServerResponse::Get(){
	cout << "GET method is called." << endl;
	
	// cgi


	// open file
	ifstream ifs(_req.getFilePath());
	if (!ifs.is_open())
	{
		cerr << "Filed to open file with this path: " << _req.getFilePath() << endl;
		setRes("404", "Not Found", "");
	}
	else
	{
		string content;
		string line;
		while (getline(ifs, line))
			content += line;
		ifs.close();
		setRes("200", "OK", content);
	}
}

void ServerResponse::Head(){
	cout << "HEAD method is called." << endl;
}

void ServerResponse::Post(){
	cout << "Post method is called." << endl;
}

void ServerResponse::Put(){
	cout << "Put method is called." << endl;
}

void ServerResponse::Delete(){
	cout << "Delete method is called." << endl;
}

void ServerResponse::Options(){
	cout << "Options method is called." << endl;
}

void ServerResponse::Trace(){
	cout << "Trace method is called." << endl;
}

ServerResponse::ServerResponse (ClientRequest &req) : 
	_req(req), _res(""), _status_code(""), _status(""), _response_message_body(""){

	// method / content length error
	bool m = false;
	if ((m = methodCheck()) || (contentLengthCheck())){
		if (m)
			setRes("405", "Method Not Allowed", "");
		else
			setRes("413", "Payload Too Large", "");
		return ;
	}

	// methods
	string methods[7] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	void (ServerResponse::*funcs[7])()
		= { &ServerResponse::Get, &ServerResponse::Head, &ServerResponse::Post, &ServerResponse::Put, 
			&ServerResponse::Delete, &ServerResponse::Options, &ServerResponse::Trace};

	// exe method func
	string method = _req.getMethod();
	for (int i = 0; i < 7; i++){
		if (method == methods[i])
		{
			(this->*funcs[i])();
			break;
		}
	}
}

ServerResponse::~ServerResponse (){}

string ServerResponse::getResponse () const { return (_res); }

//int main(void)
//{
//	ClientRequest req;
//	ServerResponse res = ServerResponse(req);
//	cout << "--- response from server ---" << endl;
//	cout << res.getResponse();
//	return (0);
//}