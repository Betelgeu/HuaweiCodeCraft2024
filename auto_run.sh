# 创建一个空文件用于存储最后一行输出
> output.txt

# 指定包含`.txt`文件的文件夹路径
folder_path="./maps"

# 循环遍历指定文件夹中的所有.txt文件
for map_file in "$folder_path"/*.txt
do
    echo ${map_file}
    map_filename=$(basename "$map_file")
    result=$(./PreliminaryJudge -d "input/${map_filename}_input.txt" "../arch/main" -m "$map_file" | tail -n 3)
    echo "Map $map_filename: $result" >> output.txt
done
