#include "ClientRequest.hpp"

ClientRequest::ClientRequest (){
	_method = "GET";
	_contentLength = 10;
	_allowedMethod.push_back("GET");
	_maxBodySize = 50;
}

ClientRequest::~ClientRequest (){}
