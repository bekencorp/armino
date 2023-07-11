1、在 partTableTool/bk7256Partitions.csv中填写好对应的分区信息
2、shell（linux）或cmd（windows）进入目录 partTableTool
3、输入命令：python gen_bk7256partitions.py bk7256Partitions.csv --output-paths=./config/output_paths.txt
4、分区工具运行，根据工具集成的cli客户端所提示的信息进行自主选择创建所需要的*.sag、*.json和*.c/*.h文件
5、文本文件partTableTool/output_paths.txt中可自定义步骤4所列的文件生成路径，重新赋值对应参数来指定对应类型文件的生成路径
	to_json = ./bk7256_partition_temp.json
	to_sag = ./bk7256_partition_temp.sag
	to_src = ./bk7256_partition_temp.c
	to_inc = ./bk7256_partition_temp.h