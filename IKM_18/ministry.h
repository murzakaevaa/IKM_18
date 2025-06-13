#include <vector>
#include <utility>
#include <string>
#include <stdexcept>

using namespace std;

// Класс, представляющий чиновника в иерархии
class Official
{
private:
    int id;                     // Идентификатор чиновника
    int bribe;                  // Размер требуемой взятки
    vector<Official*> subordinates; // Список непосредственных подчиненных

public:
    // Конструктор с проверкой на отрицательную взятку
    Official(int id, int bribe);


    void AddingSubordinate(Official* subordinate);


    int getId() const;


    int getBribe() const;


    const vector<Official*>& getSubordinates() const;


    bool FinalOfficial() const;


    bool CheckingCycles(Official* parent) const;


    void OutputHierarchy(int level = 0) const;
};

// Класс, представляющий всю иерархию чиновников
class Ministry
{
private:
    Official* head;             // Главный чиновник 
    vector<Official*> officials; // Все чиновники министерства


    pair<int, vector<int>> CalculationBride(Official* official);


    bool HierarchyCycles() const;

public:
    // Конструктор и деструктор
    Ministry();
    ~Ministry();


    void UniquenessOfficial(int id, int bribe);


    void SubordinationRelations(int bossId, int subId);


    void AppointMain(int headId);


    void HierarchyOutput() const;


    pair<int, vector<int>> ResultMain();


    void Clear();
};



