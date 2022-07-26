//
// Created by 韦晓枫 on 2022/7/26.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP

#include <string>

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
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_INTERLEAVESTRING_HPP
