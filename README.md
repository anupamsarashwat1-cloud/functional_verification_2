# TITAN-X Functional Verification (Phase 2)

This repository contains the advanced functional coverage and timing analysis results for the SMVDU TITAN-X RISC-V SoC.

## Verification Strategies Executed

1. **Static Timing Analysis (STA)**
   - **Toolchain**: Yosys (Synthesis), OpenSTA (Timing Analysis)
   - **Target Library**: OSU 180nm Open Cell Library (proxy for SCL 180nm)
   - **Target Frequency**: 200 MHz (5.0ns clock period)
   - **Results**: See `sta_report.md`. The current maximum frequency for the execution pipeline in 180nm is approximately ~88 MHz (11.36ns critical path).  

2. **Synthesis Methodology**
   - Synthesized the `rv_core_top` module independently using a custom Yosys flow (`yosys_synth.tcl`).
   - Mapped all internal logic and small memories (TLB, BPU) directly to D-Flip-Flops using standard cell technology mapping (`dfflibmap`, `abc`).
   - Black-boxed un-synthesizable SRAM IP (L1 Caches) to isolate the CPU pipeline timing.

## Next Steps for Deep Functional Coverage
* **Constrained-Random Verification**: Use advanced SV/UVM frameworks (or Python tools like Cocotb) to flood the interconnect with randomized AXI traffic to test arbitration and MPU protection.
* **Instruction Set Verification**: Run RISC-V architectural compliance test suites (e.g. `riscv-tests`) through RTL simulation to prove software boot capabilities.
