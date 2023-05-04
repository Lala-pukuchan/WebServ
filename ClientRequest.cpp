#include "ClientRequest.hpp"

ClientRequest::ClientRequest (){
	// from req
	_method = "TRACE";
	_contentLength = "40";

	// from conf
	string m[] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	for (int i = 0; i < 7; i++){
		_allowedMethod.push_back(m[i]);
	}
	_maxBodySize = 50;
}

ClientRequest::~ClientRequest (){}

string ClientRequest::getMethod () const { return (_method); }

string ClientRequest::getContentLength () const { return (_contentLength); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }
