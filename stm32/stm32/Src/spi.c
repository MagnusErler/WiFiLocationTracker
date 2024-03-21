/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : Containing all SPI functions
  ******************************************************************************
  */

#include "spi.h"

#include "led.h"
#include "main.h"       // for HAL_DBG_TRACE-functions
#include "lr1110.h"     // for getLR1110_Errors()

#include "helper.h"

#include <string.h>     // for memset

// VARIABLES FOR DEBUGGING
const bool _debug = true;
const bool _showStat1 = _debug; // Print out stat1 when sending commands   _debugStat1
const bool _showStat2 = _debug; // Print out stat2 when sending commands
const bool _printIRQ = _debug; // Print out extra data (if any) when sending commands

radio_t* radio;
void* _context;

lr1110_spi_status_t _waitForBusyState( const GPIO_PinState state, const uint32_t timeout_ms ) {

    uint32_t start = HAL_GetTick(); // Start of timeout measurement

    if( state == GPIO_PIN_RESET ) {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_SET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout (%dms) occured while waiting for BUSY to become LOW\r\n", timeout_ms);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
    } else {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_RESET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout (%dms) occured while waiting for BUSY to become HIGH\r\n", timeout_ms);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
    }
    return LR1110_SPI_STATUS_OK;
}

void printStat1(uint8_t stat1) {
    if(_showStat1) {
        HAL_DBG_TRACE_MSG_COLOR("\r\nStat1\r\n", HAL_DBG_TRACE_COLOR_YELLOW);
        HAL_DBG_TRACE_PRINTF("rbuffer[0] ");
        print_binary(stat1);
        HAL_DBG_TRACE_PRINTF("(0x%X)\r\n", stat1);

        // Extracting interrupt status (bit 0)
        unsigned char interruptStatus = (stat1 & 0x01) ? 1 : 0;
        
        // Printing interrupt status
        if (interruptStatus == 0) {
            HAL_DBG_TRACE_PRINTF("Interrupt Status: No interrupt active\r\n");
        } else {
            HAL_DBG_TRACE_PRINTF("Interrupt Status: At least 1 interrupt active\r\n");
        }
    }

    // Determining command status (bits 3:1) based on the value
    switch ((stat1 >> 1) & 0x07) {
        case 0:
            HAL_DBG_TRACE_ERROR("CMD_FAIL: The last command could not be executed\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_WARNING("CMD_PERR: The last command could not be processed (wrong opcode, arguments)\r\n");
            break;
        case 2:
            if (_showStat1) {
                HAL_DBG_TRACE_PRINTF("CMD_OK: The last command was processed successfully\r\n");
            }
            break;
        case 3:
            if (_showStat1) {
                HAL_DBG_TRACE_PRINTF("CMD_DAT: The last command was successfully processed, and data is currently transmitted instead of IRQ status\r\n");
            }
            break;
        default:
            HAL_DBG_TRACE_ERROR("Unknown command status received\r\n");
            break;
    }
}

void printStat2(uint8_t stat2) {
    HAL_DBG_TRACE_MSG_COLOR("\r\nStat2\r\n", HAL_DBG_TRACE_COLOR_YELLOW);
    HAL_DBG_TRACE_PRINTF("rbuffer[1] ");
    print_binary(stat2);
    HAL_DBG_TRACE_PRINTF("(0x%X)\r\n", stat2);

    // Printing reset source (bits 7-4)
    switch ((stat2 >> 4) & 0x0F) {
        case 0:
            HAL_DBG_TRACE_PRINTF("Reset Source: Cleared (no active reset)\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("Reset Source: Analog reset (Power On Reset, Brown-Out Reset)\r\n");
            break;
        case 2:
            HAL_DBG_TRACE_PRINTF("Reset Source: External reset (NRESET pin)\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_PRINTF("Reset Source: System reset\r\n");
            break;
        case 4:
            HAL_DBG_TRACE_PRINTF("Reset Source: Watchdog reset\r\n");
            break;
        case 5:
            HAL_DBG_TRACE_PRINTF("Reset Source: Wakeup NSS toggling\r\n");
            break;
        case 6:
            HAL_DBG_TRACE_PRINTF("Reset Source: RTC restart\r\n");
            break;
        case 7:
            HAL_DBG_TRACE_PRINTF("Reset Source: RFU\r\n");
            break;
        default:
            HAL_DBG_TRACE_ERROR("Unknown reset source received\r\n");
            break;
    }

    // Printing system mode (bits 3-1)
    switch ((stat2 >> 1) & 0x07) {
        case 0:
            HAL_DBG_TRACE_PRINTF("System Mode: Sleep\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("System Mode: Standby with RC Oscillator\r\n");
            break;
        case 2:
            HAL_DBG_TRACE_PRINTF("System Mode: Standby with external Oscillator\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_PRINTF("System Mode: FS\r\n");
            break;
        case 4:
            HAL_DBG_TRACE_PRINTF("System Mode: RX\r\n");
            break;
        case 5:
            HAL_DBG_TRACE_PRINTF("System Mode: TX\r\n");
            break;
        case 6:
            HAL_DBG_TRACE_PRINTF("System Mode: Wi-Fi og GNSS geolocation\r\n");
            break;
        case 7:
            HAL_DBG_TRACE_PRINTF("System Mode: RFU\r\n");
            break;
        default:
            HAL_DBG_TRACE_ERROR("Unknown system mode received\r\n");
            break;
    }

    // Printing execution source (bit 0)
    if ((stat2 & 0x01) == 0) {
        HAL_DBG_TRACE_PRINTF("Execution Source: Currently executing from boot-loader\r\n");
    } else {
        HAL_DBG_TRACE_PRINTF("Execution Source: Currently executing from flash\r\n");
    }
}

void printIrq(const uint8_t* buffer, const uint16_t buffer_length) {
    if (buffer_length < 3) {
        HAL_DBG_TRACE_PRINTF("Insufficient data to extract IRQ status\r\n");
        return;
    }

    // Calculate the number of bytes to read from the buffer. If >= 6 set to 6, else set to buffer_length
    uint16_t length_to_read = (buffer_length >= 6) ? 6 : buffer_length;

     // Extracting IRQ status bits
    uint32_t irq_status = 0;
    for (int i = 2; i < length_to_read; ++i) {
        irq_status |= (uint32_t)buffer[i] << (8 * (length_to_read - i - 1));
    }

    // Interpretations
    bool otherErrorDetected = false; //An error other than a command error occurred
    if (irq_status & ((1 << 31) | (1 << 30) | (1 << 29) | (1 << 26) | (1 << 18) | (1 << 17) | (1 << 16) | (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 1) | (1 << 0))) {
    HAL_DBG_TRACE_PRINTF("RFU\r\n");
    }
    if (irq_status & (1 << 28)) { HAL_DBG_TRACE_PRINTF("GnssAbort: Command GnssScan, GnssFetchTime, or GnssAlmanacUpdateFromSat aborted\r\n"); }
    if (irq_status & (1 << 27)) { HAL_DBG_TRACE_PRINTF("LoRaRxTimestamp: Last LoRa symbol received. To be used for time-stamping the received packet. The device is still in RX mode\r\n"); }
    if (irq_status & (1 << 25)) { HAL_DBG_TRACE_ERROR("FskAddrError: IRQ raised if the packet was received with an address error\r\n"); }
    if (irq_status & (1 << 24)) { HAL_DBG_TRACE_ERROR("FskLenError: IRQ raised if the packet was received with a length error\r\n"); }
    if (irq_status & (1 << 23)) { HAL_DBG_TRACE_ERROR("Error: An error other than a command error occurred (see GetErrors)\r\n");
        otherErrorDetected = true;
    }
    if (irq_status & (1 << 22)) { HAL_DBG_TRACE_ERROR("CmdError: Host command error\r\n"); }
    if (irq_status & (1 << 21)) { HAL_DBG_TRACE_PRINTF("LBD: Low Battery Detection\r\n"); }
    if (irq_status & (1 << 20)) { HAL_DBG_TRACE_PRINTF("WifiDone: Wi-Fi Scan finished\r\n"); }
    if (irq_status & (1 << 19)) { HAL_DBG_TRACE_PRINTF("GNSSDone: GNSS Scan finished\r\n"); }
    if (irq_status & (1 << 11)) { HAL_DBG_TRACE_PRINTF("LrFhssHop: LR-FHSS intra-packet hopping\r\n"); }
    if (irq_status & (1 << 10)) { HAL_DBG_TRACE_WARNING("Timeout: RX or TX timeout\r\n"); }
    if (irq_status & (1 << 9)) { HAL_DBG_TRACE_PRINTF("CadDetected: LoRa Channel activity detected\r\n"); }
    if (irq_status & (1 << 8)) { HAL_DBG_TRACE_PRINTF("CadDone: LoRa Channel activity detection finished\r\n"); }
    if (irq_status & (1 << 7)) { HAL_DBG_TRACE_ERROR("Err: LoRa: Wrong CRC received\r\n"); }
    if (irq_status & (1 << 6)) { HAL_DBG_TRACE_ERROR("HeaderErr: LoRa header CRC error\r\n"); }
    if (irq_status & (1 << 5)) { HAL_DBG_TRACE_PRINTF("Valid sync word / LoRa® header detected\r\n"); }
    if (irq_status & (1 << 4)) { HAL_DBG_TRACE_PRINTF("Preamble detected\r\n"); }
    if (irq_status & (1 << 3)) { HAL_DBG_TRACE_PRINTF("RxDone: Packet received\r\n"); }
    if (irq_status & (1 << 2)) { HAL_DBG_TRACE_PRINTF("TxDone: Packet transmission completed\r\n"); }

    if (otherErrorDetected) {
        HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
        getLR1110_Errors(_context);
        otherErrorDetected = false; // Reset the variable
    }
}

lr1110_spi_status_t _lr1110_spi_write( SPI_TypeDef* spi, const uint8_t* cbuffer, const uint16_t cbuffer_length, const uint32_t timeout_ms ) {

    uint8_t rbuffer[cbuffer_length]; // rbuffer always has the same length as cbuffer
    memset(rbuffer, 0x00, cbuffer_length); // Initialize rbuffer with 0x00

    // if (HAL_SPI_TransmitReceive( radio->hspi, ( uint8_t* ) cbuffer, rbuffer, cbuffer_length, timeout_ms ) != HAL_OK) {
    //     HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Error while transmitting and receiving data\r\n");
    //     return LR1110_SPI_STATUS_ERROR;
    // }

    for( uint16_t i = 0; i < cbuffer_length; i++ ) {
        uint32_t start = HAL_GetTick(); // Start of timeout measurement

        turnOnLED(TX_LED_GPIO_Port, TX_LED_Pin); // Turn on TX LED to show that the SPI is transmitting

        // Wait for TXE to become HIGH -> SPI is ready for transmission
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for transmission\r\n");
                turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        LL_SPI_TransmitData8( spi, cbuffer[i] ); // Transmit data

        turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin); // Turn off TX LED to show that the SPI is done transmitting
        turnOnLED(RX_LED_GPIO_Port, RX_LED_Pin); // Turn on RX LED to show that the SPI is receiving

        // Wait for RXNE to become HIGH -> SPI is ready for reception
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for reception\r\n");
                turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        rbuffer[i] = LL_SPI_ReceiveData8( spi ); // Receive data

        turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin); // Turn off RX LED to show that the SPI is done receiving
    }

    printStat1(rbuffer[0]);

    if((_showStat2 && (cbuffer_length > 1)) || (cbuffer[0] == 0x01 && cbuffer[1] == 0x00)) { // Print stat2 if debugging is enabled or getLR1110_Status cmd has been called (opcode 0x0100)
        printStat2(rbuffer[1]);
    }

    if(((cbuffer_length > 2) && _printIRQ) || (cbuffer[0] == 0x01 && cbuffer[1] == 0x00)) { // Print IRQ if debugging is enabled or getLR1110_Status cmd has been called (opcode 0x0100)
        HAL_DBG_TRACE_MSG_COLOR("\r\nIRQ Status\r\n", HAL_DBG_TRACE_COLOR_YELLOW);
        for (uint16_t i = 2; i < cbuffer_length; i++) {
            int start_bit = (cbuffer_length - i - 1) * 8;
            int end_bit = start_bit + 7;
            HAL_DBG_TRACE_PRINTF("rbuffer[%d] (%d:%d) ", i, end_bit, start_bit);
            print_binary(rbuffer[i]);
            HAL_DBG_TRACE_PRINTF("(0x%X)\r\n", rbuffer[i]);
        }

        printIrq(rbuffer, cbuffer_length);
    }

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t _lr1110_spi_read_with_dummy_byte( SPI_TypeDef* spi, uint8_t* rbuffer, const uint16_t rbuffer_length, const uint32_t timeout_ms ) {

    // uint8_t cbuffer[rbuffer_length-1];
    // memset(cbuffer, 0x00, rbuffer_length-1);

    // if (HAL_SPI_TransmitReceive( radio->hspi, cbuffer, rbuffer, rbuffer_length-1, timeout_ms ) != HAL_OK) {
    //     HAL_DBG_TRACE_ERROR("_lr1110_spi_read_with_dummy_byte(): Error while transmitting and receiving data\r\n");
    //     return LR1110_SPI_STATUS_ERROR;
    // }

    for( uint16_t i = 1; i < rbuffer_length; i++ ) {

        uint32_t start = HAL_GetTick( );

        turnOnLED(TX_LED_GPIO_Port, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_read_with_dummy_byte(): Timeout occured while waiting for SPI to become ready for transmission\r\n");
                turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        LL_SPI_TransmitData8( spi, 0x00 );
        turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin);

        turnOnLED(RX_LED_GPIO_Port, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_read_with_dummy_byte(): Timeout occured while waiting for SPI to become ready for reception\r\n");
                turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        rbuffer[i] = LL_SPI_ReceiveData8( spi );
        turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin);
    }

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t lr1110_spi_read( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, uint8_t* rbuffer, const uint16_t rbuffer_length ) {

    radio = (radio_t*) context;
    _context = (void*) context;

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR(" - Before 1st SPI transaction\r\n", HAL_DBG_TRACE_COLOR_RED);
        return LR1110_SPI_STATUS_ERROR;
    }

    // Start of 1st SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if(_showStat1 || _showStat2) {HAL_DBG_TRACE_MSG_COLOR("\r\nCOMMAND:", HAL_DBG_TRACE_COLOR_PURPLE);}
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR(" - After 1st (before 2nd) SPI transaction\r\n", HAL_DBG_TRACE_COLOR_RED);
        return LR1110_SPI_STATUS_ERROR;
    }
    // End of 1st SPI transaction

    // Start of 2nd SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if(_showStat1 || _showStat2) {HAL_DBG_TRACE_MSG_COLOR("\r\nRESPONSE:", HAL_DBG_TRACE_COLOR_PURPLE);}
    if (_lr1110_spi_write( radio->spi, 0x00, 1, 1000 ) != LR1110_SPI_STATUS_OK) {
        HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
        return LR1110_SPI_STATUS_ERROR;
    }
    if (_lr1110_spi_read_with_dummy_byte( radio->spi, rbuffer, rbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR(" - After 2nd SPI transaction\r\n", HAL_DBG_TRACE_COLOR_RED);
        return LR1110_SPI_STATUS_ERROR;
    }
    // End of 2nd SPI transaction

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t lr1110_spi_write( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length ) {

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 20000 ) != LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR(" - Before SPI transaction\r\n", HAL_DBG_TRACE_COLOR_RED);
        return LR1110_SPI_STATUS_TIMEOUT;
    }

    radio = (radio_t*) context;
    _context = (void*) context;

    // Start of SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if(_showStat1 || _showStat2) {HAL_DBG_TRACE_MSG_COLOR("\r\nCOMMAND:", HAL_DBG_TRACE_COLOR_PURPLE);}
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 20000 ) != LR1110_SPI_STATUS_OK) {
        HAL_DBG_TRACE_MSG_COLOR(" - After SPI transaction\r\n", HAL_DBG_TRACE_COLOR_RED);
        return LR1110_SPI_STATUS_TIMEOUT;
    }
    // End of SPI transaction
    
    return LR1110_SPI_STATUS_OK;
}