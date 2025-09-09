#!/bin/bash

# 2のべき乗でディレクトリを作成する最大値
MAX_POWER=8192

# 現在の値
current=1

# MAX_POWER に到達するまでループ
while [ "$current" -le "$MAX_POWER" ]; do
    # ディレクトリ作成
    mkdir "evaluation_$current"
    
    # 2のべき乗を計算
    current=$(( current * 2 ))
done
