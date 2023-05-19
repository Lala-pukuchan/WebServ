# テスト

## allow methods
- default.confに指定されたメソッドが通ること

|  結果  |  メソッド  |  パス  |
| ---- | ---- | ---- |
|  405  |  *  |  *  |


## GET
- GETメソッドでアクセス

|  結果  | Configパス| ディレクトリ  |  ファイル  |  パス  |
| ---- | ---- | ---- | ---- | ---- |
|  1  |  有  |  有  |  有  |  `curl -v http://webserv1:4242/index.html`  |
|  2  |  有  |  有  |  権限無  |  `curl -v http://webserv1:4242/autoindex/file.txt`  |
|  404  |  有  |  無  |  -  |  *  |
|  404  |  無  |  -  |  -  |  *  |

## autoindex
- ディレクトリにGETメソッドでアクセス
- パスが最後"/"で終わっていない場合も動作すること
- index.phpが存在する場合もindex.htmlが存在する場合と同様の動作をすること

|  結果  |  autoindex  |  index.html  |  パス  |
| ---- | ---- | ---- | ---- |
|  index.html表示  |  on  |  有  |  *  |
|  ディレクトリの中身を返す  |  on  |  無  |  *  |
|  index.html表示  |  off  |  有  |  *  |
|  403  |  off  |  無  |  *  |

## POST
## PUT
## DELETE

## cgi

## その他のファイル形式対応