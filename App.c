#include "Embedded_Flash.h"

#define MAIN_USER_RESET_ADDR 	(0x009400)

uint8_t DataBuffer[FLASH_BLOCK_SIZE]; // holds the image as multiples of FLASH_BLOCK_SIZE
uint32_t  AppFlashAddress =  MAIN_USER_RESET_ADDR;
FLASH_Status_TypeDef FlashWriteResult = FLASH_Status_Write_Protection_Error;

void main (void) {
	FLASH_UnlockPROG();
	if((FLASH->IAPSR & FLASH_IAPSR_PUL) == FLASH_IAPSR_PUL) {
		do
		{
			/*this line has to load (reload) DataBuffer from external EEPROM or another serial peripheral before passing to FLASH_ProgramBlock API*/
			FLASH_ProgramBlock((TU16)((AppFlashAddress - FLASH_PROGRAM_START_PHYSICAL_ADDRESS)/FLASH_BLOCK_SIZE), FLASH_MemType_Program, FLASH_ProgramMode_Standard, DataBuffer);
       		FlashWriteResult = FLASH_WaitForLastOperation(FLASH_MemType_Program);
       		if(FlashWriteResult == FLASH_Status_Successful_Operation) {
       			AppFlashAddress += FLASH_BLOCK_SIZE;
       		}
		}while(FlashWriteResult == FLASH_Status_Successful_Operation &&\
			   AppFlashAddress <= FLASH_PROGRAM_END_PHYSICAL_ADDRESS);
	}
	if(AppFlashAddress > FLASH_PROGRAM_END_PHYSICAL_ADDRESS) {
		FLASH_Lock(FLASH_MemType_Program);
	}
}