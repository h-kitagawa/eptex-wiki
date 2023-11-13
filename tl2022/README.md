# TeX Live 2022 における pTeX, e-pTeX 変更点まとめ

TeX Live 2022 には pTeX p4.0.0 と e-pTeX 220214 が収録されています．
本ページでは昨年の TeX Live 2021 (pTeX p3.9.0, e-pTeX 210218) からの変更点についてざっと説明します．

## pTeX
### 欧文文字由来のバイト列と和文文字の区別
 * 発端：[platex/84](https://github.com/texjporg/platex/issues/84)
 * [tex-jp-build/81](https://github.com/texjporg/tex-jp-build/issues/81)
 * 関連文書：
   * [pLaTeX が本格的にやばいかもという話](https://acetaminophen.hatenablog.com/entry/2021/06/18/022108)（aminophen さんによる，LaTeX 側からの記事）
   * 「pTeX 系列の文字列化における和文欧文の区別」[スライド PDF](https://github.com/h-kitagawa/presentations/blob/main/tc19ptex.pdf)
   * Hironori Kitagawa. Distinguishing 8-bit characters and Japanese characters in (u)pTeX, [TUGboat 41(3)](https://www.tug.org/TUGboat/Contents/contents41-3.html):329--334, 2020.

詳細は上記の「関連文書」を見てもらうことにして，ここでは概要のみ説明します．

TeX では入力を**トークン**単位で処理することが基本になっていますが，
 * `\write`, `\detokenize` などの命令において，「トークンを文字列化」することがある
 * `\meaning` などの命令では，文字列化に加えて「文字列化されたものをトークン列」される

 と，**文字列化**の処理が絡むことがあります．

さて，pTeX ではトークンの段階で「和文文字トークン」「欧文文字トークン」の区別がありますが，p3.9.0 以前では文字列化の過程で「和文文字」「欧文文字」の区別がありませんでした．これにより，以下のソースのように 8-bit 欧文文字（を表すバイト列）が和文文字に化けてしまうという症状がありました[^1]：
[^1]: 3 行目で「Å£」が出力されるのは，使用フォント（ec-lmtt10，T1 エンコーディング）において 0xC5 の位置のグリフが「Å」，0xBF の位置のグリフが「£」であるからです．
```tex
\font\x=ec-lmtt10\x
% 顛: C5 BF in EUC，ſ: C5 BF in UTF-8
\def\a{顛.ſ}\a% ==> 顛.Å£ （意図通り）
\meaning\a    % p3.9.0 以前：==> macro:->顛.顛 （？）
\bye
```

また，制御綴の名称についても，文字列化と類似の現象が見られ，p3.9.0 以前では
```tex
\顛
\csname 顛\endcsname
\ſ % \catcode"C5=11 \catcode"BF=11
\csname ſ\endcsname
```
の 4 つの制御綴はいずれも同じものを指し，`\expandafter\string\csname ſ\endcsname` は `\顛` を返しました．

**pTeX p4.0.0 以降では，文字列化において「このバイト列は欧文由来」「このバイト列は和文由来」という区別をつけるようにしました．** これにより，以下のソースのように「顛」「ſ」が区別されるようになりました．
```tex
% pTeX p4.0.0
\font\x=ec-lmtt10\x
% 顛: C5 BF in EUC，ſ: C5 BF in UTF-8
\def\a{顛.ſ}\a% ==> 顛.Å£
\meaning\a    % ==> macro:->顛.Å£

\catcode"C5=11 \catcode"BF=11
\顛.\csname 顛\endcsname.\string\顛 % ==> foo.foo.\顛
\ſ.\csname ſ\endcsname.\string\ſ    % ==> bar.bar.\Å£
\bye
```

#### upTeX における制御綴名の注意
上記の変更に伴い，TeX Live 2022 以降の upTeX では「ソース中の見た目は同じなのに違う制御綴を指す」ことが起こります．例を以下に示します：
```tex
% upTeX p4.0.0-...
\catcode"E3=11 \catcode"81=11 % ぁ: E3 81 81 in UTF-8
\def\ぁ{foo}\def\^^e3^^81^^81{bar}
\ぁ % ==> foo
\^^e3^^81^^81 %==> bar

\kcatcode`ぁ=15 % non-cjk
\ぁ % ==> bar
\bye
```
この例において，8 行目では「ぁ」は和文文字扱いされないため，8 行目の `\ぁ` は `\^^e3^^81^^81` と同じ制御綴を指します．一方，4 行目の `\ぁ` と 8 行目の `\ぁ` は違う制御綴です．

#### `\write18` の引数の扱い（非 Windows 環境下）
`\write18` を使って外部コマンドを実行させることができます（shell-escape[^2]）．
[^2]: ただし，標準では「安全」と認められたいくつかのプログラムのみ呼び出せるようになっています．詳細は [TeX Wiki 中の記事](https://texwiki.texjp.org/?%E5%A4%96%E9%83%A8%E3%82%B3%E3%83%9E%E3%83%B3%E3%83%89%E3%81%AE%E5%AE%9F%E8%A1%8C)を参照．

この `\write18` の引数は，pTeX p3.9.0 以前では pTeX の内部コードのままシェルに渡されるようになっていました．
一方，非 Windows 環境の pTeX p4.0.0 では，常に UTF-8 に変換されるようになります．この際，JIS X 0208 で表されないバイト列は `^^xy` 形式に直されます．

以下に例を示します．次のファイルを `-shell-escape` 付きで（つまり制限なし shell-escape で）実行させます：
```tex
\write18{echo 'あ顛ß.①.ſ' | od -t x1c}
\bye
```
すると，以下のような出力が得られます：
```
■p3.9.0，内部コードは EUC
0000000  a4  a2  c5  bf  c3  9f  2e  e2  91  a0  2e  c5  bf  0a
        244 242 305 277 303 237   . 342 221 240   . 305 277  \n
0000016
■p3.9.0，内部コードは SJIS
0000000  82  a0  93  5e  c3  9f  2e  e2  91  a0  2e  c5  bf  0a
        202 240 223   ^ 303 237   . 342 221 240   . 305 277  \n
0000016
■p4.0.0，内部コードは EUC/SJIS どちらでも
0000000  e3  81  82  e9  a1  9b  5e  5e  63  33  5e  5e  39  66  2e  5e
        343 201 202 351 241 233   ^   ^   c   3   ^   ^   9   f   .   ^
0000020  5e  65  32  5e  5e  39  31  5e  5e  61  30  2e  5e  5e  63  35
          ^   e   2   ^   ^   9   1   ^   ^   a   0   .   ^   ^   c   5
0000040  5e  5e  62  66  0a
          ^   ^   b   f  \n
0000045
```

#### ファイル名の扱い（非 Windows 下）
pTeX では，**JIS X 0208 の範囲のファイル名しか扱うことはできません**'．JIS X 0208 範囲外の文字がファイル名に含まれた場合（たとえば「`あſßa.tex`」），
 * pTeX p3.9.0 では「`あ顛.tex`」が指定されたものと解釈される（「ſ」が和文文字「顛」に化ける，「ß」(C3 9F in UTF-8) は文字列終端のヌル文字に変換されてしまう）
 * pTeX p4.0.0 では「`あ^^c5^^bf^^c3^^9fa.tex`」が指定されたものと解釈される（`^^` 記法に変換される）
となります．

ファイル名の扱いにはまだ荒削りのところがあります．また，「既定の内部コード」[^3]でない状態で pTeX を動作させることも可能です：
[^3]: Windows では Shift_JIS，それ以外の環境では EUC-JP．
```sh
eptex -ini -kanji-internal=sjis -etex -jobname=ep-sj eptex.ini 
# ↑内部コード Shift_JIS の plain eptex format
eptex -fmt=ep-sj hoge.tex
```
が，その場合，コマンドラインに渡すファイル名は ASCII の範囲に留めるべきです（JIS X 0208 の範囲内であっても和文文字は使えません）．

### 「和文文字 + グループ境界 `{}`」で行が終了した場合などの改行の扱い
 * [tex-jp-build/87](https://github.com/texjporg/tex-jp-build/issues/87), [TeX Forum 中の記事](https://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=2682)

#### pTeX の過去の挙動と p4.0.0 既定の挙動
pTeX 系列で「和文文字の直後の改行は何も発生しない」ということはよく知られていますが，「和文文字の直後に 1 つ以上のグループ境界 ({, }) がある」状況で行が終わっても，改行は何も発生させない（空白文字が発生しない）ことが知られています．
```tex
◆漢字
◆{漢字}
◆% どちらも「字」「◆」の間には空白は入らない
```

これに関連して，
 * 「`\】` のような和文文字 1 文字からなるコントロールシンボル」で行が終了した場合
```tex
\def\】{あ}
◆\】
◆{\】}
◆
```
   →pTeX p3.8.1 以前では改行から空白文字が発生したが，pTeX p3.8.2 (TeX Live 2019) から何も発生しなくなった
 * 「`\aあ}` のような和文文字で終わるコントロールワードの直後に 1 つ以上のグループ境界 ({, }) がある」で行が終了した場合
```tex
\def\ほげ{い}
◆{\ほげ}
◆
```
   →pTeX p3.9.0 (TeX Live 2021) 以前では改行から空白文字が発生したが，**pTeX p4.0.0 (TeX Live 2022) からは既定では何も発生しなくなった**

のように，和文文字と改行の扱いについては近年（今年も含め）変更が加えられています．

#### `\ptexlineendmode`
一方で，今回の変更の過程で，最初に述べた
 >「和文文字の直後に 1 つ以上のグループ境界 (`{`, ` }`) がある」状況で行が終わっても，改行は何も発生させない（空白文字が発生しない）

という従来の挙動も自然とはいえないのではないか，という話が出てきました．結果的にはこの挙動は p4.0.0 の既定としては残りましたが，ユーザ側で挙動を変更できるように，pTeX p4.0.0 では `\ptexlineendmode` というプリミティブを追加しました．

`\ptexlineendmode` は 0--7 の値を取る（それ以外の値も代入できますが，下位 3 ビットの値しか見ません）内部整数です．
各ビットは，それぞれ以下の状況で行が終わった場合に，改行が空白文字を発生させるかを制御します（0: 発生させない，1: 発生させる）：
 * 最下位ビット：`\aあ` のような和文文字で終わるコントロールワードの直後に 1 つ以上のグループ境界がある
 * 下から 2 番目のビット：`\】` のような和文文字 1 文字からなるコントロールシンボル
 * 下から 3 番目のビット：（制御綴の名称の一部でない）和文文字の直後に 1 つ以上のグループ境界がある
 * 「`\】` のような和文文字 1 文字からなるコントロールシンボルの直後に 1 つ以上のグループ境界がある」で行が終わった場合は，下から 2, 3 番目の両方とも 0 であるときのみ，改行は何も発生させません．

pTeX p4.0.0 での既定値は 0（すべて発生させない）です．また
 * pTeX p3.8.2--p3.9.0 (TeX Live 2019--2021) での挙動は`\ptexlineendmode=1`
 * pTeX p3.8.1 以前での挙動は `\ptexlineendmode=3`
でそれぞれ再現できます．

例として，以下のソースをタイプセットした結果が `\ptexlineendmode` の値でどうなるかを載せておきます：
```tex
\def\】{あ}\def\ほげ{い}
◆漢字
◆{漢字}
◆\】
◆\ほげ
◆{\】}
◆{\ほげ}
◆
```

| `\ptexlineendmode` の値 | 結果 |
| --- | --- |
| 0 | `◆漢字◆漢字◆あ◆い◆あ◆い◆` |
| 1 | `◆漢字◆漢字◆あ◆い◆あ◆い ◆` |
| 2 | `◆漢字◆漢字◆あ ◆い◆あ ◆い◆` |
| 3 | `◆漢字◆漢字◆あ ◆い◆あ ◆い ◆` |
| 4 | `◆漢字◆漢字 ◆あ◆い◆あ ◆い◆` |
| 5 | `◆漢字◆漢字 ◆あ◆い◆あ ◆い ◆` |
| 6 | `◆漢字◆漢字 ◆あ ◆い◆あ ◆い◆` |
| 7 | `◆漢字◆漢字 ◆あ ◆い◆あ ◆い ◆` |

### `\partokenname`, `\partokencontext` プリミティブの追加 ===
 * [TeX Live SVN r60054](https://tug.org/svn/texlive?view=revision&revision=60054)

どちらも (e-)(u)pTeX, pdfTeX, !XeTeX, !LuaTeX 共通に実装されたプリミティブです．

TeX では `\par` という**名称**のプリミティブは
 * 空行，非限定水平モードで垂直モード用の命令（`\vskip` など）が来た場合，`\end` の直前等に自動挿入される
 * `\long` なしマクロでの引数内で禁止される
等の特殊な役割があります．

`\partokenname` プリミティブは，この「特殊な役割」を持つプリミティブの**名称**を変更します．使用例を以下に示します．
```tex
\partokenname\MYPAR
\def\MYPAR{!!\par}
\def\hoge#1{(#1)}
a

b % 空行によって \par ではなく \MYPAR が挿入される
\hoge{x\par y} % ==> no error
\hoge{x\MYPAR y} % ==> error "! Paragraph ended before"
\bye
```

`\partokencontext` プリミティブは，「特殊な役割」を持つプリミティブ（標準だと `\par`）の挿入箇所を制御します．
 * 0：従来通りの動作．
 * 1：従来通りの箇所の他，`\vtop`, `\vbox`, `\vcenter` の末尾（非限定水平モードのとき）．
 * 2：1 の場合の挿入箇所の他，`\valign` 中の各アイテム，`\halign` 中の `\noalign`，`\insert`,  `\output`,  の末尾（非限定水平モードのとき）．
既定値は 0（つまり従来通りの動作）です．

### `\ucs`, `\toucs` プリミティブの追加
 * [tex-jp-build/121](https://github.com/texjporg/tex-jp-build/pull/121), [tex-jp-build/122](https://github.com/texjporg/tex-jp-build/issues/122)

もともと upTeX にUnicode から内部コード値への変換を行う `\ucs` プリミティブ がありましたが，pTeX でも取り入れました．逆の
> 内部コードから Unicode への変換を行う `\toucs` プリミティブ

も追加しています．pTeX では，これらは（`\euc` 他と同様に）JIS X 0208 の範囲のみ扱えます．

コード変換プリミティブ (`\euc`, `\jis`, `\sjis`, `\kuten`) に引数として不正な値を与えた場合，pTeX p3.9.1 以前では
 * 「JIS X 0208 における最初のみ定義位置」に対応する EUC A2AF, SJIS 81AD
 * 「1 区 0 点」に対応する EUC !A1A0, SJIS 813F
 * -1

など返り値が不統一でした．
`\ucs`, `\toucs` 追加に伴い，`\euc`, `\jis`, `\sjis`, `\kuten` ともども
 * 文字コード変換が不要なケースは，従来通り恒等変換
 * 文字コード変換が必要なケースは，常に -1 が返る

と統一しました．

# 和文文字トークンを `\let` した制御綴の和文カテゴリーコードの扱い
* [ptex-manual/4]([https://github.com/texjporg/ptex-manual/issues/4), [tex-jp-build/120](https://github.com/texjporg/tex-jp-build/pull/120)

欧文文字トークンと異なり，和文文字トークンについては一般に
 * pTeX では，和文カテゴリーコードは和文文字トークンには保存されず，その都度算出される
 * upTeX では，和文カテゴリーコードは和文文字トークンに保存される 
という挙動になっています． 

さて，和文文字トークンを `\let` したトークンについて，
[https://osdn.net/projects/eptex/wiki/tl2020#h3-.E5.92.8C.E6.96.87.E6.96.87.E5.AD.97.E3.83.88.E3.83.BC.E3.82.AF.E3.83.B3.E3.82.92.20.7B.7B.7B.5Clet.7D.7D.7D.20.E3.81.97.E3.81.9F.E5.88.B6.E5.BE.A1.E7.B6.B4.E3.81.AE.E5.92.8C.E6.96.87.E3.82.AB.E3.83.86.E3.82.B4.E3.83.AA.E3.83.BC.E3.82.B3.E3.83.BC.E3.83.89.E3.81.AE.E6.89.B1.E3.81.84.E3.80.90R.E3.80.91 TeX Live 2019 リリース後のリビルド] において
 `\if`, `\ifcat` において，和文文字トークンを `\let` したトークンについては和文カテゴリーコードをその都度算出する
という変更を行いましたが，`\ifx` においてはそうなっておらず，不統一な状態になっていました．
```tex
\kcatcode`あ=16\relax
\let\X=あ
\kcatcode`あ=17\relax
\let\Y=あ
\ifcat\X\Y T\else F\fi% ==> T (p3.9.0), F (p4.0.0)
\ifx\X\Y   T\else F\fi% ==> F
\end
```

`\ifx` についても和文カテゴリーコードを再計算するように変更することもできますが，その方針では pTeX と upTeX の差異が大きくなることから，
 和文文字トークンを `\let`したトークンについては和文カテゴリーコードが保存される（再計算しない）
という以前の挙動に戻すことにしました．

### 禁則テーブルのハッシュ検索バグの修正
 * [tex-jp-build/113](https://github.com/texjporg/tex-jp-build/issues/113)，[TeX forum 中の記事](https://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=3113)

[TeX Live 2018 における pTeX, e-pTeX 変更点まとめ](../tl2018/) にも記述したように，pTeX 3.8.0 以降では，グローバルに，もしくは最外グループ ( `\currentgrouplevel = 0`) で禁則ペナルティに 0 を設定した場合は，禁則テーブルから削除するようになっています．

この禁則テーブルはハッシュテーブルの形で実装されており，たとえば ```\prebreakpenalty`あ=1000``` という設定においては，
 * 「あ」の内部コードから定まるハッシュ値を *h* とする．
 * *h* 番のエントリから順に検索し，「あ」に対応するエントリか，現在使われていないエントリを探す．
 * このエントリに「「あ」の内部コードに対する `\prebreakpenalty` は 1000」という情報を書き込む
という順序で処理が行われています．
しかし，TeX Live 2021 (p3.9.0) 以前では
> 「あ」に対応するエントリか，現在使われていないエントリを探す
という部分にバグがあり，
> 実際には「あ」に対応するエントリが存在するが，テーブルから削除済みで「未使用」となっているエントリが先に見つかってしまう
という現象が起こっていました．

### 現在のフォントが `\nullfont` のとき， `\noindent a\par` すると落ちるバグを修正
 * [tex-jp-build/123](https://github.com/texjporg/tex-jp-build/issues/123)

報告元の表現を少し変えると，次のソースが segmentation fault して落ちるというものです：
```tex
%#! ptex
\nullfont\noindent a\end
```
欧文文字（の連続）の周囲には pTeX が自動で disp_node を補いますが，段落の行分割を行う際に「余計な末尾の disp_node を消去する」という処理があります．しかし，上記のソースを処理すると，
 1. 欧文フォントが「何も文字が定義されていない」`\nullfont` になるため，「a」由来の文字ノードは生成されない
 2. よって，段落は disp_node のみで構成されるが，行分割の前処理でこの disp_node が余計と判断され消去される
ことになってしまい，これが segmentation fault の元になっていました．

### `\mathcode` の適用範囲が 0--128 のみだった問題を修正
* [tex-jp-build/129](https://github.com/texjporg/tex-jp-build/issues/129)

128--255 番の文字に対して `\mathcode` の値の設定はされているものの，実際の数式モードではその値が反映されなかったという問題です．たとえば，以下のソースが例になります：
```tex
\mathcode"80="1041
\mathchardef\AB="1041
$[^^80][\AB]$
\the\mathcode"80 \bye
% ==> [][A] 4161 (pTeX p3.9.0)
% ==> [A][A] 4161 (pTeX p4.0.0)
```

日本語化パッチの該当部分が TeX 2（7 ビット入力のみ対応していた）の頃のままになっており，数式モードで入力された 128--255 番の文字が「`\mathcode` はその文字の文字コードの値」のように扱われていたのが原因です．

## e-pTeX
### `\showstream` プリミティブの追加
 * [TeX Live SVN r60992](https://tug.org/svn/texlive?view=revision&revision=60992)

e-(u)pTeX, pdfTeX, XeTeX, LuaTeX 共通に実装されたプリミティブで，以下の `\show...` 系プリミティブ
> `\show`, `\showthe`, `\showbox`, `\showlists`,  `\showifs`, `\showgroups`，e-pTeX 系には `\showmode` が加わる

の出力先を端末・ログファイルから別ファイルに変更するためのものです．

TeX は，同時に 0--15 の 16 個の入出力ストリームを持つことができます．TeX からファイルに入出力を行う際は，
 * 読み込み：即時実行
   * ```\openin10=hoge.txt``` でストリームをオープン
   * ```\read10 to \cs``` で 1 行を読み込む
   * ```\closein10``` でクローズ
 * 書き込み：`\immediate` を前置すれば即時実行，そうでなければ現在のページが dvi に出力する際にまとめて実行．
   * ```\openout10=hoge.txt``` でストリームをオープン
   * ```\write10{hoge}``` で 書き込む
   * ```\closeout10``` でクローズ
のように行います[^4]．
[^4]: 0--15 の範囲外の値，例えば `\read16to\cs` や `\immediate\write-1{hoge}` はそれぞれ端末からの入力，端末（・ログファイル）への出力となります．ただし，`\write18` は例外的に，外部コマンドの実行に使われます．

今回の `\showstream` にストリーム番号を指定することで，`show...` 系プリミティブの結果の出力先がその番号のストリームになります．ただし，0--15 以外の値や，（`\show...` 実行時に）その番号のストリームが出力ストリームとしてオープンされていなかった場合は，既定の「端末に出力する」挙動に戻ります．
以下が使用例です．

``` tex
\def\cs{foo\tenrm testあいう}

\scrollmode
\show\cs              %==> macro:->foo\tenrm testあいう
\showthe\baselineskip %==> 12.0pt

\message{OPENIN}
\openin1=sst2.txt
\showstream=1 % 入力ストリームなので効力なし
\show\cs\showthe\baselineskip
\closein1

\message{OPENOUT}
\immediate\openout1=sst.txt
\showstream=1
\show\cs\showthe\baselineskip% sst.txt に出力される
\immediate\closeout1

\message{18.}
\showstream18 % デフォルトの挙動に戻る
\show\cs\showthe\baselineskip

\message{-1.}
\showstream-1 % デフォルトの挙動に戻る
\show\cs\showthe\baselineskip
\bye
```

### `\vadjust pre`  のサポート
 * [golden_lucky さんの Twitter](https://twitter.com/golden_lucky/status/1395655595844272131)，[tex-jp-build/115](https://github.com/texjporg/tex-jp-build/pull/115) 

`\vadjust{`*vertical material*`}` プリミティブを使うことで「現在の行の直後」に *vertical material* を挿入することができることはよく知られています．

さらに pdfTeX, XeTeX, LuaTeX  では `\vadjust pre{`*vertical material*`}` とすることで「現在の行の直前」に *vertical material* を挿入することができるようになっていますが，これが e-pTeX にも追加されました．
なお，前の行に `\vadjust` が，次の行に `\vadjust pre` があった場合は
>（前の行）→（前の行の `\vadjust` 由来）→（次の行の `\vadjust pre` 由来）→（行間グルー）→（次の行）

の順序で組まれます．

### `\suppresslongerror`  他エラー抑止のプリミティブの追加
 * [tex-jp-build/127](https://github.com/texjporg/tex-jp-build/pull/127)

LuaTeX に実装されている次のエラー抑止のためのプリミティブを実装しました：
 * `\suppresslongerror`：0 でない場合，`\long` なしマクロの引数に `\par` が含まれた場合のエラーを抑止する
 * `\suppressoutererror`：0 でない場合，`\outer` 付きマクロが禁止されている状況で `\outer` 付きマクロが来た場合のエラーを抑止する
 * `\suppressmathparerror`：0 でない場合，数式モード中に `\par` が来た場合のエラーを抑止する
既定値はいずれも 0（つまり従来通りの動作）です．

詳しくは [TeX ＆ LaTeX Advent Calendar 2021](https://adventar.org/calendars/6724) の私の記事「 [e-(u)pTeX をいじってみた話](https://qiita.com/h-kitagawa/items/de963380bd3e576ab4e3)」前半部の「ネタ 1」を参照してください．


### `\lastnodefont` プリミティブの追加
 * [aminophen さんの Gist「e-(u)ptex: Add \lastnodefont primitive」](https://gist.github.com/aminophen/70f55a916b27deb27d41c8bbfe558c19)

以前，[ \lastnodechar プリミティブについて](../lastnodechar/)に
> 文字コードと同様に，「最後の和文文字のフォント」を知る方法は現時点ではなく，
> 新たに \lastnodefont とでもいう命令を作る以外にないでしょう

と書きましたが，(e-)upTeX においては「文字コードが 256 未満の和文文字ノード」が存在する関係で，`\lastnodechar` の値だけでは最後の文字が和文か欧文か判別できませんでした．これを解決するため，e-pTeX の段階で `\lastnodefont` が追加されました．

### `\lastnodesubtype` の数式モードでの挙動変更
 * [tex-jp-build/104](https://github.com/texjporg/tex-jp-build/issues/104)

数式モードにおいては，e-TeX 拡張の「現在構築中のリストの最後のノードの種別」を与える `\lastnodetype` の値はみな 15 (math mode nodes) であり，ほとんど意味をなしていませんでした．
e-pTeX では `\lastnodetype` を補完する `\lastnodesubtype` プリミティブがありますが，数式モードではこれもほぼ役に立ちませんでした．

e-pTeX 210701 以降では，数式モードにおいて `\lastnodesubtype` が「意味のある」情報を返すようにしました．
詳細は e-pTeX のマニュアルを見てもらうことにして，一部を述べると以下のようになります：
| 値 | 意味 |
| --- | --- |
| 1 | `\mathchoice` |
| 2 | `\mathord` |
| 3--5 | `\mathop`（`\displaylimits`, `\limits`, `\nolimits` で異なる値）|
| 6 | `\mathbin` |
| 7 | `\mathrel` |
| 8 | `\mathopen` |
| 9 | `\mathclose` |
| 10 | `\mathpunct` |
| 11 | `\mathinner` |

