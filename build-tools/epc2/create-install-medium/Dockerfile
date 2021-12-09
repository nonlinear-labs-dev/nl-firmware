FROM archlinux:20200908

COPY createIso.sh /

RUN pacman --noconfirm -Syyuu
RUN pacman --noconfirm -Syy

CMD /createIso.sh
