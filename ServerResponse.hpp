#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"

class ServerResponse
{

	private:
		ClientRequest _req;
		string _res;
		string _status_code;
		string _status;
		string _content_length;

		bool methodCheck(ClientRequest &req);
		bool contentLengthCheck(ClientRequest &req);

		void setStatus(string status_code, string status);
		void setResponse();

		void Cgi();

		void Get();
		void Head();
		void Post();
		void Put();
		void Delete();
		void Options();
		void Trace();

	public:
		ServerResponse (ClientRequest &req);
		~ServerResponse ();
		string getResponse () const;

};

#endif
