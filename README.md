# テスト
## GET
|  結果  | Configパス| ディレクトリ  |  ファイル  |  パス  |
| ---- | ---- | ---- | ---- | ---- |
|  1  |  有  |  有  |  有  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/index.html`  |
|  2  |  有  |  有  |  無  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/no_such_file.txt`  |
|  2  |  有  |  有  |  権限無  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/no_right_file.txt`  |

## autoindex
|  結果  |  autoindex  |  index.html  |  パス  |
| ---- | ---- | ---- | ---- |
|  1  |  on  |  有  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/autoindex/`  |
|  3  |  on  |  無  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/autoindex2/`  |
|  1  |  off  |  有  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/`  |
|  4  |  off  |  無  |  `curl -v --resolve webserv1:4242:127.0.0.1 http://webserv1:4242/autoindex3/`  |

## POST
|  結果  | ディレクトリ有無  |  ファイル有無  |  パス  |
| ---- | ---- | ---- | ---- |
|  5  |  有  |  無  | `curl -v --resolve webserv1:4242:127.0.0.1 -X POST --data-binary '@./docs/upload.txt' http://localhost:4242/upload/`  |
|  5  |  有  |  有  | `curl -v --resolve webserv1:4242:127.0.0.1 -X POST --data-binary '@./docs/upload.txt' http://localhost:4242/upload/`  |
|  2  |  無  |  有  | `curl -v --resolve webserv1:4242:127.0.0.1 -X POST --data-binary '@./docs/upload.txt' http://localhost:4242/upload/no/`  |

## DELETE
|  結果  | ディレクトリ有無  |  ファイル有無  |  パス  |
| ---- | ---- | ---- | ---- |
|  6  |  有  |  有  | `curl -v --resolve webserv1:4242:127.0.0.1 -X DELETE http://localhost:4242/dir/delete.txt`  |
|  2  |  有  |  無  | `curl -v --resolve webserv1:4242:127.0.0.1 -X DELETE http://localhost:4242/dir/no.txt`  |

## allow methods
|  結果  |  パス  |
| ---- | ---- |
|  10  |  `curl -v --resolve webserv1:4242:127.0.0.1 -X PUT http://localhost:4242/`  |

## content length
|  結果  |  パス  |
| ---- | ---- |
|  7  |  `curl -v --resolve webserv1:4242:127.0.0.1 -X POST --data-binary '@./docs/upload.txt' http://localhost:4242/upload2/`  |

## cgi
|  結果  | メソッド |  パス  |
| ---- | ---- | ---- |
|  8  | GET |  `curl -v --resolve webserv1:4242:127.0.0.1 http://localhost:4242/test.cgi`  |
|  9  | POST |  `curl -v --resolve webserv1:4242:127.0.0.1 -X POST -d 'abc' http://localhost:4242/test.cgi`  |


