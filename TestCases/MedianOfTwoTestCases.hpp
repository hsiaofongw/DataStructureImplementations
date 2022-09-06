//
// Created by 韦晓枫 on 2022/9/6.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOTESTCASES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOTESTCASES_HPP

#include <vector>
#include "../Utils/Parser/MyTestCaseParser.hpp"
#include "TestCases/MedianOfTwoSortedArrayTestCase.h"
#include <unordered_map>

namespace TestCases::MedianOfTwoSortedArray {
    struct TestCase {
        std::vector<int> lhs;
        std::vector<int> rhs;
        double median;
    };

    std::vector<TestCase> getTestCases() {
        using Utils::Parser::MyTestCaseParser::parse;
        using Utils::Parser::MyTestCaseParser::getColumnList;
        using Utils::Parser::MyTestCaseParser::getRegularTable;

        std::cout << MEDIAN_OF_TWO_SORTED_ARRAY_TESTCASES_CONTENT << std::endl;

        auto parsedResult = Utils::Parser::MyTestCaseParser::parse(MEDIAN_OF_TWO_SORTED_ARRAY_TESTCASES_CONTENT);
        if (parsedResult) {
            auto columnList = getColumnList(parsedResult.value());
            auto table = getRegularTable(columnList);
            std::vector<TestCase> testCases;
            for (auto &row : table) {
                std::vector<std::string> &lhs_s = row["l"];
                std::vector<std::string> &rhs_s = row["r"];
                std::vector<std::string> &m_s = row["m"];

                TestCase testCase;

                for (std::string &s : lhs_s) {
                    testCase.lhs.push_back(std::stoi(s));
                }

                for (std::string &s : rhs_s) {
                    testCase.rhs.push_back(std::stoi(s));
                }

                for (std::string &s : m_s) {
                    testCase.median = std::stod(s);
                    break;
                }
                testCases.emplace_back(testCase);
            }

            return testCases;
        }

        return {};
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOTESTCASES_HPP
