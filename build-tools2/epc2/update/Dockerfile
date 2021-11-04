FROM epc2-build-arch

ARG PACKAGES
COPY createUpdate.sh /createUpdate.sh
RUN chmod +x /createUpdate.sh
RUN pacman -Sy
RUN echo $PACKAGES
RUN pacman --noconfirm -S $PACKAGES

