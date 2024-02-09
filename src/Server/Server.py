import time, json, os
from flask import Flask
app = Flask(__name__)

steps = {}
Pattern = ['1','2']
lastX = -1;lastY = -1
cntStep = 0
curPlayer = 0

@app.route('/')
def index():
    ret = '<h1>记录列表</h1><br />\n<table border="1">\n'
    for i in os.listdir():
        if '.json' in i:
            ret += f'<tr><td><a href="/log/{i}">{i}</a></td></tr>\n'
    return ret + '</table>'

@app.route('/log/<fn>')
def getLog(fn):    
    with open(fn,'r') as f:
        return json.loads(f.read())

@app.route('/up_to_date/<id>')
def checkUpToDate(id):
    return 'true' if Pattern[curPlayer] == id else 'false'

@app.route('/stroke/<id>/<x>/<y>/')
def Stroke(id,x,y):
    global curPlayer, lastX, lastY, cntStep
    if Pattern[curPlayer] != id:
        return 'false'
    else:
        curPlayer = int(not curPlayer)

    lastX = x;lastY = y
    cntStep += 1
    print (f'Stroke {x} {y}')
    steps[cntStep] = {
        'Player': int(not curPlayer),
        'x': x,
        'y': y
    }
    return 'true'

@app.route('/get_last/')
def getLast():
    return f'{cntStep} {Pattern[int(not curPlayer)]} {lastX} {lastY}'

@app.route('/game_over')
def gameOver():
    global steps,lastY,lastX,cntStep,curPlayer
    Filename = f'{int(time.time())}.json'
    print('Game Over. Saved as: ',Filename)
    with open(Filename,'w') as f:
        f.write(json.dumps(steps))
    
    steps = {}
    lastX = -1;lastY = -1
    cntStep = 0
    curPlayer = 0
    return 'true'

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=1234)