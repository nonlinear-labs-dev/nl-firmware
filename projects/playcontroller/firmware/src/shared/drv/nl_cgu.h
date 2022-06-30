/******************************************************************************/
/** @file		nl_cgu.h
    @date		2015-01-28
    @brief    	CGU functions
    @author		Nemanja Nikodijevic [2015-01-28]
*******************************************************************************/
#pragma once

#define NL_LPC_CLK              (204000000ul)
#define NL_LPC_CLK_PERIOD_100PS (10000000000ul / NL_LPC_CLK)
#define NL_LPC_AUDIO_CLK        (12288000ul)

// M4 ticker interrupt period in us
#define M4_PERIOD_US (5ul)
#define M4_FREQ_HZ   (1000000 / M4_PERIOD_US)

// M0 interrupt period in ns
#define M0_IRQ_PERIOD_NS       (1500ull)
#define M0_IRQ_FREQ_HZ         (1000000000ull / M0_IRQ_PERIOD_NS)                  // M0 interrupt frequency
#define KBS_MICROSECS_PER_SCAN ((unsigned) (16ull * 1000000ull / M0_IRQ_FREQ_HZ))  // keybed scanner takes 16 M0 IRQ periods
