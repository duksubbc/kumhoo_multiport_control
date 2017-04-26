
#define IR_GLOBAL

#include "includes.h"

#define ARRAY_SIZE(X)                (sizeof(X)/sizeof(X[0]))    


//#define IR_READY                0
//#define IR_LEADER_CODE          1
//#define IR_CUSTOM_CODE          2
//#define IR_CMD_CODE             3
//#define IR_UNPLUG               10

//#define DEBUG_IR

#define USE_EEPROM

#ifdef USE_EEPROM
__eeprom __no_init u8 _eeprom_ids[20] @ 0x4002;
__eeprom __no_init u8 _eeprom_idx     @ 0x4000;
#else
u8 _eeprom_ids[20] ;
u8 _eeprom_idx;
#endif

static IR_Status _currentState = IR_READY;
static u16 ir_count = 0;

static s16 ir_necStartUp = 0;
static u16 custom_code = 0;
static u16 irCmd = 0x00;
static int bit = 0;

#ifdef  DEBUG_IR 
static u16 ir_nec_up[64]=  {0};
#endif

void EEPROM_Unlock()
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
}

void EEPROM_Lock()
{
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

int __eeprom_wait_for_last_operation(void)
{
  FLASH_Status_TypeDef status = FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  return !!(status &(FLASH_STATUS_WRITE_PROTECTION_ERROR|FLASH_STATUS_SUCCESSFUL_OPERATION)); 
}

void __eeprom_program_byte(unsigned char __near *dst,unsigned char v)
{
  EEPROM_Unlock();
  FLASH_ProgramByte((u32)dst,(u8)v);
  EEPROM_Lock();
}

void __eeprom_program_long(unsigned char __near *dst,unsigned long v)
{
  EEPROM_Unlock();
  FLASH_ProgramWord((u32)dst,(u32)v);
  EEPROM_Lock();
}

u8 ir_update_ids(u8 idx)
{
  u8 j ;
  u8 low,hi;
  
  j = idx*2;
  
  low = (u8)(remote_ids[idx] & 0xff) ;
  hi =  (u8)((remote_ids[idx] >> 8 ) & 0xff);
  if(idx++ >= 9 )idx = 0;
  
  _eeprom_ids[j] = low;
  _eeprom_ids[j+1] = hi;
  _eeprom_idx = idx;
  
  return idx;
}

#if defined(BOARD_VER_170404_6) || defined(BOARD_VER_170404_1)
void do_ir_count(void)
{
   ir_count++;
   if(_currentState > IR_READY )
   {
     if(ir_count > 250) {
        ir_count = 0;
        _currentState = IR_READY;
     }
   }
}
#else
void do_ir_count(void)
{
   ir_count++;
   if(_currentState > IR_READY )
   {
     if(ir_count > 250) {
       if(IR_Rx_CHK == 0) {
         if(_currentState == IR_LEADER_CODE)
         {
            //check remove ir unplug         
           IRKey = KEY_IR_REMOVE;
           _currentState = IR_UNPLUG;
         }
       } else {
        ir_count = 0;
        _currentState = IR_READY;
       }
     }
   }
}
#endif

u16 get_ir_count(void)
{
   return ir_count;
}

void reset_ir_count(void)
{
   ir_count = 0;
}

void enable_ir(u8 enable)
{  
  return;
}

void Init_IR(void)
{
#ifdef USE_EEPROM
  u8 i , j;
  for(i = 0,j = 0; i < 10 ;i++,j+=2) {
    remote_ids[i] = _eeprom_ids[j] |  (_eeprom_ids[j+1] << 8) ;
  } 
  IRIDX = _eeprom_idx;
#endif
  
  _currentState = IR_READY;
  reset_ir_count();
  ir_necStartUp = 0;
  custom_code = 0;
  irCmd = 0x00;
  bit = 0;
  
  IRKey   = KEY_NONE;
  
  return;
}


u8 isValidIRID(u16 id)
{
  u8 i = 0;
  for(i = 0; i < 10 ;i++) {
    if(id == remote_ids[i])
      return 1;
  }
  
  return 0;
}

u8 isValidCmdIR(u16 cmd)
{
  u8 p1,p2;
  
  p1 = cmd >> 8 ;
  p2 = cmd & 0xff;
  
  if(((p1 & p2) == 0) && ((p1 | p2) == 0xff))
  {
    return 1;
  }
  else
    return 0;
}

void HandlerIrIrq()
{
  if(_currentState == IR_READY)
  {
    // 수신 신호
    if(IR_Rx_CHK == 0)
    { // next bit start
      //ir_count = 0;
      reset_ir_count();
      ir_necStartUp = -1;
      _currentState = IR_LEADER_CODE;
      irCmd = 0;
      custom_code = 0;
      bit = 0;
    }
    
  } else if(_currentState == IR_LEADER_CODE) {
    
    if(IR_Rx_CHK == 0) 
    {
      ir_necStartUp = get_ir_count();
      reset_ir_count();
            
      if((ir_necStartUp > 100 &&  ir_necStartUp < 150))
      {
        _currentState = IR_CUSTOM_CODE;
        //ir_count = 0;
        bit = 15;
      } else {
        _currentState = IR_READY;
        bit = 15;
      }
    }
  } else if(_currentState == IR_CUSTOM_CODE) {
    
      if(IR_Rx_CHK == 0) 
      {
        u8 value = 0;
        ir_necStartUp = get_ir_count();
        reset_ir_count();
        
        if(ir_necStartUp > 18 && ir_necStartUp < 25)
          value = 1;
        else 
          value = 0;
#ifdef  DEBUG_IR
        ir_nec_up[15-bit] =ir_necStartUp;
#endif
        custom_code |= value << bit--;
        
        if(bit < 0) {
          bit = 15;
          _currentState = IR_CMD_CODE;
          return;
        }
      }
   } else if(_currentState == IR_CMD_CODE) {

        u8 value = 0;
        ir_necStartUp = get_ir_count();
        reset_ir_count();
        
        if(ir_necStartUp > 18 &&  ir_necStartUp < 25)
          value = 1;
        else 
          value = 0;
#ifdef  DEBUG_IR
        ir_nec_up[31-bit] =ir_necStartUp;
#endif
        irCmd |= value << bit--;

        if(bit < 0 ) {
          bit = 15;
          _currentState = IR_READY;
          if(isValidCmdIR(irCmd) == 1) {
            IRKey = (irCmd >> 8);
            //if(IRKey == 0x4A)
            //IRKey = KEY_IR_PAIRKEY;
            IRID = custom_code;
          }
        }
  } 
}
