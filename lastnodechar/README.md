# \lastnodechar プリミティブについて =

これは [TeX ＆ LaTeX Advent Calendar 2014](http://www.adventar.org/calendars/553) の 15 日目の記事として作成したものです．
昨日（12/14）は [hak7a3](http://d.hatena.ne.jp/hak7a3/20141214) さん，明日（12/16）は !CardinalXaro さんです．

[TeX ユーザの集い 2014](http://texconf14.tumblr.com/) の休憩中，
次のように pLaTeX で記述すると，全角カンマと開き括弧の間には全角空きが挿入されてしまうという挙動が話題になりました．
```tex
これは，\textgt{『ほげ党宣言』}の……
```
[[Embed(fig1.png)]]

この記事では，上記の挙動や，解決の助けになることを期待して実装した `\lastnodechar` プリミティブ
について解説します．
関連ページとして，
 * [e-pTeX についての Wiki](https://sourceforge.jp/projects/eptex/wiki/FrontPage) 
 * [\pdfshellescape, \lastnodechar の実装](http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=1435)（TeX Forum 中のスレッド）
を挙げておきます．

## `\lastnodechar` プリミティブが利用可能な環境 ==

```\lastnodechar` プリミティブは，
TeX Live 開発版のソースに [r35619](http://www.tug.org/svn/texlive?view=revision&revision=35619)
(2014/11/19) にコミットされたものが「まともに使えるはず」のものです．
従って，
 * 角藤さんの [W32TeX](http://w32tex.org/) では 2014/11/19 以降のもので使用できます．
 * TeX Live 2014 ではそのままでは使えません．上記の開発版のソースから自分でビルドする必要があります．
 * 来年リリースされる予定の TeX Live 2015には入るでしょう．

また，`\lastnodechar` プリミティブは，
e-拡張が有効になっている [e-pTeX](https://sourceforge.jp/projects/eptex/wiki/FrontPage), e-upTeX にのみ実装されています．
コマンド名で言えば，
 * ptex, uptex では使えません．plain TeX で使いたい場合，eptex, euptex を用いてください．
 * platex, uplatex では（e-pTeX, e-upTeX で動いているので）問題ありません．

## pTeX での和文メトリック由来のグルー ==

さて，pTeX 系列（[ASCII pTeX](http://ascii.asciimw.jp/pb/ptex/index.html), 
[upTeX](http://homepage3.nifty.com/ttk/comp/tex/uptex.html),
e-pTeX, e-upTeX）では，和文文字間に入る空白について，次のような仕様になっています．
> 1. 和文文字以外のものの直後に和文文字 *p* が来た場合は，*p* の直前に「文字クラス 0 の文字と *p* の間に入る空白」が挿入される
> 1. 和文文字 *p* の直後が和文文字でない場合は，*p* の直後に「*p* と 文字クラス 0 の文字の間に入る空白」が追加される
> 1. 和文文字 *p* の直後に和文文字 *q* が来た場合は，両者の間に「*p* と *q* の間に入る空白」が追加される
それぞれの後半部の「」の量は，フォントメトリックによって決められています．和文フォントメトリック（[pTeX 公式ページ内のファイルフォーマットの解説](http://ascii.asciimw.jp/pb/ptex/tfm/jfm.html)）では，和文文字をいくつかの文字クラスにグループ化し，文字クラスごとに寸法・空白の指定をすることになっています．

上記の空白挿入の仕様は欧文におけるリガチャ・カーニング処理に類似した仕様になっており，
誤解を恐れずに言えば「和文文字の連続が『単語』をなし，単語の前後には文字クラス 0 の和文文字が仮想的にあるものとみなす」となります．

### 基本的な例 ===

上の説明では何を述べているかわかりづらいと思うので，具体例を述べます．
例えば，[pLaTeX2e 新ドキュメントクラス](http://oku.edu.mie-u.ac.jp/~okumura/jsclasses/) などで使われる
JIS フォントメトリックでは，次のようになっています．
> a. 漢字など，ほとんどの文字は文字クラス 0
> a. 文字クラス 0 の文字と開き括弧「『」の間には半角空きが入る
> a. 全角カンマ「，」と文字クラス 0 の文字の間にも半角空きが入る
> a. 全角カンマと開き括弧「『」の間には半角空きが入る
このとき，次の入力を考えてみましょう．
```tex
x『
，x
，『
```
すると，次のようになります．
 * 1行目では，「『」の直前は和文文字でないので，1. のケース→ b. 由来の半角空きが入る
 * 2行目では，「，」の直後は和文文字でないので，2. のケース→ c. 由来の半角空きが入る
 * 3行目では，「，」の直後は和文文字「『」なので，3. のケース→ d. 由来の半角空きが入る

### 「直後」の意味 ===

上の説明には 2 点補足があります．
 * 「直後」とは，できるだけ展開し続けた結果，文字の直後にくるトークンを意味する．
 * 「和文文字」とは，ソース中に陽に現れている和文文字だけではなく，次の 2 つも含める，
>  * `\char`あ` のように `\char` プリミティブに和文文字の内部コードを指定したもの
>  * `\chardef` によって和文文字の「別名」とされた制御綴

特に今の場合重要になるのは前者で，例えば次の1行目のように入力すると，
「，」「『」の間には合計で全角空きが挿入されることになります．
```tex
，{}『
，『% 比較対象
```
[[Embed(fig2.png)]]

なぜなら以下のように，「，」「『」それぞれに由来する半角空きが挿入されるからです．
 * 「，」の直後はグループ開始を示す「{」であり，和文文字ではないので，2. のケース→半角空きが入る
 * 「『」の直前はグループ終了を表す「}」であり，和文文字ではないので，1. のケース→半角空きが入る

マクロを使った例も載せてみます．
```tex
\def\hoge{『}
，\hoge あいうえお
```
上の場合，ソース中で「，」の次にあるのは制御綴 `\hoge` ですが，
それは「『」へと展開されます．結果として，「，」の直後の展開不能トークンは「『」ということとなり，両者の間には
半角空きが入ることとなります．

pTeX 系列での「段落開始時の開き括弧が全角二分下がりになる」のもこれで説明がつきます．
```tex
\noindent あいうえお

\everypar{}% \everypar による段落頭への挿入を無効化
『ほげ党宣言』% 全角二分下がり
```
[[Embed(fig3.png)]]

最初の「『」の直前は和文文字ではないので，
1. のケースとなり，「『」の直前には半角空きが入る，というわけです．

### 本記事冒頭の解説 ===

以上を元に，本記事冒頭の例について見ていくことにします．
```tex
これは，\textgt{『ほげ党宣言』}の……
```

例えば新ドキュメントクラスを利用している場合に，`\textgt` の定義を `\show\textgt` によって調べると次のようになります．
```
> \textgt=macro:
#1->\relax \ifmmode \hbox \fi {\gtfamily #1}.
```
言い換えれば，本記事冒頭の例は，次のように書いたのとほぼ同じことになります．
```tex
これは，\relax \ifmmode \hbox \fi {\gtfamily 『ほげ党宣言』}の……
```
よって次の2点がわかります．
 * 「，」の直後は `\relax` というこれ以上展開不能な命令で，和文文字ではない
 * 「『」の直前も（詳細は省略しますが） `\size@update`（今実行されるときには `\relax` と同義）となっており，和文文字でない
結果として「，」由来の半角空き，「『」由来の半角空きの両方が「，」と「『」の間に挿入される，というわけです．

pLaTeX 標準クラス jarticle.cls などでは `\textgt` の定義が異なりますが，状況としては同様で，
「，」由来・「『」由来の両方の空きが「，」と「『」の間に挿入されることになります．

*ここまでが前振りです．*
> - -

## `\lastnodechar` の必要性 ==

「望ましくない全角空きが入る」問題解消のために考えられる方法として，和文間のグルー挿入を抑止する `\inhibitglue` を挿入するというのがあります．
例えば以下のコードでは，「『」由来の半角空き・「，」由来の半角空きの挿入を抑止することで，両者の間が半角空きになるようにしています．
```tex
これは，\textgt{\inhibitglue 『ほげ党宣言』}の……
これは，\inhibitglue \textgt{『ほげ党宣言』}の……
```
[[Embed(fig4.png)]]

しかし，自動で一律に `\inhibitglue` を入れるのは望ましくありません．
以下の例では，入ってほしいはずの半角空きがなくなります．
```tex
これは\textgt{\inhibitglue 『ほげ党宣言』}の……
これは，\inhibitglue \textgt{ほげ党宣言}という本の……
```
[[Embed(fig5.png)]]

というわけで，空きが正しくなるような「`\textgt` 改」とでも言うべき命令を作るためには，
命令の*'直前*'の文字と，命令の引数の*'最初*'の和文文字を読み取り，それに応じて処理を分岐させる必要が出てきます．

後者は `\futurelet` などの利用でまあなんとかなりますが，
前者の「命令の直前の文字を知る」方法を（少なくとも私は）知りません．たぶん不可能だったのでしょう．
それを言うためには，[TeX by Topic](http://www.eijkhout.net/texbytopic/texbytopic.html) の
第 1.1 節などに説明がある，TeX の 4 つの処理段階を述べる必要があります．

> 1. 「*'目*'」：1行毎に読み込み，トークン列へと変換
> 2. 「*'口*'」：トークン列中のマクロ，条件分岐などを展開
> 3. 「*'胃*'」：これ以上展開できない命令を実行
> 4. 「*'腸*'」：段落を複数行に分割したりなど

重要なのは，一旦「胃」で実行された後は「口」には戻ってこれないということです．
例えば，本記事の最初から出している例で`\textgt` を展開しよう，という段になってみると，
その直前の「，」は既に実行が終わっている段階なので，`\textgt` の中で直前の「，」を（トークンとして）知ることは出来ない，
というわけです．
```tex
これは，\textgt{『ほげ党宣言』}の……
```

別の言葉で述べますと，TeX では文字・空白，罫線などは最終的にノードという形になり，
段落やページの中身はノード達のリンクリストとして表現されることになります．
第 3 段階の「胃」はトークンをノードに変換する過程であり，実行が終わって
*'一度ノードに変換されたものはトークンに戻せない*'わけです．

これが，私が `\lastnodechar` プリミティブを必要だと考えたわけになります．
「直前の文字」をトークンとして知ることが出来ないにしても，ノードに変換されているはずだから，
そこから「直前の文字」についての情報を得ることはできるはずだ，ということです．

## `\lastnodechar` の仕様 ==

一言で説明すれば，次のようになります：
```\lastnodechar` は，現在構築中のリスト（ボックス，段落，ページ等）中の「最後のノード」が文字由来であればその文字コードを，
そうでなければ -1 をそれぞれ*'内部整数*'として返す．

簡単なサンプルは以下の通りです．
```tex
これは，\message{\the\lastnodechar}\textgt{『ほげ党宣言』}……% ==> 「，」の内部コード
This is a pen.\message{\the\lastnodechar} % ==> 46
aiueo\hskip 10pt\message{\the\lastnodechar}% ==> -1
```
> 1. 1行目では，`\lastnodechar` 実行時の「最後のノード」は和文文字「，」なので，「，」の内部コードが結果になります．
> この値は e-(u)pTeX の内部漢字コードで変わります．
>  * 例えば筆者の環境では e-pTeX の内部漢字コードは EUC なので，1行目からは 41380 （16 進では !A1A4）が得られます．
>  * e-upTeX でタイプセットすると，標準の内部漢字コードは Unicode なので，1行目の結果は 65290 （16 進では FF0C）となります．
> 2. 2行目では，「最後のノード」欧文文字「.」なので，は実行結果はその文字コード 46 です．
> 3. 3行目では，「最後のノード」は `\hskip` によるグルーとなり，文字由来ではありません．よって結果は -1 となります．

上記の説明で述べた「最後のノード」については注意があります．
以下，次の 2 つの命令を用いながら説明していきます．
 * `\showlists`：「現在構築中のリストの中身を表示する」プリミティブ
 * `\message{...} `：引数 `...` の中身を端末とログファイルに表示します．
なお，以下の例において，クラスファイルは jsarticle を使用しています．

### ノードに寄与しないものは無視 ===
```\lastnodechar` は「最後のノード」の情報を得るものなので，ノード生成に寄与しないもの，例えば `\relax`，
空グループ {}，レジスタへの代入処理などで結果が変わることはありません．
例えば，次のコードからは 4 行とも「e」の文字コード 101 が得られます．
```tex
hoge\message{\the\lastnodechar}
hoge{}\message{\the\lastnodechar}
hoge\relax\message{\the\lastnodechar}
hoge\setcounter{section}{42}\message{\the\lastnodechar}
```

### 欧文ベースライン補正 ===
pTeX 系列では欧文のベースライン補正が `\ybaselineshift`（横組），`\tbaselineshift`（縦組）でできるようになっています．
これらが 0 でない場合，最後のノードはベースライン補正用のノードになります．
例えば，次のコードからは，以下の内容が得られます．
```
foo\showlists
```
```
\hbox(0.0+0.0)x9.24687 % \parindent 由来
\displace 2.0
\OT1/cmr/m/n/10 f
\OT1/cmr/m/n/10 o
\kern0.27779
\OT1/cmr/m/n/10 o
\displace 0.0
```

```\displace x.y` というノードがベースライン補正用に作られた
ノード[x.y ptだけ下にずらす役割を持ちます．)]([Footnote(以降のベースラインを)]です．
しかしこれは自動挿入されるものなので，`\lastnodechar` はそれを飛ばし，きちんと「o」の文字コード 111 を返します．
```tex
foo\message{\the\lastnodechar}% ==> 111
```

### 欧文リガチャ ===
また，!LuaTeX 以外の TeX では欧文の合字はまとめて 1 つのノードとして扱われます．例えば以下のコードからは，
リガチャ「ffi」が 1 つのノードにまとめられていることがわかります．
```tex
ffi\showlists
```
```
\hbox(0.0+0.0)x9.24687 % \parindent 由来
\OT1/cmr/m/n/10 ^^N (ligature ffi)
```

この場合，*'`\lastnodechar` は，リガチャの最後の構成要素の文字コードを返す*' ことにしました．
```tex
ffi\message{\the\lastnodechar}% ==> 105
```
これは，リガチャの周囲の和欧文間空白 (`\xkanjiskip`) の挿入判定を，直前の和文文字と「最初の構成要素」の間，
「最後の構成要素」と直後の和文文字の間で行うという仕様に合わせたものです．

### メトリック由来の空白 ===

JIS フォントメトリックにおける閉じ括弧類や読点・句点等，「その文字と『文字クラス 0 の文字』との間に空白が入る」ことが
フォントメトリックによって決まっている場合があります，本記事前半の説明から，`\lastnodechar` を用いて何かをする直前に
そのような文字が来た場合は，`\lastnodechar` 実行時における「最後のノード」は
大抵空白を表すグルー（またはカーン）となります．

e-拡張で定義されている「最後のノード」の種類を返す `\lastnodetype` というプリミティブでは，その空白を拾います．
```tex
』\message{\the\lastnodetype}% ==> 11 (glue)
```

しかし，`\lastnodechar` は実装目的からしてこのような動作は望ましくないので，
このような*'メトリック由来の空白を `\lastnodechar` は無視*'します．言い換えれば，
以下のソースは期待されたとおりに「』」の内部コードを返す，というわけです．
```tex
』\message{\the\lastnodechar}% ==> 41431 (EUC), 12301 (e-upTeX)
```

### 禁則用ペナルティ ===
同様に，`\lastnodechar` 直前が和文文字で，かつその和文文字における `\postbreakpenalty` の値が 0 でない場合は，
ペナルティが最後のノードになります．
```tex
\postbreakpenalty`っ=500
あっ\showlists
```
```
\hbox(0.0+0.0)x9.24687
\JY1/mc/m/n/10 あ
\JY1/mc/m/n/10 っ
\penalty 500(for kinsoku)
```

場合によっては，メトリック由来の空白も同時に挿入されることもありえますが，
その場合は空白の方が後に来ることになります．
```tex
\postbreakpenalty`）=-1% 説明用！
例）\showlists
```
```
\hbox(0.0+0.0)x9.24687
\JY1/mc/m/n/10 例
\JY1/mc/m/n/10 ）
\penalty -1(for kinsoku)
\glue(refer from jfm) 4.62343 minus 4.62343
```

禁則用ペナルティについても，*'`\lastnodechar` は禁則用ペナルティを無視*' するようになっています．
```tex
\postbreakpenalty`）=-1
\postbreakpenalty`っ=500
あっ\message{\the\lastnodechar}% ==> 42179 (EUC)
例）\message{\the\lastnodechar}% ==> 41419 (EUC)
```


## `\lastnodechar` の応用例 ==
例として，次で定義される `\fixjfmspacing`, `\mytextgt` 命令を考えてみます．
```tex
\makeatletter
\let\fjs@kanji=漢% kanji
\let\fjs@kana=あ%  kana
\let\fjs@other=（% other

\def\fixjfmspacing{%
> % \@temp は \fixjfmspacing 直後のトークンとなる
> \futurelet\@temp\fixjfmspacing@
}

\def\fixjfmspacing@{{%
> \@tempcnta=\lastnodechar
> % \@temp の中身を展開し続けた結果の最初は和文文字か？
> \edef\@@temp{\@temp}\expandafter\fixjfmspacing@@\@@temp\relax\@nil%
> %
> \ifnum\@tempcnta>\m@ne
> \setbox0\hbox{\inhibitglue\char\@tempcnta
> \relax\@temp\inhibitglue}%
> \setbox2\hbox{\inhibitglue\char\@tempcnta
> \@temp\inhibitglue}%
> \hskip\dimexpr\wd2-\wd0\relax
> \fi
}}

% #1 はもうこれ以上展開不可能なはず
\def\fixjfmspacing@@#1#2\@nil{%
> \ifcat\fjs@kanji#1\else
> \ifcat\fjs@kana#1\else
> \ifcat\fjs@other#1\else
> \@tempcnta=\m@ne
> \fi
> \fi
> \fi
}

\DeclareRobustCommand\mytextgt[1]{%
> \relax\ifmmode\hbox\fi{%
> \gtfamily\fixjfmspacing #1%
> }\fixjfmspacing%
}
\makeatother
```

この `\fixjfmspacing` 命令を使うと，次のように書体変更命令を間に入れても，文字間には正しく半角空きが入るようになります．
```tex
】 \gtfamily \fixjfmspacing （
```
それを内部に使っている `\mytextgt` 命令を `\textgt` の代わりに用いると，
ひとまず和文文字間の空白がうまくいっている……ように見えます．
```tex
\parindent=0pt
\def\TEST{『ほ}

【例1】\\
これは，『ほげ党宣言』（ホゲ・フガ著）\\
これは，\mytextgt{『ほげ党宣言』}（ホゲ・フガ著）\\% OK
これは，{}\mytextgt{『ほげ党宣言』{}}（ホゲ・フガ著）\\% OK
これは，\relax\mytextgt{『ほげ党宣言』\relax}（ホゲ・フガ著）\\% OK
これは，\mytextgt{\TEST げ党宣言』\relax}（ホゲ・フガ著）\\% OK
これは，\textgt{『ほげ党宣言』}（ホゲ・フガ著）% NG

【例2】\\
これは『ほげ党宣言』という本の……\\
これは\mytextgt{『ほげ党宣言』}という本の……\\% OK
これは\textgt{『ほげ党宣言』}という本の……% OK
```
[[Embed(fig6.png)]]

なお，ここで作成した `\fixjfmspacing`, `\mytextgt` 命令は説明のためにある程度単純化して作ったもので，
以下のような注意事項が挙げられます．これらを解決させるのは実際の利用者達に任せることにします!^!^;
 * `\fixjfmspacing` の直後のトークンが文字トークンでない場合は動作しません．
>  * 例えば，`」\fixjfmspacing {}【` や `」\fixjfmspacing \relax【` という例があります．
>  * `\fixjfmspacing` の直後が `\char` プリミティブや，`\chardef` プリミティブで定義された制御綴の場合も
> 上記のコードでは対応できていません．
 * 間にサイズ変更命令を挟んだ `，\Large \fixjfmspacing 「` といった入力は予期しない
> 結果[[Footnote(そもそもこの場合の「正しい結果」って何なんでしょう．)]]になります．<br>
>  * これは `\fixjfmspacing` 実行時のカレントフォントを用いて補正量を計算する[`\lastnodefont` とでもいう命令を作る以外にないでしょう（名前が良くない……）．)]([Footnote(文字コードと同様に，「最後の和文文字のフォント」を知る方法は現時点ではなく，新たに)]ためです．
>  * 同様に，`\fixjfmspacing` の前後で「本質的に異なる」フォントメトリックを使っている場合にも結果が乱れるでしょう．
 * 実際にはフォントメトリック由来のグルーには自然長以外に伸び量・縮み量があります．
> しかし，この方法では一旦ボックスに格納し，ボックスの幅の差を見ている関係から，伸び量・縮み量の補正ができません．

## 番外：!LuaTeX-ja の場合 ==
!LuaTeX や，その上で動く [LuaTeX-ja](https://sourceforge.jp/projects/luatex-ja/wiki/FrontPage) の場合には，状況が異なります．

### !LuaTeX におけるリガチャ・カーニング ===
!LuaTeX では，リガチャやカーニングの処理は段落・ボックス終了時に一括してノードベースで行うことになっています．
例えば，段落途中でノードの並びを表示させてみると
```tex
ffi\showlists
```
```
\whatsit
.\localinterlinepenalty=0
.\localbrokenpenalty=0
.\localleftbox=null
.\localrightbox=null
\hbox(0.0+0.0)x9.24866, direction TLT
\OT1/cmr/m/n/10 f
\OT1/cmr/m/n/10 f
\OT1/cmr/m/n/10 i
```
となっており，`\par` などによって段落が終了したときに初めて，この f, f, i の 3 ノードがリガチャになるなけです．
そのため，従来リガチャの抑止として用いられた `ff{}i` のような方法はもはや使えません．空グループ {} はノードを生成しないからです．

以上のことはカーニングに対してもあてはまります．

### !LuaTeX-ja での和文フォント ===
pTeX 系列では，「異なる和文フォントを使う」ことは，異なるフォントメトリックを用いることを意味していました．
例えば，JIS フォントメトリック中にある横組明朝用の jis.tfm と横組ゴシック用の jisg.tfm は，内容は同じですが
異なるフォントメトリックです．

一方，!LuaTeX-ja では，*'異なる和文フォントに対しても，同じフォントメトリックを共用する*'ことを許しています．
以下の例では，IPAex明朝 (`\MC`) と IPAexゴシック (`\GT`) について，同じメトリックを共用しています．
ノード的に言えば，`\MC` の「，」の直後に `\GT` の「『」が続いている[で見ると，`\tenrm ，` と `\tenrm 『` のように，欧文フォントを用いているように見えます．しかし，実際には `\showlists` では見えないところに使っている和文フォントの情報を格納しており，問題ありません．)]([Footnote(`\showlists`)]わけですが，両者は同じメトリックなので，
両者の間には（「，」と「『」の間に入るべき）半角空きが入るようになっています．
```tex
% plain LuaTeX
\input luatexja.sty % LuaTeX-ja の読み込み
\jfont\MC=IPAexMincho:jfm=ujis at 9.62216pt
\jfont\GT=IPAexGothic:jfm=ujis at 9.62216pt
\MC

これは，{\GT 『ほげ党宣言』}（ホゲ・フガ著）の……
\end
```

[[Embed(ltj.png)]]



### !LuaTeX における `\lastnodechar` 代替物 ===
!LuaTeX で `\lastnodechar` 代替物を作ることは比較的簡単です．
```tex
% LuaTeX
\def\lastnodechar{%
> \directlua{%
> local n = tex.nest[tex.nest.ptr].tail
> if n and n.id==node.id('glyph') then
> tex.write(tostring(n.char))
> else
> tex.write('-1')
> end
> }%
}
```
本来の e-pTeX の `\lastnodechar` と異なる点として，これは返り値を*'文字列*' で返すことが挙げられます．

また，!LuaTeX-ja では，内部処理用に様々な（whatsit と呼ばれる）ノード達を使っているので，上のような単純な定義では
次のような場合に対応できません．
```tex
あいうえお\Large\message{\lastnodechar}% ==> -1 (NG)
```

[[LineComment]]
