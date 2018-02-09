#!/usr/bin/env python3
#
# pathfinding algorithm implementations of:
#  * breadth first
#  * lowest cost
#  * greedy best first
#  * A*
# heuristics:
#  * manhattan / euclidean
#
# output:
#  <type>.png
#
# (c) matt waltz - spring 2018

from PIL import Image, ImageDraw

class find(object):
    def __init__(self, filename=None, style=None, heuristic=None):
        with open(filename) as f:
            data = f.readlines()

        self.costs = {'R' : 1, 'f' : 2, 'F' : 4, 'h' : 5, 'r' : 7, 'M' : 10}
        self.width = int(data[0].split()[0])
        self.height = int(data[0].split()[1])
        self.start = tuple(map(int, data[1].split()))
        self.goal = tuple(map(int, data[2].split()))
        self.map = data[3:]

        self.style = getattr(self, style)
        self.heuristic = getattr(self, heuristic)
        self.closedset = set()
        self.path = dict()
        self.f = dict()
        self.g = dict()

        self.out = 0
        self.end = self.style()
        pass

    def test(self, state): return self.goal == state
    def cost(self, state): return self.costs[self.map[state[1]][state[0]]]
    def valid(self, state):
        x, y = state
        return (x >= 0 and y >= 0 and x < self.width and y < self.height and
                self.map[y][x] != 'W')

    def expand(self, state):
        x, y = state
        result = list()
        for neighbor in ((x, y-1), (x, y+1), (x+1, y), (x-1, y)):
            if self.valid(neighbor): result.append(neighbor)
        return result

    def add_fifo(self, state):
        self.openset.append(state)
    def add_set(self, state):
        self.openset.add(state)

    def take_first(self):
        self.out += 1
        return self.openset[self.out - 1]
    def take_sorted(self):
        index = 0
        fsort = self.sort()
        for index in range(len(fsort) - 1):
            if fsort[index] not in self.closedset:
                break
        send = fsort[index]
        self.openset.remove(send)
        return send

    def sort_heuristic_cost(self):
        return sorted(self.f, key=lambda state: self.g[state] + self.heuristic(state))
    def sort_heuristic(self):
        return sorted(self.f, key=lambda state: self.heuristic(state))
    def sort_cost(self):
        return sorted(self.f, key=lambda state: self.g[state])

    def breadth_first(self):
        self.openset = list()
        self.take = self.take_first
        self.add = self.add_fifo
        self.score = self.none
        return self.process()
    def lowest_cost(self):
        self.openset = set()
        self.take = self.take_sorted
        self.add = self.add_set
        self.sort = self.sort_cost
        self.score = self.cost
        return self.process()
    def greedy(self):
        self.openset = set()
        self.take = self.take_sorted
        self.add = self.add_set
        self.sort = self.sort_heuristic
        self.score = self.none
        return self.process()
    def a_star(self):
        self.openset = set()
        self.take = self.take_sorted
        self.add = self.add_set
        self.sort = self.sort_heuristic_cost
        self.score = self.cost
        return self.process()

    def process(self):
        self.g[self.start] = 0
        self.f[self.start] = self.heuristic(self.start)
        self.add(self.start)
        self.path[self.start] = None

        while len(self.openset):
            parent = self.take()
            if self.test(parent):
                return parent
            self.closedset.add(parent)
            for child in self.expand(parent):
                if child not in self.closedset:
                    child_cost = self.g[parent] + self.score(child)
                    if child not in self.openset or child_cost < self.g[child]:
                        self.path[child] = parent
                        self.g[child] = child_cost
                        self.f[child] = child_cost + self.heuristic(child)
                        if child not in self.openset:
                            self.add(child)

    def none(self, state):
        return 0
    def euclidean(self, state):
        x0, y0 = self.goal
        x1, y1 = state
        return ((x0 - x1)**2 + (y0 - y1)**2)**(1.0/2)
    def manhattan(self, state):
        x0, y0 = self.goal
        x1, y1 = state
        return abs(x0 - x1) + abs(y0 - y1)

    def get(self):
        total = 0
        moves = [self.end]
        state = self.path[self.end]
        while not state == None:
            moves.append(state)
            total += self.cost(state)
            state = self.path[state]
        moves.reverse()
        return moves, total

def main():
    types = [   'breadth_first', 'none',
                'lowest_cost', 'none',
                'greedy', 'none',
                'greedy', 'manhattan',
                'greedy', 'euclidean',
                'a_star', 'manhattan',
                'a_star', 'euclidean' ]

    terrain = { 'R' : [128, 128, 128], 'f' : [244, 164, 96],
                'F' : [85, 107, 47], 'h' : [205, 133, 63],
                'r' : [0, 0, 128], 'M' : [139, 69, 19],
                'W' : [25, 25, 110] }

    for j in range(0, len(types), 2):
        type_str = types[j]
        if types[j] == 'a_star' or types[j] == 'greedy':
            type_str += '_' + types[j+1]

        try:
            search = find('map.txt', types[j], types[j+1])
            path, cost = search.get()
        except:
            return print('error: no path found')

        offset = 21
        width = search.width * 10 + 1
        height = search.height * 10 + 1
        img = Image.new('RGB', (width, height + offset), color = 'black')

        draw = ImageDraw.Draw(img)
        for x in range(search.width + 1):
            nx = x * 10
            for y in range(search.height + 1):
                ny = y * 10 + offset
                if y < search.height and x < search.width:
                    color = terrain[search.map[y][x]]
                    outline = ((nx + 3, ny + 3), (nx + 7, ny + 7))
                    solid = ((nx + 4, ny + 4), (nx + 6, ny + 6))
                    draw.rectangle(((nx + 1, ny + 1), (nx + 9, ny + 9)), tuple(color))
                    if (x, y) in path:
                        draw.rectangle(solid, 'red')
                    if (x, y) in search.closedset:
                        draw.rectangle(outline, None, 'red')
                    elif (x, y) in search.openset:
                        draw.rectangle(outline, None, 'purple')
                    if (x, y) == search.start:
                        draw.rectangle(outline, None, 'yellow')
                    elif (x, y) == search.goal:
                        draw.rectangle(outline, None, 'green')
            draw.text((2, 0), type_str, (255,255,255))
            draw.text((2, 10), 'cost: ' + str(cost), (255,255,255))
            draw.text((width-67, 10), 'length: ' + str(len(path)), (255,255,255))

        print('wrote: ' + type_str + '.png')
        print('cost: ' + str(cost))
        print('length: ' + str(len(path)))
        img.save(type_str + '.png')

main()
