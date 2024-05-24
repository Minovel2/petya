#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <conio.h>
#include "dependencies/include/nlohmann/json.hpp"
#include <map>
using namespace std;
string path1, path2, path3;

struct miit
{
    string institute;
    string firstName;
    string lastName;
    string otchestvo;
    string directorNumber;
    string kafedra;
    string kafedraNumber;
};

string toEng(string& str) {
    string s;
    const string rus = "йцукенгшщзхъфывапролджэячсмитьбю";
    const string eng = "qwertyuiop[]asdfghjkl;'zxcvbnm,.";
    const string rusUp = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
    const string engUp = "QWERTYUIOP()ASDFGHJKL:!ZXCVBNM+=";

    for (size_t i = 0; i < str.length(); i++)
    {
        size_t pos1 = rus.find(str[i]);
        size_t pos2 = rusUp.find(str[i]);

        if (pos1 != string::npos) {
            s += "\\";
            s += eng[pos1];
        }
        else if (pos2 != string::npos) {
            s += "\\";
            s += engUp[pos2];
        }
        else {
            s += str[i];
        }
    }
    return s;
}

string toRus(string& str) {
    string s;
    const string rus = "йцукенгшщзхъфывапролджэячсмитьбю";
    const string eng = "qwertyuiop[]asdfghjkl;'zxcvbnm,.";
    const string rusUp = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
    const string engUp = "QWERTYUIOP()ASDFGHJKL:!ZXCVBNM+=";

    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '\\') {
            size_t pos1 = eng.find(str[i + 1]);
            size_t pos2 = engUp.find(str[i + 1]);
            if (pos1 != string::npos) {
                s += rus[pos1];
                i++;
            }
            else if (pos2 != string::npos) {
                s += rusUp[pos2];
                i++;
            }
        }
        else {
            s += str[i];
        }
    }
    return s;
}

void toEng(vector<miit>& miits) {
    for (miit& m : miits) {
        m.firstName = toEng(m.firstName);
        m.lastName = toEng(m.lastName);
        m.otchestvo = toEng(m.otchestvo);
        m.kafedra = toEng(m.kafedra);
        m.institute = toEng(m.institute);
    }
}
void toRus(vector<miit>& miits) {
    for (miit& m : miits) {
        m.firstName = toRus(m.firstName);
        m.lastName = toRus(m.lastName);
        m.otchestvo = toRus(m.otchestvo);
        m.kafedra = toRus(m.kafedra);
        m.institute = toRus(m.institute);
    }
}

void to_json(nlohmann::json& j, const miit& p) {
    j = nlohmann::json{ {"institute", p.institute}, {"lastName", p.lastName}, {"firstName", p.firstName}, {"otchestvo", p.otchestvo}, {"directorNumber", p.directorNumber}, {"kafedra", p.kafedra}, {"kafedraNumber", p.kafedraNumber} };
}

void from_json(const nlohmann::json& j, miit& p) {
    j.at("institute").get_to(p.institute);
    j.at("lastName").get_to(p.lastName);
    j.at("firstName").get_to(p.firstName);
    j.at("otchestvo").get_to(p.otchestvo);
    j.at("directorNumber").get_to(p.directorNumber);
    j.at("kafedra").get_to(p.kafedra);
    j.at("kafedraNumber").get_to(p.kafedraNumber);
}

bool canPushMiit(vector<miit>& miits, string& institute, string& lastName, string& firstName, string& otchestvo, string& directorNumber, string& kafedra, string& kafedraNumber) {
    string FIO = lastName + firstName + otchestvo;
    for (miit& m : miits) {
        string iterFIO = m.lastName + m.firstName + m.otchestvo;
        if ((m.institute == institute && FIO != iterFIO) || (m.institute != institute && FIO == iterFIO))
            return false;
        if ((FIO == iterFIO && directorNumber != m.directorNumber) || (FIO != iterFIO && directorNumber == m.directorNumber))
            return false;

        if (m.kafedra == kafedra || m.kafedraNumber == kafedraNumber)
            return false;
    }
    return true;
}

void addSomeMiits(vector<miit>& miits) {
    int kafedraCount, miitCount;
    string data;
    cout << "Сколько структур МИИТа вы хотите добавить?: ";
    cin >> miitCount;
    if (cin.fail() || miitCount < 0) {
        cout << "Ошибка";
        return;
    }

    for (int i = 0; i < miitCount; i++) {
        miit Miit;
        cout << "Введите наименование института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.institute = toEng(data);
        cout << "Введите фамилию директора института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.lastName = toEng(data);
        cout << "Введите имя директора института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.firstName = toEng(data);
        cout << "Введите отчество директора института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.otchestvo = toEng(data);
        cout << "Введите номер телефона директора института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.directorNumber = data;

        cout << "Сколько кафедр МИИТа вы хотите добавить?: ";
        cin >> kafedraCount;
        if (cin.fail() || kafedraCount < 0) {
            cout << "Ошибка";
            return;
        }
        for (int j = 0; j < kafedraCount; j++) {
            cout << "Введите название кафедры " << j + 1 << ": ";
            getline(cin >> ws, data);
            Miit.kafedra = toEng(data);
            cout << "Введите номер телефона кафедры " << j + 1 << ": ";
            getline(cin >> ws, data);
            Miit.kafedraNumber = data;
            if (canPushMiit(miits, Miit.institute, Miit.lastName, Miit.firstName, Miit.otchestvo, Miit.directorNumber, Miit.kafedra, Miit.kafedraNumber))
                miits.push_back(Miit);
        }
        cout << endl;
    }
}

void beforePrint() {
    cout << " id | ";
    cout << "Институт   | ";
    cout << "Фамилия    | ";
    cout << "Имя        | ";
    cout << "Отчество   | ";
    cout << "Тел. директора | ";
    cout << "Кафедра    | ";
    cout << "Тел. кафедры   | ";
    cout << "\n--------------------------------------------------------------------------------------------------------\n";
}
void beforePrintInFile(fstream& fs) {
    fs << " id | ";
    fs << "Имя кинотеатра       | ";
    fs << "Фильм                | ";
    fs << "\n---------------------------------------------------\n";
}

void print(vector<miit>& miits) {
    beforePrint();
    int id = 1;
    for (miit& m : miits)
    {
        cout << " " << setw(2) << id << " | ";
        cout << setw(10) << toRus(m.institute) << " | ";
        cout << setw(10) << toRus(m.lastName) << " | ";
        cout << setw(10) << toRus(m.firstName) << " | ";
        cout << setw(10) << toRus(m.otchestvo) << " | ";
        cout << setw(14) << toRus(m.directorNumber) << " | ";
        cout << setw(10) << toRus(m.kafedra) << " | ";
        cout << setw(14) << toRus(m.kafedraNumber) << " | " << endl;
        id++;
    }
}

void saveToFile(vector<miit>& miits) {
    toEng(miits);
    nlohmann::json j = miits;
    fstream fs;
    string p;
    cout << "Введите название файла без расширения: ";
    cin >> p;
    p += ".json";
    fs.open(p, fstream::out);
    fs << j;
    fs.close();
    toRus(miits);
}

const int buttonsCount1 = 7;
string* text1 = new string[buttonsCount1]{"создать список кафедр, входящих в институт, название которого вводится с клавиатуры","a2","a3","Сортировка","a2","a3","Сохранить как исходные данные"};
const int buttonsCount2 = 3;
string* text2 = new string[buttonsCount2]{ "а4","a5","a6" };
int buttonsCount;

void showMenu(int& menu, vector<miit>& miits, string* text) {

    system("cls");
    print(miits);
    cout << endl;
    for (int i = 0; i < buttonsCount; i++) {
        if (i == menu) {
            cout << "-->  ";
        }
        cout << i+1 << ". " << text[i];
        if (i == menu) {
            cout << "  <--";
        }
        cout << endl;
    }
}

void startCycle(vector<miit>& miits, int& menu, int& menuType) {
    if (menuType == 1) {
        buttonsCount = buttonsCount1;
        showMenu(menu, miits, text1);
    }
    else if (menuType == 2) {
        buttonsCount = buttonsCount2;
        showMenu(menu, miits, text2);
    }

    int ch;
    bool cycle = true;
    while (cycle) {
        ch = _getch();
        if (ch == 115 || ch == 80 || ch == 50) {
            menu = (menu + 1) % buttonsCount;
        }
        else if (ch == 119 || ch == 72 || ch == 56) {
            if (menu == 0)
                menu = buttonsCount;

            menu = (menu - 1) % buttonsCount;
        }

        if (ch != 13) {
            if (menuType == 1) {
                showMenu(menu, miits, text1);
            }
            else if (menuType == 2) {
                showMenu(menu, miits, text2);
            }
        }
        else {
            cycle = false;
            system("cls");
            if (menuType == 1) {
                switch (menu) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    menuType = 2;
                    menu = 0;
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    saveToFile(miits);
                    break;
                }
            }
            else if (menuType == 2) {
                menuType = 1;
                menu = 3;
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    system("chcp 1251");
    using namespace std;
    cout << setiosflags(ios::left);

    fstream fs;
    vector<miit> miits;

    fs.open("defaultNames.txt", fstream::in);
    fs >> path1;
    fs >> path2;
    fs >> path3;
    fs.close();
    cout << "Список кафедр, входящих в институт, название которого вводится с клавиатуры, запишется в файл: " << path1 << ".txt\nФИО директора, номер телефона директора института, название которого вводится с клавиатуры, запишется в файл: " << path2 << ".txt\nТелефон кафедры, название которой вводится с клавиатуры, запишется в файл: " << path3 << ".txt" << endl;
    cout << "Желаете изменить файлы, в которые запишется результат? 1 - да, 0 - нет: ";
    bool mode;
    cin >> mode;
    if (cin.fail()) {
        cout << "Ошибка";
        return 0;
    }

    if (mode) {
        cout << "Введите имя файла 1: ";
        cin >> path1;
        cout << "Введите имя файла 2: ";
        cin >> path2;
        cout << "Введите имя файла 3: ";
        cin >> path3;

        fs.open("defaultNames.txt", fstream::out);
        fs << path1 << endl;
        fs << path2 << endl;
        fs << path3 << endl;
        fs.close();
    }

    cout << "Ввод исходных данных с клавиатуры? 1 - да, 0 - нет: ";
    cin >> mode;

    if (mode) {
        addSomeMiits(miits);
        nlohmann::json j = miits;
        print(miits);
    }
    else {
        string p;
        cout << "Введите название файла c исходными данными (без расширения): ";
        cin >> p;
        p += ".json";
        fs.open(p, fstream::in);
        if (!fs.is_open()) {
            cout << "Ошибка открытия файла, возможно, файла с таким именем не существует.";
            return 0;
        }
        else {
            nlohmann::json j;
            fs >> j;
            miits = j;
            print(miits);
        }

        fs.close();
    }

    toRus(miits);
    int menu = 0;
    int menuType = 1;
    while (true) {
        startCycle(miits, menu, menuType);
        if (menuType == 1)
            _getch();
    }

    return 0;
}
