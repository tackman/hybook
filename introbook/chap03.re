= 開発環境

開発環境はプログラミング言語そのものの内容ではありませんが、
入門時に困りがちな情報として掲載します。

== 前提

著者の環境が Linux デスクトップなので、それを基本とした内容になっています。

Mac ユーザーの場合、本章内容の多くはほぼそのまま利用可能なはずです。
筆者の宗教上の理由で Mac 機を保有していないため、実際の動作は一切検証していません。

Windows ユーザーの方には、WSL(Windows Subsystem on Linux)の利用を推奨します。
WSL で neovim や Hy がある程度動かせることは確認済みです。

WSL は Docker が動くか怪しい、IO が絶望的に低速などの問題があります。
それでもコマンドプロンプトや Cygwin の利用で発生する問題と天秤にかけるなら、
こちらの方がマシだと筆者は考えています。
WSL 自体は Microsoft が本気で改善を続けているため、現時点での不都合は将来的な解決も期待できます。

== 各エディタの現状

メジャーどころからEmacs, vim, VSCodeの3つに加え、Emacsの拡張で別物になったSpacemacsを取り上げます。

=== Emacs

Lisp と言えば Emacs です。Hy はエディタのサポートが潤沢とは言い難い言語ですが、
Emacs（及びその拡張の Spacemacs）は相対的には一番サポートが厚いです。

hy-mode は 2019 年 9 月現在もアクティブなようで、更新頻度が高いとは言えないものの
当面は利用し続けることが期待できます。
また、Emacs の伝統的な Lisp 拡張 paredit なども利用することができます。

=== Vim

vim での Hy に関しては、現状はかなり厳しいと言わざるを得ません。
vim-hy という拡張はありますが、現在では unmentained な上に動作に問題が出ている状態です。

誰かが vim 向け拡張を作れば、それがデファクトになるオープンソースチャンスという見方はあります。

=== VSCode

VSCode のマーケットにある拡張は、現状シンタックスハイライトがつくものしかないようです。
こちらも IDE とは程遠い環境にしかならないでしょう。

=== Spacemacs

Spacemacs は Emacs の拡張で、原理的に Emacs と同等の環境構築が可能です。
Emacsでありながら、通常モード・挿入モードの使い分けなどvim風のキーバインドがあるのが特長です。
またEmacs拡張の導入など、面倒なところの面倒を見てくれます。

特筆すべき点として、Spacemacs 公式から hy-lyaer が提供されています。
そのためドットファイル（.spacemacs）に一行追加をするだけで hy-mode と関連する拡張が利用可能になります。
結果として得られる文明レベルは生の Emacs と同等ですが、導入の容易さでは Spacemacs に軍配が上がるでしょう。

=== で、どのエディタがいいのか

大前提として、全体的に未整備な世界なので己の信じるエディタがある人はそれを貫いた方が幸せになるでしょう。

_特にこだわりがない人には、Spacemacs を推奨します。_

 * vim, VSCodeに比べて相対的にはHy関係の拡張に恵まれている
 * 導入が容易。Emacs や vim と異なり、拡張管理のための環境整備などが不要

何かのエディタのプロでない人には、これらの特長は初心者にやさしくメリットが大きいです。

== Spacemacs での環境構築

ここでは Spacemacs の導入を簡単に解説しつつ、Hy 向けの設定を有効化する手順を説明します。

=== Spacemacs のしくみ

Spacemacs の実体は、.emacs.d に作られた Emacs の拡張です。そのため、Emacs のインストールは前提となります。
そのため、既存の Emacs を乗っ取る形で動作します。
通常の emacs との共存は困難なので、既存 Emacs ユーザーは注意が必要です。

=== Spacemacs の導入手順

前提として、Emacs をインストールします。

その後、Spacemacs のリポジトリを .emacs.d に clone します。

//emlist{
git clone https://github.com/syl20bnr/spacemacs ~/.emacs.d
//}

master のバージョンはやや古いままアップデートされていないので、develop に追随します。
筆者が検証した時点では、SpacemacsデフォルトバンドルのHyレイヤーを利用したい場合は
developのバージョンでないと動作をしませんでした。

//emlist{
cd .emacs.d
git checkout develop
git pull origin develop
//}

ここで Emacs を起動します。初回起動時の拡張が色々インストールされ、各種ファイルの生成も走ります。
起動が終わったら、 ホームディレクトリに生成される .spacemacs を編集します。
.spacemacs の dotspacemacs-configuration-layers に、使用する「レイヤー」を追記していきます。

「レイヤー」をものすごく雑に説明すると、Spacemacsで拡張や設定をパッケージ化したようなものです。
このあたりを掘り始めるとEmacs Lispの沼に沈み込んでいくので、詳細な解説は文献をあたってください。

//emlist{
   dotspacemacs-configuration-layers
   '( python
     hy
     auto-completion
     better-defaults
     emacs-lisp
     …
//}

"python" や "hy" といったレイヤーはSpacemacs公式から提供されているため、
上記の設定のみでユーザーは利用可能になります。

エディタを再起動するか、 SPC f e R　(dotspacemacs/sync-configuration-layers) コマンドを実行すると
hy-layer に必要な拡張がインストールされます。これで hy-mode が利用可能になりました！

=== その他の設定

拡張の導入は以上で完了するのですが、paredit など一部の拡張向けのキーバインドは
自動では設定されません。そのため、ユーザーによる設定でキーバインドを追加する必要があります。

キーバインドの設定方法は複数ありますが、はじめのうちは.spacemacsの user-config にベタ書きしてしまうのが
一番楽だと思います。
もちろん、Spacemacsにはモジュール分割を使ったより高度な設定方法も存在します。

下記に筆者の追加したキーバインド例を挙げます。一般的でないキーバインドにしているため
この通りにするのは推奨しませんが、必要なキーバインド設定の例として眺めてみたください。

//emlist{
(defun dotspacemacs/user-config ()
  (define-key evil-normal-state-map (kbd "SPC (") 'sp-wrap-round)
  (define-key evil-normal-state-map (kbd "SPC )") 'sp-unwrap-sexp)
  (define-key evil-normal-state-map (kbd "SPC [") 'sp-wrap-square)
  (define-key evil-normal-state-map (kbd "SPC ]") 'sp-unwrap-sexp)
  (define-key evil-normal-state-map (kbd "spc >") 'sp-next-sexp)
  (define-key evil-normal-state-map (kbd "spc <") 'sp-previous-sexp)
   )
//}

このキーバインドでは、例えば "SPC (" を入力すると選択部分を()で囲み、"SPC )" でカッコを外します。
このあたりの設定は、みなさんの好みに設定してください。


== 実行環境の作り方

Hy は Python のランタイム上で動作します。そのため、実行環境づくりも Python での手法に準拠します。

素朴な手法としては pip install などでグローバルにあれこれインストールする手法がありますが、
これは _絶対にやめましょう。_ Python は dependency hell を起こしやすく、
さらに Python2 系と Python3 系が混在するのが当たり前のシステムが大半です。
どんなに注意深くやっても事故は起こりますし、仮にあなたが達人でもその注意力を建設的な作業にふりむけられるようにするべきでしょう。

Python の経験が浅い読者は「大げさだなあ」と思われるかもしれません。
読者が必要を感じられない場合は、一旦この節の内容は忘れて素朴な管理方法をしてみるのもいいでしょう。
色々あって破綻して、OS 再インストールをしたくなった時に再読していただければ幸いです。

=== Docker

Docker に全てを閉じ込めてしまえば、その中では環境汚染し放題です。
システムインストールではやる気にならないお行儀の悪いことも躊躇なくできます。

Docker を利用する場合、Dockerfile に pip install を書き連ねていくスタイルになります。

Docker 内で Anaconda, virtualenv などは不要です。
ライブラリなどは Docker イメージ単位で管理ができますし、何かが壊れてもコンテナを破棄すればそれで済みます。
一方で virtualenv 等のインストールはイメージサイズを肥大化させてしまいます。
Docker のメリットを活かすためには不要でしょう。

Docker イメージについて、Hy 公式から提供されているものもありますが、
ベースイメージや Python のバージョンなどで融通が効きにくいです。
筆者は Python 公式が提供している Docker イメージをベースに、その上に Hy 及び必要なライブラリをインストールする
Dockerfile を作成するスタイルにしています。

//emlist{
from python:3.7-alpine

RUN pip install hy && pip install 必要なパッケージ
//}

==== Docker 利用での開発スタイル

hy コマンドで実行するだけなら、 docker run をするだけで通常のコマンド実行に近い感覚で実行できます。

//emlist{
docker run -rm your-hy-image -v /to/your/data:/data main.hy
//}

コーディングを行う場合には、考慮する点が出てきます。

hy-mode を利用した自動補完や、REPL 駆動をする場合はエディタの動作している hy の環境が必要になります。
Docker に実行環境を閉じ込めている場合、ホスト側で動作するエディタはこれらを利用できません。

解決方法の 1 つは、Docker イメージないにエディタをインストールしてしまい、コンテナ内で vim や Emacs を開いて編集するというものです。
ただしこれは CUI 限定になりますし、ホスト側での動作と機能性が異なってくる場合があります。

その他の方法としては、VSCode preview に搭載されているようなコンテナ接続機能を使う方法です。

=== virtualenv

Python の環境管理ツールの標準です。過去には似たようなツールも出回っていましたが、virtualenv の利用におおむね収束したと言えます。
virtualenv 自体の利用法は Python と変わりません。

== REPL 駆動

Lisp での開発と言えば REPL 駆動です。
REPL駆動とは、書いているコードの一部をREPLで評価しながら、その動作を見て開発をしていく手法です。
その場限りの書き捨て単体テストを連ねていくスタイルとも言えます。

実際には1行ずつhyのコンソール上で評価するわけではなく、エディタの機能を使います。

=== Spacemacs での REPL 駆動

hy-modeが有効化されていると、hy-shell-eval-xxx といったコマンドが利用できるようになります。
利用例として、

 1. 評価したいコード部分をビジュアルモードで選択
 2. , s r を入力（もしくはSPC SPCから、hy-shell-eval-regionを入力）

これだけで、Spacemacsが内部で持っているHyのREPL上でコードが評価されます。
数行程度書く→REPLで評価する→結果を見てコーディング、のループを回すと気持ちよく開発できます。

== デプロイ

Docker コンテナを実行できる環境へのデプロイなら、Docker イメージを作成してデプロイで済みます。
これは他の言語やフレームワークと何も変わりません。

Hy特有の例として、Cloud Functions へのデプロイを取り上げます。

=== Python への変換と Cloud Functions へのデプロイ例

Cloud Functions は、

 * サーバーインスタンスの面倒を直接見る必要がなく、
 * 実行時間の分のみの課金で済むので利用方法によっては大きくコストを削減できる

という利点の一方で、

 * ランタイムがGCP側で準備したものしか使えない
  
という制約があります。
AWS Labmda も同様ですが、この手のサービスで普段遣いしている言語がなくて悲しい思いをしがちです。

Hyのようなマイナー言語自体は、当然ながらCloud Functionsではサポートされていません。
しかし、Pythonならサポートされています。
そしてHyはPythonと完全にコンパチかつ、Pythonへのトランスパイラも付属しています！

ここまで来れば、やることは非常に単純です。

 * Cloud FunctionsでPythonランタイムのサービスを作成
 * Hyで書いたコードを、hy2pyでPythonに変換
 * 変換したPythonコードを、通常のPythonファイルのデプロイと同様に処理

やっていることは非常に簡単ですが、これはHyならではの利点です。
「困ったらPythonコードを出力すれば何とかなる」という手法の一例として、
Cloud Functionsへのデプロイは典型と言えます。
