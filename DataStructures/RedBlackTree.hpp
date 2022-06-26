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
                        debugPrintTreeExpr(root->left->left);
                        printSeparator();
                        debugPrintTreeExpr(root->left->right);
                        printSeparator();
                        debugPrintTreeExpr(root->right->left);
                        printSeparator();
                        debugPrintTreeExpr(root->right->right);
                        std::cout << "]";
                    } else {
                        // 3-Node
                        std::cout << "NodeType3[";
                        print3Node(root);
                        std::cout << "]";

                        // Children of 3-Node
                        std::cout << "[";
                        debugPrintTreeExpr(root->left->left);
                        printSeparator();
                        debugPrintTreeExpr(root->left->right);
                        printSeparator();
                        debugPrintTreeExpr(root->right);
                        std::cout << "]";
                    }
                } else {
                    // 2-Node
                    std::cout << "NodeType2[";
                    printNodeKeyValuePair(root);
                    std::cout << "]";

                    // Children of 2-Node

                    std::cout << "[";
                    debugPrintTreeExpr(root->left);
                    printSeparator();
                    debugPrintTreeExpr(root->right);
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
     static bool debugCheckDefinition(NodePtr root, bool suppressDebug = false) {

         auto pass = checkDef1_1(root, suppressDebug) && checkDef1_2(root, suppressDebug) && checkDef1_3(root, suppressDebug);
         if (!suppressDebug) {
             std::cout << "Checking definition 1... ";
             std::cout << pass << "\n";
         }

         return pass;
     }

     /** 删除红黑树中最小的 key 对应的节点，返回一个指针指向删除了最小节点之后的树 */
     static NodePtr deleteMin(NodePtr root) {
         return doDeleteMin(root);
     }

private:

    /** 尝试更新一个节点的 size */
    static void updateSize(NodePtr root) {
        if (root) {
            root->size = 1 + getSize(root->left) + getSize(root->right);
        }
    }

    /** 检验定义 1 的条件 (1) */
    static bool checkDef1_1(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Checking condition (1) of definition 1...";
        }

        bool pass = doCheckDef1_1Recursive(root);

        if (!suppressDebug) {
            std::cout << pass << "\n";
        }

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
    static bool checkDef1_2(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Checking condition (2) of definition 1... ";
        }

        bool pass = doCheckDef1_2Recursive(root);

        if (!suppressDebug) {
            std::cout << pass << "\n";
        }

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
    static bool checkDef1_3(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Traversing every path from root to nil... \n";
        }

        std::deque<NodePtr> path {};
        std::vector<std::deque<NodePtr>> paths {};
        std::vector<size_t> heights {};
        auto onPathAddedDoNothing = [](
            std::deque<NodePtr>& path,
            size_t height,
            std::vector<std::deque<NodePtr>>& paths,
            std::vector<size_t>& heights
        ) {};

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

        if (suppressDebug) {
            traverseAllPaths(
                    root,
                    0,
                    path,
                    paths,
                    heights,
                    onPathAddedDoNothing
            );
        } else {
            traverseAllPaths(
                    root,
                    0,
                    path,
                    paths,
                    heights,
                    onPathAdded
            );
        }

        auto beginIt = std::begin(heights);
        auto endIt = std::end(heights);
        auto minHeight = std::min_element(beginIt, endIt);
        auto maxHeight = std::max_element(beginIt, endIt);

        auto pass = *minHeight == *maxHeight;
        if (!suppressDebug) {
            std::cout << "Checking condition (3) of definition 1... ";
            std::cout << pass << "\n";
        }
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
        updateSize(root);

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
        updateSize(root);

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
     *
     *          root                newRoot
     *         /                    /
     *        B               A -- B -- C
     *      /  \      ==>    / \       / \
     *     /    \           1  2      3  4
     *    A      C
     *   / \    / \
     *  1   2  3   4
     *
     */
    static void merge2Nodes(NodePtr root) {
        if (root) {
            if (isBlack(root->left) && isBlack(root->right)) {
                root->left->color = LinkType::RED;
                root->right->color = LinkType::RED;
                root->color = LinkType::BLACK;
            }
        }
    }

    /**
     * 设 root 指向一个 2-Node, 该 2-Node 的左儿子是一个 2-Node,
     * 右儿子是一个 3-Node, 设 root 的 key 是 B, 左儿子的 key 是 A,
     * 右儿子的 key 是 C, D.
     *
     *      B
     *    /  \
     *   A  C D
     *
     * 那么该函数返回一个指针指向这样的 2-Node:
     *
     *    C
     *   / \
     * A B  D
     *
     * 注意不要将该函数简单地等同于左旋操作函数。
     *
     */
    static NodePtr moveSiblingFromRight(NodePtr root) {
        if (root && root->left && root->right && isRed(root->right->left)) {
            root->right = rotateRight(root->right);
            NodePtr newRoot = rotateLeft(root);
            newRoot->left->left->color = LinkType::RED;
            newRoot->left->color = LinkType::BLACK;

            return newRoot;
        }

        // Trying to moveSiblingFromRight a NodePtr that shouldn't.
        assert((false));
    }

    /**
     * 设 root 指向一个 2-Node, 该 2-Node 的左儿子是一个 3-Node,
     * 右儿子是一个 2-Node, 设 root 的 key 是 C, 左儿子的 key 是 A B,
     * 右儿子的 key 是 D.
     *
     *      C
     *     / \
     *  A B  D
     *
     * 那么该函数返回一个指针指向这样的 2-Node:
     *
     *    B
     *   / \
     *  A  C D
     *
     * 注意不要将该函数简单地等同于右旋操作函数。
     *
     */
    static NodePtr moveSiblingFromLeft(NodePtr root) {
        if (root && root->left && isRed(root->left->left) && root->right) {
            NodePtr newRoot = rotateRight(root);
            newRoot->right->color = LinkType::BLACK;
            newRoot->right = rotateLeft(newRoot->right);
            updateSize(newRoot);
        }

        assert((false));
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
        root = reAdjustLinkColor(root);

        updateSize(root);

        return root;
    }

    /** 判定是否是 2-Node */
    static bool is2Node(NodePtr root) {
        if (root) {
            if ((!isRed(root->left)) && (!isRed(root->right))) {
                return true;
            }
        }

        return false;
    }

    /** 判定是否是 3-Node */
    static bool is3Node(NodePtr root) {
        if (root && isRed(root->left) && (!isRed(root->right))) {
            return true;
        }

        return false;
    }

    /** 判定是否是 4-Node */
    static bool is4Node(NodePtr root) {
        if (root && isRed(root->left) && isRed(root->right)) {
            return true;
        }

        return false;
    }

    /** 删除 key 最小的节点 */
    static NodePtr doDeleteMin(NodePtr root) {
        // 在根节点部位，先进行一下预处理：
        // 主要针对的是当 root 为 2-Node 时 root->left 为 2-Node 或者当 root 为 3-Node 时，root->left->left 为 2-Node 的情况。
        if (root) {
            if (is2Node(root)) {
                if (is2Node(root->left)) {
                    if (is2Node(root->right)) {
                        merge2Nodes(root);
                        root = doDeleteMinRecursive(root);
                    } else if (is3Node(root->right)) {
                        root = moveSiblingFromRight(root);
                        root->left = doDeleteMinRecursive(root->left);
                    } else {
                        // root->left point to a 2-Node, but root->right is neither 2-Node nor 3-Node
                        assert((false));
                    }
                } else if (is3Node(root->left)) {
                    root->left = doDeleteMinRecursive(root->left);
                } else {
                    return nullptr;
                }
            } else if (is3Node(root)) {
                root = doDeleteMinRecursive(root);
            } else {
                assert((false));
            }

            updateSize(root);
        }

        return root;
    }
    static NodePtr doDeleteMinRecursive(NodePtr root) {
        // 经过 doDeleteMin 函数的初步处理，到这里时，root 已经不再可能是一个 2-Node，只可能会是 3-Node 或者 4-Node.
        if (is3Node(root) || is4Node(root)) {
            NodePtr nextLevelLeft = root->left->left;
            NodePtr nextLevelRight = root->left->right;
            if (is2Node(nextLevelLeft)) {
                if (is2Node(nextLevelRight)) {
                    merge2Nodes(root->left);
                    root->left->color = LinkType::BLACK;
                    root->left = doDeleteMinRecursive(root->left);
                    updateSize(root->left);
                    updateSize(root);
                } else if (is3Node(nextLevelRight)) {
                    root->left = moveSiblingFromRight(root->left);
                    root->left->left = doDeleteMinRecursive(root->left->left);
                    updateSize(root->left->left);
                    updateSize(root->left);
                    updateSize(root);
                } else {
                    assert((false));
                }
            } else if (is3Node(nextLevelLeft) || is4Node(nextLevelLeft)) {
                root->left->left = doDeleteMinRecursive(root->left->left);
                updateSize(root->left->left);
                updateSize(root->left);
                updateSize(root);
            } else {
                // 到达最底层
                if (is3Node(root)) {
                    root->left = nullptr;
                    updateSize(root);
                    return root;
                } else if (is4Node(root)) {
                    root->left = nullptr;
                    NodePtr newRoot = rotateLeft(root);
                    updateSize(newRoot);
                    return newRoot;
                } else {
                    return nullptr;
                }
            }

            // 后续处理
            root = reAdjustLinkColor(root);

            updateSize(root);

            return root;
        } else {
            return nullptr;
        }
    }

    /** 获取最小值节点 */
    static NodePtr min(NodePtr root) {
        if (root) {
            if (root->left) {
                return min(root->left);
            }

            return root;
        }

        return nullptr;
    }

    /** 获取最大值节点 */
    static NodePtr max(NodePtr root) {
        if (root) {
            if (root->right) {
                return max(root->right);
            }

            return root;
        }

        return nullptr;
    }

    /** 删除 key 最大的节点 */
    static NodePtr doDeleteMax(NodePtr root) {
        if (is2Node(root)) {
            if (is2Node(root->right)) {
                if (is3Node(root->left)) {
                    root = moveSiblingFromLeft(root);
                    return doDeleteMaxRecursive(root->right);
                } else if (is2Node(root->left)) {
                    merge2Nodes(root);
                    return doDeleteMaxRecursive(root);
                } else {
                    assert((false));
                }
            } else if (is3Node(root->right)) {
                return doDeleteMaxRecursive(root->right);
            } else {
                return nullptr;
            }
        } else if (is3Node(root)) {
            return doDeleteMaxRecursive(root);
        } else {
            return root;
        }
    }
    static NodePtr doDeleteMaxRecursive(NodePtr root) {
        bool is3NodeOr4Node = is3Node(root) || is4Node(root);
        assert((is3NodeOr4Node));

        if (is3Node(root)) {
            if (is2Node(root->right)) {
                if (is3Node(root->left)) {
                    root = moveSiblingFromLeft(root);
                    root->right = doDeleteMaxRecursive(root->right);
                } else if (is2Node(root->left)) {
                    merge2Nodes(root);
                    root = doDeleteMaxRecursive(root);
                } else {
                    assert((false));
                }
            } else if (is3Node(root->right)) {
                root->right = doDeleteMaxRecursive(root->right);
            } else {
                return root->left;
            }
        } else if (is4Node(root)) {
            if (is2Node(root->right->right)) {
                if (is3Node(root->right->left)) {
                    root->right = moveSiblingFromLeft(root->right);
                    root->right->right = doDeleteMaxRecursive(root->right->right);
                    updateSize(root->right);
                    updateSize(root);
                } else if (is2Node(root->right->left)) {
                    merge2Nodes(root->right);
                    root->right = doDeleteMaxRecursive(root->right);
                    updateSize(root);
                } else {
                    assert((false));
                }
            } else if (is3Node(root->right->right)) {
                root->right->right = doDeleteMaxRecursive(root->right->right);
                updateSize(root->right);
                updateSize(root);
            } else {
                root->right = nullptr;
                updateSize(root);
                return root;
            }
        } else {
            assert((false));
        }

        root = reAdjustLinkColor(root);
        return root;
    }

    static NodePtr reAdjustLinkColor(NodePtr root) {
        if (isRed(root->left) && isRed(root->left->left)) {
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

    static void assignNodeValue(NodePtr lhs, NodePtr rhs) {
        if (lhs) {
            if (rhs) {
                lhs->key = rhs->key;
                lhs->value = rhs->value;
                return;
            }
        }

        assert((false));
    }

    static NodePtr delete2NodeSelf(NodePtr root) {
        assert((is2Node(root)));
        if (is2Node(root)) {
            if (is2Node(root->left) && is2Node(root->right)) {
                merge2Nodes(root);
                return doDeleteNodeByKeyRecursive(root, root->key);
            } else if (is3Node(root->left)) {
                NodePtr maxNodeInLHS = max(root->left);
                root->left = doDeleteMax(root->left);
                updateSize(root);
                root->key = maxNodeInLHS->key;
                root->value = maxNodeInLHS->value;
                return root;
            } else if (is3Node(root->right)) {
                NodePtr minNodeInRHS = min(root->right);
                root->right = doDeleteMin(root->right);
                updateSize(root);
                root->key = minNodeInRHS->key;
                root->value = minNodeInRHS->value;
            } else {
                return nullptr;
            }
        } else if (is3Node(root)) {
            if (is3Node(root->right)) {
                // 可以直接从 root->right 提取 replacement
                NodePtr repl = min(root->right);
                root->right = doDeleteMin(root->right);
                updateSize(root);
                assignNodeValue(root, repl);
                return root;
            } else {
                // 现在，尝试从 root->left 寻找 replacement, 主要是找 max
                if (is2Node(root->left->right)) {
                    if (is2Node(root->left->left)) {
                        merge2Nodes(root->left);
                        NodePtr repl = max(root->left);
                        root->left = doDeleteMax(root->left);
                        assignNodeValue(root, repl);
                        updateSize(root);
                        return root;
                    } else if (is3Node(root->left->left)) {
                        root->left = moveSiblingFromLeft(root->left); // 从 root->left->left 往 root->left->right 移一个
                        NodePtr repl = max(root->left->right);
                        root->left->right = doDeleteMax(root->left->right);
                        assignNodeValue(root, repl);
                        updateSize(root);
                        return root;
                    } else {
                        assert((false));
                    }
                } else if (is3Node(root->left->right)) {
                    NodePtr repl = max(root->left->right);
                    root->left->right = doDeleteMax(root->left->right);
                    assignNodeValue(root, repl);
                    updateSize(root);
                    return root;
                } else {
                    assert((false));
                }
            }
        } else if (is4Node(root)) {
            // 仅仅考虑从 root->right 提取 replacement, 也就是找到 root->right 的 min, 然后 deleteMin
            if (is2Node(root->right->left)) {
                if (is2Node(root->right->right)) {
                    merge2Nodes(root->right);
                    NodePtr repl = min(root->right);
                    root->right = doDeleteMin(root->right);
                    updateSize(root);
                    assignNodeValue(root, repl);
                    return root;
                } else if (is3Node(root->right->right)) {
                    root->right = moveSiblingFromRight(root->right); // 从 root->right->right 借调一个到 root->right->left
                    NodePtr repl = min(root->right);
                    root->right->left = doDeleteMin(root->right->left);
                    updateSize(root->right);
                    updateSize(root);
                    assignNodeValue(root, repl);
                    return root;
                } else {
                    assert((false));
                }
            }
        } else {
            return nullptr;
        }
    }

    /** 删除给定 key 对应的节点，从而让相应的键值对被删除掉 */
    static NodePtr doDeleteNodeByKey(NodePtr root, const KeyT& key) {
        if (is3Node(root)) {
            return doDeleteNodeByKeyRecursive(root, key);
        }
        else if (is2Node(root)) {
            if (key > *root->key) {
                if (is2Node(root->right)) {
                    if (is3Node(root->left)) {
                        root = moveSiblingFromLeft(root);
                        return doDeleteNodeByKeyRecursive(root, key);
                    }
                    else if (is2Node(root->left)) {
                        merge2Nodes(root);
                        return doDeleteNodeByKeyRecursive(root, key);
                    } else {
                        assert((false)); // Non-balanced tree
                    }
                } else if (is3Node(root->right)) {
                    return doDeleteNodeByKeyRecursive(root->right, key);
                } else {
                    // root->right is null
                    return root;
                }
            } else if (key < *root->key) {
                if (is3Node(root->left)) {
                    return doDeleteNodeByKeyRecursive(root->left, key);
                }
                else if (is2Node(root->left)) {
                    if (is3Node(root->right)) {
                        root = moveSiblingFromRight(root);
                        return doDeleteNodeByKeyRecursive(root->left);
                    } else if (is2Node(root->right)) {
                        merge2Nodes(root);
                        return doDeleteNodeByKeyRecursive(root);
                    } else {
                        assert((false));
                    }
                }
                else {
                    // root -> left is null
                    return root;
                }
            }
            else {
                // key == *root->key
                return delete2NodeSelf(root);
            }
        } else {
            return root;
        }
    }

    static NodePtr deleteBottomNode(NodePtr root, const KeyT& key) {
        // for security
        assert(((is3Node(root) || is4Node(root))));

        if (is4Node(root)) {
            if (key == *root->left->key) {
                root->left = nullptr;
                root = rotateLeft(root);
                return root;
            }

            if (key == *root->key) {
                NodePtr newRoot = root->right;
                newRoot->left = root->left;
                newRoot->left->color = LinkType::RED;
                newRoot->color = root->color;
                updateSize(newRoot);

                return newRoot;
            }

            if (key == *root->right->key) {
                root->right = nullptr;
                return root;
            }

            // delete nothing
            return root;
        }

        if (is3Node(root)) {
            if (key == *root->key) {
                NodePtr newRoot = root->left;
                newRoot->color = root->color;
                return newRoot;
            }

            if (key == *root->left->key) {
                root->left = nullptr;
                updateSize(root);
                return root;
            }

            // delete nothing
            return root;
        }
    }

    static NodePtr deleteFromChildOf2Node(NodePtr root, const KeyT& key) {
        if (is2Node(root->left) && is2Node(root->right)) {
            merge2Nodes(root);
            return doDeleteNodeByKeyRecursive(root, key);
        }

        if (key < *root->key) {
            if (is2Node(root->left)) {
                root = moveSiblingFromRight(root);
            }
            return doDeleteNodeByKeyRecursive(root->left, key);
        } else if (key > *root->key) {
            if (is2Node(root->right)) {
                root = moveSiblingFromLeft(root);
            }
            return doDeleteNodeByKeyRecursive(root->right, key);
        } else {
            // key == *root->key
            return delete2NodeSelf(root);
        }
    }

    static NodePtr doDeleteNodeByKeyRecursive(NodePtr root, const KeyT& key) {
        assert(((is3Node(root) || is4Node(root))));
        if (root->left) {
            if (is3Node(root)) {
                if (key == *root->left->key) {
                    root->left = delete2NodeSelf(root->left);
                    updateSize(root);
                } else if (key == *root->key) {
                    root = rotateRight(root);
                    root->right = delete2NodeSelf(root->right);
                    updateSize(root);
                    root = rotateLeft(root);
                    updateSize(root);
                } else if (key < *root->left->key || (key > *root->left->key && key < *root->key)) {
                    root->left = deleteFromChildOf2Node(root->left);
                    updateSize(root);
                } else if (key > *root->key) {
                    // search child 3, i.e. root->right
                    root = rotateRight(root);
                    root->right = deleteFromChildOf2Node(root->right);
                    updateSize(root);
                    root = rotateLeft(root);
                } else {
                    assert((false));
                }
            }
            else if (is4Node(root)) {
                if (key < *root->key) {
                    root->left = deleteFromChildOf2Node(root->left);
                    updateSize(root);
                } else if (key > *root->key) {
                    root->right = deleteFromChildOf2Node(root->right);
                } else {
                    root = delete2NodeSelf(root);
                    updateSize(root);
                }
            } else {
                assert((false));
            }

            root = reAdjustLinkColor(root);

            return root;
        } else {
            // Reached bottom
            return deleteBottomNode(root, key);
        }
    }
};

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
