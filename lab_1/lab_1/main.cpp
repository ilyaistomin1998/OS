#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/io.h>
typedef unsigned long long ull;
typedef long long ll;

using namespace std;

int sundaram(int n)
{
    int *a = new int[n], i, j, k;
    memset(a, 0, sizeof(int) * n);
    for (i = 1; 3 * i + 1 < n; i++)
    {
        for (j = 1; (k = i + j + 2 * i*j) < n && j <= i; j++)
            a[k] = 1;
    }
    //Выбирает из списка простых чисел ближайшее к заданному.
    for (i = n - 1; i >= 1; i--)
        if (a[i] == 0)
        {
            return (2 * i + 1);
            break;
        }
    delete[] a;
}

ull gcd(ull a, ull b)
{
    ull c;
    while (b)
    {
        c = a % b;
        a = b;
        b = c;
    }
    return (ull)abs((ll)a);
}

char * catZero(char * str, int n)
{
    if (strlen(str) < n)
    {
        while (strlen(str) != n)
        {
            char buf[20] = "0";
            strcat(buf, str);
            strcpy(str, buf);
        }
    }
    return str;
}

void cryptFile(const char * nameInput, const char * nameOutput, int r, int q, int e)
{
    int inputFile = open(nameInput, O_RDONLY);
    int outputFile = open(nameOutput, O_WRONLY | O_TRUNC | O_CREAT);
    if (!fork())
        execl("/bin/chmod", "/bin/chmod", "755", nameOutput, NULL);
    else
        wait(NULL);
    ull n = r * q;
    //ull f = (r - 1) * (q - 1);
    int b = 301;
    int end = 1;
    while (end)
    {
        char c1;
        end = read(inputFile, (char*)&c1, 1);
        int c = 1;
        int ASCII = static_cast<int>(c1) + b;
        unsigned int i = 0;
        while (i < e)
        {
            c = c * ASCII;
            c %= n;
            i++;
        }
        b++;
        char symbol[20];
        sprintf(symbol, "%d ", c);
        catZero(symbol, 7);
        write(outputFile, (char*)symbol, 7);
    }
    close(inputFile);
    close(outputFile);
}

void uncryptFile(const char * nameInput, const char * nameOutput, int r, int q, int d)
{
    int inputFile = open(nameInput, O_RDONLY);
    int outputFile = open(nameOutput, O_WRONLY | O_CREAT | O_TRUNC);
    if (!fork())
        execl("/bin/chmod", "/bin/chmod", "755", nameOutput, NULL);
    else
        wait(NULL);
    ull n = r * q;
    int m = 1;
    int b = 301;
    int end = 1;

    while (end)
    {
        unsigned int i = 0;
        char str[7], p;
        read(inputFile, (char*)str, 6);
        end = read(inputFile, (char*)&p, 1);
        int symbol = atoi(str);
        m = 1;
        while (i < d)
        {
            m = m * symbol;
            m = m % n;
            i++;
        }
        m = m - b;
        b++;
        write(outputFile, (char*)&m, 1);
    }
    close(inputFile);
    close(outputFile);
}

void initil(int * p, int * q, int * e, int * d)
{
    int a = 0, b = 0;
    char *c = getenv("XDG_GREETER_DATA_DIR");
    for (int i = 0; i < strlen(c); i++)
        a ^= c[i];

    c = getenv("XDG_DATA_DIRS");
    for (int i = 0; i < strlen(c); i++)
        b ^= c[i];
    *p = sundaram(b);
    *q = sundaram(a);

    int n = (*p) * (*q);
    int f = (*p - 1) * (*q - 1);
    int d_simple = 0;
    c = getenv("PATH");
    *d = 0;
    int i = 0;
    while (d_simple != 1)
    {
        *d ^= c[i];
        d_simple = gcd(*d, f);
    }

    int e_simple = 0;
    *e = 0;
    while (e_simple != 1)
    {
        (*e) += 1;
        e_simple = ((*e)*(*d)) % f;
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    if (argc < 2)
    {
        printf("Отсутствуют необходимые параметры для запуска программы.\n\n");
        printf("Чтобы узнать параметры, посмотрите справку (/help)\n");
        return 0;
    }
    else if (argv[1][0] == '/')
    {
        if (!strcmp(argv[1], "/help"))
        {
            printf("%s [<параметр>] <список файлов>\n\n<Параметр>:\n\n", argv[0]);
            printf("/help        Получение справки\n");
            printf("/del         Удаление файлов\n");
            printf("/new         Создание файлов\n");
            printf("/zip         Архивирование файлов\n");
            printf("/unzip       Разархивирование файлов\n");
            printf("/crpt        Шифрование файлов\n");
            printf("/uncrpt      Расшифрование файлов\n");
            printf("Для открытия файлов производится запуск программы без параметра\n");
        }
        else if (!strcmp(argv[1], "/open"))
        {
            for (int i = 2; i < argc; i++)
            {
                int id = open(argv[i], O_RDONLY);
                int end = 1;
                while (end)
                {
                    char c;
                    end = read(id, (char*)&c, 1);
                    printf("%c", c);
                }
                close(id);
            }
        }
        else if (!strcmp(argv[1], "/del"))
        {
            for (int i = 2; i < argc; i++)
            {
                remove(argv[i]);
            }
        }
        else if (!strcmp(argv[1], "/zip"))
        {
            for (int i = 2; i < argc; i++)
            {
                char name[1000];
                int j;
                for (j = 0; argv[i][j] != '.'; j++)
                    name[j] = argv[i][j];
                name[j] = '\0';
                if(!fork())
                    execl("/usr/lib/p7zip/7z", "/usr/lib/p7zip/7z","a", strcat(name, ".zip"), argv[i], NULL);
                else
                    wait(NULL);
            }
        }
        else if (!strcmp(argv[1], "/unzip"))
        {
            for (int i = 2; i < argc; i++)
            {
                if (!fork())
                    execl("/usr/lib/p7zip/7z", "/usr/lib/p7zip/7z", "x", argv[i], NULL);
                else
                    wait(NULL);
            }
        }
        else if (!strcmp(argv[1], "/new"))
        {
            for (int i = 2; i < argc; i++)
            {
                int id = open(argv[i], O_CREAT);
                if (!fork())
                    execl("/bin/chmod", "/bin/chmod", "755", argv[i], NULL);
                else
                    wait(NULL);
                close(id);
            }
        }
        else if (!strcmp(argv[1], "/crpt"))
        {
            int p, q, e, d;
            initil(&p, &q, &e, &d);
            cryptFile(argv[2], argv[3], p, q, e);
        }
        else if (!strcmp(argv[1], "/uncrpt"))
        {
            int p, q, e, d;
            initil(&p, &q, &e, &d);
            uncryptFile(argv[2], argv[3], p, q, d);
        }
        else
        {
            printf("Заданный параметр \"%s\" не найден.\nПодробнее об используемых параметрах см. справку (/help)\n", argv[1]);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
            if (!fork())
                execl("/usr/bin/gedit", "/usr/bin/gedit", argv[i], NULL);
    }
    return 0;
}
