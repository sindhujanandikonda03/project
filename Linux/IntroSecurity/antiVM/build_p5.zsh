#!/bin/zsh
nop="__asm__(\"nop\");\n"
nop_pad=()
foreach i (1 2 3 4 5)  
  export STR1=`cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 7 | head -n 1 | awk '{print $1}'`
  #echo $STR1
  cat p5.h.template | sed s/ABCDEF/$STR1/ > p5.h
  #---------------------------------------------------------------------
  foreach m (1 2 3)
     let "r = $RANDOM % 10"
     #echo $r
     for j in {0..$r}
        do
          nop_pad[$m]+=$nop
        done
  end
  #for i in ${nop_pad[@]} 
  #      do
  #        echo $i
  #      done 
  sed "s/NOP_PAD1/${nop_pad[1]}/g;s/NOP_PAD2/${nop_pad[2]}/g;s/NOP_PAD3/${nop_pad[3]}/g" p5.c.template > p5.c 
  gcc -m32 -o ${i}_p5 p5.c
  echo "${i}_p5\t$STR1" >> p5result.txt
  rm p5.h
  rm p5.c
end
