FROM ubuntu:19.10

ARG docker_group_id
ARG user
ARG user_id

ENV LANG en_US.UTF-8

RUN apt-get update -y
RUN apt-get install -y git locales
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && dpkg-reconfigure --frontend=noninteractive locales && update-locale LANG=en_US.UTF-8

RUN groupadd -g $docker_group_id $user
RUN useradd -l -u $user_id -g $user $user
RUN apt-get update
RUN apt-get install -y git qemu-system-x86 cmake build-essential wget cpio python unzip bc doxygen curl libcurl4-openssl-dev openjdk-8-jdk bash fuseiso fuse squashfs-tools sudo genisoimage xorriso syslinux-utils
