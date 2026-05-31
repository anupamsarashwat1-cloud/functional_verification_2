# TITAN-X Synthesis & STA Scripts (SCL 180nm)

This directory contains the scripts required to synthesize the TITAN-X SoC for the SCL 180nm process node and perform Static Timing Analysis (STA) to verify timing closure.

## Prerequisites

You must have `yosys` and `OpenSTA` installed. If you are using the Anaconda environment provided in the setup:

```bash
conda activate opensta_env
```

Ensure that the RTL files exist at the paths referenced in `yosys_synth.tcl`. By default, this script expects to be run in a directory adjacent to the `Project_penguin3` source code, or the paths in `yosys_synth.tcl` must be updated to point to your RTL directories.

## 1. Logic Synthesis (Yosys)

Run Yosys with the provided TCL script. This script will read the Verilog source code, map it to the `osu018_stdcells.lib` library, and optimize the logic for a 7.1ns / 140MHz delay target.

```bash
yosys -s yosys_synth.tcl > yosys_synthesis.log
```

This will produce:
- `titan_x_netlist.v`: The mapped Verilog netlist using SCL 180nm standard cells.
- `yosys_synthesis.log`: The synthesis log containing area and cell statistics.

## 2. Static Timing Analysis (OpenSTA)

After the netlist is generated, run OpenSTA to verify setup and hold timing constraints. The constraints are defined in `titan_x.sdc` (currently targeting a 7.2ns clock period / 138.8 MHz).

```bash
sta opensta.tcl > opensta.log
```

Check `opensta.log` for the final Slack metrics. 
- A **Positive Slack (MET)** for both setup and hold indicates timing closure.
- A **Negative Slack (VIOLATED)** means the logic path is too slow for the target clock period.

## 3. Fanout Analysis (Optional)

If you encounter massive routing delays during STA, you can use the fanout reporting script to identify nets with extreme fanouts (like pipeline flushes):

```bash
sta report_fanout.tcl > fanout_report.txt
```
