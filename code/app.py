from dotenv import load_dotenv
import os

from flask import Flask, render_template, request, url_for, redirect

from models import UserModel,db,login
import time

from colorwebsites import colors as cw_blueprint

from login import security as login_blueprint
from login import login_manager

from mqtt import mqttc

app = Flask(__name__)

load_dotenv()
app.secret_key = os.getenv('secret_key')

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///users.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db.init_app(app)

login_manager.init_app(app)

app.register_blueprint(cw_blueprint)
app.register_blueprint(login_blueprint)

@app.before_first_request
def create_all():
    db.create_all()


if __name__ == "__main__":
   app.run(host='0.0.0.0', port=8181, ssl_context=('cert.pem', 'key.pem'))
