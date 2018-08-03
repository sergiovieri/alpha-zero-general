from Coach import Coach
from troops.TroopsGame import TroopsGame as Game
from troops.pytorch.NNet import NNetWrapper as nn
from utils import *
import sys

args = dotdict({
    'numIters': 1000,
    'numEps': 100,
    'tempThreshold': 1000,
    'updateThreshold': 0.6,
    'maxlenOfQueue': 200000,
    'numMCTSSims': 100,
    'arenaCompare': 10,
    'cpuct': 2,
    'drawThreshold': 200,

    'checkpoint': './temp/',
    'load_model': True,
    'load_folder_file': ('./temp/','best.pth.tar'),
    'numItersForTrainExamplesHistory': 20,

})

if __name__=="__main__":
    sys.setrecursionlimit(1000000)
    g = Game(8, args)
    nnet = nn(g)

    if args.load_model:
        nnet.load_checkpoint(args.load_folder_file[0], args.load_folder_file[1])

    c = Coach(g, nnet, args)
    if args.load_model:
        print("Load trainExamples from file")
        c.loadTrainExamples()
    c.learn(iterStart=26)
