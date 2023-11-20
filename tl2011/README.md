# TeX Live 2011 への追加日本語パッチについて

TeX forum における[スレッド](http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=675)で，
TeX Live 2011 への追加の日本語関連パッチを投稿しましたが，e-pTeX 関連にも修正を加えることが必要になったので，
以降はここで公開していくことにしました．旧版は削除．
ここで公開されているスクリプトを用いると，e-pTeX のバージョンは次のように変更されます．
```
This is e-pTeX, Version 3.1415926-p3.2-110825-2.3 (utf8.euc) (TeX Live 2011)
```

~~[tlptexlive リポジトリ](http://tutimura.ath.cx/ptexlive/?tlptexlive%A5%EA%A5%DD%A5%B8%A5%C8%A5%EA)では，
本スクリプトによってコンパイルされたバイナリを用いています．~~ 独自のスクリプトに置き換わりました (2012/1/7)．
**tlptexlive では upTeX もパッケージングされているので，十分なディスク容量等がある場合は，そちらを使うほうをお勧めします．**

 * [tl11supp-111108-b3.tar.xz](tl11supp-111108-b3.tar.xz?raw=1)
   * tl11supp-20111105 の pxdvi の改変をさらに進め，pxdvi.cfg の "replace" entry でも `/AJ16` 表記を可能とした．
   * 上流に japanese, otf 両パッケージが取り込まれたことにより，これらのパッケージの展開を自前で行わない `-o` オプションを新設．
   * updmap.cfg の値を `pxdviUse true` に変更．pxdvi 用フォントマップは xdvi-ptex.map（ヘッダ以外は kanjix.map のコピー）です．
 * [tl11supp-111120.tar.xz](tl11supp-111120.tar.xz?raw=1)
   * **バイナリは -111108-b3 によるものから変更はありません．**
   * dvipdfmx 用マップファイルを変更（角藤さん，ありがとうございます）．
   * updmap を上流の変更（r24602）に追従．
   * コメント欄にある munepi さんによる tl11/common.sh へのパッチをとりこみました．
 * [tl11supp-111214.tar.xz](tl11supp-111214.tar.xz?raw=1)
   * **バイナリは pxdvi(-xaw or -motif) のみ変わっています．**
   * pxdvi のみを upTeX 対応に，また「Fonts Darker」等による濃さの調整を和文文字にも効くようにしました．
   * コメント欄にある munepi さんによる texmf/XDvi へのパッチをとりこみました．
   * 上流の SVN branch の変更 (r24766) を取り込みました．
 * [tl11supp-120120.tar.xz](tl11supp-120120.tar.xz?raw=1)
   * pMetaPost が Mac OS X 下で落ちてしまう問題に対応しました（Mac は個人所有していないので十分にテストできていないですが）．
   * TeX Live SVN に取り込まれた，```\catcode`(和文文字)=...``` に関する修正をパックポートしました．次が参考ページです：
     *  [check_kanji() と \kcatcode の範囲チェック](http://tutimura.ath.cx/ptetex/?check_kanji%A4%C8kcatcode%A4%CE%C8%CF%B0%CF%A5%C1%A5%A7%A5%C3%A5%AF) (in ptetex wiki)
     * [`\catcode＜漢字＞`について](http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=544)．
     * `\catcode` に和文文字コードを与えた場合，実際には上位バイトの中身 (in \[0, 255]) だと解釈されます．

## コメント欄 ==
  * Thank you, Norbert-san. I overlooked this page for few days.  
    > is there something I should include in the build script?  
    No, there isn't. The changes in t11supp-120120 are pmpost changes, and a fix of pTeX (I backported this from TL SVN sources). So the current build-tlptexlive package is fine.  
      -- h7k (2012-04-17 14:29:55 JST)
  * One more question: Your changes in t11supp-120120, is there something I should include in the build script? I guess the pmetapost changes only  
    -- Norbert (2012-04-15 23:17:55 JST)
  * 北川さん：I have updated the patch for pxdvi and pmpost (the pmpost.ch file needs changes) for current TL source. They are now in the build-tlptexlive packages. No need for you for now I guess, since your scripts are based on the 2011 source release. But I wanted to inform you, that there is no need to redo the patches later on.  
    -- Norbert (2012-04-15 23:11:01 JST)
  * H.Kagotani  さん，報告ありがとうございます．確認しました．PostScript はよくわからないのですが，こちらの環境では，[このパッチ](pxdvik-20120330-scalefont.diff?raw=1) で，とりあえず警告は出なくなりました．  
    -- h7k (2012-03-30 21:52:40 JST)
  * `\documentclass{article}\usepackage{hyperref}\begin{document}X\end{document}`
  というコードをplatexでコンパイルしてpxdviで表示すると、
  `pxdvi-xaw: cannot get scaling factors.`  
  という警告が頻繁に出ます。これは hyperref が scalefont を利用するのですが、pxdvi が strstr(PostScript_cmd, "scale") というコードでマッチさせて解析するためだと思われます。  
    緊急性はまったくないですが、よろしくお願いします。  
    -- H.Kagotani (2012-03-29 12:43:17 JST)
  * ↓のコメントは私の勘違いでした（vf の生成を間違えてました）^^;  
    -- h7k (2011-12-17 21:43:19 JST)
  * （upTeX 用に otf パッケージ用の tfm, vf をしかるべき方法で作った上で）otftest 内の brsgtest.tex を upTeX 用に手直ししたものを処理すると，「□あ・□」の中黒と□の間の空白が半角分大きくなってしまうという症状がおこります．  
      upTeX で作った dvi は内部コードが unicode になるということが何か関係しているのかもしれませんが，原因は調査中です．
      -- h7k (2011-12-17 20:41:58 JST)
  * texmf/XDvi へのパッチです。他の XLFD の設定から、helvetica がぬかれているので、これも抜いておくほうがよさそうです。本件は、細かすぎる修正だと思うので、何かのついでがあれば程度で、ご検討くださいませ。
```
--- texlive-20110705-source/texk/pxdvik/texmf/XDvi.vine	2011-11-27 00:47:49.000000000 +0900
+++ texlive-20110705-source/texk/pxdvik/texmf/XDvi	2011-11-27 00:52:09.994634231 +0900
@@ -158,11 +158,11 @@ xdviSelFile*international: false
 !!! TOPIC_HEADING: Heading for help text in help window
 !!!
 *fontList: \
    - *-helvetica-medium-r-*-*-12-*-*-*-*-*-*-*=NORMAL,\
    - *-helvetica-medium-r-*-*-12-*-*-*-*-*-*-*=UNMARKED,\
    - *-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*=MARKED,\
    - *-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*=TOPIC_LABEL,\
    - *-helvetica-bold-r-*-*-14-*-*-*-*-*-*-*=TOPIC_HEADING
+-*-*-medium-r-*-*-12-*-*-*-*-*-*-*=NORMAL,\
+-*-*-medium-r-*-*-12-*-*-*-*-*-*-*=UNMARKED,\
+-*-*-bold-r-*-*-12-*-*-*-*-*-*-*=MARKED,\
+-*-*-bold-r-*-*-12-*-*-*-*-*-*-*=TOPIC_LABEL,\
+-*-*-bold-r-*-*-14-*-*-*-*-*-*-*=TOPIC_HEADING
 
 
 !!! Color used for page border, image bounding boxes and the ruler in `Ruler mode'.

Diff finished.  Sun Nov 27 00:52:15 2011
```
    -- munepi  (2011-11-27 01:40:40 JST)
  * tl11/common.sh の終了時の返り値を 0 にしておいた方が嬉しいです。と申しますのも、パッケージをビルドするときに、1 が返ってきたら、その瞬間に処理が終わってしまいます(^^  
   最下行に `exit 0` を入れるか、以下のようなパッチを適応するかで対応可能と思います。ご一考下さい。
```
        - tl11/common.sh.vine	2011-11-08 15:56:41.000000000 +0900
+++ tl11/common.sh	2011-11-14 02:26:45.901096758 +0900
@@ -106,5 +126,5 @@ done
 
 MAKE=make
 PATCH=patch
- gmake   --version > /dev/null 2>&1 && MAKE=gmake
- gpatch  --version > /dev/null 2>&1 && PATCH=gpatch
+gmake   --version > /dev/null 2>&1 && MAKE=gmake ||:
+gpatch  --version > /dev/null 2>&1 && PATCH=gpatch ||:

```
    -- munepi (2011-11-15 23:33:40 JST)
  * misho さん，報告ありがとうございます．scripts/fontlink.sh の文字コードについては，20111108(-b3)版で修正したはずです（こちらでも，Mac 上でテストしたとき，同じことに気づきました）．ここ数週間の更新で，TeX Live 本体に IPAex フォントが入ったので，fontconfig を強制的に off にするオプションを作るのもありかもしれませんね．XeTeX は……普段 make しないので分かりません^^;
    -- h7k (2011-11-09 16:18:35 JST)
  * Debian は普通に入ったのですが，20111023版を MacOSX 10.6 に入れたとき，いくつか修正する必要がありました。それ以降の変更で既に obsolete になっているかも知れませんが，一応comment しておきます。http://www.misho-web.com/phys/computing.html#texlive_macosx  
  -- misho misho  (2011-11-09 13:20:52 JST)
  * ptexlive-maps-20111029.tar.xz 中の otf-ipaex.map にバグを入れてしまいました，すみません．代わりに ptexlive-maps-20111102.tar.xz を用いてください．  
    -- h7k (2011-11-02 16:37:49 JST)
  * Norbert さんに教えられたのですが，他にも TeX Live 2011 用に適用しないといけないパッチ (r23716, r23740, r24159, r24346) があるようです．できるだけ早く仕事します（今日中にできればいいなあ）．  
    -- h7k (2011-10-22 14:24:52 JST)
  * ほんとだ，書き込む時に勘違いしていたようです（手元の script ではちゃんと texmf/...）になってます．  
    -- h7k  (2011-08-19 22:59:57 JST)
  * タイポだと思いますが，h7k (2011-07-26 06:13:40 JST) の記事で updmap-????????-tl11 のコピー先は “texmf-dist/..." ではなく texmf/scripts/tetex/updmap.pl とすべきと思います．これで日本語も問題ないことと思います．  
    -- takahashi (2011-08-19 11:39:45 JST)
  * OSX Lion上でdvipdfmxがAbortを出しやすくなったことを報告したものです。その後調べた結果、このことはLion上でXcode 4.1.1を使って，texliveをbuildした場合に起こることで、[Live 2011 への追加日本語パッチが原因ではないことがわかりました。
  お騒がせして申し訳ありません。texlive-20110705-source.tar.xzを`./Build --disable-xdv2pdf --disable-xdvipdfmx  --disable-xetex`としてbuildし、できたdvipdfmxで同じようにAbortが発生することを確認しました。  
    -- anonymous (2011-08-05 21:04:39 JST)
  * 日本語対応のupdmap、私の配布物（MacTeX2011への追加モジュール）にも流用させていただきました。  
    -- H.Ogawa (2011-08-01 22:24:28 JST)
  * /usr/local/texlive/2011/bin/x86_64-darwin/dvipdfmxをインストールして試しましたが、うまくタイプセットできました。sizeは868156です。  
    -- anonymous (2011-08-01 21:13:23 JST)
  * sizeが違うのは、universalとintelの違いのせいかもしれません。  
    -- anonymous (2011-08-01 20:23:02 JST)
  * versionはどちらもdvipdfm-0.13.2cでしたが、サイズがかなり違いました。また、p2011のほうは、なぜか所有者がlocal userになっています。
```
% ll /usr/local/texlive/2011/bin/universal-darwin/dvipdfmx
-rwxr-xr-x  1 root  wheel  1563484  6 22 07:11 /usr/local/texlive/2011/bin/universal-darwin/dvipdfmx
% ll /usr/local/texlive/p2011/bin/x86_64-apple-darwin11.0.0/dvipdfmx
-rwxr-xr-x  1 huge  wheel  854272  8  1 19:56 /usr/local/texlive/p2011/bin/x86_64-apple-darwin11.0.0/dvipdfmx
```
    -- anonymous (2011-08-01 20:21:38 JST)
  * \dtouを外してみましたが、同じようにp2011のdvipdfmxでのみAbortが出てpdfが作れませんでした。Abortが出るのはbeamerのファイルで、写真などを含めて25Mぐらいあるので、検証用にお送りするのが難しいかもしれません。もう少し小さなファイルにしたときに出ないか調べてみます。  
    -- anonymous (2011-08-01 20:11:24 JST)
  * とりあえず，ほとんど誰の約にも立っていないであろう \dtou パッチを外してみる（scripts/unpack.sh の 25行目の「zpatch ...」の行をコメントアウトする）とどうなるでしょうか？ （dvipdfmx にあてているパッチはこれだけのはず）
  あと，再現してくれるソースがあれば，こちらでも検証できるので嬉しいです．
    -- h7k (2011-08-01 06:00:51 JST)
  * 2011のdvipdfmxでは大丈夫なのですが、p2011のdvipdfmxでAbortが出る頻度が高くなりました。環境はOSX Lionです。はっきりしない報告ですみません。  
    -- anonymous (2011-07-31 22:32:08 JST)
  * ありがとうございます。よろしくお願いいたします。  
    -- H.Ogawa (2011-07-27 11:49:08 JST)
  * 了解しました．数日中に更新します．>角藤さんが作成してくださったdvipsのパッチの取り込み
    \# リンクは直しておきました．
    -- h7k  (2011-07-27 07:09:57 JST)
  * すいません、リンクが変になってしまいました。  
    -- H.Ogawa (2011-07-27 01:56:22 JST)
  * [こちらのスレッド](http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=678)の角藤さんが作成してくださったdvipsのパッチの取り込みもご検討いただけないでしょうか？  
    -- H.Ogawa (2011-07-27 01:55:22 JST)
  * ご提示いただいた修正で、こちらでも正常に動作しました。ありがとうございます。  
    -- H.Ogawa (2011-07-26 19:25:12 JST)
  * 報告ありがとうございます．
  > updmapが生成するkanjix.mapの書式が、dvipsのものになってしまう
   
   たまたま使える Mac を探し出せたのでビルドしてみたら，全く同じ症状にぶち当たりました．
   とりあえず updmap-????????-tl11 の 1031 行目〜 1035 行目ぐらいまでを
```
push @tmpkanji1, &getLines(@tmpkanji0);
&writeLines(">$dvipdfmoutputdir/kanjix.map",
            @tmpkanji1);
@tmpkanji1 = &normalizeLines(@tmpkanji1);
my @tmpkanji2 = &cidx2dvips(\@tmpkanji1);
```
   のように行を入れ替えたりしていろいろいじっていたら，いつの間にか直っていました．よくわからないですが……．  
     -- h7k (2011-07-26 17:38:19 JST)
  * updmapが生成するkanjix.mapの書式が、「フォント名　CMap　実フォント」のdvipdfmxのものではなく「フォント名　実フォント-CMap」のdvipsのものになってしまうようです。  
    -- H.Ogawa (2011-07-26 16:40:44 JST)
  * 必要最小限のみをビルドした状況で再インストールしたところ有効になりました．原因は分かりませんが，書き換えに失敗したまま，インストールしてしまったようです．
    -- htfs (2011-07-26 11:42:51 JST)
  * 自由にいじれる Mac を持ってないこともあり，よくわからないです．updmap-????????-tl11 が（書き換えた後の）本体なので，texmf-dist/scripts/tetex/updmap.pl としてコピーすればいいように思いますが……．  
    -- h7k (2011-07-26 06:13:40 JST)
  * フォント設定を行うと"updmap: Unsupported option kanjiEmbed"となりますので，パッチが有効になっていないと思われます．
    -- htfs (2011-07-25 23:20:47 JST)
