FROM ubuntu:jammy

RUN apt update && apt install -y \
    cmake \
    git \
    g++-12 \
    make \
    clang-format \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s /usr/bin/g++-12 /usr/bin/g++

# install yaml-cpp
ENV YAML_CPP_REPO_URL="https://github.com/jbeder/yaml-cpp.git"
RUN git clone ${YAML_CPP_REPO_URL} \
    && cd yaml-cpp \
    && mkdir build \
    && cd build \
    && cmake -DYAML_BUILD_SHARED_LIBS=ON .. \
    && make \
    && make install \
    && cd ../.. \
    && rm -rf yaml-cpp \
    && ln -s /usr/local/lib/libyaml-cpp.so.0.8 /usr/lib/libyaml-cpp.so.0.8


# install gtest
RUN apt update && apt install -y \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src/gtest \
    && cmake CMakeLists.txt \
    && make \
    && cp ./lib/libgtest*.a /usr/lib \
    && cd .. \
    && rm -rf gtest

# install gmock
RUN apt update && apt install -y \
    libgmock-dev \
    && rm -rf /var/lib/apt/lists/*


