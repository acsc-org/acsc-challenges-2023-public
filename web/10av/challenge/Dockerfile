FROM debian:bullseye
MAINTAINER <Orange Tsai> orange@chroot.org

EXPOSE 5000/tcp

RUN apt update && DEBIAN_FRONTEND=noninteractive apt install --no-install-recommends clamav clamav-daemon clamdscan postfix mailutils python3-pip -y
RUN pip install flask

RUN /usr/bin/freshclam
RUN mkdir /var/run/clamav/ /app/ /data/

COPY files/app.py           /app/
COPY files/entry.sh         /
COPY files/clamd.conf       /etc/clamav/
COPY files/10mail           /etc/clamav/virusevent.d/
COPY files/openmediavault   /etc/default/

COPY files/readflag         /
COPY files/flag             /


RUN chmod 777 /var/run/clamav/
RUN chmod 400 /flag
RUN chmod +x /readflag /app/app.py /entry.sh /etc/clamav/virusevent.d/10mail
RUN chmod +s /readflag

WORKDIR /app/

CMD ["/entry.sh"]
