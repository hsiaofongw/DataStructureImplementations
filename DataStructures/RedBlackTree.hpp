//
// Created by 韦晓枫 on 2022/6/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP

#include <memory>
#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <functional>
#include <algorithm>

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

    /** 获取一颗红黑树上存储的键值对的数量 */
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

    /**
     * 检验红黑树定义是否被满足：
     *
     * 定义 1（Red-Black BST): 一个 Red-Black BST 是满足下列要求的 BST:
     *
     * (1) 红边总是指左；
     * (2) 每个节点都不会同时出现两个红边；
     * (3) 从根节点到每一个空边的路径中经过的黑边的数量都是相等的；
     */
     static bool debugCheckDefinition(NodePtr root) {

         auto pass = checkDef1_1(root) && checkDef1_2(root) && checkDef1_3(root);
         std::cout << "Checking definition 1 ... ";
         std::cout << pass << "\n";
         return pass;
     }

     /** 删除红黑树中最小的 key 对应的节点，返回一个指针指向删除了最小节点之后的树 */
     static NodePtr deleteMin(NodePtr root) {
         return doDeleteMin(root);
     }

private:

    /** 检验定义 1 的条件 (1) */
    static bool checkDef1_1(NodePtr root) {
        std::cout << "Checking condition (1) of definition 1 ... ";
        bool pass = doCheckDef1_1Recursive(root);
        std::cout << pass << "\n";
        return pass;
    }
    static bool doCheckDef1_1Recursive(NodePtr root) {
        if (root) {
            bool checkLeft = false;
                checkLeft = doCheckDef1_1Recursive(root->left);

            bool checkRight = false;
            checkRight = doCheckDef1_1Recursive(root->right);
            if (root->right) {
                if (isRed(root->right)) {
                    return false;
                }
            }

            return checkLeft && checkRight;
        }

        return true;
    }

    /** 检验定义 1 的条件 (2) */
    static bool checkDef1_2(NodePtr root) {
        std::cout << "Checking condition (2) of definition 1 ... ";
        bool pass = doCheckDef1_2Recursive(root);
        std::cout << pass << "\n";
        return pass;
    }
    static bool doCheckDef1_2Recursive(NodePtr root) {
        if (root) {
            if (root->left && root->right) {
                if (isRed(root->left) && isRed(root->right)) {
                    return false;
                }
            }

            return doCheckDef1_2Recursive(root->left) && doCheckDef1_2Recursive(root->right);
        }

        return true;
    }

    /** 检验定义 1 的条件 (3) */
    static bool checkDef1_3(NodePtr root) {
        std::cout << "Traversing every path from root to nil ... \n";
        std::deque<NodePtr> path {};
        std::vector<std::deque<NodePtr>> paths {};
        std::vector<size_t> heights {};
        auto onPathAdded = [](
                std::deque<NodePtr>& path,
                size_t height,
                std::vector<std::deque<NodePtr>>& paths,
                std::vector<size_t>& heights
        ) -> void {
            std::cout << "Path: ";
            size_t segmentIndex = 0;
            for (const auto& segment : path) {
                if (segmentIndex != 0) {
                    if (segment->color == LinkType::RED) {
                        std::cout << " -- ";
                    } else {
                        std::cout << " --> ";
                    }
                }
                std::cout << (*segment->key);
                ++segmentIndex;
            }
            std::cout << " -> (nil), ";
            std::cout << "Height: ";
            std::cout << height;
            std::cout << "\n";
        };

        traverseAllPaths(
                root,
                0,
                path,
                paths,
                heights,
                onPathAdded
        );

        auto beginIt = std::begin(heights);
        auto endIt = std::end(heights);
        auto minHeight = std::min_element(beginIt, endIt);
        auto maxHeight = std::max_element(beginIt, endIt);

        std::cout << "Checking condition (3) of definition 1 ... ";
        auto pass = *minHeight == *maxHeight;
        std::cout << pass << "\n";
        return pass;
    }

    /** 递归遍历每一条路径，到达 nil 链的时候将路径以及路径长度记录下来 */
    static void traverseAllPaths(
            NodePtr root,
            size_t height,
            std::deque<NodePtr>& path,
            std::vector<std::deque<NodePtr>>& paths,
            std::vector<size_t>& heights,
            const std::function<void (
                    std::deque<NodePtr>& path,
                    size_t height,
                    std::vector<std::deque<NodePtr>>& paths,
                    std::vector<size_t>& heights
            )>& onPathAdded
    ) {
        if (root) {
            path.emplace_back(root);
            traverseAllPaths(root->left, isRed(root->left) ? height : height + 1, path, paths, heights, onPathAdded);
            traverseAllPaths(root->right, isRed(root->right) ? height: height + 1, path, paths, heights, onPathAdded);
            path.pop_back();
        }
        else {
            paths.emplace_back(std::deque<NodePtr> { path });
            heights.push_back(height);

            onPathAdded(path, height, paths, heights);
        }
    }

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

    /** 判定是否是 2-Node */
    static bool is2Node(NodePtr root) {
        if (root) {
            if (root->left && isBlack(root->left) && root->right && isBlack(root->right)) {
                return true;
            }
        }

        return false;
    }

    /** 判定是否是 3-Node */
    static bool is3Node(NodePtr root) {
        if (root) {
            if (root->left && isRed(root->left) && root->right && isBlack(root->right)) {
                return true;
            }
        }

        return false;
    }

    /** 判定是否是 4-Node */
    static bool is4Node(NodePtr root) {
        if (root) {
            if (root->left && root->right && isRed(root->left) && isRed(root->right)) {
                return true;
            }
        }

        return false;
    }

    /** 删除 key 最小的节点 */
    static NodePtr doDeleteMin(NodePtr root) {
        // 在根节点部位，先进行一下预处理：

        // case 0:
        // root 指向一颗空树（没有任何节点的树），则原样返回
        if (!root) {
            return root;
        }

        // case 1:
        // root 是一个 2-Node, root->left 是一个 2-Node, root->right 也是一个 2-Node.
        // 这种情况下我们选择合并, 把 root->left, root, root->right 放置到同一层
        if (is2Node(root) && is2Node(root->left) && is2Node(root->right)) {
            merge2Nodes(root);
        }

        // case 2:
        // root 是一个 2-Node, root->left 是一个 2-Node, 但是 root->right 是一个 3-Node.
        // 这种情况下我们选择将 root 移往左，而原先 root->right 的左子节点接替 root
        else if (is2Node(root) && is2Node(root->left) && is3Node(root->right)) {
            root = moveSibling(root);
        }

        // case 3:
        // root 是一个 3-Node, root->left->left 是一个 2-Node,
        // 无论 root->left->right 是不是 2-Node, 这类情况都会在递归下降的过程中得到妥善处置。
        else {
            ; // No op.
        }

        // 其他边边角角情况处理：
        if (!root->right) {
            root->left = nullptr;
            return root;
        }
        if (!root->left) {
            return root;
        }

        root = doDeleteMinRecursive(root);
    }

    static NodePtr doDeleteMinRecursive(NodePtr root) {
        // 经过 doDeleteMin 函数的初步处理，到这里时，root 已经不再可能是一个 2-Node，只可能会是 3-Node 或者 4-Node.
        if (root) {
            if (root->left && isRed(root->left)) {
                // 总是会执行到这里
                if (root->left->left) {
                    // 还有下一层，判定一下它的最左直接 child 是不是 2-Node,
                    // 如果是，还要进行相应的处理，把它转为非 2-Node.
                    if (is2Node(root->left->left)) {
                        if (is2Node(root->left->right)) {
                            root->left = merge2Nodes(root->left);
                            root->left->color = LinkType::BLACK;
                        } else if (is3Node(root->left->right)) {
                            root->left = moveSibling(root->left);
                        } else {
                            ; // No op.
                        }
                    }
                    root->left = doDeleteMinRecursive(root->left);
                } else {
                    // 到达最底层
                    if (is3Node(root)) {
                        root->left = nullptr;
                        return root;
                    } else if (is4Node(root)) {
                        root->left = nullptr;
                        return rotateLeft(root);
                    } else {
                        ; // No op.
                    }
                }

                if (isRed(root->left) && root->left->left && isRed(root->left->left)) {
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
        }

        assert((false));
    }
};

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP