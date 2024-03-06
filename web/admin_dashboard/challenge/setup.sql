CREATE DATABASE dashboard_db;

GRANT SELECT,INSERT,UPDATE ON dashboard_db.* TO 'admin'@'%' IDENTIFIED BY 'admin';
USE dashboard_db;

CREATE TABLE users (
        username varchar(100) PRIMARY KEY,
        password varchar(100),
        role varchar(10)
);

CREATE TABLE secrets (
        A BINARY(16),
        C BINARY(16)
);

INSERT INTO users (username, password, role) VALUES ('admin','a2e9ea8acee09604484a3c26e97005c969266df3a36f0ff67438a4d75b3b0b94','admin');
INSERT INTO secrets (A,C) VALUES (RANDOM_BYTES(16),RANDOM_BYTES(16));