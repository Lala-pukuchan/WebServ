#ifndef CLIENT_REQ_H
#define CLIENT_REQ_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

//class ClientRequest : public ServerConfig
class ClientRequest
{

	private:
		string _method;
		string _contentLength;
		string _file_absolute_path;
		string _file_ext;
		string _request_message_body;

		// 再定義せずに、ServerConfigを継承
		vector<string> _allowedMethod;
		int _maxBodySize;

	public:
		ClientRequest ();
		~ClientRequest ();

		// getter
		string getMethod () const;
		string getContentLength () const;
		string getFileAbsolutePath() const;
		string getFileExt() const;
		vector<string> getAllowedMethod () const;
		int getMaxBodySize() const;
		string getRequestMessageBody() const;

};

#endif
