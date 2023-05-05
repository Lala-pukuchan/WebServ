#include "ClientRequest.hpp"

// constr
ClientRequest::ClientRequest (){
	// need to initialize firstly

	// from req
	_method = "HEAD";
	_contentLength = "40";
	_file_path = "./test/test.html";
	_file_ext = ".html";

	// from conf
	string m[] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	for (int i = 0; i < 7; i++){
		_allowedMethod.push_back(m[i]);
	}
	_maxBodySize = 50;
}

// destr
ClientRequest::~ClientRequest (){}

// getter
string ClientRequest::getMethod () const { return (_method); }

string ClientRequest::getContentLength () const { return (_contentLength); }

string ClientRequest::getFilePath() const { return (_file_path); }

string ClientRequest::getFileExt() const { return (_file_ext); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }
