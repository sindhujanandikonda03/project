#!/bin/zsh
foreach i (1 2 3)  
  export STR1=`cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 7 | head -n 1 | awk '{print $1}'`
  echo $STR1
  export STR2=`cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 7 | head -n 1 | awk '{print $1}'`
  echo $STR2
  export VAR1=`echo ${STR1:1:4}`
  export VAR2=`echo ${STR2:1:4}`
  export VAR3=`echo ${STR1:0:5}`
  
  echo "CONCAT:"$VAR1$VAR2
  echo --------------------------------------------
  cat p2.h.template | sed s/ABCDEF/$STR1/ > p2.h.temp
  cat p2.h.temp | sed s/SSS/$STR2/ > p2.h
  rm p2.h.temp
  gcc -m32 -o ${i}_p2 p2.c
  echo "${i}_p2\t$VAR1$VAR2\t$VAR3" >> p2result.txt
  rm p2.h
end 
