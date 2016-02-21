#include <stdio.h>
#include <stdio_ext.h>
#include <locale.h>
#include <string.h>
// #include <windows.h>

//  ===== interface functions =====
void stringToFile();
void stringToConsole();
void processString(void (*)(char*));

// ===== helper functions =====
void encodeString(char*);
void decodeString(char*);
void displayMenu();
void printLine();


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

    printLine();
    printf("Выберите действие: \n");
	printf("1 - Записать в файл\n");
	printf("2 - Показать содержимое файла\n");
	printf("3 - Закодировать строку\n");
	printf("4 - Декодировать строку\n");
    printLine();
	scanf("%d", &mode);

	switch (mode)
	{
		case 1:
			stringToFile();
            displayMenu();
			break;
		case 2:
			stringToConsole();
            displayMenu();
			break;
		case 3:
			processString(encodeString);
            displayMenu();
			break;
		case 4:
			processString(decodeString);
            displayMenu();
			break;
		default:
			printf("Выходим...");
	}
}


void stringToFile()
{
	FILE *f;
	char *string;

	string = new char [1024];
	if (string != NULL)
	{
		puts("Введите строку для сохранения в файл:\n");
        __fpurge(stdin);
		gets(string);
		f = fopen("text.txt", "w");
		if (f != NULL)
		{
			fputs(string, f);
			printf("Строка была сохранена\n");
            printLine();
			fclose(f);
		}
		delete []string;
	}
	else
	{
		puts("Недостаточно памяти\n");
	}
}


void stringToConsole()
{
	FILE *f;
	char *string;

	string = new char [1024];
	if (string != NULL)
	{
		f = fopen("text.txt", "r");

		if (f != NULL)
		{
			fgets(string, 1024, f);
			printf("%s\n", string);
			fclose(f);
		}
		delete []string;
	}
	else
	{
		printf("Недостаточно памяти\n");
	}
}


void processString(void (*func)(char*))
{
    FILE *f;
    char *string;

    string = new char [1024];
    if (string != NULL)
    {
        f = fopen("text.txt", "w");
        if (f != NULL)
        {
            fgets(string, 1024, f);
            func(string);
            fputs(string, f);
            fclose(f);
        }
        delete []string;
    }

}


void encodeString(char *string)
{
    int i;

	for (i=0; i<strlen(string); i++)
	{
		if (*(string+i) != 'я' && *(string+i) != 'z' && *(string+i) != ' ')
		{
			(*(string+i))++;
		}
    }
}


void decodeString(char *string)
{
    int i;

    for (i=0; i<strlen(string); i++)
	{
		if (*(string+i) != 'я' && *(string+i) != 'z' && *(string+i) != ' ')
		{
			(*(string+i))--;
		}
    }
}


void printLine()
{
    printf("------------------------\n");
}
