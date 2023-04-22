from flask import *
import random

app = Flask(__name__, static_folder='static')


@app.route('/')
def home():
    return render_template('main.html')


@app.route('/result', methods=['GET', 'POST'])
def result():
    camels_amount = random.randint(1, 99) + random.randint(1, 10)
    if request.method == 'POST':
        data = [render_template_string(request.form['shortcode']), camels_amount]
        return render_template("result.html", data=data)


if __name__ == "__main__":
    app.run(port=5555)
