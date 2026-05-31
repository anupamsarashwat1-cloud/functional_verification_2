# Static Timing Analysis (STA) Report
Target CPU: SMVDU TITAN-X (rv_core_top)
Standard Cell Library: OSU 180nm (Proxy for SCL 180nm)
Target Clock Frequency: 200 MHz (5.0 ns period)
Tool: Yosys + OpenSTA

## Summary
* Slack: VIOLATED (-6.45 ns)
* Data Arrival Time (Critical Path): 11.36 ns (equivalent to ~88 MHz maximum frequency in 180nm)
* Total Negative Slack (TNS): -747.39 ns
* Worst Negative Slack (WNS): -6.45 ns

## Critical Path Details
The critical path begins at a register inside the execution/decode stage (`_2839_`) and ends at another register (`_2888_`), passing through numerous combinational logic gates (INV, NAND3, OR2). 
The extremely deep logic depth, combined with the slow delays of the 180nm technology node, results in a total combinational delay of 11.36 ns. 

To achieve 200 MHz (5.0 ns):
1. The CPU pipeline (specifically the ALU, multipliers, and branch calculation logic) must be broken down into substantially more pipeline stages.
2. The current 180nm target frequency might need to be relaxed to ~100 MHz for this specific microarchitecture unless aggressive retiming is applied.
