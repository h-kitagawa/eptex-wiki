# 旧版

このページには以前のバージョンに関する情報を載せてあります．

 * [eptex-110315.tar.xz](eptex-110315.tar.xz?raw=1)
   * `\pdfpagewidth`, `\pdfpageheight` の挙動に関するバグを修正．
   * [qa:55775](http://oku.edu.mie-u.ac.jp/~okumura/texfaq/qa/55775.html) に報告されているバグを修正（したと思う）.
 * [eptex-110227.tar.xz](eptex-110227.tar.xz?raw=1)
   * #24542 を解決．
   * ↓のパッチをベースに，`\pdfsavepos` まわりを追加．
     * `\pdfpagewidth`, `\pdflastxpos` は「物理的な」ページ幅/水平位置を意味します．
     * `\special{papersize=100pt,200pt}` のように papersize special が与えられたとき，
       e-pTeX 内部でそれを解釈し，`\pdfpagewidth` 等を変更するようにしました．解釈される条件は↓と同じです．
   * TeX Live svn における pTeX / e-pTeX への修正を取り込みました．
 * [eptex-110206-pdfsavepos-draft-tl10.patch](eptex-110206-pdfsavepos-draft-tl10.patch?raw=1)  
  ↓の e-pTeX 110102 に対し，pdfTeX 由来の `\pdfsavepos`, `\pdflastxpos`, `\pdflastypos` を追加する試験的なパッチです．
  また，`\pdflastypos` の処理などを考慮した結果，`\special{papersize=...}` を e-pTeX 内部である程度解釈するようにしています．この処理の都合で，`\special`  命令まわりで誤動作が生じる可能性がありますので，本番の TeX 環境には適用しないようにしてください．
 * [eptex-110102.tar.xz](eptex-110102.tar.xz?raw=1)
   * e-pTeX 101231 を若干修正したものです．\pdfstrcmp における和文文字の比較を UTF-8 で行うようにしました．
    また，前版では入れていなかった utfsrc.zip, otfstable.zip を同梱しました．
   * autoreconf を実行するので，GNU autoconf (==2.65), GNU automake (>=1.11?) が必要です．
     m4 のバージョンにも依存するかも（こちらの環境では GNU m4-1.4.15 です）．
 * [eptex-101231.tar.xz](eptex-101231.tar.xz?raw=1)  
  TeX Live 2010 に対応した**試験版**．公開が非常に遅くなってしまいましたが，テストは相変わらず不十分です，  
  2010/12/31 15:38 ごろにファイルを差し替えました（doc/ の欠落と INSTALL.txt 不備のため）．
   * 前バージョン（100420）からの更新部分：
     * インストール方法を変えた．e-pTeX の核心部分は前バージョンから変わっていないはず．
     * [upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) との同時コンパイルですが，上の配布ファイル内では見送っています．
       （最初から同時コンパイルを目標に入れて開発していましたが，upTeX のパッチを全部再構成してしまったので^^;）
   * autoreconf を実行するので，GNU autoconf (==2.65), GNU automake (>=1.11?) が必要です．
     m4 のバージョンにも依存するかも（こちらの環境では GNU m4-1.4.15 です）．
   * LaTeX3 に必要な \pdfstrcmp に対応 (pdfstrcmp.ch)．多分前バージョンや e-TeX にも適用可能．角藤さん，ありがとうございました．
     * 和文文字を \pdfstrcmp で比較する際には注意．内部漢字コードによって結果が変わる恐れがあります．
 * [eptex-100420.tar.bz2](eptex-100420.tar.bz2?raw=1)  
   [ptexlive-20100322](http://tutimura.ath.cx/ptexlive/) に対応した**試験版**．手っ取り早く作ったので，環境によってうまく動かないかもしれません．
   * 前バージョン（100201）からの更新部分：
     * インストール方法を変えた．e-pTeX の核心部分は前バージョンから変わっていないはず．
     * （こちらも開発環境が ptexlive ベースに移行するであろう）[upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) とのコンパイルは，upTeX 側の実装方針がまだわからなかったのでとりあえず本版では削除（早々と何とかしたい）．
     * [ptex-qtrip（**未変換**）](../ptex-qtrip/README.md)に同梱した pTeX の「bugfix」達も，ptex-qtrip をダウンロードすれば自動で当てます．
   * autoreconf を実行するので，GNU autoconf (>=2.63?), GNU automake (>=1.11?) が必要です．
     m4 のバージョンにも依存するかも（こちらの環境では GNU m4-1.4.14 です）．
   * 0eptex.sh, 6babel.sh に対するパッチ（次版に取り込み予定）：[eptex-100420-patch1.diff](eptex-100420-patch1.diff?raw=1) (2010/05/30)  
     eptex-100420.tar.bz2 の展開後に当ててください．非 Linux 環境への対応強化や，eptexdefs.lib のコピーし忘れ，6babel.sh での sed の置換ミス（Z.R.さん，ありがとうございます）への修正が行われます．
 * [eptex-100201.tar.bz2](eptex-100201.tar.bz2?raw=1)：
   * 前バージョン（100131）からの更新部分：  
     script/install での誤植で，e-upTeX の format が extended modeになっていなかったのを修正．（通りすがりさん，Dora さん，ありがとうございます）
   * コンパイルは以下の環境を考えています．[upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.htmll) と合わせてコンパイルすることが可能です（upTeX-0.29 でテスト）．
     * [ptexlive-20091009](http://tutimura.ath.cx/ptexlive/)：
       [ptl08-compile-100131.tar.lzma](ptl08-compile-100131.tar.lzma?raw=1) を用いる．
       このアーカイブ中の compile.txt を参照．upTeX も（任意に）同時コンパイルすることができます．
     * [ptetex3-20090610](http://tutimura.ath.cx/ptetex/) またはその上の upTeX-0.29：e-pTeX のアーカイブ内の INSTALL.txt を参照してください．

 * [eptex-100131.tar.bz2](eptex-100131.tar.bz2?raw=1)
   * 前バージョン（091003）からの更新部分：
     * upTeX-0.29 に対応したつもり（upTeX-0.29 内にある euptex-091003/ 以下をベースにしています）．
   * コンパイルは以下の環境を考えています．[upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) と合わせてコンパイルすることが可能です（upTeX-0.29 でテスト）．
     * [ptexlive-20091009](http://tutimura.ath.cx/ptexlive/)：
       [ptl08-compile-100131.tar.lzma](ptl08-compile-100131.tar.lzma?raw=1) を用いる．
       このアーカイブ中の compile.txt を参照．upTeX も（任意に）同時コンパイルすることができます．
     * [ptetex3-20090610](http://tutimura.ath.cx/ptetex/) またはその上の upTeX-0.29：e-pTeX のアーカイブ内の INSTALL.txt を参照してください．

 * [eptex-091003.tar.bz2](eptex-091003.tar.bz2?raw=1)：  
   下の eptex-090927 で見つかった typo を修正したものです（デビ丸さん，ありがとうございます）．
   コンパイル方法とかはまったく変わらないはずです．
   eptex-090927 をコンパイルできていれば本版を無理して入れる必要はありません．

 * [eptex-090927.tar.bz2](eptex-090927.tar.bz2?raw=1)：
   一応，変更点が多いので**ベータ版的扱い**ですが，今後の開発は本版をベースとするつもりなので，テストをお願いします．
   * 前バージョン（090309）からの更新部分：
     * 数式フォントの 16 個制限を緩和する「FAM256」パッチを標準で有効に．
     * 浮動小数点演算は，実装が中途半端だったことから，本版では削除してみた．
       （どうしても使いたい場合は，090309 版の fp.ch がそのまま使えるようです）
     * 「e-TeX ベースとした方が良い」とのアドバイスをもらったので，そのようにしてみた．動作自体は 090309 版と変わらないはず．
   * 多忙なのとリリースを急いだため，コンパイルは以下の環境しか今は考えていません．[upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) と合わせてコンパイルすることが可能です．（とりあえず upTeX-0.28 でテスト）
     * [ptexlive-20090904](http://tutimura.ath.cx/ptexlive/)：
       [ptl08-compile-100108.tar.lzma](ptl08-compile-100108.tar.lzma?raw=1) を用いる．
       このアーカイブ中の compile.txt を参照．upTeX も（任意に）同時コンパイルすることができます．
     * [ptetex3-20090610](http://tutimura.ath.cx/ptetex/) またはその上の upTeX-0.28：e-pTeX のアーカイブ内の INSTALL.txt を参照してください．
       upTeX-0.28 で提供されている e-pTeX の自動インストール機能は，本版では使えません．

 * [eptex-090309.tar.bz2](eptex-090309.tar.bz2?raw=1)
   * 090223 からの更新部分：
     * 090223 の方針を進めて，コンパイルスクリプトの整備．
     * texmfmem.h, ptexenc まわりを見直した．CFLAGSの「`-funsigned-char`」などが不要になり，eplatex.fmt 作成時に segfault するバグも直ったかも．
     * e-TeX の条件分岐拡張（`\ifcsname ... \endcsname`）の部分で漢字を考慮していなかった動作だったのを修正．
   * コンパイルの方法について：以下の TeX ディストリビューションの上でコンパイルできます．
     * [TeX Live 2008](http://www.tug.org/texlive/)：[tl08-compile-090707.tar.lzma](tl08-compile-090707.tar.lzma?raw=1) を用いる．詳細は [TeX Live 2008でのコンパイル（**未変換**）](../tl08/README.md) を参照．upTeX のコンパイルも同時に行います．（option 中で指定した $CMP_TEMP は bld1.sh 実行前に予め作っておいてください）
     * [ptexlive](http://tutimura.ath.cx/ptexlive/)
       * **ptexlive-20080715**：[tl07-compile-090819.tar.bz2](tl07-compile-090819.tar.bz2?raw=1) を用いる．詳細はこのファイル内の compile.txt を参照．upTeX も（任意に）同時コンパイルすることができます．
       * **ptexlive-20090807（以前，以後もOK？）**：[ptl08-compile-090818.tar.lzma](ptl08-compile-090818.tar.lzma?raw=1)　を用いる．upTeX のコンパイルも同時に行います．コンパイル方法はこのファイル内の compile.txt　を参照．**試験版です（あんまり動作確認してません）**．
     * [ptetex3](http://tutimura.ath.cx/ptetex/) またはその上の upTeX-0.26：e-pTeX のアーカイブ内の INSTALL.txt を参照してください．これだけインストールが手動です．upTeX-0.27 の側でも e-pTeX の自動インストールが可能になっているので，そちらを使ってもいいでしょう．

 * [eptex-090223.tar.bz2](/eptex-090223.tar.bz2?raw=1)
   * 090220 からの変更点：コンパイルスクリプトまわりを改変．upTeX-0.26 で確認しているが，それ以降でもそのまま通る可能性あり．
    * [TeX Live 2008](http://www.tug.org/texlive/) でのコンパイルには，[tl08-compile-090303.tar.bz2](tl08-compile-090303.tar.bz2?raw=1)を用いてください．
 * [eptex-090220.tar.bz2](eptex-090220.tar.bz2?raw=1)
   * 前バージョン（90218）からの更新部分：
     * バージョン番号を 6 桁に．（デビ丸さん，ありがとうございます）
     * （試験的な機能であり，標準では有効にしてコンパイルされませんが）数式フォントの 16 個制限を緩和．
    * [TeX Live 2008](http://www.tug.org/texlive/) でのコンパイルには，[tl08-compile-090305.tar.bz2](tl08-compile-090305.tar.bz2?raw=1)を用いてください．
      * このアーカイブ内の option の最後の 2 行には誤植がありますので，次のように直してください（tl-bsd fan さん，ありがとうございます）．
```
   gtar    --version > /dev/null 2>&1 && TAR=gtar
   gzcat   --version > /dev/null 2>&1 && ZCAT=gzcat
```
 * [eptex-90218.tar.bz2](eptex-90218.tar.bz2?raw=1)
    * 前バージョン（90111.20）からの更新部分：
     * 昔の更新によって trip test などの動作がおかしくなっていたことに対する修正（副作用として，TeX--XeT の出力がまた変になってるかもしれません）
     * e-pTeX, e-upTeX 本体の version もバイナリ起動時に出力するようにした．
   * [TeX Live 2008](http://www.tug.org/texlive/) でのコンパイルには，[tl08-compile-90218.tar.bz2](tl08-compile-90218.tar.bz2?raw=1)を用いてください．
 * [eptex-90111.20.tar.bz2](eptex-90111.20.tar.bz2?raw=1)
   * 前バージョン（80131.21）からの更新部分：インストールに関するドキュメントやスクリプトの更新．前バージョンを用いている場合は無理して更新する必要はありません．
   * [TeX Live 2008](http://www.tug.org/texlive/) でのコンパイルには，[tl08-compile-90124.14.tar.gz](tl08-compile-90124.14.tar.gz?raw=1) を用いてください．
   * **既知のバグ**: まだよく分からないがdiscretionary nodeまわりでtrip testが通らない．
 * [eptex-80131.21.tar.bz2](eptex-80131.21.tar.bz2?raw=1)
   * install方法については，中身のREADME.txtとかHOWTOINST.txtをお読みください． 
   * 前バージョン（80125.09）からの更新部分：TeX--XeTまわりの処理を変更．具体的にはdisplacement node （和欧文のベースライン補正とかに使われる）まわりを書き換えた．  
 * upTeX-0.21が出てるのに全く気づきませんでした．上のファイルたちを使ってupTeX-0.21とマージさせる場合には，uptex.diff中のu0.20をu0.21に書き換えればよいようです．（[upTeX on Debian](http://www1.pm.tokushima-u.ac.jp/~kohda/tex/uptex.html)というページにありました．ありがとうございます）
 * [eptex-80316.xx.tar.bz2](eptex-80316.xx.tar.bz2?raw=1)
   * 浮動小数点の演算部に[MPFR library](http://www.mpfr.org/)を用いた**試作品**である．浮動小数点数の形式も変更され，2進78桁（最上位の1は省略されて格納されるので実際には79bit）となったりしている．
   * e-TeX拡張については↑と変更はない．
 * **この 2 ファイルより前の物は**~~http://www.ms.u-tokyo.ac.jp/~kitagawa/ 旧ページ]に置いています．~~ **もうありません！**

## TeX Live 2014 の情報
TeX Live 2014 の e-pTeX で利用できる pdfTeX のプリミティブは以下のとおりです．
 * `\pdfstrcmp`: 文字列の比較．和文文字は UTF-8 バイト列に直して比較しています．
 * `\pdffilemoddate`, `\pdfcreationdate`, `\pdffilesize`: standalone パッケージを使う場合に必要． 
   TeX フォーラム中の [pLaTeX と standalone パッケージ（TikZとTexLive2012使用時）](http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=1024) が発端．
 * `\pdfsavepos`, `\pdfpage{width,height} `, `\pdflast{x,y}pos`: 出力結果 (dvi) における絶対値の取得．
 * `\pdffiledump`: ファイルの中身を 16 進でダンプ．`bmpsize` パッケージ等で使われる．（角藤さん，実装ありがとうございます）

## TeX Live 2015 の情報
TeX Live 2015 の e-pTeX では，上記に加えて以下の 2 プリミティブが使用可能です．
 * `\pdfshellescape`: pdfTeX の同名のプリミティブ．1: \write18 有効, 2: restricted \write18, 0: \write18 無効．
 * `\lastnodechar`: 直前のノードが文字（欧文のリガチャも含む）の場合，その文字コードを出力します．
   * [TeX & LaTeX Advent Calender 2014](http://www.adventar.org/calendars/553) 用に解説記事を本 Wiki 内の [lastnodechar](../lastnodechar/README.md) に書きました．(2014/12/15) 
上記 2 プリミティブについては TeX Live SVN r35615, r35619 としてコミットされました．角藤さん，ありがとうございます． 

また，2015/07/04 に，TeX Live SVN !r37756 として
 * `\pdfmdfivesum`: pdfTeX の同名のプリミティブ．`\pdfmdfivesum{...} ` で引数の MD5 sum を，
   `\pdfmdfivesum file {<filename>} `でファイル `<filename>` の中身の MD5 sum を計算．
の実装が行われました．これは XeTeX のメーリングリストの[投稿](http://tug.org/pipermail/xetex/2015-July/026044.html)以降の議論を元に，
角藤さんからリクエストのあったものです．e-pTeX の内部漢字コードで結果が変わるのは困るので，
内部漢字コードに関わらず，`\pdfmdfivesum{...} ` の引数中の和文文字は，UTF-8 だとして MD5 sum を計算しています．
