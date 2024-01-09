FROM ubuntu:22.04

ENV PROJPATH /root/allbackup

RUN mkdir ${PROJPATH}
COPY . ${PROJPATH}
WORKDIR ${PROJPATH}

# 设置时区
ENV TZ="Asia/Shanghai"

# 构建环境
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN apt update
RUN apt install -y build-essential cmake libssl-dev zlib1g-dev git

# 安装gtest
RUN cd googletest && mkdir build && cd build && cmake .. && make
RUN cd googletest && cp -r googletest/include/* /usr/include && cp -r googlemock/include/* /usr/include && cp -r build/lib/* /usr/lib

# 编译安装
RUN rm -rf build && mkdir build
RUN cd build && cmake .. && make && make install