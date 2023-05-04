#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

bool ServerResponse::methodCheck() {
    for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); ++i) {
		if (_req.getMethod() == _req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::contentLengthCheck(){
	return (stoi(_req.getContentLength()) > _req.getMaxBodySize());
}

void ServerResponse::setResponse(){
	_res = "HTTP/1.1 " + _status_code + " " + _status + "\r\n";
}

//void ServerResponse::Cgi(){}
void ServerResponse::Get(){
	cout << "GET method is called.";
}

void ServerResponse::Head(){
	cout << "HEAD method is called.";
}

void ServerResponse::Post(){
	cout << "Post method is called.";
}

void ServerResponse::Put(){
	cout << "Put method is called.";
}

void ServerResponse::Delete(){
	cout << "Delete method is called.";
}

void ServerResponse::Options(){
	cout << "Options method is called.";
}

void ServerResponse::Trace(){
	cout << "Trace method is called.";
}

void ServerResponse::setStatus(string status_code, string status){
	_status_code = status_code;
	_status = status;
}

// content-LengthはClientReqからデフォルト設定していいのか？
ServerResponse::ServerResponse (ClientRequest &req) : 
	_req(req), _status_code("200"), _status("OK"), _content_length(req.getContentLength()){

	// method / content length error
	bool m = false;
	if ((m = methodCheck()) || (contentLengthCheck())){
		if (m)
			setStatus("405", "Method Not Allowed");
		else
			setStatus("413", "Payload Too Large");
		setResponse();
		return ;
	}

	// execute targeted method func
	string methods[7] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	void (ServerResponse::*funcs[])()
		= { &ServerResponse::Get, &ServerResponse::Head, &ServerResponse::Post, &ServerResponse::Put, &ServerResponse::Delete, &ServerResponse::Options, &ServerResponse::Trace};
	string method = _req.getMethod();
	for (int i = 0; i < 7; i++){
		if (method == methods[i])
			(this->*funcs[i])();
	}
}

ServerResponse::~ServerResponse (){}

string ServerResponse::getResponse () const { return (_res); }
