FROM debian:latest

RUN apt-get update -y && apt-get install -y build-essential cmake

RUN cmake -B build && (cd build && make)

WORKDIR /usr/src/
COPY ./build/SBFlite /usr/src/

RUN make clean && make
CMD ["./main"]
