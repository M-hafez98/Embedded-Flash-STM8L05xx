#include "Embedded_Flash.h"



void FLASH_UnlockPROG(void) {
  FLASH->PUKR = MASS_PROG_KEY_1;
  FLASH->PUKR = MASS_PROG_KEY_2;
}
void FLASH_Lock(FLASH_MemType_TypeDef FLASH_MemType) {
	FLASH->IAPSR &= (uint8_t)FLASH_MemType;
}

__ramfunc void FLASH_ProgramBlock(uint16_t BlockNum, FLASH_MemType_TypeDef FLASH_MemType, FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer) {
  uint16_t Count = 0;
  uint32_t startaddress = 0;
  
  if(FLASH_MemType == FLASH_MemType_Program) {
    startaddress = FLASH_PROGRAM_START_PHYSICAL_ADDRESS;
  }
  else {
    startaddress = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS;
  }
  
  /* BlockNum = (FLASH_PROGRAM_START_PHYSICAL_ADDRESS - MAIN_USER_RESET_ADDR)/FLASH_BLOCK_SIZE */
  startaddress = startaddress + ((uint32_t)BlockNum * FLASH_BLOCK_SIZE);
  
  /* Selection of Standard or Fast programming mode */
  if(FLASH_ProgMode == FLASH_ProgramMode_Standard) {
    /* Standard programming mode */
    FLASH->CR2 |= FLASH_CR2_PRG;
  }
  else {
    /* Fast programming mode */
    FLASH->CR2 |= FLASH_CR2_FPRG;
  }
  
  /* Copy data bytes from RAM to FLASH memory sequentially */
  for(Count = 0; Count < FLASH_BLOCK_SIZE; Count++) {
    *((FAR uint8_t*) (uint32_t)startaddress + Count) = ((uint8_t)(Buffer[Count]));
  }
}

__ramfunc FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef FLASH_MemType)
{
  uint16_t timeout = OPERATION_TIMEOUT;
  uint8_t flagstatus = 0x00;
  
  /* Wait until operation completion or write protected page occurred */
  if(FLASH_MemType == FLASH_MemType_Program) {
    while((flagstatus == 0x00) && (timeout != 0x00)) {
      flagstatus = (uint8_t)(FLASH->IAPSR & (uint8_t)(FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS));
      timeout--;
    }
  }
  else {
    while((flagstatus == 0x00) && (timeout != 0x00)) {
      flagstatus = (uint8_t)(FLASH->IAPSR & (uint8_t)(FLASH_IAPSR_HVOFF | FLASH_IAPSR_WR_PG_DIS));
      timeout--;
    }
  }
  
  if(timeout == 0x00) {
    flagstatus = FLASH_Status_TimeOut;
  }
  
  return((FLASH_Status_TypeDef)flagstatus);
}