#!/bin/zsh
nop="__asm__(\"nop\");\n"
nop_pad=()
foreach i (1 2 3)  
  export STR1=`cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 7 | head -n 1 | awk '{print $1}'`
  #echo $STR1
  cat p3.h.template | sed s/ABCDEF/$STR1/ > p3.h
  #---------------------------------------------------------------------
  # Generat 1-11 random lines of nop instructions
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
  sed "s/NOP_PAD1/${nop_pad[1]}/g;s/NOP_PAD2/${nop_pad[2]}/g;s/NOP_PAD3/${nop_pad[3]}/g" p3.c.template > p3.c 
  
  gcc -m32 -o ${i}_p3 p3.c
  echo "${i}_p2\t$STR1" >> p3result.txt
  rm p3.h
  #rm p3.c
end
