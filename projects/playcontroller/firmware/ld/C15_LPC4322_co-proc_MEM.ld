/* Define each memory region, for M0 core -- running from RAM */
/* Memory Model is for the smallest suitable LPC43xx, the LPC4322 */

MEMORY
{
  RamLoc32     (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000 /* 32K bytes -- primary RAM (data & stack), 
                                                               and code and inititalized data image, will be uploaded by M4 */  
  RamLoc40     (rwx) : ORIGIN = 0x10080000, LENGTH = 0xa000 /* 40K bytes -- do not use (this is M4's private RAM) */  
  RamAHB16     (rwx) : ORIGIN = 0x20000000, LENGTH = 0x4000 /* 16K bytes -- shared RAM between M0 and M4*/  
  RamAHB_ETB16 (rwx) : ORIGIN = 0x2000c000, LENGTH = 0x4000 /* 16K bytes -- unused */  
}

/* Define a symbol for base and top of each memory region */
  __base_RamLoc40 = 0x10080000  ;
  __top_RamLoc40 = 0x10080000 + 0xa000 ; /* 40K bytes */  

  __base_RamAHB16 = 0x20000000  ;
  __top_RamAHB16 = 0x20000000 + 0x4000 ; /* 16K bytes */  

  __base_RamLoc32 = 0x10000000  ;
  __top_RamLoc32 = 0x10000000 + 0x8000 ; /* 32K bytes */  

  __base_RamAHB_ETB16 = 0x2000c000 ;
  __top_RamAHB_ETB16 = 0x2000c000 + 0x4000 ; /* 16K bytes */  
