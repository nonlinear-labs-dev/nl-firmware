Copy this directory to /tmp on beagle bone.
Make files executable : readout-ribbon-adcs.sh, lpc, lpc-read, make-cal-bin
Run "readout-ribbon-adcs.sh" and collect values.
Enter "calibration" folder and edit X-values in ribbon calibration input files (use templates).
Run "../make-cal-bin" in that folder to create LPC message.
Copy results to epc::/persistent.
 