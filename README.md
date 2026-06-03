# TITAN-X SoC — Verification, Synthesis, & STA (Phase 2)

This repository contains the full verification and synthesis infrastructure for the TITAN-X RISC-V SoC (Project Penguin 3). It houses all logs, testbenches, setup scripts, and synthesis results targeting the SCL 180nm process (via the OSU018 library).

## 🗂️ Repository Structure

```text
functional_verification_2/
├── README.md                    # This file (Summary of all processes and results)
├── VERIFICATION_PLAN.md         # Detailed plan for functional coverage
├── VERIFICATION_REPORT.md       # Status of current verification and sign-off
├── VERIFICATION_TASKS.md        # Remaining tasks checklist
├── sta_report.md                # Summary of Static Timing Analysis results
│
├── software_boot/               # Boot rom payload generation scripts
│
├── synthesis/                   # Scripts to compile the RTL
│   ├── README.md
│   ├── gen_yosys.py             # Script to generate synth run files
│   ├── yosys_synth.tcl          # Yosys synthesis script for SCL 180nm
│   ├── opensta.tcl              # OpenSTA setup script
│   ├── titan_x.sdc              # SDC timing constraints (7.2ns / 138.8 MHz target)
│   └── report_fanout.tcl        # Custom script for fanout analysis
│
└── logs/                        # Output logs from all verification flows
    ├── compilation/             # iverilog syntax check logs
    ├── lint/                    # Verilator structural linting log
    ├── synthesis/               # Main synthesis flow and optimization logs
    └── sta/                     # OpenSTA timing reports & fanout traces
```

## 🛠️ Step-by-Step Process & Results

### 1. Structural Linting
- **Tool**: Verilator (`--lint-only`)
- **Process**: The top-level module `titan_x_top.v` and all 60+ submodules were checked for structural integrity, floating inputs, unassigned wires, and bit-width mismatches.
- **Result**: **PASS**. The SoC fabric (15-Master/9-Slave AXI4 crossbar, AHB/APB bridges, and all peripherals) successfully interconnects with no floating ports or implicit type conversion warnings.
- **Log Location**: `logs/lint/lint.log`

### 2. Logic Synthesis
- **Tool**: Yosys + ABC
- **Target**: OSU 180nm Standard Cell Library (`osu018_stdcells.lib` - proxy for SCL 180nm).
- **Process**: 
  - Synthesized the `rv_core_top` module (the quad-core RISC-V CPU subsystem).
  - L1 Instruction (`rv_icache`) and Data (`rv_dcache`) caches were explicitly black-boxed using the `(* blackbox *)` attribute, as they will be replaced by actual SCL 180nm SRAM macros later.
  - Mapped all internal logic (TLBs, MMU, BPU) directly to D-Flip-Flops (`dfflibmap`).
  - Iterative optimizations were applied, specifically restructuring the ALU carry chains and multiplier logic for 180nm.
- **Result**: **COMPLETE**. The CPU core successfully synthesized into a gate-level netlist optimized for delay.
- **Log Location**: `logs/synthesis/yosys_synthesis_180nm*.log`

### 3. Static Timing Analysis (STA) & Optimization
- **Tool**: OpenSTA (v2.3.1)
- **Constraint**: `titan_x.sdc` (Clock: `clk` at 7.2ns / 138.8 MHz)
- **Initial Result (Failure)**: Initially, the design failed timing severely (max frequency ~88 MHz) due to a massive fanout bottleneck on the `flush_de_raw` control signal, which cleared over 300 flip-flops across the Fetch and Decode stages upon a branch misprediction or exception.
- **Optimization Process**:
  - Identified the critical path using `report_fanout.tcl`.
  - Edited the RTL (`rv_execute.v`, `rv_decode.v`, `rv_core_top.v`) to explicitly instantiate `BUFX4` clock buffer primitives to create a balanced buffering tree for the `flush` signal.
  - Split pipeline stall logic to alleviate long combinational paths from the D-Cache stall request back to the IF/ID stages.
- **Final Result**: **TIMING CLOSED**.
  - **Operating Frequency**: 138.8 MHz (7.2 ns period)
  - **Setup WNS (Worst Negative Slack)**: 0.00 ns ✅ 
  - **Setup TNS (Total Negative Slack)**: 0.00 ns ✅
  - **Hold WNS**: 0.00 ns ✅
  - **Hold TNS**: 0.00 ns ✅
- **Log Location**: `logs/sta/opensta_180nm*.log`

### 4. Code Organization
- **Process**: All verification scripts, logs, and artifacts were stripped out of the main design repository (`Project_penguin3`) and migrated to this dedicated repository (`functional_verification_2`).
- **Result**: The design repo is clean (RTL only), while this repo houses the entire sign-off toolchain and historic logs for traceability.

## 🚀 Next Phases

The RTL design and timing closure for the SCL 180nm technology node are fully complete. The subsequent phases are:
1. **Functional Simulation**: Running the testbench (`tb_titan_x_top.sv`) with actual software boot payloads to verify instruction execution correctness.
2. **Physical Design (PnR)**: Floorplanning, placement, clock tree synthesis (CTS), and routing in OpenROAD or Cadence Innovus to achieve the final GDSII. 

*(Developed by the world's leading RTL Design team)*
