#include "student.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

const string specStr[SPEC_COUNT] =
{
    "Комп'ютерні науки", "Інформатика", "Математика та економіка",
    "Фізика та інформатика", "Трудове навчання"
};

namespace
{
    // Кількість символів (а не байтів) у рядку UTF-8 – для рівного вирівнювання кирилиці
    int Len(const string& s)
    {
        int n = 0;
        for (unsigned char c : s)
            if ((c & 0xC0) != 0x80)
                n++;
        return n;
    }

    // Рядок, доповнений пробілами праворуч до ширини w
    string PadRight(const string& s, const int w)
    {
        int d = w - Len(s);
        return s + string(d > 0 ? d : 0, ' ');
    }

    // Рядок, доповнений пробілами ліворуч до ширини w
    string PadLeft(const string& s, const int w)
    {
        int d = w - Len(s);
        return string(d > 0 ? d : 0, ' ') + s;
    }

    // Рядок, відцентрований у ширині w
    string Center(const string& s, const int w)
    {
        int d = w - Len(s);
        if (d <= 0) return s;
        return string(d / 2, ' ') + s + string(d - d / 2, ' ');
    }

    // Виведення таблиці; якщо I == nullptr – у порядку зберігання, інакше – за індексним масивом
    void PrintTable(const Student* s, const int* I, const int N, const string& title)
    {
        const int w[7] = { 4, 16, 6, 25, 8, 11, 13 };
        const string head[7] = { "№", "Прізвище", "Курс", "Спеціальність", "Фізика",
                                 "Математика", "Інформатика" };

        int total = 1;
        for (int k = 0; k < 7; k++)
            total += w[k] + 1;

        const string eq(total, '='), dash(total, '-');

        cout << eq << endl;
        cout << "|" << Center(title, total - 2) << "|" << endl;
        cout << eq << endl;

        cout << "|";
        for (int k = 0; k < 7; k++)
            cout << Center(head[k], w[k]) << "|";
        cout << endl << dash << endl;

        for (int i = 0; i < N; i++)
        {
            const Student& st = s[I ? I[i] : i];

            cout << "|" << PadLeft(to_string(i + 1), w[0] - 1) << " "
                << "|" << " " << PadRight(st.prizv, w[1] - 1)
                << "|" << Center(to_string(st.kurs), w[2])
                << "|" << " " << PadRight(specStr[st.spec], w[3] - 1)
                << "|" << Center(to_string(st.fizyka), w[4])
                << "|" << Center(to_string(st.matematyka), w[5])
                << "|" << Center(to_string(st.informatyka), w[6])
                << "|" << endl;
        }
        cout << eq << endl << endl;
    }

    // Порівняння ключа пошуку з елементом у порядку індексного впорядкування:
    // повертає <0, якщо елемент st стоїть ПЕРЕД ключем, 0 – якщо збігається, >0 – якщо ПІСЛЯ ключа
    int CompareWithKey(const Student& st, const string& prizv, const int kurs, const int sum)
    {
        int g = GradeSum(st);
        if (g != sum)             return g < sum ? -1 : 1;
        if (st.prizv != prizv)    return st.prizv < prizv ? -1 : 1;
        if (st.kurs != kurs)      return st.kurs < kurs ? -1 : 1;
        return 0;
    }
}

int ReadInt(const string& prompt, const int lo, const int hi)
{
    while (true)
    {
        cout << prompt;
        string line;
        if (!getline(cin, line))
            return lo;                      // кінець вводу

        istringstream in(line);
        int x; char extra;
        if ((in >> x) && !(in >> extra) && x >= lo && x <= hi)
            return x;

        cout << "     Помилка! Введіть ціле число від " << lo << " до " << hi << "." << endl;
    }
}

double ReadDouble(const string& prompt, const double lo, const double hi)
{
    while (true)
    {
        cout << prompt;
        string line;
        if (!getline(cin, line))
            return lo;                      // кінець вводу

        for (char& c : line)
            if (c == ',') c = '.';          // допускаємо кому як роздільник

        istringstream in(line);
        double x; char extra;
        if ((in >> x) && !(in >> extra) && x >= lo && x <= hi)
            return x;

        cout << "     Помилка! Введіть число від " << lo << " до " << hi << "." << endl;
    }
}

string ReadSurname(const string& prompt)
{
    while (true)
    {
        cout << prompt;
        string line;
        if (!getline(cin, line))
            return "?";                     // кінець вводу

        size_t b = line.find_first_not_of(" \t\r");
        size_t e = line.find_last_not_of(" \t\r");
        if (b != string::npos)
            return line.substr(b, e - b + 1);

        cout << "     Помилка! Прізвище не може бути порожнім." << endl;
    }
}

void Create(Student* s, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;

        s[i].prizv = ReadSurname("     прізвище: ");
        s[i].kurs = ReadInt("     курс (1-6): ", 1, 6);

        cout << "     спеціальність:" << endl;
        for (int k = 0; k < SPEC_COUNT; k++)
            cout << "        " << k << " - " << specStr[k] << endl;
        s[i].spec = (Spec)ReadInt("     вибір (0-4): ", 0, SPEC_COUNT - 1);

        s[i].fizyka = ReadInt("     оцінка з фізики (1-5): ", 1, 5);
        s[i].matematyka = ReadInt("     оцінка з математики (1-5): ", 1, 5);
        s[i].informatyka = ReadInt("     оцінка з інформатики (1-5): ", 1, 5);
        cout << endl;
    }
}

void Print(const Student* s, const int N)
{
    PrintTable(s, nullptr, N, "Список студентів");
}

void PrintIndexSorted(const Student* s, const int* I, const int N)
{
    PrintTable(s, I, N, "Список студентів (індексне впорядкування)");
}

int GradeSum(const Student& st)
{
    return st.fizyka + st.matematyka + st.informatyka;
}

double Average(const Student& st)
{
    return GradeSum(st) / 3.0;
}

bool AverageToSum(const double avg, int& sum)
{
    sum = (int)lround(avg * 3);
    return fabs(sum / 3.0 - avg) < 0.005 + 1e-9;
}

bool BeforePhysical(const Student& a, const Student& b)
{
    if (GradeSum(a) != GradeSum(b)) return GradeSum(a) < GradeSum(b);  // середній бал ↑
    if (a.prizv != b.prizv)         return a.prizv > b.prizv;          // прізвище ↓
    return a.kurs < b.kurs;                                            // курс ↑
}

bool BeforeIndexed(const Student& a, const Student& b)
{
    if (GradeSum(a) != GradeSum(b)) return GradeSum(a) < GradeSum(b);  // середній бал ↑
    if (a.prizv != b.prizv)         return a.prizv < b.prizv;          // прізвище ↑
    return a.kurs < b.kurs;                                            // курс ↑
}

void Sort(Student* s, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)          // метод "бульбашки"
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (BeforePhysical(s[i1 + 1], s[i1]))
            {
                tmp = s[i1];
                s[i1] = s[i1 + 1];
                s[i1 + 1] = tmp;
            }
}

int* IndexSort(const Student* s, const int N)
{
    int* I = new int[N > 0 ? N : 1];    // створили індексний масив

    for (int i = 0; i < N; i++)
        I[i] = i;                       // заповнили його початковими даними

    for (int i = 1; i < N; i++)         // метод вставки для масиву індексів
    {
        int value = I[i];
        int j = i - 1;
        while (j >= 0 && BeforeIndexed(s[value], s[I[j]]))
        {
            I[j + 1] = I[j];
            j--;
        }
        I[j + 1] = value;
    }
    return I;
}

int BinSearch(const Student* s, const int* I, const int N,
    const string& prizv, const int kurs, const int sum)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        int m = (L + R) / 2;
        int c = CompareWithKey(s[I[m]], prizv, kurs, sum);
        if (c == 0)
            return m;
        if (c < 0)
            L = m + 1;      // елемент стоїть перед ключем – шукаємо праворуч
        else
            R = m - 1;      // елемент стоїть після ключа – шукаємо ліворуч
    }
    return -1;
}