#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"

typedef std::pair<std::string, std::string> stringpair_t;
const stringpair_t mime[] = {
  stringpair_t(".txt", "text/plain"),
  stringpair_t(".html", "text/html"),
  stringpair_t(".htm", "text/html"),
  stringpair_t(".xml", "text/xml"),
  stringpair_t(".js", "text/javascript"),
  stringpair_t(".vbs", "text/vbscript"),
  stringpair_t(".css", "text/css"),
  stringpair_t(".gif", "image/gif"),
  stringpair_t(".jpg", "image/jpeg"),
  stringpair_t(".jpeg", "image/jpeg"),
  stringpair_t(".png", "image/png"),
  stringpair_t(".cgi", "application/x-httpd-cgi"),
  stringpair_t(".doc", "application/msword"),
  stringpair_t(".pdf", "application/pdf")
};
const int mime_size = sizeof(mime) / sizeof(mime[0]);
extern std::map <std::string, std::string> mime_mapper;

class ServerResponse
{

	private:
		// req & res
		ClientRequest _req;
		string _res;
		string _method;
		string _file_absolute_path;

		// error check
		bool methodCheck();
		bool contentLengthCheck();

		// create res
		void setRes(string status_code, string status, string response_message_body, string content_type);

		// cgi exe
		void Cgi();

		// handle file
		bool existFile();
		void getFileContents();
		void updateFileContents();

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
