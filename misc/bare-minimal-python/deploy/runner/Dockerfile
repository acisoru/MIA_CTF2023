FROM ubuntu@sha256:7a57c69fe1e9d5b97c5fe649849e79f2cfc3bf11d10bbd5218b4eb61716aebe6

RUN apt-get update -y
RUN apt-get install xinetd dos2unix -y

RUN useradd -m limiteduser

COPY flag.txt /root/flag.txt
COPY run.sh /run.sh
COPY xinetd /etc/xinetd.d/minipython
COPY build.tar.gz /build.tgz

RUN chown root:root /root/flag.txt
RUN chmod 700 /root/flag.txt


RUN chown root:root /etc/xinetd.d/minipython
RUN chmod 700 /etc/xinetd.d/minipython

RUN tar xzvf build.tgz && rm build.tgz
RUN chown -R root:root /run.sh python Lib
RUN chmod -R 755 /run.sh python Lib
RUN chmod -R +x /run.sh python Lib

RUN chown root:root /give_me_flag
RUN chmod 4755 /give_me_flag
RUN chmod +x /give_me_flag

RUN dos2unix /etc/xinetd.d/minipython
