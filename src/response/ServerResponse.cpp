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
	//if (_method == "OPTIONS")
	//{
	//	res_allow = "Allow: ";
	//	for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); i++){
	//		if (i != 0)
	//			res_allow += ",";
	//		res_allow += _req.getAllowedMethod()[i];
    //	}
	//	res_allow += "\r\n";
	//}

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
    for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); i++) {
		if (_method == _req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::checkContentLength(){ return (stoi(_req.getContentLength()) > _req.getMaxBodySize()); }

bool ServerResponse::checkClientRequest() {
	bool m = false;
	if ((m = checkMethod()) || (checkContentLength())){
		if (m)
			setResponse("405", "", "");
		else
			setResponse("413", "", "");
		return (true);
	}
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

void ServerResponse::getFile(){
	ifstream ifs(_file_true_path);
	//if (isDir())
	//{
	//	cout << "path is directory: " << path << endl;
	//  directoryListing();
	//}
	//else 
	if (!ifs.is_open())
		setResponse("404", "", "");
	else
	{
		string content;
		string line;
		while (getline(ifs, line))
			content += line;
		setResponse("200", content, mime_mapper.at(_req.getFileExt()));
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
	if (_req.getIsCgi())
		getCgiResults();
	else
		getFile();
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
	_req(req), _res(""), _method(_req.getMethod()), _file_true_path(_req.getFileAbsolutePath()){

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
