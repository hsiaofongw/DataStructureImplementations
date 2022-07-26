//
// Created by 韦晓枫 on 2022/7/26.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP

#include <string>
#include <vector>

namespace Algorithm::InterleaveString {
    /**
     * 交错子序列判定问题
     *
     * 定义：
     *
     * 给定字符串 std::string s1, s2, s3，
     * 若存在 s1 的子序列和 s2 的子序列保持各自原先的相对位次合并之后得到 s3,
     * 则称 s3 是由 s1 与 s2 的子序列交错构成的。
     *
     * 举例：
     *
     * 例 1：
     *
     * s1 = "aabcc", s2 = "dbbca", s3 = "aadbbcbcac",
     * 则 ss1 = [ "aa", "bc", "c" ] 是 s1 的一个子序列，ss2 = [ "dbbc", "a" ] 是 s2 的一个子序列，
     * 让 ss1 和 ss2 保持顺序合并（合并不唯一），其中一种合并得到：
     * ss1_ss2 = [ "aa", "dbbc", "bc", "a", "c" ]
     *              |     |       |     |    |
     *             s1    s2      s1    s2   s1
     * 如您所见，在 ss1 与 ss2 的一个合并 ss1_ss2 中，来自 s1 和来自 s2 的子序列都保持着各自原先在原列表中的次序，
     * 并且前后拼接起来正好是 s3, 因此 s3 是 s1, s2 的一个交错子序列。
     *
     */
    class Solution1 {
        /**
         * Solution1 是一个 TLE 的递归式解法，用于启发人进行思考，加深人对问题本身的理解。
         */
    public:
        bool isInterleave(std::string s1, std::string s2, std::string s3) {
            if (s1.empty()) {
                return s2 == s3;
            } else if (s2.empty()) {
                return s1 == s3;
            } else {
                // both s1, s2 non-empty
                if (s3.empty())
                    return false;

                // 归结成为若干个（有限个）规模更小的子问题
                // 原问题可以归结为规模严格小的子问题是递归函数可收敛性的一个有效保证（可以理解为「充分性」）。
                std::string restS1 = s1.substr(1, s1.size()-1);
                std::string restS2 = s2.substr(1, s2.size()-1);
                std::string restS3 = s3.substr(1, s3.size()-1);

                if (s1[0] == s3[0] && isInterleave(restS1, s2, restS3))
                    return true;

                if (s2[0] == s3[0] && isInterleave(s1, restS2, restS3))
                    return true;

                return false;
            }
        }

        /**
         * 定义函数 f:
         * 对于 i=0,1,...,s1.size(), j=0,1,...,s2.size(), k=0,1,...,s3.size() 定义
         * f(i,j,k) = s3 的 k 长后缀是否是 s1 的 i 长后缀与 s2 的 j 长后缀的 interleaveSubString.
         *
         * 也就是相当于：
         * f(i,j,k) = isInterleave(
         *   s1.subStr(s1.size()-i,i),
         *   s2.substr(s2.size()-j,j),
         *   s3.substr(s3.size()-k,k)
         * ).
         *
         * 我们用一个三阶数组存储 f(i,j,k) 的值，
         * 约定：后面的 f[i,j,k], f[i][j][k], 以及 f(i,j,k) 说的都是同一回事儿。
         *
         * 然后我们用 DP 的方法去把数组 int f[s1.size()][s2.size()][s3.size()] 的每个 cell 的值都算出来。
         */
        class Solution2 {
        public:
            bool isInterleave(const std::string &s1, const std::string &s2, const std::string &s3) {
                size_t n1 = s1.size(), n2 = s2.size(), n3 = s3.size();
                std::vector<std::vector<std::vector<uint8_t>>> f;
                f.resize(n1+1);
                for (auto &layer : f) {
                    layer.resize(n2+1);
                    for (auto &row : layer) {
                        row.resize(n3+1, 0);
                    }
                }

                // 让我们来先计算 f[0][j][k], j=0,1,...,n2, k=0,1,...,n3 的每一个值
                // 相当于模拟 s1 为空串的情况。
                f[0][0][0] = 1;
                for (size_t j = 0; j <= n2; ++j)
                    for (size_t k = 0; k <= n3; ++k)
                        if (j >= 1 && k >= 1 && j == k && s2[n2-j] == s3[n3-k] && f[0][j-1][k-1] == 1)
                            f[0][j][k] = 1;

                // 类似地求 f[i][0][k], i=0,1,...,n1, k=0,1,...,n3 的每一个值，
                // 也就是模拟 s2 为空串的情况。
                for (size_t i = 0; i <= n1; ++i)
                    for (size_t k = 0; k <= n3; ++k)
                        if (i >= 1 && k >= 1 && i == k && s1[n1-i] == s3[n3-k] && f[i-1][0][k-1] == 1)
                            f[i][0][k] = 1;

                // s1, s2 皆非空串.
                for (size_t k = 1; k <= n3; ++k)
                    for (size_t i = 1; i <= n1; ++i)
                        for (size_t j = 1; j <= n2; ++j)
                            if (
                                (s1[n1-i] == s3[n3-k] && f[i-1][j][k-1] == 1) ||
                                (s2[n2-j] == s3[n3-k] && f[i][j-1][k-1] == 1)
                            )
                                f[i][j][k] = 1;

                return f[n1][n2][n3] == 1;
            }
        };
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP
