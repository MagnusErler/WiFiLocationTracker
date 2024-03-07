#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_GNSS_CMD_LENGTH_NO_PARAM                 2
#define LR1110_GNSS_CMD_LENGTH_GET_GNSS_VERSION         LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_GET_GNSS_ALMANAC_STATUS  LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_SCAN_GNSS_AUTONOMOUS     LR1110_GNSS_CMD_LENGTH_NO_PARAM + 7
#define LR1110_GNSS_CMD_LENGTH_GET_NUMBER_OF_SATELLITES LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_GET_RESULTS              LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_GET_SATELLITES_DETECTED  LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_GET_CONSUMPTION          LR1110_GNSS_CMD_LENGTH_NO_PARAM
#define LR1110_GNSS_CMD_LENGTH_SET_CONSTELLATION        LR1110_GNSS_CMD_LENGTH_NO_PARAM + 1

// LENGTHS FOR RESPONSES
#define LR1110_GNSS_LENGTH_NO_PARAM                     1
#define LR1110_GNSS_LENGTH_GET_GNSS_VERSION             LR1110_GNSS_LENGTH_NO_PARAM + 2
#define LR1110_GNSS_LENGTH_GET_NUMBER_OF_SATELLITES     LR1110_GNSS_LENGTH_NO_PARAM + 1
#define LR1110_GNSS_LENGTH_GET_RESULTS                  LR1110_GNSS_LENGTH_NO_PARAM + 2
#define LR1110_GNSS_LENGTH_GET_SATELLITES_DETECTED      LR1110_GNSS_LENGTH_NO_PARAM + 1      // longer than 1 byte
#define LR1110_GNSS_LENGTH_GET_CONSUMPTION              LR1110_GNSS_LENGTH_NO_PARAM + 8
#define LR1110_GNSS_LENGTH_GET_GNSS_ALMANAC_STATUS      LR1110_GNSS_LENGTH_NO_PARAM + 52

// LR1110 CHIP COMMANDS
#define LR1110_GNSS_CMD_SET_CONSTELLATION               0x0400
#define LR1110_GNSS_CMD_GET_GNSS_VERSION                0x0406
#define LR1110_GNSS_CMD_SCAN_GNSS_AUTONOMOUS            0x0409
#define LR1110_GNSS_CMD_GET_RESULTS                     0x040D
#define LR1110_GNSS_CMD_GET_NUMBER_OF_SATELLITES        0x0417
#define LR1110_GNSS_CMD_GET_SATELLITES_DETECTED         0x0418
#define LR1110_GNSS_CMD_GET_CONSUMPTION                 0x0419
#define LR1110_GNSS_CMD_GET_GNSS_ALMANAC_STATUS         0x0457

/*!
 * @brief Scan for GNSS satellites
 *
 * @param [in] context Radio abstraction
 * 
 * @return Number of detected satellites
 */
uint8_t getLR1110_GNSS_Number_of_Detected_Satellites( const void* context );

/*!
 * @brief Scan for GNSS satellites
 *
 * @param [in] context Radio abstraction
 * @param [in] nb_of_satellites Number of detected satellites
 */
void getLR1110_GNSS_Detected_Satellites( const void* context, const uint8_t nb_of_satellites );

/*!
 * @brief Scan for GNSS satellites
 *
 * @param [in] context Radio abstraction
 * @param [in] effort_mode Search mode for GNSS scan
 * @param [in] result_mask Bit mask to select which results to include in the response
 * @param [in] nb_sv_max Maximum number of satellites to search for
 */
void scanLR1110_GNSS_Satellites( const void* context, const uint8_t effort_mode, const uint8_t result_mask, const uint8_t nb_sv_max );

/*!
 * @brief Get LR1110 GNSS version
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_GNSS_Version( const void* context);

/*!
 * @brief Reads the duration of the Radio capture and the CPU processing phases of the GNSS Scanning capture.
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_GNSS_Consumption( const void* context);

// /*!
//  * @brief Get the status of the GNSS Almanac
//  *
//  * @param [in] context Radio abstraction
//  */
// void getLR1110_GNSS_Almanac_Status( const void* context );

/*!
 * @brief Set the GNSS constellation to be used
 *
 * @param [in] context Radio abstraction
 * @param [in] constellation GNSS constellation to be used
 */
void setLR1110_GNSS_Constellation( const void* context, const uint8_t constellation );