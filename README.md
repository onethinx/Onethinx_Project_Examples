# Onethinx_Project_Examples
This folder contains several project examples for use with the Onethinx LoRa(WAN) module.

These projects combine the Visual Code setup for code editing with the magnificent PSoC Creator Schematic Editor & Design Wide Resources Editor for chip setup.

## Preparation
1. Download and install [PSoC Creator](https://www.infineon.com/cms/en/design-support/tools/sdk/psoc-software/psoc-creator/) (optionally)
    - Unfortunately PSoC Creator is Windows only. PSoC Creator is only used for chip setup and isn't needed for writing and debugging code.
    - The projects in this folder are pre-configured and don't need PSoC Creator to run.
1. Download and install the [Visual Studio Code IDE](https://code.visualstudio.com) 
1. Follow the instructions in the link to install the Onethinx Dependencies Pack:
    - For Windows: [VSCode_OnethinxPack_Windows](https://github.com/onethinx/VSCode_OnethinxPack_Windows)
    - For Linux: [VSCode_OnethinxPack_Linux](https://github.com/onethinx/VSCode_OnethinxPack_Linux)
    - For MacOS: [VSCode_OnethinxPack_MacOS](https://github.com/onethinx/VSCode_OnethinxPack_MacOS)
1. Clone or download the project to your local machine.
## Configure the Chip Setup (IOs, peripherals etc.) using PSoC Creator
  This step is only needed when you need to change the chip configuration.
1. Start PSoC Creator
1. Open the Onethinx_Creator.cywrk workspace(File >> Open >> Project/Workspace)
1. The PSoC Schematic Editor and the Design Wide Resources can be accessed at the left 'Workspace Explorer' pane:
<br>`..Onethinx_Project_Examples\Example_X\Onethinx_Creator.cydsn\Onethinx_Creator.cywrk`<br><br>
![PSoC_Creator_Config](https://github.com/onethinx/Onethinx_Project_Examples/blob/master/assets/PSoC_Creator_Config.png)
1. After the changes to the chip configuration are done, build the Onethinx Creator project (Build >> Build Onethinx_Creator or Shift + F5)
1. Make sure the project builds well and the Onethinx CoreStripper has run:<br>
![Onethinx CoreStripper](https://github.com/onethinx/Onethinx_Project_Examples/blob/master/assets/PSoC_Creator_Build.png)
1. The chip configuration is done and PSoC Creator may be closed.
## Write and debug the code
1. Start VS Code
1. Load the project by selecting the project folder: (File >> Open Folder):
<br>`..\Onethinx_Project_Examples\Example_HelloWorld`<br>
1. If your project has LoRaWAN keys:
    - Navigate to source/OnethinxCore/LoRaWAN_keys.h and fill in your LoRaWAN keys.
1. The following buttons in the statusbar can be used to configure, build and debug the project.
![VScode Buttons](https://github.com/onethinx/Onethinx_Project_Examples/blob/master/assets/VScode_buttons.png)<br>
1. Click "⚙︎ Build" in the status bar to build the project.
    * if it fails to build, try "Clean Reconfigure" + "Clean Rebuild" from the Cmake extension (left bar).
    * if it still fails, try deleting the contents of the 'build/' folder.
1. Debug the project using the (Cortex-) Debug extension from the left bar.
    * depending on your debug adapter select "CM4 Debug Kitprog" or "CM4 Debug J-Link" and press 'start'.

