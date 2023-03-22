# AT Command example project
Library to use AT commands for the Onethinx Core LoRaWAN module<br><br>
![LoRa AT Commands wiring](https://github.com/onethinx/Readme_assets/blob/main/OTX_DevKit%20AT_commands.png?raw=true)<br/>

The following AT commands can be used with this AT commands firmware:

## System commands
| Command             | Response   | Description                               |
|---------------------|------------|-------------------------------------------|
| AT+RESET            | [info]     | Resets the module                         |
| AT+PING             | OK         | Ping                                      |
| AT+INFO             | [info]     | Show the info of the module               |
| AT+STATUS           | [status]   | Show the status of the module             |
| AT+UNLOCK           | OK         | Unlocks the module for RAW Radio commands |
| AT+SLEEPMODE [mode] | OK / Error | Set sleepmode                             |

## LoRaWAN commands
| Command             | Response   | Description                          |
|---------------------|------------|--------------------------------------|
| AT+SET_OTAA [keys]  | OK / Error | Set OTAA keys                        |
| AT+JOIN             | OK / Error | Join the LoRaWAN network             |
| AT+TX [data]        | OK / Error |                                      |
| AT+RX_LENGTH        | [length]   | Show amount of data in the RX buffer |
| AT+RX               | [data]     | Show received data                   |

## Radio LoRa / FSK commands
| Command                                     | Response       | Description                                  |
|---------------------------------------------|----------------|----------------------------------------------|
| AT+RADIO_SET_FREQUENCY [frequency]          | OK / Error     | Set Radio Frequency                          |
| AT+RADIO_SET_LORA_MODULATION [mod.settings] | OK / Error     | Sets the Radio LoRa Modulation configuration |
| AT+RADIO_SET_LORA_PACKET [packet.settings]  | OK / Error     | Sets the Radio LoRa Packet configuration.    |
| AT+RADIO_SET_FSK_MODULATION [mod.settings]  | OK / Error     | Sets the Radio FSK Modulation configuration  |
| AT+RADIO_SET_FSK_PACKET [packet.settings]   | OK / Error     | Sets the Radio FSK Packet configuration.     |
| AT+RADIO_TX [data]                          | OK / Error     | Send the [data]                              |
| AT+RADIO_RX [RX timeout in ms]              | RX:[data] / OK | Activates the receiver for [timeout ms]      |

##Connections:
| Module   | Function      | To                                                         |
|----------|---------------|------------------------------------------------------------|
| P9_0     | Module RX     | Host UART TX (TTL level)                                   |
| P9_1     | Module TX     | Host UART RX (TTL level)                                   |
| P0_4     | Module wakeup | Host UART TX (TTL level) or external wakeup (falling edge) |


## Notes
- All commands are terminated by "\n"
- Sleep IO: 
    - low: enter sleep mode
    - high: wake up
- UART settings: 
    - baudrate 19200
    - databits: 8
    - parity: N
    - stopbits: 1
- All data is in hexadecimal format.
- OTAA keys [keys] format:
    - [public network] [DevEUI] [AppEUI] [AppKey]
    - public network: 1 or 0 (1 for public network, 0 for private network)
    - DevEUI: hexadecimal values 8 byte DevEUI (16 characters)
    - AppEUI: hexadecimal values 8 byte AppEUI (16 characters)
    - AppKey: hexadecimal values 16 byte AppKey (32 characters)
- Sleep mode [mode] value:
    - 0: Hibernate
    - 1: Deep Sleep	
    - 2: Sleep
- Wakeup IO:
    - connect to external wakeup signal (voltage may not exceed module voltage) for wakeup on falling edge
    - connect to Host TX for wakeup on TX activity
    
# AT Command LoRa & FSK Settings Generator
Use [this tool](https://onethinx.com/AT_Command_SettingsGen) to generate the AT Command Settings for LoRa and FSK settings:<br><br>
<a href="https://onethinx.com/AT_Command_SettingsGen" rel="Onethinx">![AT_Command_SettingsGen](https://github.com/onethinx/Readme_assets/blob/main/AT_Command_SettingsGen.png)</a>
## Examples

- Set OTAA Keys
    - Public network: true (1)
    - DevEUI: DE01020304050607
    - AppEUI: AE01020304050607
    - AppKey: AA0102030405060708090A0B0C0D0E0F
   
    ```AT+SET_OTAA 1 DE01020304050607 AE01020304050607 AA0102030405060708090A0B0C0D0E0F```

- Send data over LoRaWAN
    - Data: "Hello World!" (hexadecimal = 48656c6c6f20576f726c6421)
    
    ```AT+TX 48656c6c6f20576f726c6421```

- Enter sleep mode
    - mode: Deep Sleep (1)
    
    ```AT+SLEEPMODE 1```
