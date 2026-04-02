.data

.text
.globl main

main:
    jal ra, vulnerable     # call function
    j end

vulnerable:
    addi sp, sp, -16       # allocate stack
    sw ra, 12(sp)          # store return address

    # -------- NORMAL BUFFER WRITE --------
    li t0, 10
    sw t0, 0(sp)
    sw t0, 4(sp)

    # -------- OVERFLOW PART --------
    la t0, secret          # address of secret function
    sw t0, 12(sp)          # overwrite return address

    # -------- RETURN --------
    lw ra, 12(sp)
    addi sp, sp, 16
    jr ra                  # jump using corrupted ra

secret:
    li t1, 999             # just a marker instruction
    j end

end:
    nop
