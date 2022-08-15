FROM ubuntu:20.04

# Install required components

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
    libboost-all-dev \
    libbz2-dev \
    libgtest-dev \
    libgmock-dev \
    libicu-dev \
    libsqlite3-dev \
    python-dev \
    gdb \
    cmake \
    wget


# Build boost

WORKDIR /root

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.bz2
RUN mkdir boost
RUN tar -xf boost_1_80_0.tar.bz2

WORKDIR /root/boost_1_80_0

RUN ./bootstrap.sh --with-toolset=clang
RUN ./b2 --build_type=complete --toolset=clang install
