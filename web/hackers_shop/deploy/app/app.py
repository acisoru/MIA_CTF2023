from flask import Flask, render_template, request, redirect

app = Flask(__name__)

@app.route("/")
def index():
    page = request.args.get('page', '')
    if page == '':
        return redirect("/?page=pages/inwork.html", 301) 
    print(page)
    try:
        with open(page, "r", encoding="utf-8") as f:
            content = f.read()
            return content
    except:
        return '<h1>404 Error. Page not found!</h1>', 404

if __name__ == "__main__":
    app.run()