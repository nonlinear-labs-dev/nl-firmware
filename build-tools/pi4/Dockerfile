FROM debian:bullseye-slim
            
RUN apt-get update && apt-get install -y \
    apt-utils curl build-essential automake autogen bash build-essential bc bzip2 ca-certificates \
    curl file git gzip make ncurses-dev pkg-config libtool python rsync sed bison flex tar vim wget \
    runit xz-utils

RUN apt -y update && apt install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf cmake make mc
RUN dpkg --add-architecture armhf
RUN apt -y update
RUN apt -y install libsoup2.4-dev:armhf libflac-dev:armhf libglibmm-2.4-dev:armhf libasound2-dev:armhf libboost-dev:armhf
RUN apt -y install libfreetype-dev:armhf
RUN apt -y install libpng++-dev
RUN apt -y install debhelper dh-make quilt fakeroot lintian 
RUN apt -y install sudo

ARG BOB_ID
RUN useradd -m -u $BOB_ID bob-the-builder
RUN usermod -aG sudo bob-the-builder
RUN echo "bob-the-builder ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
COPY build.sh /build.sh
RUN chmod +x /build.sh
USER bob-the-builder
ENV PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig

CMD /build.sh