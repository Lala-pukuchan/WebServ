#ifndef CLIENT_REQ_H
#define CLIENT_REQ_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class ClientRequest
{

	private:
		string _method;
		int _contentLength;

		// 再定義せずに、ServerConfigを継承
		vector<string> _allowedMethod;
		int _maxBodySize;

	public:
		ClientRequest ();
		~ClientRequest ();
		string getMethod () const;
		vector<string> getAllowedMethod () const;

};

#endif
