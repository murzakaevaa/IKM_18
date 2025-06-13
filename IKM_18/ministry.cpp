#include "ministry.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// Конструктор с проверкой на отрицательную взятку
Official::Official(int id, int bribe) : id(id), bribe(bribe)
{
    if (bribe < 0) throw invalid_argument("Взятка не может быть отрицательной");
}

// Добавление подчиненного с проверкой на null
void Official::AddingSubordinate(Official* subordinate)
{
    if (!subordinate) throw invalid_argument("Подчинённый не может быть nullptr");
    subordinates.push_back(subordinate);
}

// Геттеры
int Official::getId() const { return id; }
int Official::getBribe() const { return bribe; }
const vector<Official*>& Official::getSubordinates() const { return subordinates; }

// Проверка, является ли чиновник конечным в иерархии
bool Official::FinalOfficial() const { return subordinates.empty(); }

// Рекурсивная проверка на циклы в поддереве
bool Official::CheckingCycles(Official* parent) const
{
    if (this == parent) return true;  // Обнаружен цикл
    for (auto sub : subordinates) {
        if (sub->CheckingCycles(parent)) return true;
    }
    return false;
}

// Рекурсивный вывод иерархии 
void Official::OutputHierarchy(int level) const
{
    for (int i = 0; i < level; ++i) cout << "    ";
    cout << "|--- [" << id << "] (взятка: " << bribe << ")\n";


    for (auto sub : subordinates)
        sub->OutputHierarchy(level + 1);
}



// Конструктор
Ministry::Ministry() : head(nullptr) {}

// Деструктор с очисткой памяти
Ministry::~Ministry() { Clear(); }

// Добавление чиновника с проверкой на уникальность ID
void Ministry::UniquenessOfficial(int id, int bribe)
{
    for (auto official : officials)
    {
        if (official->getId() == id)
        {
            throw invalid_argument("Чиновник с ID " + to_string(id) + " уже существует");
        }
    }
    officials.push_back(new Official(id, bribe));
}

// Установка отношения подчинения с проверкой на циклы
void Ministry::SubordinationRelations(int bossId, int subId)
{
    Official* boss = nullptr, * sub = nullptr;

    // Поиск начальника и подчинённого
    for (auto official : officials)
    {
        if (official->getId() == bossId) boss = official;
        if (official->getId() == subId) sub = official;
    }

    // Проверка существования чиновников
    if (!boss || !sub)
    {
        throw invalid_argument("Неверный ID начальника (" + to_string(bossId) +
            ") или подчинённого (" + to_string(subId) + ")");
    }

    // Проверка на создание цикла
    if (sub->CheckingCycles(boss))
    {
        throw logic_error("Обнаружен цикл в иерархии при добавлении подчинённого");
    }

    boss->AddingSubordinate(sub);
}

// Назначение главного чиновника
void Ministry::AppointMain(int headId)
{
    for (auto official : officials)
    {
        if (official->getId() == headId)
        {
            head = official;
            return;
        }
    }
    throw invalid_argument("Главный чиновник с ID " + to_string(headId) + " не найден");
}

// Проверка всей иерархии на циклы
bool Ministry::HierarchyCycles() const
{
    if (!head) return false;
    return head->CheckingCycles(nullptr);
}

// Вывод иерархии с проверкой на наличие циклов
void Ministry::HierarchyOutput() const
{
    if (!head) throw logic_error("Иерархия не создана - главный чиновник не назначен");
    if (HierarchyCycles()) throw logic_error("Обнаружен цикл в иерархии");
    head->OutputHierarchy();
}

// Рекурсивный расчёт минимальной взятки и оптимального пути
pair<int, vector<int>> Ministry::CalculationBride(Official* official)
{
    if (!official) throw invalid_argument("Чиновник не может быть nullptr");

    // Базовый случай: если чиновник - без подчинённых, берём его взятку
    if (official->FinalOfficial())
    {
        return { official->getBribe(), {official->getId()} };
    }

    int minTotal = numeric_limits<int>::max();
    vector<int> bestPath;

    // Перебор всех подчинённых для поиска оптимального пути
    for (Official* sub : official->getSubordinates())
    {
        auto result = CalculationBride(sub);
        int total = result.first + official->getBribe();

        // Выбор пути с минимальной суммарной взяткой
        if (total < minTotal)
        {
            minTotal = total;
            bestPath = result.second;
        }
    }

    // Добавление текущего чиновника в путь
    bestPath.push_back(official->getId());
    return { minTotal, bestPath };
}

// Получение результата для главного чиновника
pair<int, vector<int>> Ministry::ResultMain()
{
    if (!head) throw logic_error("Невозможно расчитать - главный чиновник не назначен");
    if (HierarchyCycles()) throw logic_error("Невозможно расчитать - обнаружен цикл в иерархии");
    return CalculationBride(head);
}

// Очистка всех данных с освобождением памяти
void Ministry::Clear()
{
    for (auto official : officials)
        delete official;
    officials.clear();
    head = nullptr;
}