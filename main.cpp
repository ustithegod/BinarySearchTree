#include <iostream>
#include <set>
#include <stack>
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

    void clear(Node *node) {
        if (node) {
            clear(node->leftSon);  // Освобождаем левое поддерево
            clear(node->rightSib); // Освобождаем правое поддерево
            delete node;           // Освобождаем текущий узел
        }
    }

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

    void MAKENULL() {
        clear(root);    // Освобождаем память
        root = nullptr; // Устанавливаем корень в nullptr
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

    class ReverseIterator {
    private:
        std::stack<Node *> stack;

        void pushLeft(Node *node) {
            while (node) {
                stack.push(node);
                node = node->leftSon; // Идём по левым дочерним узлам
            }
        }

    public:
        ReverseIterator(Node *root) {
            pushLeft(root);
        }

        bool hasNext() {
            return !stack.empty();
        }

        Node *next() {
            if (stack.empty()) {
                return nullptr;
            }

            Node *current = stack.top();
            stack.pop();

            // Сначала добавляем правого потомка
            pushLeft(current->rightSib);

            return current;
        }

        // Операторы для совместимости с range-for
        bool operator!=(const ReverseIterator &other) const {
            return !stack.empty() || !other.stack.empty();
        }

        Node *operator*() const {
            return stack.top();
        }

        ReverseIterator &operator++() {
            next();
            return *this;
        }
    };

    ReverseIterator begin() {
        return ReverseIterator(root);
    }

    ReverseIterator end() {
        return ReverseIterator(nullptr);
    }

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

    static Tree CREATE(int n, const std::vector<Tree> &subtrees) {
        Tree newTree;
        newTree.root = new Node(n);

        for (const auto &subtree : subtrees) {
            if (subtree.ROOT()) {
                Node *child = subtree.ROOT();
                child->parent = newTree.root; // Устанавливаем родительское отношение
                if (!newTree.root->leftSon) {
                    newTree.root->leftSon = child; // Первый дочерний узел
                } else {
                    Node *sibling = newTree.root->leftSon;
                    while (sibling->rightSib) {
                        sibling = sibling->rightSib; // Находим последнего дочернего узла
                    }
                    sibling->rightSib = child; // Добавляем как следующего брата
                }
            }
        }

        return newTree;
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

    std::cout << "Дерево А:\n\n";
    treeA.printTree();

    std::cout << "\nДерево B:\n\n";
    treeB.printTree();

    treeA.reversePostOrder(treeB.ROOT());

    std::cout << "Дерево 'A = AUrev.B':\n\n";
    treeA.printTree();

    std::cout << "\n\nОбратный обход дерева А:\n";
    for (Tree::Node *node : treeA) { // Используем range-for
        if (node && !node->isVirtual) {
            std::cout << node->key << " ";
        }
    }
}