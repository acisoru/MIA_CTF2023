FROM golang:alpine

WORKDIR /app_flag_srv
COPY src/flag_srv.go src/go.* /app_flag_srv/
RUN go build -o /bin/app_flag_srv

WORKDIR /app_main
COPY src/main.go src/go.* src/*.html /app_main/
RUN go build -o /bin/app_main
