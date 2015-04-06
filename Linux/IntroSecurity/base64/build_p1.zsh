#!/bin/zsh
foreach i (bob alice jim)
  #echo $i $i $i $i $i | sum | awk '{print $1}'
  export VAR=`echo $i | awk '{print $1}'`
  echo $VAR
  export VAR1=`echo $VAR | tr -d '\n' |openssl base64 `
  #echo $VAR1
  cat p1.h.template | sed s/ABCDEF/$VAR/ >! p1.h
  # Generate the answer to each program
  echo ${i}_p1\t$VAR1 >> p1_answer.txt

  gcc -m32 -o ${i}_p1 p1.c -lcrypto -lm
  rm p1.h
end
