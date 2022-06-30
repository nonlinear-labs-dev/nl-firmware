FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin

RUN apt-get update
RUN apt-get install -y nodejs npm gcc build-essential
RUN npm install -g typescript
