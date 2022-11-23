# Onethinx_Creator_HelloWorld
This basic example shows how to send a "Hello World" text over LoRaWAN.
## Description
The red LED will flash with the rate defined in the PSoC Creator project.
The blue LED will first be on during the join procedure.
If successfully joined, the blue LED will turn off for one second. 
A loop where the char-values of 'Hello World' will be sent every 10s is then entered.
If not joined -this may take a long while-, the blue LED will blink fast forever.