## 1. 简介
- mklittlefs 是在linux下用于制作littlefs 的映像文件工具

## 2. mklittlefs 使用说明
USAGE: 

   ./mklittlefs  {-c <pack_dir>|-u <dest_dir>|-l} [-d <0-5>] [-a] [-b
                 <number>] [-p <number>] [-s <number>] [--] [--version]
                 [-h] <image_file>


Where: 

   -c <pack_dir>,  --create <pack_dir>
     (OR required)  create littlefs image from a directory
         -- OR --
   -u <dest_dir>,  --unpack <dest_dir>
     (OR required)  unpack littlefs image to a directory
         -- OR --
   -l,  --list
     (OR required)  list files in littlefs image


   -d <0-5>,  --debug <0-5>
     Debug level. 0 means no debug output.

   -a,  --all-files
     when creating an image, include files which are normally ignored;
     currently only applies to '.DS_Store' files and '.git' directories

   -b <number>,  --block <number>
     fs block size, in bytes

   -p <number>,  --page <number>
     fs page size, in bytes

   -s <number>,  --size <number>
     fs image size, in bytes

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <image_file>
     (required)  littlefs image file
	 
## 3. 制作步骤
	1. 将要制作的源文件放置到littlefs下
	2. /mklittlefs -c littlefs/ -b 4096 -p 256 -s 65536 littlefs.bin  * 储介质block size 4096字节,page size 256字节,文件系统映像size 65536字节
	3. 生成的映像文件为 littlefs.bin