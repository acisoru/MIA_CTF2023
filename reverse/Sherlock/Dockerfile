FROM ubuntu
RUN apt-get update -y && apt-get install -y tcpd xinetd
RUN useradd elf
COPY app/server-config /etc/xinetd.d/server
COPY app/bin/server /server
COPY app/files/flag.txt /files/flag.txt
COPY app/files/elf /files/elf
COPY app/files/targz /files/zip
COPY app/files/targz /files/targz
RUN chmod 666 /files/flag.txt
RUN chmod 666 /files/elf
RUN chmod 666 /files/zip
RUN chmod 666 /files/targz
EXPOSE 7878
CMD xinetd -dontfork
