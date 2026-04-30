FROM debian:latest

RUN apt-get update -y && apt-get install -y build-essential 

WORKDIR /usr/src/
COPY . /usr/src/

RUN make clean && make
CMD ["./main"]
