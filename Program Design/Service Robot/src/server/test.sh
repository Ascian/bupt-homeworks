g++ -o test.exe test/test.cpp  base/script_interpreter/step.cpp base/script_interpreter/syntax_tree.cpp base/script_interpreter/interpreter.cpp

is_error=0
#脚本解析自动测试
script_test_num=16
i=1
while(($i<=$script_test_num))
do
    ./test.exe test/script_test/script_test.$i.txt > out.txt
    if ! cmp -s out.txt test/script_test/script_test.$i.out.txt;
    then
        echo script_test.$i: BAD
        is_error=1
    fi
    let "i++"
done

#脚本解释执行自动测试1
userinput_test_1_num=2
i=1
while(($i<=$userinput_test_1_num))
do
    ./test.exe test/userinput_test_1/script.txt test/userinput_test_1/userinput_test.$i.txt name=陈先生 amount=100 > out.txt
    if ! cmp -s out.txt test/userinput_test_1/userinput_test.$i.out.txt;
    then
        echo userinput_test_1.$i: BAD
        is_error=1
    fi
    let "i++"
done

#脚本解释执行自动测试2
userinput_test_2_num=2
i=1
while(($i<=$userinput_test_2_num))
do
    ./test.exe test/userinput_test_2/script.txt test/userinput_test_2/userinput_test.$i.txt order=5 logistcs=3 phone=10001 qq=123455 > out.txt
    if ! cmp -s out.txt test/userinput_test_2/userinput_test.$i.out.txt;
    then
        echo userinput_test_2.$i: BAD
        is_error=1
    fi
    let "i++"
done

rm -f out.txt
rm -f test.exe

if (($is_error==1));
then
    echo 按任意键关闭
    read -n 1
fi
