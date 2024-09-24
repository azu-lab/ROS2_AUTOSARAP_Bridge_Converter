#!/bin/bash

# routingmanager プロセスのPIDを取得
PIDS=$(ps aux | grep 'routingmanagerd' | grep -v 'grep' | awk '{print $2}')

# 各PIDに対してkillコマンドを実行
for PID in $PIDS
do
    echo "Killing routingmanagerd with PID $PID"
    kill -9 $PID
    # 強制終了が必要な場合は以下を使用
    # kill -9 $PID
done

echo "All routingmanagerd processes have been killed."
