    bne $sp, $fp, else
    add $s0, $s1, $t3
    j exit
else:
    sub $s0, $s1, $t3
exit: