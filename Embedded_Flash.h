/*
 * Embedded_Flash.h
 *
 *  Created on: Jan 27, 2022
 *      Author: Mohamed.Hafez
*/

#ifndef		EMBEDDED_FLASH_H
#define		EMBEDDED_FLASH_H

#include "stm8l15x.h"
#include "stdint.h"

#define IAR_RAM_EXECUTION  (1) // to enable the FLASH functions execution from RAM for IAR compiler

#define FLASH_BLOCK_SIZE                           ((uint8_t)0x80)        /*!< Number of bytes in a Block*/
#define FLASH_PROGRAM_START_PHYSICAL_ADDRESS       ((uint32_t)0x00008000) /*!< Flash: start address */  
#define FLASH_PROGRAM_END_PHYSICAL_ADDRESS         ((uint32_t)0x00017FFF) /*!< Flash: end address */
#define OPERATION_TIMEOUT					 	               ((uint16_t)0xFFFF)
#define MASS_PROG_KEY_1							               (0x56)
#define MASS_PROG_KEY_2							               (0xAE)


typedef enum {
  FLASH_FLAG_HVOFF     = (uint8_t)0x40,     /*!< End of high voltage flag */
  FLASH_FLAG_DUL       = (uint8_t)0x08,     /*!< Data EEPROM unlocked flag */
  FLASH_FLAG_EOP       = (uint8_t)0x04,     /*!< End of programming (write or erase operation) flag */
  FLASH_FLAG_PUL       = (uint8_t)0x02,     /*!< Flash Program memory unlocked flag */
  FLASH_FLAG_WR_PG_DIS = (uint8_t)0x01      /*!< Write attempted to protected page flag */
}FLASH_FLAG_TypeDef;

typedef enum {
  FLASH_ProgramMode_Standard = (uint8_t)0x00, /*!< Standard programming mode */
  FLASH_ProgramMode_Fast     = (uint8_t)0x10  /*!< Fast programming mode */
}FLASH_ProgramMode_TypeDef;

typedef enum {
  FLASH_Status_Write_Protection_Error = (uint8_t)0x01, /*!< Write attempted to protected Block */
  FLASH_Status_TimeOut                = (uint8_t)0x02, /*!< Time out error */
  FLASH_Status_Successful_Operation   = (uint8_t)0x04  /*!< End of operation flag */
}FLASH_Status_TypeDef;

typedef enum {
 FLASH_MemType_Program    = (uint8_t)0xFD, /*!< Program memory */
 FLASH_MemType_Data      = (uint8_t)0xF7  /*!< Data EEPROM memory */
}FLASH_MemType_TypeDef;

void FLASH_UnlockPROG(void);
void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType);
__ramfunc void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer);
__ramfunc FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType);


#endif //EMBEDDED_FLASH_H