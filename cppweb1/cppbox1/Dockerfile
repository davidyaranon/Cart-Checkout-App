FROM gcc:7.3.0

RUN apt-get -qq update
RUN apt-get -qq upgrade
RUN apt-get -qq install cmake
RUN apt-get -qq install build-essential libtcmalloc-minimal4 && \
    ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

RUN apt update \
	&& apt install -y \
		build-essential \
		libboost-all-dev 

RUN apt-get install -y libmongoc-dev
RUN apt-get install -y libbson-dev
RUN apt-get install -y libmongoc-1.0-0
RUN apt-get install -y libbson-1.0-0
RUN apt-get install -y cmake libssl-dev libsasl2-dev

RUN wget -qO- "https://cmake.org/files/v3.14/cmake-3.14.0-Linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

RUN git clone https://github.com/trusch/libbcrypt && \
cd libbcrypt && \
mkdir build && \
cd build && \
cmake .. && \
make && \
make install && \
ldconfig

RUN git clone https://github.com/Thalhammer/jwt-cpp \
&& cd jwt-cpp \
&& mkdir build \
&& cd build \
&& cmake .. \
&& make \
&& make install \
&& ldconfig

WORKDIR /usr/src

RUN wget https://github.com/mongodb/mongo-c-driver/releases/download/1.17.0/mongo-c-driver-1.17.0.tar.gz \
&& tar xzf mongo-c-driver-1.17.0.tar.gz && cd mongo-c-driver-1.17.0 \
&& mkdir cmake-build && cd cmake-build \
&& cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. \
&& make install && ldconfig /usr/local/lib

RUN curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.6/mongo-cxx-driver-r3.6.6.tar.gz \
&& tar -xzf mongo-cxx-driver-r3.6.6.tar.gz && cd mongo-cxx-driver-r3.6.6/build \
&& cmake \
-DBSONCXX_POLY_USE_MNMLSTC=1 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/usr/local \
-DLIBMONGOC_DIR=/usr/lib/x86_64-linux-gnu \
-DLIBBSON_DIR=/usr/lib/x86_64-linux-gnu \
-DCMAKE_MODULE_PATH=/usr/src/mongo-cxx-driver-r3.6.6/cmake .. \
&& make EP_mnmlstc_core && make && make install && ldconfig /usr/local/lib

ENV SECRET_KEY=DR#>=DflH4z9<%<P8Ko.hR.Likn:%$`;^/B@bxQQXx:gdHbsrCoc%cO^19Y

