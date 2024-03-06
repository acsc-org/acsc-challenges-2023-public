FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing
RUN apt-get -y upgrade
RUN apt-get -y install xinetd
RUN groupadd -r pwn && useradd -r -g pwn pwn

ADD challenge/pwn.xinetd /etc/xinetd.d/pwn
ADD challenge/init.sh /etc/init.sh
ADD challenge/run.sh /home/pwn/run.sh
RUN chmod 700 /etc/init.sh
RUN chmod 550 /home/pwn/run.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

WORKDIR /home/pwn
ADD challenge/flag.txt flag.txt
ADD distfiles/chall chall
RUN chmod 440 flag.txt
RUN chmod 550 chall
RUN mv flag.txt flag-$(md5sum flag.txt | awk '{print $1}').txt

RUN chown -R root:pwn /home/pwn
