#include <vector>
#include <utility>
#include <string>
#include <stdexcept>

using namespace std;

// �����, �������������� ��������� � ��������
class Official
{
private:
    int id;                     // ������������� ���������
    int bribe;                  // ������ ��������� ������
    vector<Official*> subordinates; // ������ ���������������� �����������

public:
    // ����������� � ��������� �� ������������� ������
    Official(int id, int bribe);


    void AddingSubordinate(Official* subordinate);


    int getId() const;


    int getBribe() const;


    const vector<Official*>& getSubordinates() const;


    bool FinalOfficial() const;


    bool CheckingCycles(Official* parent) const;


    void OutputHierarchy(int level = 0) const;
};

// �����, �������������� ��� �������� ����������
class Ministry
{
private:
    Official* head;             // ������� �������� 
    vector<Official*> officials; // ��� ��������� ������������


    pair<int, vector<int>> CalculationBride(Official* official);


    bool HierarchyCycles() const;

public:
    // ����������� � ����������
    Ministry();
    ~Ministry();


    void UniquenessOfficial(int id, int bribe);


    void SubordinationRelations(int bossId, int subId);


    void AppointMain(int headId);


    void HierarchyOutput() const;


    pair<int, vector<int>> ResultMain();


    void Clear();
};



