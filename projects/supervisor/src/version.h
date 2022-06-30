// version information
// KSTR 2019-06-07

#ifndef __version_h__
#define __version_h__

#if defined(__AVR_ATmega16A__)
#warning "Compiling for ATmega16A"
#define CHIP_TYPE        "ATmega16A"
#define IS_TYPE_XX4_CHIP (0)
#elif defined(__AVR_ATmega164A__)
#warning "Compiling for ATmega164A"
#define CHIP_TYPE        "ATmega164A"
#define IS_TYPE_XX4_CHIP (1)
#else
#error "Device must be either ATmega16A or ATmega164A"
#endif

/****
Firmware versions and Internal ID Numbers
The ID Numbers+1 will be displayed as LED blinks during Power-Up, after HW- Version
	5.0			= 1
	5.1			= 2
	5.2			= 3
	etc
****/
#define FW_Version_ID_Major 5
#define FW_Version_ID_Minor 3
#define FW_Version_ID       (FW_Version_ID_Major * 10 + FW_Version_ID_Minor)

#define STR_IMPL_(x) #x            //stringify argument
#define STR(x)       STR_IMPL_(x)  //indirection to expand argument macros
static const char VERSION_STRING[] = "\n\nC15 Supervisor, " CHIP_TYPE ", FIRMWARE VERSION: " STR(FW_Version_ID_Major) "." STR(FW_Version_ID_Minor) " \n\n\0\0\0";

#endif
