# TA-AI.py
import sys
from random import random, choice
import os, sys
sys.path.insert(0, os.path.realpath('../utils'))
from log import *

def printX(*message):
	for msg in message:
		sys.stderr.write(repr(msg) + ' ') 
	sys.stderr.write('\n') 

N = int(raw_input())
ROLE = raw_input()
board = []
for i in range(0, N):
	boardRow = []
	for j in range(0, N):
		boardRow.append('-')
	board.append(boardRow)

def isGameOver():
	for i in range(0, N):
		for j in range(0, N):
			if (board[i][j] == '-'):
				return False
	return True


## --------------------

def getPossibleOrderMoves(x, y):
	possibleMoves = []

	for iterator in range(x-1,-1,-1):
		if board[iterator][y]=='-':
			possibleMoves.append((iterator,y))
		else:
			break

	for iterator in range(y-1,-1,-1):
		if board[x][iterator]=='-':
			possibleMoves.append((x,iterator))
		else:
			break

	for iterator in range(x+1,N):
		if board[iterator][y]=='-':
			possibleMoves.append((iterator,y))
		else:
			break

	for iterator in range(y+1,N):
		if board[x][iterator]=='-':
			possibleMoves.append((x,iterator))
		else:
			break

	return possibleMoves


def chaosAI(piece):
	openSquares=[]
	for x in xrange(N):
		for y in xrange(N):
			if board[x][y]=="-":
				openSquares.append((x,y))
	openSquares
	return choice(openSquares)


def orderAI():
	capturedSquares=[]
	for x in xrange(N):
		for y in xrange(N):
			if board[x][y]!="-":
				capturedSquares.append((x,y,board[x][y]))

	capturedSquares = sorted(capturedSquares, key=lambda t:(t[0],t[1]))

	while(True):
		fromPosition = choice(capturedSquares)
		possibleMoves = getPossibleOrderMoves(fromPosition[0], fromPosition[1])
		if len(possibleMoves)!=0:
			mv = choice(possibleMoves)
			ans = (fromPosition[0], fromPosition[1], mv[0], mv[1])
			break

	return ans


## --------------------
# some fancy stuff...
COLORS = [bcolors.OKRED, bcolors.OKCYAN, bcolors.OKGREEN, bcolors.OKBLUE, bcolors.OKYELLOW, bcolors.OKWHITE]
TEXTCONV = {'A': 'R', 'B': 'C', 'C': 'G','D':'B', 'E':'Y', '-':'-'}
def color(tile): # character
	index = ord(tile) - ord('A')
	if (tile == '-'):
		index = 5
	return COLORS[index] + TEXTCONV[tile] + bcolors.ENDC

def printBoard():
	for x in xrange(N):
		print >>sys.stderr,  "".join( list( map( lambda x: color(x), board[x] ) ) )
	print >>sys.stderr, '\n'


# returns if the move was successful or not
def makeChaosMove(x, y, color):
	global board
	if (board[x][y] != '-'):
		return False
	board[x][y] = color 
	return True
	
# returns if the move was successful or not
def makeOrderMove(a, b, c, d):
	global board	
	if (a == c and b == d):
		return
	board[c][d] = board[a][b]
	board[a][b] = '-'
	return True

def playAsOrder():
	printX('ORDER')
	while True:
		# printBoard()
		line = raw_input()
		# printX ('LINE:', line)
		(x, y, color) = line.split(' ')
		(x, y) = (int(x), int(y))
		board[x][y] = color
		
		if (isGameOver()):
			return

		(a, b, c, d) = orderAI()
		makeOrderMove(a, b , c, d)
		# printBoard()
		print '%d %d %d %d' % (a, b, c, d)
		sys.stdout.flush()

	
def playAsChaos():
	printX('CHAOS')
	color = raw_input()
	(x, y) = chaosAI(color)
	board[x][y] = color
	print '%d %d' %(x, y)
	printBoard()

	while True:
		if (isGameOver()):
			return
		his_move = raw_input()
		# printX ('his move: %s'%his_move)
		(a, b, c, d) = map(lambda x: int(x), his_move.split(' '))
		makeOrderMove(a, b, c, d)
		color = raw_input()
		(x, y) = chaosAI(color)
		board[x][y] = color
		# printBoard()
		print '%d %d' %(x, y)
			

if (ROLE == 'ORDER'):
	playAsOrder()
elif(ROLE == 'CHAOS'):
	playAsChaos()
else:
	print >> sys.stderr, 'I am not intelligent for this role: %s' %ROLE

printX ('--graceful exit by TA-AI--')
