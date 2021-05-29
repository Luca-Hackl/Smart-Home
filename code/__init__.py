from flask import Blueprint

main = Blueprint('pythonfiles', __name__)

from . import colorwebsites, mqtt
