#include "ClientRequest.hpp"
#include "ServerResponse.hpp"
#include "CgiExe.hpp"

map<string, string> mime_mapper (mime, mime + mime_size);
map<string, string> status_mapper (status, status + status_size);

bool ServerResponse::methodCheck() {
    for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); i++) {
		if (_method == _req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::contentLengthCheck(){ return (stoi(_req.getContentLength()) > _req.getMaxBodySize()); }

void ServerResponse::setRes(string status_code, string response_message_body, string content_type){

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
	if (_method != "HEAD")
		res_response_message_body = response_message_body;
	if (_method == "OPTIONS")
	{
		res_allow = "Allow: ";
		for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); i++){
			if (i != 0)
				res_allow += ",";
			res_allow += _req.getAllowedMethod()[i];
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

bool ServerResponse::isCgi(){
	// check whether cgi or not
	return (true);
	// return (false);
}

bool ServerResponse::existFile(){
	struct stat buffer;
	return (!stat(_file_absolute_path.c_str(), &buffer));
}

void ServerResponse::getFileContents(){
	ifstream ifs(_file_absolute_path);
	//if (isDir())
	//{
	//	cout << "path is directory: " << path << endl;
	//  directoryListing();
	//}
	//else 
	if (!ifs.is_open())
		setRes("404", "", "");
	else
	{
		string content;
		string line;
		while (getline(ifs, line))
			content += line;
		setRes("200", content, mime_mapper.at(_req.getFileExt()));
	}
	ifs.close();
}

void ServerResponse::Get(){
	// cgi / get file
	if (isCgi()){
		CgiExe cgi(_req);
		cgi.exe();
		string status;
		if ((status = cgi.getStatus()) == "200")
			_res = cgi.getResult();
		else
			setRes(status, "", "");
	} else
		getFileContents();
}

void ServerResponse::Head(){
	// get file, but not return contents
	getFileContents();
}

void ServerResponse::Post(){

	// cgi / upload file
	if (isCgi()){
		CgiExe cgi(_req);
		cgi.exe();
		string status;
		if ((status = cgi.getStatus()) == "200")
			_res = cgi.getResult();
		else
			setRes(status, "", "");
	} else
		setRes("200", "POST success", "text/plain");
}

void ServerResponse::Put(){

	// create / update file
	bool exist = false;
	ifstream ifs(_file_absolute_path);
	if ((exist = existFile()) && !ifs.is_open()){
		setRes("403", "", "");
	} else {
		ofstream ofs(_file_absolute_path, ios::trunc);
		ofs << _req.getRequestMessageBody();
		if (!exist)
			setRes("201", "", "");
		else
			setRes("204", "", "");
		ofs.close();
	}
	ifs.close();
}

void ServerResponse::Delete(){

	// delte file
	ifstream ifs(_file_absolute_path);
	if (!existFile()){
		setRes("404", "", "");
	} else {
		try {
			if (!remove(_file_absolute_path.c_str()))
				setRes("200", "", "");
			else
				setRes("403", "", "");
		} catch (const exception& e) {
			cout << e.what() << endl;
			setRes("500", "", "");
		}
	}
	ifs.close();
}

void ServerResponse::Options(){
	// return allow methods
	setRes("200", "", "");
}

ServerResponse::ServerResponse (ClientRequest &req) : 
	_req(req), _res(""), _method(_req.getMethod()), _file_absolute_path(_req.getFileAbsolutePath()){

	// method / content length error
	bool m = false;
	if ((m = methodCheck()) || (contentLengthCheck())){
		if (m)
			setRes("405", "", "");
		else
			setRes("413", "", "");
		return ;
	}

	// methods
	string methods[6] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS" };
	void (ServerResponse::*funcs[6])()
		= { &ServerResponse::Get, &ServerResponse::Head, &ServerResponse::Post, &ServerResponse::Put, 
			&ServerResponse::Delete, &ServerResponse::Options };

	// exe method func
	for (int i = 0; i < 6; i++){
		if (_method == methods[i])
		{
			(this->*funcs[i])();
			break;
		}
	}
}

ServerResponse::~ServerResponse (){}

string ServerResponse::getResponse () const { return (_res); }

int main(void)
{
	ClientRequest req;
	ServerResponse res = ServerResponse(req);
	cout << "--- response from server (START) ---" << endl;
	cout << res.getResponse();
	cout << "--- response from server (END)   ---" << endl;
	return (0);
}

//__attribute__((destructor))
//static void destructor() {
//	system("leaks -q a.out");
//}
