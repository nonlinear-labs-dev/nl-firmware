FROM archlinux:base-20211010.0.36274

COPY createRootFS.sh /

RUN pacman --noconfirm -Syyuu 
RUN pacman --noconfirm -Syy 
RUN pacman --noconfirm -S arch-install-scripts pacman-contrib

CMD /createRootFS.sh