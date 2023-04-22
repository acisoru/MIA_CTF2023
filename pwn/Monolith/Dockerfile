FROM ubuntu
RUN apt update -y && apt install -y tcpd xinetd make gcc gcc-multilib
RUN useradd monolith
COPY ./app/monolith-config /etc/xinetd.d/monolith
COPY ./app/src/main.c /main.c
COPY ./app/Makefile /Makefile
COPY ./app/flag.txt /flag.txt
RUN chmod 666 /flag.txt
RUN make docker
EXPOSE 20203
CMD xinetd -dontfork
