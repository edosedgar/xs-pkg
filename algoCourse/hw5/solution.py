# To struggle with optimization problem the algorithm from
# https://www-bcf.usc.edu/~shanghua/teaching/Spring2010/public_html/files/HW3_Solutions_A.pdf
# was implemented here

def calc_wrapping(L, cost, N):
    # To convert array of positions of new lines to
    # number of lines
    def get_num_of_lines(p, N):
        if (p[N] == 1):
            return 1
        else:
            return get_num_of_lines(p, p[N] - 1) + 1

    # Generate array of position for each new line
    # and buffer array for storing square num of spaces left till end on line
    c = list([2**32 for x in range(N + 1)])
    p = list([0 for x in range(N + 1)])
    c[0] = 0
    for j in range(1, N + 1):
        space_left = L - cost[j - 1]
        for i in range(j - 1, max(1, j - L // 2 + 2) - 1, -1):
            if i != j:
                space_left = space_left - cost[i - 1] - 1
            if (space_left >= 0 and j - 1 >= i):
                instant_cost = space_left ** 2
            else:
                instant_cost = 2 ** 32
            if (c[i - 1] + instant_cost < c[j]):
                c[j], p[j] = c[i - 1] + instant_cost, i
    return c[N], get_num_of_lines(p, N)

# Prepare
input_string = open("input.txt", "r")
answer_file = open("output.txt", "w")
L = int(input_string.readline())
words = input_string.readline().split()
words_len = list([len(x) for x in words])
N = len(words)

# Do all work
min_squares, num_of_line = calc_wrapping(L, words_len, N)

# Print results
answer_file.write(str(min_squares) + '\n')
answer_file.write(str(num_of_line))
# Go away
answer_file.close()
input_string.close()
