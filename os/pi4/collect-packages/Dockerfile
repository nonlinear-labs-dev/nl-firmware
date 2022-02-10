FROM multiarch/debian-debootstrap:armhf-bullseye-slim 
RUN apt -y update
RUN apt -y install jq
COPY collectPackages.sh /collectPackages.sh
COPY collectPackageFiles.sh /collectPackageFiles.sh
