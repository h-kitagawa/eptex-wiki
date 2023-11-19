# ptex-qtrip について

このページはまだ書きかけです．

## 概要
pTeX とそこから派生した [e-pTeX](../README.md), [upTeX](http://www.t-lab.opal.ne.jp/tex/uptex.html) との動作が
どれほど異なるかを検査するために作ったテストソースです．具体的には，pTeX で拡張された部分を全て実行することを目標としたのですが，
まだ完全ではありません．**北川個人が作ったものであり，非公式です．**

## ダウンロード ==
~~本ページ上のメニュー「ダウンロード」よりどうぞ．~~
[ptex-qtrip-110227.tar.gz](../old_versions/ptex-qtrip-110227.tar.gz?raw=1) から長い間放置しています．

## 使い方
展開後，
```
 $ ptex -ini -kanji=euc qtrip
 $ ptex -fmt=qtrip -kanji=euc qtrip
```

としてください（本家 TRIP test と同様に，2段階での実行です）．
2回目の実行で，ログが大量に吐かれますので，e-pTeX, upTeX などでも同じことを行い，
ログを比較してみてください．

## 注意 ==
このテストソースは tetex-src-3.0 上で（ptexenc 未適用の）pTeX-3.1.11 を使って製作しています．そのため，qtrip,tex 本体の漢字コードは EUC で，ドキュメント類も EUC で作ってます．

2回目の実行の最後に，
```
Completed. )
(see the transcript file for additional information)
```
などと出ずに， 無限ループに陥ったり Segmentation Fault が発生する可能性があります．
その場合，同梱している次のパッチを ptex-base.ch に適用すれば解決するかもしれません：
   *  ptex-base.ch.0308.diff：
        [http://oku.edu.mie-u.ac.jp/tex/mod/forum/discuss.php?d=407#p2159] にある角藤さんによるパッチです．
        ptexlive にある archive/ptex-src-3.1.11-box.patch はこれのサブセットのようです．
   *  ptex-base.ch.0321.diff
   *  ptex-base.ch.0324.diff
   *  ptex-base.ch.0330b.diff

なお，以上4つのパッチは角藤版 W32TeX や TeX Live 2010 では取り込まれているようです．

## 変更履歴
 * [ptex-qtrip-100710 ](../old_versions/ptex-qtrip-100710.tar.gz?raw=1) では，[qa:55068](http://oku.edu.mie-u.ac.jp/~okumura/texfaq/qa/55068.html) 以降のスレッドに関連した部分を追加しました．
   自作の pTeX へのパッチ (ptex-base.ch.0709.diff) も入れてあります．
   いつものように，思わぬ副作用とかがあるかもしれません．
 * [ptex-qtrip-100816](../old_versions/ptex-qtrip-100816.tar.gz?raw=1) では，upTeX での実行も考慮するようにしました．  
   `--kanji-internal=uptex` の場合に，読み込むフォントを upTeX 用のものにしてあります．  
   また，次のパッチを作りました：
   * qa:55068 への対応パッチを更新（ptex-base.ch.0711.diff）
   * `\accent\char\euc'130244` のような時の動作がおかしくなるバグを修正（ptex-base.ch.0814.diff）[[BR]]
     なお，ptexenc 適用時（ptexlive 下でコンパイルするときなど）や，upTeX にはこのパッチはそのままではあたりません．

## コメント欄 ==
  * ptex-qtrip-100710 内の ptex-base.ch.0709.diff とe-pTeX 100420 が干渉しないかどうかは未確認なので，e-pTeX をコンパイルするときは ptex-qtrip-100402.11 が安全です．qa:55068 関連が（自分の中で）落ち着いたら，また更新します．
    -- h7k (2010-07-11 18:02:01 JST)
    * ptex-base.ch.0711.diff ではたぶん大丈夫だと思います．（2010-08-16）
