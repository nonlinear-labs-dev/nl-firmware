SET(DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/../nonlinear-labs-downloads" CACHE STRING "Where to store downloads needed for NonlinearLabs-Builds")
SET(NL_SERVER h2949050.stratoserver.net)
SET(NL_UPLOAD_PATH /var/www/html/build-tools2) # used with rsync
SET(NL_DOWNLOAD_PATH /build-tools2) # used with http
SET(ARCH_SERVER "https://archive.archlinux.org/packages/.all")

ADD_SUBDIRECTORY(epc1)
ADD_SUBDIRECTORY(epc2)
