def calc_wrapping(cost, N):
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
        c[j] = 2**32
        for i in range(1, j + 1):
            if c[i - 1] + cost[i][j] < c[j]:
                c[j], p[j] = c[i - 1] + cost[i][j], i
    return c[N], get_num_of_lines(p, N)

# Prepare
input_string = open("input.txt", "r")
answer_file = open("output.txt", "w")
L = int(input_string.readline())
words = input_string.readline().split()
N = len(words)

# Generate cost array
cost = list([list([2**32 for x in range(N + 1)]) for y in range(N + 1)])
extra = list([list([2**32 for x in range(N + 1)]) for y in range(N + 1)])
for i in range(1, N + 1):
    extra[i][i] = L - len(words[i - 1])
    for j in range(i + 1, N):
        extra[i][j] = extra[i][j - 1] - len(words[j - 1]) - 1
for i in range(1, N + 1):
    for j in range(i, N + 1):
        if extra[i][j] < 0:
            cost[i][j] = 2*32
        elif j == N and extra[i][j] >= 0:
            cost[i][j] = 0
        else:
            cost[i][j] = extra[i][j] ** 2

# Do all work
min_squares, num_of_line = calc_wrapping(cost, N)

# Print results
answer_file.write(str(min_squares) + '\n')
answer_file.write(str(num_of_line))
# Go away
answer_file.close()
input_string.close()
