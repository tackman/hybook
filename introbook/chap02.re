= 言語ひとめぐり

本章では Hy の雰囲気を見ていきます。
「だいたいこういう感じになるんだな」という感触と、
「Pythonだとこうなるんだ」という相場をつかんでもらうのが本章の目的です。

== 雰囲気を見るためのロゼッタストーン

Hyのサンプルと、これをhy2pyでPythonとして出力したコードを掲載します。

//emlist{
(import sys)

(defn printer [x]
  (print x))

(defn arg2str []
  (if (> (len sys.argv) 0) (get sys.argv 1)
      (get sys.argv 0)))

(printer (arg2str))

(defclass A [][a "Attr"]
  (defn __init__ [self]
    (setv self.x "member variable"))

  (defn printme [self]
    (print self.a self.x)))

(defclass B [A]
  [dict {"key" "Value" :hi "Hy!"}
   lst [1 4 "Yeah!"]]

  (defn $$?? [self]
    (, self.dict self.lst)))

(setv b (B))
(b.printme)
(print (b.$$??))
//}


//emlist{
from hy import HyKeyword
import sys


def printer(x):
    return print(x)


def arg2str():
    return sys.argv[1] if len(sys.argv) > 0 else sys.argv[0]


printer(arg2str())


class A:
    a = 'Attr'

    def __init__(self):
        self.x = 'member variable'
        return None

    def printme(self):
        return print(self.a, self.x)


class B(A):
    dict = {'key': 'Value', HyKeyword('hi'): 'Hy!'}
    lst = [1, 4, 'Yeah!']

    def hyx_is_Xdollar_signXXdollar_signXXquestion_markX(self):
        return self.dict, self.lst


b = B()
b.printme()
print(b.hyx_is_Xdollar_signXXdollar_signXXquestion_markX())
//}

S式の読み方にさえ慣れていれば、HyとPythonがどういう風に対応するのか想像するのは難しくないと思います。

== S式の読み方

 * Hy の構文はシンボルとフォームで構成される
 * フォームはシンボルの並びを円カッコ()で囲んだものである
 * フォームは値を持つ（値を返す）
 * フォームの 1 番目のシンボルは呼び出しをされ、残りは引数になる

多少の例外はあるものの、以上のルールを頭に入れるだけで Hy のコードは人力パースできてしまいます。
具体例を挙げてみましょう。

//emlist{
(+ 2 3 x)
//}

上記のフォームでは、 「+」「2」「3」「x」がシンボルです。
このうち、+が先頭にあるため「+」が関数として呼び出され、残りの「2」「3」「x」が引数となります。

Python 風に書けば、以下のようになります。

//emlist{
plus_func(2, 3, x)
//}

フォームは入れ子にもなります。

//emlist{
;;; Hy
(F "some" (G x))

# Python
F("some", G(x))
//}

Lisp族の構文の基本形はシンプルです。上記のように「カッコで囲まれた要素がある」
「カッコで囲まれた先頭要素が呼び出され、残りは引数扱いになる」
でほぼ全てです。この原則だけ頭に入れておけば、コードリーディングであまり困らないでしょう。

これを踏まえて、Hyの構文の要素を冒頭のロゼッタストーンを参照しながら解説します。

== 使える記号とデリミター

冒頭コードでは $$?? のようなケッタイな名前をつけている関数があります。
Hy は一般的なプログラミング言語と異なり、名前への記号の使用にほとんど制約がありません。
絵文字も使いたければ使えます。以下は合法な名前の例です。

//emlist{
$*
2dConv
your-favorite_hifn
,,,
++C++;
*->
CamelCase
snake_case
💩💩
//}

これらのPythonでは違法な文字を使った名前は、コンパイル時に一定の規則で
Python向けの名前に変換されます。

名前付けへの制約で大きな特徴は、Hy では使用可能な文字がホワイトリスト式ではなく
ブラックリスト式な点でしょう。そのため、上記のような自由な名前付けができます。

原則として名前に含められない、ブラックリスト入りの記号は下記のものです。

//emlist{
. (ドット)
  （半角スペース）
` （バッククオート）
' （クオート）
先頭の " （ダブルクオート。文字列になる）
//}

「原則として」と奥歯に挟まった言い方をしているのは、
リーダーマクロなどの黒魔術を使えばあらゆる行為は合法にできるからです。
本書では高度なマクロの解説はしませんが、必要な時に人はそこに導かれると思います。

== 関数定義

Built-in にある defn マクロを使うことで関数定義が可能です。
{//footnote defun ではありません。Common Lisp とのキーワード衝突が回避されているため、これを利用して
無理矢理 Common Lisp とインンターオペをするという手法も存在します//}

おおむね Pythonのdefによる関数定義と1対1に対応していますが、返り値の書き方だけは多少異なります。
Hy では defn 内のコードパスで一番最後に評価された値が返り値になります。
{//footnote Scala などと同様の方式です//}
Python では明示的に return した値のみが返り値になりますが、
Hy では全ての関数が値を返します。意図しない返り値を呼び出し側に渡さないためにも、
全てのパターンで返り値を書いた方がいいでしょう。

== 代入

setv という代入用の built-in があります。

この setv はフォームの値としては None を返しますが、Python3.8 のセイウチ演算子に対応した
値を返す版の setx もあります。

//emlist{
;;; Hy
(setv a (setx x "Hy, world!"))

# Python
a = (x := 'Hy, world!')
//}

setx は Python3.8 上での実行が必要になります。

== 各種データの生成

=== tuple

タプルは , （カンマ）を使って生成します。

//emlist{
(, "str" 123)

-> ('str', 123)
//}

タプルを使った代入にも対応しています。

//emlist{
(defn f [] (, "タプルの" "値"))

(setv (, a b) (f))

(print a b) 
;;; 出力； タプルの 値
//}

=== dictionary

{} で囲むと辞書型になります。

//emlist{
{"key" "Value" :key 1234}

-> {"key" : "Value, HySymbol("key") : 1234}
//}

{key1 value1 key2 value2 ...} のように、key と value を交互に並べていきます。
Python のようにコロン及びカンマでの区切りはありません。

{} 内の要素が奇数だとコンパイルエラーになります。

=== list

[] で囲むとリストになります。

//emlist{
[1 2 4 6]

-> [1, 2, 4, 6]
//}

list, dictionary ともに値の取得は get を使います。

//emlist{
(get dict "key")
(get ary 2)
//}

Python と異なり、x[i]のようにインデックス参照をして値を取得することはできません。

//emlist{
(setv x ary[2])
//}

のように書いた場合、下記のようにパースされます。

//emlist{
(setv x ary [2])) ; aryと[2]が別々に扱われる
//}

この場合は setv が 2 つの引数（代入先と値）しか取ることを許していないため、コンパイルエラーになってくれます。
しかし、関数の返り値で ary[2] のようにした場合は、
ary の 2 番目の要素「ではなく」、 [2] というリストが返り値になります。
慣れてくるとそもそもこのような書き方はしなくなりますが、
Python から来たばかりの人はハマることがあるかもしれません。

=== 文字列

ダブルクオートで囲むと文字列になります。
Python と異なり、シングルクオートには Lisp 特有の用途（quote）があるため
必ずダブルクオートになります。quote / unquote はマクロで頻用されます。

//emlist{
"string literal"

-> 'string literal'
//}

=== アトリビュートへのアクセス . (ドット)

ドットは Python と同じような使い方で、アトリビュートへのアクセスができます。
冒頭掲載コードのように、記号でつけられたアトリビュートへも b.$$?? のようにアクセスできます。

このような書き方ができることか察せるように、Hyでドットはパース時に特別な扱いを受けています。

=== シンボル :symbol-name

: (コロン)から始まる名前は symbol になります。
関数のキーワード呼び出しなどで使われます。また、定数値として必要だけど値自体には意味がない時などにも使えます。

これはPythonには対応する機能がありません。コンパイル後にはHySymbol('xxx')のように変換されます。

== コメント

; (セミコロン)から始まる行はコメントになります。また、セミコロン以降は行末までコメントになります。

== Python との相互利用

Hy のデータ型は Python のデータ型のスーパーセットになっています。
そのため Python のモジュールを import した上で、全ての内容を利用可能です。

モジュールのインポートは import 文を使います。

//emlist{
(import torch)
(import [numpy :as np])

;;; from module_name import some_class には対応していません

(torch.randn 3 2) ;;; -> [[0.3134, 0.66134, 0.234265], [0.12312,0.6781,0.89138]]
//}

== クラス定義

defclassというbuilt inでクラスを作成できます。

//emlist{
(defclass クラス名 [継承するクラス]
  [属性名1 属性値1 属性名2 属性値2 …]
  
  (defn __init__ [self]) …)
  以下メソッドの定義
  …
  ))
//}

属性の扱いを除き、Pythonのclassキーワードによる定義とほぼ同様です。

属性はクラス生成後に上書きができません。
そのため、クラス定義時に決定できる値を設定する、定数値のような運用が主になると思います。
変更しづらいという点を使って、データ型専用のクラスを作るのに使うという手もあるでしょう。

== マクロ

Lispと言えばマクロです。HyもLispの一員なので、defmacroでマクロを定義できます。

Lispのマクロについて解説することは、「ショートイントロブック」としての本書の範囲を超えてしまいます。
加えて、Hy公式ドキュメントでもマクロの解説は十分とは言い難いように思います。

幸いHyのマクロは他のLisp族と似たようなもののようなので、Common Lispなどのマクロの解説を適用していけるでしょう。
Lispにおけるマクロが詳述された書籍として、Paul Graham著「On Lisp」を挙げて代わりとしておきます。
