#include "CgiExe.hpp"
#include "ServerResponse.hpp"

CgiExe::CgiExe (ClientRequest &req) : _req(req), _result(""), _status(""){

	// arg
	_arg = new char*[2];
	_arg[0] = new char[strlen(_cmd) + 1];
	strcpy(_arg[0], _cmd);
	_arg[1] = NULL;

	// env
	stringpair_t env[] = {
		stringpair_t("REQUEST_METHOD", _req.getMethod()),
		stringpair_t("SERVER_PROTOCOL", "HTTP/1.1"),
		stringpair_t("PATH_INFO", _req.getPathInfo())
	};
	const int env_size = sizeof(env) / sizeof(env[0]);
	map<string, string> envMap (env, env + env_size);
	_env = new char*[envMap.size() + 1];
	int i = 0;
	map<string, string>::iterator it;
	for (it = envMap.begin(); it != envMap.end(); ++it) {
		string env = it->first + "=" + it->second;
		_env[i] = new char[env.size() + 1];
		strcpy(_env[i++], env.c_str());
    }
	_env[i] = NULL;
}

CgiExe::~CgiExe (){

	// delete
	delete _arg[0];
	delete[] _arg;
	int i = 0;
	while (_env[i])
		delete _env[i++];
	delete[] _env;

}

void CgiExe::exe (){

	// write input
	FILE *tmpFile = tmpfile();
	int	inputFd = fileno(tmpFile);
	if (_req.getMethod() == "POST"){
		write(inputFd, _req.getRequestMessageBody().c_str(), _req.getRequestMessageBody().size());
		lseek(inputFd, 0, SEEK_SET);
	}

	// pipe
	int fd[2];
	if (pipe(fd) == -1){
		cout << "Failed to pipe." << endl;
		_status = "500";
		return ;
	}

	// fork
	pid_t pid = fork();

	if (pid == -1){
	
		cout << "Failed to fork." << endl;
		_status = "500";
		return ;
	
	} else if (!pid){

		// stdin
		dup2(inputFd, STDIN_FILENO);

		// stdout
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		
		// exe
		if (_req.getLocationConfig().getCgiPath() != "")
			execve(_req.getLocationConfig().getCgiPath().c_str(), _arg, _env);
		else if (_req.getFileAbsolutePath() != "")
			execve(_req.getFileAbsolutePath().c_str(), _arg, _env);

		// exe error
		cout << "Failed to exe." << endl;
		_status = "500";
		exit (1);

	} else {

		// wait
		close(fd[1]);
		waitpid(pid, NULL, 0);

		// result
		int size = 0;
		char buf[BUFFER_SIZE + 1];
		while ((size = read(fd[0], buf, BUFFER_SIZE)) > 0)
		{
			buf[size] = '\0';
			_result += buf;
		}

		// should be checked
		if (_status != "500")
			_status = "200";
	}

	fclose(tmpFile);
	close(inputFd);
}

string CgiExe::getResult() const{ return (_result); }

string CgiExe::getStatus() const{ return (_status); }
