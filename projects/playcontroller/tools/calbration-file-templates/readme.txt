---- Ribbon and Aftertouch Calibration ----

calibration.ini points to the two individual ribbon calibration files Rxxxx.cal and Ryyyy.cal
and to the Aftertouch calibration file Azzzz.cal

Replace xxxx and yyyy with ribbon serial numbers and check for proper content in those files.
Replace zzzz with ribbon serial number and check for proper content in this file.
(see projects/playcontroller/tools/calbration-file-templates).

Make a folder in the product passport named "calibration" (lower case), and place the four files
in there.

Then, make the binary calibration file "calibration.bin" using the tool "make-cal-bin".
You may use the on-board deployed tool located on the BBB directly.

Altenatively, use the windows tool:
Copy the .exe to your system and set a path to it, or simply copy it to C:\windows\system32
open a cmd console, navigate to the drive and folder (product passport folder, calibration subfolder)
and run "make-cal-bin.exe calibration.ini calibration.bin".
Write-protect all four files.

All four then go files go to /usr/C15/scripts/calibration/ on the BBB, providing a backup
They go to /persistent on the ePC where calibration.bin is actually used by playground and
uploaded to the playcontroller.

To test if the calbration is working navigate on the BBB to /usr/C15/scripts/calibration/
and run cat calibration.bin > /dev/lpc_bb_driver
