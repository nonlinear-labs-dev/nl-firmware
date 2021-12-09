
SET(COLLECT_PACKAGES_INPUT_ARG ${EPC2_BASE_ADDITIONAL_PACKAGES})
CONFIGURE_FILE(BaseDockerfile base-packages-docker-container/Dockerfile @ONLY)
CONFIGURE_FILE(collectPackages.sh base-packages-docker-container/collectPackages.sh @ONLY)

SET(COLLECT_PACKAGES_INPUT_ARG "${EPC2_BASE_ADDITIONAL_PACKAGES} ${EPC2_UPDATE_PACKAGES}")
CONFIGURE_FILE(UpdateDockerfile update-packages-docker-container/Dockerfile @ONLY)
CONFIGURE_FILE(collectPackages.sh update-packages-docker-container/collectPackages.sh @ONLY)

SET(COLLECT_PACKAGES_INPUT_ARG "${EPC2_BUILD_TOOLS}")
CONFIGURE_FILE(BuildDockerfile build-packages-docker-container/Dockerfile @ONLY)
CONFIGURE_FILE(collectPackages.sh build-packages-docker-container/collectPackages.sh @ONLY)

SET(BASE_KERNEL_FILENAME linux-rt-${EPC2_BASE_KERNEL}-x86_64.pkg.tar.zst)
SET(UPDATE_KERNEL_FILENAME linux-rt-${EPC2_UPDATE_KERNEL}-x86_64.pkg.tar.zst)

ADD_CUSTOM_COMMAND(
    COMMENT "Build 'Collect Base Packages'-Docker Container"
    OUTPUT .collect-base-packages-docker-container
    DEPENDS BaseDockerfile
    DEPENDS CMakeLists.txt
    DEPENDS ../CMakeLists.txt
    DEPENDS collectPackages.sh
    VERBATIM
    COMMAND 
      cmp --silent ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/base-packages-docker-container/${BASE_KERNEL_FILENAME} ||
      cp ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/base-packages-docker-container/${BASE_KERNEL_FILENAME} > /dev/null || 
      wget --show-progress -q http://h2949050.stratoserver.net/build-tools2/${BASE_KERNEL_FILENAME} -O ${CMAKE_CURRENT_BINARY_DIR}/base-packages-docker-container/${BASE_KERNEL_FILENAME}
    COMMAND docker build -t collect-base-packages-docker-container ${CMAKE_CURRENT_BINARY_DIR}/base-packages-docker-container
    COMMAND touch .collect-base-packages-docker-container
    )
  
ADD_CUSTOM_COMMAND(
    COMMENT "Build 'Collect Update Packages'-Docker Container"
    OUTPUT .collect-update-packages-docker-container
    DEPENDS UpdateDockerfile
    DEPENDS CMakeLists.txt
    DEPENDS ../CMakeLists.txt
    DEPENDS collectPackages.sh
    VERBATIM 
    COMMAND 
      cmp --silent ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${BASE_KERNEL_FILENAME} ||
      cp ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${BASE_KERNEL_FILENAME} > /dev/null || 
      wget --show-progress -q http://h2949050.stratoserver.net/build-tools2/${BASE_KERNEL_FILENAME} -O ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${BASE_KERNEL_FILENAME}
    COMMAND 
      cmp --silent ${DOWNLOAD_DIR}/epc2/packages/${UPDATE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${UPDATE_KERNEL_FILENAME} ||
      cp ${DOWNLOAD_DIR}/epc2/packages/${UPDATE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${UPDATE_KERNEL_FILENAME} > /dev/null || 
      wget --show-progress -q http://h2949050.stratoserver.net/build-tools2/${UPDATE_KERNEL_FILENAME} -O ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${UPDATE_KERNEL_FILENAME} || 
      wget --show-progress -q https://pkgbuild.com/~dvzrv/repos/realtime/x86_64/${UPDATE_KERNEL_FILENAME} -O ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container/${UPDATE_KERNEL_FILENAME} 
    COMMAND docker build -t collect-update-packages-docker-container ${CMAKE_CURRENT_BINARY_DIR}/update-packages-docker-container
    COMMAND touch .collect-update-packages-docker-container
    )
  
ADD_CUSTOM_COMMAND(
    COMMENT "Build 'Collect Build Packages'-Docker Container"
    OUTPUT .collect-build-packages-docker-container
    DEPENDS BuildDockerfile
    DEPENDS CMakeLists.txt
    DEPENDS ../CMakeLists.txt
    DEPENDS collectPackages.sh
    VERBATIM 
    COMMAND 
      cmp --silent ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/build-packages-docker-container/${BASE_KERNEL_FILENAME} ||
      cp ${DOWNLOAD_DIR}/epc2/packages/${BASE_KERNEL_FILENAME} ${CMAKE_CURRENT_BINARY_DIR}/build-packages-docker-container/${BASE_KERNEL_FILENAME} > /dev/null || 
      wget --show-progress -q http://h2949050.stratoserver.net/build-tools2/${BASE_KERNEL_FILENAME} -O ${CMAKE_CURRENT_BINARY_DIR}/build-packages-docker-container/${BASE_KERNEL_FILENAME}
    COMMAND docker build -t collect-build-packages-docker-container ${CMAKE_CURRENT_BINARY_DIR}/build-packages-docker-container
    COMMAND touch .collect-build-packages-docker-container
    )
  
ADD_CUSTOM_COMMAND(
    COMMENT "Collect Base Packages"
    OUTPUT base-packages.txt
    DEPENDS .collect-base-packages-docker-container
    VERBATIM
    COMMAND docker run collect-base-packages-docker-container /collectPackages.sh > base-packages.txt
    )
    
ADD_CUSTOM_COMMAND(
    COMMENT "Collect Update Packages"
    OUTPUT update-packages.txt
    DEPENDS .collect-update-packages-docker-container
    VERBATIM
    COMMAND docker run collect-update-packages-docker-container /collectPackages.sh > update-packages.txt
    )
  
ADD_CUSTOM_COMMAND(
    COMMENT "Collect Build Packages"
    OUTPUT build-packages.txt
    DEPENDS .collect-build-packages-docker-container
    VERBATIM
    COMMAND docker run collect-build-packages-docker-container /collectPackages.sh > build-packages.txt
    )
    
ADD_CUSTOM_COMMAND(
    COMMENT "Collect final packages" 
    OUTPUT packages.txt
    DEPENDS base-packages.txt
    DEPENDS update-packages.txt
    DEPENDS build-packages.txt
    VERBATIM
    COMMAND cat base-packages.txt update-packages.txt build-packages.txt | sort -u > packages.txt
    )

ADD_CUSTOM_TARGET(epc2-collect-packages DEPENDS packages.txt)