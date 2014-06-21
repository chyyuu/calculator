#!/bin/bash
 
function del_comment_file()
{
    #delete the comment line begin with '//comment'
    sed -i "/^[ \t]*\/\//d" $file    #i选项表示直接对文件而不是副本操作
   
    #delete the commnet line end with '//comment'
    sed -i "s/\/\/[^\"]*//" $file
 
    #delete the comment only occupied one line '/* commnet */'
    sed -i "s/\/\*.*\*\///" $file
   
    #delete the comment that occupied many lines '/*comment
    #                                              *comment
    #                                              */
    sed -i "/^[ \t]*\/\*/,/.*\*\//d" $file
   
}
 
function del_comment()
{
    for file in `ls `; do   #取cd后的参数进行循环
        case $file in      
        *.c)                   #如果是.c文件，就直接调用
            del_comment_file
            ;;
        *.cpp)               #如果是.cpp文件，也直接调用
            del_comment_file
            ;;
        *.h)                   #如果是.h文件，同样直接调用
            del_comment_file
            ;;
        *)                      
            if [ -d $file ]; then     #如果是个目录
                cd $file      打开目录进行递归调用
                del_comment
                cd ..
            fi
        ;;
    esac
    done
}
 
#从第一个参数中获取源文件名或源文件目录
DIR=$1
if [ ! -e $DIR ]; then  //如果不存在
    echo "The file or directory does not exist."
    exit 1;
fi
 
#如果是一个文件
if [ -f $DIR ]; then
    file=`basename $DIR`   #去掉文件的后缀名
    if [[ `echo $DIR | grep /` == $DIR ]]; then
        cd `echo $DIR | sed -e "s/$file//"`  #将文件名中的前边部分全部用空换掉，s是替换的意思
        del_comment_file
    else
        del_comment_file
    fi
 
    exit 0;
fi
 
if [ -d $DIR ]; then     #如果是目录
    cd $DIR                 #打开目录，然后进入目录进行处理调用
    del_comment  
    exit 0;
fi
