from operator import itemgetter

class Polyhedron:
    def __init__(self, rect):
        self.xmin = rect[0]
        self.ymax = rect[1]
        self.xmax = rect[2]
        self.ymin = rect[3]
    def min(self, axis):
        if (axis == 0):
            return self.xmin
        else:
            return self.ymin
    def max(self, axis):
        if (axis == 0):
            return self.xmax
        else:
            return self.ymax
    def is_in(self, point):
        if (point[0] <= self.xmax and point[0] >= self.xmin and \
            point[1] <= self.ymax and point[1] >= self.ymin):
            return 1
        else:
            return 0

class Node:
    def __init__(self, info, left, right):
        self.pos = info[:2]
        self.weight = info[2]
        self.left = left
        self.right = right
        self.touched = 0
    def __repr__(self):
        return self.__str__(0)
    def __str__(self, n):
        out = str(self.pos) + ", " + str(self.weight)
        if (self.left is not None):
            out += '\n' + n * '  ' + "-> L: " + self.left.__str__(n + 1)
        if (self.right is not None):
            out += '\n' + n * '  ' + "-> R: " + self.right.__str__(n + 1)
        return out
    def touch(self):
        self.touched = 1

# Build tree from points given in miners
def kdtree_build(miners, depth=0):
    if not len(miners):
        return None
    axis = depth % 2
    miners.sort(key=itemgetter(axis))
    median = len(miners) // 2
    return Node(
        info = miners[median],
        left = kdtree_build(miners[:median], depth + 1),
        right = kdtree_build(miners[median + 1:], depth + 1))

# Process just one rect
def miners_power(tree, rect, level):
    if (tree is None):
        return 0
    index = level % 2
    power = 0

    if (rect.min(index) <= tree.pos[index]):
        power += miners_power(tree.left, rect, level + 1)

        if (rect.is_in(tree.pos) and tree.touched == 0):
            power += tree.weight
            tree.touch()

    if (tree.pos[index] < rect.max(index)):
        power += miners_power(tree.right, rect, level + 1)

        if (rect.is_in(tree.pos) and tree.touched == 0):
            power += tree.weight
            tree.touch()

    return power

# Process each rect one by one
def all_miners_power(tree, query):
    power = 0
    for rect in query:
        power += miners_power(tree, Polyhedron(rect), 0)
    return power

# Prepare
points_queries = open("input.txt", "r")
answer_file = open("output.txt", "w")
points_num_max = int(points_queries.readline())
points = []
# Load points
for i in range(points_num_max):
    points.append(tuple([int(a) for a in points_queries.readline().split()]))
# Build kd-tree
tree = kdtree_build(points)
# Load queries and find points
queries_num_max = int(points_queries.readline())
for i in range(queries_num_max):
    query = []
    rect_num_max = int(points_queries.readline())
    for j in range(rect_num_max):
        query.append(tuple([int(a) for a in points_queries.readline().split()]))
    tree = kdtree_build(points)
    power = all_miners_power(tree, query)
    if (i != queries_num_max - 1):
        answer_file.write(str(power) + '\n')
    else:
        answer_file.write(str(power))
# Go away
answer_file.close()
points_queries.close()
