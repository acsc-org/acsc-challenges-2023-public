FROM python:3.10-slim-buster

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -yqq socat
ADD challenge/task.py task.py
ADD challenge/flag flag

CMD socat TCP-L:13579,fork,reuseaddr EXEC:"python task.py"