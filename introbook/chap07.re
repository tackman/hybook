= 寄稿：HylangとPythonライブラリをつかってロボットを動かしてみよう

城野遼太さんにロボット操作でのHyの使用レポートをいただきました。
城野さんはHy自体はこの日初めて触っていますが、本稿のように支障なく使えたようです。
Hyの利用範囲のポテンシャルを感じていただければ幸いです。

== 概要

巷で大人気のPythonには豊富なライブラリ群があります。教材の類も充実して
ますし、会社や研究室の先行プロジェクト資産もあるでしょう。Hylangは幸いにし
てPythonとの連携機能が充実しているのでこういった先行資産を活用しながら、
徐々にLisp色に染めていくというスタイルを現実的に実行可能なヤベぇ特徴を備えて
います。

今回はオープンソースなロボットアームOpen Manipulator Xをシリアル通信経
由で操作する単純なアプリケーションを考えます。

== 実際の利用

当該のロボットアームはUbuntu端末からシリアル通信経由で所定の指令文を送
ることで操作します。Pythonでやると、こんな感じになります。

//emlist{
import serial
ser = serial.Serial('/dev/ttyACM0', 57600)
ser.write("task,up\n")
//}

これをHylangで書き直してみましょう。

Pythonライブラリのインポート
まず、ライブラリインポートですが、これはそのまんまの記法です。

//emlist{
(import serial)
//}

次に変数設定。
letは残念ながらない(欲しい時は自分で作りましょう)ので、
Hy公式のスタイルに則ってsetvで値を定義します。
文字列はダブルクオートじゃないと受け取って貰えないので注意!

//emlist{
(setv ser ( serial.Serial "/dev/ttyACM0" 57600))
//}

=== シリアルの書き込み

次に書き込みです。なんも考えないと、こんな感じの実装になるかと思いま
す。

//emlist{
(ser.write (str.encode "task,back\n"))
//}

Clojure由来らしい魔界記法”スレッドマクロ"をつかうと......以下のように書き
下せます。

//emlist{
(-> (str.encode "task,up\n") 
  (ser.write))
//}


指令文本体は単体の要素として先に切り出して渡してあげてもいいので......

//emlist{
(-> "task,up\n" (str.encode) 
  (ser.write) )
//}

この例ですと旨味がないですが、"."記法を使う場合、スレッドテイル・マクロ
を使い末尾にくっつけるようにしてこう書くこともできますね。

//emlist{
(->> "task,down\n" 
  (.encode str)
  (.write ser) )
//}

スレッドテイル方式のものを、書き込み部だけ関数に切り分けましょう。
defunじゃなくてdefnです。

//emlist{
(defn robotTask [dist] (-> (+ "task," dist "\n") (str.encode) (ser.write)))

(robotTask "up")
//}

繰り返し処理も見通しよくなりますね。

== 感想
データをつくって渡していく構造、シェルスクリプト っぽいですね。本格的に
S式で作り込んで行けば、PythonやCだと目が回るような複雑な処理もスマートに記
述できそうです。