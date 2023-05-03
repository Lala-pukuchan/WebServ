#ifndef CLIENT_REQ_H
#define CLIENT_REQ_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//class ClientRequest : public ServerConfig
class ClientRequest
{

	private:
		string _method;
		string _contentLength;

		// 再定義せずに、ServerConfigを継承
		vector<string> _allowedMethod;
		int _maxBodySize;

	public:
		ClientRequest ();
		~ClientRequest ();

		string getMethod () const;
		string getContentLength () const;

		vector<string> getAllowedMethod () const;
		int getMaxBodySize() const;

};

#endif
