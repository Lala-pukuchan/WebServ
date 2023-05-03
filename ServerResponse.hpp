#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"

class ServerResponse
{

	private:
		ClientRequest _req;
		string _res;

		bool methodCheck(ClientRequest &req);
		bool contentLengthCheck();

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

};

#endif
