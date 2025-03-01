    .text
    .globl main
main:
    lw $s0, 1020($0)  
    lw $t8, 1004($0)	
    add $t0, $s0, $t8
    lw $t3, 1004($0)     
    lw $t4, 1004($0)
    
    factorial_loop:
        beq $t0, $t4, end_loop  
        mul $t3, $t3, $t4         
        add $t4, $t4, $t8       
        j factorial_loop
 end_loop:
    	sw $t3, 6000($0)	
    
    

    
    
    	
    	

