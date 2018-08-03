from __future__ import print_function
import sys
sys.path.append('..')
from Game import Game
from .TroopsLogic import Board
import numpy as np


class TroopsGame(Game):

    __moves = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

    def __init__(self, n, args):
        self.n = n
        self.args = args

        print("precompute actions")
        # precompute actions
        self.actions = []
        for origin_r in range(n):
            for origin_c in range(n):
                if origin_r == 0 and origin_c == 0: continue
                if origin_r == 0 and origin_c == self.n - 1: continue
                if origin_r == self.n - 1 and origin_c == 0: continue
                if origin_r == self.n - 1 and origin_c == self.n - 1: continue

                for direction_r, direction_c in self.__moves:
                    target_r = origin_r + direction_r
                    target_c = origin_c + direction_c

                    if target_r == 0 and target_c == 0: continue
                    if target_r == 0 and target_c == self.n - 1: continue
                    if target_r == self.n - 1 and target_c == 0: continue
                    if target_r == self.n - 1 and target_c == self.n - 1: continue
                    if 0 <= target_r < n and 0 <= target_c < n:
                        for delta_r in range(-1, 2):
                            for delta_c in range(-1, 2):
                                shoot_r = target_r + delta_r
                                shoot_c = target_c + delta_c
                                if 0 <= shoot_r < n and 0 <= shoot_c < n:
                                    self.actions.append((origin_r, origin_c, target_r, target_c, shoot_r, shoot_c))

        

        print("sz %d" % len(self.actions))

    def getInitBoard(self):
        # return initial board (numpy board)
        b = Board(self.n)
        return np.array(b.pieces), 0

    def getBoardSize(self):
        # (a,b) tuple
        return (self.n, self.n)

    def getActionSize(self):
        # return number of actions
        return len(self.actions)

    def getNextState(self, board, player, action):
        # if player takes action on board, return next (board,player)
        # action must be a valid move
        board, ply = board
        b = Board(self.n)
        b.pieces = np.copy(board)
        b.execute_move(self.actions[action], player)
        return ((b.pieces, ply + 1), -player)

    def getValidMoves(self, board, player):
        # return a fixed size binary vector
        board, ply = board
        valids = [0]*self.getActionSize()
        b = Board(self.n)
        b.pieces = np.copy(board)
        for idx, action in enumerate(self.actions):
            if b.is_legal(action, player): valids[idx] = 1
        return np.array(valids)

    def getGameEnded(self, board, player):
        board, ply = board
        if ply > self.args.drawThreshold: return 2
        b = Board(self.n)
        b.pieces = np.copy(board)
        num = [0, 0, 0]
        for i in [0, self.n - 1]:
            for j in [0, self.n - 1]:
                num[b.pieces[i][j] + 1] += 1

        if num[0] != num[2]:
            if num[0] > num[2]:
                assert(num[0] == 2 and num[2] == 1)
                return -player
            else:
                assert(num[0] == 1 and num[2] == 2)
                return player

        assert(num[0] == 2)

        for i in range(self.n):
            for j in range(self.n):
                num[b.pieces[i][j] + 1] -= 1

        if num[0] == 0:
            return player
        if num[2] == 0:
            return -player

        return 0

    def getCanonicalForm(self, board, player):
        # return state if player==1, else return -state if player==-1
        return (player * board[0], board[1])

    def stringRepresentation(self, board):
        # nxn numpy array (canonical board)
        return board[0].tostring() + str(board[1]).encode()

    def getSymmetries(self, board, pi):
        # mirror, rotational
        assert(len(pi) == self.getActionSize())  # 1 for pass
        return [(board, pi)]

def display(board):
    board, ply = board
    print("Ply: %d" % ply)

    n = board.shape[0]

    for y in range(n):
        print (y,"|",end="")
    print("")
    print(" -----------------------")
    for y in range(n):
        print(y, "|",end="")    # print the row #
        for x in range(n):
            piece = board[y][x]    # get the piece to print
            if piece == -1: print("b ",end="")
            elif piece == 1: print("W ",end="")
            else:
                if x==n:
                    print("-",end="")
                else:
                    print("- ",end="")
        print("|")

    print("   -----------------------")
