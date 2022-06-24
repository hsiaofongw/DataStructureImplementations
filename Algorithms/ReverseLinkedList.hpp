//
// Created by 韦晓枫 on 2022/6/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REVERSELINKEDLIST_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REVERSELINKEDLIST_HPP

#include <iostream>
#include <memory>
#include <vector>

namespace ReverseLinkedList {

    /** 链表节点 struct 声明 */
    template <typename T>
    struct ListNode;

    /** 链表节点 struct 定义 */
    template <typename T>
    struct ListNode {
        ListNode( ) : value(nullptr), next(nullptr) { }
        explicit ListNode(std::shared_ptr<T> _value) : value(_value), next(nullptr) {}
        std::shared_ptr<T> value;
        std::shared_ptr<ListNode> next;
    };

    /** 链表节点指针类型定义 */
    template <typename T>
    using NodePtr = std::shared_ptr<ListNode<T>>;

    /** 反转链表 */
    template <typename T>
    NodePtr<T> reverseLinkedList(NodePtr<T> head) {
        NodePtr<T> prev = nullptr;
        NodePtr<T> curr = head;

        while (curr) {
            NodePtr<T> next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        return prev;
    }

    /** 插入元素到链表前 */
    template <typename T>
    NodePtr<T> insert(NodePtr<T> root, std::shared_ptr<T> value) {
        NodePtr<T> newRoot = std::make_shared<ListNode<T>>(value);
        newRoot->next = root;
        return newRoot;
    }

    void test() {
        auto values = std::vector<uint64_t> { 3, 2, 6, 1, 7, 4, 8, 0, 9, 5 };
        auto root = NodePtr<uint64_t> {};
        std::cout << "Insert: ";
        for (const auto& value : values) {
            auto valuePtr = std::make_shared<uint64_t>(value);
            std::cout << (*valuePtr) << " ";
            root = insert(root, valuePtr);
        }
        std::cout << "\n";

        auto print = [](NodePtr<uint64_t> root) -> void {

            NodePtr<uint64_t> head = root;
            while (head) {
                std::cout << (*head->value) << " ";
                head = head->next;
            }
            std::cout << "\n";
        };

        std::cout << "Print: ";
        print(root);

        std::cout << "Reverse: ";
        root = reverseLinkedList(root);
        print(root);
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_REVERSELINKEDLIST_HPP
