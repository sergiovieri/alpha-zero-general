import numpy as np
from subprocess import Popen, PIPE

class AlphaBetaPlayer():
	def __init__(self, game):
		self.game = game
		self.troops = Popen(['./AB'], stdin=PIPE, stdout=PIPE)

	def encode(self, board):
		board, ply = board
		args = "position "
		for i in range(board.shape[0]):
			for j in range(board.shape[0]):
				args += "{} ".format(board[i][j])
		args += "\n";
		args += "go depth 4\n"
		return args.encode()

	def play(self, board):
		self.troops.stdin.write(self.encode(board))
		self.troops.stdin.flush()
		bestmove = self.troops.stdout.readline().decode()
		while bestmove[:9] != "bestmove ":
			print("got " + bestmove, end='')
			bestmove = self.troops.stdout.readline().decode()

		print("now " + bestmove, end='')

		ori_r = int(bestmove[9])
		ori_c = int(bestmove[10])
		tar_r = int(bestmove[12])
		tar_c = int(bestmove[13])
		shoot_r = int(bestmove[15])
		shoot_c = int(bestmove[16])
		for i in range(len(self.game.actions)):
			if (ori_r, ori_c, tar_r, tar_c, shoot_r, shoot_c) == self.game.actions[i]:
				return i
		assert(False)
