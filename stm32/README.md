# STM32WiFiLocationTracker

Still in progress of implementation

#### Todo

* [X] Communication with host-computer for debugging (UART)
* [ ] Communication with LR1110 (SPI)
* [ ] Implement WiFi scanner
* [ ] Implement LoRaWAN
* [ ] Implement Satellite scanner

## Setup

Install [STM32CubeMX](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-for-stm32-configuration-and-initialization-c-code-generation-stmicroelectronics.pdf) and [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

Currently using a [NUCLEO-L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html) as the stm32-chip and [LR1110](https://www.semtech.com/products/wireless-rf/lora-edge/lr1110) for scanning WiFI AP's and satellite

**LR1110**

* LoRa RF Transceiver
* GNSS scanner (GPS and BeiDou)

  * ~8s
  * Satellite data and timing
  * ~35 to 50 bytes
* WiFi scanner (2.4 GHz)

  * Access points and RSSI, # of AP
  * ~28 to 112 bytes
* GNSS and WiFi scan data is collected on the device and sent to a cloud-based solver to resolve into a position
* Semtech provide a Cloud API (fees apply)

## Usage
