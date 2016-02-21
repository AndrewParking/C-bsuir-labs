#include <stdio.h> // библиотека ввода-вывода
#include <stdlib.h> // стандартная библиотека встоенных функций, нужна для malloc
#include <time.h> // библиотека для генерации случайных значений

// эти константы добавлены исключительно для улучшения читаемости кода
#define TRUE 1 // константа, обозначающая булеву истину
#define FALSE 0 // константа, обозначающая булеву ложь
#define SUCCESS 1 // сигнал успешного выполнения функции
#define FAIL 0 // сигнал неудачного выполнения функции


struct Node { // структура элемента списка
    int value; // значение элементы списка
    struct Node *next; // указатель на следующий элемент в списке
};


struct Node* traverse_list(struct Node*); // функция получения последнего элемента в списке
double list_average(struct Node*); // функция нахождение среднего значения элементов списка
struct Node* create_list(void); // функция создания списка посредством создания первого элемента списка
void view_list(struct Node*); // функция вывода списка в консоль
void update_list(struct Node*); // функция добавления элемента в список
struct Node* create_random_list(void); // функция создания случайного списка
void count_less_than_average(struct Node*); // функция подсчета элементов со значением меньше среднего
void drop_less_than_average(struct Node*); // функция удаления элементов со значением меньше среднего
void free_memory(struct Node*); // функция очистки памяти


int main()
{
    char list_exists = FALSE, list_is_random = FALSE; // создание и инициализация флагов присутствия списка и его случайности
    int choice; // переменная для выбора функции программы для выполнения
    struct Node* list; // указатель на первый элемент списка

    srand(time(NULL)); // установить в качестве базы для генерации случайного числа текущее время

    do {
        /*
        Вывод меню для выбора функции на выполнение
        */
        puts("Выберите команду для выполнения:");
        puts("1 - Создать пустой список");
        puts("2 - Просмотреть список");
        puts("3 - Добавить элемент в список");
        puts("4 - Создать список случайных элементов");
        puts("5 - Посчитать количество элементов со значением меньше среднего");
        puts("6 - Удалить элементы со значением меньше среднего");

        scanf("%d", &choice); // ввод пользователем желаемой номера функции

        switch (choice) // выполнение команд исходя из пользовательского ввода
        {
            case 1:
                list = create_list(); // создать список
                if (list != NULL) // если список был создан
                {
                    list_exists = TRUE; // обновление флага присутствия списка
                    list_is_random = FALSE; // установка флага случайности списка в false
                    puts("Список создан"); // уведомить пользователья о создании списка
                }
                else
                {
                    list_exists = FALSE; // иначе установить флаг присутсвия списка в false
                    puts("В процессе создания списка возникла ошибка"); // уведомить пользователя
                }
                break;
            case 2:
                if (list_exists) // если список создан
                {
                    view_list(list); // вызов функции вывода списка в консоль
                }
                else
                {
                    puts("Список не был создан. Нечего показывать"); // иначе сообщить пользователю, что нечего показывать
                }
                break;
            case 3:
                if (list_exists) // если список создан
                {
                    update_list(list); // добавить элемент в список
                }
                else
                {
                    puts("Список не был создан. Некуда добавлять"); // иначе сообщить пользователю, что некуда добавлять
                }
                break;
            case 4:
                list = create_random_list(); // создать случайный список
                if (list != NULL) // если список создан
                {
                    list_exists = TRUE; // обновить флаг существования списка
                    list_is_random = TRUE; // обновить флаг случайности списка
                    puts("Список случайных элементов создан"); // уведомить пользователя
                }
                else
                {
                    list_exists = FALSE; // иначе установить флаг существования списка false
                    list_is_random = FALSE; // установить флаг случайности false
                    puts("В процессе создания произошла ошибка"); // уведомить пользователя
                }
                break;
            case 5:
                if (list_exists && list_is_random) // если существует случайный список
                {
                    count_less_than_average(list); // посчитать количество элементов, значения которых меньше среднего
                }
                else
                {
                    puts("Список либо не создан, либо не состоит из случайных значений"); // уведомить пользователя
                }
                break;
            case 6:
                if (list_exists && list_is_random) // если существует случайный список
                {
                    drop_less_than_average(list); // удалить элементы, значения которых меньше среднего
                }
                else
                {
                    puts("Список либо не создан, либо не состоит из случайных значений"); // уведомить пользователя
                }
                break;
            default:
                free_memory(list); // очистить память, выделенную под список
                return 0; // выйти из программы со статус-кодом 0 (успешное завершение)
        }
    /*
    данный цикл выполняется до тех пор, пока пользователем на вопрос о выборе
    команды на выполнение вводится число, поддерживаемое оператором switch, описанным
    выше. при несовпадении значений выполяется блок default, который освобождает
    выделенную под список память и завершает программу со статусом 0 (выполнена успешно)
    */
    } while (TRUE);
}


struct Node* traverse_list(struct Node *list_beginning)
{
    struct Node *list_ending; // указатель на конец списка
    list_ending = list_beginning; // проинициализировать указатель на конец списка адресом первого элемента списка

    while (list_ending->next != NULL) // пока существует следующий элемент
    {
        list_ending = list_ending->next; // перемещать указатель на следующий элемент
    }

    return list_ending; // вернуть из функции адрес конца списка
}


double list_average(struct Node *list)
{
    double sum = 0, count = 0; // переменные суммы значений элементов и их количества проинициализированы нулями
    struct Node *pointer; // указатель на элемент списка

    pointer = list; // указатель проинициализирован адресом первого элемента списка

    while (pointer != NULL) // пока элемент существует
    {
        sum += pointer->value; // добавить его значение к общей сумме
        pointer = pointer->next; // передвинуть указатель на следующий элемент
        count++; // увеличить счетчик количества элементов на единицу
    }

    return sum/count; // рассчитать среднее значение списка

}


struct Node* create_list()
{
    struct Node* new_list; // указать на новый список
    int initial_value; // значение первого элемента списка

    puts("Проинициализируйте первый элемент списка:");
    scanf("%d", &initial_value); // ввод пользователем значения первого элемента списка

    new_list = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти под первый элемент нового списка
    if (new_list == NULL) // если память не выделена
    {
        puts("Не удалось выделить память."); // уведомить об этом пользователя
        return NULL; // вернуть NULL
    }
    new_list->value = initial_value; // присваивание значения первому элементу списка
    new_list->next = NULL; // инициализация указателя на следующий элемент списка значением NULL

    puts("Создан новый список c первым элементом со значением по умолчанию."); // уведомить подльзователя о создании списка

    return new_list; // вернуть указатель на первый элемент созданного списка
}


void update_list(struct Node *list)
{
    struct Node *new_list_node, *last_list_node; // объявление указателя на новый элемент списка и на последний
    int new_value; // объявление переменной для значения нового элемента списка

    new_list_node = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти под новый элемент списка
    if (new_list_node == NULL) // если не удалось выделить память
    {
        puts("Недостаточно памяти для создания нового элемента в списке."); // оповестить об этом пользователя
        return FAIL; // вернуть сигнал неудачного завершения работы функции
    }

    puts("Введите значение нового элемента списка:");
    scanf("%d", &new_value); // ввод пользователем значения нового элемента списка
    new_list_node->value = new_value; // инициализация значения нового элемента списка
    new_list_node->next = NULL; // инициализация указателя на следующий элемент списка значением NULL

    last_list_node = traverse_list(list); // получение последнего элемента списка
    last_list_node->next = new_list_node; // установление указателя на следующий элемент последнего элемента списка
    // в значение адреса нового элемента

    return SUCCESS // возвращение сигнала об успешном завершении работы функции
}


void view_list(struct Node *list)
{
    struct Node* current_node; // указатель на элемент списка
    current_node = list; // инициализация указателя адресом первого значения списка

    while (current_node != NULL) // пока элемент существует
    {
        printf("| %d ", current_node->value); // вывести его с отделяющей чертой без символа новой строки
        current_node = current_node->next; // перевести указатель на следующий элемент
    }
    printf("|\n"); // напечатать закрывающую список черту и символ конца строки
}


struct Node* create_random_list()
{
    int length; // длина списка
    struct Node *random_list; // указатель на начало списка случайных чисел
    struct Node *current_element; // указатель на элемент списка случайных чисел
    struct Node *tmp; // вспомогательный указатель на элемент списка случайных чисел

    do {
        puts("Введите длину списка случайных элементов:");
        scanf("%d", &length); // ввод пользователем значения длины случайного списка
    }
    while (length <= 0); // пока длина меньше или равна нулю

    current_element = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти под первый элемент списка
    if (current_element == NULL) // если память выделить не удалось
    {
        puts("Не удалось выделить память под элемент."); // известить об этом пользователя
        return NULL; // вернуть из функции значение NULL
    }
    current_element->value = rand()/10000000; // генерация случайного значения элемента списка
    current_element->next = NULL; // инициализация указателя на следующий элемент списка значением NULL
    random_list = current_element; // сохранение указателя на первый элемент случайного списка в random_list

    while (length-1 > 0) // пока созданы не все элементы списка
    {
        tmp = (struct Node*) malloc(sizeof(struct Node)); // выделение памяти под новый элемент случайного списка
        if (tmp == NULL)
        {
            put("Не удалось выделить память под элемент."); // известить об этом пользователя
            return NULL; // вернуть из функции значение NULL
        }
        tmp->value = rand()/10000000; // генерация случайного значения элемента списка
        tmp->next = NULL; // инициализация указателя на следующий элемент списка значением NULL
        current_element->next = tmp; // присвоение указателю на следующий элемент последнего элемента
        // списка адреса нового элемента
        current_element = tmp; // присвоение указателя текущего последнего элемента случайного списка
        length--; // уменьшение счетчика длины на единицу
    }

    return random_list; // возврат из функции указателя на первый элемент случайного списка
}


void count_less_than_average(struct Node *list)
{
    int counter = 0; // переменная счетчик
    double average; // среднее арифметическое элементов списка
    struct Node *pointer; // указатель на элемент списка

    pointer = list; // инициализация указателя на элемент списка адресом первого элемента списка

    average = list_average(list); // нахождение и инициализация среднего значения элементов списка

    while (pointer != NULL) // пока элемент существует
    {
        if (pointer->value < average) // если значение элемента меньше среднего значения элементов списка
        {
            counter++; // увеличить счетчик элементов на единицу
        }
        pointer = pointer->next; // перевести указатель на следующий элемент списка
    }

    printf("Меньше среднего значения %d элементов списка\n", counter); // вывести результат работы функции
}


void drop_less_than_average(struct Node *list)
{
    double average; // среднее значение элементов списка
    struct Node *current_element, *previous_element; // указатели на текущий элемент и предыдущий

    average = list_average(list); // инициализация среднего значения элементов списка
    previous_element = NULL; // инициализация указателя на предыдущий элемент списка значением NULL, так как текущий - первый
    current_element = list; // инициализация указателя на текущий элемент списка адресом первого элемента списка

    while (current_element != NULL) // пока элемент списка существует
    {
        if (current_element->value < average) // если значение элемента меньше среднего
        {
            if (previous_element != NULL) // если предыдущий элемент существует, т.е. текущий - не первый
            {
                previous_element->next = current_element->next; // присвоить указателю на следующий элемент предудущего
                // элемента адрес следующего элемента текущего элемента
                free(current_element); // очистить память, выделенную под текущий элемент
                current_element = previous_element->next; // передвинуть указатель на следующий элемент
            }
            else
            {
                list = current_element->next; // иначе, присвоить указателю на первый элемент адрес
                // следующего за первым элемента
                free(current_element); // освободить память, выделенную под первый элемент
                current_element = list; // передвинуть указатель на новый первый элемент
            }
        }
        else
        {
            previous_element = current_element; // передвинуть указатель на предыдущий элемент
            current_element = current_element->next; // передвинуть указатель на текущий элемент
        }
    }

    puts("Элементы удалены."); // известить пользователя о выполнении функции
}


void free_memory(struct Node *node)
{
    if (node != NULL) // если элемент списка существует
    {
        free_memory(node->next); // рекурсивно вызвать функцию освобождения памяти для следующего элемента
        free(node); // освободить память, выделенную под элемент списка
    }
}
