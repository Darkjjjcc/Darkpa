#!/bin/bash

# 1. 定义文件所在的文件夹
FILE_DIR="./tests"  # 文件所在的文件夹

# 2. 定义 Makefile 所在的文件夹
MAKE_DIR=""  # Makefile 所在的目录

# 3. 遍历文件夹中的所有文件
for file in "$FILE_DIR"/*; do
    if [[ -f $file ]]; then
        echo "Processing $file ..."
        
        # 4. 切换到 Makefile 所在的文件夹
        pushd "$MAKE_DIR" > /dev/null
        
        # 5. 执行 make run，并将文件的路径传递给 Makefile
        make run FILE="$file"
        
        # 6. 检查 make run 的结果
        if [[ $? -ne 0 ]]; then
            echo "Make run failed for $file"
            popd > /dev/null
            exit 1
        fi
        
        # 7. 返回原来的工作目录
        popd > /dev/null
    fi
done

echo "All files processed successfully."
