import copy

'''
Algorithm explanation:
1) Read new box dims
2) Iterate through pairs of dims and add pairs to hast table
3) After adding each pair it checks whether it collides with
   any pair in a given items located in particular index of hast table

'''
class hash_table:
    def __init__(self, size):
        self.size = size
        self.hash_table = [[] for x in range(0, self.size)]
    def add(self, pos_one, pos_two, box, num):
        max_sum = 0
        pos_three = 3 - pos_two - pos_one
        pair_num = -1
        enum = [[0, 1], [1, 0], [1, 2], [2, 1], [2, 0], [0, 2]]
        index = (box[pos_two] * box[pos_one]) % self.size
        for elem in self.hash_table[index]:
            for pos in enum:
                or_pos = 3 - pos[0] - pos[1]
                if (box[pos_one] == elem[0][pos[0]] and
                    box[pos_two] == elem[0][pos[1]]):
                    if (box[pos_three] + elem[0][or_pos] > max_sum):
                        max_sum = box[pos_three] + elem[0][or_pos]
                        pair_num = elem[1]
        copy_box = copy.deepcopy(box)
        self.hash_table[index].append([copy_box, num])
        if (min(box) < min([box[pos_one], box[pos_two], max_sum])):
            return 2, pair_num, min([box[pos_one], box[pos_two], max_sum])
        else:
            return 1, -1, min(box)
    def __str__(self):
        ht_print = ''
        for idx,val in enumerate(self.hash_table):
            if len(val):
                ht_print += str(idx) + ":" + str(val) + '\n'
        return ht_print

max_ball_dim = 0
pair = [-1, -1]
box_num = 1
ht1 = hash_table(100003)
boxes_desc = open("input.txt", "r")
answer_file = open("output.txt", "w")
box_num_max = int(boxes_desc.readline())

line_num = 0
for line in boxes_desc:
    line_num += 1
    # Processing starts here
    elem = [int(a) for a in line.split()]

    # No reason to proccess box if its maximum dim size is
    # smaller than current max diameter
    if (max(elem) < max_ball_dim):
        continue

    # Consider collision of first two dimensions with any others in hash table
    # and save the best results
    res1 = ht1.add(0, 1, elem, line_num)
    if res1[2] > max_ball_dim:
        max_ball_dim = res1[2]
        box_num = res1[0]
        pair[0] = line_num
        pair[1] = res1[1]

    #Consider collision of second two dimensions with any others in hash table
    #and save the best results
    res2 = ht1.add(1, 2, elem, line_num)
    if res2[2] > max_ball_dim:
        max_ball_dim = res2[2]
        box_num = res2[0]
        pair[0] = line_num
        pair[1] = res2[1]

    # Consider collision of third two dimensions with any others in hash table
    # and save the best results
    res3 = ht1.add(2, 0, elem, line_num)
    if res3[2] > max_ball_dim:
        max_ball_dim = res3[2]
        box_num = res3[0]
        pair[0] = line_num
        pair[1] = res3[1]

# Store the number of box
answer_file.write(str(box_num)+'\n')
# Delete unused box index
if (box_num == 1):
    pair.remove(-1)
#It's just sort of two numbers
pair.sort()
# Print indexes of selected boxes
answer_file.write(' '.join(str(a) for a in pair[0:box_num]) + '\n')
# Print max ball diameter
answer_file.write(str(max_ball_dim))
boxes_desc.close()
answer_file.close()
