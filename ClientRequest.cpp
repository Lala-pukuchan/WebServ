#include "ClientRequest.hpp"

ClientRequest::ClientRequest (){
	_method = "GET";
	_contentLength = "10";
	_allowedMethod.push_back("GET");
	_allowedMethod.push_back("POST");
	_maxBodySize = 50;
}

ClientRequest::~ClientRequest (){}

string ClientRequest::getMethod () const { return (_method); }

string ClientRequest::getContentLength () const { return (_contentLength); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }
