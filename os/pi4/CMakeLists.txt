cmake_minimum_required(VERSION 3.2)

SET(NL_DOWNLOAD_PATH /os/pi4) # used with http
SET(NL_UPLOAD_PATH /var/www/html${NL_DOWNLOAD_PATH}) # used with rsync
SET(DOWNLOAD_BASE_URL http://${NL_SERVER}${NL_DOWNLOAD_PATH})
SET(OS_IMAGE_FILE_NAME "2021-10-30-raspios-bullseye-armhf.zip")

SET(PI4_PACKAGES udhcpd hostapd i2c-tools cpufrequtils libsoup2.4-1 libasound2 libflac8 libfreetype6 libglibmm-2.4-1v5 )

add_subdirectory(build-rootfs)
add_subdirectory(collect-packages)
add_subdirectory(download-package-cache) 
add_subdirectory(fetch-packages)
add_subdirectory(upload-package-cache)

# TODO:
# use rpiboot to flash device from make command
