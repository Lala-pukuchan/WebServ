#ifndef CLIENT_REQ_H
#define CLIENT_REQ_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

using namespace std;

class ClientRequest
{

	private:
		string _method;
		string _contentLength;
		// 見つからなかったらよしなに対応
		bool _is_content;
		string _transfer_encoding;
		string _file_absolute_path;

		string _file_ext;
		string _request_message_body;
		// 実行ファイルだった場合のみ、その後ろのパスを格納
		string _cgi_path_info;
		string _path;
		string _version;
		string _query_string;

		// added by yuhmatsu
		string _port;
		string _server_name;
		string _content_type;
		bool _is_cgi;
		bool _is_redirect;
		string _authorization;

		vector<string> _allowedMethod;
		int _maxBodySize;

		LocationConfig _location;

		ServerConfig _server;

		ClientRequest();
		void readClientRequest(std::string requestMessage);
		void setPath();

	public:
		ClientRequest (string requestMessage, ServerConfig Server);
		ClientRequest (string requestMessage);
		~ClientRequest ();

		// getter
		string getMethod () const;
		string getContentLength () const;
		bool getIsContent() const;
		string getTransferEncoding () const;
		string getFileAbsolutePath() const;
		string getFileExt() const;
		vector<string> getAllowedMethod () const;
		int getMaxBodySize() const;
		string getRequestMessageBody() const;
		string getPathInfo() const;
		bool getIsCgi() const;
		ServerConfig getServerConfig() const;
		LocationConfig getLocationConfig() const;
		string getHost() const;
		string getPort() const;
		string getContentType() const;
		string getQueryString() const;
		string getAuthorization() const;

		//for debug
		void PrintRequest();
};

#endif
