#pragma once
#include <memory>

// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {

    template<typename T>
    class LinkedList {
    private:
        struct node {
            std::weak_ptr<node> up;
            std::shared_ptr<node> down;
            T data;
        };

        std::shared_ptr<node> front;
        std::weak_ptr<node> back;
    public:
        LinkedList<T>() {}
        
        class Iterator {
        private:
            friend LinkedList<T>;
            std::weak_ptr<node> node_reference;
            Iterator(std::weak_ptr<node> node) : node_reference(node) {}
        public:
            auto operator*() const->T;
            auto operator++()->Iterator&;
            auto operator==(Iterator const& rhs)->bool;
            auto operator!=(Iterator const& rhs)->bool;
        };

        auto begin() const->Iterator;
        auto end() const->Iterator;
        auto isEmpty() const -> bool;
        auto getFront() const->T;
        auto getBack() const->T;
        auto enqueue(T element) -> void;
        auto dequeue()->void;
        auto pop() ->void;
        auto clear()->void;
        auto contains(T element) const -> bool;
        auto remove(T element) -> void;
    };


    template<typename T>
    auto LinkedList<T>::begin() const->Iterator {
        return Iterator(front);
    }

    template<typename T>
    auto LinkedList<T>::end() const->Iterator {
        return Iterator(std::weak_ptr<node>());
    }

    template<typename T>
    auto LinkedList<T>::isEmpty() const -> bool {
        return !static_cast<bool>(front);
    }

    template<typename T>
    auto LinkedList<T>::getFront() const->T {
        return front->data;
    }

    template<typename T>
    auto LinkedList<T>::getBack() const->T {
        return back.lock()->data;
    }

    template<typename T>
    auto LinkedList<T>::enqueue(T element) -> void {
        auto n = std::make_shared<node>(node{
            back,
            std::shared_ptr<node>(),
            element
            });
        if (!front) {
            front = n;
            back = n;
            return;
        }
        back.lock()->down = n;
        back = back.lock()->down;
    }

    template<typename T>
    auto LinkedList<T>::dequeue()->void {
        if (!front) { return; }
        front = front->down;
    }

    template<typename T>
    auto LinkedList<T>::pop() ->void {
        if (back.expired()) { return; }
        if (back.lock()->up.expired()) {
            front.reset();
            return;
        }
        back = back.lock()->up;
        back.lock()->down.reset();
    }

    template<typename T>
    auto LinkedList<T>::clear()->void {
        front.reset();
    }

    template<typename T>
    auto LinkedList<T>::contains(T element) const -> bool {
        for (const auto& n : *this) {
            if (n == element) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    auto LinkedList<T>::remove(T element) -> void {
        for (std::weak_ptr<node> t = front; !t.expired(); t = t.lock()->down) {
            if (t.lock()->data == element) {
                if (t.lock()->down) {
                    t.lock()->down->up = t.lock()->up;
                }
                if (t.lock()->up.expired()) {
                    if (t.lock()->down) {
                        front = t.lock()->down;
                    }
                    else {
                        front.reset();
                    }
                }
                else {
                    t.lock()->up.lock()->down = t.lock()->down;
                }
                return;
            }
        }
    }

    template<typename T>
    auto LinkedList<T>::Iterator::operator*() const->T {
        return node_reference.lock()->data;
    }

    template<typename T>
    auto LinkedList<T>::Iterator::operator++()->Iterator& {
        node_reference = node_reference.lock()->down;
        return *this;
    }


    template<typename T>
    auto LinkedList<T>::Iterator::operator==(Iterator const& rhs)->bool {
        return operator!=(rhs);
    }

    template<typename T>
    auto LinkedList<T>::Iterator::operator!=(Iterator const& rhs)->bool {
        if (node_reference.expired() && rhs.node_reference.expired()) {
            return false;
        }
        if (node_reference.expired()) {
            return true;
        }
        if (rhs.node_reference.expired()) {
            return true;
        }
        return node_reference.lock() != rhs.node_reference.lock();
    }
    
} }  // namespace ufl_cap4053::fundamentals


