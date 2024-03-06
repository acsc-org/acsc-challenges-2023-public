FROM node:14.21-slim
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install nano apt-utils nginx php7.3-fpm php7.3-mysql php7.3-gmp curl software-properties-common gnupg2 libfontconfig1 libgconf-2-4 libatk1.0-0 libatk-bridge2.0-0 libgdk-pixbuf2.0-0 libgtk-3-0 libgbm-dev libnss3-dev libxss-dev libasound2 -y

RUN curl -LsS -O https://downloads.mariadb.com/MariaDB/mariadb_repo_setup && bash mariadb_repo_setup --mariadb-server-version=10.10 && apt update && apt install mariadb-server mariadb-client unzip -y

COPY challenge/nginx.conf /etc/nginx/nginx.conf

RUN rm -rf /var/www/html/*
COPY challenge/files/ /var/www/html
COPY challenge/bot.zip /
RUN unzip bot.zip && rm bot.zip

COPY challenge/setup.sql /setup.sql
COPY challenge/setup.sh /setup.sh
RUN chmod +x /setup.sh
RUN cd /bot && yarn
RUN cp -r /root/.cache /var/www

ENTRYPOINT /setup.sh