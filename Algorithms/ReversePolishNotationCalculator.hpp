//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REVERSEPOLISHNOTATIONCALCULATOR_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REVERSEPOLISHNOTATIONCALCULATOR_HPP

#include <string>
#include <vector>
#include <stack>

namespace Algorithm {
    namespace ReversePolishNotationCalculator {

        /**
         * 逆波兰表达式计算器：
         * 思路：维护一个栈，遇到数字时把数字入栈，
         * 遇到运算符就把需要的操作数悉数逆序出栈，
         * 执行运算操作，再把运算结果入栈。
         */

        using std::vector;
        using std::string;

        class Solution {
        public:
            int evalRPN(vector<string>& tokens) {
                std::stack<int> s;

                auto tokenIt = tokens.begin();
                auto tokenEnd = tokens.end();
                while (tokenIt != tokenEnd) {
                    if (*tokenIt == "+") {
                        int operand2 = s.top();
                        s.pop();
                        int operand1 = s.top();
                        s.pop();
                        int result = operand1 + operand2;
                        s.push(result);
                    } else if (*tokenIt == "-") {
                        int operand2 = s.top();
                        s.pop();
                        int operand1 = s.top();
                        s.pop();
                        int result = operand1 - operand2;
                        s.push(result);
                    } else if (*tokenIt == "*") {
                        int operand2 = s.top();
                        s.pop();
                        int operand1 = s.top();
                        s.pop();
                        int result = operand1 * operand2;
                        s.push(result);
                    } else if (*tokenIt == "/") {
                        int operand2 = s.top();
                        s.pop();
                        int operand1 = s.top();
                        s.pop();
                        int result = operand1 / operand2;
                        s.push(result);
                    } else {
                        s.push(atoi(tokenIt->c_str()));
                    }

                    ++tokenIt;
                }

                return s.top();
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_REVERSEPOLISHNOTATIONCALCULATOR_HPP
