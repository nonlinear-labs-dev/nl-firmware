### Compiling steps (Atmel Studio 7):
   - Load Solution  "supervisor.atsln"
   - Clean Solution
   - Select  _"supervisorATM16A"_  in Solution Explorer as active project
   - Build-->Rebuild _"supervisorATM16A"_
   - Select  _"supervisorATM164A"_  in Solution Explorer as active project
   - Build-->Rebuild  _"supervisorATM164A"_
   The **"Rebuild"** step is important to get the correct version strings into the .elf artifacts
   DO NOT USE just simply "build solution", etc

### Programming:
  - Loading a Solution/Project is not required
  - Select proper device (ATmega16A or ATmega164A) in the progger, then press "apply", then "read
    ("read" makes sure AtmelStudio will complain if the hardware does not match the selection)
  - Select proper image file (\*.elf) in the progger, from the "Release" folder.
  
### Fuse Settings:
  - For ATmega16A:
	  - Set fuse LOW.SUT_CKSEL to "INT.RC.OSC 2MHZ 6CK 64MS"
	  - Set fuse LOW.BODLEVEL to "2.7V"
	  - Set fuse LOW.BODON to "ON"
  - For ATmega164A:
    - Set fuse LOW.SUT_CKSEL to "Int RC, 6CK + 65m"
    - Set fuse LOW.CKDIV8 to OFF
    - Set fuse EXTENDED.BODLEVEL to "2.7V"
