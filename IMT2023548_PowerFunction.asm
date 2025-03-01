    .text
    .globl main
    
main:
    lw $t0, 1012($0)     
    lw $t1, 1016($0)      

    lw $t2, 1004($0)
    lw $t3, 1004($0)
    sub $t4, $0, $0    

    beq $t1, $0, end   

power_loop:
    mul $t2, $t2, $t0     
    sub $t1, $t1, $t3   
    beq $t1, $t4, end 
    j power_loop
end:
    sw $t2, 6000($0)

