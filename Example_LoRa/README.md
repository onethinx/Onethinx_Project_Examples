# Onethinx LoRa To LoRa communication example
   
![LoRa to LoRa](https://github.com/onethinx/Readme_assets/blob/main/LoRa-to-LoRa.gif?raw=true)<br/>

This example presents LoRa to LoRa communication. 

For this example to work you will need 2 Onexthinx DevKits:
  - The transmitting Devkit (TX), line 48 in main.c should be: #define MODULE_TX 1
  - The receiving Devkit (RX), line 48 in main.c should be: #define MODULE_TX 0

You can build the code and program each DevKit. If you press the button on the TX DevKit to change LED color, the RX DevKit LED will also change to match the TX DevKit, demonstrating LoRa communication between these two DevKits.

The used API for sending and receiving data over LoRa is as follows:
  - This command should be used to transmit data over LoRa<br>
  `LoRa_TX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaTXbuffer, sizeof(LoRaTXbuffer), 3000);`
  - This command should be used to receive data over LoRa<br>
  `LoRa_RX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaRXbuffer, sizeof(LoRaRXbuffer), 30000);`
   
   The LoRaParams are holding the LoRa parameters (frequency, spreading factor etc).
