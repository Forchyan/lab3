//Реализация алгоритмов добавления и обхода в рекурсивной форме.
// Обход осуществляется в LtR - форме. Поиск данных по ключу.
// Метод, реализующий поиск, возвращает булев флаг, принимающий 
// истинное значение в случае, если элемент найден, и ложное в случае, 
// если такой ключ отсутствует.
// Данные, соответствующие ключу, передаются по ссылке.
#include <iostream>
#include <locale.h>

using namespace std;

template <typename K, typename D> class BST {
private:
    struct Node {
        Node* pRight, *pLeft;
        K key;
        D data;
        Node(const K& k, const D& d) {
            pRight = NULL;
            pLeft = NULL;
            key = k;
            data = d;
        }
    };
    Node* pRoot;
    size_t Size;
    
    Node* BSTInsert(Node* pCurrent,const K& k,const D& dat, bool &inserted) {
        if (pCurrent == NULL)
        {
            pCurrent = new Node(k, dat);
            Size++;
            inserted = true;
            return pCurrent;
        }
        else
        {
            if (pCurrent->key == k) // ключ повторился
            {
                inserted = false;
                return pCurrent;
            }
            else
            {
                if (pCurrent->key > k)
                {
                    //go left
                    pCurrent->pLeft = BSTInsert(pCurrent->pLeft, k, dat, inserted);
                }
                else 
                {
                // go right
                pCurrent->pRight = BSTInsert(pCurrent->pRight, k, dat, inserted);
                }
                return pCurrent;
            }
        }
    }

    Node* BSTRootInsert(Node* pRoot, const K& k, const D& dat, bool& inserted) {
        if (pRoot == NULL) // нужный лист найден
        {
            pRoot = new Node(k, dat);
            Size++;
            inserted = true;
            return pRoot;
        }
        else
        {
            if (pRoot->key == k) // ключ существует
            {
                inserted = false;
                return pRoot;
            }
            else
            {
                // спуск до нужного листа
                if (pRoot->key > k)
                {
                    pRoot->pLeft = BSTRootInsert(pRoot->pLeft, k, dat, inserted);
                    if (inserted) return R(pRoot); // элемент был добавлен, нужно вращение вправо
                    else return pRoot;
                }
                else
                {
                    pRoot->pRight = BSTRootInsert(pRoot->pRight, k, dat, inserted);
                    if (inserted) return L(pRoot); // элемент был добавлен, нужно вращение влево
                    else return pRoot;
                }
            }
        }
    }

    bool BSTSearch(const K& k, bool& found, Node* pNode, void(*process)(const K& k, const D& d)) {
        if (pNode == NULL) {
            found = false;
            return found;
        }
        BSTSearch(k, found, pNode->pLeft, process);
        BSTSearch(k, found, pNode->pRight, process);
        if (k == pNode->key)
        {
            found = true;
            process(pNode->key, pNode->data);
            return found;
        }
        return false;
    }

    void ltr(Node* pNode, void (*process)(const K& k, const D& d)) // рекурсивный обход.
        // ltr - сначала обработка левого поддерева, потом узла, потом правого поддерева
    {
        if (!pNode) return;
        ltr(pNode->pLeft, process);
        if (process) // обработка
        {
            process(pNode->key, pNode->data);
        }
        ltr(pNode->pRight, process);
    }
    
    void deleteBST(Node* pNode) {
        if (pNode == NULL) {
            return;
        }
        deleteBST(pNode->pLeft);
        deleteBST(pNode->pRight);
        delete pNode;
    }

    Node* L(Node* t) { // левый поворот
        Node* temp = t->pRight;
        t->pRight = temp->pLeft;
        temp->pLeft = t;
        return temp;
    }

    Node* R(Node* t) { // правый поворот
        Node* temp = t->pLeft;
        t->pLeft = temp->pRight;
        temp->pRight = t;
        return temp;
    }

    Node* BSTDelete_by_key(Node* t, const K& k, bool& deleted) {
        if (t == NULL) {
            deleted = false;
            return t;
        }

        if (t->key > k) {
            t->pLeft = BSTDelete_by_key(t->pLeft, k, deleted);
            return t;
        }
        else if (t->key < k) {
            t->pRight = BSTDelete_by_key(t->pRight, k, deleted);
            return t;
        }
        else {
            deleted = true;
            Size--;
            if (t->pLeft == NULL && t->pRight == NULL) {
                delete t;
                return NULL;
            }
            else if (t->pLeft == NULL) {
                Node* temp = t->pRight;
                delete t;
                return temp;
            }
            else if (t->pRight == NULL) {
                Node* temp = t->pLeft;
                delete t;
                return temp;
            }
            else {
                t->pRight = BSTDel(t->pRight, t);
                return t;
            }
        }
    }

    // функция поиска элемента замещения
    Node* BSTDel(Node* pSubRoot, Node* t) {
        //t - указатель на элемент дерева
        //pSubRoot - указатель на правое поддерево элемента t

        // Спуск пока не будет найден элемент без левого потомка
        if (pSubRoot->pLeft != NULL) {
            pSubRoot->pLeft = BSTDel(pSubRoot->pLeft, t);
            return pSubRoot;
        }
        // Элемент найден, перенос ключа и данных
        t->key = pSubRoot->key;
        t->data = pSubRoot->data;
        // Временное хранение первого потомка замещающего элемента
        Node* temp = pSubRoot->pRight;
        delete pSubRoot;
        return temp;
        
    }

    Node* Copy(Node* pNode) {
        if (pNode == NULL) {
            return NULL;
        }
        Node* pTemp = new Node(pNode->key, pNode->data);
        pTemp->pLeft = Copy(pNode->pLeft);
        pTemp->pRight = Copy(pNode->pRight);
        return pTemp;
    }

public:
    BST() { // конструктор дерева
        pRoot = NULL;
        Size = 0;
    }
    ~BST() {
        deleteBST(pRoot);
    }

    BST(const K& k, const D& dat) { // конструктор с пар. дерева
        pRoot = new Node(k, dat);
        Size = 1;
    }

    BST(const BST& other_BST) { // 
        pRoot = Copy(other_BST.pRoot);
        Size = other_BST.Size;
    }

    BST& operator=(const BST& other_BST) { //конструктор присваивания
        if (this == &other_BST) {
            return *this;
        }
        // Удаляем существующее дерево
        deleteBST(pRoot);
        // Копируем новое дерево
        pRoot = Copy(other_BST.pRoot);
        Size = other_BST.Size;
        return *this;
    }

    bool Insert(const K& k, const D& dat)// функция обертка для вставки. Нужна потому что при вставке нужен корень,
        //а давать к нему доступ снаружи нельзя, по этому вызываем отсюда приватную функцию вставки, в которую
        // переадим корень
    {
        bool inserted;
        pRoot = BSTInsert(pRoot, k, dat, inserted); // тут указатель на корень нам доступен (мы внутри класса)
        return inserted;
    }

    bool RootInsert(const K& k, const D& dat)// функция обертка для вставки в корень. НЕ ЗНАЮ РАБОТАЕТ ВЕРНО ИЛИ НЕТ ПОКА
    {
        bool inserted;
        pRoot = BSTRootInsert(pRoot, k, dat, inserted); // 
        return inserted;
    }

    void LTR(void (*process)(const K& k, const D& d) = NULL)
    {
        ltr(pRoot, process);
    }

    bool Delete(const K& k) { // обёртка для удаления по ключу
        bool deleted;
        pRoot = BSTDelete_by_key(pRoot, k, deleted);
        return deleted;
    }

    void DeleteTree() {
        deleteBST(pRoot);
        pRoot = NULL;
        Size = 0;
    }

    int GetSize() {
        return Size;
    }

    bool SearchByKey(const K& k, void(*process)(const K& k, const D& d) = NULL) {
        bool flag;
        BSTSearch(k, flag, pRoot, process);
        return flag;
    }
};

template <typename K, typename D>

void BSTProcess(const K& k, const D& d) // шаблонная функция для обхода. Она просто выводит ключ и данные в консоль
{
    std::cout << "Key: " << k << " " << " Data: " << d << endl;
}

void menu() {
    BST<int, int> bst;
    int data, key, exit = 0;
    char symbol1;
    for (int i = 1,j = 1; i <= 100; i++,j+=5) {
        bst.Insert(i, j);
    }

    while (exit != 1) {
        system("cls");
        cout << "------MENU------\n";
        cout << "Добавление элемента в дерево: 0" << endl << "Поиск данных по ключу: 1" << endl << "Удалить узел с заданным ключом: 2" << endl << "Очистить дерево: 3" << endl << "Вывод дерева(ltr-обход): 4" << endl << "Вставка в корень: 5" << endl << "Выход из меню: z" << endl << " >> ";
        cin >> symbol1;
        switch (symbol1) {

            // Добавление элемента в дерево
        case '0':
            system("cls");
            cout << "Введите ключ и данные: ";
            cin >> key >> data;
            bst.Insert(key,data);
            break;

            // Поиск данных по ключу
        case '1':
            system("cls");
            if (bst.GetSize() != 0) {
                cout << "Введите ключ, по которому получить данные: ";
                cin >> key;
                bool flag = bst.SearchByKey(key, BSTProcess<int>);
                if (!flag) cout << "Такого ключа нет!" << endl;
            }
            else {
                cout << "Дерево пустое!" << endl;
            }
            system("pause > null");
            break;

            // Удалить узел с заданным ключом
        case '2':
            system("cls");
            if (bst.GetSize() != 0) {
                cout << "Введите ключ узла, который хотите удалить: ";
                cin >> key;
                cout << endl;
                if (bst.Delete(key) == true) {
                    cout << "Узел удалён из дерева!" << endl;
                }
                else 
                    cout << "Узел не найден!" << endl;
            }
            else cout << "Дерево пустое!" << endl;
            system("pause > null");
            break;
            // Очистить дерево
        case '3':
            system("cls");
            bst.DeleteTree();
            cout << "Дерево очищено!" << endl;
            system("pause > null");
            break;

            // Вывод дерева
        case '4':
            system("cls");
            if (bst.GetSize() != 0) {
                bst.LTR(BSTProcess<int>);
            }
            else cout << "Дерево пустое!";
            system("pause > null");
            break;


            // Вставка в корень
        case '5':
            system("cls");
            cout << "Введите ключ и данные: ";
            cin >> key >> data;
            bst.RootInsert(key, data);
            system("pause > null");
            break;

            // выход из меню
        case 'z':
            system("cls");
            cout << "Выход...";
            exit = 1;
            system("pause > null");
            break;

        default:
            system("cls");
            cout << "Вы промазали по клавишам, попробуйте еще раз! " << endl;
            system("pause > null");
            break;

        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    menu();
    return 0;
}

