#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"

class ServerResponse
{

	private:
		// req & res
		ClientRequest _req;
		string _res;

		// to create res
		string _status_code;
		string _status;
		string _content_length;

		// error check
		bool methodCheck();
		bool contentLengthCheck();

		// create res
		void setStatus(string status_code, string status);
		void setResponse();

		// cgi exe
		void Cgi();

		// http method
		void Get();
		void Head();
		void Post();
		void Put();
		void Delete();
		void Options();
		void Trace();

	public:
		// const & dest
		ServerResponse (ClientRequest &req);
		~ServerResponse ();

		// getter
		string getResponse () const;

};

#endif
