# main.py: Provides the server client infra for gaming
# python2 server.py <port> <max_clients>
# YES YOU CAN ONLY USE PYTHON "2" and NOT "3".
import os, sys, socket, signal, game, json, humanSocket

if (os.path.realpath('../utils') not in sys.path):
	sys.path.insert(0, os.path.realpath('../utils'))
from log import *

DEBUG = False
connections = []
config = {}
sock = None

def loadConfig():
	global config
	f = open('config.txt', 'r')
	lines = f.readlines()
	for line in lines:
		line = line.strip()
		if (len(line) == 0 or line.startswith('#')):
			continue

		keyVal = list( map(lambda x: x.strip(), line.split(':') ) )
		config[keyVal[0]] = keyVal[1]

	# override config with sys args:
	if (len(sys.argv) > 1):
		config['port'] = int(sys.argv[1])

	if (len(sys.argv) > 2):
		config['max_clients'] = sys.argv[2]

	# TODO: Some keys are REQUIRED. maybe check that they are present.
	print ('Server config:', config)

def letClientsJoin():
	global connections, sock
	port = int(config.get('port'))
	hostname = '0.0.0.0'
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind((hostname, port))
	sock.listen(1)
	print ('Listening on %s:%d for clients for 20s' % (hostname, port))
	signal.signal(signal.SIGALRM, lambda x: (_ for _ in ()).throw(Exception('TimeOut')) )
	signal.alarm(20)
	try:
		while True:
			# Wait for connections
			print 'Waiting'
			conn, addr = sock.accept()
			botname = conn.recv(100) # a client must return information...
			# conn.setblocking(1)
			print('Bot ' + botname + ' connected')
			connections.append((conn, addr, botname))
			if (len(connections) >= int(config.get('max_clients') or 20) ):
				raise Exception('TimeOut')
	except:
		print (sys.exc_info()[0])
		pass
	
	signal.alarm(0)
	print('Timed out for joining...')


def printMenu():
	Warn('-----------------Tournament Manager-----------------')
	print('Bots connected: ')
	i = 0
	for (conn, addr, botname) in connections:
		print (str(i) + '. ' + botname + ' ' + repr(addr))
		i += 1
	print(bcolors.OKYELLOW + 'command examples: ' + bcolors.ENDC + \
		'"1 v 2", "1 v 2 v 4" (league - N.Y.S), \
		\n\tuse "h" as an index for human e.g "0 v h" \
		\n\tq for quitting (close clients first)')

def quit():
	for c in connections:
		c[0].close()
	sock.close()
	exit(0)

# can throw exception
def getPlayer(name_or_index):
	name_or_index = name_or_index.strip()
	if (DEBUG):
		Info('|' + name_or_index + '|')
	if (name_or_index == 'h'):
		return (humanSocket.HumanSocket(), -1, 'human')
	return connections[int(name_or_index)]

def beginGameGod():
	while True:
		printMenu()
		cmd = raw_input(bcolors.OKRED + "At your command: " + bcolors.ENDC)
		cmd = cmd.strip()
		if (cmd.startswith('q')):
			print ('quitting')
			quit()
		elif ('v' in cmd):
			try:
				players = list(map(lambda x: getPlayer(x), cmd.split('v')))
			except:
				Error('Bad Input')
				continue
			game.startWar(players[0], players[1])
	
if __name__ == "__main__":
	loadConfig()
	letClientsJoin()	# TODO: upgrade to letClientsJoinForever - threading thingy.
	beginGameGod()



# Maybe useful:
# Async input acceptance for a lively server.
# import select
# try:
#   if select.select([sys.stdin,], [], [], 0)[0]:
#     thisChar = sys.stdin.read(1)
#   else:
#     print ('went here')
# except IOError:
#   #nothing to read. should really have checked first
#   pass