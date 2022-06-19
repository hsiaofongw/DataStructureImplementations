//
// Created by 韦晓枫 on 2022/6/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP

#include <memory>
#include <string>
#include <iostream>

/** 表颜色 */
enum class LinkType {
    RED, BLACK
};

/** 红黑树节点 */
template <typename KeyT, typename ValT>
struct RedBlackNode {

    RedBlackNode(const std::shared_ptr<KeyT>& k, const std::shared_ptr<ValT>& v, LinkType color)
    : key(k), value(v), color(color), left(nullptr), right(nullptr), size(1) { }

    LinkType color;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> left;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> right;
    std::shared_ptr<KeyT> key;
    std::shared_ptr<ValT> value;
    size_t size;
};

/** 红黑树节点指针 */
template <typename KeyT, typename ValT>
using RedBlackNodePtr = std::shared_ptr<RedBlackNode<KeyT, ValT>>;

/** 红黑树操作句柄 */
template <typename KeyT, typename ValT>
class RedBlackTreeHandle {
    using RedBlackNode = RedBlackNode<KeyT, ValT>;
    using NodePtr = RedBlackNodePtr<KeyT, ValT>;
    using KeyPtr = std::shared_ptr<KeyT>;
    using ValuePtr = std::shared_ptr<ValT>;

public:

    static size_t getSize(NodePtr root) {
        if (root) {
            return root->size;
        }

        return 0;
    }

    /** 插入操作 */
    static NodePtr insert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        NodePtr result = doInsert(root, k, v);
        result->color = LinkType::BLACK;
        return result;
    }

    /** 打印 Key 表达式 */
    static void debugPrintTreeExpr(NodePtr root) {

        auto printSeparator = []() -> void {
            std::cout << ", ";
        };

        auto printNodeKeyValuePair = [printSeparator](const NodePtr& root) -> void {
            std::cout << "Pair[";
            std::cout << "Key -> ";
            std::cout << *root->key;
            printSeparator();
            std::cout << "Value -> ";
            std::cout << *root->value;
            std::cout << "]";
        };

        auto print3Node = [printNodeKeyValuePair, printSeparator](const NodePtr& root) -> void {
            std::cout << "Pairs[";
            printNodeKeyValuePair(root->left);
            printSeparator();
            printNodeKeyValuePair(root);
            std::cout << "]";
        };

        auto print4Node = [printNodeKeyValuePair, printSeparator](const NodePtr& root) -> void {
            std::cout << "Pairs[";
            printNodeKeyValuePair(root->left);
            printSeparator();
            printNodeKeyValuePair(root);
            printSeparator();
            printNodeKeyValuePair(root->right);
            std::cout << "]";
        };

        if (root) {
            // Non-Null
            if (root->left) {
                // Non-Leaf
                if (isRed(root->left)) {
                    if (isRed(root->right)) {
                        // 4-Node
                        std::cout << "NodeType4[";
                        print4Node(root);
                        std::cout << "]";

                        // Children of 4-Node
                        std::cout << "[";
                        if (root->left->left) {
                            debugPrintTreeExpr(root->left->left);
                            printSeparator();
                            debugPrintTreeExpr(root->left->right);
                            printSeparator();
                            debugPrintTreeExpr(root->right->left);
                            printSeparator();
                            debugPrintTreeExpr(root->right->right);
                        }
                        std::cout << "]";
                    } else {
                        // 3-Node
                        std::cout << "NodeType3[";
                        print3Node(root);
                        std::cout << "]";

                        // Children of 3-Node
                        std::cout << "[";
                        if (root->left->left) {
                            debugPrintTreeExpr(root->left->left);
                            printSeparator();
                            debugPrintTreeExpr(root->left->right);
                            printSeparator();
                            debugPrintTreeExpr(root->right);
                        }
                        std::cout << "]";
                    }
                } else {
                    // 2-Node
                    std::cout << "NodeType2[";
                    printNodeKeyValuePair(root);
                    std::cout << "]";

                    // Children of 2-Node

                    std::cout << "[";
                    if (root->left) {
                        debugPrintTreeExpr(root->left);
                        printSeparator();
                        debugPrintTreeExpr(root->right);
                    }
                    std::cout << "]";
                }
            } else {
                // Leaf
                std::cout << "NodeLeaf[";
                printNodeKeyValuePair(root);
                std::cout << "]";
            }
        }
    }

private:

    /** 从左到右旋转，右旋 */
    static NodePtr rotateRight(NodePtr root) {
        assert((root->left));

        NodePtr result { root->left };
        root->left = result->right;
        result->right = root;
        result->color = root->color;
        root->color = LinkType::RED;
        result->size = root->size;
        root->size = 1 + getSize(root->left) + getSize(root->right);

        return result;
    }

    /** 从右到左旋转，左旋 */
    static NodePtr rotateLeft(NodePtr root) {
        assert((root->right));

        NodePtr result { root->right };
        root->right = result->left;
        result->left = root;
        result->color = root->color;
        root->color = LinkType::RED;
        result->size = root->size;
        root->size = 1 + getSize(root->left) + getSize(root->right);

        return result;
    }

    /** 颜色反转 */
    static void flipColor(NodePtr root) {
        if (root) {
            root->color = LinkType::RED;

            if (root->left) {
                root->left->color = LinkType::BLACK;
            }

            if (root->right) {
                root->right->color = LinkType::BLACK;
            }
        }
    }

    /** 判断一个节点的父边是否是红色的 */
    static bool isRed(NodePtr root) {
        if (root) {
            if (root->color == LinkType::RED) {
                return true;
            }
        }

        return false;
    }

    /** 判断一个节点的父边是否是黑色的 */
    static bool isBlack(NodePtr root) {
        if (root) {
            if (root->color == LinkType::BLACK) {
                return true;
            }
        }

        return false;
    }

    /**
     * 设 root 是一个 2-node 的指针，该 2-node 的左右 child 都是 2-node,
     * 那么此函数就把这 3 个 2-node 合并成一个 4-node, 然后返回一个指向这个 4-node 的指针。
     */
    static void merge2Nodes(NodePtr root) {
        if (root) {
            if (isBlack(root->left) && isBlack(root->right)) {
                root->left->color = LinkType::RED;
                root->right->color = LinkType::RED;
            }
        }
    }

    /**
     * 2(2, 3) => 2(3, 2), 2(2, 4) => 2(3, 3)
     * b(a, cd) => c(ab, d), b(a, cde) => c(ab, de)
     */
    static NodePtr moveSibling(NodePtr root) {
        if (root && root->left && root->right && isRed(root->right->left)) {
            root->left->color = LinkType::RED;
            root->right = rotateRight(root->right);
            NodePtr newRoot = rotateLeft(root);
            newRoot->left = LinkType::BLACK;
            newRoot->right = LinkType::BLACK;

            if (newRoot->right && newRoot->right->right) {
                newRoot->right = rotateLeft(newRoot->right);
            }

            return newRoot;
        }

        return root;
    }

    /** 插入操作 */
    static NodePtr doInsert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        if (!root) {
            return std::make_shared<RedBlackNode>(k, v, LinkType::RED);
        }

        if (*k > *root->key) {
            root->right = doInsert(root->right, k, v);
        } else if (*k < *root->key) {
            root->left = doInsert(root->left, k, v);
        } else {
            root->value = v;
        }

        // color 和 size 的更新都是在左、右旋的过程中进行
        if (root->left && isRed(root->left) && isRed(root->left->left)) {
            root = rotateRight(root);
        }

        if ((!isRed(root->left)) && isRed(root->right)) {
            root = rotateLeft(root);
        }

        if (isRed(root->left) && isRed(root->right)) {
            flipColor(root);
        }

        return root;
    }
};

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
