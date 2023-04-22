FROM ubuntu
RUN apt update -y && apt install -y tcpd xinetd make gcc gcc-multilib
RUN useradd defender
COPY ./app/defender-config /etc/xinetd.d/defender
COPY ./app/src/main.c /main.c
COPY ./app/Makefile /Makefile
COPY ./app/flag.txt /flag.txt
RUN chmod 666 /flag.txt
RUN make docker
EXPOSE 17171
CMD xinetd -dontfork
