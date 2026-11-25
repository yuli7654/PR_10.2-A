//   1. Фізичне впорядкування: середній бал ↑, прізвище ↓ (зворотний алфавітний), курс ↑.
//   2. Індексне впорядкування: середній бал ↑, прізвище ↑, курс ↑.
//   3. Бінарний пошук студента за прізвищем, курсом і середнім балом.
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>   // забезпечення відображення кирилиці
#endif

#include "student.h"

using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);        // кодування UTF-8 для вводу
    SetConsoleOutputCP(CP_UTF8);  // кодування UTF-8 для виводу
#endif

    int N = ReadInt("Введіть кількість студентів N (1-1000): ", 1, 1000);

    Student* s = new Student[N]();   // () – початкове обнулення елементів
    bool filled = false;             // чи введено дані

    int menuItem;
    do {
        cout << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - індексне впорядкування та вивід даних" << endl;
        cout << " [5] - бінарний пошук студента" << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        menuItem = ReadInt("Введіть значення: ", 0, 5);
        cout << endl;

        if (menuItem >= 2 && !filled)
        {
            cout << "Спочатку введіть дані (пункт 1)." << endl;
            continue;
        }

        switch (menuItem)
        {
        case 1:
            Create(s, N);
            filled = true;
            break;
        case 2:
            Print(s, N);
            break;
        case 3:
            Sort(s, N);
            cout << "Масив фізично впорядковано." << endl;
            Print(s, N);
            break;
        case 4:
        {
            int* I = IndexSort(s, N);
            PrintIndexSorted(s, I, N);
            delete[] I;
            break;
        }
        case 5:
        {
            cout << "Введіть ключі пошуку:" << endl;
            string prizv = ReadSurname("     прізвище: ");
            int kurs = ReadInt("     курс (1-6): ", 1, 6);
            double avg = ReadDouble("     середній бал (1-5, напр. 4,33): ", 1, 5);
            cout << endl;

            int sum;
            if (!AverageToSum(avg, sum))
            {
                cout << "Студента з таким середнім балом бути не може "
                    "(середній бал = сума трьох оцінок / 3)." << endl;
                break;
            }

            int* I = IndexSort(s, N);        // бінарний пошук потребує впорядкованого масиву
            int pos = BinSearch(s, I, N, prizv, kurs, sum);
            if (pos != -1)
                cout << "Знайдено студента: позиція у впорядкованому списку – " << pos + 1
                << ", номер у масиві – " << I[pos] + 1 << " ("
                << s[I[pos]].prizv << ", курс " << s[I[pos]].kurs
                << ", середній бал " << fixed << setprecision(2) << Average(s[I[pos]])
                << ")" << endl;
            else
                cout << "Шуканого студента не знайдено" << endl;
            delete[] I;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] s;
    return 0;
}
