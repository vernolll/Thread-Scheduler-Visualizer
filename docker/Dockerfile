FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    g++ \
    gdb \
    qtbase5-dev \
    qtdeclarative5-dev \
    libqt5svg5-dev \
    qml-module-qtquick2 \
    qml-module-qtquick-controls2 \
    qml-module-qtquick-layouts \
    qml-module-qtquick-window2 \
    qtchooser \
    qt5-qmake \
    ca-certificates \
    && rm -rf /var/lib/apt-get/lists/*

WORKDIR /workspace

CMD ["bash"]