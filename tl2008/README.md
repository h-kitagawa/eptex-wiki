# TeX Live 2008でのコンパイル
## 注意事項
 * **以下の記述やファイル内容には間違いがある可能性があります．十分注意してください．**
 * このページに書かれたのと異なるバージョンのソースファイルを使った場合，うまくいかない可能性が大きいです．
 
## コンパイルにあたり ==
 * TeX Live 2008 は $TL 以下にインストールされているものと仮定． 
 * 日本語化 dvips のコマンド名は `pdvips`，日本語化した xdvi のコマンド名は `xdvi` です．ファイル名が dvips のバイナリはできません．
 * その他にも，いくつかのバイナリの名前が異なります（`ppltotf`, `ptftopl`, `pbibtex`, `upbibtex` など）．
 * jmpost, updvi2tty はコンパイルされません．
 * スクリプト中でのエラー処理はまともに行っていません．
 
 以下，
```
[ ]$
```
   で通常ユーザでのプロンプトを， 
```
< >#
```
   でrootでのプロンプトを表す．
 
 ## 必要なファイル
以下のファイルは，最後を除いて全部 $SRCにあると仮定します．バージョンが違うとうまくいかない可能性が大きいです．
 * **lzma 圧縮のファイルを展開するため，unlzma コマンドにパスが通っている状態にしておくこと）**
 * [texlive-20080816-source.tar.lzma](http://www.ctan.org/pub/tex-archive/systems/texlive/Source/texlive-20080816-source.tar.lzma) （TeX Live 2008のDVDイメージ中にもある．）
 * [eptex-090309.tar.bz2](../old_versions/eptex-090309.tar.bz2?raw=1)
 * ptexlive-20090705.tar.gz （土村さんのサイト [ptexlive Wiki](http://tutimura.ath.cx/ptexlive/)より）
 * uptex-0.27.tar.gz, uptex_font-0.22.tar.gz（ttk さんのサイト [TeX memo - upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) から）
 * [tl08-compile-090707.tar.lzma](../old_versions/tl08-compile-090707.tar.lzma?raw=1)
 * [uptex-0.27-qa53556-tl08c.patch](../uptex-0.27-qa53556-tl08c.patch?raw=1)

## コンパイル ==
 * tl08-compile-090707.tar.lzma を $SRC で解凍．
 * $SRC/tl08-compile/patches/ に uptex-0.27-qa53556-tl08c.patch を配置．
 * $SRC/tl08-compile/option を環境に合うように編集． 
   **特に $TL, $SRC, $CFLAGS は必ず正しい値に設定してください**．
   $CFLAGS がよくわからないという人は，
```
CFLAGS="-O2 -s "
```
   ぐらいにでもしとくといいかもしれません．
 * 実際の作業に入ります：
```
   [ ]$ cd $SRC/tl08-compile
   [ ]$ ./bld1.sh
   [ ]$ ./bld2.sh
   [ ]$ su
   < ># ./bld3.sh
```
   `bld1.sh` で build 前のパッチ当てと build 作業を，`bld2.sh` で各種パッケージなどの設定，そして最後の `bld3.sh` でインストールとフォント設定（の一部）を行います．
 * `./bld3.sh` 実行後のメッセージに従い，次を行う．（**自動ではやってくれません**）
   * $PATH の設定
   * Truetype / Opentype フォント，CMap の symbolic link （`bld3.sh` の実行後のメッセージに書かれているディレクトリ名は例であって，実際にそのディレクトリが存在しているかなどのチェックはしていません）
   * （自由選択）韓国語，中国語 Truetype fontのダウンロード（実はここは map file も含めて間違ったままになってます）[[BR]]
     もともとこれらがシステムにインストールされていて，すぐ上の作業で symbolic link したのならば，
     わざわざもう一回ダウンロードする必要はない．
      * 中国語繁体字: bsmi00lp.ttf, bkai00mp.ttf (Arphic)
      * 中国語簡体字: gbsn00lp.ttf, gkai00mp.ttf (Arphic)
      * 韓国語      : UnBatang.ttf, UnDotum.ttf  (un-fonts)
   * 再び mktexlsr
 * 試験的な「FAM256 パッチ」を使うには，bld1.sh 実行前に，option の
```
   FAM256=0
```
   という行を
```
   FAM256=1
```
   と書き換えておいて，上に述べた普通の手順を行ってください．
   また．`bld1.sh` 実行中に，
```
*** Make FAM256 enabled binaries of e-pTeX and e-upTeX ***
         Return key to continue, or Ctrl+C to abort
```
   と出て一時停止します．Return キーで続行すると，FAM256 パッチが適用なバイナリが生成されます．
   ここで中断すると FAM256 が無効なバイナリが得られます．

## コメント欄 ==
2009/5/24以前のコメントは[過去コメント](../old_comments/README.md)の後半部に移しました．

  * ptexlive-20090705 は ptexenc の都合によりうまく通りません．  
    -- h7k (2009-07-07 19:55:00 JST)
    * [tl08-compile-090707.tar.lzma](../old_versions/tl08-compile-090707.tar.lzma?raw=1) に置いてみたが，あんまりテストしてない（本来ならスクリプトを ptexlive ベースにする方が先だと思うが）  
      -- h7k (2009-07-07 22:29:10 JST)
  * すみません．拡張子を.lzmaに変えたのを忘れていました．  
  -- h7k  (2009-07-01 14:59:06 JST)
  * tl08-compile-090630.tar.bz2 がリンク切れになっているようです。  
    -- Oyayubi (2009-07-01 14:22:23 JST)
