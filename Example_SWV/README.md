# OTX_SWV_Example
Sample project to demonstrate SWV debugging with the Onethinx module using the JLink debugger.

Single Wire Viewer is an ARM implementation to send debug messages over a single wire (SWO) with a high datarate (usually at several Mbps).

Unfortunately, the Onethinx Module doesn't have the SWO pin routed for use outside the module. This can be overcome by using a separate UART configured at 6Mbps and emulating the ARM SWV protocol.


![OTX_SWV_Example](../assets/SWO_Example.png?raw=true)<br/>
_SWV Example: sending debug info to VS Code output window_

1. Prerequisites
    - Onethinx LoRaWAN Core Module
    - J-Jink debugger
    - Visual Studio Code
    - Onethinx Core VS Code dependencies pack (compiler, Cypress tools, Cypress PDL, linkersettings etc.)
      - [VSCode_OnethinxPack_macOS](https://github.com/onethinx/VSCode_OnethinxPack_macOS)
      - [VSCode_OnethinxPack_Windows](https://github.com/onethinx/VSCode_OnethinxPack_Windows)
      - [VSCode_OnethinxPack_Linux](https://github.com/onethinx/VSCode_OnethinxPack_Linux)
    - PSoC Creator 4.4 (when reconfiguration of the UART / IO is required).

2. Hardware connection setup.
   - P9_0 should be connected with pin 6 (SWO / TDO) of the 10 pin ARM SWD connector of the J-Link debugger (J-Link Adapter CortexM)<br/><br/>
   - ![ARM_10_pin](../assets/ARM_10pin.png?raw=true)<br/> _ARM SWD 10 pin connector_<br/><br/>
   - ![ARM_10_pin](../assets/OTX_DevKit.png?raw=true)<br/> _Onethinx Dev Kit_<br/><br/>
   
3. Building the example
   - Clean Reconfigure All Projects<br/>
   ![Clean Reconfigure](../assets/Debug_info%201.png?raw=true)<br/>
     *if you get errors, try to delete the contents of the build/ folder*<br/><br/>
   - Build & Launch<br/>
   ![Build & Launch](../assets/Debug_info%202.png?raw=true)
   

   
4. Adapting the hardware configuration (SWO pin selection, UART baudrate etc.)
   - Start PSoC Creator and open \[Project Folder\]/Onethinx_Creator.cydsn/Onethinx_Creator.cywrk
   - Modify your configuration accordingly and do a clean-rebuild
   - Go back to VS Code and do a \[Clean Reconfigure\] and \[Build & Launch\] (see steps above).
  
![OTX_SWV_Example](../assets/SWV_UART.png?raw=true)<br/>
_6 MBaud UART Example in PSoC Creator: double click and change settings_

