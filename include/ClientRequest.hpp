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

using namespace std;

class ClientRequest
{

	private:
		string _method;
		string _contentLength;
		// 見つからなかったらよしなに対応
		string _file_absolute_path;

		string _file_ext;
		string _request_message_body;
		// 実行ファイルだった場合のみ、その後ろのパスを格納
		string _cgi_path_info;
		string _path;
		string _version;

		// added by yuhmatsu
		string _port;
		string _server_name;
		string _content_type;
		bool _is_cgi;

		vector<string> _allowedMethod;
		int _maxBodySize;

		ServerConfig _server;

		void readClientRequest(std::string requestMessage);
		void setPath();

	public:
		ClientRequest ();
		ClientRequest (string requestMessage, ServerConfig Server);
		~ClientRequest ();

		// getter
		string getMethod () const;
		string getContentLength () const;
		string getFileAbsolutePath() const;
		string getFileExt() const;
		vector<string> getAllowedMethod () const;
		int getMaxBodySize() const;
		string getRequestMessageBody() const;
		string getPathInfo() const;
		bool getIsCgi() const;

		//for debug
		void PrintRequest();
};

#endif
