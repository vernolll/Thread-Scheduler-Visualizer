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
    qml-module-qtquick-dialogs \
    qtchooser \
    qt5-qmake \
    ca-certificates \
    && rm -rf /var/lib/apt-get/lists/*

WORKDIR /workspace

COPY . /workspace

RUN rm -rf build && \
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -G Ninja && \
    cmake --build build --config Release

RUN cd build && ctest --output-on-failure

CMD ["bash"]