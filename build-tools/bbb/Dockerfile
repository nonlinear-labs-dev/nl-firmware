FROM ubuntu:16.04

ARG docker_group_id
ARG user
ARG user_id

RUN groupadd -g $docker_group_id $user
RUN useradd -l -u $user_id -g $user $user

ENV LANG en_US.UTF-8

RUN apt-get update -y
RUN apt-get install -y git locales
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && dpkg-reconfigure --frontend=noninteractive locales && update-locale LANG=en_US.UTF-8

RUN apt-get update
RUN apt-get install -y git cmake build-essential wget cpio python unzip bc doxygen curl libcurl4-openssl-dev bash
