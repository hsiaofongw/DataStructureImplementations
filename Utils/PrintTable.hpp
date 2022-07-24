//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PRINTTABLE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PRINTTABLE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace Utils {
    void PrintTable(
            std::vector<std::string> &indexCol,
            std::vector<std::string> &headers,
            std::vector<std::vector<std::string>> &cells,
            int indexColWidth,
            std::vector<int> &normalColWidths
    ) {
        int leftPadding = 2;

        std::cout << std::setw(indexColWidth) << "";
        for (size_t j = 0; j < headers.size(); ++j) {
            std::cout << std::setw(normalColWidths[j]+leftPadding) << headers[j];
        }
        std::cout << "\n";

        for (size_t i = 0; i < cells.size(); ++i) {
            std::cout << std::setw(indexColWidth) << indexCol[i];
            for (size_t j = 0; j < cells[i].size(); ++j) {
                std::cout << std::setw(normalColWidths[j]+leftPadding) << cells[i][j];
            }
            std::cout << "\n";
        }
    }

    void PrintTable(
        std::vector<std::string> &indexCol,
        std::vector<std::string> &headers,
        std::vector<std::vector<std::string>> &cells
    ) {
        int indexColWidth = 1;
        for (const auto &content : indexCol) {
            indexColWidth = std::max(indexColWidth, static_cast<int>(content.size()));
        }

        int minNormalColWidth = 1;
        std::vector<int> normalColWidths (headers.size(), minNormalColWidth);
        for (size_t j = 0; j < normalColWidths.size(); ++j) {
            normalColWidths[j] = std::max(normalColWidths[j], static_cast<int>(headers[j].size()));
            for (size_t i = 0; i < cells.size(); ++i) {
                normalColWidths[j] = std::max(normalColWidths[j], static_cast<int>(cells[i][j].size()));
            }
        }

        PrintTable(indexCol, headers, cells, indexColWidth, normalColWidths);
    }

    void PrintTable(
        std::vector<int64_t> &indexCol,
        std::vector<int64_t> &headers,
        std::vector<std::vector<int64_t>> &cells
    ) {
        std::vector<std::string> indexCol_s;
        std::vector<std::string> headers_s;
        std::vector<std::vector<std::string>> cells_s;

        for (const auto &x : indexCol) {
            indexCol_s.push_back(std::to_string(x));
        }

        for (const auto &x : headers) {
            headers_s.push_back(std::to_string(x));
        }

        for (const auto &row : cells) {
            std::vector<std::string> row_s;
            for (const auto &x : row) {
                row_s.push_back(std::to_string(x));
            }
            cells_s.emplace_back(row_s);
        }

        PrintTable(indexCol_s, headers_s, cells_s);
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_PRINTTABLE_HPP
