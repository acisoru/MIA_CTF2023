FROM ubuntu
#RUN apt update -y && apt install -y tcpd xinetd make gcc gcc-multilib
RUN apt update -y
RUN useradd snake
COPY ./app/snake-config /etc/xinetd.d/snake
COPY ./app/src/main.c /main.c
COPY ./app/Makefile /Makefile
COPY ./app/flag.txt /flag.txt
RUN chmod 666 /flag.txt
RUN make docker
EXPOSE 3777
CMD xinetd -dontfork
