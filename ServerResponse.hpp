#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"

class ServerResponse
{

	private:
		ClientRequest _req;
		string _res;

		bool methodCheck();
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
		ServerResponse (const ClientRequest &req);
		~ServerResponse ();

};

#endif
