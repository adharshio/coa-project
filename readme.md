# 🛡️ Architectural Analysis of Memory Integrity and Control Flow via RISC-V Buffer Overflow

![RISC-V](https://img.shields.io/badge/Architecture-RISC--V-blue.svg)
![Assembly](https://img.shields.io/badge/Language-Assembly-orange.svg)
![Simulator](https://img.shields.io/badge/Simulator-Ripes-brightgreen.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

## 📌 Project Overview
This project provides a cycle-by-cycle architectural analysis of a **Stack-Based Buffer Overflow** vulnerability. By utilizing the Ripes 5-stage pipeline simulator, this project bridges the gap between low-level software vulnerabilities and hardware execution. 

We demonstrate exactly how unvalidated memory writes can overwrite a saved Return Address (`ra`), hijack the Program Counter (`pc`), and introduce a severe **Control Hazard** into the processor's datapath.

### 👥 Team (Group 4)
* **Adharsh K**
* **Aswin Manoj**
* **Aswin R**
* **Angelino Saul**
* *NSS College of Engineering, Palakkad*

---

## 🎯 Core Objectives
1. **Explore Memory Maps:** Visualize the allocation and manipulation of the stack memory space.
2. **Simulate a Hardware Attack:** Execute an intentional out-of-bounds `sw` (Store Word) instruction to crush adjacent memory.
3. **Visualize Control Hazards:** Track the corrupted return address through the Instruction Fetch (IF) and Memory (MEM) pipeline stages to observe an unauthorized program jump.

---

## 🛠️ Tools Required
* **Simulator:** [Ripes](https://github.com/mortbopet/Ripes) (Visual computer architecture simulator and assembly code editor)
* **Architecture:** 5-stage RISC-V Pipeline (RV32IM)
* **Language:** RISC-V Assembly (`.s`)

---

## 🚀 How to Run the Simulation

1. **Setup:** Download and open the Ripes Simulator.
2. **Load Code:** Paste the contents of `buffer_overflow.s` into the Ripes Editor tab.
3. **Configure Hardware:** Ensure the processor model is set to the **5-stage processor**.
4. **Locate the Stack:** Step through the first instruction (`jal ra, vulnerable`) and note the address of the Stack Pointer (`sp` / `x2`).
5. **Execute the Exploit:** * Switch to the **Memory Tab** and navigate to the `sp` address.
   * Use the **Step** button to move through the `vulnerable` function.
   * Observe the `sw t0, 12(sp)` instruction overwrite the legitimate return address.
6. **Verify Hijack:** Step through the `jr ra` instruction and observe the Program Counter jump to the `secret` function, updating register `t1` with `0x3e7` (999 in decimal).

---

## 🧠 The Vulnerability Explained (The "Lockers" Concept)
In a secure execution, the CPU allocates a set of temporary memory slots (the stack frame) for a function. 
* **Safe Zone:** The first few slots hold normal variables (the buffer).
* **Control Zone:** The highest slot holds the Return Address (`ra`)—the exact location the CPU must return to when the function ends.

Because basic RISC-V lacks inherent memory boundary protection, an attacker can feed excessive data into the "Safe Zone". The hardware blindly continues writing this data into adjacent memory, ultimately crushing the "Control Zone". When the function attempts to return, it loads the attacker's malicious address into the Program Counter, successfully hijacking the system.

---

## 💻 Source Code (`buffer_overflow.s`)

```assembly
.data
.text
.globl main

main:
    jal ra, vulnerable     # 1. Call function (saves return address in ra)
    j end                  # Normal exit (This will be bypassed!)

vulnerable:
    addi sp, sp, -16       # 2. Allocate 16 bytes (4 words) on the stack
    sw ra, 12(sp)          # 3. Store the legitimate return address at the top of the stack

    # -------- NORMAL BUFFER WRITE --------
    li t0, 10
    sw t0, 0(sp)           # Write to slot 1
    sw t0, 4(sp)           # Write to slot 2

    # -------- THE EXPLOIT (OVERFLOW) --------
    la t0, secret          # 4. Load the address of the malicious function
    sw t0, 12(sp)          # 5. OVERFLOW: Forcefully overwrite the return address!

    # -------- FUNCTION RETURN --------
    lw ra, 12(sp)          # 6. Load the corrupted return address into ra
    addi sp, sp, 16        # Deallocate stack
    jr ra                  # 7. TRIGGER HAZARD: Jump blindly to the corrupted address

secret:
    li t1, 999             # 8. VICTORY FLAG: If t1 holds 0x3e7, the hijack is successful
    j end

end:
    nop
