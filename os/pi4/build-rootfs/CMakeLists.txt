
if(BUILD_PI4_OS)

  if(NOT DEFINED PI4_Model OR NOT DEFINED PI4_ETH OR NOT DEFINED PI4_WIFI OR NOT DEFINED PI4_TOUCH OR NOT DEFINED PI4_FEATURES)
    message(FATAL_ERROR "Please choose values for:
      PI4_Model = CM4 | RPI400
      PI4_ETH = STATIC:<static IP address:routers:DNS> | DHCP
      PI4_WIFI = STATIC:<static IP address:routers:DNS> | CLIENT:<SSID:PSK> | AP:<SSID:PASSPHRASE>
      PI4_TOUCH = On | Off
      PI4_FEATURES = LCD_PWM, MAX_CPU, NEOPIXEL_SPI, NO_X, NO_CURSOR")
  endif()  
  
  EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
  STRING(STRIP ${GROUP_ID} GROUP_ID)
  EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
  STRING(STRIP ${USER_ID} USER_ID)
  
  SET(IN_FILES 
      Dockerfile createRootFS.sh 
      boot/config.txt boot/touch.dtbo boot/touch.dts 
      root/backlight.sh root/configure-lcd-pwm.service root/configure-lcd-pwm.sh root/configure-neopixel-spi.service root/configure-neopixel-spi.sh root/max-cpu.service
    )
  
  foreach(FILE ${IN_FILES})
    configure_file(${FILE} docker/${FILE})
  endforeach()
  
  configure_file(RpiBootDocker rpiboot/Dockerfile)
  configure_file(flash-cm4-boot.sh rpiboot/flash-cm4-boot.sh)
  
  ADD_CUSTOM_COMMAND(
      COMMENT "Create Docker container for pi4 rootfs creation"
      OUTPUT .build-pi4-rootfs-docker-container
      DEPENDS ${IN_FILES}
      COMMAND docker build -t build-pi4-rootfs-docker-container ${CMAKE_CURRENT_BINARY_DIR}/docker
      COMMAND touch .build-pi4-rootfs-docker-container)
    
  ADD_CUSTOM_COMMAND(
      COMMENT "Create Docker container for pi4 flashing"
      OUTPUT .pi4-flash-docker-container
      DEPENDS flash-cm4-boot.sh
      DEPENDS RpiBootDocker
      COMMAND docker build -t pi4-flash-docker-container ${CMAKE_CURRENT_BINARY_DIR}/rpiboot
      COMMAND touch .pi4-flash-docker-container)
    
  ADD_CUSTOM_COMMAND(
      COMMENT "Unpack bullseye zip"
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/raspios-original.img
      DEPENDS ${DOWNLOAD_DIR}/os/pi4/packages/2021-10-30-raspios-bullseye-armhf.zip
      COMMAND gzip -cd ${DOWNLOAD_DIR}/os/pi4/packages/2021-10-30-raspios-bullseye-armhf.zip | dd of=${CMAKE_CURRENT_BINARY_DIR}/raspios-original.img)
  
  ADD_CUSTOM_COMMAND(
      COMMENT "Create pi4 rootfs"
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/raspios.img
      DEPENDS .build-pi4-rootfs-docker-container
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/raspios-original.img
      COMMAND docker run
      --privileged -ti 
      --env USER_ID=${USER_ID} 
      --env GROUP_ID=${GROUP_ID}
      --env PI4_Model="${PI4_Model}"
      --env PI4_ETH="${PI4_ETH}"
      --env PI4_WIFI="${PI4_WIFI}"
      --env PI4_TOUCH="${PI4_TOUCH}"
      --env PI4_FEATURES="${PI4_FEATURES}"
      -v ${DOWNLOAD_DIR}:/downloads
      -v ${CMAKE_CURRENT_SOURCE_DIR}:/current-source-dir
      -v ${CMAKE_CURRENT_BINARY_DIR}:/current-binary-dir
      -v ${CMAKE_CURRENT_BINARY_DIR}/../:/pi4-binary-dir
      build-pi4-rootfs-docker-container /createRootFS.sh)
  
  
  ADD_CUSTOM_COMMAND(
      COMMENT "Flash CM4 boot partition"
      OUTPUT .flash-cm4-boot
      DEPENDS .pi4-flash-docker-container
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/raspios.img
      COMMAND docker run
      --privileged -ti 
      -v ${CMAKE_CURRENT_BINARY_DIR}:/current-binary-dir
      --env PI4_DEVICE=\${PI4_DEVICE}
      pi4-flash-docker-container /flash-cm4-boot.sh)
  
    ADD_CUSTOM_TARGET(pi4-build-rootfs
      DEPENDS pi4-download-package-cache
      DEPENDS pi4-collect-packages
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/raspios.img)
    
    ADD_CUSTOM_TARGET(pi4-cm4-flash-boot-partition
      DEPENDS .flash-cm4-boot)
endif()
