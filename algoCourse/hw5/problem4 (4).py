min_sum = 100000
min_path = 0
def split_line(words, l, k, cost, path):  
    global min_sum, min_path, sent
    if not words: 
        cost+=(l-k)**2
        path+=1
#         if sentence:
#             print(sentence, cost, k)
        if cost<min_sum:            
            min_sum = cost
            min_path = path
        return min_sum, min_path

    # current word
    whitespace = 0
    if k>0:
        whitespace = 1
    
    if k+len(words[0])+whitespace>l:
        split_line(words, l, 0, cost+(l-k)**2, path + 1) # split
    else:         
        k+=len(words[0])+whitespace
        split_line(words[1:], l, 0, cost+(l-k)**2, path + 1) # split
        split_line(words[1:], l, k, cost, path) # continue line

     
    return min_sum, min_path
  
with open('input.txt') as src:
    L = int(src.readline())
    words = src.readline().split()
    n = len(words)
    opt_sum, opt_path = split_line(words, L ,0, 0, 0)
    with open('output.txt', 'w') as dst:
        dst.write(str(opt_sum)+'\n')
        dst.write(str(opt_path))