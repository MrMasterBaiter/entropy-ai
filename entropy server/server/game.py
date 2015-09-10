# game.py: The refree for the game.
# Does not support async yet - once started, it shall end till the game is finished!
# TODO: make a separate class "boardManager" to keep it modular.

import random, json, time, humanSocket
import copy

gameN = 5
board = []
pcs = []
timers = [60.0, 60.0]

import os, sys
if (os.path.realpath('../utils') not in sys.path):
	sys.path.insert(0, os.path.realpath('../utils'))
from log import *

COLORS = [bcolors.OKRED, bcolors.OKCYAN, bcolors.OKGREEN, bcolors.OKBLUE, bcolors.OKYELLOW, bcolors.OKWHITE]
TEXTCONV = {'A': 'R', 'B': 'C', 'C': 'G','D':'B', 'E':'Y', '-':'-'}
def color(tile): # character
	index = ord(tile) - ord('A')
	if (tile == '-'):
		index = 5
	return COLORS[index] + TEXTCONV[tile] + bcolors.ENDC

def printBoardX(b):
	global timers
	pr_okblue('Timers ORDER: %.3f, CHAOS: %.3f'% (timers[0], timers[1]))
	pr_okred('   0 1 2 3 4 (cols)')
	for x in xrange(gameN):
		print str(x) + '  ' +  " ".join( list( map( lambda x: color(x), b[x] ) ) )
	print ''

def printBoard():
	printBoardX(board)


def resetSelf():
	global board, pcs, timers
	timers = [60.0, 60.0]
	pcs = []
	board = []
	for i in range(0, gameN):
		boardRow = []
		for j in range(0, gameN):
			pcs.append(chr(i+ord('A')))
			boardRow.append('-')
		board.append(boardRow)
	random.shuffle(pcs)
	# print(board)
	# print(pcs)

def isGameOver():
	if (timers[0] < 0 or timers[1] < 0):
		return True

	for i in range(0, gameN):
		for j in range(0, gameN):
			if (board[i][j] == '-'):
				return False
	return True

def isValidMoveByOrder(a, b, c, d):
	if (a not in range(0, gameN) or \
		b not in range(0, gameN) or \
		c not in range(0, gameN) or \
		d not in range(0, gameN) \
	):
		return False

	if (a != c and b != d):
		return False
	
	# if (a == c and b == d):
	# 	# is it a valid move to pass? - idk
	# 	return False
		
	if (a == c):
		for col in range(b+1, d):
			if (board[a][col] != '-'):
				return False
	
	if (b == d):
		for row in range(a+1, c):
			if (board[row][b] != '-'):
				return False
	
	return True

# returns if the move was successful or not
def makeChaosMove(x, y, color):
	global board
	if (x not in range(0, gameN) or \
		y not in range(0, gameN) \
	):
		return False

	if (board[x][y] != '-'):
		return False
	board[x][y] = color 
	return True
	
# returns if the move was successful or not
def makeOrderMove(a, b, c, d):
	global board
	if not isValidMoveByOrder(a, b, c, d):
		return False
	
	if (a == c and b == d):
		return True

	board[c][d] = board[a][b]
	board[a][b] = '-'
	return True


def nextColor():
	return pcs.pop(0)

# row is a list of colored items
# TEST this thingy
def scoreHelp(row):
	MAX = len(row)
	isOk = lambda x: True if x >= 0 and x < MAX and row[x] != '-' else False
	score = 0 
	for ind in range(1, MAX):
		# epicenter b/w ind-1 and ind
		length = 0
		scoreX = 0
		right = ind
		left = ind - 1
		while isOk(right) and isOk(left) and row[left] == row[right]:
			scoreX += (length+2); length += 2; right += 1; left -= 1
		score += scoreX
		
		# epicenter at ind
		length = 1 
		scoreX = 0
		right = ind + 1
		left = ind - 1
		while isOk(right) and isOk(left) and row[left] == row[right]:
			scoreX += (length + 2); length += 2; right += 1; left -= 1
		score += scoreX
	return score

def calculateScore():
	if (timers[0] < 0):
		Error('Penalizing ORDER:')
		return 0
	elif (timers[1] < 0):
		Error('Penalizing CHAOS:')
		return 100	# Some REASONABLY good score should be given!

	score = 0
	for rowList in board:
		score += scoreHelp(rowList)
	
	for col in range(0, gameN):
		colList = []
		for row in range(0, gameN):
			colList.append(board[row][col])
		score += scoreHelp(colList)
	
	return score
	
# -----------------------------------------
def updateTimer(player, delta):
	# If a player is human, its timer is never incremented by much (blocking trick)
	global timers
	if (player == 'ORDER'):
		timers[0] -= delta
	elif (player == 'CHAOS'):
		timers[1] -= delta

# Give them the ultimate penalty!!
def killChaos():
	updateTimer('CHAOS', 1000)

def killOrder():
	updateTimer('ORDER', 1000)

# -----------------------------------------
# TODO: Too much optimism here - can be a little pessimistic (and watchful)
# GURANTEE: If the client.py is not tampered, AND the network is not physically 
#			disrupted Then a response is guranteed
def getResponse(connection):
	# print 'waiting for:', connection
	data = connection[0].recv(1024)
	# print 'data = ' + data + str(len(data))
	return data
		
def resetClient(connection):
	data = json.dumps({'purpose': 'reset'})
	connection[0].send(data)

def initOrder(connection):
	resetClient(connection)
	time.sleep(0.1)
	payload = '%d\nORDER\n' %gameN
	data = json.dumps({'payload': payload, 'purpose': 'update_only'})
	connection[0].send(data)

def initChaos(connection, color):
	resetClient(connection)
	time.sleep(0.1)
	payload = payload = '%d\nCHAOS\n%s\n' %(gameN, color)
	data = json.dumps({'payload': payload, 'purpose': 'update'})
	connection[0].send(data)
	return getResponse(connection)

def playBattle(order, chaos):
	resetSelf()
	printBoard()
	# pre game data:
	initOrder(order)
	color = nextColor()
	
	lastTime = time.time()
	chaos_reply = json.loads(initChaos(chaos, color))
	delta = time.time() - lastTime
	updateTimer('CHAOS', delta)

	if (chaos_reply['status'] == False):
		print ('client side problema by %s (chaos): %s' % (chaos[2], chaos_reply))
		killChaos()
		handlePremature(chaos_reply, chaos)
		return
	
	while not isGameOver():
		move = chaos_reply['payload']
		lastMoveWasInvalid = False
		try:
			(x, y) = move.strip().split(' ')
			(x, y) = (int(x), int(y))
		except:
			lastMoveWasInvalid = True
				
		if lastMoveWasInvalid or not makeChaosMove(x, y, color):
			print ('bad move by %s (chaos)' % chaos[2])
			killChaos()
			handlePremature(chaos_reply, chaos)
			return
		
		printBoard()
		payloadToOrder = '%d %d %s\n' %(x, y, color)
		data = {'purpose': 'update', 'payload': payloadToOrder}
		if isGameOver():
			data['purpose']= 'update_only'			
			order[0].send(json.dumps(data))
			print 'GAME END -------RESULTS-------\n'
			return

		order[0].send(json.dumps(data))
		
		# ---------------------------------------		
		lastTime = time.time()
		order_reply = json.loads(getResponse(order))
		delta = time.time() - lastTime
		updateTimer('ORDER', delta)

		if (order_reply['status'] == False):
			print ('client side problema by %s (order): %s' % (order[2], order_reply))
			killOrder()
			handlePremature(order_reply, order)
			return
		
		lastMoveWasInvalid = False
		try:		
			(a, b, c, d) = list( \
				map(lambda x: int(x), order_reply['payload'].strip().split(' ')) \
			)
		except:
			lastMoveWasInvalid = True

		if lastMoveWasInvalid or not makeOrderMove(a, b, c, d):
			print ('bad move by %s (order) ' % order[2])
			killOrder()
			handlePremature(order_reply, order)
			return

		printBoard()
		if isGameOver():
			print 'GAME END (bad) -------RESULTS-------\n'
			return

		color = nextColor()
		payloadToChaos = '%d %d %d %d\n%s\n'%(a, b, c, d, color)
		data = {'purpose': 'update', 'payload': payloadToChaos}
		chaos[0].send(json.dumps(data))
		
		lastTime = time.time()
		chaos_reply = json.loads(getResponse(chaos))
		delta = time.time() - lastTime
		updateTimer('CHAOS', delta)

		if (chaos_reply['status'] == False):
			print ('client side problema by %s (chaos): %s' % (chaos[2], chaos_reply))
			killChaos()
			handlePremature(chaos_reply, chaos)
			return

# do something about this!
def handlePremature(response, culprit):
	print 'game prematurely ended. last response: %s'%response
	
# A war has many battles
# lets have 2 games to be fair of sides 
# (but what about luck on which pieces one gets?)
def startWar(bot1, bot2):
	global board
	scores = []
	playBattle(bot1, bot2)
	scores.append(calculateScore())
	board1 = copy.deepcopy(board)
	
	print 'sides change, %s vs %s:' %(bot2[2], bot1[2])
	playBattle(bot2, bot1)
	scores.append(calculateScore())
	board2 = copy.deepcopy(board)

	if (scores[0] > scores[1]):
		pr_blink('\tBOT: %s WINS'%bot1[2])
	elif (scores[1] > scores[0]):
		pr_blink('\tBOT: %s WINS'%bot2[2])
	else:
		pr_blink('\tDRAW')

	print '%s as ORDER: %d'%(bot1[2], scores[0])
	printBoardX(board1)
	print '%s as ORDER: %d'%(bot2[2], scores[1])
	printBoard()
	print repr(scores)

# optimism
def testScoreHelp():
	row = list('-ABBA--AA')
	ans = scoreHelp(row)
	if (ans != 8):
		raise Exception('scoring fn err. %d'%ans)
	else:
		print 'scoring working properly (hopefully)'

if __name__ == '__main__':
	print 'running unit tests...'
	testScoreHelp()
