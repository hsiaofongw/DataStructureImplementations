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
#include <utility>

namespace DataStructure {
    namespace RedBlackTree {

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

            /**
             * 插入操作，注意不要用原来的指针，返回的新指针才是指向插入（或者更新）了传入的键值对的树的指针。
             * 例如你可以传入一个 nullptr 作为 root 的实参（这代表一个空树或者说指向空树的指针），
             * 但是该函数会在插入操作成功后返回一个指向非空树的指针，那又是另一个指针了。
             */
            static NodePtr insert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
                NodePtr result = doInsert(root, k, v);
                result->color = LinkType::BLACK;
                return result;
            }

            /** 搜索操作，返回相应的节点指针，如果没有满足的，返回空指针。 */
            static NodePtr searchNodeByKey(NodePtr root, const KeyT& key) {
                NodePtr head = root;
                while (head) {
                    if (key > *head->key) {
                        head = head->right;
                    } else if (key < *head->key) {
                        head = head->left;
                    } else {
                        return head;
                    }
                }

                return nullptr;
            }

            /**
             * 搜索操作，返回相应的键值对（键和值都是指针形式，键值对对象本身以 std::pair 模板实例的对象形式体现），
             * 假如说没有找到，键值对里的值部分会是一个空指针，正常情况下（找到里），键值对的值会是一个指向值的非空的共享指针。
             */
            static std::pair<KeyPtr, ValuePtr> searchKeyValuePairByKey(NodePtr root, const KeyT& key) {
                NodePtr result = searchNodeByKey(root, key);
                if (result) {
                    return std::pair<KeyPtr, ValuePtr> { result->key, result->value };
                }

                return std::pair<KeyPtr, ValuePtr> { nullptr, nullptr };
            }

            /** 打印 Key 表达式 */
            static void debugPrintTreeExpr(NodePtr root) {
                std::string displayContent;
                treeToRepresentation(root, 0, displayContent, 4);
                std::cout << displayContent;
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

                auto check1 = checkDef1_1(root, suppressDebug);
                auto check2 = checkDef1_2(root, suppressDebug);
                auto check3 = checkDef1_3(root, suppressDebug);
                auto pass = check1 && check2 && check3;
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
                    bool checkLeft = doCheckDef1_1Recursive(root->left);
                    bool checkRight = doCheckDef1_1Recursive(root->right);
                    bool checkThis = !isRed(root->right);

                    return checkLeft && checkRight && checkThis;
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

            /** 从左到右旋转，右旋，完事后得到的新的 root 是指右的，也就是说它的右儿子的颜色为 RED. */
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

            /** 从右到左旋转，左旋，完事后得到的新的 root 是指左的，也就是说它的左儿子的颜色为 RED. */
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
                    newRoot->right->color = LinkType::BLACK;

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

            /** 判定一个 NodePtr 是否指向一个 2-Node */
            static bool is2Node(NodePtr root) {
                if (root) {
                    if ((!isRed(root->left)) && (!isRed(root->right))) {
                        return true;
                    }
                }

                return false;
            }

            /** 判定一个 NodePtr 是否指向一个 3-Node */
            static bool is3Node(NodePtr root) {
                if (root && isRed(root->left) && (!isRed(root->right))) {
                    return true;
                }

                return false;
            }

            /** 判定一个 NodePtr 是否指向一个 4-Node */
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

            /** 获取指向最小值节点的指针 */
            static NodePtr min(NodePtr root) {
                if (root) {
                    if (root->left) {
                        return min(root->left);
                    }

                    return root;
                }

                return nullptr;
            }

            /** 获取指向最大值节点的指针 */
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
                        return nullptr;
                    }
                } else if (is3Node(root)) {
                    root = doDeleteMaxRecursive(root);
                } else {
                    return root;
                }

                updateSize(root);
                return root;
            }
            static NodePtr doDeleteMaxRecursive(NodePtr root) {
                bool is3NodeOr4Node = is3Node(root) || is4Node(root);
                assert((is3NodeOr4Node));

                if (is3Node(root)) {
                    if (is2Node(root->right)) {
                        root = rotateRight(root);
                        if (is3Node(root->right->left)) {
                            root->right = moveSiblingFromLeft(root->right);
                            root->right->right = doDeleteMaxRecursive(root->right->right);
                            updateSize(root->right);
                        } else if (is2Node(root->left->right)) {
                            merge2Nodes(root->right);
                            root->right = doDeleteMaxRecursive(root->right);
                        } else {
                            assert((false));
                        }
                        root = rotateLeft(root);
                        updateSize(root);
                    } else if (is3Node(root->right)) {
                        root->right = doDeleteMaxRecursive(root->right);
                    } else {
                        // 触底
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
                        // 触底
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

            /** 将红链接向上传递，使得该函数最后从根部退出时，红黑树的性质得以保持。 */
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

            /** 将 rhs 指向的节点的 key 和 value 同步到 lhs 指向的 */
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

            /**
             * 假定 root 指向一个 2-Node, 3-Node 或者 4-Node, 再假定它们的儿子都不为 null.
             * 该函数删除 root 指向的那个节点本身，具体是通过在 root 的左分支或者右分支找到接替者 repl,
             * 用 repl 的 key 和 value 覆盖 root 的 key 和 value, 然后把这个 repl 删除。
             */
            static NodePtr deleteNodeSelf(NodePtr root) {
                if (is2Node(root)) {
                    if (is2Node(root->left) && is2Node(root->right)) {
                        merge2Nodes(root);
                        return doDeleteNodeByKeyRecursive(root, root->key);
                    } else if (is3Node(root->left)) {
                        NodePtr maxNodeInLHS = max(root->left);
                        root->left = doDeleteMax(root->left);
                        updateSize(root);
                        assignNodeValue(root, maxNodeInLHS);
                        return root;
                    } else if (is3Node(root->right)) {
                        NodePtr minNodeInRHS = min(root->right);
                        root->right = doDeleteMin(root->right);
                        updateSize(root);
                        assignNodeValue(root, minNodeInRHS);
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
                        return deleteNodeSelf(root);
                    }
                } else {
                    return root;
                }
            }

            /**
             * 设 root 指向一个 2-Node, 3-Node 或者 4-Node, 再假定它们的儿子都为 null.
             * 该函数删除 root 以及与 root 同层的节点中，其 key 与传入的参数 key *恰恰相等*的那个节点。
             */
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

            /**
             * 设 root 指向一个 2-Node.
             * 该函数根据传入的 key 与 root 所指节点的 key 的相对大小关系来决定是否应该：
             * (1) 删除该 2-Node 节点本身；
             * (2) 在左分支继续寻找要被删除的节点并在找到后删除之；
             * (3) 在右分支继续寻找要被删除的节点并在找到后删除之；
             */
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
                    return deleteNodeSelf(root);
                }
            }

            /** 沿着搜索路径递归地搜索 key 恰巧等于传入的 key 的节点并在找到后将它删除。 */
            static NodePtr doDeleteNodeByKeyRecursive(NodePtr root, const KeyT& key) {
                assert(((is3Node(root) || is4Node(root))));
                if (root->left) {
                    if (is3Node(root)) {
                        if (key == *root->left->key) {
                            root->left = deleteNodeSelf(root->left);
                            updateSize(root);
                        } else if (key == *root->key) {
                            root = rotateRight(root);
                            root->right = deleteNodeSelf(root->right);
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
                            root = deleteNodeSelf(root);
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

            /**
             * 打印一颗树的 Key 和结构
             * 参数说明：
             * NodePtr root: 指向当前节点的指针
             * uint32_t level: 当前节点的 level, 如果 root 指向的恰巧是根节点，那么 level 理应被置为 0, 而每一个当前节点的子节点的 level 都为当前 level+1
             * std::string &displayContent: 存储树的展示内容
             * uint32_t identSize: Level 每增加一级，缩进增加几个空格
             */
            static void treeToRepresentation(
                    NodePtr root,
                    uint32_t level,
                    std::string &displayContent,
                    uint32_t identSize
            ) {
                auto plusIdent = [&displayContent, level, identSize]() -> void {
                    for (uint32_t i = 0; i < level; ++i) {
                        for (uint32_t j = 0; j < identSize; ++j) {
                            displayContent += " ";
                        }
                    }
                };
                plusIdent();

                if (root) {
                    bool hasChildren = false;
                    std::vector<NodePtr> children {};
                    if (is2Node(root)) {
                        displayContent += "NodeType2[";
                        displayContent += *root->key;
                        displayContent += "]";

                        if (root->left || root->right) {
                            hasChildren = true;
                            children.push_back(root->left);
                            children.push_back(root->right);
                        }
                    } else if (is3Node(root)) {
                        displayContent += "NodeType3[";
                        displayContent += *root->left->key;
                        displayContent += ", ";
                        displayContent += *root->key;
                        displayContent += "]";

                        if (root->left->left || root->left->right || root->right) {
                            hasChildren = true;
                            children.push_back(root->left->left);
                            children.push_back(root->left->right);
                            children.push_back(root->right);
                        }
                    } else if (is4Node(root)) {
                        displayContent += "NodeType4[";
                        displayContent += *root->left->key;
                        displayContent += ", ";
                        displayContent += *root->key;
                        displayContent += ", ";
                        displayContent += *root->right->key;
                        displayContent += "]";

                        if (root->left->left || root->left->right || root->right->left || root->right->right) {
                            hasChildren = true;
                            children.push_back(root->left->left);
                            children.push_back(root->left->right);
                            children.push_back(root->right->left);
                            children.push_back(root->right->right);
                        }
                    } else {
                        // No op.
                    }

                    if (hasChildren) {
                        displayContent += "[{";
                        for (size_t idx = 0; idx < children.size(); ++idx) {
                            displayContent += "\n";
                            treeToRepresentation(children[idx], level+1, displayContent, identSize);
                            if (idx != children.size()-1) {
                                displayContent += ",";
                            }
                        }

                        displayContent += "\n";
                        plusIdent();
                        displayContent += "}]";
                    }
                }
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
