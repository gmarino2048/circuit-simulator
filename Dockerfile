FROM ubuntu:20.04

RUN DEBIAN_FRONTEND=noninteractive apt update
RUN DEBIAN_FRONTEND=noninteractive apt -y upgrade

RUN DEBIAN_FRONTEND=noninteractive apt -y install \
    build-essential \
    git \
    clang \
    llvm \
    lldb \
    gdb \
    cmake
