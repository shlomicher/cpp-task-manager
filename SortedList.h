#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {
    template <typename T>
    class SortedList {
    private:
        struct Node{
            Node* next;
            Node* previous;
            T data;
        };

        Node* head;
        Node* tail;
        unsigned int size;


        void clear();


    public:
        SortedList();
        SortedList(const SortedList& other);
        SortedList& operator=(const SortedList& other);
        ~SortedList();
        class ConstIterator;
        ConstIterator begin() const;
        ConstIterator end() const;


        void insert(const T& data);
        void remove(const ConstIterator& constIterator);


        int length() const;


        template <typename Predicate>
        SortedList filter(Predicate function) const;

        template <typename Operation>
        SortedList apply(Operation operation) const;

    };

    template <class T>
    class SortedList<T>::ConstIterator {
    private:
        friend class SortedList<T>;

        Node* current;
    public:
        ConstIterator();
        explicit ConstIterator(Node* node);
        ConstIterator(const ConstIterator& other) = default;

        ConstIterator& operator=(const ConstIterator& other) = default;

        ~ConstIterator() = default;

        const T& operator*() const;

        ConstIterator& operator++();

        bool operator!=(const ConstIterator& other) const;


    };
    template <typename T>
    SortedList<T>::SortedList(): head(nullptr),tail(nullptr) ,size(0) {}

    template <typename T>
    SortedList<T>::SortedList(const SortedList& other)
    : head(nullptr), tail(nullptr), size(0) {
        try {
            Node* current = other.head;
            while (current) {
                insert(current->data);
                current = current->next;
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    template <typename T>
    SortedList<T>& SortedList<T>:: operator=(const SortedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        SortedList temp(other);
        Node* copyHead = this->head;
        Node* copyTail = this->tail;
        unsigned int copy_size = this->size;
        this->head = temp.head;
        this->tail = temp.tail;
        this->size = temp.size;

        temp.head = copyHead;
        temp.tail = copyTail;
        temp.size = copy_size;
        return *this;
    }

    template <typename T>
    SortedList<T>::~SortedList() {
        clear();
    }
    template <typename T>
    void SortedList<T>::insert(const T& data) {
        Node* current = tail;
        while (current && data > current->data) {
            current = current->previous;
        }

        Node* newNode = new Node{nullptr, nullptr, data};
        if (!head) {
            head = tail = newNode;
        } else if (!current) {
            newNode->next = head;
            head->previous = newNode;
            head = newNode;
        } else {
            newNode->previous = current;
            newNode->next = current->next;
            if (current->next) {
                current->next->previous = newNode;
            } else {
                tail = newNode;
            }
            current->next = newNode;
        }
        ++size;
    }

    template <typename T>
    void SortedList<T>::clear() {
        Node* current;
        while (this->head) {
            current = this->head;
            this->head = this->head->next;
            delete current;
            this->size--;
        }
        this->tail = nullptr;
        this->size = 0;
    }

    template <typename T>
    void SortedList<T>::remove(const typename SortedList<T>::ConstIterator& const_iterator) {
        Node* node = const_iterator.current;
        if (node == nullptr) {
            return;
        }
        if (node == head && node == tail) {
            head = nullptr;
            tail = nullptr;
        }
        else if (node == head){
            head = node->next;
            head->previous = nullptr;
        }
        else if (node == tail) {
            tail = node->previous;
            tail->next = nullptr;
        }
        else {
            (node->next)->previous = node->previous;
            (node->previous)->next = node->next;
        }
        node->next = nullptr;
        node->previous = nullptr;
        delete node;
        size--;
    }

    template<typename T>
    int SortedList<T>::length() const {
        return this -> size;
    }

    template<typename T>
    template<typename Predicate>
    SortedList<T> SortedList<T>::filter(Predicate function) const{
        SortedList<T> result;
        ConstIterator current = this -> begin();
        while (current != this -> end()) {
            if (function(*current)) {
                result.insert(*current);
            }
            ++current;
        }
        return result;
    }

    template<typename T>
    template<typename Operation>
    SortedList<T> SortedList<T>::apply(Operation operation) const{
        SortedList<T> result;
        ConstIterator current = this -> begin();
        while (current != this -> end()) {
            result.insert(operation(*current));
            ++current;
        }
        return result;
    }


    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const{
        return  ConstIterator(head);
    }

    template <typename T>
    typename SortedList<T>:: ConstIterator SortedList<T>::end() const{
        return ConstIterator(nullptr);
    }


    template <typename T>
    SortedList<T>::ConstIterator::ConstIterator() : current(nullptr) {}

    template <typename T>
    SortedList<T>::ConstIterator::ConstIterator(Node* node) {
        current = node;
    }


    template<typename T>
    const T &SortedList<T>::ConstIterator::operator*() const {
        if (!current) {
            throw std::out_of_range("invalid iterator");
        }
        return current->data;
    }

    template<typename T>
    typename SortedList<T>::ConstIterator &SortedList<T>::ConstIterator::operator++() {
        if (this -> current == nullptr) {
            throw std::out_of_range("invalid iterator");
        }
        this -> current = this -> current -> next;
        return *this;
    }

    template<typename T>
    bool SortedList<T>::ConstIterator::operator!=(const ConstIterator &other) const {
        return current != other.current;
    }
}

