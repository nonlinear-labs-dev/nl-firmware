FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin

RUN apt-get update
RUN apt-get install -y nodejs npm gcc build-essential

RUN mkdir -p /tmp/package
COPY package.json /tmp/package/package.json
RUN npm install -g typescript
RUN npm install -g /tmp/package

CMD cd /sourceDir && bash process.sh && bash copy-results-to-source.sh /sourceDir /binaryDir