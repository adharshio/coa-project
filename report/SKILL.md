# SKILL.md — LaTeX Project Report: Buffer Overflow & Control Hazards in RISC-V
## For use with TeXmaker | Based on NSS College Guidelines + Reference Report Structure

---

## 1. OVERVIEW

This skill file guides the creation of a complete, formatted LaTeX project report titled:
**"Buffer Overflow and Control Hazards in RISC-V Architecture"**
for Group No. 4 — Aswin Manoj, Aswin R, Adharsh K, Angelino Saul.
Course: Computer Organization & Architecture | NSS College of Engineering, Palakkad.

The report must follow the college formatting guidelines exactly and mirror the chapter
structure of the reference report (The CPI Gap) in quality and depth.

---

## 2. COLLEGE FORMATTING RULES (MANDATORY)

These must be enforced throughout every LaTeX file.

### 2.1 Font
- Entire report: **Times New Roman**
- Use LaTeX package: `\usepackage{times}` or `\usepackage{mathptmx}`
- Chapter/Section Title: **18pt, Bold, Centered**
- Heading 2 (subsection): **16pt, Bold, Left-aligned**
- Heading 3 (subsubsection): **14pt, Bold, Left-aligned**
- Body text: **12pt, Normal**

### 2.2 Spacing
- Between Heading 2 entries: 3 blank lines
- Line spacing in paragraphs: **1.5 lines** → use `\usepackage{setspace}` with `\onehalfspacing`
- Use `\vspace{1em}` between sections as needed

### 2.3 Alignment
- Chapter/Section titles: **Centered**
- Heading 2 & 3: **Left-aligned** (default in LaTeX)
- Body text: **Justified** → LaTeX default, confirm with `\justifying`

### 2.4 Figures & Tables
- All figures: numbered, caption **below** the figure
  ```latex
  \begin{figure}[h]
    \centering
    \includegraphics[width=0.8\textwidth]{filename}
    \caption{Description of figure.}
    \label{fig:label}
  \end{figure}
  ```
- All tables: numbered, caption **above** the table
  ```latex
  \begin{table}[h]
    \caption{Description of table.}
    \centering
    \begin{tabular}{...}
    ...
    \end{tabular}
    \label{tab:label}
  \end{table}
  ```

### 2.5 Page Margins (standard for bound reports)
```latex
\usepackage[top=1in, bottom=1in, left=1.5in, right=1in]{geometry}
% Left margin 1.5in for binding
```

---

## 3. DOCUMENT ORDER (from guidelines)

Generate these files/sections IN THIS ORDER:

```
i.   Top Cover (cover.tex)
ii.  Title Page (titlepage.tex)
iii. Certification Page (certification.tex)
iv.  Acknowledgement (acknowledgement.tex)
v.   Abstract (abstract.tex)
vi.  Table of Contents (auto-generated)
vii. List of Figures and Tables (auto-generated)
viii.Chapters (chapter1.tex through chapter6.tex)
ix.  Appendix (appendix.tex)
x.   References/Bibliography (references.bib)
```

All files are \input{} into a single `main.tex` master file.

---

## 4. CHAPTER STRUCTURE

Mirror the reference report's chapter structure, adapted for this project:

```
Chapter 1: Introduction
  1.1 Background and Motivation
  1.2 Problem Statement
  1.3 Objectives
  1.4 Study Scope and Constraints
  1.5 Report Structure

Chapter 2: Theoretical Background
  2.1 Memory and Stack Architecture in RISC-V
  2.2 The 5-Stage Pipeline
  2.3 Pipeline Hazards
  2.4 Control Hazards and Branch Flush Penalty
  2.5 Buffer Overflow: Definition and Classification
  2.6 Stack-Based Buffer Overflow Mechanics
  2.7 Return Address Corruption and PC Hijack

Chapter 3: Methodology
  3.1 Experimental Setup (Ripes configuration)
  3.2 Program Design and Logic
  3.3 Stack Memory Map
  3.4 Instruction Flow Design
  3.5 Measurement Procedure

Chapter 4: Results and Observations
  4.1 Normal Stack Setup (sw ra, 12(sp))
  4.2 Buffer Write Phase
  4.3 The Overflow Event (sw t0, 12(sp))
  4.4 Return Address Corruption (lw ra, 12(sp))
  4.5 PC Hijack via jr ra
  4.6 Control Hazard in the Pipeline
  4.7 Memory Comparison (Before vs After)
  4.8 Output Analysis Table

Chapter 5: Discussion
  5.1 Memory Integrity Violation
  5.2 Control Flow Hijacking
  5.3 Pipeline Impact
  5.4 Security Implications
  5.5 Mitigations (Stack Canaries, ASLR, DEP, Bounds Checking)
  5.6 Limitations of the Study

Chapter 6: Conclusion

Appendix A: RISC-V Assembly Source Code
References
```

---

## 5. KEY CONTENT FOR EACH CHAPTER

### Chapter 1 — Introduction

**1.1 Background and Motivation**
- Modern processors use pipelining for throughput
- Security vulnerabilities at instruction level affect architectural behavior
- Buffer overflow is a historical and ongoing real-world threat
- Ripes allows architectural-level observation not possible in high-level languages

**1.2 Problem Statement**
- Direct quote from project brief:
  *"The core problem is to bridge the gap between theoretical concepts of memory mapping,
  instruction execution, and pipeline hazards with a practical demonstration of how
  vulnerabilities at the instruction level can propagate through processor stages and affect
  overall system behavior."*

**1.3 Objectives** (4 bullet objectives from presentation slide 3)

**1.4 Study Scope and Constraints**
- Simulator: Ripes (5-stage, RV32I, RV32IM)
- Language: RISC-V assembly
- Attack type: Stack-based return address overwrite
- No OS, no cache modelling
- Controlled, intentional overflow (not accidental)

---

### Chapter 2 — Theoretical Background

**2.1 Memory and Stack Architecture**
- Stack grows downward in RISC-V (sp decreases)
- Stack frame layout: [buf0 | buf1 | padding | saved_ra]
- sp + 0 = buf[0], sp + 4 = buf[1], sp + 12 = saved return address
- Draw this as a table:

```
| Offset | Address (example)  | Contents           |
|--------|--------------------|--------------------|
| sp+0   | 0x7fffffe0         | buf[0] = 10        |
| sp+4   | 0x7fffffe4         | buf[1] = 10        |
| sp+8   | 0x7fffffe8         | (unused/padding)   |
| sp+12  | 0x7fffffec         | saved ra (TARGET)  |
```

**2.2 The 5-Stage Pipeline** — table of IF/ID/EX/MEM/WB stages

**2.3 Pipeline Hazards** — structural, data, control hazard table

**2.4 Control Hazards**
- Branch resolution at EX stage
- 2-cycle flush penalty formula: Penalty = 2 cycles per taken branch
- Trace table (like reference report Table 2.3):

```
| Instruction | C1 | C2 | C3  | C4  | C5 |
|-------------|----|----|-----|-----|----|
| jr ra       | IF | ID | EX  | MEM | WB |
| Fetch+1     |    | IF |flush|     |    |
| Fetch+2     |    |    | IF  |flush|    |
| secret[0]   |    |    |     | IF  | ID |
```

**2.5 Buffer Overflow Definition**
- NIST/textbook definition
- Stack-based vs heap-based distinction
- Why sp+12 is the critical location

**2.6 Stack-Based Overflow Mechanics**
- Normal write: sp+0, sp+4 → within buffer
- Overflow write: sp+12 → overwrites saved ra

**2.7 Return Address Corruption**
- ra is architectural register x1
- jal writes PC+4 into ra
- Corrupted ra → PC takes wrong value → execution hijacked

---

### Chapter 3 — Methodology

**3.1 Experimental Setup**
- Ripes v[version] configured:
  - Processor: 5-Stage Pipelined
  - ISA: RV32IM
  - Data forwarding: default
  - Branch prediction: none
- Include Figure: Ripes processor selection screenshot (Screenshot 1 from simulation)

**3.2 Program Design**
Include full annotated program:
```asm
.text
.globl main

main:
    jal ra, vulnerable      # call function; ra = addr of "j end"
    j end

vulnerable:
    addi sp, sp, -16        # allocate 16-byte stack frame
    sw ra, 12(sp)           # save legitimate return address

    # Normal buffer writes
    li t0, 10
    sw t0, 0(sp)            # buf[0] = 10
    sw t0, 4(sp)            # buf[1] = 10

    # Overflow: overwrite saved return address
    la t0, secret
    sw t0, 12(sp)           # OVERFLOW: saved ra → address of secret

    # Return using corrupted ra
    lw ra, 12(sp)           # ra = address of secret (corrupted)
    addi sp, sp, 16
    jr ra                   # PC hijacked → jumps to secret

secret:
    li t1, 999              # marker: proves execution reached here
    j end

end:
    nop
```

**3.3 Stack Memory Map** — table as in section 2.1

**3.4 Instruction Flow** — 6-step flow (from presentation slides 6–7)

**3.5 Measurement Procedure**
- Load program into Ripes
- Single-step execution (▶| button)
- Observe: Register panel, Memory panel, Pipeline diagram
- Screenshot at each critical instruction

---

### Chapter 4 — Results and Observations

Each subsection corresponds to one screenshot taken in Ripes.
Format each subsection as: [what was done] → [screenshot] → [what it shows]

**4.1 Normal Stack Setup**
- After `sw ra, 12(sp)`
- ra (x1) = 0x00000004 in register panel
- Memory at sp+12 = 0x00000004
- → Figure 4.1: ss2 screenshot
- Pipeline shows all 5 stages active (WB/MEM/EX/ID/IF)

**4.2 Buffer Write Phase**
- After `sw t0, 0(sp)` and `sw t0, 4(sp)`
- Memory at sp+0 = sp+4 = 0x0000000A (value 10)
- sp+12 still intact = 0x00000004

**4.3 The Overflow Event** ⭐ Most Important
- After `sw t0, 12(sp)`
- → Figure 4.2: ss3 screenshot
- Memory at sp+12 CHANGED from 0x00000004 to 0x00000034
- t0 (x5) = 0x0000001c (intermediate address value loaded by la)
- This is the buffer overflow: a protected memory location overwritten

**4.4 Return Address Corruption**
- After `lw ra, 12(sp)`
- → Figure 4.3: ss4 screenshot
- ra (x1) = 0x00000034 (address of secret)
- Compare: ra was 0x00000004, now 0x00000034
- Register file is now corrupted

**4.5 PC Hijack via jr ra**
- → Figure 4.4: ss4 screenshot (same or next step)
- PC has jumped to 0x00000034 = address of secret
- li t1, 999 is now the current instruction
- t1 = 999 proves code inside secret is executing
- Normal path: main → vulnerable → main
- Actual path: main → vulnerable → secret

**4.6 Control Hazard in the Pipeline**
- → Figure 4.5: ss5 screenshot
- jr ra in EX stage
- li t1, 999 in ID, j end in IF — speculatively fetched from secret
- These are wrong instructions that pipeline accepted
- Control hazard: processor did not know target until EX resolved
- 2-cycle penalty incurred

**4.7 Memory Comparison (Before vs After)**
- → Figure 4.6: side-by-side memory screenshot
- Left: sp+12 (0x7fffffec) = 0x00000004 (before overflow)
- Right: sp+12 (0x7fffffec) = 0x00000034 (after overflow)
- sp+0 and sp+4 unchanged at 0x0000000A in both

**4.8 Output Analysis Table**
Reproduce the 10-stage output analysis table from the presentation:

```latex
\begin{table}[h]
\caption{Output Analysis: Register and Memory State at Each Execution Stage}
\centering
\begin{tabular}{|c|l|l|l|}
\hline
\textbf{Stage} & \textbf{Condition} & \textbf{Register/Memory State} & \textbf{Behavior} \\
\hline
1 & Before Function Call & ra = return to main & Normal execution begins \\
2 & After jal & ra updated & Control to vulnerable \\
3 & Stack Setup & sp decreased, ra at 12(sp) & Stack frame created \\
4 & Normal Buffer Write & 0(sp), 4(sp) updated & Valid memory ops \\
5 & Before Overflow & ra in memory = original & System stable \\
6 & \textbf{After Overflow} & \textbf{12(sp) = secret addr} & \textbf{Return addr CORRUPTED} \\
7 & Load Return Address & ra = address of secret & Corrupted value loaded \\
8 & Function Return (jr ra) & PC = secret & Control flow changed \\
9 & Execution at secret & t1 = 999 & Unexpected instruction \\
10 & Program End & Jump to end & Abnormal termination \\
\hline
\end{tabular}
\end{table}
```

---

### Chapter 5 — Discussion

**5.1 Memory Integrity Violation**
- sp+12 is a protected architectural slot (saved ra)
- No hardware prevented the write
- Absence of bounds checking is the root vulnerability

**5.2 Control Flow Hijacking**
- Definition: execution redirected without explicit call
- Normal: main → vulnerable → main (via ra = 0x4)
- Actual: main → vulnerable → secret (via ra = 0x34)
- Real-world parallel: ROP (Return-Oriented Programming) attacks

**5.3 Pipeline Impact**
- jr ra resolves at EX stage → 2-cycle penalty
- Speculatively fetched instructions: li t1,999 in ID, j end in IF
- These happen to be from secret — no flush visible, but wrong path accepted
- Control hazard type: corrupted indirect branch target

**5.4 Security Implications**
Connect to real CVEs:
- Stack smashing (classic exploit since Morris Worm 1988)
- Return-to-libc attacks
- Why RISC-V's simplicity makes this easier to demonstrate

**5.5 Mitigations**
For each mitigation, explain what it does and how it would prevent this attack:

| Mitigation | How it works | Would it stop this? |
|---|---|---|
| Stack Canary | Sentinel value between buffer and saved ra; checked before return | Yes |
| ASLR | Randomizes memory layout so attacker can't know address of secret | Yes |
| DEP/NX | Marks stack as non-executable | Partial |
| Bounds Checking | Hardware or compiler validates sw target | Yes |
| Safe Languages | Compiler prevents direct memory manipulation | Yes |

**5.6 Limitations**
- Intentional overflow (not input-driven)
- No OS or privilege levels modelled
- Ripes has no cache, no superscalar, no out-of-order
- Fixed address of secret (no ASLR in simulation)

---

### Chapter 6 — Conclusion

Cover these points:
1. Demonstrated complete chain: sw instruction → memory corruption → ra overwrite → PC hijack → control hazard
2. Verified using Ripes screenshots at each stage
3. Proved that even a single sw instruction to the wrong address can subvert processor control flow
4. Identified the 2-cycle pipeline penalty from the jr ra control hazard
5. Connected to real-world security implications
6. Emphasised need for memory protection mechanisms at both hardware and software levels

---

### Appendix A — RISC-V Assembly Source Code

Full annotated listing of the program (as in section 3.2, but untruncated).
Use LaTeX `listings` package:
```latex
\usepackage{listings}
\lstset{language=[RISC-V]Assembler, basicstyle=\ttfamily\small, ...}
```

### References

Minimum 5 references. Suggested:
1. Patterson & Hennessy — Computer Organization and Design RISC-V Edition
2. Stallings — Computer Organization and Architecture
3. Aleph One — "Smashing the Stack for Fun and Profit" (Phrack, 1996)
4. NIST — Definition of Buffer Overflow (NVD/CWE-121)
5. Waterman & Asanovic — RISC-V ISA Manual

---

## 6. LATEX MASTER FILE STRUCTURE (main.tex)

```latex
\documentclass[12pt,a4paper]{report}

% --- Packages ---
\usepackage{times}           % Times New Roman
\usepackage{setspace}        % Line spacing
\usepackage{geometry}        % Margins
\usepackage{graphicx}        % Figures
\usepackage{booktabs}        % Professional tables
\usepackage{tabularx}        % Flexible tables
\usepackage{caption}         % Figure/table captions
\usepackage{listings}        % Code listings
\usepackage{amsmath}         % Math equations
\usepackage{hyperref}        % Hyperlinks in PDF
\usepackage{fancyhdr}        % Headers/footers
\usepackage{titlesec}        % Section title formatting
\usepackage{ragged2e}        % Justification

% --- Geometry ---
\geometry{top=1in, bottom=1in, left=1.5in, right=1in}

% --- Spacing ---
\onehalfspacing

% --- Title formatting ---
% Chapter title: 18pt Bold Centered
\titleformat{\chapter}[display]
  {\normalfont\fontsize{18}{22}\bfseries\centering}
  {Chapter \thechapter}{20pt}{}

% Section (Heading 2): 16pt Bold Left
\titleformat{\section}
  {\normalfont\fontsize{16}{20}\bfseries}
  {\thesection}{1em}{}

% Subsection (Heading 3): 14pt Bold Left
\titleformat{\subsection}
  {\normalfont\fontsize{14}{18}\bfseries}
  {\thesubsection}{1em}{}

% --- Header/Footer ---
\pagestyle{fancy}
\fancyhf{}
\rhead{\textit{Buffer Overflow and Control Hazards in RISC-V}}
\lhead{\textit{NSS College of Engineering, Palakkad}}
\cfoot{\thepage}

% --- Document begins ---
\begin{document}

\input{cover}
\input{titlepage}
\input{certification}
\input{acknowledgement}
\input{abstract}

\tableofcontents
\listoffigures
\listoftables

\input{chapter1}
\input{chapter2}
\input{chapter3}
\input{chapter4}
\input{chapter5}
\input{chapter6}
\input{appendix}

\bibliographystyle{ieeetr}
\bibliography{references}

\end{document}
```

---

## 7. SCREENSHOT-TO-FIGURE MAPPING

| Ripes Screenshot | Figure Number | Caption | Chapter |
|---|---|---|---|
| ss1 — Program loaded in Ripes | Figure 3.1 | Ripes simulator with 5-stage processor and program loaded | Ch. 3 |
| ss2 — After sw ra,12(sp) | Figure 4.1 | Register file showing ra=0x00000004 after legitimate save | Ch. 4 |
| ss3 — After sw t0,12(sp) | Figure 4.2 | sw t0,12(sp) in MEM stage — overflow instruction executing | Ch. 4 |
| ss4 — ra=0x34, secret executing | Figure 4.3 | ra corrupted to 0x00000034; li t1,999 is current instruction | Ch. 4 |
| ss5 — jr ra in pipeline | Figure 4.4 | jr ra in EX stage; secret instructions in ID and IF | Ch. 4 |
| Memory side-by-side | Figure 4.5 | Memory at sp+12 before (0x4) and after (0x34) overflow | Ch. 4 |

Place all screenshot image files in an `images/` folder next to main.tex.
Reference in LaTeX as: `\includegraphics[width=\textwidth]{images/ss2.png}`

---

## 8. TEXMAKER WORKFLOW

### First-time setup in TeXmaker:
1. Open TeXmaker → Options → Configure TeXmaker
2. Set pdflatex as default compiler
3. Enable "Use a build system" → pdflatex + bibtex + pdflatex × 2

### Build order:
```
pdflatex main.tex    ← first pass (creates .aux)
bibtex main          ← processes bibliography
pdflatex main.tex    ← second pass (resolves citations)
pdflatex main.tex    ← third pass (resolves cross-references)
```

### Or use the Quick Build button (F1) if configured to run all steps.

### File organization in TeXmaker project folder:
```
project/
├── main.tex
├── cover.tex
├── titlepage.tex
├── certification.tex
├── acknowledgement.tex
├── abstract.tex
├── chapter1.tex
├── chapter2.tex
├── chapter3.tex
├── chapter4.tex
├── chapter5.tex
├── chapter6.tex
├── appendix.tex
├── references.bib
└── images/
    ├── ss1.png
    ├── ss2.png
    ├── ss3.png
    ├── ss4.png
    ├── ss5.png
    └── memory_comparison.png
```

---

## 9. FRONT MATTER CONTENT

### Cover Page content:
```
NSS COLLEGE OF ENGINEERING, PALAKKAD
(Autonomous)
Department of Computer Science and Engineering

Project Report
on

BUFFER OVERFLOW AND CONTROL HAZARDS
IN RISC-V ARCHITECTURE

Submitted in partial fulfillment of the requirements for the course
Computer Organization & Architecture (PBCST404)

Submitted by:
Aswin Manoj       (Roll No: ___)
Aswin R           (Roll No: ___)
Adharsh K         (Roll No: ___)
Angelino Saul     (Roll No: ___)

Group No: 4

Academic Year: 2025-2026
```

### Certification Page content:
```
This is to certify that the project report entitled
"Buffer Overflow and Control Hazards in RISC-V Architecture"
is a bonafide record of the work done by Group No. 4
[names and roll numbers]
under our guidance during the academic year 2025-2026.

Signature of Guide          Signature of HOD
Name:                       Name:
Date:                       Date:
```

### Acknowledgement (suggested content):
```
We express our sincere gratitude to [Guide Name], our project guide,
for their valuable guidance and support throughout this project.
We thank the Department of Computer Science and Engineering,
NSS College of Engineering, Palakkad, for providing access to the
necessary tools and resources. We also acknowledge the open-source
Ripes simulator project for enabling practical RISC-V pipeline analysis.
```

### Abstract (use the one already written in the presentation):
Key points to include:
- Project goal (analyze buffer overflow impact on RISC-V pipeline)
- Method (Ripes simulator, RISC-V assembly, 5-stage pipeline)
- Program used (vulnerable function with stack-based overflow)
- Key finding (ra corrupted from 0x00000004 to 0x00000034, PC hijacked to secret)
- Pipeline result (control hazard, 2-cycle penalty, jr ra resolved at EX)
- Keywords: Buffer Overflow, RISC-V, Stack-Based Attack, Return Address, Control Hazard, Pipeline Flush, Ripes Simulator

---

## 10. IMPORTANT LATEX TIPS FOR THIS REPORT

### For code listings (assembly):
```latex
\usepackage{listings}
\lstdefinelanguage{RISCV}{
  morekeywords={addi,sw,lw,jal,jalr,jr,la,li,bne,blt,add,sub,nop,j},
  sensitive=false,
  morecomment=[l]{\#},
}
\lstset{
  language=RISCV,
  basicstyle=\ttfamily\small,
  keywordstyle=\bfseries,
  commentstyle=\itshape,
  numbers=left,
  numberstyle=\tiny,
  frame=single,
  breaklines=true,
  captionpos=b
}
```

### For hex values inline:
```latex
\texttt{0x00000004}   % always use \texttt for hex values
```

### For the memory map table:
Use `tabularx` with fixed column widths for clean alignment.

### For the before/after memory comparison figure:
Use `\begin{figure}` with `\subfigure` or `minipage`:
```latex
\begin{figure}[h]
  \centering
  \begin{minipage}{0.48\textwidth}
    \includegraphics[width=\textwidth]{images/memory_before.png}
    \caption*{Before: sp+12 = 0x00000004}
  \end{minipage}
  \hfill
  \begin{minipage}{0.48\textwidth}
    \includegraphics[width=\textwidth]{images/memory_after.png}
    \caption*{After: sp+12 = 0x00000034}
  \end{minipage}
  \caption{Side-by-side memory comparison showing return address corruption at sp+12.}
  \label{fig:memory_comparison}
\end{figure}
```

---

## 11. QUALITY CHECKLIST BEFORE SUBMISSION

- [ ] All fonts are Times New Roman throughout
- [ ] Chapter titles are 18pt Bold Centered
- [ ] Section headings are 16pt Bold Left-aligned
- [ ] Subsection headings are 14pt Bold Left-aligned
- [ ] Body text is 12pt, 1.5 line spacing, justified
- [ ] Left margin is 1.5in (for binding)
- [ ] All figures have captions BELOW
- [ ] All tables have captions ABOVE
- [ ] All figures and tables are numbered and referenced in text
- [ ] Screenshots placed in correct chapters
- [ ] Appendix contains full source code
- [ ] References list has minimum 5 entries
- [ ] Table of Contents is auto-generated (not manual)
- [ ] List of Figures and List of Tables are present
- [ ] Certification page has space for signatures
- [ ] Abstract includes keywords

---

*This SKILL.md was generated for the Buffer Overflow & Control Hazards project report by Group 4, NSS College of Engineering, Palakkad. Use this file to guide chapter-by-chapter LaTeX generation in TeXmaker.*
