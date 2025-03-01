
    .text
    .globl main
    
main:
    lw $a0, 1040($0) 
    lw $t8, 1004($0)
    
    lw $s0, 1004($0)          
    beq $a0, $0, fib_return0     
    beq $a0, $s0, fib_return1

    lw $t1, 1000($0)   
    lw $t2, 1004($0)   
    lw $t3, 1008($0)   
    
fib_loop:
    beq $t3, $a0, fib_return
    
    add $t4, $t1, $t2
   
    add $t1, $t2, $0 
    add $t2, $t4, $0   
    add $t3, $t3, $t8 
    
    j fib_loop

fib_return:
    sw $t2, 6000($0)         
    j exit_loop
fib_return0:
    sw $a0, 6000($0) 
    j exit_loop
fib_return1:
    sw $a0, 6000($0)  
    j exit_loop
exit_loop:
    add $0, $0, $0
    
