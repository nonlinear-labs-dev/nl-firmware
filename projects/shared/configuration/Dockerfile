FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin

RUN apt-get update
RUN apt-get install -y nodejs npm gcc build-essential
RUN npm install -g typescript@4.7.4

COPY package-lock.json /package-lock.json
COPY package.json /package.json
COPY tsconfig.json /tsconfig.json
RUN npm install