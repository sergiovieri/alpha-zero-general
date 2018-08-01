class Board():

    def __init__(self, n):
        self.n = n
        self.pieces = [None] * n
        for i in range(n):
            self.pieces[i] = [0] * n

        for i in range(2):
            for j in range(n):
                self.pieces[i][j] = 1
                self.pieces[n - i - 1][j] = -1

    # add [][] indexer syntax to the Board
    def __getitem__(self, index): 
        return self.pieces[index]

    def is_legal(self, move, player):
        origin_r, origin_c, target_r, target_c, shoot_r, shoot_c = move
        if self.pieces[origin_r][origin_c] != player: return False
        if self.pieces[target_r][target_c] != 0: return False
        if shoot_r == target_r and shoot_c == target_c: return True
        return self.pieces[shoot_r][shoot_c] == -player

    def execute_move(self, move, player):
        origin_r, origin_c, target_r, target_c, shoot_r, shoot_c = move
        self.pieces[origin_r][origin_c] = 0
        self.pieces[shoot_r][shoot_c] = 0
        self.pieces[target_r][target_c] = player


