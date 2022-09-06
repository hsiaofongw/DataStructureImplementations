//
// Created by 韦晓枫 on 2022/9/6.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MYTESTCASEPARSER_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MYTESTCASEPARSER_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <deque>
#include <cstdlib>
#include <optional>
#include <unordered_map>

namespace Utils::Parser::MyTestCaseParser {

    /**
     * <CSV> ::= <Row> <MoreRows>
     * <MoreRows> ::= eps
     *   | ';' <CSV>
     * <Row> ::= <Key> <CellList>
     * <CellList> ::= <Cell> <CellList>
     *   | <eps>
     * <Cell> ::= string
     * <Key> ::= string
     */

    enum class NodeType {
        CSV, Row, MoreRows, Key, CellList, Cell, Leaf
    };

    struct Node;
    using NodePtr = std::shared_ptr<Node>;

    struct Token {
        std::string content;
    };

    using TokenPtr = std::shared_ptr<Token>;

    struct Node {
        bool isLeaf;
        NodeType type;
        std::shared_ptr<std::vector<NodePtr>> children;
        std::shared_ptr<Token> tokenPtr;
        size_t length;
    };

    NodePtr makeLeaf(const TokenPtr &tokenPtr) {
        NodePtr nodePtr = std::make_shared<Node>();
        nodePtr->isLeaf = true;
        nodePtr->type = NodeType::Leaf;
        nodePtr->tokenPtr = tokenPtr;
        nodePtr->length = 1;
        return nodePtr;
    }

    NodePtr makeNonLeaf(NodeType type) {
        NodePtr nodePtr = std::make_shared<Node>();
        nodePtr->isLeaf = false;
        nodePtr->type = type;
        nodePtr->children = std::make_shared<std::vector<NodePtr>>();

        return nodePtr;
    }

    bool parseCSV(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);
    bool parseRow(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);
    bool parseMoreRows(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);
    bool parseCellList(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);
    bool parseCell(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);
    bool parseKey(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);

    bool parseCSV(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        NodePtr row = makeNonLeaf(NodeType::Row);
        if (parseRow(s, offset, row)) {
            NodePtr moreRows = makeNonLeaf(NodeType::MoreRows);
            if (parseMoreRows(s, offset + row->length, moreRows)) {
                if (root->children) {
                    root->children->push_back(row);
                    root->children->push_back(moreRows);
                    root->length = row->length + moreRows->length;
                }
                return true;
            }
        }

        return false;
    }

    bool parseRow(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size() && s[offset]) {
            NodePtr keyNodePtr = makeNonLeaf(NodeType::Key);
            if (parseKey(s, offset, keyNodePtr)) {
                NodePtr cellListNodePtr = makeNonLeaf(NodeType::CellList);
                if (parseCellList(s, offset + keyNodePtr->length, cellListNodePtr)) {
                    if (root->children) {
                        root->children->push_back(keyNodePtr);
                        root->children->push_back(cellListNodePtr);
                        root->length = keyNodePtr->length + cellListNodePtr->length;
                    }
                    return true;
                }
            }
        }

        return false;
    }

    bool parseKey(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size() && s[offset]) {
            NodePtr stringNodePtr = makeLeaf(s[offset]);
            if (root->children) {
                root->children->push_back(stringNodePtr);
                root->length = 1;
            }
            return true;
        }
        return false;
    }

    bool parseMoreRows(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size() && s[offset]) {
            if (s[offset]->content == ";") {
                NodePtr csvNodePtr = makeNonLeaf(NodeType::CSV);
                if (parseCSV(s, offset + 1, csvNodePtr)) {
                    if (root->children) {
                        root->children->push_back(makeLeaf(s[offset]));
                        root->children->push_back(csvNodePtr);
                        root->length = 1 + csvNodePtr->length;
                    }

                    return true;
                }
            }
        }

        return true;
    }

    bool parseCellList(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size()) {
            if (auto &tokenPtr = s[offset]) {
                auto cellNodePtr = makeNonLeaf(NodeType::Cell);
                if (parseCell(s, offset, cellNodePtr)) {
                    auto cellListNode = makeNonLeaf(NodeType::CellList);
                    if (parseCellList(s, offset + 1, cellListNode)) {
                        if (root->children) {
                            root->children->push_back(cellNodePtr);
                            root->children->push_back(cellListNode);
                            root->length = cellNodePtr->length + cellListNode->length;
                        }
                        return true;
                    }
                }
            }
        }

        return true;
    }

    bool parseCell(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size() && s[offset] && s[offset]->content != ";") {
            if (root->children) {
                root->children->push_back(makeLeaf(s[offset]));
                root->length = 1;
            }
            return true;
        }

        return false;
    }

    std::vector<TokenPtr> split(const std::string &s) {
        size_t N = s.size(), i = 0;
        std::vector<TokenPtr> tokens;
        std::string token;
        auto reapToken = [&tokens](std::string s) -> void {
            if (!s.empty()) {
                auto tokenPtr = std::make_shared<Token>();
                tokenPtr->content = std::move(s);
                tokens.push_back(tokenPtr);
            }
        };
        while (i < N) {
            if (isspace(s[i])) {
                reapToken(token);
                token.clear();
            } else if (s[i] == ';') {
                reapToken(token);
                token.clear();

                std::string semicolon = ";";
                reapToken(semicolon);
            } else {
                token += s[i];
            }
            i++;
        }
        if (!token.empty()) {
            reapToken(token);
        }

        return tokens;
    }

    std::vector<std::string> traverseCellList(NodePtr &root) {
        std::vector<std::string> result;
        std::deque<NodePtr> candidates;
        candidates.push_back(root);
        while (!candidates.empty()) {
            NodePtr head = candidates.front();
            candidates.pop_front();

            if (head) {
                if (head->type == NodeType::Cell) {
                    if (head->children && !head->children->empty()) {
                        auto numberNode = head->children->at(0);
                        if (numberNode && numberNode->tokenPtr) {
                            result.push_back(numberNode->tokenPtr->content);
                        }
                    }
                } else if (head->type == NodeType::CellList) {
                    if (head->children) {
                        for (auto &childNode: *head->children) {
                            if (childNode) {
                                candidates.push_back(childNode);
                            }
                        }
                    }
                }
            }
        }

        return result;
    }

    std::vector<std::pair<std::string, std::vector<std::string>>> traverseRows(
            NodePtr &root
    ) {
        std::vector<std::pair<std::string, std::vector<std::string>>> result;
        std::deque<NodePtr> candidates;
        candidates.push_back(root);
        while (!candidates.empty()) {
            NodePtr head = candidates.front();
            candidates.pop_front();

            if (head) {
                if (head->type == NodeType::CSV) {
                    if (head->children) {
                        if (head->children->at(0)) {
                            candidates.push_back(head->children->at(0));
                        }
                        if (head->children->at(1)) {
                            candidates.push_back(head->children->at(1));
                        }
                    }
                } else if (head->type == NodeType::Row) {
                    if (head->children && !head->children->empty()) {
                        auto keyNode = head->children->at(0);
                        auto cellListNode = head->children->at(1);
                        if (keyNode && keyNode->children && !keyNode->children->empty()) {
                            auto stringNode = keyNode->children->at(0);
                            if (stringNode->tokenPtr) {
                                if (cellListNode) {
                                    std::pair<std::string, std::vector<std::string>> pair;
                                    pair.first = stringNode->tokenPtr->content;
                                    auto numListVector = traverseCellList(cellListNode);
                                    pair.second = numListVector;
                                    result.emplace_back(pair);
                                }
                            }
                        }
                    }
                } else if (head->type == NodeType::MoreRows) {
                    if (head->children && !head->children->empty() && head->children->at(1)) {
                        candidates.push_back(head->children->at(1));
                    }
                }
            }
        }

        return result;
    }

    using ParsedPair = std::pair<std::string, std::vector<std::string>>;

    std::optional<std::vector<ParsedPair>> parse(const std::string &s) {
        NodePtr root = makeNonLeaf(NodeType::CSV);
        std::vector<TokenPtr> tokens = split(s);

        if (!parseCSV(tokens, 0, root)) {
            std::cerr << "Parse failed." << std::endl;
            return {};
        }

        return traverseRows(root);
    }


    template <typename CellT>
    using ColumnData = std::vector<CellT>;

    template <typename CellT>
    using ColumnList = std::unordered_map<std::string, ColumnData<CellT>>;

    ColumnList<std::vector<std::string>> getColumnList(const std::vector<ParsedPair> &parsedPairs) {
        std::unordered_map<std::string, std::vector<std::vector<std::string>>> transposedTable;
        for (auto &pair : parsedPairs) {
            transposedTable[pair.first].emplace_back(pair.second);
        }

        return transposedTable;
    }

    template <typename CellT>
    using Row = std::unordered_map<std::string, CellT>;

    std::vector<Row<std::vector<std::string>>> getRegularTable(const std::unordered_map<std::string, ColumnData<std::vector<std::string>>> &transposedTable) {
        size_t nRows = 0;
        for (auto &pair : transposedTable) {
            nRows = pair.second.size();
            break;
        }
        for (auto &pair : transposedTable) {
            nRows = std::min(nRows, pair.second.size());
        }

        std::vector<Row<std::vector<std::string>>> table;
        for (size_t rowIdx = 0; rowIdx < nRows; rowIdx++) {
            Row<std::vector<std::string>> row;
            for (auto &pair : transposedTable) {
                row[pair.first] = pair.second[rowIdx];
            }
            table.emplace_back(std::move(row));
        }

        return table;
    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MYTESTCASEPARSER_HPP
