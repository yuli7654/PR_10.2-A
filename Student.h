#pragma once
#include <string>

// Спеціальність студента
enum Spec
{
    KOMP_NAUKY,          // Комп'ютерні науки
    INFORMATYKA,         // Інформатика
    MATEM_EKONOMIKA,     // Математика та економіка
    FIZYKA_INFORMATYKA,  // Фізика та інформатика
    TRUDOVE_NAVCHANNYA   // Трудове навчання
};

const int SPEC_COUNT = 5;

extern const std::string specStr[SPEC_COUNT];   // назви спеціальностей

struct Student
{
    std::string prizv;       // прізвище
    int         kurs;        // курс
    Spec        spec;        // спеціальність
    int         fizyka;      // оцінка з фізики
    int         matematyka;  // оцінка з математики
    int         informatyka; // оцінка з інформатики
};

// ---------- введення з контролем помилок (кожна функція зчитує рядок повністю) ----------

// Введення цілого числа з діапазону [lo; hi]; при помилці запит повторюється.
// При завершенні потоку вводу повертає lo.
int ReadInt(const std::string& prompt, const int lo, const int hi);

// Введення дійсного числа з діапазону [lo; hi] (крапка або кома як роздільник)
double ReadDouble(const std::string& prompt, const double lo, const double hi);

// Введення непорожнього прізвища
std::string ReadSurname(const std::string& prompt);

// ---------- формування та вивід ----------

// Введення даних про N студентів з клавіатури
void Create(Student* s, const int N);

// Виведення масиву у вигляді таблиці (у порядку зберігання)
void Print(const Student* s, const int N);

// Виведення масиву у вигляді таблиці в порядку, визначеному індексним масивом I
void PrintIndexSorted(const Student* s, const int* I, const int N);

// ---------- обчислення ----------

// Повертає суму трьох оцінок студента (цілі числа – порівняння без похибок)
int GradeSum(const Student& st);

// Повертає середній бал студента
double Average(const Student& st);

// Перетворює середній бал avg у відповідну суму трьох оцінок sum (sum = avg*3);
// повертає false, якщо жоден студент не може мати такий середній бал
// (avg має збігатися з sum/3 з точністю до 0,005 – двох знаків після коми)
bool AverageToSum(const double avg, int& sum);

// ---------- впорядкування та пошук (варіант 20) ----------

// Порівняння для ФІЗИЧНОГО впорядкування: повертає true, якщо a має стояти строго перед b:
// середній бал – за зростанням, прізвище – за спаданням, курс – за зростанням
bool BeforePhysical(const Student& a, const Student& b);

// Порівняння для ІНДЕКСНОГО впорядкування: повертає true, якщо a має стояти строго перед b:
// середній бал – за зростанням, прізвище – за зростанням, курс – за зростанням
bool BeforeIndexed(const Student& a, const Student& b);

// Фізичне впорядкування масиву (метод «бульбашки», стійкий)
void Sort(Student* s, const int N);

// Індексне впорядкування: повертає новий динамічний масив індексів із N елементів
// (звільняється викликом delete[]); сам масив s не змінюється
int* IndexSort(const Student* s, const int N);

// Бінарний пошук в індексно впорядкованому масиві за ключами (середній бал як сума оцінок, прізвище, курс).
// Повертає позицію знайденого студента у впорядкованому списку (тобто s[I[позиція]]) або -1
int BinSearch(const Student* s, const int* I, const int N,
    const std::string& prizv, const int kurs, const int sum);