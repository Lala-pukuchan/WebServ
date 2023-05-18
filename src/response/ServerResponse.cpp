#include "ClientRequest.hpp"
#include "ServerResponse.hpp"
#include "CgiExe.hpp"

/* setting */
map<string, string> mime_mapper (mime, mime + mime_size);
map<string, string> status_mapper (status, status + status_size);

/* response getter / setter */
string ServerResponse::getResponse () const { return (_res); }

void ServerResponse::setResponse(string status_code, string response_message_body, string content_type){

	// init
	string res_content_length = "";
	string res_content_type = "";
	string res_response_message_body = "";
	string res_allow = "";

	// update res contents
	if (_method != "DELETE")
		res_content_length = "Content-Length: " + to_string(response_message_body.size()) + "\r\n";
	if (!content_type.empty())
		res_content_type = "Content-Type: " + content_type + "\r\n";
	res_response_message_body = response_message_body;
	if (status_code == "405")
	{
		res_allow = "Allow: ";
		for (int i = 0; i < static_cast<int>(_conf.getAllowedMethods().size()); i++){
			if (i != 0)
				res_allow += ",";
			res_allow += _conf.getAllowedMethods()[i];
    	}
		res_allow += "\r\n";
	}

	// create res
	ostringstream os;
	os << 
		"HTTP/1.1 " << status_code << " " << status_mapper.at(status_code) << "\r\n"
		<< res_allow
		<< res_content_length
		<< "Connection: close\r\n"
		<< res_content_type
		<< "\r\n"
		<< res_response_message_body;
	_res = os.str();
}

/* request checker */
bool ServerResponse::checkMethod() {
    for (int i = 0; i < static_cast<int>(_conf.getAllowedMethods().size()); i++) {
		if (_method == _conf.getAllowedMethods()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::checkContentLength(){ return (stoi(_req.getContentLength()) > _req.getMaxBodySize()); }

bool ServerResponse::checkPath(){ return (_file_true_path.empty()); }

bool ServerResponse::checkClientRequest() {
	if (checkMethod())
		setResponse("405", "", "");
	else if (checkContentLength())
		setResponse("413", "", "");
	else if (checkPath())
		setResponse("404", "", "");
	if (!_res.empty())
		return (true);
	return (false);
}

/* cgi */
void ServerResponse::getCgiResults(){
	CgiExe cgi(_req);
	cgi.exe();
	string cgiStatus;
	if ((cgiStatus = cgi.getStatus()) == "200")
		_res = cgi.getResult();
	else
		setResponse(cgiStatus, "", "");
}

/* File Handler */
bool ServerResponse::existFile(){
	struct stat buffer;
	return (!stat(_file_true_path.c_str(), &buffer));
}

void ServerResponse::existIndexFile(){
	string bkFile = _file_true_path;
	for (size_t i = 0; i < _conf.getIndexes().size(); ++i) {
		string index = _conf.getIndexes()[i];
		_file_true_path = bkFile + index;
		if (existFile()){
			size_t lastDotPos = index.find_last_of('.');
			if (lastDotPos != string::npos) {
				_file_ext = index.substr(lastDotPos, index.size() -1);
			}
			break;
		}
	}
}

bool ServerResponse::getDir(){

	bool isDir = false;
	string bkDir = _file_true_path;

	if (_file_ext.empty()){
		if (_file_true_path.at(_file_true_path.size() - 1) != '/')
			_file_true_path += "/";
		existIndexFile();
		if (!existFile()){
			isDir = true;
			if (_conf.getAutoindex() == "on"){
				DIR* dir;
				struct dirent* entry;
				char* dirPath = new char[bkDir.length() + 1];
    			strcpy(dirPath, bkDir.c_str());
				dir = opendir(dirPath);
				delete [] dirPath;
				if (!dir) {
					setResponse("404", "", "");
					return (isDir);
				}
				string content;
				while ((entry = readdir(dir))) {
					if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
					{
						if (!content.empty())
							content += ",";
						content += entry->d_name;
					}
				}
				content += "\n";
				setResponse("200", content, "text/plain");
				closedir(dir);
			} else
				setResponse("403", "", "");
		}
	}
	return (isDir);
}

void ServerResponse::getFile(){
	ifstream ifs(_file_true_path);
	if (!existFile()){
		setResponse("404", "", "");
	} else if (!ifs.is_open())
		setResponse("403", "", "");
	else
	{
		string content;
		string line;
		while (getline(ifs, line))
			content += line;
		setResponse("200", content, mime_mapper.at(_file_ext));
	}
	ifs.close();
}

void ServerResponse::setFile(){
	bool exist = false;
	ifstream ifs(_file_true_path);
	if ((exist = existFile()) && !ifs.is_open()){
		setResponse("403", "", "");
	} else {
		ofstream ofs(_file_true_path, ios::trunc);
		ofs << _req.getRequestMessageBody();
		if (!exist)
			setResponse("201", "create file success", "text/plain");
		else
			setResponse("204", "update file success", "text/plain");
		ofs.close();
	}
	ifs.close();
}

void ServerResponse::deleteFile(){
	ifstream ifs(_file_true_path);
	if (!existFile()){
		setResponse("404", "", "");
	} else {
		try {
			if (!remove(_file_true_path.c_str()))
				setResponse("200", "", "");
			else
				setResponse("403", "", "");
		} catch (const exception& e) {
			cout << e.what() << endl;
			setResponse("500", "", "");
		}
	}
	ifs.close();
}

/* HTTP Method */
void ServerResponse::Get(){
	if (_req.getIsCgi()){
		if (!existFile())
			setResponse("404", "", "");
		else
			getCgiResults();
	} else {
		if (!getDir())
			getFile();
	}
}

void ServerResponse::Post(){
	if (_req.getIsCgi())
		getCgiResults();
	else
		setFile();
}

void ServerResponse::Put(){ setFile(); }

void ServerResponse::Delete(){ deleteFile(); }

/* constructor / destructor */
ServerResponse::ServerResponse (ClientRequest &req) : 
	_req(req), _conf(req.getServerConfig()), _res(""), _method(req.getMethod()), _file_true_path(req.getFileAbsolutePath()), _file_ext(req.getFileExt()){

	if (checkClientRequest())
		return ;

	string methods[4] = { "GET", "POST", "PUT", "DELETE" };
	void (ServerResponse::*funcs[4])() = { &ServerResponse::Get, &ServerResponse::Post, &ServerResponse::Put, &ServerResponse::Delete };
	for (int i = 0; i < 4; i++){
		if (_method == methods[i])
		{
			(this->*funcs[i])();
			break;
		}
	}
}

ServerResponse::~ServerResponse (){}
