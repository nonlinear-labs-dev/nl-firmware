/* Define each memory region, for M4 core */
/* Memory Model is for the smallest suitable LPC43xx, the LPC4322 */

MEMORY
{
  MFlash256  (   rx) : ORIGIN = 0x1a000000, LENGTH = 0x40000   /* 256K bytes   -- code and initialized data images, for M4 and M0 */  
  RamLoc40     (rwx) : ORIGIN = 0x10080000, LENGTH = 0xa000    /* 40K bytes    -- primary RAM (data & stack) for M4 */  
  RamLoc32     (rwx) : ORIGIN = 0x10000000, LENGTH = 0x8000    /* 32K bytes    -- do not use (this is M0's private RAM), also RAM where 
                                                                  M0 code&ini-data is uploaded to. This must be the second RAM definition here! */  
  RamAHB16     (rwx) : ORIGIN = 0x20000000, LENGTH = 0x4000    /* 16K bytes    -- shared RAM between M0 and M4*/  
  RamAHB_ETB16 (rwx) : ORIGIN = 0x2000c000, LENGTH = 0x4000    /* 16K bytes    -- unused */  
}

/* Define a symbol for base and top of each memory region */
  __base_MFlash256 = 0x1a000000  ; /* MFlash256 */  
  __top_MFlash256 = 0x1a000000 + 0x40000 ; /* 256K bytes */  
  
  __base_RamLoc40 = 0x10080000  ; /* RamLoc40 */  
  __top_RamLoc40 = 0x10080000 + 0xa000 ; /* 40K bytes */  
  
  __base_RamAHB16 = 0x20000000  ; /* RamAHB16 */  
  __top_RamAHB16 = 0x20000000 + 0x4000 ; /* 16K bytes */  
  
  __base_RamLoc32 = 0x10000000  ; /* RamLoc32 */  
  __top_RamLoc32 = 0x10000000 + 0x8000 ; /* 32K bytes */  

  __base_RamAHB_ETB16 = 0x2000c000  ; /* RamAHB_ETB16 */  
  __top_RamAHB_ETB16 = 0x2000c000 + 0x4000 ; /* 16K bytes */  
  