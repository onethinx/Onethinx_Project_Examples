# Onethinx_Creator_CapSense
This example shows the use of CapSense with Onethinx LoRaWAN module.
## Description
Blue LED will turn on while joining. After the device is joined, it will go to sleep and wake up from it every 2 seconds to check if the CapSense button is being held. If the button is held for over 10 seconds total, it will send a message "Onethinx CapSense" over LoRaWAN.

Uses 2.2nF capacitor on the IO_3 (which is the Cmod pin). CapSense sense element is a one cent coin soldered to a wire, a 560ohm resistor and the P9.0 of the DevKit.
