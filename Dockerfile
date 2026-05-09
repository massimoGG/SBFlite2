FROM debian:latest

RUN apt-get update -y && apt-get install -y build-essential cmake

# Configuration file
RUN mkdir -p /etc/sbflite

WORKDIR /usr/src/
COPY ./build/SBFlite /usr/src/

RUN make clean && make
CMD ["./main"]
