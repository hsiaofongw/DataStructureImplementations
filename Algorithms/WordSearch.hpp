//
// Created by 韦晓枫 on 2022/7/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_WORDSEARCH_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_WORDSEARCH_HPP

#include <vector>
#include <string>
#include <deque>
#include <unordered_set>
#include <unordered_map>

namespace Algorithm::WordSearch {
    /**
     * 文字搜索问题：
     *
     * 给定一个字母矩阵 std::vector<std::vector<char>> matrix, 一个文字 std::string word,
     * 试在 matrix 中开辟一条道路，该道路所到之处经过的 char 形成 word.
     *
     * 例 1：
     *
     * matrix = {
     *   { 'a', 'b', 'c', 'e' },
     *   { 's', 'f', 'c', 's' },
     *   { 'a', 'd', 'e', 'e' }
     * };
     *
     * word = "abcced";
     *
     * 则这样的路径存在，路径为 path = { {0,0}, {0,1}, {0,2}, {1,2}, {2,2}, {2,1} }.
     *
     * 例 2:
     *
     * matrix = {
     *   { 'a', 'b', 'c', 'e' },
     *   { 's', 'f', 'c', 's' },
     *   { 'a', 'd', 'e', 'e' }
     * };
     *
     * word = "see";
     *
     * 则这样的路径存在，路径为 path = { {1,3}, {2,3}, {2,2} }.
     *
     * 例 3:
     *
     * matrix = {
     *   { 'a', 'b', 'c', 'e' },
     *   { 's', 'f', 'c', 's' },
     *   { 'a', 'd', 'e', 'e' }
     * };
     *
     * word = "abcb"
     *
     * 则易证，这样的路径不存在。
     *
     */
    class Solution {
    public:
        struct TestCase {
            std::vector<std::vector<char>> matrix;
            std::string word;
            bool expectedOutput;
        };

        std::vector<TestCase> getTestCases() {
            return {
                    TestCase {
                        .matrix = {{'A','A','A','A','A','A'},{'A','A','A','A','A','A'},{'A','A','A','A','A','A'},{'A','A','A','A','A','A'},{'A','A','A','A','A','A'},{'A','A','A','A','A','A'}},
                        .word = "AAAAAAAAAAAABAA",
                        .expectedOutput = false
                    },
                    TestCase {
                            .matrix = {{'a','a','b','a','a','b'},{'a','a','b','b','b','a'},{'a','a','a','a','b','a'},{'b','a','b','b','a','b'},{'a','b','b','a','b','a'},{'b','a','a','a','a','b'}},
                            .word = "bbbaabbbbbab",
                            .expectedOutput = false
                    },
                    TestCase {
                            .matrix = {{'a','b'},{'c','d'}},
                            .word = "cdba",
                            .expectedOutput = true
                    },
                    TestCase{
                        .matrix = {
                                {'a', 'b', 'c', 'e'},
                                {'s', 'f', 'c', 's'},
                                {'a', 'd', 'e', 'e'}
                        },
                        .word = "abcb",
                        .expectedOutput = false
                    },

                    TestCase{
                            .matrix = {
                                    {'a', 'b', 'c', 'e'},
                                    {'s', 'f', 'c', 's'},
                                    {'a', 'd', 'e', 'e'}
                            },
                            .word = "abcced",
                            .expectedOutput = true
                    },
                    TestCase{
                            .matrix = {
                                    {'a', 'b', 'c', 'e'},
                                    {'s', 'f', 'c', 's'},
                                    {'a', 'd', 'e', 'e'}
                            },
                            .word = "see",
                            .expectedOutput = true
                    }
            };
        }

        bool exist(
            const std::vector<std::vector<char>> &matrix,
            const std::string &word
        ) {
            std::vector<std::unordered_map<char, size_t>> dfa (word.size());
            std::unordered_set<char> uniqueChars (word.begin(), word.end());
            assert((!word.empty()));
            dfa[0][word[0]] = 1;
            for (size_t prev = 0, curr = 1; curr < word.size(); ++curr) {
                for (const char &c : uniqueChars) {
                    dfa[curr][c] = dfa[prev][c];
                }
                dfa[curr][word[curr]] = curr+1;
                prev = dfa[prev][word[curr]];
            }


            size_t N = matrix.size();
            assert((N >= 1));
            size_t M = matrix[0].size();
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < M; ++j) {
                    // std::cout << "i: " << i << "j: " << j << "\n";
                    std::unordered_set<size_t> traversed;
                    std::deque<Point> candidates;
                    // std::cout << "Start\n";
                    if (dfs(Point {i,j}, traversed, 0, M, N, dfa, matrix, word)) {
                        return true;
                    }
                }
            }

            return false;
        }
    private:
        struct Point {
            Point(size_t _rowIdx, size_t _colIdx) : rowIdx(_rowIdx), colIdx(_colIdx) { }
            size_t rowIdx;
            size_t colIdx;
        };

        bool dfs(
            Point p,
            std::unordered_set<size_t> &traversed,
            size_t dfaState,
            const size_t &M,
            const size_t &N,
            std::vector<std::unordered_map<char, size_t>> &dfa,
            const std::vector<std::vector<char>> &matrix,
            const std::string &word
        ) {
            char c = matrix[p.rowIdx][p.colIdx];
            if (word[dfaState] != c) {
                return false;
            }

            traversed.insert(p.rowIdx*M+p.colIdx);
            size_t currentState = dfa[dfaState][c];

            // std::cout << "(" << p.rowIdx << ", " << p.colIdx << ") " << matrix[p.rowIdx][p.colIdx] << " " << dfaState << " -> " << currentState << "\n";

            if (currentState == word.size()) {
                return true;
            }

            if (p.colIdx < M-1 && traversed.count(p.rowIdx*M+p.colIdx+1) == 0) {
                if (dfs(Point { p.rowIdx, p.colIdx+1}, traversed, currentState, M, N, dfa, matrix, word)) {
                    return true;
                }
            }
            if (p.rowIdx < N-1 && traversed.count((p.rowIdx+1)*M+p.colIdx) == 0) {
                if (dfs(Point { p.rowIdx+1, p.colIdx}, traversed, currentState, M, N, dfa, matrix, word)) {
                    return true;
                }
            }
            if (p.colIdx > 0 && traversed.count(p.rowIdx*M+p.colIdx-1) == 0) {
                if (dfs(Point { p.rowIdx, p.colIdx - 1}, traversed, currentState, M, N, dfa, matrix, word)) {
                    return true;
                }
            }
            if (p.rowIdx > 0 && traversed.count((p.rowIdx-1)*M+p.colIdx) == 0) {
                if (dfs(Point { p.rowIdx-1, p.colIdx}, traversed, currentState, M, N, dfa, matrix, word)) {
                    return true;
                }
            }

            traversed.erase(p.rowIdx*M+p.colIdx);
            return false;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_WORDSEARCH_HPP
