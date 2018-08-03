import Arena
from MCTS import MCTS
from troops.TroopsGame import TroopsGame, display
from troops.TroopsPlayers import *
from troops.pytorch.NNet import NNetWrapper as NNet

import numpy as np
from utils import *

"""
use this script to play any two agents against each other, or play manually with
any agent.
"""


def argmax(self, pi):
  mx = np.amax(pi)
  return np.random.choice(np.flatnonzero(pi == mx))

g = TroopsGame(8, dotdict({
    	'drawThreshold': 100
		}))

# all players
# rp = RandomPlayer(g).play
# gp = GreedyOthelloPlayer(g).play
# hp = HumanOthelloPlayer(g).play

# nnet players
n1 = NNet(g)
n1.load_checkpoint('./temp/','best.pth.tar')
args1 = dotdict({'numMCTSSims': 50, 'cpuct':2.0})
mcts1 = MCTS(g, n1, args1)
n1p = lambda x: argmax(mcts1.getActionProb(x, temp=1))


n2 = NNet(g)
n2.load_checkpoint('./temp/','best.pth.tar')
args2 = dotdict({'numMCTSSims': 50, 'cpuct':2.0})
mcts2 = MCTS(g, n2, args2)
n2p = lambda x: argmax(mcts2.getActionProb(x, temp=1))

arena = Arena.Arena(n1p, n2p, g, display=display)
print(arena.playGames(2, verbose=True))
