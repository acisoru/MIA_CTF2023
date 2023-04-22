from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
from threading import Thread
import sys
from urllib.parse import urljoin
admin_creds = ("Andrey_Ivanovich", "qwerty123321qwerty777")
try:
    options = Options()
    options.add_argument("--headless")
    options.add_argument('--no-sandbox')
    options.add_argument('start-maximized')
    options.add_argument('disable-infobars')
    options.add_argument("--disable-extensions")
    browser = webdriver.Chrome(options=options)
    url = sys.argv[1]
    browser.get(url)
    try:
        WebDriverWait(browser, 5).until(EC.presence_of_element_located((By.CLASS_NAME, "login-container")))
    except TimeoutException:
        raise Exception('Page loading failed or url changed')
    # sleep without thread freeze
    try:
        WebDriverWait(browser, 5).until(EC.presence_of_element_located((By.CLASS_NAME, "sleeping")))
    except: pass
    if urljoin(browser.current_url, '/') != urljoin(url, '/'):
        raise Exception('Url changed')
    username_input = browser.find_element(By.NAME, 'username')
    password_input = browser.find_element(By.NAME, 'password')
    submit_button = browser.find_element(By.XPATH, '//button[@type="submit"]')
    username_input.send_keys(admin_creds[0])
    password_input.send_keys(admin_creds[1])
    try:
        WebDriverWait(browser, 2).until(EC.presence_of_element_located((By.CLASS_NAME, "sleeping")))
    except: pass
    submit_button.click()
    # sleep without thread freeze
    try:
        WebDriverWait(browser, 1).until(EC.presence_of_element_located((By.CLASS_NAME, "sleeping")))
    except: pass
    browser.quit()
except Exception as ex:
    print(f"ERROR: {ex}", flush=True)
