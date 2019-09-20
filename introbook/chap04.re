= 少し洗練された書き方

第2章で、Hyの構文をごくごく簡単に紹介しました。
Pythonや他のプログラミング言語の経験があれば、S式のルールになじんでしまえば
とりあえず書くだけならそこまで困らないでしょう。

本章では、ざっと構文を眺めただけだと気づきにくい機能・スタイルからピックアップして紹介をします。

== メソッドチェーン

//emlist{
obj.f().g(x).h(y, z)
//}

のような形のメソッドチェーンは、Hy の構文とあまり相性が良くありません。
しかし、Python のライブラリではこのような形で API が提供されている場合もあり、
一時変数を使うよりはメソッドチェーンを利用したい場合があります。

Hy にはメソッドチェーンを呼び出しやすくするための、doto という構文があります。

//emlist{
(doto obj (.f) (.g x) (.h y z))
//}

これで、上記の Python と等価なコードに展開されます。

== ドットの機能と利用法

attribute へのアクセスのために、ドット（.）が用意されています。
ドットは Hy の構文上、特殊な扱いを受けています。

=== 基本

//emlist{
(. obj attr subattr [(+ 1 2)] subsubattr)
//}

上記のフォームは、Python で

//emlist{
obj.attr.subattr[1 + 2].subsubattr
//}

のように展開されます。

(.) フォームは、最初の引数をオブジェクトとして、以下各引数で attribute への参照を行います。
今回の例では、第一引数の obj がオブジェクトで、第 2 引数の attr という名前の obj.attr という attribute への参照を行い、
以下の引数にも同様の attr への参照を繰り返します。

attribute へのアクセスで利用できるのは、シンボル（生の変数名）かリスト[]のみです。
その他の要素、"str" のような文字列や、:hysymbol などの即値を引数にするとコンパイルエラーになります。

(.) でのアクセスはあくまで attribute へのアクセスのため、メソッド呼び出しの場合はさらに
呼び出しのフォームとして囲ってあげる必要があります。

//emlist{
((. obj F) "some argument")
//}

=== 簡易形

ドットによる attribute アクセスは、シンボル中でも動作しています。
上記の関数呼び出しは、下記のようにも書けます。

//emlist{
(obj.F "some argument")
//}

オブジェクト指向なメソッド呼び出しでは、こちらの方が読みやすいでしょう。
本当は attribute へのアクセスが裏で行われているのですが、何となくこう書くと動いてしまうので
雰囲気で attribute へのアクセスを含むコードは書けてしまいます。

Hy のスタイルガイドでは (obj.F) より (. obj F) のスタイルが推奨されています。
ただ、筆者の意見としてはクラスベースオブジェクト指向なライブラリで毎回ドットから始まるフォームを書くのは
あまりに冗長なので、適宜 Python ライクな obj.attr の attribute アクセスは使ってしまって問題ないと思います。

変数名の中でドットを使うスタイルは便利ですし、可読性を上げることもあります。
しかし、そのためにドットは Hy の構文解釈上で特殊な扱いを受けているため
自由に変数名などに使うことが難しいです。

このあたりは言語の構文としては若干ダーティな感じにはなっていますが、
Hy が Python であることを選択してる結果と言えるでしょう。

== Threading macro の利用

Hy には threading macro（->, ->>）が標準で用意されています。 
これらを使うと、Python では一時変数と再代入が必要だったコードがそれらなしできれいに書けることがあります。

=== 一般的な利用法

公式ドキュメントには下記のように説明されています。

//emlist{
The threading macro inserts each expression into the next expression’s first argument place.
(訳：スレッディングマクロは、[訳注：スレッド内の]それぞれの式を次の式の最初の引数に配置します。)
//}

文字通りの意味なのですが、具体例を出した方が分かりやすいので下記に概念的なマクロ展開例を出してみます。

//emlist{
; (defn f [x y z] (+ x y z)) など何か適当な関数
(-> 10
  (f 1 2)
  (f 3 4))

(->> 10
  (f 1 2)
  (f 3 4))

↓↓↓

(f (f 10 1 2) 3 4)

(f 3 4 (f 1 2 10))
//}

=== 実際の使用例

挙動は分かったとして、これだけだと有用性がピンと来ないかもしれません。
ここでは私が Python から Hy に移植したコードの抜とを比較してみます。

//emlist{
# 移植元のPythonコード
# https://github.com/huggingface/pytorch-pretrained-BigGAN/blob/master/pytorch_pretrained_biggan/model.py より

    def forward(self, cond_vector, truncation):
        z = self.gen_z(cond_vector)

        z = z.view(-1, 4, 4, 16 * self.config.channel_width)
        z = z.permute(0, 3, 1, 2).contiguous()

        for i, layer in enumerate(self.layers):
            if isinstance(layer, GenBlock):
                z = layer(z, cond_vector, truncation)
            else:
                z = layer(z)

        z = self.bn(z, truncation)
        z = self.relu(z)
        z = self.conv_to_rgb(z)
        z = z[:, :3, ...]
        z = self.tanh(z)
        return z
//}

//emlist{
;;; 移植したHyコード

(setv ,,, Ellipsis)

(defn forward [self z truncation]
    (with [(torch.no_grad)]
      (-> (self.G.gen_z z)
          (doto (.view -1 4 4 (* 16 self.g.config.channel_width))
            (. permute 0 3 1 2)
            (.contiguous))

          (layers-forward self.G.layers z truncation)

          (self.G.bn truncation)
          (self.G.relu)
          (self.G.conv_to_rgb)
          (get [(slice None None) (slice None 3) ,,,])
(self.G.tanh))))

(defn layers-forward [z* layers z truncation]
  (for [layer layers]
    (cond [(in "GenBlock" layer.__class__.__name__) (setv z* (layer z* z truncation))]
          [True (setv z* (layer z*))]))
  z*)
//}

Python 版では z = … を毎行書いていたのが、Hy 版では一時変数と代入が forward 関数中からは撲滅できています。 
1 引数関数の呼び出しでチェインする部分は特に見通しが良くなっているのではないでしょうか。

一時変数を使わないとネストが深くなってコードが見づらくなる場合には、
-> や ->> の使用は検討の価値があると思います。
