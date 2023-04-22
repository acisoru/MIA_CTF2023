from flask import Flask, render_template, request
import subprocess

app = Flask(__name__)

admin_creds = ("Andrey_Ivanovich", "qwerty123321qwerty777")

@app.route('/')
def index():
    return render_template('main.html')


@app.route("/login", methods=["POST", "GET"])
def login():
    if request.method == "GET":
        title = request.args.get('title', 'Автоматизированная информационная система розыска преступников "Глаз"')
        return render_template('login.html', title=title)
    elif request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        if (username, password) == admin_creds:
            return "<h1>CTF{D4ngl1ng_M4rkup_Inj3ct10n}</h1>"
        else:
            return "<h1>Invalid login</h1>"

@app.route("/send", methods=["POST"])
def send():
    url = request.form["link"]
    print("opening browser", flush=True)
    subprocess.Popen(['python3', 'checker.py', url])
    return '<h1>Спасибо за ваше предложение!</h1>'

if __name__ == "__main__":
    app.run(debug=True)
