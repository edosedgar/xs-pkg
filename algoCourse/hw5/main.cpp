#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <climits>

/*
int get_lines(int p[], int n)
{
    if (p[n] == 1)
        return 1;
    else
        return get_lines(p, p[n]-1) + 1;
}
*/

int main(int argc, char *argv[]) {
        int line_length = 0;
        int i, j, spaces_left;
        std::ifstream input("input.txt");
        std::ofstream answer("output.txt");
        std::vector<std::string> words;

        /*
         * Read input file
         */
        input >> line_length;
        std::copy(std::istream_iterator<std::string>(input),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(words));
        int N = words.size();

        /*
         * Generate cost table
         */
        int cost[N + 1][N + 1];
        int c[N + 1], p[N + 1] = {0};
        c[0] = 0;

        for (i = 1; i <= N; i++) {
                spaces_left = line_length + 1;
                for (j = i; j <= N; j++) {
                        spaces_left = spaces_left - words[j - 1].size() - 1;
                        std::cout << spaces_left << std::endl;
                        if (spaces_left >= 0)
                                cost[i][j] = spaces_left ^ 2;
                        else
                                cost[i][j] = INT_MAX;
            }
        }

        for (j = 1; j <= N; j++) {
                c[j] = INT_MAX;
                for (i = 1; i <= j; i++) {
                        if ((c[i-1] != INT_MAX && cost[i][j] != INT_MAX) &&
                            (c[i-1] + cost[i][j] < c[j])) {
                                c[j] = c[i-1] + cost[i][j];
                                p[j] = i;
                        }
                }
        }

        std::cout << c[N];
        answer << c[N] << "\n";
        //fout << get_num_lines(p, n);
        return 0;
}
