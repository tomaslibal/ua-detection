FROM gliderlabs/alpine:3.4

RUN apk update

RUN apk add --no-cache gcc g++ make cmake cppunit cppunit-dev

WORKDIR /opt/uadet2

COPY ./src /opt/uadet2/src
COPY ./config /opt/uadet2/config
COPY ./CMakeLists.txt /opt/uadet2
COPY ./data_in.txt /opt/uadet2

EXPOSE 10128/tcp

RUN cmake . -DDISABLE_TESTS=YES
RUN make

CMD /opt/uadet2/dist/uadet2d
