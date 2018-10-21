import copy

"""
    The description of the algorithm used in the program

"""

# Simple queue implementation
class queue:
    def __init__(self):
        self.queue = []
    def enqueue(self, elem):
        self.queue.append(elem)
    def dequeue(self):
        if not len(self.queue):
            raise ValueError('Queue is empty')
        elem = copy.deepcopy(self.queue[0])
        self.queue.remove(elem)
        return elem
    def is_empty(self):
        if len(self.queue):
            return 0
        else:
            return 1
    def __str__(self):
        return '[< ' + ' '.join([str(x) for x in self.queue]) + ' <]'

# Graph implementation using adjacency list
# Refer to 'problem.txt' to figure out the way the path len is calculated
class graph:
    def __init__(self):
        self.verts = 0
        self.edges = 0
        self.nodes = []
        self.queue = queue()
    def add_param(self, vert, edge):
        self.verts = vert
        self.edges = edge
        self.nodes = [{'n': x, 'col': "w", 'd': 0, 'prev': -1, 'next': []} for x in range(vert)]
    def add_edge(self, edge):
        self.nodes[int(edge[0])]['next'].append(int(edge[1]))
    def calc_weight_and_bfs(self, source, dest):
        path_len = 0
        self.nodes[source]['col'] = "g"
        self.queue.enqueue(self.nodes[source]['n'])
        while not self.queue.is_empty():
            u_n = self.queue.dequeue()
            for node in self.nodes[u_n]['next']:
                if self.nodes[node]['col'] == "w":
                    self.nodes[node]['col'] = "g"
                    self.nodes[node]['d'] = self.nodes[u_n]['d'] + 1
                    self.nodes[node]['prev'] = self.nodes[u_n]['n']
                    self.queue.enqueue(node)
            self.nodes[u_n]['col'] = "b"
        for i in range(self.nodes[dest]['d']):
            if (i % 2 == 0):
                path_len += 1
            else:
                path_len += 3
        return path_len
    def __str__(self):
        return str(self.nodes)

# Create graph and read parameters
grph = graph()
graph_desc = open("input.txt", "r")
answer_file = open("output.txt", "w")
vertices_num = int(graph_desc.readline())
edges_num = int(graph_desc.readline())
grph.add_param(vertices_num, edges_num)
# Read source and dest nodes
source_node = int(graph_desc.readline())
dest_node = int(graph_desc.readline())
# Read and add edges connections
for line in graph_desc:
    grph.add_edge(line.split())
# Calculate all paths according to the rule given in the task and find shortest
# path from a source to a dest
path_len = grph.calc_weight_and_bfs(source_node, dest_node)
answer_file.write(str(path_len))
# Close all service files and terminate
graph_desc.close()
answer_file.close()
