#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char* requestMethod = getenv("REQUEST_METHOD");
	printf("HTTP/1.1 200 OK\r\n");
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html>\n");
    printf("<head><title>CGI Response</title></head>\n");
    printf("<body>\n");
    printf("<h1>CGI Response</h1>\n");
	printf("<p>hello...</p>\n");
	if (strcmp(requestMethod, "POST") == 0) {
		printf("<h1 style=color:red;>");
		char buffer[1024];
		fgets(buffer, sizeof(buffer), stdin);
		printf("%s", buffer);
		printf("</h1>\n");
	}
    printf("</body>\n");
    printf("</html>\n");
    return 0;
}

