# WEB 言語のサンプル

元々は2007年度「計算数学II」の発表会に利用しようと思ってために書いたものであるが，結局発表には使わなかった．
内容は2007年度夏学期の計算数学Iのレポートに使ったFortranのプログラムをWEBに書き直したものである．しかし，日本語を使ったので本来のtangle, weaveでは通らず，松山さんの開発された「日本語WEB」(http://www.matsuand.com/web/index.html ([2006-03-26 時点の Internet Archive](https://web.archive.org/web/20060327073415/http://www.matsuand.com/web/index.html)))を使用する．詳細は [resume.pdf](../resume.pdf?raw=1) へ．

   * [ks1.web](ks1.web?raw=1)：もともとのWEBソース．
   * [ks1.p](ks1.p?raw=1)：ks1.webをtangleにかけてできたPascalソース．
   * [ks1.tex](ks1.tex?raw=1)：ks1.webをweaveにかけてできた，ドキュメントのTeXソース．
   * [ks1.h](ks1.h?raw=1), [ks1.c](ks1.c?raw=1), [ks1-a.h](ks1-a.h?raw=1)：Web2Cにks1.pをかけてCソースに変換したもの．ks1-a.hはそのために自分で書き起こした最小限のヘッダファイルである． 
