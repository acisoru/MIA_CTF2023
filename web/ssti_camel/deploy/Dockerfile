FROM python:3.10.8
WORKDIR /task
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt
EXPOSE 5555
COPY . .
CMD ["flask", "run", "-p", "5555"]