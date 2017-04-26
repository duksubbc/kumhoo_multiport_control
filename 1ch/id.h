#ifndef ___ID_H___
#define ___ID_H___

#ifdef ID_GLOBAL
	#define ID_EXT
#else
	#define ID_EXT extern
#endif


ID_EXT char Read_ID(void );
ID_EXT char Update_ID(char id);
#endif	//___PORT_H___

