FROM centos AS build-env

RUN yum install gcc gcc-c++ wget make zlib zlib-devel -y

ADD ./ /code

RUN cd /code && wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz && tar -zxvf libiconv-1.15.tar.gz && \
    cd libiconv-1.15 && ./configure --prefix=/root && make -j$(nproc) && make install

RUN cd /code && wget https://github.com/edenhill/librdkafka/archive/v1.0.0.tar.gz && tar -zxvf v1.0.0.tar.gz && \
    cd librdkafka-1.0.0 && ./configure --prefix=/root && make -j$(nproc) && make install

RUN cd /code && wget http://apache.fayea.com/zookeeper/zookeeper-3.4.14/zookeeper-3.4.14.tar.gz && tar -zxvf zookeeper-3.4.14.tar.gz && \
    cd zookeeper-3.4.14/zookeeper-client/zookeeper-client-c && ./configure --prefix=/root && make -j$(nproc) && make install

RUN cd /code/src && make -j$(nproc) -f makefile.Linux && make -j$(nproc) -f makefile.Linux install
RUN cd /code/src-plugins && make -j$(nproc) -f makefile.Linux logpipe-input-file.so logpipe-output-tcp.so logpipe-output-file.so logpipe-input-tcp.so logpipe-output-kafka.so logpipe-output-kafka-with-zookeeper.so && \
    mkdir -p /root/so && cp logpipe-input-file.so logpipe-output-tcp.so logpipe-output-file.so logpipe-input-tcp.so logpipe-output-kafka.so logpipe-output-kafka-with-zookeeper.so /root/so

FROM debian:stable-slim

ENV TZ=Asia/Shanghai

COPY --from=build-env /root/bin/logpipe /usr/local/bin/logpipe
COPY --from=build-env /root/lib/liblogpipe_api.so /lib/x86_64-linux-gnu/liblogpipe_api.so
COPY --from=build-env /root/lib/libiconv.so.2 /lib/x86_64-linux-gnu/libiconv.so.2
COPY --from=build-env /root/lib/librdkafka.so.1 /lib/x86_64-linux-gnu/librdkafka.so.1
COPY --from=build-env /root/lib/libzookeeper_mt.so.2 /lib/x86_64-linux-gnu/libzookeeper_mt.so.2
COPY --from=build-env /root/so /root/so

RUN mkdir -p /logpipe

WORKDIR /logpipe

ENTRYPOINT ["logpipe"]
CMD ["-f", "logpipe.conf", "--no-daemon"]