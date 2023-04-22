import requests

for n in range(len('CTF{w1r3sh@rk_enj0y3r}')):
    x = requests.get(f'http://127.0.0.1:8080/flag?symbol={n}')
    print(n, x.text)
