# Onethinx_Creator_LoRa
This example presents LoRa communication. For this example to work you will need 2 Onexthinx DevKits.

The transmitting Devkit (TX), line 48 in main.c should be: #define MODULE_TX 1
The receiving Devkit (RX), line 48 in main.c should be: #define MODULE_TX 0

You can build the code and program each DevKit. If you press the button on the TX DevKit to change LED color, the RX DevKit LED will also change to match the TX DevKit, demonstrating LoRa communication between these two DevKits.
