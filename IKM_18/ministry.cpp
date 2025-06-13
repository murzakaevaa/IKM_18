#include "ministry.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// ����������� � ��������� �� ������������� ������
Official::Official(int id, int bribe) : id(id), bribe(bribe)
{
    if (bribe < 0) throw invalid_argument("������ �� ����� ���� �������������");
}

// ���������� ������������ � ��������� �� null
void Official::AddingSubordinate(Official* subordinate)
{
    if (!subordinate) throw invalid_argument("���������� �� ����� ���� nullptr");
    subordinates.push_back(subordinate);
}

// �������
int Official::getId() const { return id; }
int Official::getBribe() const { return bribe; }
const vector<Official*>& Official::getSubordinates() const { return subordinates; }

// ��������, �������� �� �������� �������� � ��������
bool Official::FinalOfficial() const { return subordinates.empty(); }

// ����������� �������� �� ����� � ���������
bool Official::CheckingCycles(Official* parent) const
{
    if (this == parent) return true;  // ��������� ����
    for (auto sub : subordinates) {
        if (sub->CheckingCycles(parent)) return true;
    }
    return false;
}

// ����������� ����� �������� 
void Official::OutputHierarchy(int level) const
{
    for (int i = 0; i < level; ++i) cout << "    ";
    cout << "|--- [" << id << "] (������: " << bribe << ")\n";


    for (auto sub : subordinates)
        sub->OutputHierarchy(level + 1);
}



// �����������
Ministry::Ministry() : head(nullptr) {}

// ���������� � �������� ������
Ministry::~Ministry() { Clear(); }

// ���������� ��������� � ��������� �� ������������ ID
void Ministry::UniquenessOfficial(int id, int bribe)
{
    for (auto official : officials)
    {
        if (official->getId() == id)
        {
            throw invalid_argument("�������� � ID " + to_string(id) + " ��� ����������");
        }
    }
    officials.push_back(new Official(id, bribe));
}

// ��������� ��������� ���������� � ��������� �� �����
void Ministry::SubordinationRelations(int bossId, int subId)
{
    Official* boss = nullptr, * sub = nullptr;

    // ����� ���������� � �����������
    for (auto official : officials)
    {
        if (official->getId() == bossId) boss = official;
        if (official->getId() == subId) sub = official;
    }

    // �������� ������������� ����������
    if (!boss || !sub)
    {
        throw invalid_argument("�������� ID ���������� (" + to_string(bossId) +
            ") ��� ����������� (" + to_string(subId) + ")");
    }

    // �������� �� �������� �����
    if (sub->CheckingCycles(boss))
    {
        throw logic_error("��������� ���� � �������� ��� ���������� �����������");
    }

    boss->AddingSubordinate(sub);
}

// ���������� �������� ���������
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
    throw invalid_argument("������� �������� � ID " + to_string(headId) + " �� ������");
}

// �������� ���� �������� �� �����
bool Ministry::HierarchyCycles() const
{
    if (!head) return false;
    return head->CheckingCycles(nullptr);
}

// ����� �������� � ��������� �� ������� ������
void Ministry::HierarchyOutput() const
{
    if (!head) throw logic_error("�������� �� ������� - ������� �������� �� ��������");
    if (HierarchyCycles()) throw logic_error("��������� ���� � ��������");
    head->OutputHierarchy();
}

// ����������� ������ ����������� ������ � ������������ ����
pair<int, vector<int>> Ministry::CalculationBride(Official* official)
{
    if (!official) throw invalid_argument("�������� �� ����� ���� nullptr");

    // ������� ������: ���� �������� - ��� ����������, ���� ��� ������
    if (official->FinalOfficial())
    {
        return { official->getBribe(), {official->getId()} };
    }

    int minTotal = numeric_limits<int>::max();
    vector<int> bestPath;

    // ������� ���� ���������� ��� ������ ������������ ����
    for (Official* sub : official->getSubordinates())
    {
        auto result = CalculationBride(sub);
        int total = result.first + official->getBribe();

        // ����� ���� � ����������� ��������� �������
        if (total < minTotal)
        {
            minTotal = total;
            bestPath = result.second;
        }
    }

    // ���������� �������� ��������� � ����
    bestPath.push_back(official->getId());
    return { minTotal, bestPath };
}

// ��������� ���������� ��� �������� ���������
pair<int, vector<int>> Ministry::ResultMain()
{
    if (!head) throw logic_error("���������� ��������� - ������� �������� �� ��������");
    if (HierarchyCycles()) throw logic_error("���������� ��������� - ��������� ���� � ��������");
    return CalculationBride(head);
}

// ������� ���� ������ � ������������� ������
void Ministry::Clear()
{
    for (auto official : officials)
        delete official;
    officials.clear();
    head = nullptr;
}