# TeX Live 2020 における pTeX, e-pTeX 変更点まとめ =

TeX Live 2020 には pTeX p3.8.3 と e-pTeX 191112 が収録されています．
本ページでは昨年の TeX Live 2019 (pTeX p3.8.2, e-pTeX 190131) からの変更点についてざっと説明します．

なお，TeX Live 2019 の (e-)(u)pTeX は 2019-05-30 (r51236) でリビルドが行われました（Acetaminophen さんの[ブログ記事](http://acetaminophen.hatenablog.com/entry/tl2019-02#main-7)でもこの件が触れられています）．
このリビルドですでに取り込まれた修正については見出しに【R】マークをつけています．

[depth=2)]([PageOutline(start=2,)]


## pTeX ==

### 和文文字トークンを `\let` した制御綴の和文カテゴリーコードの扱い【R】 ===
> * [ZR さんの Twitter](https://twitter.com/zr_tex8r/status/1119952801952485377)，[ptex-manual/4](https://github.com/texjporg/ptex-manual/issues/4)

欧文文字トークンと異なり，和文文字トークンについては一般に
> * pTeX では，和文カテゴリーコードは和文文字トークンには保存されず，その都度算出される
> * upTeX では，和文カテゴリーコードは和文文字トークンに保存される
という挙動になっています．

しかし，TeX Live 2019 当初の pTeX では
```tex
\kcatcode`あ=18
\let\yA=あ
\kcatcode`あ=17
\ifcat あ\yA O\else X\fi % (1)
```
というソースから `X` が出力されていました．これは和文文字トークンを `\let` した際に和文カテゴリーコードが保存されるため，(1) の `\ifcat` では次が比較されたことによるものです：
>  「`あ`」：ソース中に直書きされた，和文カテゴリーコード 17 の「あ」<br>
>  `\yA`：和文カテゴリーコード 18 の「あ」

TeX Live 2019 では 2019-05-30 のリビルド後の pTeX p3.8.2，及び TeX Live 2020 の pTeX p3.8.3 では，
> pTeX では，和文カテゴリーコードは和文文字トークンには保存されず，その都度算出される
という挙動で統一させるため，`\if`, `\ifcat` の修正を行いました．これによって，pTeX p3.8.3 では，上記ソースの (1) の `\ifcat` では
>  「`あ`」：ソース中に直書きされた和文文字トークン→`\ifcat` 時の和文カテゴリーコードは 17<br>
>  `\yA`：`\let`でできた和文文字トークン→`\ifcat` 時の和文カテゴリーコードは 17
と，「和文カテゴリーコード 17 の『あ』」同士が比較され，結果は `O` となります．

### `\if`, `\ifcat` における制御綴の扱い【R】 ===
> * [ZR さんの Twitter](https://twitter.com/zr_tex8r/status/1119966520321265664)，[tex-jp-build/68](https://github.com/texjporg/tex-jp-build/issues/68)

TeX では `\if`（文字コードの比較）や　`\ifcat`（カテゴリーコードの比較）において，制御綴を
>  カテゴリーコード 0，文字コード 256
として扱っています．pTeX ではこれで問題ないのですが，upTeX 1.24 では
>  文字コード 256 の和文文字トークン「Ā」(U+0100)
が存在するため，これと `\relax` が `\if` で等価と判定されてしまいました．

以上の症状の改善のため，upTeX 1.25（TeX Live 2019 では 2019-05-30 のリビルド以降）では制御綴を
>  カテゴリーコード 0，文字コードは和文文字コードとしてありえない値（upTeX では 0x1000000）
として扱うことにしました．これに合わせ，pTeX p3.8.3 では「制御綴の文字コード」を 65536 と扱うようにしていますが，これに伴う pTeX の挙動の変化は起こらないはずです．

### 和文文字トークンを `\let` した制御綴に ``` が適用できてしまう症状を修正 ===
> * [TeX Forum 中の記事](https://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=852)（前段階），[tex-jp-build/82](https://github.com/texjporg/tex-jp-build/issues/82)

TeX でときどき使われる整数の使い方として，``` の後に文字トークンを直接続けるか，``` の後に欧文1文字の制御綴（`\C`, `\%` など）を続けるというものがあります．

昔の pTeX では ``` の後に続けられるものの判定が甘く，````\AA` がなぜか通ってしまうという症状がありました．2012 年に上の「前段階」で判定を厳しくしたのですが，まだその判定は不完全であったことが 2019 年 7 月にわかりました．
具体的には，TeX Live 2019 の pTeX では，下のソースの「和文文字トークンを `\let` した」制御綴 `\EE` に対して，その内部コードが ````\EE` として得られてしまいます．
```tex
\let\C=あ
\let\DD=a
\let\EE=あ

\count100=`\C \message{(\the\count100)}
>  % ==> 67
\count100=`\DD \message{(\the\count100)}
>  % ==> ! Improper alphabetic or KANJI constant.
\count100=`\EE \message{(\the\count100)}
>  % ==> 464 (!?)
\bye
```

pTeX 3.8.3 では，``` の後に続けられるものの判定を更に厳しくし，文字トークンか欧文1文字の制御綴しか許容しないようにしました．

### 不正な UTF-8 シーケンスが JIS X 0208 未定義の「和文文字」に化ける症状を修正 === #header-a2af
> * [platex/86](https://github.com/texjporg/platex/issues/86)，[tex-jp-build/90](https://github.com/texjporg/tex-jp-build/pull/90) (pull request)

現行（TeX Live 2019 以前も，2020 も）の pTeX では，例えば
```tex
%#!ptex
\message{aßa}
\immediate\openout1=z.out
\immediate\write1{aßa}
\immediate\closeout1
\font\a=ec-lmtt10\a % T1 encoding
\input z.out % <==
\bye
```
のように，ß (U+00DF, `C3 9F` in UTF-8) を出力すると，
> `a` `<C3>` `^` `^` `9` `f` `a`
のように出力されます．
この出力自体に不思議な点はある（[tex-jp-build/81](https://github.com/texjporg/tex-jp-build/issues/81)）のですが，TeX Live 2019 以前の ptexenc で問題なのは，z.out を `\input` すると，結果が
> `a〓§a`（「〓」は実際には JIS コードで 222F の符号位置であり，JIS X 0208 では未定義）
のように，単独バイト `<C3>` が化けてしまうことにありました．

これは，
> * 単独の `<C3>` は UTF-8 シーケンスとして不正（後には `<80>`--`<Bf>` の範囲のバイトが 1 つ続くはずだが，そうでない）である
> * TeX Live 2019 以前の ptexenc では，（必要なバイト数だけ続いていない）不正な UTF-8 シーケンスを U+0000 と解釈し，さらにこれが「JIS コードで 222F の文字」に変換[にある JIS X 0208 から Unicode の変換テーブルでは，JIS X 0208 未定義文字には 0 と記録されています．ptexenc はこのテーブルを逆引きして Unicode から JIS X 0208 への変換を行う（ただし configure 時に iconv を使用可能にできる）ので，U+0000 から JIS X 0208 の最初の未定義文字 JIS 222F に変換されるというわけです．)]([Footnote(ptexenc)]されて pTeX の内部処理にかかる
ということによるものでした．TeX Live 2020 の ptexenc では，不正な UTF-8 シーケンスは（JIS X 0208 範囲外の Unicode 文字と同じく）  `^^` 記法に直して pTeX に渡すようにしています．

### `\endlinechar=128` で落ちるバグを修正 ===
> * [tex-jp-build/89](https://github.com/texjporg/tex-jp-build/issues/89)

報告元から引用しますが，次のソースが高確率で `free(): double free detected` などのエラーで落ちるというものです．
```
\endlinechar=128
\immediate\openout10=xout.tex
\immediate\closeout10
\end
```

このエラーは，ファイル名を UTF-8 から pTeX の内部エンコーディングに変換する ptexenc の `ptenc_from_utf8_string_to_internal_enc` 関数中のミスが原因でした．結果を格納する buffer = buf という 2 配列は，長さが足りなくなりそうなときに `xrealloc` で長さを増やして再確保されますが，そのとき buffer のみが再確保されてしまう（buf は古いアドレスのまま残る）コードになっていました．
TeX Live には[すぐ上の修正](tl2020#header-a2af)と同時に取り込まれました．

### `\kansujichar` の取得が可能に ===
> * [tex-jp-build/93](https://github.com/texjporg/tex-jp-build/issues/93)

`\kansujichar` は代入できるのになぜか取得できないようになっていたのですが，pTeX p3.8.3 では取得が可能になりました．
`\count123=\kansujichar45` のような不正な引数については，`\kansujichar` を無視した `\count123=45` と同義になります．
これは `\kansujichar` の代入時の挙動に合わせたもので，TeX82 の他の命令とは少々異なります（「`I changed this one to zero.`」と引数を 0 と扱って進むのが一般的なようです）．

### フォーマットファイルの圧縮（pTeX 系列，pdfTeX，e-TeX） ===
> * [tex-jp-build/96](https://github.com/texjporg/tex-jp-build/issues/96)

元々 !LaTeX のフォーマットファイルは 4 MB 強程度の容量があったのですが，[LaTeX 2020-02-02](https://www.latex-project.org/news/2020/02/02/issue31-of-latex2e-released/) で expl3 がプリロードされることになったことに伴い，`latex` フォーマット (pdfTeX) は 8.3 MB，`platex` フォーマット (e-pTeX) は 10.6 MB と大幅に増大してしまいました[と `platex` で 2 MB 以上もの差ができているのは，①TFM 情報を格納する配列 *font_info* の 1 要素が pdfTeX では 4 バイトなのに対し，pTeX 系列では 8 バイトである，②expl3 は読み込み時に 65536 個の `\fontdimen` を持つフォントを 8 個定義する，③「65536 個の `\fontdimen` を持つフォント」はそれぞれ *font_info* を約 65000 要素使う，というのが主な理由です．)]([Footnote(ちなみに，`latex`)]．


この容量増加を少しでも食い止めるために，TeX Live 2020 では (e-)(u)pTeX, pdfTeX (, e-TeX) でフォーマットファイルを zlib によって圧縮するようにしています．zlib フォーマット圧縮自体は !XeTeX, !LuaTeX が以前から取り入れており，今回はその処理を上に挙げたエンジンにも適用するように拡張したというわけです．これにより，例えば `platex` フォーマット (e-pTeX) は 2.4 MB と 1/4 にまで抑えることができました．

なお，zlib を使う理由は「既に TeX Live に含まれているから」です（pTeX 系列にも，zlib は !SyncTeX のため既にリンクされています）．個人的には，SSD での使用を考慮し，展開スピードが速い lz4（圧縮は lz4hc）の方が向いていると思いますが，TeX Live 上流ではわざわざライブラリを増やすことのデメリットの方が大きいようです．

### `\ifjfont`, `\iftfont` プリミティブの追加 ===
> * [jsclasses/53](https://github.com/texjporg/jsclasses/issues/53#issuecomment-581934655)（発端），[tex-jp-build/97](https://github.com/texjporg/tex-jp-build/pull/97) (pull request)

発端となった [jsclasses/53](https://github.com/texjporg/jsclasses/issues/53#issuecomment-581934655) は，!LaTeX の `\do@subst@correction` が和文に対応していない関係で，「和文フォントを選択する」制御綴が「欧文フォントを選択する」ものに置き換わってしまうという症状です．この症状への対応の議論の過程で，`\font`, `\jfont`, `\tfont` による「フォントを選択する」制御綴（*fontdef* トークン）が，和文・横組用和文・縦組用和文のどのフォントを選択するのか容易にはわからないということが話題に挙がりました．例えば `\meaning\tenmin` と `\meaning` を使っても 「`select font min10`」と TFM 名が返されるだけですし，e-TeX 拡張の `\iffontchar` は欧文フォントに対して文字コード 0--255 しか許容しないようになった（[本ページ内の対応箇所](tl2020#header-iffoncchar)）ので使えません．

そこで，*fontdef* トークンの選択するフォントが横組用和文フォントか否か・縦組用和文か否かを判定する `\ifjfont`, `\iftfont` プリミティブがあると便利そうだ，となり実装されたというわけです．実際，以下のソースできちんと判定できていることがわかります．
```tex
%#!ptex
\font\tenrm=cmr10     % 欧文フォント
\jfont\tenmin=min10   % 和文横組用
\tfont\tentmin=tmin10 % 和文縦組用
\def\FX#1{%
>  <\meaning#1: %
>  \ifjfont#1JY\else\iftfont#1JT\else AL\fi\fi>%
}
\message{\FX\tenrm}  % ==> <select font cmr10: AL>
\message{\FX\tenmin} % ==> <select font min10: JY>
\message{\FX\tentmin}% ==> <select font tmin10: JT>
\bye
```


ちなみに，2020-02-09 現在[検討中の実装](https://github.com/texjporg/platex/tree/for-tl2020) では e-TeX 拡張を使う代わりに `\ifjfont`, `\iftfont` プリミティブを使わない方針になっています．

### `\input` プリミティブでブレースで囲まれたファイル名指定が可能に ===
> * [TeX Live svn r53729](https://www.tug.org/svn/texlive?view=revision&revision=53729)

従来，`\input` プリミティブでファイル名として許される文字は
> * 欧文文字であって，カテゴリーコードが 12 (*other_char*) 以下で 空白「` `」以外のもの
> * 和文文字は全て
でした．Web2C ではこれに加え
> * 半角二重引用符「`"`」で囲んだ中では空白「` `」もファイル名として使える（「`"`」自体はファイル名の一部としては扱われない）
という仕様が加わりました．これによって，例えば
```tex
\input "h o g e.tex a"
\input "h o g e".tex" "a
```
は両者とも 「`h o g e.tex a`」というファイルを読み込むようになっています[`Transcript written on ...` で表示されるファイル名では全体が「`"`」で囲まれますが，これはファイル名出力の際に TeX が付加するものです．)]([Footnote(エラーメッセージや最後の)]．

一方，!LuaTeX では `\input` プリミティブの段階でブレースで囲まれたファイル名指定が可能になっています．この場合，ブレースで囲まれた引数の中の「`"`」はファイル名の一部となります．
```tex
\input "h o g e".tex" "a  %==> 「h o g e.tex a」
\input{"h o g e".tex" "a} %==> 「"h o g e".tex" "a」
\input{h o g e.tex a}     %==> 「h o g e.tex a」
```
今回の TeX Live 2020 における変更は，!LuaTeX 以外のエンジンでも上記のような `\input` プリミティブでブレースで囲まれたファイル名指定を利用可能にするものです．

#### !LaTeX における `\input` 命令 ====
一方，!LaTeX では ltfiles.dtx において，次のように `\input` が再定義され，マクロレベルでブレースで囲まれたファイル名指定を利用可能にしています．
```tex
\ifx\@@input\@undefined\let\@@input\input\fi
...
\def\input{\@ifnextchar\bgroup\@iinput\@@input}
\def\@iinput#1{%
>  \InputIfFileExists{#1}{}%
>  {\filename@parse\@curr@file
>   \edef\reserved@a{\noexpand\@missingfileerror
>     {\filename@area\filename@base}%
>     {\ifx\filename@ext\relax tex\else\filename@ext\fi}}%
>   \reserved@a}}
```
ブレース内にファイル名を指定した場合の詳細については，ltfiles.dtx の「Safe Input Macros」節を参照して下さい．

なお，上記の定義からわかるとおり !LaTeX でも
```tex
\input hoge.tex
```
とブレースで囲まずファイル名を指定した場合はプリミティブの `\input` が呼び出されます．

プリミティブとしての `\input` と !LaTeX で再定義された `\input` の違いとして目につくのは存在しないファイル名を指定したときの状況です．例えば次の入力を考えます．
```tex
%#!latex
\input{nonexistent.tex}
\message{HOGE!}
\input "nonexistent.tex"
\message{FUGA!}
```
> * 2 行目の `\input` からは `! LaTeX Error: File `nonexistent.tex' not found.` というエラーが発生しますが，Enter キーを一回押すことで次の処理（3 行目の `\message`）に進むことができます．
> * 一方，4 行目は結果として `\input` プリミティブを呼び出すため，`! I can't find file `non existent.tex'.` というエンジンレベルのエラーが発生します．Enter キーを押してもリトライされるだけで，別のファイル名を入力するか，Ctrl+D を押してタイプセット処理を終わらせるかしか抜け出す方法がありません．

## e-pTeX ==
### `\ifincsname` プリミティブの追加 ===
> * [latex2e/95](https://github.com/latex3/latex2e/issues/95#issuecomment-509631724)，[tex-jp-build/83](https://github.com/texjporg/tex-jp-build/issues/83)

`\csname...\endcsname` の中で判定が実行されれば真，そうでなければ偽となる条件文です．以下が実行例となります．
```tex
%#!eptex
\def\TEST{\ifincsname CSNAME\else NOT\fi}
\message{%
>  \TEST % ==> NOT
>  \expandafter\string\csname a\TEST b\endcsname} % ==> \aCSNAMEb
\bye
```
元々は pdfTeX に実装されたものであり，!XeTeX, !LuaTeX にも実装されていましたが，e-pTeX だけ未実装でした（e-pTeX 190709 で実装）．

`\ifincsname` を e-pTeX に実装することになったのは，!LaTeX 2019-10-01 における Unicode 対応改善（ラベル中に Unicode 文字を扱えるように）の開発版コードで `\ifincsname` が使われたためです．`\expanded` プリミティブ（e-pTeX には TeX Live 2019 で追加）と同様に，!LaTeX team によるリクエストと言っても良いでしょう．

### `\iffontchar`, `\fontcharwd` 他の引数の扱い === #header-iffontchar
> * [ptex-manual/3](https://github.com/texjporg/ptex-manual/issues/3)，[tex-jp-build/77](https://github.com/texjporg/tex-jp-build/pull/77) (pull request)

TeX Live 2019 の e-pTeX では，`\iffontchar`, `\fontchar??` (`??`: `wd`, `ht`, `dp`, `ic`) について [ptex-manual/3](https://github.com/texjporg/ptex-manual/issues/3) の親記事にかかれていたように不統一でした．

e-pTeX 190709 では，次のように挙動が拡張・統一されました．なお `\iffontchar`, `\fontchar??` は，いずれも
`\iffontchar\tenmin```あ` のように対象となるフォント ⟨*font*⟩ と整数 ⟨*number*⟩ をとります．

> * ⟨*font*⟩ が欧文フォントのとき[[BR]]
>   オリジナルの e-TeX と同様に，文字コード ⟨*number*⟩ 番の文字の存在判定・寸法取得（存在しない文字については 0 pt）を行う．
>   <number> が 0--255 の範囲外のときは，「`Bad Character Code`」エラーとなる．
> * ⟨*font*⟩ が和文フォントのとき[[BR]]
>   * ⟨*number*⟩ が 0 以上のときは文字コードとみなし，和文文字コードとして有効か否かの判定や，文字コード ⟨*number*⟩ 番の文字の寸法取得（和文文字コードとして無効ならば 0 pt）を行う．
>   * ⟨*number*⟩ が負のときは，文字タイプ -(⟨*number*⟩+1) についての存在判定・寸法取得（存在しない文字タイプについては 0 pt）を行う．


### `\readline` で和文が取得できない症状を修正 ===
> * [tex-jp-build/88](https://github.com/texjporg/tex-jp-build/issues/88)

e-TeX の拡張プリミティブ `\readline` の和文対応処理が抜けていたことが判明したので，e-pTeX 190908 で修正されました．
それより前の e-pTeX では，次のソースで 2 つめの `\Y` の実行結果が `[ではなく `[ťÁżú‚](漢字‚]`)` に化けてしまいます．
```tex
\font\x=ec-lmtt10\x

\openout0=hoge.out
\write0{漢字}
\closeout0

\openin0=hoge.out
\read0to\Y     [==> [漢字](\Y]%)
\closein0
\openin0=hoge.out
\readline0to\Y [==> [ťÁżú‚](\Y]%) (!?)
\closein0

\bye
```

### `\currentspacingmode`, `\currentxspacingmode` (, `\currentcjktoken`) プリミティブの追加 ===
> * [tex-jp-build/94](https://github.com/texjporg/tex-jp-build/pull/94) (pull request)

pTeX には
> *  `\`(|`no`)`autospacing`：標準の和文間空白 (`\kanjiskip`) の自動挿入を有効化・無効化
> *  `\`(|`no`)`autoxspacing`：標準の和欧文間空白 (`\xkanjiskip`) の自動挿入を有効化・無効化
というプリミティブが，また upTeX には
> *  `\`(`enable`|`disable`|`force`)`cjktoken`：Unicode 文字を「欧文扱い」「和文扱い」のどちらにするかの全体的な設定
>   * `\enablecjktoken`：和文カテゴリーコード (`\kcatcode`) にしたがって定める
>   * `\disablecjktoken`：すべての Unicode 文字を欧文（バイト列）扱いとする
>   * `\enablecjktoken`：U+0080 以上のすべての Unicode 文字（つまり，ASCII 以外）をすべて和文扱いとする
というプリミティブがあります．

前者 2 つの状態は `\showmode` プリミティブを使えば
```
> auto spacing mode;
> auto xspacing mode.
```
のように端末やログに出力されますが，`\show` などと同様にタイプセットが一旦停止してしまいます．
また，最後の `\`(`enable`|`disable`|`force`)`cjktoken` の状態を返すプリミティブはありませんでした．

e-TeX には `\currentgrouplevel` などの「現在の状態」を取得して内部整数を返すプリミティブがあります．
これらとの類似で， e-pTeX 191112 には
> * `\currentspacingmode`：`\kanjiskip` の自動挿入が有効ならば 1，無効ならば 0
> * `\currentxspacingmode`：`\xkanjiskip` の自動挿入が有効ならば 1，無効ならば 0
> * `\currentcjktoken`（e-upTeX のみ）：`\enablecjktoken` ならば 0，`\disablecjktoken` ならば 1，`\forcecjktoken` ならば 2
が追加されました．（2020-12-19 誤記修正）

### `\Uchar`, `\Ucharcat` プリミティブの追加 ===
> * [ZR さんの Twitter](https://twitter.com/zr_tex8r/status/1189063963381600257)，[tex-jp-build/95](https://github.com/texjporg/tex-jp-build/issues/95)

!XeTeX, !LuaTeX ある同名のプリミティブが由来です，大雑把な使い方は
> * `\Uchar`⟨*number*⟩：内部コード ⟨*number*⟩ を持つ文字トークンに展開される
> * `\Ucharcat`⟨*number*⟩` `⟨*catcode*⟩：内部コード ⟨*number*⟩ とカテゴリーコード ⟨*catcode*⟩ を持つ文字トークンに展開される
というもので，`\edef` 内などの展開限定文脈で文字コードから文字トークンを作り出すために利用できます（通常の `\char` は展開可能ではないので，この目的には使えません）．
```tex
%#!luatex
\edef\A{\char65 \Uchar65}
\message{\meaning\A}% ==> macro:->\char 65 A
\bye
```

正確に述べると，次のようになります．
> * `\Uchar`⟨*number*⟩：文字コード ⟨*number*⟩ を持つ文字トークンに展開される．カテゴリーコードは `\string` などの「文字列化」の規則に従う．
>   * ⟨*number*⟩≦255 以下のときは欧文文字トークン．カテゴリーコードは ⟨*number*⟩=32 のときは 10 (*space*)，他は 12 (*other*)．
>   * ⟨*number*⟩≧256 のときは和文文字トークン．e-upTeX で ⟨*number*⟩ 番の文字の和文カテゴリーコードが 15 (*non_cjk*) だったときは生成されたトークンは和文カテゴリーコード 18 (*other_kchar*) を持つ．それ以外の場合は現状の和文カテゴリーコードに従う．
> * `\Ucharcat`⟨*number*⟩` `⟨*catcode*⟩：内部コード ⟨*number*⟩ とカテゴリーコード ⟨*catcode*⟩ を持つ文字トークンに展開される．
>   * e-pTeX では 0≦⟨*number*⟩≦255 （つまり欧文文字トークンの生成）しかサポートされない[では和文文字トークンには和文カテゴリーコードの情報が保存されないため．)]([Footnote(既に述べたように，(e-)pTeX)]．⟨*catcode*⟩ として有効な値は 1--4, 6--8, 10--13 に限られる．
>   * e-upTeX では，和文文字トークンも生成することができる．詳細は「e-pTeX について」（ptex-manual 内）を参照．
>     * 0≦⟨*number*⟩≦127 のときは欧文文字トークンしか生成できない
>     * 128≦⟨*number*⟩≦255 のときは欧文文字トークン・和文文字トークンとも生成できる
>     * ⟨*number*⟩≧256 のときは和文文字トークンしか生成できない

なお，欧文文字，つまり文字コードが 0--255 に限られる場合は，同様のことはマクロで行なえます（上に挙げた ZR さんの Twitter，[StackExchange の記事](https://tex.stackexchange.com/questions/296211/expansion-of-char-in-def)中のコメントを参照）．
TeX Live 2020，およびそれ以前の pTeX 系列ではこれをトリッキーに使うことにより「和文版 `\Uchar`」を実装することができます：
```tex
%#!eptex
\input expl3-generic % for \char_generate:nn
\ExplSyntaxOn
\cs_generate_variant:Nn \cs_to_str:N { c }
\cs_new:Npn \tkchar #1 {
> \cs_to_str:c {
>  \char_generate:nn % upper byte
>   { \int_div_truncate:nn { #1 } { 256 } }
>   { 12 }
>  \char_generate:nn % lower byte
>   { \int_mod:nn { #1 } { 256 } } { 12 }
> }
}
\ExplSyntaxOff
\edef\A{\tkchar{`漢}\tkchar{`字}}
\message{\meaning\A}% ==> macro:->漢字
\bye
```

しかし，将来このトリッキーな使い方が[封じられる可能性](https://github.com/texjporg/tex-jp-build/issues/81) があるため，そうなる前に e-pTeX で `\Uchar`, `\Ucharcat` が実装されたことになります．


#### コメント ====
[[LineComment]]
>  * おっと，早とちりしていました，修正しました．[[id:h7k h7k]([BR]]--) (2020-12-19 19:11:41 JST)
>  * \currentcjktoken は正しくは \enablecjktoken ならば 0，\disablecjktoken ならば 1 です。[[id:aminophen aminophen]([BR]]--) (2020-12-19 13:36:05 JST)
