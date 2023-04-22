FROM ubuntu@sha256:7a57c69fe1e9d5b97c5fe649849e79f2cfc3bf11d10bbd5218b4eb61716aebe6
WORKDIR /build
RUN apt update && apt install wget build-essential -y
RUN wget https://github.com/python/cpython/archive/refs/tags/v3.12.0a6.tar.gz
RUN tar xzvf v3.12.0a6.tar.gz
WORKDIR /build/cpython-3.12.0a6
RUN rm Modules/posixmodule.c
COPY posixmodule.c Modules/posixmodule.c
RUN (./configure && make -j); exit 0
COPY flag_reader.c fread.c
RUN gcc fread.c -o give_me_flag
RUN tar -cvzf build.tar.gz python give_me_flag Lib/encodings