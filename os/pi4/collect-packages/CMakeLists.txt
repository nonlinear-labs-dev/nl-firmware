
CONFIGURE_FILE(Dockerfile docker/Dockerfile @ONLY)
CONFIGURE_FILE(collectPackages.sh docker/collectPackages.sh @ONLY)
CONFIGURE_FILE(collectPackageFiles.sh docker/collectPackageFiles.sh @ONLY)

ADD_CUSTOM_COMMAND(
    COMMENT "Build 'Collect Base Packages'-Docker Container"
    OUTPUT .pi4-collect-packages-docker-container
    DEPENDS Dockerfile
    DEPENDS ../CMakeLists.txt
    DEPENDS collectPackages.sh
    DEPENDS collectPackageFiles.sh
    VERBATIM
    COMMAND 
    COMMAND docker build -t pi4-collect-packages-docker-container ${CMAKE_CURRENT_BINARY_DIR}/docker
    COMMAND touch .pi4-collect-packages-docker-container
    )
  
ADD_CUSTOM_COMMAND(
    COMMENT "Collect PI4 Packages"
    OUTPUT packages.txt
    DEPENDS .pi4-collect-packages-docker-container
    VERBATIM
    COMMAND docker run pi4-collect-packages-docker-container /collectPackages.sh > packages.txt
    )
    
ADD_CUSTOM_COMMAND(
    COMMENT "Collect PI4 Package Files"
    OUTPUT package-files.txt
    DEPENDS packages.txt
    DEPENDS .pi4-collect-packages-docker-container
    VERBATIM
    COMMAND docker run -ti -v ${CMAKE_CURRENT_BINARY_DIR}:/bindir pi4-collect-packages-docker-container /collectPackageFiles.sh | tr -d "\\015" > package-files.txt
    )
    
ADD_CUSTOM_TARGET(pi4-collect-packages DEPENDS packages.txt package-files.txt)