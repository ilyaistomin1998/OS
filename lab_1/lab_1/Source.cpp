#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <process.h>
#include <io.h>
#include <ctime>

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
	//Âûáèðàåò èç ñïèñêà ïðîñòûõ ÷èñåë áëèæàéøåå ê çàäàííîìó.
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

void cryptFile(const char * nameInput, const char * nameOutput, int r, int q, int e, int d)
{
	int inputFile = _open(nameInput, O_RDONLY | _O_CREAT, _S_IREAD);
	int outputFile = _open(nameOutput, O_WRONLY | _O_TRUNC | _O_CREAT, _S_IWRITE);
	ull n = r * q;
	ull f = (r - 1) * (q - 1);
	int b = 301;
	while (!_eof(inputFile))
	{
		char c1;
		_read(inputFile, (char*)&c1, 1);
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
		strcat(_itoa(c, symbol, 10), " ");
		catZero(symbol, 7);
		_write(outputFile, (char*)symbol, 7);
	}
	_close(inputFile);
	_close(outputFile);
}

void uncryptFile(const char * nameInput, const char * nameOutput, int r, int q, int e, int d)
{
	int inputFile = _open(nameInput, _O_RDONLY | _O_CREAT, _S_IREAD);
	int outputFile = _open(nameOutput, _O_WRONLY | _O_CREAT | _O_TRUNC, _S_IWRITE);
	ull n = r * q;
	ull f = (r - 1) * (q - 1);
	int m = 1;
	int b = 301;
	
	while (!_eof(inputFile))
	{
		unsigned int i = 0;
		char str[7], p;
		_read(inputFile, (char*)str, 6);
		_read(inputFile, (char*)&p, 1);
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
		_write(outputFile, (char*)&m, 1);
	}
	_close(inputFile);
	_close(outputFile);
}

void initil(int * p, int * q, int * e, int * d)
{
	int a = 0, b = 0;
	char *c = getenv("TEMP");
	for (int i = 0; i < strlen(c); i++)
		a ^= c[i];

	c = getenv("PATHEXT");
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
		printf("Îòñóòñòâóþò íåîáõîäèìûå ïàðàìåòðû äëÿ çàïóñêà ïðîãðàììû.\n\n");
		printf("×òîáû óçíàòü ïàðàìåòðû, ïîñìîòðèòå ñïðàâêó (/help)\n");
		return 0;
	}
	else if (argv[1][0] == '/')
	{
		if (!strcmp(argv[1], "/help"))
		{
			printf("%s [<ïàðàìåòð>] <ñïèñîê ôàéëîâ>\n\n<Ïàðàìåòð>:\n\n", argv[0]);
			printf("/help        Ïîëó÷åíèå ñïðàâêè\n");
			printf("/del         Óäàëåíèå ôàéëîâ\n");
			printf("/new         Ñîçäàíèå ôàéëîâ\n");
			printf("/zip         Àðõèâèðîâàíèå ôàéëîâ\n");
			printf("/unzip       Ðàçàðõèâèðîâàíèå ôàéëîâ\n");
			printf("/crpt        Øèôðîâàíèå ôàéëîâ\n");
			printf("/uncrpt      Ðàñøèôðîâàíèå ôàéëîâ\n");
			printf("Äëÿ îòêðûòèÿ ôàéëîâ ïðîèçâîäèòñÿ çàïóñê ïðîãðàììû áåç ïàðàìåòðà\n");
		}
		else if (!strcmp(argv[1], "/open"))
		{
			for (int i = 2; i < argc; i++)
			{
				int id = _open(argv[i], _O_RDONLY | _O_TEXT, _S_IREAD);
				while (!_eof(id))
				{
					char c;
					_read(id, (char*)&c, 1);
					printf("%c", c);
				}
				_close(id);
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
				_spawnl(_P_WAIT, "7z.exe", "7z.exe", "a", strcat(name, ".zip"), argv[i], NULL);
			}
		}
		else if (!strcmp(argv[1], "/unzip"))
		{
			for (int i = 2; i < argc; i++)
			{
				char name[1000];
				int j;
				for (j = 0; argv[i][j] != '.'; j++)
					name[j] = argv[i][j];
				name[j] = '\0';
				_spawnl(_P_WAIT, "7z.exe", "7z.exe", "x", /*strcat(name, ".zip"), */argv[i], NULL);
			}
		}
		else if (!strcmp(argv[1], "/new"))
		{
			for (int i = 2; i < argc; i++)
			{
				int id = _open(argv[i], O_CREAT, _S_IWRITE);
				_close(id);
			}
		}
		else if (!strcmp(argv[1], "/crpt"))
		{
			int p, q, e, d;
			initil(&p, &q, &e, &d);
			cryptFile(argv[2], argv[3], p, q, e, d);
		}
		else if (!strcmp(argv[1], "/uncrpt"))
		{
			int p, q, e, d;
			initil(&p, &q, &e, &d);
			uncryptFile(argv[2], argv[3], p, q, e, d);
		}
		else
		{
			printf("Çàäàííûé ïàðàìåòð \"%s\" íå íàéäåí.\nÏîäðîáíåå îá èñïîëüçóåìûõ ïàðàìåòðàõ ñì. ñïðàâêó (/help)\n", argv[1]);
		}
	}
	else
	{
		for (int i = 1; i < argc; i++)
			_spawnl(_P_NOWAIT, "C:\\Windows\\notepad.exe", "C:\\Windows\\notepad.exe", argv[i], NULL);
	}
	return 0;
}
