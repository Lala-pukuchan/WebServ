#ifndef CGI_EXE_H
#define CGI_EXE_H

#include "ClientRequest.hpp"
#include <unistd.h>

#define BUFFER_SIZE 42

class CgiExe
{

	private:
		ClientRequest _req;
		string _result;
		string _status;
		const char *_cmd;
		char **_arg;
		char **_env;

	public:
		CgiExe (ClientRequest &req);
		~CgiExe ();
		void exe ();
		string getResult() const;
		string getStatus() const;
};

#endif
