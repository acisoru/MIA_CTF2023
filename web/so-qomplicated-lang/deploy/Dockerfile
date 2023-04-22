FROM golang:alpine

WORKDIR /app

COPY *.go ./
COPY go.* ./
COPY styles styles/
COPY templates templates/


RUN go mod download
RUN go build -o appbin
