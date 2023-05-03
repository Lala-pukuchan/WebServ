#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

bool ServerResponse::methodCheck(ClientRequest &req) {
    for (int i = 0; i < static_cast<int>(req.getAllowedMethod().size()); ++i) {
		if (req.getMethod() == req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

//bool ServerResponse::contentLengthCheck(){}
//void ServerResponse::Cgi(){}
//void ServerResponse::Get(){}
//void ServerResponse::Head(){}
//void ServerResponse::Post(){}
//void ServerResponse::Put(){}
//void ServerResponse::Delete(){}
//void ServerResponse::Options(){}
//void ServerResponse::Trace(){}

ServerResponse::ServerResponse (ClientRequest &req) {
	if (methodCheck(req)){
		cout << "methoderror";
	}
}

ServerResponse::~ServerResponse (){}
