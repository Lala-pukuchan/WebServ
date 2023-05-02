#ifndef CLIENT_REQ_H
#define CLIENT_REQ_H

#include <string>
#include <vector>

using namespace std;

class ClientRequest
{

	private:
		string _method;
		int _contentLength;
		vector<string> _allowedMethod;
		int _maxBodySize;

	public:
		ClientRequest ();
		~ClientRequest ();

};

#endif
