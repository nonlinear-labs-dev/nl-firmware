FROM archlinux:20200908

COPY createIso.sh /

RUN pacman --noconfirm -Sy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -Syy
RUN pacman --noconfirm -S archlinux-keyring
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib dosfstools

CMD /createIso.sh
