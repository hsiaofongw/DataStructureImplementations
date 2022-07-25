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
                    },
                    TestCase{
                            .matrix = {
                                    {'a', 'b', 'c', 'e'},
                                    {'s', 'f', 'c', 's'},
                                    {'a', 'd', 'e', 'e'}
                            },
                            .word = "abcb",
                            .expectedOutput = false
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
                    std::cout << "Start\n";
                    size_t dfaState = 0;
                    std::unordered_map<size_t, std::unordered_set<size_t>> traversed;
                    std::deque<Point> candidates;
                    candidates.emplace_front(i, j);
                    while (!candidates.empty()) {
                        Point p = candidates.front();
                        candidates.pop_front();
                        traversed[p.rowIdx].insert(p.colIdx);

                        dfaState = dfa[dfaState][matrix[p.rowIdx][p.colIdx]];

                        // std::cout << "(" << p.rowIdx << ", " << p.colIdx << ") " << dfaState << "\n";

                        if (dfaState == word.size()) {
                            return true;
                        }

                        if (p.colIdx < M-1)
                            if (!(traversed[p.rowIdx].count(p.colIdx+1)))
                                candidates.emplace_front(p.rowIdx, p.colIdx+1);
                        if (p.rowIdx < N-1)
                            if (!(traversed[p.rowIdx+1].count(p.colIdx)))
                                candidates.emplace_front(p.rowIdx+1, p.colIdx);
                        if (p.rowIdx > 0)
                            if (!(traversed[p.rowIdx-1].count(p.colIdx)))
                                candidates.emplace_front(p.rowIdx-1, p.colIdx);
                        if (p.colIdx > 0)
                            if (!(traversed[p.rowIdx].count(p.colIdx-1)))
                                candidates.emplace_front(p.rowIdx, p.colIdx-1);
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
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_WORDSEARCH_HPP
