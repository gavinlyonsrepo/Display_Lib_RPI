/*! 
 * @file Bi_Color_Bitmap_Data.hpp
 * @brief  Data file to hold bi-color bitmap's and icon data
 * @author Gavin Lyons.
*/

#pragma once

#include <cstdint>

/*! Pointers to bitmap data which is in cpp file */
extern const uint8_t * pBackupMenuBitmap;    /**< Pointer to bitmap data Full screen bitmap 128X128 */
extern const uint8_t * pSunText;          /**< Pointer to bitmap data small image 'SUN' 40x96*/
extern const unsigned char * pPowerIcon;     /**< Pointer to icon data */
extern const unsigned char * pSpeedIcon;     /**< Pointer to icon data */
extern const unsigned char * pSignalIcon;    /**< Pointer to icon data */
extern const unsigned char * pMsgIcon;       /**< Pointer to icon data */
extern const unsigned char * pAlarmIcon;     /**< Pointer to icon data */
extern const unsigned char * pBatIcon;       /**< Pointer to icon data */

// ** EOF **
