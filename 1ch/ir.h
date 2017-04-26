#ifndef ___IR_H___
#define ___IR_H___

#ifdef IR_GLOBAL
	#define IR_EXT
#else
	#define IR_EXT extern
#endif

// variable definition


typedef enum {
  IR_UNPLUG,
  IR_PLUG,
} irPlugStatus;

typedef enum {
  //IR_UNPLUG,
  IR_READY,
  IR_LEADER_CODE,
  IR_CUSTOM_CODE,
  IR_CMD_CODE
} IR_Status;

// variable definition
IR_EXT irPlugStatus mIrPlugStatus;

IR_EXT u8  IRIDX;
IR_EXT u8  IRKey;
IR_EXT u16 IRID;

IR_EXT u16 remote_ids[10];
IR_EXT u8 isValidIRID(u16 id);
IR_EXT u8 ir_update_ids(u8 idx);
IR_EXT void Init_IR(void);
IR_EXT void HandlerIrIrq();
IR_EXT void do_ir_count(void);
IR_EXT u16 get_ir_count(void);
IR_EXT void reset_ir_count(void);
IR_EXT void enable_ir(u8 enable);

#endif	//___IR_H___

