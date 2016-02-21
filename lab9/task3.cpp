#include <stdio.h> // библиотека ввода-вывода
#include <stdlib.h> // стандартная библиотека встоенных функций, нужна для malloc
#include <locale.h> // библиотека для работы с текстом, отличным от символов ascii
#include <stdio_ext.h> // расширение стандартной библиотеки, нужно для __fpurge

// четыре константы ниже добавлены исключительно для улучшения читаемости кода
#define TRUE 1 // константа, обозначающая булеву истину
#define FALSE 0 // константа, обозначающая булеву ложь
#define SUCCESS 1 // сигнал успешного выполнения функции
#define FAIL 0 // сигнал неудачного выполнения функции
#define MAXLINE 100 // макимальная длина строки данных узла дерева


struct Node { // структура для узла дерева
    int key; // ключ узла дерева
    char *data; // указатель на строку данных узла дерева
    struct Node *parent; // указатель на узел родителя
    struct Node *left_child; // указатель на левое поддерево
    struct Node *right_child; // указатель на правое поддерево
};


void branch_counter(struct Node*, int*); // рекурсивная функция подсчета узлов дерева
void tree_viewer(struct Node*, int); // рекурсивная функция вывода на экран узлов дерева в зависимости от глубины

struct Node* create_tree(void); // функция создания дерева посредством создания корневого узла
short int add_new_node(struct Node*); // функция создания
struct Node* search_by_key(struct Node*, int, short int); // функция поиска значения в дереве по ключу
short int remove_by_key(struct Node*); // функция удаления узла по ключу
void print_tree(struct Node*); // функция вывода дерева в консоль. использует tree_viewer
void count_right_branch_nodes(struct Node*); // функция подсчета узлов правого поддерева. использует branch_counter
void free_memory(struct Node*); // функция очистки памяти, выделенной под дерево


int main()
{
    setlocale(LC_CTYPE, "Russian"); // включение поддержки символов русского языка

    char tree_exists = FALSE; // индикатор наличия созданного дерева
    int choice, key; // переменная для выбора функции программы для выполнения, переменная для ключа поиска
    struct Node *tree; // указатель на корневой узел дерева

    do {
        /*
        Вывод меню для выбора функции на выполнение
        */
        puts("Выберите команду для исполнения:");
        puts("1 - создать дерево");
        puts("2 - добавить узел в дерево");
        puts("3 - найти данные по ключу");
        puts("4 - удалить узел по ключу");
        puts("5 - вывести дерево на экран");
        puts("6 - посчитать количество узлов в правой ветви дерева");

        scanf("%d", &choice); // ввод пользователем желаемой номера функции

        switch(choice) // выполнение команд исходя из пользовательского ввода
        {
            case 1:
                tree = create_tree(); // вызов функции создания дерева
                if (tree != NULL) // если дерево создано без ошибок
                {
                    tree_exists = TRUE; // присвоение индикатору созданного дерева значения истины
                    puts("Дерево создано");
                }
                else
                {
                    tree_exists = FALSE; // иначе, проверить, чтобы индикатор содержал ложное значение
                    puts("В процессе создания дерева возникла ошибка");
                }
                break;
            case 2:
                if (tree_exists) // если дерево создано
                {
                    if (add_new_node(tree))
                    /*
                    вызов функции добавления узла и проверка на истиность
                    возвращаемого функцией статуса выполнения
                    */
                    {
                        puts("Узел добавлен");
                    }
                    else
                    {
                        puts("Во время добавления узла произошла ошибка");
                    }
                }
                else // иначе - вывод информирующего сообщения об отсутствии дерева
                {
                    puts("Дерево не создано. Некуда добавлять узел");
                }
                break;
            case 3:
                if (tree_exists) // если дерево создано
                {
                    puts("Введите ключ для поиска:");
                    scanf("%d", &key); // ввод ключа для поиска
                    search_by_key(tree, key, TRUE); // вызов функции поиска по ключу
                }
                else
                {
                    puts("Дерево не создано. Негде искать узел");
                }
                break;
            case 4:
                if (tree_exists) // если дерево создано
                {
                    remove_by_key(tree); // вызов процедуры удаления узла по ключу
                }
                else
                {
                    puts("Дерево не создано. Неоткуда удалять элемент");
                }
                break;
            case 5:
                if (tree_exists) // если дерево создано
                {
                    print_tree(tree); // вызов процедуры вывода дерева в консоль
                }
                else
                {
                    puts("Дерево не создано. Нечего выводить на экран");
                }
                break;
            case 6:
                if (tree_exists) // если дерево создано
                {
                    count_right_branch_nodes(tree); // вызов процедуры подсчета узлов правого поддерева
                }
                else
                {
                    puts("Дерево не создано. Нечего выводить на экран");
                }
                break;
            default:
                if (tree_exists) // если дерево создано
                {
                    free_memory(tree); // вызов процедуры очистки памяти
                }
                return 0; // выйти из программы со статус-кодом 0 (успешное завершение)
        }
    /*
    данный цикл выполняется до тех пор, пока пользователем на вопрос о выборе
    команды на выполнение вводится число, поддерживаемое оператором switch, описанным
    выше. при несовпадении значений выполяется блок default, который освобождает
    выделенную под дерево память и завершает программу со статусом 0 (выполнена успешно)
    */
    } while(TRUE);
}


void branch_counter(struct Node *tree_node, int *counter)
{
    /*
    функция использует передачу целочисленной переменной счетчика по указателю.
    это позволяет всем рекурсивным вызовам функции увеличивать одну и ту же переменную,
    опираясь на ее адрес, а не на передаваемое значение.
    */
    if (tree_node->left_child != NULL) // если у узла есть левое поддерево
    {
        (*counter)++; // увеличить счетчик на единицу
        branch_counter(tree_node->left_child, counter); // рекурсивно вызвать функцию для левого поддерева
    }
    if (tree_node->right_child != NULL) // если у узла есть правое поддерево
    {
        (*counter)++; // увеличить счетчик на единицу
        branch_counter(tree_node->right_child, counter); // рекурсивно вызвать функцию для правого поддерева
    }
}


void tree_viewer(struct Node *tree_node, int depth)
{
    int i; // переменная-счетчик для цикла

    if (tree_node != NULL) // если узел существует
    {
        tree_viewer(tree_node->left_child, depth+1); // вызвать функцию рекурсивно для левого поддерева
        for (i = 0; i < depth; i++)
        {
            printf("---"); // визуально обозначить глубину, на которой расположен узел
        }
        printf("%d -> %s\n", tree_node->key, tree_node->data); // вывести ключ и значение самого узла
        tree_viewer(tree_node->right_child, depth+1); // вызвать функцию рекурсивно для правого поддерева
    }
}


struct Node* create_tree()
{
    int key; // переменная для ключа корня дерева
    char *data; // переменная для строки данных корня дерева
    struct Node *tree; // указатель на структуру корневого узла дерева

    __fpurge(stdin); // очистка стандартного потока ввода в UNIX-системах

    data = (char*) malloc(MAXLINE); // выделение памяти под строку данных структуры
    if (data == NULL) // если не удалось выделить память
    {
        return data; // вернуть из функции NULL
    }

    puts("Введите ключ корня дерева:");
    scanf("%d", &key); // ввод позьзователем ключа корня
    puts("Введите данные корня дерева:");
    scanf("%s", data); // ввод пользователем данных корня


    tree = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти под корневой узел
    if (tree == NULL) // если память выделить не удалось
    {
        free(data); // очистить память, выделенную под строку данных
        return tree; // вернуть из функции NULL
    }

    tree->key = key; // присвоение корню ключа
    tree->data = data; // присвоение корню данных
    tree->parent = NULL; // присвоение корню указателя на родительский узел, то бишь на NULL, так как у корня оного нет
    tree->left_child = NULL; // установка левого поддерева в NULL, так как его еще нет
    tree->right_child = NULL; // установка правого поддерева в NULL, так как его еще нет

    return tree; // возврат указателя на структуру корня из функции
}


struct Node* search_by_key(struct Node *tree, int key, short int verbose)
{
    struct Node *current_node; // объявление указателя на узел для перебора узлов
    struct *desired_node; // объявление указателя на узел, предшествующий ему

    current_node = tree; // присвоение указателю на узел адреса корневого узла

    while (desired_node->key != key && current_node != NULL)
    // пока узел с ключом не найден и возможно дальнейшее продвижение по узлам дерева
    {
        desired_node = current_node; // сохраняем адрес на узел до перехода к следующему узлу
        if (current_node->key > key) // если ключ узла больше искомого ключа
        {
            current_node = current_node->left_child; // переходим к его левому поддереву
        }
        else
        {
            current_node = current_node->right_child; // иначе переходим к его правому поддереву
        }
    }

    if (desired_node->key == key)
    // так как выход из предыдущего цикла возможен и по причине достижения
    // листа дерева, необходима проверка на то, найден ли был искомый узел
    {
        if (verbose) // если необходимо выводить результаты работы функции в консоль
        {
            printf("Узел найден: ключ -> %d, данные -> %s\n", desired_node->key, desired_node->data); // вывести данные узла
        }
        return desired_node; // вернуть указатель на узел
    }
    else
    {
        if (verbose) // если необходимо выводить результаты работы функции в консоль
        {
            printf("Узел с ключом %d не найден\n", key); // иначе сообщить о том, что узел не найден
        }
        return NULL; // вернуть из функции NULL
    }
}


short int add_new_node(struct Node *tree)
{
    int key; // ключ нового узла
    char *data; // строка данных нового узла
    struct Node *p_node, *new_node, *parent; // указатели на место в дереве для узла, сам узел и его родителя

    __fpurge(stdin); // очистка стандартного потока ввода в UNIX-системах

    data = (char*) malloc(MAXLINE); // выделение памяти под строку данных нового узла
    if (data == NULL) // если память выделить не удалось
    {
        return FAIL; // вернуть сигнал о неудачном завершении функции
    }

    puts("Введите ключ для нового элемента:");
    scanf("%d", &key); // ввод пользователем ключа нового узла
    puts("Введите данные для нового элемента:");
    scanf("%s", data); // ввод пользователем строки данных нового узла

    if (search_by_key(tree, key, FALSE) != NULL) // если такой узел найден в дереве
    /*
    для поиска используется функция search_by_key с флагом verbose равным FALSE
    это значит, что ход и результаты её выполнения не логгируются в консоль
    */
    {
        puts("Узел с таким ключом уже присутствует в дереве"); // сообщить пользователю о его существовании
        return FAIL; // вернуть сигнал о неудачном завершении функции
    }

    new_node = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти для нового узла
    if (new_node == NULL) // если память выделить не удалось
    {
        return FAIL; // вернуть из функции сигнал о неудачном завершении ее работы
    }
    new_node->key = key; // присвоение ключа новому узлу
    new_node->data = data; // присвоение строки данных новому узлу

    p_node = tree; // установление указателя на место для нового узла в адрес корня

    while (p_node != NULL) // пока узел существует
    {
        parent = p_node; // сохранить ссылку на узел до ее изменения ниже
        if (p_node->key > key) // если ключ узла больше ключа нового элемента
        {
            p_node = p_node->left_child; // перейти к его левому поддереву
        }
        else
        {
            p_node = p_node->right_child; // иначе, перейти к его правому поддереву
        }
    }

    if (parent->key > key) // если ключ родителя больше ключа нового узла
    /*
    фактически, предыдущий цикл дает нам лишь адрес потенциального родителя нового узла.
    необходима дополнительная проверка для установления того, должен он быть правым или левым листом
    */
    {
        parent->left_child = new_node; // новый узел становится левым листом родителя
    }
    else
    {
        parent->right_child = new_node; // иначе, новый узел становится правым листом родителя
    }
    new_node->parent = parent; // сохранение ссылки на родителя в структуре нового узла
    new_node->left_child = NULL; // инициализация левого поддерева в NULL, так как его пока нет
    new_node->right_child = NULL; // инициализация правого поддерева в NULL, так как его пока нет

    return SUCCESS; // вернуть сигнал успешного выполнения функции
}


short int remove_by_key(struct Node *tree)
{
    int key; // ключ узла, который необходимо удалить
    struct Node *node_to_delete, *successor; // указатели на удаляемый узел и тот, что его заменит

    puts("Введите ключ для удаления:");
    scanf("%d", &key); // ввод пользователем ключа узла, который необходимо удалить

    node_to_delete = search_by_key(tree, key, FALSE); // поиск по ключу узла, который необходимо удалить
    if (node_to_delete != NULL) // если узла не существует
    {
        printf("Узла с ключом %d не существует\n"); // уведомить об этом пользователя
        return FAIL; // вернуть из функции сигнал о неудачном ее завершении
    }

    if (node_to_delete->left_child == NULL && node_to_delete->right_child == NULL) // если удаляемый узел - лист
    {
        if (node_to_delete->parent->left_child == node_to_delete) // если удаляемый узел - левое поддерево родителя
        {
            node_to_delete->parent->left_child = NULL; // обнулить указатель родителя на левое поддерево
        }
        else
        {
            node_to_delete->parent->right_child = NULL; // иначе обнулить указатель родителя на правое поддерево
        }
    }
    else if (node_to_delete->left_child == NULL && node_to_delete->right_child != NULL)
    // если у удаляемого элемента есть только правое поддерево
    {
        if (node_to_delete->parent->left_child == node_to_delete) // если удаляемый узел - левое поддерево родителя
        {
            // правое поддерево удаляемого узла займет место левого поддерева его родителя после удаления
            node_to_delete->parent->left_child = node_to_delete->right_child;
        }
        else
        {
            // правое поддерево удаляемого узла займет место правого поддерева его родителя после удаления
            node_to_delete->parent->right_child = node_to_delete->right_child;
        }
        // обновить ссылку на родителя в наследнике удаляемого узла
        node_to_delete->right_child->parent = node_to_delete->parent;
    }
    else if (node_to_delete->left_child != NULL && node_to_delete->right_child == NULL)
    // если у удаляемого элемента есть только левое поддерево
    {
        if (node_to_delete->parent->left_child == node_to_delete) // если удаляемый узел - левое поддерево родителя
        {
            // левое поддерево удаляемого узла займет место левого поддерева его родителя после удаления
            node_to_delete->parent->left_child = node_to_delete->left_child;
        }
        else
        {
            // левое поддерево удаляемого узла займет место правого поддерева его родителя после удаления
            node_to_delete->parent->right_child = node_to_delete->left_child;
        }
        // обновить ссылку на родителя в наследнике удаляемого узла
        node_to_delete->left_child->parent = node_to_delete->parent;
    }
    else
    {
        /*
        Если у удаляемого узла есть сразу два поддерева, наследником станет корень самого левого  правого поддереваы
        */
        // инициализировать наследника указателем на правое поддерево узля для удаления
        successor = node_to_delete->right_child;
        while (successor->left_child != NULL) // пока не найден самый левый узел поддерева
        {
            successor = successor->left_child; // переводим указатель на левое поддерева текущего узла
        }
        if (successor->right_child != NULL) // если у узла-наследника есть правое поддерево
        /*
        проверка на наличие левого поддерева не производится, так как условием выхода из
        предыдущего цикла было то, что наследник является самым левым узлом поддерева
        */
        {
            successor->right_child->parent = successor->parent; // его родителем становится родитель узла-наследника
            successor->parent->left_child = successor->right_child; // оно становится левым поддеревом родителя узла-наследника
        }
        else
        {
            successor->parent->left_child = NULL; // иначе указатель на левое поддерево родителя узла-наследника обнуляется
        }
        successor->parent = node_to_delete->parent; // родитель удаляемого элемента становится родителем наследника
        successor->left_child = node_to_delete->left_child;
    }
    free(node_to_delete->data); // очистить память, выделенную под строку данных узла
    free(node_to_delete); // очистить память, выделенную под структуру узла

    return SUCCESS; // вернуть сигнал об успешном завершении функции
}


void count_right_branch_nodes(struct Node *tree)
{
    int counter = 0; // счетчик узлов

    if (tree->right_child != NULL) // если у корня есть правое поддерево
    {
        counter++; // посчитать его
        branch_counter(tree->right_child, &counter); // начать рекурсивный подсчет узлов правого поддерева
    }

    printf("Количество узлов правой ветви дерева составило %d\n", counter); // вывод информации для пользователя
}


void print_tree(struct Node *tree)
{
    tree_viewer(tree, 1); // вызов рекурсивной функции для вывода дерева в консоль
}


void free_memory(struct Node *tree)
{
    if (tree != NULL) // если узел существует
    {
        free_memory(tree->left_child); // вызвать функцию освобождения памяти для левого поддерева
        free_memory(tree->right_child); // вызвать функцию освобождения памяти для правого поддерева
        free(tree->data); // освободить память, выделенную под строку данных узла
        free(tree); // освободить память под узел
    }
}