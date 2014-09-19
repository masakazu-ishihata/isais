# Suffix Array

[Suffix Array][sa] を線形時間で構築するアルゴリズムである [SA-IS][sais] の実装。   
[このページ][Shogo] を参考に実装しました。  
ぶっちゃけなぜこれでうまくいくのかは正しく理解しておりせん…。

# 実装

    isais *isais_new(ui n, ui *s);

長さ n の配列 s の Suffix Array を構築する。

    int *isais_match(isais *a, ui m, ui *q);

クエリ q が配列 s 中に表れる最初の場所を返す。
出現しない場合は -1 を返す。


[sa]: http://ja.wikipedia.org/wiki/%E6%8E%A5%E5%B0%BE%E8%BE%9E%E9%85%8D%E5%88%97 "接尾辞配列 - Wikipedia"
[sais]: http://ieeexplore.ieee.org/ielx5/4976434/4976435/04976463.pdf?tp=&arnumber=4976463&isnumber=4976435 "SA-IS"
[Shogo]: http://shogo82148.github.io/homepage/memo/algorithm/suffix-array/ "接尾辞配列(Suffix Array) - Shogo Computing Laboratory"
