FROM archlinux:base-20211010.0.36274

COPY createIso.sh /

RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -Syy

CMD /createIso.sh
