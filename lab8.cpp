#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

// ====== structures ======

struct Record
{
    char surname[30];
    int year;
    char phone[15];
};

// ====== interface functions ======

void createFile();
void displayFileContent();
void createRecord();
void deleteRecord();
void updateRecord();
void displayBySurname();
void displayCountSinceYear();

// ====== support functions ======

void displayMenu();
void bubbleSort(struct Record[30], int);

int main()
{
    setlocale(LC_CTYPE, "Russian");
	fflush(stdin);

    displayMenu();

    return 0;
}


void displayMenu()
{
    int mode;

    puts("Выберите команду:");
    puts("1 - Создать файл");
    puts("2 - Показать содержимое файла");
    puts("3 - Добавить запись в файл");
    puts("4 - Удалить запись из файла");
    puts("5 - Изменить запись из файла");
    puts("6 - Отсортировать по фамилии и отобразить");
    puts("7 - Показать количество телефонов с определенного года");
    scanf("%d", &mode);

    switch (mode)
    {
        case 1:
            createFile();
            displayMenu();
            break;
        case 2:
            displayFileContent();
            displayMenu();
            break;
        case 3:
            createRecord();
            displayMenu();
            break;
        case 4:
            deleteRecord();
            displayMenu();
            break;
        case 5:
            updateRecord();
            displayMenu();
            break;
        case 6:
            displayBySurname();
            displayMenu();
            break;
        case 7:
            displayCountSinceYear();
            displayMenu();
            break;
        default:
            puts("Выходим...");
    }
}


void bubbleSort(struct Record list[30], int counter)
{
    int i, j;
    struct Record buffer;

    for (i=0; i<counter; i++)
    {
        for (j=i; j<counter; j++)
        {
            if (list[i].year>list[j].year)
            {
                buffer = list[i];
                list[i] = list[j];
                list[j] = buffer;
            }
        }
    }
}


void createFile()
{
    FILE *f;
    char *filename;
    const char *constf;

    filename = new char [1024];

    if (filename != NULL)
    {
        puts("Введите имя файла:\n");
        scanf("%s", filename);

        constf = filename;

        remove(constf);
        f = fopen(constf, "wb");

        if (f != NULL)
        {
            fclose(f);
            puts("Новый файл был успешно создан.");
        }
        else
        {
            puts("Недостаточно памяти для создания файла.");
        }

        delete []filename;
    }
    else
    {
        puts("Недостаточно памяти для сохранения имени файла.");
    }
}


void displayFileContent()
{
    FILE *f;
    char *filename;
    const char *fconst;
    int size, counter, i;
    struct Record phonebook[30];

    counter = 0;
    size = sizeof(struct Record);
    filename = new char [1024];

    if (filename != NULL)
    {
        puts("Введите имя файла:\n");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "rb");
        if (f != NULL)
        {
            while (fread(&phonebook[counter], size, 1, f) == 1)
            {
                counter++;
            }
            for (i=0; i<counter; i++)
            {
                printf("Фамилия: %s; Телефон: %s; Год: %d\n", phonebook[i].surname, phonebook[i].phone, phonebook[i].year);
            }
            fclose(f);
        }
        else
        {
            puts("Недостаточно памяти для открытия файла.");
        }
        delete []filename;
    }
    else
    {
        puts("Недостаточно памяти для сохранения имени файла.");
    }
}


void createRecord()
{
    FILE *f;
    char ch;
    char *filename;
    const char *fconst;
    int size, counter, i;
    struct Record new_phone;

    counter = 0;
    size = sizeof(struct Record);
    filename = new char [1024];

    if (filename != NULL)
    {
        puts("Введите имя файла:\n");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "ab");
        if (f != NULL)
        {
            puts("Введите фамилию:");
            scanf("%s", &new_phone.surname);
            puts("Введите телефон:");
            scanf("%s", &new_phone.phone);
            puts("Введите год:");
            scanf("%d", &new_phone.year);
            counter = 0;
            fwrite(&new_phone, size, 1, f);
            fclose(f);
        }
        else
        {
            puts("Недостаточно памяти для открытия файла.");
        }
        delete []filename;
    }
    else
    {
        puts("Недостаточно памяти для сохранения имени файла.");
    }
}


void deleteRecord()
{
    FILE *f;
    char *filename;
    char *surname;
    const char *fconst;
    struct Record phonebook[30];
    int counter, size, id, i, j;

    counter = 0;
    surname = new char [1024];
    filename = new char [1024];
    size = sizeof(struct Record);

    if (filename != NULL && surname != NULL)
    {
        puts("Введите имя файла, запись из которого необходимо удалить:");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "rb");
        if (f != NULL)
        {
            while (fread(&phonebook[counter], size, 1, f) == 1)
            {
                counter++;
            }
            puts("Введите фамилию для удаления:");
            scanf("%s", surname);
            for (i = 0; i < counter; i++)
            {
                if (strcmp(phonebook[i].surname, surname) == 0)
                {
                    break;
                }
            }
            for (j=i; j < counter - 1; j++)
            {
                phonebook[j] = phonebook[j+1];
            }
            freopen(fconst, "wb", f);
            for (j=0; j < counter -1; j++)
            {
                fwrite(&phonebook[j], size, 1, f);
            }
            puts("Запись успешно удалена.");
            fclose(f);
        }
        delete []filename;
        delete []surname;
    }
    else
    {
        puts("Недостаточно памяти для открытия файла.");
    }
}


void updateRecord()
{
    FILE *f;
    char *filename;
    char *surname;
    const char *fconst;
    struct Record phonebook[30];
    int counter, size, i;

    counter = 0;
    surname = new char [1024];
    filename = new char [1024];
    size = sizeof(struct Record);

    if (filename != NULL && surname != NULL)
    {
        puts("Введите имя файла, запись из которого необходимо изменить:");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "rb");
        if (f != NULL)
        {
            while (fread(&phonebook[counter], size, 1, f) == 1)
            {
                counter++;
            }
            puts("Введите фамилию для изменения:");
            scanf("%s", surname);
            for (i = 0; i < counter; i++)
            {
                if (strcmp(phonebook[i].surname, surname) == 0)
                {
                    puts("Введите новый телефон:");
                    scanf("%s", &phonebook[i].phone);
                    break;
                }
            }
            freopen(fconst, "wb", f);
            for (i=0; i < counter; i++)
            {
                fwrite(&phonebook[i], size, 1, f);
            }
            puts("Запись успешно обновлена.");
            fclose(f);
        }
        delete []filename;
        delete []surname;
    }
    else
    {
        puts("Недостаточно имени для сохранения имени файла.");
    }
}


void displayBySurname()
{
    FILE *f;
    char *filename;
    char *surname;
    const char *fconst;
    struct Record phonebook[30];
    struct Record surnames[30];
    int counter, counter_s, size, i;

    counter = 0;
    counter_s = 0;
    surname = new char [1024];
    filename = new char [1024];
    size = sizeof(struct Record);

    if (filename != NULL && surname != NULL)
    {
        puts("Введите имя файла, записи из которого необходимо показать:");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "rb");
        if (f != NULL)
        {
            while (fread(&phonebook[counter], size, 1, f) == 1)
            {
                counter++;
            }
            puts("Введите фамилию для отображения телефонов:");
            scanf("%s", surname);
            for (i = 0; i < counter; i++)
            {
                if (strcmp(phonebook[i].surname, surname) == 0)
                {
                    surnames[counter_s] = phonebook[i];
                    counter_s++;
                }
            }
            fclose(f);
            bubbleSort(surnames, counter_s);
            for (i=0; i < counter_s; i++)
            {
                printf("Фамилия: %s; Телефон: %s; Год: %d\n", surnames[i].surname, surnames[i].phone, surnames[i].year);
            }
        }
        delete []filename;
        delete []surname;
    }
    else
    {
        puts("Недостаточно памяти для открытия памяти.");
    }
}


void displayCountSinceYear()
{
    FILE *f;
    char *filename;
    const char *fconst;
    struct Record buffer;
    int counter, size, year;

    counter = 0;
    filename = new char [1024];
    size = sizeof(struct Record);

    if (filename != NULL)
    {
        puts("Введите имя файла, записи из которого необходимо посчитать:");
        scanf("%s", filename);
        fconst = filename;
        f = fopen(fconst, "rb");
        if (f != NULL)
        {
            puts("Введите год, с которого необходимо отображать записи:");
            scanf("%d", &year);
            while (fread(&buffer, size, 1, f) == 1)
            {
                if (buffer.year >= year)
                {
                    counter++;
                }
            }
            printf("С %d года было записано %d номеров.\n", year, counter);
        }
        delete []filename;
    }
    else
    {
        puts("Недостаточно памяти для сохранения имени файла.");
    }
}
