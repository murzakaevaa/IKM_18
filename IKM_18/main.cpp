// IKM_18.cpp : Чиновники.Есть министерство из n чиновников.Каждый из них может иметь
//только одного непосредственного начальника и произвольное количество
//подчиненных.Однако в министерстве работает только один чиновник, не
//имеющий начальников, – главный чиновник.Для того чтобы
//предприниматель мог получить некоторую лицензию, необходимо получить
//подпись на документе главного чиновника.Каждый чиновник для
//выставления своей подписи требует подпись на документе хотя бы одного из
//своих непосредственных подчиненных и, может быть, некоторую взятку – известное число у.е.
//Необходимо определить, какое минимальное количество денег
//предпринимателю нужно заплатить, чтобы получить лицензию, и указать
//соответствующий этой сумме порядок получения подписей.

#include "ministry.h"
#include <iostream>
#include <limits>

using namespace std;

// Функция  ввода целого числа в заданном диапазоне
int EnteringInteger(const string& prompt, int min, int max)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (!(cin >> value) || value < min || value > max)
        {
            cout << "Ошибка! Введите число от " << min << " до " << max << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            return value;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Ministry ministry;

    try
    {
        cout << "Программа расчёта минимальных взяток в иерархии чиновников\n";

        int n = EnteringInteger("Введите количество чиновников: ", 1, 1000);

        for (int i = 0; i < n; ++i)
        {
            try
            {
                int bribe = EnteringInteger("Введите размер взятки для чиновника [" + to_string(i) + "]: ", 0, 1000000);
                ministry.UniquenessOfficial(i, bribe);
            }
            catch (const exception& e)
            {
                cout << "Ошибка: " << e.what() << "\n";
                i--;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            try
            {
                int subs = EnteringInteger("Введите количество подчинённых у чиновника [" + to_string(i) + "]: ", 0, n - 1);
                for (int j = 0; j < subs; ++j)
                {
                    try
                    {
                        int subId = EnteringInteger("Введите индекс подчинённого №" + to_string(j + 1) +
                            " для чиновника [" + to_string(i) + "]: ", 0, n - 1);
                        ministry.SubordinationRelations(i, subId);
                    }
                    catch (const exception& e)
                    {
                        cout << "Ошибка: " << e.what() << "\n";
                        j--;
                    }
                }
            }
            catch (const exception& e)
            {
                cout << "Ошибка: " << e.what() << "\n";
                i--;
            }
        }

        // Назначение главного чиновника (с ID 0)
        try
        {
            ministry.AppointMain(0);
        }
        catch (const exception& e)
        {
            cout << "Критическая ошибка: " << e.what() << "\n";
            return 1;
        }


        try
        {
            cout << "\nИерархия чиновников:\n";
            ministry.HierarchyOutput();
        }
        catch (const exception& e)
        {
            cout << "Ошибка при выводе иерархии: " << e.what() << "\n";
            return 1;
        }


        try
        {
            auto result = ministry.ResultMain();
            cout << "\nРезультаты расчёта:\n";
            cout << "Минимальная сумма взятки, для получения лицензии: " << result.first << "\n";
            cout << "Оптимальный порядок получения подписей : ";

            // Вывод пути получения подписей
            for (size_t i = 0; i < result.second.size(); ++i)
            {
                if (i != 0) cout << " ";
                cout << result.second[i];
            }
            cout << "\n";
        }
        catch (const exception& e)
        {
            cout << "Ошибка при расчёте минимальной суммы взятки: " << e.what() << "\n";
            return 1;
        }

    }
    catch (const exception& e)
    {
        cerr << " ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}