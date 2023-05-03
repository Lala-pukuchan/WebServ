#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

bool ServerResponse::methodCheck(ClientRequest &req) {
    for (int i = 0; i < static_cast<int>(req.getAllowedMethod().size()); ++i) {
		if (req.getMethod() == req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::contentLengthCheck(ClientRequest &req){
	return (stoi(req.getContentLength()) > req.getMaxBodySize());
}

void ServerResponse::setResponse(){
	// _res = ...
}

//void ServerResponse::Cgi(){}
//void ServerResponse::Get(){}
//void ServerResponse::Head(){}
//void ServerResponse::Post(){}
//void ServerResponse::Put(){}
//void ServerResponse::Delete(){}
//void ServerResponse::Options(){}
//void ServerResponse::Trace(){}

void ServerResponse::setStatus(string status_code, string status){
	_status_code = status_code;
	_status = status;
}

// content-LengthはClientReqからデフォルト設定していいのか？
ServerResponse::ServerResponse (ClientRequest &req) : _status_code("200"), _status("OK"), _content_length(req.getContentLength()){
	bool m = false;
	if ((m = methodCheck(req)) || (contentLengthCheck(req))){
		if (m)
			setStatus("405", "NG");
		else
			setStatus("413", "NG");
		setResponse();
		return ;
	}
}

ServerResponse::~ServerResponse (){}

string ServerResponse::getResponse () const { return (_res); }
