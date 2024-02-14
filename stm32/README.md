# STM32 WiFiLocationTracker

Still in progress of implementation

#### Todo

* [X] Communication with host-computer for debugging (UART)
* [X] Communication with LR1110 (SPI)
* [ ] Implement WiFi scanner
* [ ] Implement LoRaWAN
* [ ] Implement Satellite scanner

## Setup

Currently using a [NUCLEO-L476RG](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html) as the STM32-chip and [LR1110](https://www.semtech.com/products/wireless-rf/lora-edge/lr1110) for scanning WiFi AP's and satellites.

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

When using the Semtech LR1110 shield for STM32 Nucleo board is is also possible to use that shields with an Arduino Uno (see [arduino.ino](arduino/arduino.ino)).


The setup can be done in either STM32CubeIDE og Visual Studio Code.

##### STM32CubeIDE

Install:

* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

##### Visual Studio Code

Install:

* [Visual Studio Code](https://code.visualstudio.com/download)
* [STM32CubeMX](https://www.st.com/resource/en/user_manual/dm00104712-stm32cubemx-for-stm32-configuration-and-initialization-c-code-generation-stmicroelectronics.pdf)
* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

## Usage

##### STM32CubeIDE

1. Import the stm32-folder as a project
2. Click on Debug

##### Visual Studio Code

1. Open the stm32-folder in VSC
3. Click on **Run and Debug** and click on **Launch**
4. Another window pops up, click continue in the debug-menu (You can close the ned window now)

To receive UART-messages for e.g. debugging use in Linux `cu`

Check which Serial port the STM32 is connected to with `ls /dev/tty*` (should be something like `/dev/ttyACM0`).

View serial ouput: `cu -l /dev/ttyACM0 -s 115200`

## Troubleshooting

**Unable to start debugging. No process is associated with this object.**

1. Look inside **launch.json** for `miDebuggerServerAddress` and `debugServerArgs`, and not the number (e.g. 3333)
2. Open a terminal and run `sudo lsof -i :<NUMBER>`
3. Note the `PID` and run `sudo kill <PID>`
4. Check with step 2 again to determine if the process is gone (otherwise kill it again)
5. Try debugging again
