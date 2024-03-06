FROM golang

COPY flag /flag

RUN mkdir /app
COPY main.go /app
COPY template.html /app
COPY go.mod /app
COPY go.sum /app

WORKDIR /app
RUN go build .

ENTRYPOINT ./easyssti
EXPOSE 3000
