# テスト
## autoindex
- ディレクトリにGETメソッドでアクセス
- パスが最後"/"で終わっていない場合も動作すること
- index.phpが存在する場合もindex.htmlが存在する場合と同様の動作をすること

|  結果  |  autoindex  |  index.html  |  パス  |
| ---- | ---- | ---- | ---- |
|  index.html表示  |  on  |  有  |  *  |
|  ディレクトリの中身を返す  |  on  |  無  |  *  |
|  index.html表示  |  off  |  有  |  *  |
|  403エラー  |  off  |  無  |  *  |

## allow methods
- default.confに指定されたメソッドが通ること

|  結果  |  メソッド  |  パス  |
| ---- | ---- | ---- |
|  405エラー  |  *  |  *  |


## GET
## POST
## PUT
## DELETE
