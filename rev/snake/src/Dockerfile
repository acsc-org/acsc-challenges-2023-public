FROM node:16-bullseye-slim

RUN apt-get -y update --fix-missing
RUN apt-get -y upgrade
RUN apt-get -y install socat python3 python3-pip
RUN pip3 install lief numpy
RUN groupadd -r ctf && useradd -r -g ctf ctf
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/ctf
ADD server/flag.txt .
ADD server/server.py   .
ADD server/Makefile   .
ADD server/rng.c   .
ADD distfiles/snake    .
RUN make
RUN chmod 440 flag.txt
RUN chmod 440 snake
RUN chmod 550 server.py

RUN chown -R root:ctf /home/ctf

USER ctf
CMD socat TCP-L:4444,reuseaddr,fork EXEC:"./server.py"