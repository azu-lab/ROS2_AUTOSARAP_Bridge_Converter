#!/bin/bash

# 最初のビルドを実行し、出力を/dev/nullにリダイレクトして非表示にする
colcon build  > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "Build for succeeded"
    exit 0
fi

# ファイルをコピー
cp -r ros -r build/gnss_someip_lib/
if [ $? -ne 0 ]; then
    echo "Failed to copy files"
    exit 1
fi

# 再度ビルド
colcon build
if [ $? -ne 0 ]; then
    echo "Final build failed"
    exit 1
fi

echo "Build completed successfully"

