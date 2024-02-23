import time, json, os
from flask import Flask
app = Flask(__name__)

steps = {}
Pattern = ['1','2']
lastX = -1;lastY = -1
cntStep = 0
curPlayer = 0

UseAITech = True #是否启用AI对战
if UseAITech:
    import numpy as np
    import tensorflow
    model = tensorflow.keras.models.load_model('renju_cnn.h5')
    input = np.zeros((20,20))

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

    if UseAITech:
        input[int(y),int(x)] = 1
        inputxx = np.expand_dims(input, axis=(0, -1)).astype(np.float32)
        output = model.predict(inputxx).squeeze()
        output = output.reshape((20,20))
        output_y, output_x = np.unravel_index(np.argmax(output), output.shape)
        output_x %= 15; output_y %= 15
        input[output_y,output_x] = -1

        curPlayer = int(not curPlayer)
        lastX = output_x;lastY = output_y
        cntStep += 1
        print (f'AI Stroke {output_x} {output_y}')
        print (f'Now Player={Pattern[int(not curPlayer)]}, x={lastX}, y={lastY}')
        steps[cntStep] = {
            'Player': int(not curPlayer),
            'x': lastX,
            'y': lastY
        }
    
    return 'true'

@app.route('/get_last/')
def getLast():
    return f'{cntStep} {Pattern[int(not curPlayer)]} {lastX} {lastY}'

@app.route('/game_over')
def gameOver():
    global steps,lastY,lastX,cntStep,curPlayer,UseAITech
    Filename = f'{int(time.time())}.json'
    print('Game Over. Saved as: ',Filename)
    with open(Filename,'w') as f:
        f.write(json.dumps(steps))
    
    steps = {}
    lastX = -1;lastY = -1
    cntStep = 0
    curPlayer = 0

    if UseAITech:
        input = np.zeros((20,20))
    return 'true'

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=1234)