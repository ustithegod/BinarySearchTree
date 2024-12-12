#include <iostream>
#include <set>
#include <vector>

class Tree {
public:
    struct Node {
        int key;
        bool isVirtual;

        Node *leftSon, *rightSib, *parent;

        Node(int p_key, bool isVirtual = false) : key(p_key), isVirtual(isVirtual),
                                                  leftSon(nullptr), rightSib(nullptr), parent(nullptr) {}
    };

private:
    Node *root;
    Node *L = nullptr;

    void rec_push(Node *node, int key) {
        if (node->key > key) {
            if (!node->leftSon) {
                node->leftSon = new Node(key);
                node->leftSon->parent = node;
                return;
            }

            else if (node->leftSon->isVirtual) {
                node->leftSon->key = key;
                node->leftSon->isVirtual = false;
                return;
            }

            else {
                rec_push(node->leftSon, key);
            }
        }

        else if (node->key < key) {
            if (!node->leftSon) {
                node->leftSon = new Node(-1, true);
                node->leftSon->rightSib = new Node(key);
                node->leftSon->parent = node;
                node->leftSon->rightSib->parent = node;
                return;
            }

            else if (!node->leftSon->rightSib) {
                node->leftSon->rightSib = new Node(key);
                node->leftSon->rightSib->parent = node;
                return;
            }

            else {
                rec_push(node->leftSon->rightSib, key);
            }
        }

        else {
            std::cerr << "cannot push non-unique node (" << key << ") in tree\n";
            return;
        }
    }

public:
    Tree() : root(nullptr) {}

    Node *ROOT() const {
        return root;
    }

    Node *PARENT(Node *node) const {
        return node->parent ? node->parent : L;
    }

    Node *LEFT_CHILD(Node *node) const {
        return node->leftSon ? node->leftSon : L;
    }

    Node *RIGHT_SIBLING(Node *node) const {
        return node->rightSib ? node->rightSib : L;
    }

    int KEY(Node *node) const {
        return node->key;
    }

    void push(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }

        rec_push(root, key);
    };

    void reversePostOrder(Node *node) {
        if (!node)
            return;

        reversePostOrder(node->leftSon);

        reversePostOrder(node->rightSib);

        if (!node->isVirtual) {
            push(node->key);
        }
    }

    void printTree(Node *node = nullptr, int depth = 0) const {
        if (!node) {
            node = root; // Начинаем с корня
            if (!root) {
                std::cout << "The tree is empty.\n";
                return;
            }
        }

        std::cout << std::string(depth, '\t') << node->key;
        if (node->isVirtual) {
            std::cout << "(v)";
        }
        std::cout << "\n";

        if (node->leftSon) {
            printTree(node->leftSon, depth + 1);
        }

        if (node->rightSib) {
            printTree(node->rightSib, depth);
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Tree treeA;
    Tree treeB;

    std::vector<int> numbersA;
    std::set<int> uniqueNumbers;

    while (numbersA.size() < 15) {
        int randomNum = rand() % 99 + 1;
        if (uniqueNumbers.find(randomNum) == uniqueNumbers.end()) {
            treeA.push(randomNum);
            numbersA.push_back(randomNum);
            uniqueNumbers.insert(randomNum);
        }
    }

    int countB = 0;
    while (countB < 15) {
        int randomNum = rand() % 99 + 1;
        if (uniqueNumbers.find(randomNum) == uniqueNumbers.end()) {
            treeB.push(randomNum);
            uniqueNumbers.insert(randomNum);
            countB++;
        }
    }

    std::cout << "Tree A structure:\n\n";
    treeA.printTree();

    std::cout << "\nTree B structure:\n\n";
    treeB.printTree();

    treeA.reversePostOrder(treeB.ROOT());

    std::cout << "Tree 'A = AUrev.B' structure:\n\n";
    treeA.printTree();
}