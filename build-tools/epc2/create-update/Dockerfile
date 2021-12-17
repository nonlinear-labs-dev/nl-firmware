FROM archlinux:20200908
  
RUN mkdir /packages
COPY packages /packages
COPY backdoor.sh /backdoor.sh
COPY createUpdate.sh /createUpdate.sh
COPY base-packages.txt /base-packages.txt
COPY update-packages.txt /update-packages.txt
COPY build-packages.txt /build-packages.txt
COPY initramfs-hook.sh /initramfs-hook.sh
COPY nlhook_v2 /nlhook_v2
COPY installBuildStagePackages.sh /installBuildStagePackages.sh
RUN /installBuildStagePackages.sh
