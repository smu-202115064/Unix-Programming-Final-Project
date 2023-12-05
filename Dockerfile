# Download base image ubuntu 20.04
FROM ubuntu:20.04

# LABEL about the custom image
LABEL maintainer="김동주"
LABEL version="0.0.1"
LABEL description="[Seoul] UNIX Programming (HAEA0014 (1)) [2nd Semester] 수업 기말 과제 프로젝트 :: 서버의 로드밸런싱"

WORKDIR /root

# # Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

# # Update Ubuntu Software repository
RUN apt update
RUN apt-get install git -y
RUN apt-get install man-db -y
RUN apt-get install gcc -y
RUN apt-get install make
RUN apt-get install language-pack-ko -y
RUN apt-get install vim -y

RUN yes | unminimize

# COPY vimrc /etc/vim/vimrc
