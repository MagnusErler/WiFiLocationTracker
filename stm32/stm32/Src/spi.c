/**
  ******************************************************************************
  * @file           : spi.c
  * @brief          : Containing all SPI functions
  ******************************************************************************
  */

#include "spi.h"

#include "led.h"
#include "main.h"       // for HAL_DBG_TRACE-functions
#include "lr1110.h"     // for getErrors()

#include "helper.h"

#include <string.h>     // for memset

// Comment out the following line to disable debug messages
//#define DEBUG

radio_t* radio;
void* context;

lr1110_spi_status_t _waitForBusyState( GPIO_PinState state, uint32_t timeout_ms ) {

    uint32_t start = HAL_GetTick();

    if( state == GPIO_PIN_RESET ) {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_SET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout occured while waiting for BUSY to become LOW\r\n");
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
    } else {
        while( HAL_GPIO_ReadPin( radio->busy.port, radio->busy.pin ) == GPIO_PIN_RESET ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("Timeout occured while waiting for BUSY to become HIGH\r\n");
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
    }
    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t _lr1110_spi_write( SPI_TypeDef* spi, const uint8_t* cbuffer, const uint16_t cbuffer_length, const uint32_t timeout_ms, const bool get_status ) {

    uint8_t rbuffer[cbuffer_length];
    memset(rbuffer, 0x00, cbuffer_length);

    // if (HAL_SPI_TransmitReceive( radio->hspi, ( uint8_t* ) cbuffer, rbuffer, cbuffer_length, timeout_ms ) != HAL_OK) {
    //     HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Error while transmitting and receiving data\r\n");
    //     return LR1110_SPI_STATUS_ERROR;
    // }

    for( uint16_t i = 0; i < cbuffer_length; i++ ) {
        uint32_t start = HAL_GetTick();

        turnOnLED(TX_LED_GPIO_Port, TX_LED_Pin);
        while( LL_SPI_IsActiveFlag_TXE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for transmission\r\n");
                turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        LL_SPI_TransmitData8( spi, cbuffer[i] );
        turnOffLED(TX_LED_GPIO_Port, TX_LED_Pin);

        turnOnLED(RX_LED_GPIO_Port, RX_LED_Pin);
        while( LL_SPI_IsActiveFlag_RXNE( spi ) == 0 ) {
            if( ( uint32_t )( HAL_GetTick() - start ) > ( uint32_t ) timeout_ms ) {
                HAL_DBG_TRACE_PRINTF("\r\n");
                HAL_DBG_TRACE_ERROR("_lr1110_spi_write(): Timeout occured while waiting for SPI to become ready for reception\r\n");
                turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin);
                return LR1110_SPI_STATUS_TIMEOUT;
            }
        };
        rbuffer[i] = LL_SPI_ReceiveData8( spi );
        turnOffLED(RX_LED_GPIO_Port, RX_LED_Pin);
    }

    #ifdef DEBUG
    HAL_DBG_TRACE_PRINTF("\r\n---Stat1 Results---\r\n");
    HAL_DBG_TRACE_PRINTF("rbuffer[0]: ");
    print_binary(rbuffer[0]);
    HAL_DBG_TRACE_PRINTF("\r\n");
    switch ( rbuffer[0] & 0x01 ) {
        case 0:
            HAL_DBG_TRACE_PRINTF("No interrupt active\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("At least 1 interrupt active\r\n");
            break;
    }
    #endif

    switch( ( rbuffer[0] & 0x0E ) >> 1 ) {
        case 0:
            HAL_DBG_TRACE_ERROR("CMD_FAIL: The last command could not be executed\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_WARNING("CMD_PERR: The last command could not be processed (wrong opcode, arguments). It is possible to generate an interrupt on DIO if a command error occurred\r\n");
            break;
        #ifdef DEBUG
        case 2:
            HAL_DBG_TRACE_PRINTF("CMD_OK: The last command was processed successfully\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_PRINTF("CMD_DAT: The last command was successfully processed, and data is currently transmitted instead of IRQ status\r\n");
            break;
        #endif
    }

    #ifdef DEBUG
    HAL_DBG_TRACE_PRINTF("\r\n---Stat2 Results---\r\n");
    HAL_DBG_TRACE_PRINTF("rbuffer[1]: ");
    print_binary(rbuffer[1]);
    HAL_DBG_TRACE_PRINTF("\r\n");
    switch ( rbuffer[1] & 0x01 ) {
        case 0:
            HAL_DBG_TRACE_PRINTF("Bootloader: currently executes from boot-loader\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("Bootloader: currently executes from flash. The ResetStatus field is cleared on the first GetStatus() command after a reset. It is not cleared by any other command\r\n");
            break;
    }
    #endif

    #ifdef DEBUG
    switch( ( rbuffer[1] & 0x0E ) >> 1 ) {
        case 0:
            HAL_DBG_TRACE_PRINTF("Chip mode: Sleep\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("Chip mode: Standby with RC Oscillator\r\n");
            break;
        case 2:
            HAL_DBG_TRACE_PRINTF("Chip mode: Standby with external Oscillator\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_PRINTF("Chip mode: FS\r\n");
            break;
        case 4:
            HAL_DBG_TRACE_PRINTF("Chip mode: RX\r\n");
            break;
        case 5:
            HAL_DBG_TRACE_PRINTF("Chip mode: TX\r\n");
            break;
        case 6:
            HAL_DBG_TRACE_PRINTF("Chip mode: WiFi or GNSS geolocation\r\n");
            break;
    }
    #endif

    #ifdef DEBUG
    switch( ( rbuffer[1] & 0x70 ) >> 4 ) {
        case 0:
            HAL_DBG_TRACE_PRINTF("Reset status: Cleared (no active reset)\r\n");
            break;
        case 1:
            HAL_DBG_TRACE_PRINTF("Reset status: Analog reset (Power On Reset, Brown-Out Reset)\r\n");
            break;
        case 2:
            HAL_DBG_TRACE_PRINTF("Reset status: External reset (NRESET pin)\r\n");
            break;
        case 3:
            HAL_DBG_TRACE_PRINTF("Reset status: System reset\r\n");
            break;
        case 4:
            HAL_DBG_TRACE_PRINTF("Reset status: Watchdog reset\r\n");
            break;
        case 5:
            HAL_DBG_TRACE_PRINTF("Reset status: Wakeup NSS toggling\r\n");
            break;
        case 6:
            HAL_DBG_TRACE_PRINTF("Reset status: RTC restart\r\n");
            break;
    }
    #endif

    #if defined(DEBUG) || defined(get_status)
        #define BIT_0 0b00000001
        #define BIT_1 0b00000010
        #define BIT_8 0b00000100
        #define BIT_12 0b00001000
        #define BIT_16 0b00010000
        #define BIT_20 0b00100000
        #define BIT_24 0b01000000
        #define BIT_28 0b10000000

        HAL_DBG_TRACE_PRINTF("\r\n---Irq Status Results---\r\n");
        // HAL_DBG_TRACE_PRINTF("IRQStat(31:24): 0x%X\r\n", rbuffer[2]);
        // HAL_DBG_TRACE_PRINTF("IRQStat(23:16): 0x%X\r\n", rbuffer[3]);
        // HAL_DBG_TRACE_PRINTF("IRQStat(15:8): 0x%X\r\n", rbuffer[4]);
        // HAL_DBG_TRACE_PRINTF("IRQStat(7:0): 0x%X\r\n", rbuffer[5]);

        HAL_DBG_TRACE_PRINTF("rbuffer[2] (31:24): ");
        print_binary(rbuffer[2]);
        HAL_DBG_TRACE_PRINTF("\r\n");
        if (rbuffer[2] & BIT_0) {
            HAL_DBG_TRACE_PRINTF("FskLenError (IRQ raised if the packet was received with a length error)\r\n");
        }
        if (rbuffer[2] & BIT_1) {
            HAL_DBG_TRACE_PRINTF("FskAddrError (IRQ raised if the packet was received with an address error)\r\n");
        }
        if (rbuffer[2] & BIT_8) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[2] & BIT_12) {
            HAL_DBG_TRACE_PRINTF("LoRaRxTimestamp (Last LoRa symbol received. To be used for time-stamping the received packet. The device is still in RX mode)\r\n");
        }
        if (rbuffer[2] & BIT_16) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[2] & BIT_20) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[2] & BIT_24) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[2] & BIT_28) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }

        HAL_DBG_TRACE_PRINTF("\r\nrbuffer[3] (23:16): ");
        print_binary(rbuffer[3]);
        HAL_DBG_TRACE_PRINTF("\r\n");
        if (rbuffer[3] & BIT_0) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[3] & BIT_1) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[3] & BIT_8) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[3] & BIT_12) {
            HAL_DBG_TRACE_PRINTF("GNSSDone (GNSS Scan finished)\r\n");
        }
        if (rbuffer[3] & BIT_16) {
            HAL_DBG_TRACE_PRINTF("WifiDone (Wi-Fi Scan finished)\r\n");
        }
        if (rbuffer[3] & BIT_20) {
            HAL_DBG_TRACE_PRINTF("LBD (Low Battery Detection)\r\n");
        }
        if (rbuffer[3] & BIT_24) {
            HAL_DBG_TRACE_ERROR("CmdError (Host command error)\r\n");
        }
        if (rbuffer[3] & BIT_28) {
            HAL_DBG_TRACE_ERROR("Error (An error other than a command error occurred (see GetErrors))\r\n");
            //getErrors(context);
        }

        HAL_DBG_TRACE_PRINTF("\r\nrbuffer[4] (15:8): ");
        print_binary(rbuffer[4]);
        HAL_DBG_TRACE_PRINTF("\r\n");
        if (rbuffer[4] & BIT_0) {
            HAL_DBG_TRACE_PRINTF("CadDone (LoRa Channel activity detection finished)\r\n");
        }
        if (rbuffer[4] & BIT_1) {
            HAL_DBG_TRACE_PRINTF("CadDetected (LoRa Channel activity detected)\r\n");
        }
        if (rbuffer[4] & BIT_8) {
            HAL_DBG_TRACE_WARNING("Timeout (RX or TX timeout)\r\n");
        }
        if (rbuffer[4] & BIT_12) {
            HAL_DBG_TRACE_PRINTF("LrFhssHop (LR-FHSS intra-packet hopping)\r\n");
        }
        if (rbuffer[4] & BIT_16) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[4] & BIT_20) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[4] & BIT_24) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[4] & BIT_28) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }

        HAL_DBG_TRACE_PRINTF("\r\nrbuffer[5] (7:0): ");
        print_binary(rbuffer[5]);
        HAL_DBG_TRACE_PRINTF("\r\n");
        if (rbuffer[5] & BIT_0) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[5] & BIT_1) {
            HAL_DBG_TRACE_PRINTF("RFU\r\n");
        }
        if (rbuffer[5] & BIT_8) {
            HAL_DBG_TRACE_PRINTF("TxDone (Packet transmission completed)\r\n");
        }
        if (rbuffer[5] & BIT_12) {
            HAL_DBG_TRACE_PRINTF("RxDone (Packet received)\r\n");
        }
        if (rbuffer[5] & BIT_16) {
            HAL_DBG_TRACE_PRINTF("PreambleDetected (Preamble detected)\r\n");
        }
        if (rbuffer[5] & BIT_20) {
            HAL_DBG_TRACE_PRINTF("SyncWordValid / HeaderValid (Valid sync word / LoRa header detected)\r\n");
        }
        if (rbuffer[5] & BIT_24) {
            HAL_DBG_TRACE_ERROR("HeaderErr (LoRa header CRC error)\r\n");
        }
        if (rbuffer[5] & BIT_28) {
            HAL_DBG_TRACE_ERROR("Err (Packet received with error. LoRa: Wrong CRC received)\r\n");
        }
    #endif

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t _lr1110_spi_read_with_dummy_byte( SPI_TypeDef* spi, uint8_t* rbuffer, uint16_t rbuffer_length, uint32_t timeout_ms ) {

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

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }

    // Start of 1st SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000, false ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    // End of 1st SPI transaction

    // Start of 2nd SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, 0x00, 1, 1000, false ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    if (_lr1110_spi_read_with_dummy_byte( radio->spi, rbuffer, rbuffer_length, 1000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    // End of 2nd SPI transaction

    return LR1110_SPI_STATUS_OK;
}

lr1110_spi_status_t lr1110_spi_write( const void* context, const uint8_t* cbuffer, const uint16_t cbuffer_length, const bool get_status ) {

    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_TIMEOUT;
    }

    radio = (radio_t*) context;

    // Start of SPI transaction
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_RESET );
    if (_lr1110_spi_write( radio->spi, cbuffer, cbuffer_length, 1000, get_status ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_ERROR;
    }
    HAL_GPIO_WritePin( radio->nss.port, radio->nss.pin, GPIO_PIN_SET );
    // Wait for BUSY to become LOW -> LR1110 is ready for a new command
    if (_waitForBusyState( GPIO_PIN_RESET, 2000 ) != LR1110_SPI_STATUS_OK) {
        return LR1110_SPI_STATUS_TIMEOUT;
    }
    // End of SPI transaction
    
    return LR1110_SPI_STATUS_OK;
}