<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# Phase Sequence Detection of Three-Phase AC Supply


The AVR® DA family of microcontrollers are peripheral rich, high performance and low-power 8-bit microcontroller devices. The built-in Zero-Cross Detector (ZCD), high resolution Analog-to-Digital Converter (ADC), timer peripherals are essential for realizing phase sequence detection applications.

The application demonstrator showcases the phase sequence detection and phase loss detection on emulated three phase AC supply using AVR128DA64 microcontroller.

Phase Sequence Detection of Three-Phase AC Supply demonstration and implementation is discussed in the <a href="https://microchip.com/DS00003607" rel="nofollow">Application Note.

## Application Firmware

The demonstration is implemented with both AVR128DA64 and AVR128DA48 microcontrollers and it is available on microchip GitHub page-

* Using <a href="https://github.com/microchip-pic-avr-examples/avr128da64-phase-sequence-detection-of-ac-supply-studio" rel="nofollow">AVR128DA64
* Using <a href="https://github.com/microchip-pic-avr-examples/avr128da48-phase-sequence-detection-of-ac-supply-studio" rel="nofollow">AVR128DA48 

## Software Tools Used 

Microchip’s IDEs, compiler and graphical code generators are used throughout the application firmware development to provide an easy and hassle-free user experience. 
Following are the tools which are used for this demo application:
* <a href="https://www.microchip.com/mplab/avr-support/atmel-studio-7" rel="nofollow">Atmel Studio (v7.0.2397)
* <a href="https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers" rel="nofollow">AVR-GCC compiler (v5.4.0)
* <a href="https://www.microchip.com/mplab/avr-support/atmel-start" rel="nofollow">Atmel Start (v1.7.360)
* <a href="https://www.microchip.com/mplab/avr-support/data-visualizer" rel="nofollow">Standalone Data Visualizer (v2.20.674)

## Hardware Used

* <a href="https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164151" rel="nofollow">AVR128DA48 Curiosity Nano board
* <a href="https://www.microchip.com/developmenttools/ProductDetails/AC164162" rel="nofollow">Curiosity Nano base for click boards
* <a href="https://www.mikroe.com/waveform-click" rel="nofollow">Three waveform clicks 
* SMA male cable
* Berg connectors 
* <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/STK600-AVR-Flash-MCU-Starter-Kit-Users-Guide-DS40001904D.pdf" rel="nofollow">STK600
* Socket card <a href="https://www.microchip.com/Developmenttools/ProductDetails/ATSTK600-SC14" rel="nofollow">(STK600 TQFP64-2)
* Routing card (STK600-RC064AVR-108)
* USB cables 
  - Micro B USB cables
  - Type B USB connector
  
## Demonstration Setup

The following steps explains the procedure for setting up the hardware for three-phase sequence detection using AVR128DA64 microcontroller, and figure 1 shows the hardware setup used for the application demo.
* Insert AVR128DA64 microcontroller in the microcontroller slot of socket card.

**Note:** For proper orientation while inserting the microcontroller, refer the marking on AVR128DA64 microcontroller.

Connects AVR device to the STK600 using an innovative routing (STK600-RC064AVR-108) and socket (STK600-TQFP64-2) card sandwich system, which routes the i/o pins from the device to the appropriate hardware.

* Insert three waveform clicks in the three mikroBUS slots available on Curiosity nano base board.
* Connect SMA output of the waveform clicks to the respective microcontroller I/O pins using SMA connectors. Refer Application Note Appendix section for more information about the specific microcontroller I/O pins to be used.
* Detach waveform click 2 OEN pin from mikroBUS slot2 PWM2 pin.
* Detach waveform click 3 CS pin from mikroBUS slot3 CS3 pin.
* Connect PWM2 pin of mikroBUS slot2 to pin PB5, and CS3 pin of mikroBUS slot3 to pin PB4.
* Connect the curiosity nano board to a host computer (PC) using the standard micro B USB cable.
* Using female-female jumper wire, connect PC 7 pin (PORT C) of the STK600 board to SW0 pin (Switches) of the STK600 board.
* Using female-female jumper wire, connect VTG and Gnd pins of the STK600 board to VCC and GND pins of the curiosity nano adapter board, respectively.
* Connect UART 1 pins PC0 and PC1 (Tx and Rx) from STK600 board to PC through USB-UART convertor. 
* Connect USB Type B cable for powering the STK600 board.
* Press SW0 button on STK600 board for selecting appropriate frequency (50Hz/60Hz) for the emulated three-phase AC signal.  

<p align="center">
  <img width=750 height=auto src="images/DemoSetup.png">
  <br><strong>Figure 1: Demonstration Hardware Setup<br>
</p>

## Demonstration Operation 

The data visualizer tool is used as a graphical user interface while demonstrating the application by displaying the emulated three phase supply waveforms, output status and for receiving the user inputs.

Pre-configured data streamer file and custom dashboard file is available with the firmware package. The below steps given for data visualizer configuration uses these files to get the setup ready.

Here is the list of Data Visualizer modules used for Demonstration –
* Serial Port (Baud Rate – 230400)
* Graph 
* Custom Dashboard (Custom Dashboard file is given for the demonstration) 
 -	Element Label
 -	Element Signal
 -	Radio Group
* Data Streamer (Data Streamer file is given for the demonstration)
Refer Data Visualizer User Guide for more information about <a href="http://ww1.microchip.com/downloads/Secure/en/DeviceDoc/40001903B.pdf" rel="nofollow">Data Visualizer. 

**The following steps explains the procedure for demonstration after the demonstration setup is ready:** 

1.	Configure serial port for communication with the Curiosity nano board. 
2.	Configure data streamer for interfacing Graph window and Dashboard window with the serial port control panel. Data streamer configuration **(data-streamer-config.txt)** file is available in example firmware folder.
3.	Configure Graph window for displaying the emulated three phase supply AC waveforms, generated by the waveform clicks.
4.	Configure custom dashboard window to display the output and to receive user inputs. Custom dashboard **(custom-dashboard-config)** design file is available in example firmware folder. 
5.	The Blue labeled elements on custom dashboard are Inputs and Red labeled elements are Outputs.

**User inputs**
 
-	Recover three-phase supply from faults
-	Insert phase loss fault on L1 phase
-	Insert phase loss fault on L2 phase
-	Insert phase loss fault on L3 phase
-	Insert phase reversal (PR) fault

**Application Output Indicators**

-	Phase reversal status indicator
-	Phase loss status indicator
-	L1 Vrms, L2 Vrms, and L3 Vrms values
-	Operating Freq (Hz)
6.	Connect the hardware and prepare demonstrator setup as per the instructions provided in Demonstration setup section.
7.	Click Serial Port Control Panel Connect button.
8.	Using push button (SW0) on the STK600 board select appropriate frequency for the three-phase signal. The selected frequency is displayed on the custom dashboard window.
9.	Zoom in or Zoom out the graph on Graph window by clicking the +/- magnifier symbol below the Graph window.
10.	Insert phase loss fault by clicking the L1 radio button on dashboard, which disables the L1 signal out of three-phase signal to emulate phase loss input. 
11.	Insert phase loss fault by clicking the L2 radio button on dashboard, which disables the L2 signal out of three-phase signal to emulate phase loss input.
12.	Insert phase loss fault by clicking the L3 radio button on dashboard, which disables the L3 signal out of three-phase signal to emulate phase loss input.
13.	Insert phase reversal by clicking the PR radio button on dashboard, which changes the sequence of the three-phase signal to L1-L3-L2.

**NOTE:** For more details on application demonstration setup refer Phase Sequence Detection of Three-Phase AC Supply <a href="http://www.microchip.com" rel="nofollow">Application Note section 7. 

Figure 2 shows the Data Visualizer Dashboard for Phase Sequence Detection of Three-phase AC Supply. 

<p align="center">
  <img width=750 height=auto src="images/DashBoard.png">
  <br><strong>Figure 2: Data Visualizer Dashboard<br>
</p>

