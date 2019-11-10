# flaskapp.py

from flask import Flask, render_template, request
import requests, json as jconvert
app = Flask(__name__)

speed = 500
color = 500
bright = 255
motion = "off"
rainbow = "off"

url = 'https://api.particle.io/v1/devices/johnnyruz-particle-1'
key = '457ad875041a62a3912cc13e1bef7e9130b45a62'


@app.route("/", methods=["GET","POST"])
def index():

	path = getPath('conditions')
	print(path)

	isOffline = False

	try:
		r = requests.get(path, timeout=5)
		json = r.json()


		if 'result' in json:
			result = json['result']

			resultJson = jconvert.loads(result);

			if 'speed' in resultJson:
				global speed
				speed = resultJson['speed']

			if 'color' in resultJson:
				global color
				color = resultJson['color']

			if 'brightness' in resultJson:
				global bright
				bright = resultJson['brightness']

			if 'motion' in resultJson:
				global motion
				motion = "off" if resultJson['motion'] == 0 else "on"

			if 'rainbow' in resultJson:
				global rainbow
				rainbow = "off" if resultJson['rainbow'] == 0 else "on"

	except:
		print("IS OFFLINE")
		isOffline = True

	finally:

		return render_template("index.html", speed = speed, color = color, bright = bright, motion = motion, rainbow = rainbow, isOffline = isOffline)


@app.route("/id2", methods=["GET"])
def index2():

	return render_template("index2.html", speed = speed, color = color, motion = motion, rainbow = rainbow)


@app.route("/updateMotion", methods=["POST"])
def updateMotion():

	if request.method == "POST":

		switch = request.form['data']
		toggleVal = "on" if switch == "true" else "off"

		payload = dict(arg=toggleVal)
		headers = {'content-type': 'application/json'}

		path = getPath('motion')

		r = requests.post(path, payload, headers)

		json = r.json()

		if 'return_value' in json:
			print("Return Value: " + str(json['return_value']))
			print('Motion toggle: ' + toggleVal)

			global motion
			motion = toggleVal

			return "Success"

		return "Error"

@app.route("/updateRainbow", methods=["POST"])
def updateRainbow():

	if request.method == "POST":
		switch = request.form['data']
		toggleVal = "on" if switch == "true" else "off"

		payload = dict(arg=toggleVal)
		headers = {'content-type': 'application/json'}

		r = requests.post('https://api.particle.io/v1/devices/johnnyruz-particle-1/rainbow?access_token=457ad875041a62a3912cc13e1bef7e9130b45a62', payload, headers)

		json = r.json()

		if 'return_value' in json:
			print("Return Value: " + str(json['return_value']))
			print('Rainbow Toggle: ' + switch)

			global rainbow
			rainbow = toggleVal	

			return "Success"

		return "Error"

@app.route("/updateSpeed", methods=["POST"])
def updateSpeed():

	if request.method == "POST":

		speedVal = request.form['data']
		print("Speed:" + str(speedVal))

		payload = dict(arg=str(speedVal))
		headers = {'content-type': 'application/json'}

		r = requests.post('https://api.particle.io/v1/devices/johnnyruz-particle-1/speed?access_token=457ad875041a62a3912cc13e1bef7e9130b45a62', payload, headers)

		json = r.json()
		print("Return Value: " + str(json['return_value']))
		print('Set Speed: ' + speedVal)

		global speed
		speed = speedVal;

		return "Success"

@app.route("/updateBright", methods=["POST"])
def updateBright():

	if request.method == "POST":

		brightVal = request.form['data']
		print("Speed:" + str(brightVal))

		payload = dict(arg=str(brightVal))
		headers = {'content-type': 'application/json'}

		r = requests.post('https://api.particle.io/v1/devices/johnnyruz-particle-1/bright?access_token=457ad875041a62a3912cc13e1bef7e9130b45a62', payload, headers)

		json = r.json()
		print("Return Value: " + str(json['return_value']))
		print('Set Brightness: ' + brightVal)

		global bright
		speed = brightVal;

		return "Success"

@app.route("/updateColor", methods=["POST"])
def updateColor():

	if request.method == "POST":

		colorVal = request.form['data']

		payload = dict(arg=colorVal)
		headers = {'content-type': 'application/json'}

		r = requests.post('https://api.particle.io/v1/devices/johnnyruz-particle-1/color?access_token=457ad875041a62a3912cc13e1bef7e9130b45a62', payload, headers)

		json = r.json()
		print("Return Value: " + str(json['return_value']))
		print('Set Color: ' + colorVal)

		global color
		color = colorVal;

		return "Success"

def getPath( p ):
	return str(url) + '/' + str(p) + '?access_token=' + str(key)


if __name__ == "__main__":
	app.run(host='0.0.0.0')
