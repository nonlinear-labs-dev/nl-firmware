FROM archlinux:20200908
  
COPY createUpdate.sh /createUpdate.sh
COPY base-packages.txt /base-packages.txt
COPY update-packages.txt /update-packages.txt
COPY build-packages.txt /build-packages.txt
COPY setup-base-os.sh /setup-base-os.sh
COPY packages /packages


RUN echo "Server = https://archive.archlinux.org/repos/2020/09/08/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist

RUN sed "s/^\[core\]/[core]\nSigLevel = Optional TrustAll/g" -i /etc/pacman.conf
RUN sed "s/^\[extra\]/[extra]\nSigLevel = Optional TrustAll/g" -i /etc/pacman.conf
RUN sed "s/^\[community\]/[community]\nSigLevel = Optional TrustAll/g" -i /etc/pacman.conf

RUN pacman --noconfirm -Sy

RUN pacman --noconfirm -S icu npm nodejs

RUN /setup-base-os.sh
