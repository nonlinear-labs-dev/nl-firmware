FROM debian:bullseye-slim
            
RUN apt-get update && apt-get install -y \
    apt-utils \
    curl \
    build-essential \
    automake \
    autogen \
    bash \
    build-essential \
    bc \
    bzip2 \
    ca-certificates \
    curl \
    file \
    git \
    gzip \
    make \
    ncurses-dev \
    pkg-config \
    libtool \
    python \
    rsync \
    sed \
    bison \
    flex \
    tar \
    vim \
    wget \
    runit \
    xz-utils

RUN apt -y update && apt install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf cmake make mc
RUN dpkg --add-architecture armhf
RUN apt -y update
RUN apt -y install libsoup2.4-dev:armhf libflac-dev:armhf libglibmm-2.4-dev:armhf libasound2-dev:armhf libboost-dev:armhf
RUN apt -y install libfreetype-dev:armhf
RUN apt -y install libpng++-dev
RUN apt -y install debhelper dh-make quilt fakeroot lintian 

ARG BOB_ID
RUN useradd -m -u $BOB_ID bob-the-builder
COPY DEBIAN/* /DEBIAN/
USER bob-the-builder
ENV PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig

CMD cmake \
        -DCMAKE_PREFIX_PATH=/build/NL-1.0/usr/local \
        -DCMAKE_INSTALL_PREFIX=/build/NL-1.0/usr/local \
        -DCMAKE_TOOLCHAIN_FILE=/src/build-tools/pi4/pi4.cmake \
        -DBUILD_AUDIOENGINE=On \
        -DBUILD_PLAYGROUND=Off \
        -S /src \
        -B /build && \
    make -C /build -j12 install && \
    mkdir -p /build/NL-1.0/DEBIAN && \
    cp /DEBIAN/* /build/NL-1.0/DEBIAN/ && \
    chmod 775 /build/NL-1.0/DEBIAN/postinst && \
    chmod 775 /build/NL-1.0/DEBIAN/postrm && \
    dpkg-deb -b /build/NL-1.0
