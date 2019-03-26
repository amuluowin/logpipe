FROM centos AS build-env

RUN yum install gcc gcc-c++ make wget zlib zlib-devel -y

ADD ./ /code

RUN cd /code && wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz && tar -zxvf libiconv-1.15.tar.gz && \
    cd libiconv-1.15 && ./configure --prefix=/usr/local && make && make install

RUN cd /code && wget https://github.com/edenhill/librdkafka/archive/v1.0.0.tar.gz && tar -zxvf v1.0.0.tar.gz && \
    cd librdkafka-1.0.0 && ./configure --prefix=/root && make && make install

RUN cd /code/src && make -f makefile.Linux && make -f makefile.Linux install
RUN cd /code/src-plugins && make -f makefile.Linux logpipe-input-file.so logpipe-output-tcp.so logpipe-output-file.so logpipe-input-tcp.so logpipe-output-kafka.so && \
    mkdir -p /root/so && cp logpipe-input-file.so logpipe-output-tcp.so logpipe-output-file.so logpipe-input-tcp.so logpipe-output-kafka.so /root/so

FROM alpine

MAINTAINER albert <63851587@qq.com>

ENV TIMEZONE Asia/Shanghai

RUN mkdir -p /lib64

COPY --from=build-env /root/bin/logpipe /usr/local/bin/logpipe
COPY --from=build-env /root/lib/liblogpipe_api.so /lib64/liblogpipe_api.so
COPY --from=build-env /usr/local/lib/libiconv.so.2 /lib64/libiconv.so.2
COPY --from=build-env /root/lib/librdkafka.so.1 /lib64/librdkafka.so.1
COPY --from=build-env /lib64/ld-2.17.so /lib64/ld-linux-x86-64.so.2
COPY --from=build-env /lib64/libc-2.17.so /lib64/libc.so.6
COPY --from=build-env /lib64/libdl-2.17.so /lib64/libdl.so.2
COPY --from=build-env /lib64/libm-2.17.so /lib64/libm.so.6
COPY --from=build-env /lib64/libz.so.1.2.7 /lib64/libz.so.1
COPY --from=build-env /lib64/libcom_err.so.2.1 /lib64/libcom_err.so.2
COPY --from=build-env /lib64/libcrypto.so.1.0.2k /lib64/libcrypto.so.10
COPY --from=build-env /lib64/libfreebl3.so /lib64/libfreebl3.so
COPY --from=build-env /lib64/libgssapi_krb5.so.2.2 /lib64/libgssapi_krb5.so.2
COPY --from=build-env /lib64/libkeyutils.so.1.5 /lib64/libkeyutils.so.1
COPY --from=build-env /lib64/libkrb5.so.3.3 /lib64/libkrb5.so.3
COPY --from=build-env /lib64/libkrb5support.so.0.1 /lib64/libkrb5support.so.0
COPY --from=build-env /lib64/libpcre.so.1.2.0 /lib64/libpcre.so.1
COPY --from=build-env /lib64/libpthread-2.17.so /lib64/libpthread.so.0
COPY --from=build-env /lib64/libresolv-2.17.so /lib64/libresolv.so.2
COPY --from=build-env /lib64/librt-2.17.so /lib64/librt.so.1
COPY --from=build-env /lib64/libsasl2.so.3.0.0 /lib64/libsasl2.so.3
COPY --from=build-env /lib64/libselinux.so.1 /lib64/libselinux.so.1
COPY --from=build-env /lib64/libssl.so.1.0.2k /lib64/libssl.so.10
COPY --from=build-env /root/so /root/so

RUN mkdir -p /logpipe && echo 'Asia/Shanghai' > /etc/timezone && \
    rm -rf /var/cache/apk/* /tmp/* /usr/share/man

WORKDIR /logpipe

ENTRYPOINT ["logpipe"]
CMD ["-f", "logpipe.conf", "--no-daemon"]