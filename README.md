# Onethinx_Project_Examples
This folder contains several project examples for use with the Onethinx LoRa(WAN) module.

These projects combine the Visual Code setup for code editing with the magnificent PSoC Creator Schematic Editor & Design Wide Resources Editor for chip setup.

## 1 Preparation
1. Download and install [PSoC Creator](https://www.infineon.com/cms/en/design-support/tools/sdk/psoc-software/psoc-creator/) (optionally)
    - Unfortunately PSoC Creator is Windows only. PSoC Creator is used for chip configuration only and isn't needed for writing and debugging code.
    - The projects in this folder are pre-configured and don't need PSoC Creator to run.
1. Download and install the [Visual Studio Code IDE](https://code.visualstudio.com) 
1. Follow the instructions in the link to install the Onethinx Dependencies Pack:
    - For Windows: [VSCode_OnethinxPack_Windows](https://github.com/onethinx/VSCode_OnethinxPack_Windows)
    - For Linux: [VSCode_OnethinxPack_Linux](https://github.com/onethinx/VSCode_OnethinxPack_Linux)
    - For MacOS: [VSCode_OnethinxPack_MacOS](https://github.com/onethinx/VSCode_OnethinxPack_MacOS)
1. Clone `https://github.com/onethinx/Onethinx_Project_Examples.git` or [download](https://github.com/onethinx/Onethinx_Project_Examples/archive/refs/heads/master.zip) and unpack the Example Projects to your local machine.

## 2 Configure the Chip Setup (IOs, peripherals etc.) using PSoC Creator
  This step is only needed when you need to change the chip configuration.
1. Start PSoC Creator
1. Open the Onethinx_Creator.cywrk workspace (File >> Open >> Project/Workspace)
1. The PSoC Schematic Editor and the Design Wide Resources can be accessed at the left 'Workspace Explorer' pane:
<br>`..Onethinx_Project_Examples\Example_X\Onethinx_Creator.cydsn\Onethinx_Creator.cywrk`<br><br>
![PSoC_Creator_Config](https://github.com/onethinx/Readme_assets/blob/main/PSoC_Creator_Config.png)
1. After the changes to the chip configuration are done, build the Onethinx Creator project (Build >> Build Onethinx_Creator or Shift + F5)
1. Make sure the project builds well from the output tab and the Onethinx CoreStripper has run:<br>
![Onethinx CoreStripper](https://github.com/onethinx/Readme_assets/blob/main/PSoC_Creator_Build.png)
1. The chip configuration is done and PSoC Creator may be closed.

## 3 Firmware Coding and Debugging (uses Visual Studio Code)
1. Start Visual Studio Code
1. Load the project by selecting the project folder (File >> Open Folder):
<br>For example: `....\Onethinx_Project_Examples\Example_HelloWorld`<br>
1. The following buttons in the statusbar can be used to configure, build and debug the project.
![VScode Buttons](https://github.com/onethinx/Readme_assets/blob/main/Clean_Build_Launch.png?raw=true)<br>
1. As the project has not run on the PC before, it needs to be Clean-Reconfigured before it can be built. Hit the `Clean-Reconfigure` button from the status bar at the bottom of VS Code<br>
1. After successfull configuration (the terminal window will show: "Terminal will be reused by tasks, press..." when ready), the project can be Built and Launched from the debugger. 
  Make sure the device is connected to the debugger and PC before launching the debug session<br>
  Hit the `Build-And-Launch` button from the status bar at the bottom of VS Code to start programming and debugging the code.
