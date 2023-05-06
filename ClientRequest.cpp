#include "ClientRequest.hpp"

// constr
ClientRequest::ClientRequest (){
	// need to initialize firstly

	// from req
	_method = "PUT";
	_contentLength = "40";
	_file_absolute_path = "./test/test.txt";
	_file_ext = ".html";
	_request_message_body = "test2";

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

string ClientRequest::getFileAbsolutePath() const { return (_file_absolute_path); }

string ClientRequest::getFileExt() const { return (_file_ext); }

vector<string> ClientRequest::getAllowedMethod () const { return (_allowedMethod); }

int ClientRequest::getMaxBodySize() const { return (_maxBodySize); }

string ClientRequest::getRequestMessageBody() const { return (_request_message_body); }
