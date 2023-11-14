# TeX Live 2018 における pTeX, e-pTeX 変更点まとめ

TeX Live 2018 には pTeX p3.8.0 と e-pTeX p3.8.0-180226 が導入される予定です．
本ページでは TeX Live 2017 (p3.7.1) からの変更点についてざっと説明します．
なお，TeX Live 2018 における (p)!LaTeX などの変更点については，
aminophen さんの記事 [TeX Live 2018 注目ポイントまとめ (1)](http://acetaminophen.hatenablog.com/entry/tl2018-01), [(2)](http://acetaminophen.hatenablog.com/entry/tl2018-02) も参照して下さい．

## pTeX

### 組方向変更が可能な条件を厳格化

 * [tex-jp-build/#21](https://github.com/texjporg/tex-jp-build/issues/21), [tex-jp-build/#31](https://github.com/texjporg/tex-jp-build/issues/31), [tex-jp-build/#22](https://github.com/texjporg/tex-jp-build/pull/22)
 * 非限定水平モード（段落を組む）や数式モードでの組方向変更は禁止
 * \discretionary の引数の組方向もチェック（周囲の水平リストと同じ組方向でないといけない）
 * 外部垂直モードの場合は，次の2点が同時に満たされる場合のみ組方向変更可能
  * current page の中身には box, rule, insertion はなく，mark, whatsit のみ (page_contents=empty)
  * recent contributions の中身にも box, rule, insertion はない（ページビルダーの過程で，先頭にある glue, kern, penalty は破棄されるので無視して良い）

#### 非限定水平モード・数式モードについての従来の挙動

段落を開始して非限定水平モードに入ると，TeX の内部では新たにリストが開始されます．
通常の字下げありの段落では先頭に `\parindent` 由来のボックスが挿入されるのでこのリストは空ではありませんが，`\noindent` で開始された字下げなしの段落ではリストが空です．

そのため，従来の pTeX では次のように「段落内の組方向を変える」ことができました：
```tex
\tentgt % 縦組用和文フォント
\noindent\tate かきく\hbox{\dtou あいう}
\bye
```

上の例では，段落自体は縦組で組まれますが，その後の行分割が上位リストの組方向（つまり横組）で進むので，結局「横組の段落を縦組用和文フォントで組んだ」状態になっていました．
文中数式モードでも全く同じことが次のような入力の場合に起こっていました：
```tex
さ$\tate \hbox{しg}$
```

#### 外部垂直モードについて補足

従来の pTeX では，外部垂直モードで組方向が変更なのは recent contributions が空であるときでした．
これは `\par` を実行するなどしてページビルダーが実行されたときにも発生しうる状況だったので，
[tex-jp-build/#21](https://github.com/texjporg/tex-jp-build/issues/21) の dir.tex のように「ページ途中で組方向を変える」ということが可能だった（出力結果は変なことになりますが……）ことになります．

安直に「current page と recent contributions の両方が空のとき」と厳しくすることも考えられましたが，次の要因で見送られました．
 *  tarticle など標準縦組クラスの改修を要する（これらで `\tate` が実行されるとき，current page には `\write` ノードがある）
 * （ただでさえ [forum:1508](https://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=1508) などの症状が起こる）pLaTeX における縦組の使い勝手がさらに悪くなる

### `\hbox{・}` の処理

 *  [tex-jp-build/#25](https://github.com/texjporg/tex-jp-build/issues/25)

#### pTeX の仕様と従来の挙動

pTeX は，水平リストの先頭が JFM 由来のグルーであった場合，`adjust_hlist` という内部の関数で JFM グルーを消す仕様になっています．例えば `\hbox{（} ` としてボックスを作った場合，`adjust_hlist` 関数に渡されるときには，リストの中身は以下のように先頭に JFM グルーがある状態になっています．
```
\glue(refer from jfm) 4.58203 minus 2.291
\tenmin （
\penalty 10000(for kinsoku)
```

`adjust_hlist` 関数でこのグルーは削除され，結局ボックスの中身は次のようになります．
```
\tenmin （
\penalty 10000(for kinsoku)
```

同様に末尾が JFM グルーであった場合もそのグルーは削除されます（正確には 0 にされる）．

しかし，`\hbox{・} `のように `\prebreakpenalty` が設定されている場合は状況が異なり，`adjust_hlist` 関数に渡される時点で，リストは先頭が JFM グルーでありません：
```
\penalty 10000(for kinsoku)
\glue(refer from jfm) 3.04276 minus 3.04276
\tenmin ・
\glue(refer from jfm) 3.04276 minus 3.04276
```

そのため，従来の pTeX では，`\hbox{・} `の最終的な中身は以下のようになっていました．つまり，「前側のグルーだけがなぜか消えない」状況だった，ということです．
```
\penalty 10000(for kinsoku)
\glue(refer from jfm) 3.04276 minus 3.04276
\tenmin ・
\glue(refer from jfm) 0.0
```

#### pTeX p3.8.0 の挙動

pTeX p3.8.0 では，この挙動を修正し，
> リストの先頭が禁則用ペナルティであり，かつその後が（最初の文字に対する）JFM グルーであった場合，その JFM グルーを消す

ようにしました．これにより，`\hbox{・} `の中身は次のようになり，「`・`」の前後に空白が入らないことになります．
```
\penalty 10000(for kinsoku)
\displace 0.0
\tenmin ・
\glue(refer from jfm) 0.0
```

なお，この挙動を調べている最中に，「禁則用ペナルティに 0 が設定されていた場合と未設定の場合で挙動が違う」こともわかりました．後者では禁則用ペナルティ `\penalty0` は挿入されないのに対し，前者では挿入されてしまいます．今回，設定値が 0 であった場合には禁則用ペナルティ `\penalty0` をわざわざ挿入しないようにしました．

### 和文のコントロールシンボル

 * [ZR さんのブログ記事](http://d.hatena.ne.jp/zrbabbler/20171114/1510682869), [tex-jp-build/#37](https://github.com/texjporg/tex-jp-build/issues/37), [uplatex/#3](https://github.com/texjporg/uplatex/issues/3)

トークン列の文字列化の際，TeX82 では次のルールで制御綴が文字列化されています．
 * `\a`, `\relax` など「カテゴリーコードが11の文字」からなる命令（コントロールワード）の後ろには空白 "` `" をつける．
 * それ以外の文字一文字からなる `\!` のような命令（コントロールシンボル）の後ろには空白はつけない．
 * アクティブ文字の後ろも，同様の理由で空白はつけない．

上のルールは，
 * コントロールワードの直後の空白は「命令の名称はここで終わり」という区切りの意味しか持たず読み飛ばされる
 * コントロールシンボルやアクティブ文字の直後の空白は読み飛ばされない

という仕様を反映しています．
そのため，例えば次の入力からは「`a\p \relax \!fuga~12`」が得られ，この文字列をもう一度 TeX に入力として与えても `\hoge` と書くのと同じことになります．
```tex
\def\hoge{a\p\relax\!fuga~12}
\message{\meaning\hoge}
```

同様に，pTeX においては `\（` のような「`\kcatcode` が 18 の文字」一文字からなる命令の直後にある空白も読み飛ばされません．例えば「`\def\（{あ}\message{\（ あ} `」という入力からは「`あ あ`」が得られます．

従来の pTeX では，
> 「`\kcatcode` が 18 の文字」一文字からなる命令も，内部的にはコントロールワードと同様に扱い，文字列化のときに後ろに空白をつける

仕様になっていました．しかしこれでは，
> 目次など文字列化の結果が再度 TeX に入力された際に余計な空白が出力される

という症状がありました（上記 ZR さんのブログ記事参照）．

pTeX p3.8.0 では「`\kcatcode` が 18 の文字」一文字からなる命令か判定するルーチンを追加したので，前段落のような症状は起こらなくなっています．
```tex
\def\1{A}\def\（{あ}
\def\p{\1A\（い}
\message{\meaning\p}
% ==> macro:->\1A\（ い （pTeX p3.7.2 以前）
% ==> macro:->\1A\（い  （pTeX p3.8.0 以降）
```

### エラーメッセージのヘルプ

 * 報告：[ワトソンさんの Twitter](https://twitter.com/wtsnjp/status/943350079263559680)

TeX が長さの単位を正しく読み取れなかったときに返す，次のエラーメッセージのヘルプ（`h` と入力することで見れる）において，pTeX で追加された単位が述べられていないという報告がありました．
```
! Illegal unit of measure (pt inserted).
```

pTeX p3.8.0 ではこの点を修正し，エラーメッセージとそのヘルプは次のようになりました：
```
! Illegal unit of measure (pt inserted).
<to be read again>
> \relax
<*> \hsize=1\relax

? h
Dimensions can be in units of em, ex, zw, zh, in, pt, pc,
cm, mm, dd, cc, bp, H, Q, or sp; but yours is a new one!
I'll assume that you meant to say pt, for printer's points.
To recover gracefully from this error, it's best to
delete the erroneous units; e.g., type `2' to delete
two letters. (See Chapter 27 of The TeXbook.)
```

なお，pTeX で追加された単位は `zw`, `zh`, `H`, `Q` です．

### `\ptexversion` プリミティブなどの追加

 * [tex-jp-build/#14](https://github.com/texjporg/tex-jp-build/pull/14)

pdfTeX などの他のエンジンには `\pdftexversion` などとバージョン情報を取得するプリミティブがあるのに，pTeX 系列にはいままでその種の命令がありませんでした．

他のエンジンに実装されているバージョン情報取得のプリミティブと，その返り値は次のようになっています．
()の中は戻り値で，単なる数は内部整数，" " で囲まれているものは文字列です．
|拡張|命令1|命令2|命令3|
| --- | --- | --- | --- |
|eTeX|`\eTeXversion` (2)|`\eTeXrevision` ("`.6`")|
|pdfTeX|`\pdftexversion` (140)|`\pdftexrevision` ("`.19`")|
|Omega|`\Omegaversion` (1)|`\Omegaminorversion` (15)|\`Omegarevision` ("`15`")|
|Aleph|`\Alephversion` (0)|`\Alephminorversion` (0)|`\Alephrevision` ("`.0`")|
|XeTeX|`\XeTeXversion` (0)|`\XeTeXrevision` ("`.99999`")|
|LuaTeX|`\luatexversion`(107)|`\luatexrevision` ("`0`")|

上の調査を元に，pTeX 系列においてバージョン情報取得のプリミティブを次のように定めました：
|拡張|命令1|命令2|命令3|
| --- | --- | --- | --- |
|pTeX|`\ptexversion` (3)|`\ptexminorversion` (8)|`\ptexrevision` ("`.0`")|
|e-pTeX|`\epTeXversion` (180226)|
|upTeX|`\uptexversion` (1)|`\uptexrevision` ("`.23`")|

### disp_node の処理

 * [tex-jp-build/#40](https://github.com/texjporg/tex-jp-build/issues/40)

pTeX では，欧文文字のベースライン補正を管理するために内部で disp_node と呼ばれるノードを（文字，ボックス，グルーなどと合わせて）段落中に作成しています．従来の pTeX では，この disp_node 関連の処理で，
次の plain pTeX のソースで2つの「`「`」の位置がずれる（1つめの「`「`」だけ半角下がる）という症状がありました：
```tex
\ybaselineshift2pt A\par
\noindent「\par
\noindent「
\bye
```

pTeX p3.8.0 では上記の症状が起こらないように修正しています．

また，ベースライン補正量が 0 のとき，`\hbox{あs} `からは
```
\hbox(7.77588+1.38855)x15.97214, yoko direction
.\tenmin あ
.\glue(\xkanjiskip) 2.40553 plus 1.0 minus 1.0
.\tenrm s
```

と，従来は disp_node がまったくないボックスが作られましたが，pTeX p3.8.0 では
```
\hbox(7.77588+1.38855)x15.97214, yoko direction
.\displace 0.0
.\tenmin あ
.\glue(\xkanjiskip) 2.40553 plus 1.0 minus 1.0
.\tenrm s
```

と，中身の先頭に disp_node を付け加えるようにしました．これはボックスを unbox するときに，ボックス内部と周囲でベースライン補正量が違う場合に対する安全策です．

### 禁則用ペナルティや `\inhibitxspmode` でデフォルト値を設定した場合

 * [tex-jp-build/#57](https://github.com/texjporg/tex-jp-build/issues/57), [tex-jp-build/#26](https://github.com/texjporg/tex-jp-build/pull/26)

和文文字の禁則用ペナルティ (`\prebreakpenalty`, `\postbreakpenalty`) の設定は，256 文字分の領域を持ったテーブルに格納されています．pTeX p3.8.0 以前では，
 * 新たな文字に対して `\prebreakpenalty`, `\postbreakpenalty` を設定した場合には，その設定値がデフォルトの値 0 であったとしても，このテーブルへ登録される．
 * 一旦テーブルに登録されると，デフォルト値に戻す（0 を設定）ことを行ってもテーブルから登録が削除されない

という仕様であり，
ASCII の倉沢さんによる「TeX システムの日本語化」という文書にある次の文言と挙動が一致しませんでした：
> この禁則テーブルからの登録の削除は、
> ペナルティ値!`0'を設定することによって行われるが、
> グローバルレベルでの設定でなければ、
> このテーブルの領域は解放されないことに注意して欲しい。

そのため，pTeX p3.8.0 では，
 * もともとテーブルに登録されていない文字に対して禁則用ペナルティに 0 を設定しても，テーブルへの登録は行わない
 * 既にテーブルに登録されている文字に対して，グローバル，もしくは最外のグループ (`\currentgrouplevel` = 0) で禁則用ペナルティに 0 を設定した場合には，テーブルから登録を削除する

仕様に変更しました．

**但し，TeX Live 2018 に収録されている pTeX 3.8.0 では，この「テーブルからの登録の削除」の処理の際に「テーブルからの検索」に関する考慮忘れがあり，[tex-jp-build/#57](https://github.com/texjporg/tex-jp-build/issues/57) に述べられているような regression が混入してしまっています．**「既に 0 でない禁則用ペナルティが設定されている文字に対して，禁則用ペナルティに 0 を設定する」行為は，TeX Live 2018 においては避けるようにして下さい．

本項目の内容は，禁則用ペナルティだけでなく，「前後に和欧文間空白の挿入を許可するか」を指定する`\inhibitxspmode` についても全く同様のことが当てはまります．

## e-pTeX
### `\epTeXversion` プリミティブの追加

上記「`\ptexversion` など」の項目を参照．

### `\epTeXinputencoding` プリミティブの動作修正

 * [tex-jp-build/#23](https://github.com/texjporg/tex-jp-build/issues/23)

 * Segfault で落ちる場合があったことへの対応
 * 端末からの入力でも効果を発揮するように
 * `\scantokens` 由来の仮想ファイルや `\openin` からの読み込み時には無効

### `\pdfprimitive`, `\ifpdfprimitive` の動作修正

 * [ZR さんの Twitter](https://twitter.com/aminophen/status/910476314691244032), [tex-jp-build/#29](https://github.com/texjporg/tex-jp-build/issues/29)

e-pTeX には pdfTeX 由来のプリミティブとして次の命令が定義されている：
 * `\pdfprimitive\cs`：`\cs` がプリミティブと同じ名前の制御綴だった場合，同名のプリミティブの意味で実行
 * `\ifpdfprimitive\cs`：`\cs` が同名のプリミティブの意味から変更されていなければ真

しかし，（LuaTeX 以外の）TeX では `\ ` といった単一文字の制御綴と複数文字の制御綴の扱いが内部で異なることの考慮が十分になされておらず，
 * 真に判定されるはずの `\ifpdfprimitive\ ` が偽に判定される
 * `\pdfprimitive\ ` がなぜか `\vrule` と同じ動作になる

といった症状が発生したりしていました．他にも，「水平モードだと `\pdfprimitive\ \q ` で発生するはずの `! Undefined control sequence.` エラーがでない」などの症状もありました．

e-pTeX 170924 以降で前段落に述べたような症状が解決された他，同様の修正が TeX Live 2018 の pdfTeX, XeTeX にも適用されています．

### `! File ended ...` エラーの修正

 * [tex-jp-build/#39](https://github.com/texjporg/tex-jp-build/issues/39)

`\pdfstrcmp`, `\pdfmdfivesum` などで引数の読み取り中に `\endinput` が来た場合のエラーメッセージが
```
! File ended while scanning text of \pdfmdfivesum.
```

のようになるべきところ，
```
! File ended while scanning text of ???.
```

となっていたので修正しました．

### `\lastnodesubtype` プリミティブの追加

e-TeX にはリストの最後のノードの種類を返す `\lastnodetype` という命令があります．しかし，これでは例えば「カーンであること」は分かっても，それが明示的な `\kern` によるものなのか，アクセント `\accent` の処理によるものなのか，などがわかりませんでした．

最近，[tex-jp-build/#28](https://github.com/texjporg/tex-jp-build/issues/28) の議論などで，「JFM由来グルーのみを消す」処理の必要性が唱えられてきたので，それを実装するために追加したプリミティブがこの `\lastnodesubtype` です．

`\lastnodesubtype` は，内部で「最後のノード」の subtype を表すのに使っている整数を，内部整数としてほぼそのまま返します．
 * e-pTeX の `\lastnodetype` と同様に，自動挿入される disp_node は読み飛ばされます．dir_node についても，そのノードが格納している hlist_node, vlist_node についての情報を返すことになります．
 * 文字ノードに対しては `\lastnodesubtype` の値は 0 です．リストの先頭では -1 です．

`\lastnodesubtype` の活用例は，pLaTeX 2018-03-09 で追加された `\removejfmglue` 命令です：
```tex
\def\pltx@gluetype{11}
\def\pltx@jfmgluesubtype{21}
\protected\def\removejfmglue{%
  \ifnum\lastnodetype=\pltx@gluetype\relax
    \ifnum\lastnodesubtype=\pltx@jfmgluesubtype\relax
    \unskip
    \fi
  \fi}
```

## JFM の改善・拡張

 * [tex-jp-build/#8](https://github.com/texjporg/tex-jp-build/issues/8), [tex-jp-build/#47](https://github.com/texjporg/tex-jp-build/pull/47)

### 0x10000 以上の文字コードをサポート

従来の JFM では文字コード 0xaabb がその文字クラス 0xccdd に属することを `aa bb cc dd` という 4 バイトで表現していましたが，文字クラスは glue/kern テーブルの仕様により 0--255 の範囲しか扱えていないので，cc のところは 00 固定となっており，この 8 ビットが無駄になっていました．

（pTeX では文字コードは 2 バイトの範囲に収まるので大丈夫ですが）upTeX では BMP に収まらない，つまり文字コードの値が 0x10000 以上の漢字も使います．しかし，前段落に述べた仕様では BMP 外の文字の文字クラスは 0（既定値）に固定されてしまいます．それも考慮して，TeX Live 2018 以降では上記の `aa bb cc dd` という 4 バイトを
> 文字コード 0xccaabb の文字は，文字クラス 0xdd に属する

と解釈し直して，0x10000 以上の文字コードを持つ文字にも文字クラスが設定できるようにしました．

### glue/kern テーブルの制限緩和

オリジナルの PLtoTF では，
 * 異なったカーニング量（kern テーブルのエントリ数）は 500 個まで
 * リガチャ・カーニングの指定（lig_kern テーブルのエントリ数）は 5000 個まで

という制約がありましたが，Web2C 版ではそれぞれ 5000, 32510 まで増やされています．

一方，(u)pPLtoTF では，
 * 異なったカーニング量（kern テーブルのエントリ数）は 255 個まで
 * JFM グルー・カーンの指定（glue/kern テーブルのエントリ数）も 255 個まで

となっていました．min10.tfm や jis.tfm を作成する上では問題がありませんでしたが，プロポーショナル仮名用の tfm を作る場合などにこの制限に引っかかることが容易に想定されます．

そのため，TeX Live 2018 の (u)pPLtoTF では
 * 異なったカーニング量（kern テーブルのエントリ数）は 5000 個まで
 * JFM グルー・カーンの指定（glue/kern テーブルのエントリ数）も 32510 個まで

と制限をオリジナルの PLtoTF 並に緩和しています．

また，欧文 tfm の元となる pl ファイルでは，リガチャ・カーニングの指定を行う LIGTABLE 内に SKIP 指定というのを記述することが可能でした．jfm の元となる jpl ファイルでも，[仕様書 (Web Archive)](https://web.archive.org/web/20160404231913/http://ascii.asciimw.jp:80/pb/ptex/pl/index.html) では使用可能と書かれていたのに実際にはそうではなかったですが，これも使えるように修正されました．

## 検討中の事項→結局 TeX Live 2018 には入りませんでした．

この節の内容は TeX Live 2018 に適用されませんでしたが，2019 以降に向け引き続き検討中の事項です．

### `\inhibitglue` の有効範囲

 * 検討場所：[tex-jp-build/#28](https://github.com/texjporg/tex-jp-build/issues/28)

[以前](http://doratex.hatenablog.jp/entry/20140714/1405302796)にも問題になりましたが，「`\inhibitglue` が有効となる『このプリミティブを挿入した箇所』はどこを指すのか？」という話題です．

（書きかけ）

現在 inhibit_flag_list ブランチにて仕様の検討が行われています．ポイントを抜き出すと以下の 3 つで，そのうち最初の 2 つは `\spacefactor` の仕様をヒントとしたものです．
 * `\inhibitglue` が有効か否かの情報は単一グローバル変数でなく，リストごとに持つ
 * ノードを作らない命令（`\relax`，各種代入など）は `\inhibitglue` の状態をリセットしない．一方，文字やその他ノードが作られたら `\inhibitglue` の状態はリセットされる
 * `\disinhibitglue` プリミティブ（`\inhibitglue` 命令の効果を打ち消す）を追加

注意すべき挙動として，次の入力では「）」「（」の間に JFM 由来の空白が**入らない**（代わりに `\kanjiskip` が挿入される）ことが挙げられます：
```tex
）\inhibitglue\relax（
```

これは，上の入力から以下のように処理が進むためです：
 * 「`）\inhibitglue\relax`」により，本来「）」と `\relax` の間に入るはずの JFMグルー（半角）の挿入は抑止される．
 * `\inhibitglue` の次の `\relax` はノードを作らないので，`\inhibitglue` の状態はリセットされない．
 * 従って，「`\relax（`」の間に入るはずの JFM グルーの挿入も抑止される．

