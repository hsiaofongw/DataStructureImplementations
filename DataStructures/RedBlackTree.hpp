//
// Created by 韦晓枫 on 2022/6/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP

#include <memory>
#include <string>

enum class LinkType {
    RED, BLACK
};

template <typename KeyT, typename ValT>
struct Node {

    Node(std::shared_ptr<KeyT> key, std::shared_ptr<ValT> value)
    : key(key), value(value), left(nullptr), right(nullptr) { }

    std::shared_ptr<KeyT> key;
    std::shared_ptr<ValT> value;
    std::shared_ptr<Node<KeyT, ValT>> left;
    std::shared_ptr<Node<KeyT, ValT>> right;
};

template <typename KeyT, typename ValT>
struct RedBlackNode : public Node<KeyT, ValT> {

    RedBlackNode(const std::shared_ptr<KeyT>& k, const std::shared_ptr<ValT>& v)
    : Node<KeyT, ValT>(k, v), color(LinkType::BLACK), left(nullptr), right(nullptr) { }

    RedBlackNode(const std::shared_ptr<KeyT>& k, const std::shared_ptr<ValT>& v, LinkType color)
    : Node<KeyT, ValT>(k, v), color(color), left(nullptr), right(nullptr) { }

    LinkType color;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> left;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> right;
};

template <typename KeyT, typename ValT>
using RedBlackNodePtr = std::shared_ptr<RedBlackNode<KeyT, ValT>>;

template <typename KeyT, typename ValT>
class RedBlackTreeHandle {
    using RedBlackNode = RedBlackNode<KeyT, ValT>;
    using NodePtr = RedBlackNodePtr<KeyT, ValT>;
    using KeyPtr = std::shared_ptr<KeyT>;
    using ValuePtr = std::shared_ptr<ValT>;

public:

    /** 从左到右旋转，右旋 */
    static NodePtr rotateRight(NodePtr root) {
        assert((root->left));

        NodePtr result { root->left };
        root->left = result->right;
        result->right = root;
        result->color = root->color;
        root->color = LinkType::RED;

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

        return result;
    }

    /** 颜色反转 */
    static void flipColor(NodePtr root) {
        root->color = LinkType::RED;

        if (root->left) {
            root->left->color = LinkType::BLACK;
        }

        if (root->right) {
            root->right->color = LinkType::BLACK;
        }
    }

    /** 插入操作 */
    static NodePtr insert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        NodePtr result = RedBlackTreeHandle::doInsert(root, k, v);
        result->color = LinkType::BLACK;
        return result;
    }

private:

    /** 插入操作 */
    static NodePtr doInsert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        if (!root) {
            return std::make_shared<RedBlackNode>(k, v, LinkType::RED);
        }

        if (k > *root->key) {
            root->right = RedBlackTreeHandle::doInsert(root->right, k, v);
        } else if (k < *root->key) {
            root->left = RedBlackTreeHandle::doInsert(root->left, k, v);
        } else {
            root->value = v;
        }

        if (root->left->color == LinkType::BLACK && root->right->color == LinkType::RED) {
            root = RedBlackTreeHandle::rotateLeft(root);
        }

        if (root->left->left && root->left->left->color == LinkType::RED && root->left->color == LinkType::RED) {
            root = RedBlackTreeHandle::rotateRight(root);
        }

        if (root->left->color == LinkType::RED && root->right->color == LinkType::RED) {
            RedBlackTreeHandle::flipColor(root);
        }

        return root;
    }
};

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
