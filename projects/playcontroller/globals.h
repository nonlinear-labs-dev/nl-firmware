// system-wide globals / defines / compile switches
#pragma once

// LPC compile time switches, feature options :
#define LPC_KEYBED_DIAG (0)  // enable key event counter diagnostics

// LPC compile time switches, debug specials (breaking normal function) :
#define LPC_DGB_ADC_STRESS_TEST (0)  // use emulated ADC values for stress testing

// LPC compile time switches, select USB1 port
#define LPC_DGB_USB1 (0)  // use USB1 port rather than USB0
