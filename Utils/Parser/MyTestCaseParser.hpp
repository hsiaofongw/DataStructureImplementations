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


namespace Utils::Parser::MyTestCaseParser {

    /**
     * <CSV> ::= <Row> <MoreRows>
     * <MoreRows> ::= eps
     *   | ';' <CSV>
     * <Row> ::= <Key> <NumberList>
     * <NumberList> ::= <Number> <NumberList>
     *   | <eps>
     * <Number> ::= digit
     *   | '-' digit
     * <Key> ::= string
     */

    enum class NodeType {
        CSV, Row, MoreRows, NumberList, Number, Key, Leaf
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

    bool parseNumberList(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);

    bool parseNumber(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root);

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
                NodePtr numberListNodePtr = makeNonLeaf(NodeType::NumberList);
                if (parseNumberList(s, offset + keyNodePtr->length, numberListNodePtr)) {
                    if (root->children) {
                        root->children->push_back(keyNodePtr);
                        root->children->push_back(numberListNodePtr);
                        root->length = keyNodePtr->length + numberListNodePtr->length;
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

    bool parseNumberList(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size()) {
            if (auto &tokenPtr = s[offset]) {
                auto numberNodePtr = makeNonLeaf(NodeType::Number);
                if (parseNumber(s, offset, numberNodePtr)) {
                    auto numberListNode = makeNonLeaf(NodeType::NumberList);
                    if (parseNumberList(s, offset + 1, numberListNode)) {
                        if (root->children) {
                            root->children->push_back(numberNodePtr);
                            root->children->push_back(numberListNode);
                            root->length = numberNodePtr->length + numberListNode->length;
                        }
                        return true;
                    }
                }
            }
        }

        return true;
    }

    bool parseNumber(const std::vector<TokenPtr> &s, size_t offset, NodePtr &root) {
        if (offset < s.size()) {
            if (auto &tokenPtr = s[offset]) {
                std::string &tokenContent = tokenPtr->content;
                if (!tokenContent.empty()) {
                    char c = tokenContent.at(0);
                    if (std::isdigit(c)) {
                        if (root->children) {
                            root->children->push_back(makeLeaf(tokenPtr));
                            root->length = 1;
                        }
                        return true;
                    }

                    if (c == '-') {
                        if (root->children) {
                            root->children->push_back(makeLeaf(tokenPtr));
                            root->length = 1;
                        }
                        return true;
                    }
                }
            }
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
            if (s[i] == ' ') {
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

    std::vector<int64_t> traverseNumList(NodePtr &root) {
        std::vector<int64_t> result;
        std::deque<NodePtr> candidates;
        candidates.push_back(root);
        while (!candidates.empty()) {
            NodePtr head = candidates.front();
            candidates.pop_front();

            if (head) {
                if (head->type == NodeType::Number) {
                    if (head->children && !head->children->empty()) {
                        auto numberNode = head->children->at(0);
                        if (numberNode->tokenPtr && numberNode->tokenPtr->content.size() < 20) {
                            // An int64_t in decimal has at most 20 digits, i.e.: Log10[2^64-1] ~= 20
                            // So if an integer's digits count exceed 20, there might be something wrong,
                            // and we currently don't support BigInt yet.
                            int64_t num = std::stol(numberNode->tokenPtr->content);
                            result.push_back(num);
                        }
                    }
                } else if (head->type == NodeType::NumberList) {
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

    std::vector<std::pair<std::string, std::vector<int64_t>>> traverseRows(
            NodePtr &root
    ) {
        std::vector<std::pair<std::string, std::vector<int64_t>>> result;
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
                        auto numListNode = head->children->at(1);
                        if (keyNode && keyNode->children && !keyNode->children->empty()) {
                            auto stringNode = keyNode->children->at(0);
                            if (stringNode->tokenPtr) {
                                if (numListNode) {
                                    std::pair<std::string, std::vector<int64_t>> pair;
                                    pair.first = stringNode->tokenPtr->content;
                                    auto numListVector = traverseNumList(numListNode);
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

    std::optional<std::vector<std::pair<std::string, std::vector<int64_t>>>> parse(std::string s) {
        NodePtr root = makeNonLeaf(NodeType::CSV);
        std::vector<TokenPtr> tokens = split(s);

        if (!parseCSV(tokens, 0, root)) {
            std::cerr << "Parse failed." << std::endl;
            return {};
        }

        return traverseRows(root);
    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MYTESTCASEPARSER_HPP
