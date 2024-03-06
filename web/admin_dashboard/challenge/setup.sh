# curl -LsS -O https://downloads.mariadb.com/MariaDB/mariadb_repo_setup
# bash mariadb_repo_setup --mariadb-server-version=10.10
# apt update
# apt install mariadb-server mariadb-client -y
service php7.3-fpm start
mysqld_safe &

while ! mysql -e "show databases;"; do # Wait for MariaDB
  sleep 1
done

mysql < /setup.sql
nginx
while true; do sleep 1000; done