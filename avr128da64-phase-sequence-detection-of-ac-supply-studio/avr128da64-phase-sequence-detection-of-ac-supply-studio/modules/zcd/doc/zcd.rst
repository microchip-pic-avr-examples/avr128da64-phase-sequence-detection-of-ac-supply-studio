===========
ZCD driver
===========
The Zero-Cross Detector (ZCD) detects when an alternating voltage crosses through a threshold voltage
near ground potential.The connection from the ZCD input pin (ZCIN) to the alternating voltage must be made through a series
current-limiting resistor (RSERIES). When the applied voltage is greater than the reference voltage, the ZCD
sinks current. When the applied voltage is less than the reference voltage, the ZCD sources current. 

Features
--------
* Initialization

Applications
------------
* Detects the zero crossings of an input alternating voltage by giving output high. 

Dependencies
------------
* CLKCTRL/CLK for clock
* CPUINT/PMIC for Interrupt
* PORT for I/O lines and connections

Note
----
* ISR will be generated only when Global Interrupt checkbox and driver_isr Harness checkbox are enabled

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A