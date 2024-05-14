/*!
 * @file      lr1110_modem_helper.c
 *
 * @brief     helper functions implementation for LR1110 modem
 *
 * Revised BSD License
 * Copyright Semtech Corporation 2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr1110_modem_helper.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*!
 * @brief Offset for constellation ID
 */
#define LR1110_MODEM_HELPER_GNSS_ALMANAC_SV_CONSTELLATION_ID_BUFFER_LOCATION ( 21 )

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

lr1110_modem_response_code_t lr1110_modem_helper_get_utc_time( const void* context, uint32_t* time )
{
    lr1110_modem_response_code_t rc;
    uint32_t                     gps_time;

    *time = 0;

    rc = lr1110_modem_get_gps_time( context, &gps_time );

    if( ( rc == LR1110_MODEM_RESPONSE_CODE_OK ) && ( gps_time != 0 ) )
    {
        *time =
            gps_time + ( LR1110_MODEM_HELPER_INITAL_GNSS_DATE_SECONDS - LR1110_MODEM_HELPER_GNSS_LEAP_SECONDS_OFFSET );
    }

    return rc;
}

lr1110_modem_helper_status_t lr1110_modem_helper_get_event_data( const void*           context,
                                                                 lr1110_modem_event_t* modem_event )
{
    lr1110_modem_helper_status_t status              = LR1110_MODEM_HELPER_STATUS_ERROR;
    lr1110_modem_response_code_t modem_response_code = LR1110_MODEM_RESPONSE_CODE_OK;
    lr1110_modem_event_fields_t  event_fields;

    modem_response_code = lr1110_modem_get_event( context, &event_fields );

    if( modem_response_code == LR1110_MODEM_RESPONSE_CODE_OK )
    {
        status                     = LR1110_MODEM_HELPER_STATUS_OK;
        modem_event->event_type    = event_fields.event_type;
        modem_event->missed_events = event_fields.missed_events_count;

        switch( modem_event->event_type )
        {
        case LR1110_MODEM_LORAWAN_EVENT_RESET:
            modem_event->event_data.reset.count = ( ( uint16_t ) event_fields.buffer[0] << 8 ) + event_fields.buffer[1];
            break;
        case LR1110_MODEM_LORAWAN_EVENT_TX_DONE:
            modem_event->event_data.txdone.status = ( lr1110_modem_tx_done_event_t ) event_fields.buffer[0];
            break;
        case LR1110_MODEM_LORAWAN_EVENT_DOWN_DATA:
            modem_event->event_data.downdata.rssi  = ( ( int8_t ) event_fields.buffer[0] ) - 64;
            modem_event->event_data.downdata.snr   = ( ( ( int8_t ) event_fields.buffer[1] ) >> 2 );
            modem_event->event_data.downdata.flag  = ( lr1110_modem_down_data_flag_t ) event_fields.buffer[2];
            modem_event->event_data.downdata.fport = event_fields.buffer[3];
            modem_event->event_data.downdata.length =
                event_fields.buffer_len - 4;  // remove rssi/snr/flag and fport from buffer
            for( uint8_t i = 0; i < modem_event->event_data.downdata.length; i++ )
            {
                modem_event->event_data.downdata.data[i] = event_fields.buffer[i + 4];
            }
            break;
        case LR1110_MODEM_LORAWAN_EVENT_UPLOAD_DONE:
            modem_event->event_data.upload.status = ( lr1110_modem_upload_event_t ) event_fields.buffer[0];
            break;
        case LR1110_MODEM_LORAWAN_EVENT_SET_CONF:
            modem_event->event_data.setconf.tag = ( lr1110_modem_event_setconf_tag_t ) event_fields.buffer[0];
            break;
        case LR1110_MODEM_LORAWAN_EVENT_MUTE:
            modem_event->event_data.mute.status = ( lr1110_modem_mute_t ) event_fields.buffer[0];
            break;
        case LR1110_MODEM_LORAWAN_EVENT_WIFI_SCAN_DONE:
            modem_event->event_data.wifi.len = event_fields.buffer_len;
            for( uint16_t i = 0; i < event_fields.buffer_len; i++ )
            {
                modem_event->event_data.wifi.buffer[i] = event_fields.buffer[i];
            }
            break;
        case LR1110_MODEM_LORAWAN_EVENT_GNSS_SCAN_DONE:
            modem_event->event_data.gnss.len = event_fields.buffer_len;
            for( uint16_t i = 0; i < event_fields.buffer_len; i++ )
            {
                modem_event->event_data.gnss.nav_message[i] = event_fields.buffer[i];
            }
            break;
        case LR1110_MODEM_LORAWAN_EVENT_TIME_UPDATED_ALC_SYNC:
            modem_event->event_data.time.status = ( lr1110_modem_alc_sync_state_t ) event_fields.buffer[0];
            break;
        default:
            break;
        }
    }

    return status;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/* --- EOF ------------------------------------------------------------------ */
