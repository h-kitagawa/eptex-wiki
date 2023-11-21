# e-pTeX について

このページは，「e-pTeX Wiki」(https://osdn.net/projects/eptex/wiki/FrontPage) を，osdn.net への接続が不安定であるために手動で移行したものである（もう Wiki ではない）．osdn.net の前は ~~http://www.ms.u-tokyo.ac.jp/~kitagawa/~~ （リンク切れ） で公開していた．

## 概要

現在の pTeX は，TeX の持っていたレジスタ 256 個という制限を未だに引きずっている．その一方，欧文の世界では，TeX Live ですでに e-TeX 機能が取り込まれた pdfTeX が標準となり，32768 個のレジスタを利用できるようになっている．

この状況を受け，僕は東京大学理学部数学科 3 年対象の授業「計算数学II」( ~~http://ks.ms.u-tokyo.ac.jp/~~ （リンク切れ）) 内の project として pTeX に e-TeX 機能を取り込むということを行った．skip レジスタを使用した 10 進 21 桁の浮動小数点演算も暇だったので新たに書き起こした（**注：浮動小数点演算の機能はしばらく前から取り外されています**）．これが本プロジェクトの e-pTeX である．

なお，2007/12/30 に角藤さんから同様の peTeX が発表された（[qa:50527](http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50527.html) 以降）が，本プログラムはそれとは独立に実装したものである（残念ながら peTeX は廃棄されたようである（[qa:50551](http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50551.html)））．

ライセンスについては，最初はあまり深く考えていなかったが，
[qa:50654](http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50654.htm) を契機に定めることにした．
（ pTeX や奥村さんの jsclasses が採用している）modified BSD を使うのが適当だと思ったので，そのようにした．

[TeX Live 2011 以降](http://www.tug.org/texlive/)や，[W32TeX](http://w32tex.org/index-ja.html) では，pTeX をインストールすると，e-pTeX も導入されるようになっています．
そのため，本ページでは，今後 **（1年に1回リリースされる）TeX Live に対する修正の情報を載せる**という形になります．

なお，最近は，[LuaTeX-ja プロジェクト](http://sourceforge.jp/projects/luatex-ja/wiki/FrontPage)の方を中心に取り組んでいます．

## 他ページへのリンク
 * [TeX Live 2021→2022 に伴う pTeX, e-pTeX の変更点](tl2022/README.md)
 * [TeX Live 2019→2020 に伴う pTeX, e-pTeX の変更点](tl2020/README.md)
 * [TeX Live 2017→2018 に伴う pTeX, e-pTeX の変更点](tl2018/README.md)
 *  [`\lastnodechar` プリミティブについて](lastnodechar/README.md)：e-pTeX 141109 以降に導入された`\lastnodechar` プリミティブについて
 *  [過去コメント](old_comments/README.md)：2018-04-01 以前の過去コメント（**移行中**）
 *  [旧版](old_versions/README.md)：過去のバージョンに関する情報
    *  [TeX Live 2008でのコンパイル](tl2008/README.md)
    *  [TeX Live 2011 への追加日本語パッチについて](tl2011/README.md)
    *  [`ptex-qtrip` について](ptex-qtrip/README.md)：テストソース`ptex-qtrip` についての説明
 *  [WEB 言語のサンプル](websample/README.md)
 
<!--
以下は TL11 以降の情報
 * eplatex というコマンドはなくなり，platex というコマンドで e-pTeX が起動されるようになりました（参考：[http://oku.edu.mie-u.ac.jp/~okumura/texfaq/qa/55944.html qa:55944]）．
 * '''TeX Live 2011 の iso に収録されているバージョンにはバグが存在します．'''以下のどれかを用いて更新することをお勧めします．
   * [http://tutimura.ath.cx/ptexlive/?tlptexlive%A5%EA%A5%DD%A5%B8%A5%C8%A5%EA tlptexlive リポジトリ] からアップデートする．
   * ↑の元になった [TeX_Live_2011] のページにある（やや古い）「追加の日本語関連パッチ」を使ってバイナリを作る．
   * TeX Live 2012（2012/7/8にリリースされました）では修正されています．
 * '''[http://sourceforge.jp/projects/eptex/wiki/FrontPage/attach/eptex-pdffilemoddate-130605.diff.xz eptex-pdffilemoddate-130605.diff.xz]''': pdfTeX で実装されている {{{\pdffilemoddate}}}, {{{\pdfcreationdate}}}, {{{\pdffilesize}}} を e-pTeX, e-upTeX に追加するパッチ．
   * TeX フォーラム中の [http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=1024 pLaTeX と standalone パッケージ（TikZとTexLive2012使用時）] が気になったので作成．
   * TeX Live SVN !r30819 で作業しています．パッチ適用後は ./reautoconf が必要です．TeX Live 2013 には間に合いませんでしたが，
     TeX Live SVN !r30897 で取り込まれたので TeX Live 2014 には入るでしょう．
   * 早速角藤さんが !W32TeX のバイナリを更新してくださいました．
   * パッチを適用すると，サンプル modtest/test1.tex が作られます（これでしかテストしていません）．
   * このパッチにより，e-pTeX のバージョンは '''130605''' となります．
-->

## 文書類
 * 「pTeX 系列の文字列化における和文欧文の区別」（[TeXConf 2019（中止）](https://texconf2019.tumblr.com/)，北川）
   * 使う予定だったスライドの [PDF](https://github.com/h-kitagawa/presentations/blob/main/tc19ptex.pdf?raw=1)
   * これが元になった TUGBoat の記事 (H. Kitagawa, "[Distinguishing 8-bit characters and Japanese characters in (u)pTeX](https://tug.org/TUGboat/tb41-3/tb129kitagawa-char.pdf)", TUGBoat 41:3, 329--334, 2020)
 * 「TeX Live 2016 の pTeX 系列のプリミティブ」（[TeXユーザの集い2016](https://texconf16.tumblr.com/)，北川）
   * スライドの [PDF](https://github.com/h-kitagawa/presentations/blob/main/tc16ptex.pdf?raw=1)
 * [resume.pdf](resume.pdf?raw=1)：2008/1/31 までの「計算数学II」での作業記録であり，
   [eptex-100131.tar.bz2](old_versions/eptex-100131.tar.bz2?raw=1) 中にも入っている．内輪ネタも入っているが，その一方で，実装方法とかの若干詳しい情報も書いてある．
   [texjporg/ptex-manual](https://github.com/texjporg/ptex-manual) では eptex_resume.pdf という名前になっている．
 
