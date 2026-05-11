FROM gcc:16.1

RUN apt-get update -y && apt-get install -y cmake

# Configuration file
RUN mkdir -p /etc/sbflite

# Copy application to /usr/src/
COPY . /usr/src/
WORKDIR /usr/src/

# Build application
RUN cmake -B build 
RUN (cd build && make -j)

# Run application
CMD ["./build/SBFlite"]