import subprocess,time, os, pyaudio, sys, requests

if len(sys.argv) == 1 or sys.argv[1] == '-h' or sys.argv[1] == '--help':
    print("(c) 2024 2362401031 Soochow University")
    print ("Bridge.exe [-h/--help] [-r/--record] [-s/--send]")
    print ("-h/--help: Show this page.")
    print ("-r/--record: Record the screen to ./out.mp4, touch ./_stop to stop")
    print ("-g/--get [URL]: Send message to the server method 'GET' at '[URL]', get result at ./_res")
    sys.exit(0)
elif sys.argv[1] == '-r' or sys.argv[1] == '--record':
    dir = os.getcwd()
    stop_dir = dir + '\\_stop';
    out_dir = dir + '\\out.mp4';
    ffmpeg_dir = dir + '\\ffmpeg.exe'

    def findInternalRecordingDevice():
        p = pyaudio.PyAudio()
        target = '麦克风'
        for i in range(p.get_device_count()):
            devInfo = p.get_device_info_by_index(i)   
            if devInfo['name'].find(target)>=0 and devInfo['maxInputChannels']>=2:      
                return devInfo['name']
        print('No Audio Device Found!')
        sys.exit(0)

    if os.path.exists(stop_dir):
        os.unlink(stop_dir)
    if os.path.exists(out_dir):
        os.unlink(out_dir)

    try:
        obj = subprocess.Popen(f'{ffmpeg_dir} -f gdigrab -i desktop -f dshow -i audio="{findInternalRecordingDevice()}" -pix_fmt yuv420p {out_dir}',shell=True, stdin=subprocess.PIPE)
        while not os.path.exists(stop_dir):
            pass

        time.sleep(2)
        obj.stdin.write('q'.encode('utf-8'))
        obj.communicate(timeout=15)
    except subprocess.TimeoutExpired:
        obj.kill()
elif sys.argv[1] == '-g' or sys.argv[1] == '--get':
    if os.path.exists('_res'):
        os.unlink('_res')
    try:
        r = requests.get(sys.argv[2])
        if r.status_code != 200:
            print('Error Connecting Server: ', r.status_code)
        
        with open('_res','w') as f:
            f.write(r.text)
    except Exception as e:
        print("Error:",e)
