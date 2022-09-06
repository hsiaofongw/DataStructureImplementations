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

            for (auto &testCase : testCases) {
                auto &l = testCase.lhs;
                auto &r = testCase.rhs;
                l.insert(l.end(), r.begin(), r.end());
                std::sort(l.begin(), l.end());
                double m;
                if (l.size() % 2 == 1) {
                    m = l[(l.size()-1)/2];
                } else {
                    size_t n = l.size();
                    size_t i = n/2 - 1;
                    size_t j = n/2;
                    m = (l[i]+l[j])/2.0;
                }

                assert((m == testCase.median));
            }

            return testCases;
        }

        return {};
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOTESTCASES_HPP
