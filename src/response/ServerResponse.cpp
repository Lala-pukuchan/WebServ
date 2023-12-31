#include "ClientRequest.hpp"
#include "ServerResponse.hpp"
#include "CgiExe.hpp"

/* setting */
map<string, string> mime_mapper (mime, mime + mime_size);
map<string, string> status_mapper (status, status + status_size);

/* get time */
string ServerResponse::getCurrentTime() {
    time_t currentTime = time(nullptr);
    tm* currentTimeStruct = gmtime(&currentTime);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", currentTimeStruct);
    return static_cast<string>(buffer);
}

string ServerResponse::getLastModifiedTime(const string& filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0)
        return "";
    time_t lastModifiedTime = fileStat.st_mtime;
    tm* lastModifiedTimeStruct = gmtime(&lastModifiedTime);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", lastModifiedTimeStruct);
    return string(buffer);
}

/* response getter / setter */
string ServerResponse::getResponse () const { return (_res); }

void ServerResponse::setResponse(string status_code, string response_message_body, string content_type){

	// init
	string res_content_length = "";
	string res_content_type = "";
	string res_response_message_body = "";
	string res_allow = "";
	string res_location = "";
	string last_modified_time = "";

	// update res contents
	if (_method != "DELETE")
		res_content_length = "Content-Length: " + to_string(response_message_body.size()) + "\r\n";
	if (!content_type.empty()){
		res_content_type = "Content-Type: " + content_type + "\r\n";
		if (status_code == "200")
			last_modified_time = "Last-Modified: " + getLastModifiedTime(_file_true_path) + "\r\n";
	} else
		res_content_type = "Content-Type: " + mime_mapper.at(".html") + "\r\n";
	
	res_response_message_body = response_message_body;

	if (status_code == "405"){
		res_allow = "Allow: ";
		for (int i = 0; i < static_cast<int>(_conf.getAllowedMethods().size()); i++){
			if (i != 0)
				res_allow += ",";
			res_allow += _conf.getAllowedMethods()[i];
    	}
		res_allow += "\r\n";
	}
	if (status_code == "301"){
		try {
			res_location = "Location: " + _conf.getReturnRedirect().at(301) + "\r\n";
		} catch (const exception& e){
			cerr << "Failt to take redirection url. " << e.what() << endl;
		}
	}

	ostringstream os;
	os << 
		"HTTP/1.1 " << status_code << " " << status_mapper.at(status_code) << "\r\n"
		<< "Server: Webserv/1.0\r\n"
		<< "Date: " << getCurrentTime() << "\r\n"
		<< res_allow
		<< res_location
		<< res_content_type
		<< res_content_length
		<< last_modified_time
		<< "Connection: close\r\n"
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

bool ServerResponse::checkContentLength(){
	if (_req.getContentLength().empty())
		return (false);
	return (stoi(_req.getContentLength()) > _conf.getMaxBodySize());
}

bool ServerResponse::checkPath(){ return (_file_true_path.empty()); }

bool ServerResponse::checkClientRequest() {
	if (checkMethod())
		setResponse("405", getErrorBody(405), "");
	else if (checkContentLength())
		setResponse("413", getErrorBody(413), "");
	else if (checkPath())
		setResponse("404", getErrorBody(404), "");
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
		setResponse(cgiStatus, getErrorBody(stoi(cgiStatus)), "");
}

/* File Handler */
bool ServerResponse::existFile(){
	struct stat buffer;
	return (!stat(_file_true_path.c_str(), &buffer));
}

void ServerResponse::existIndexFile(){
	string bkFile = _file_true_path;
	if (!_conf.getIndexes().size()){
		_file_true_path = bkFile + DEFAULT_INDEX_PAGE;
		_file_ext = ".html";
	} else {
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
			if (_conf.getAutoindex()){
				DIR* dir;
				struct dirent* entry;
				char* dirPath = new char[bkDir.length() + 1];
    			strcpy(dirPath, bkDir.c_str());
				dir = opendir(dirPath);
				delete [] dirPath;
				if (!dir) {
					setResponse("404", getErrorBody(404), "");
					return (isDir);
				}
				stringstream content;
				content << "<html><head><title>Index of /</title></head><body><h1>Index of /</h1><ul>";
				while ((entry = readdir(dir))) {
					if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
						content << "<li>" << entry->d_name << "</li>";
				}
				content << "</ul></body></html>";
				setResponse("200", content.str(), "text/html");
				closedir(dir);
			} else
				setResponse("404", getErrorBody(404), "");
		}
	}
	return (isDir);
}

string ServerResponse::getErrorBody(int status_code){
	string content = "";
	string error_page = "";
	try {
		error_page = _conf.getErrorPage().at(status_code);
	} catch (const exception& e) {
		error_page = DEFAULT_ERROR_PAGE;
	}
	ifstream ifs(error_page);
	if (!ifs.is_open())
		content = "";
	else {
		stringstream buffer;
		buffer << ifs.rdbuf();
		content = buffer.str();
	}
	ifs.close();
	return (content);
}

void ServerResponse::getFile(){
	ifstream ifs(_file_true_path);
	if (!ifs.is_open())
		setResponse("404", getErrorBody(404), "");
	else {
		string content;
		stringstream buffer;
		buffer << ifs.rdbuf();
		content = buffer.str();
		string ext = "";
		try {
			ext = mime_mapper.at(_file_ext);			
		} catch (const exception& e) {
			ext = "application/octet-stream";
		}
		setResponse("200", content, ext);
	}
	ifs.close();
}

void ServerResponse::setFile(){
	ifstream ifs(_file_true_path);
	DIR* dir;
	dir = opendir(_file_true_path.c_str());
	if (!dir){
		setResponse("404", getErrorBody(404), "");
	} else {
		_file_true_path += UPLOAD_PAGE;
		ofstream ofs(_file_true_path, ios::trunc);
		ofs << _req.getRequestMessageBody();
		setResponse("201", "File Created.", "text/plain");
		ofs.close();
	}
	ifs.close();
}

void ServerResponse::setFile_for_PUT(){
	bool exist = false;
	ifstream ifs(_file_true_path);
	if ((exist = existFile()) && !ifs.is_open()){
		setResponse("403", getErrorBody(403), "");
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
		setResponse("404", getErrorBody(404), "");
	} else {
		try {
			if (!remove(_file_true_path.c_str()))
				setResponse("204", "", "");
			else
				setResponse("403", getErrorBody(403), "");
		} catch (const exception& e) {
			cout << e.what() << endl;
			setResponse("500", getErrorBody(500), "");
		}
	}
	ifs.close();
}

/* HTTP Method */
void ServerResponse::Get(){
	if (_req.getIsCgi()){
		getCgiResults();
	} else if (_conf.getReturnRedirect().size() != 0) {
		setResponse("301", getErrorBody(301), "");
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

void ServerResponse::Put(){ setFile_for_PUT(); }

void ServerResponse::Delete(){ deleteFile(); }

/* constructor / destructor */
ServerResponse::ServerResponse (ClientRequest &req) : 
	_req(req), _conf(req.getLocationConfig()), _res(""), _method(req.getMethod()), _file_true_path(req.getFileAbsolutePath()), _file_ext(req.getFileExt()){

	req.PrintRequest();
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
