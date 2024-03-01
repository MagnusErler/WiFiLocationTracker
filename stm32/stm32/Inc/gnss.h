#include <stdint.h>

// LENGTHS FOR COMMANDS
#define LR1110_CMD_NO_PARAM_LENGTH                  2
#define LR1110_GET_GNSS_VERSION_CMD_LENGTH          LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_GNSS_ALMANAC_STATUS_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_SCAN_GNSS_AUTONOMOUS_CMD_LENGTH      LR1110_CMD_NO_PARAM_LENGTH + 7
#define LR1110_GET_NUMBER_OF_SATELLITES_CMD_LENGTH  LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_RESULTS_CMD_LENGTH               LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_SATELLITES_DETECTED_CMD_LENGTH   LR1110_CMD_NO_PARAM_LENGTH
#define LR1110_GET_CONSUMPTION_CMD_LENGTH           LR1110_CMD_NO_PARAM_LENGTH

// LENGTHS FOR RESPONSES
#define LR1110_NO_PARAM_LENGTH                      1
#define LR1110_GET_GNSS_VERSION_LENGTH              LR1110_NO_PARAM_LENGTH + 2
#define LR1110_GET_NUMBER_OF_SATELLITES_LENGTH      LR1110_NO_PARAM_LENGTH + 1
#define LR1110_GET_RESULTS_LENGTH                   LR1110_NO_PARAM_LENGTH + 2
#define LR1110_GET_SATELLITES_DETECTED_LENGTH       LR1110_NO_PARAM_LENGTH + 1      // longer than 1 byte
#define LR1110_GET_CONSUMPTION_LENGTH               LR1110_NO_PARAM_LENGTH + 8
#define LR1110_GET_GNSS_ALMANAC_STATUS_LENGTH       LR1110_NO_PARAM_LENGTH + 52

// LR1110 CHIP COMMANDS
#define LR1110_GET_GNSS_VERSION_CMD                 0x0406
#define LR1110_SCAN_GNSS_AUTONOMOUS_CMD             0x0409
#define LR1110_GET_RESULTS_CMD                      0x040D
#define LR1110_GET_NUMBER_OF_SATELLITES_CMD         0x0417
#define LR1110_GET_SATELLITES_DETECTED_CMD          0x0418
#define LR1110_GET_CONSUMPTION_CMD                  0x0419
#define LR1110_GET_GNSS_ALMANAC_STATUS_CMD          0x0457

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
void getLR1110_GNSS_Detected_Satellites( const void* context, uint8_t nb_of_satellites );

/*!
 * @brief Scan for GNSS satellites
 *
 * @param [in] context Radio abstraction
 * @param [in] effort_mode Search mode for GNSS scan
 * @param [in] result_mask Bit mask to select which results to include in the response
 * @param [in] nb_sv_max Maximum number of satellites to search for
 */
void scanLR1110_GNSS_Satellites( const void* context, uint8_t effort_mode, uint8_t result_mask, uint8_t nb_sv_max );

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

/*!
 * @brief Get the status of the GNSS Almanac
 *
 * @param [in] context Radio abstraction
 */
void getLR1110_GNSS_Almanac_Status( const void* context );