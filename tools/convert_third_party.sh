#!/bin/bash
$current_dir=$1
file_list=`find $current_dir -type f -name "*[h|cpp|cc|BUILD]"`
for file in $file_list
do
  echo $file
  sed -i 's/thirdparty/third_party/g' $file
done

