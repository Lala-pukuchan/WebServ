# テスト
--resolve webserv1:4242:127.0.0.1
## GET
|  結果  | Configパス| ディレクトリ  |  ファイル  |  パス  |
| ---- | ---- | ---- | ---- | ---- |
|  1  |  有  |  有  |  有  |  `curl -v http://webserv1:4242/index.html`  |
|  2  |  有  |  有  |  無  |  `curl -v http://webserv1:4242/autoindex/file.txt`  |
|  2  |  有  |  有  |  権限無  |  `curl -v http://webserv1:4242/autoindex/no_file.txt`  |

## autoindex
|  結果  |  autoindex  |  index.html  |  パス  |
| ---- | ---- | ---- | ---- |
|  1  |  on  |  有  |  `curl -v http://webserv1:4242/autoindex/`  |
|  3  |  on  |  無  |  `curl -v http://webserv1:4242/autoindex2/`  |
|  1  |  off  |  有  |  `curl -v http://webserv1:4242/`  |
|  4  |  off  |  無  |  `curl -v http://webserv1:4242/autoindex3/`  |

## POST
|  結果  | ディレクトリ有無  |  ファイル有無  |  ファイル形式  |  パス  |
| ---- | ---- | ---- | ---- | ---- |
|  1  |  有  |  有  |  .txt  | `curl -v -X POST --data-binary '@./docs/upload.txt' http://localhost:4242/upload/`  |

## DELETE

## allow methods
- default.confに指定されたメソッドが通ること

|  結果  |  メソッド  |  パス  |
| ---- | ---- | ---- |
|  10  |  *  |  *  |

## content length

## cgi
