# client.py: 
# python2 client.py <port> <address> <botname> <path_to_run.sh> <delay>
#  purpose: 
# 	- language independence for AI coders
#	- better control for killing, resetting
# 	- tournament can be (partially/fully) automated at server side

# TO TEST: what if a client segfaults - hopefully controlled?

from subprocess import Popen, PIPE
import sys, socket, json, time
import os
sys.path.insert(0, os.path.realpath('../utils/'))
from nbstreamreader import NonBlockingStreamReader as NBSR

botname = 'bazooka'	 # PLEASE CHOOSE A UNIQUE BOTNAME

SAFETY_TIMEOUT = 65 # For dumb clients, who think too much or die while thinking
server_address = 'localhost'
server_port = 8000
aiProc = None
advStdout = None
pathToRun = 'run.sh'

# print >>sys.stderr, repr(sys.argv)

if (len(sys.argv) > 1):
	server_port = int(sys.argv[1])

if (len(sys.argv) > 2):
	server_address = sys.argv[2]

if (len(sys.argv) > 3):
	botname = sys.argv[3]

if (len(sys.argv) > 4):
	pathToRun = sys.argv[4]

if (len(sys.argv) > 5):
	delay = float(sys.argv[5])
	time.sleep(delay)

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect((server_address, server_port))
clientsocket.send(botname)

print '%s connected to server (hopefully)'% botname
if (botname != 'TA-AI'):
	print ('extra options: python client.py <optional-port> <address>')

def resetCode(args):
	global aiProc, advStdout
	if (aiProc != None):
		aiProc.kill()
		
	aiProc = Popen (['sh', pathToRun], stdin = PIPE, stdout = PIPE, bufsize=0)
	advStdout = NBSR(aiProc.stdout)

def writeToProc(payload):
	try:
		aiProc.stdin.write(payload)
	except:
		print 'Something bad happened with the client (# need to kill it (X1)?)'

def updateCode(args):
	try:
		writeToProc(args['payload'])
		my_move = advStdout.readline(SAFETY_TIMEOUT)
	except:
		print 'Something bad happened with the client (# need to kill it (X2)?)'
		my_move = None
	
	print 'My move (%s): %s'%(botname, my_move)
	if not my_move:
		# idiot bot still thinking :-/
		payload = json.dumps({'status': False, 'comment': 'timeout'})
	else:
		payload = json.dumps({'payload': my_move, 'status': True})
	clientsocket.send(payload)
	
safety_count = 1
SAFETY_MAX_CHATTER = 1000 # this is safe unless n > 
# wait for command...
while True:
	data = clientsocket.recv(1024)
	safety_count += 1
	if (safety_count > SAFETY_MAX_CHATTER):
		print 'too much talking: killing and exiting \
			\n - this is not student\'s fault (hopefully). Sorry'
		aiProc.kill()
		exit(0)
		
	if (len(data) <= 0):
		continue
	
	dct = json.loads(data)
	# print 'packet recv:', dct
	
	if (dct['purpose'] == 'reset'):
		safety_count = 0
		resetCode(dct)
	
	elif (dct['purpose'] == 'kill'):
		safety_count = 0
		aiProc.kill()
	
	elif (dct['purpose'] == 'update'): # this is actually update and wait for output
		updateCode(dct)
	
	elif (dct['purpose'] == 'update_only'):
		writeToProc(dct['payload'])
		




	