#!/bin/bash

#과제 출력과 형식을 맞추고자 ./을 제외
scriptname=$(basename "$0")

echo "script name: $scriptname"
echo "# of argument: $#"

#오류처리: if문으로 인자수가 0인지 판단
if [ $# -eq 0 ] 
then
    echo "Error: 입력된 argument가 없습니다!!"
    exit 1
fi

n=1
#$@은 모든 파라미터를 공백으로 구분해 담고 있음
for i in $@
do
    echo "argument $n: \"$i"\"
    n=$((n + 1))
done