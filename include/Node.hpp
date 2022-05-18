#ifndef FAMILYTREE_NODE_HPP
#define FAMILYTREE_NODE_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <utility>
#include <ostream>
#include <memory>
#include <queue>
#include "Person.hpp"

template <class T>
class Node {
private:
    std::shared_ptr<T> _person;
    Node *_parent = nullptr;
    std::vector<std::shared_ptr<Node<T>>> _children;
public:
    explicit Node () = default;
    // Constructor taking person.
    // Creating person objects with unique ownership in the node.
    explicit Node (const T &t) : _person(std::make_unique<T>(t)) {}
    // Constructor for empty node

    [[nodiscard]] T &getPerson() const {
        return *_person;
    }

    [[nodiscard]] std::shared_ptr<Node> &getParent () const {
      return _parent;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Node<T>>> getChildren () const {
      return _children;
    }

    void setChildren (std::vector<std::shared_ptr<Node<T>>> children){
      _children = children;
    }

    [[nodiscard]] bool isRoot() const {
        return (_parent == nullptr);
    }

    [[nodiscard]] bool isLeaf() const {
        return _children.empty();
     }

    [[nodiscard]] bool isEmpty () const {
      return (_person == nullptr);
    }

    bool hasChild() {
        return _parent != nullptr;
    }

    void traverseUpwards(std::function<void(Node<T> &)> f) {
        f(*this);
        if (hasChild()) {
            _parent->traverseUpwards(f);
        }
    }

    std::vector<std::shared_ptr<Node<T>>> traverseBreadth (std::shared_ptr<Node<T>> node, std::vector<std::shared_ptr<Node<T>>> v) {
      auto children = node->getChildren();

      for (auto c: children) {
        v = traverseBreadth(c, v);
        std::cout << node->getPerson().getFirstName() << " " << node->getPerson().getLastName() << std::endl;
        children = node->getChildren();
        v.push_back(c);
      }
      return v;
    }

    void printBreadth (std::vector<std::shared_ptr<Node<T>>> v) {
      for (int i = v.size(); i > 0; i--){
        auto node = v[i-1];
        std::cout << node->getPerson().getFirstName() << " " << node->getPerson().getLastName() << std::endl;
      }
    }

    void traverseDepth (std::function<void(Node<T> &)> f) {
        f(*this);
        for (auto c: _children) {
            c->traverseDepth(f);
        }
    }

    void traverseDepth (std::function<void(Node<T> &, int)> f, int depth = 0) {
        f(*this, depth);
        for (auto c: _children) {
            c->traverseDepth(f, depth + 1);
        }
    }

    void traverseDepth (std::function<void(Node<T> &)> f, std::string firstName, std::string lastName) {
      if (getPerson().getFirstName() == firstName && getPerson().getLastName() == lastName){
        f(*this);
      }
      else {
        for (auto c: _children) {
          c->traverseDepth(f, firstName, lastName);
        }
      }
    }

    void traverseDepthSearch (std::shared_ptr<Node<T>> root, std::string firstname, std::string lastname, std::function <void(T&)> editingFunc ) {
        root->traverseDepth([firstname, lastname, editingFunc](Node<T> &node) {
            if (node.getPerson().getFirstName() == firstname && node.getPerson().getLastName() == lastname) {
                std::cout << node.getPerson().getFirstName() << " " << node.getPerson().getLastName() << " exists in tree." << std::endl;
                auto &myPerson = node.getPerson();
                editingFunc(myPerson);
            }
        });
    }

    void add(T value) {
      auto n = std::make_shared<Node <T>>(value);
      n->_parent = this;
      _children.emplace_back(n);
    }

    void removePerson (std::shared_ptr<Node<T>> root, std::string firstName, std::string lastName) {

      auto lambda1 = [](Node<Person> &node) {
        if (node.isLeaf()){
          std::shared_ptr<Node<Person>> parentNode = node.getParent();
          std::vector<std::shared_ptr<Node<Person>>> childrenVector = parentNode->getChildren();
          // TODO Remove from parents children vector
        }
      };
      root->traverseDepth (lambda1, firstName, lastName);
      auto lambda2 = [](Person &p){
        p.setFirstName("Empty");
        p.setLastName("Person");
        p.setBirth("00.00.0000");
        p.setDeath("00.00.0000");
        p.setSex("Other");
      };
      root->traverseDepthSearch (root,firstName,lastName, lambda2);
    }
    /*
    void removePerson(std::shared_ptr<Node<T>> root, std::string firstName, std::string lastName) {
      auto lambda = [ *this ](Node<Person> &node) {
        if (node.isLeaf()) {
          std::shared_ptr<Node<Person>> parentNode = node.getParent();
          std::remove(parentNode->_children.begin(), parentNode->_children.end(), this);
        } else {
          Person &p = node.getPerson();
          p.setFirstName("Empty");
          p.setLastName("Person");
          p.setBirth("00.00.0000");
          p.setDeath("00.00.0000");
          p.setSex("0");
        }
      };
      root->traverseDepth(lambda, firstName, lastName);
    }
     */
    template<class E>
    friend std::ostream &operator<<(std::ostream &, const Node<E> &);
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const Node<T> &l) {
    auto &myPerson = l.getPerson();
    os << myPerson.getFirstName();
    os << " ";
    os << myPerson.getLastName();
    return os;
}

#endif//FAMILYTREE_NODE_HPP
