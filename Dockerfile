FROM ubuntu:20.04

RUN DEBIAN_FRONTEND=noninteractive apt update
RUN DEBIAN_FRONTEND=noninteractive apt -y upgrade

RUN DEBIAN_FRONTEND=noninteractive apt -y install \
    build-essential \
    doxygen \
    graphviz \
    git \
    clang \
    llvm \
    lldb \
    libgtest-dev \
    libgmock-dev \
    gdb \
    cmake
