#!/bin/bash
# Author: yweiyun@163.com (Ares Young)

namespace_usage="instance namespace_name"
function get_coding_base() {
  local flag=`ls | grep devel`
  local old_pwd=`pwd`
  while [ -z "$flag" ]
  do
    cd -P ../
    dir=`pwd`
    flag=`ls | grep devel`
  done
  cd $old_pwd
  echo $dir
}

coding_home=`get_coding_base`
lint_bin=${coding_home}/devel/cpplint.py
if [[ $# -gt 0 ]]; then
  code_dir=$1
  shift
else
  code_dir=`pwd`
fi
file_list=`find $code_dir -type f ! -path '*/.svn/*' -name "*[h|cc|cpp]"`
python $lint_bin $file_list 2>&1 |
while read line
do
  file_name=${line%%:*}
  file_line=`echo $line |awk -F':' '{print $2}'`
  echo $file_name, $file_line
  if echo $line | grep -P "readability/namespace" 2>&1 1>/dev/null
  then
    if [[ $# -lt 1 ]]; then
      echo $namespace_usage
      exit
    fi
    $namespace=$1
    sed -i "$file_line s/$/  \/\/ namespace $namespace/g" $file_name
  elif echo $line |grep -P "build/header_guard" 2>&1 1>/dev/null
  then
    guard_head=`echo $line|sed 's/"//g'|awk '{print $(NF-2)}'`
    echo $file_name, $file_line, $guard_head
    sed -i "$file_line s/[A-Z_]\+$/$guard_head/g" $file_name
  elif echo $line |grep -P "whitespace/blank_line" 2>&1 1>/dev/null
  then
    #sed -i "$file_line d" $file_name
    echo "unsupports"
  fi
done
