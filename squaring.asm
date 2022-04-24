    addi $a0 $zero 201  / max value to square + 1 (N)
    add $s0 $zero $zero / current value of square (n^2)
    add $s1 $zero $zero / current number squaring (n)
loop:
    sw $s0 0($t0)       / write the value of the current square to memory
    add $s0 $s0 $s1     / increment $s0 by $s1
    add $s0 $s0 $s1     / increment $s0 by $s1
    addi $s1 $s1 1      / increment $s1
    addi $s0 $s0 1      / increment $s0
    sll $t0 $s1 2       / set $t0 = 4 * $s1
    bne $s1 $a0 loop    / if $s1 is not equal to $a0, jump to 'loop'
    exit                / exit program