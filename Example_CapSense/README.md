# Onethinx_Creator_CapSense
This example shows the use of CapSense with Onethinx LoRaWAN module.
## Description
Blue LED will turn on while joining. After the device is joined, it will go to sleep and wake up from it every 2 seconds to check if the CapSense button is being held. If the button is held for over 10 seconds total, it will send a message "Onethinx CapSense" over LoRaWAN.

## Hardware setup
1. Connect Cmod (a 2.2nF capacitor) to IO3 (P7.7/P10.2)
    - Remind that P7.7 and P10.2 are interconnected internally: if using the Cmod on P7.7, P10.2 has to be unconfigured and unconnected
1. Connect a 'capsense button' to P9.0 (for example a one cent coin soldered to a wire).
    - Capsense buttons may use a 560R series resistor to protect against static discharges.

