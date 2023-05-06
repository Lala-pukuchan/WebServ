#include "ClientRequest.hpp"
#include "ServerResponse.hpp"

std::map<std::string, std::string> mime_mapper (mime, mime + mime_size);

bool ServerResponse::methodCheck() {
    for (int i = 0; i < static_cast<int>(_req.getAllowedMethod().size()); i++) {
		if (_method == _req.getAllowedMethod()[i])
			return (false);
    }
	return (true);
}

bool ServerResponse::contentLengthCheck(){ return (stoi(_req.getContentLength()) > _req.getMaxBodySize()); }

void ServerResponse::setRes(
	string status_code, string status, string response_message_body, string content_type){

	// init
	string res_content_length = "";
	string res_content_type = "";
	string res_response_message_body = "";

	// update res contents
	if (_method != "DELETE")
		res_content_length = "Content-Length: " + to_string(response_message_body.size()) + "\r\n";
	if (!content_type.empty())
		res_content_type = "Content-Type: " + content_type + "\r\n";
	if (_method != "HEAD")
		res_response_message_body = response_message_body;
	
	// create res
	ostringstream os;
	os << 
		"HTTP/1.1 " << status_code << " " << status << "\r\n"
		<< res_content_length
		<< "Connection:close\r\n"
		<< res_content_type
		<< "\r\n"
		<< res_response_message_body;
	_res = os.str();
}

void ServerResponse::Cgi(){}

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
	{
		cout << "Filed to open file with this path: " << _file_absolute_path << endl;
		setRes("404", "Not Found", "", "");
	}
	else
	{
		string content;
		string line;
		while (getline(ifs, line))
			content += line;
		ifs.close();
		setRes("200", "OK", content, mime_mapper.at(_req.getFileExt()));
	}
}

void ServerResponse::updateFileContents(){
}

void ServerResponse::Get(){
	cout << "GET method is called." << endl;
	//if (isCgi())
	//	Cgi();
	//else
		getFileContents();
}

void ServerResponse::Head(){
	cout << "HEAD method is called." << endl;
	getFileContents();
}

void ServerResponse::Post(){
	cout << "Post method is called. message: " << _req.getRequestMessageBody() << endl;
	//if (isCgi())
	//	Cgi();
	//else
		setRes("200", "OK", "POST success", "text/plain");
}

void ServerResponse::Put(){
	cout << "Put method is called." << endl;
	
	// create / update file
	bool exist = false;
	ifstream ifs(_file_absolute_path);
	if ((exist = existFile()) && !ifs.is_open()){
		setRes("403", "Forbidden", "", "");
	} else {
		ofstream ofs(_file_absolute_path, std::ios::trunc);
		ofs << _req.getRequestMessageBody();
		if (!exist)
			setRes("201", "Created", "", "");
		else
			setRes("204", "No Content", "", "");
		ofs.close();
		ifs.close();
	}
}

void ServerResponse::Delete(){
	cout << "Delete method is called." << endl;
}

void ServerResponse::Options(){
	cout << "Options method is called." << endl;
}

void ServerResponse::Trace(){
	cout << "Trace method is called." << endl;
}

ServerResponse::ServerResponse (ClientRequest &req) : _req(req), _res(""), _method(_req.getMethod()), _file_absolute_path(_req.getFileAbsolutePath()){

	// method / content length error
	bool m = false;
	if ((m = methodCheck()) || (contentLengthCheck())){
		if (m)
			setRes("405", "Method Not Allowed", "", "");
		else
			setRes("413", "Payload Too Large", "", "");
		return ;
	}

	// methods
	string methods[7] = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE" };
	void (ServerResponse::*funcs[7])()
		= { &ServerResponse::Get, &ServerResponse::Head, &ServerResponse::Post, &ServerResponse::Put, 
			&ServerResponse::Delete, &ServerResponse::Options, &ServerResponse::Trace};

	// exe method func
	for (int i = 0; i < 7; i++){
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