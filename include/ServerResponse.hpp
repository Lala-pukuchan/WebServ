#ifndef SERVER_RES_H
#define SERVER_RES_H

#include "ClientRequest.hpp"
#include <dirent.h>
#include <ctime>

#define DEFAULT_ERROR_PAGE "./docs/error_page/default_error.html"
#define DEFAULT_INDEX_PAGE "index.html"
#define UPLOAD_PAGE "upload"

/* setting */
// mime-type
typedef pair<string, string> stringpair_t;
const stringpair_t mime[] = {
  stringpair_t(".txt", "text/plain"),
  stringpair_t(".html", "text/html"),
  stringpair_t(".htm", "text/html"),
  stringpair_t(".php", "text/html"),
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
extern map <string, string> mime_mapper;

// status
const stringpair_t status[] = {
  stringpair_t("200", "OK"),
  stringpair_t("201", "Created"),
  stringpair_t("204", "No Content"),
  stringpair_t("301", "Moved Permanently"),
  stringpair_t("403", "Forbidden"),
  stringpair_t("404", "Not Found"),
  stringpair_t("405", "Method Not Allowed"),
  stringpair_t("413", "Payload Too Large"),
  stringpair_t("500", "Internal Server Error")
};
const int status_size = sizeof(status) / sizeof(status[0]);
extern map <string, string> status_mapper;

class ServerResponse
{

	private:
		ClientRequest _req;
		LocationConfig _conf;
		string _res;
		string _method;
		string _file_true_path;
		string _file_ext;

		/* get time for res */
		string getCurrentTime();
		string getLastModifiedTime(const string& filePath);

		/* response getter / setter */
		void setResponse(string status_code, string response_message_body, string content_type);

		/* request checker */
		bool checkMethod();
		bool checkContentLength();
		bool checkPath();
		bool checkClientRequest();

		/* cgi */
		void getCgiResults();

		/* File Handler */
		bool existFile();
		void existIndexFile();
		bool getDir();
		string getErrorBody(int status_code);
		void getFile();
		void setFile();
		void setFile_for_PUT();
		void deleteFile();

		/* HTTP Method */
		void Get();
		void Post();
		void Put();
		void Delete();

	public:
		/* constructor / destructor */
		ServerResponse (ClientRequest &req);
		~ServerResponse ();

		/* response getter / setter */
		string getResponse () const;

};

#endif
