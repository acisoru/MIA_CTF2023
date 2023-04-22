# syntax=docker/dockerfile:1
FROM python:3.7
WORKDIR /task
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
COPY app ./
COPY requirements.txt ./
COPY flag.txt /
RUN pip3 install -r requirements.txt
CMD ["flask", "run", "-p", "8082"]
