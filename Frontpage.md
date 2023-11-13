= e-pTeX についての wiki =

元々 ~~!http://www.ms.u-tokyo.ac.jp/~kitagawa/~~（リンク切れ） で公開していたものであるが，諸事情によってこの ~~sourceforge.jp~~ osdn.net（サイト名称変更）内の wiki に一本化した．

== 概要 ==

現在の pTeX は，TeX の持っていたレジスタ 256 個という制限を未だに引きずっている．その一方，欧文の世界では，TeX Live ですでに e-TeX 機能が取り込まれた pdfTeX が標準となり，32768 個のレジスタを利用できるようになっている．

この状況を受け，僕は東京大学理学部数学科 3 年対象の授業「計算数学II」( http://ks.ms.u-tokyo.ac.jp/ ) 内の project として pTeX に e-TeX 機能を取り込むということを行った．skip レジスタを使用した 10 進 21 桁の浮動小数点演算も暇だったので新たに書き起こした（'''注：浮動小数点演算の機能はしばらく前から取り外されています'''）．これが本プロジェクトの e-pTeX である．

なお，2007/12/30 に角藤さんから同様の peTeX が発表された（[http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50527.html qa:50527] 以降）が，本プログラムはそれとは独立に実装したものである（残念ながら peTeX は廃棄されたようである（[http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50551.html qa:50551]））．

ライセンスについては，最初はあまり深く考えていなかったが，
[http://oku.edu.mie-u.ac.jp/%7Eokumura/texfaq/qa/50654.html qa:50654] を契機に定めることにした．
（ pTeX や奥村さんの jsclasses が採用している）modified BSD を使うのが適当だと思ったので，そのようにした．

[http://www.tug.org/texlive/ TeX Live 2011 以降]や，[http://w32tex.org/index-ja.html W32TeX] では，pTeX をインストールすると，e-pTeX も導入されるようになっています．
そのため，本ページでは，今後'''（1年に1回リリースされる）TeX Live に対する修正の情報を載せる'''という形になります．

なお，最近は，[http://sourceforge.jp/projects/luatex-ja/wiki/FrontPage LuaTeX-ja] プロジェクトの方を中心に取り組んでいます．

 * TeX Live 2021→2022 に伴う pTeX, e-pTeX の変更点については [tl2022 こちら] を参照して下さい．
 * TeX Live 2019→2020 に伴う pTeX, e-pTeX の変更点については [tl2020 こちら] を参照して下さい．
 * TeX Live 2017→2018 に伴う pTeX, e-pTeX の変更点については [tl2018 こちら] を参照して下さい．

{{{ comment
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
```

== その他の情報 ==
 * 以前の版は，[old_versions 旧版のページ]からたどれます．
 * 文書類：
   * 「pTeX 系列の文字列化における和文欧文の区別」（~~[https://texconf2019.tumblr.com/ TeXConf 2019]~~中止，北川）
     * 使う予定だったスライドの [https://osdn.net/projects/eptex/docs/tc19ptex/ja/1/tc19ptex.pdf PDF]
   * 「TeX Live 2016 の pTeX 系列のプリミティブ」（[https://texconf16.tumblr.com/ TeXユーザの集い2016]，北川）
     * スライドの [https://osdn.net/projects/eptex/docs/tc16ptex/ja/1/tc16ptex.pdf PDF]（11/08 20:30 リンク先更新．中身は同じ）
   * [https://sourceforge.jp/projects/eptex/document/resume/ja/1/resume.pdf resume.pdf]：2008/1/31 までの「計算数学II」での作業記録であり，
     eptex-100131.tar.bz2 中にも入っている．内輪ネタも入っているが，その一方で，実装方法とかの若干詳しい情報も書いてある．[https://github.com/texjporg/ptex-manual texjporg/ptex-manual] では eptex_resume.pdf という名前になっている．
   * 以前のページに載せていた WEB のサンプルはこちら：[websample WEB のサンプル]
 
== コメント ==
長くなってきたので，2018-04-01 以前のコメントは[wiki:過去コメント 過去コメント]の前半部に移しました．

コメントは下のボックスに遠慮なくお書きください．
また，バグ報告・機能リクエストなどは[http://sourceforge.jp/projects/eptex/ticket/ こちら]へどうぞ（コメント欄だと管理しきれなくなる）．

[[LineComment(direction=below, rows=3)]]

