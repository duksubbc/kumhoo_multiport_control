
#define PORTSCAN_GLOBAL

#include <stm8s.h>

#include "main.h"
#include "id.h"

#define ID_ADDRESS      0x4000

char Read_ID()
{
  return FLASH_ReadByte(ID_ADDRESS);
}

char Update_ID(char id)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(ID_ADDRESS,id);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  return FLASH_ReadByte(ID_ADDRESS);
}
