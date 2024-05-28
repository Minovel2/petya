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
#include <regex>
using namespace std;
string path1, path2, path3;
string searchInstitute1 = "", searchInstitute2 = "", searchKafedra3 = "";
int instituteSize, firstNameSize, lastNameSize, otchestvoSize, kafedraSize, idSize;
int numberSize = 17;

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

string getBigLine(int n) {
    string s = "\n";
    for (int i = 0; i < n; i++) {
        s += '-';
    }
    s += '\n';
    return s;
}

void updateSizes(vector<miit>& miits) {
    instituteSize = 10, firstNameSize = 10, lastNameSize = 10, otchestvoSize = 10, kafedraSize = 10, idSize = 2;
    int k = 1, idSizeCounter = 0;
    for (miit& m : miits) {
        if (k % 10 == 0) {
            idSizeCounter++;
        }
        if (m.institute.length() > instituteSize) {
            instituteSize = m.institute.length();
        }
        if (m.firstName.length() > firstNameSize) {
            firstNameSize = m.firstName.length();
        }
        if (m.lastName.length() > lastNameSize) {
            lastNameSize = m.lastName.length();
        }
        if (m.otchestvo.length() > otchestvoSize) {
            otchestvoSize = m.otchestvo.length();
        }
        if (m.kafedra.length() > kafedraSize) {
            kafedraSize = m.kafedra.length();
        }
        k++;
    }

    if (idSizeCounter > idSize)
        idSize = idSizeCounter;
}

void afterError() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool hasInstitute(vector<miit>& miits, string institute, miit& result) {
    for (miit& m : miits) {
        if (m.institute == institute) {
            result = m;
            return true;
        }
    }
    return false;
}

bool isPhoneNumber(const std::string& number) {
    std::regex pattern("(\\+7|8)\\s?\\(?\\d{3}\\)?\\s?\\d{3}\\-?\\d{2}\\-?\\d{2}");
    return std::regex_match(number, pattern);
}

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
        afterError();
        return;
    }

    for (int i = 0; i < miitCount; i++) {
        miit Miit;
        cout << "Введите наименование института " << i + 1 << ": ";
        getline(cin >> ws, data);
        Miit.institute = data;
        miit result;
        if (hasInstitute(miits, Miit.institute, result)) {
            cout << "Фамилия директора института " << i + 1 << ": " << result.lastName << endl;
            Miit.lastName = result.lastName;
            cout << "Имя директора института " << i + 1 << ": " << result.firstName << endl;
            Miit.firstName = result.firstName;
            cout << "Отчество директора института " << i + 1 << ": " << result.otchestvo << endl;
            Miit.otchestvo = result.otchestvo;
            cout << "Номер тел. директора института " << i + 1 << ": " << result.directorNumber << endl;
            Miit.directorNumber = result.directorNumber;
        }
        else {
            cout << "Введите фамилию директора института " << i + 1 << ": ";
            getline(cin >> ws, data);
            Miit.lastName = data;
            cout << "Введите имя директора института " << i + 1 << ": ";
            getline(cin >> ws, data);
            Miit.firstName = data;
            cout << "Введите отчество директора института " << i + 1 << ": ";
            getline(cin >> ws, data);
            Miit.otchestvo = data;

            while (true) {
                cout << "Введите номер телефона директора института " << i + 1 << ": ";
                getline(cin >> ws, data);
                if (isPhoneNumber(data)) {
                    Miit.directorNumber = data;
                    break;
                }
                else {
                    cout << "Это не номер телефона, введите номер корректно." << endl;
                }
            }
        }
        
        cout << "Сколько кафедр МИИТа вы хотите добавить?: ";
        cin >> kafedraCount;
        if (cin.fail() || kafedraCount < 0) {
            cout << "Ошибка";
            afterError();
            return;
        }
        for (int j = 0; j < kafedraCount; j++) {
            cout << "Введите название кафедры " << j + 1 << ": ";
            getline(cin >> ws, data);
            Miit.kafedra = data;
            while (true) {
                cout << "Введите номер телефона кафедры " << j + 1 << ": ";
                getline(cin >> ws, data);
                if (isPhoneNumber(data)) {
                    Miit.kafedraNumber = data;
                    break;
                }
                else {
                    cout << "Это не номер телефона, введите номер корректно." << endl;
                }
            }
            
            if (canPushMiit(miits, Miit.institute, Miit.lastName, Miit.firstName, Miit.otchestvo, Miit.directorNumber, Miit.kafedra, Miit.kafedraNumber)) {
                miits.push_back(Miit);
                cout << "Данные добавлены!" << endl;
            } else {
                cout << "Данные не добавлены. Проверьте внимательно данные (к примеру, директор не может иметь несколько разных номеров тел., кафедры не могут иметь несколько номеров тел., директор не может управлять несколькими кафедрами, институт не может иметь кафедры из других институтов)" << endl;
            }
        }
        cout << endl;
    }
}

void beforePrint() {
    cout << " " << setw(idSize) << "id" << " | ";
    cout << setw(instituteSize) << "Институт" << " | ";
    cout << setw(lastNameSize) << "Фамилия" << " | ";
    cout << setw(firstNameSize) << "Имя" << " | ";
    cout << setw(otchestvoSize) << "Отчество" << " | ";
    cout << setw(numberSize) << "Тел. директора" << " | ";
    cout << setw(kafedraSize) << "Кафедра" << " | ";
    cout << setw(numberSize) << "Тел. кафедры" << " | ";
    cout << getBigLine(idSize + instituteSize + lastNameSize + firstNameSize + otchestvoSize + numberSize * 2 + kafedraSize + 8 * 3);
}

void print(vector<miit>& miits) {
    beforePrint();
    int id = 1;
    for (miit& m : miits)
    {
        cout << " " << setw(idSize) << id << " | ";
        cout << setw(instituteSize) << toRus(m.institute) << " | ";
        cout << setw(lastNameSize) << toRus(m.lastName) << " | ";
        cout << setw(firstNameSize) << toRus(m.firstName) << " | ";
        cout << setw(otchestvoSize) << toRus(m.otchestvo) << " | ";
        cout << setw(numberSize) << toRus(m.directorNumber) << " | ";
        cout << setw(kafedraSize) << toRus(m.kafedra) << " | ";
        cout << setw(numberSize) << toRus(m.kafedraNumber) << " | " << endl;
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
    cout << "Сохранено в " << p;
    toRus(miits);
}

void megaSort(int& sortField, vector<miit>& miits) {
    bool mode;
    cout << "Введите 1 (сортировать по возрастанию) или 0 (по убыванию): ";
    cin >> mode;
    if (cin.fail())
        return;

    switch (sortField) {
    case 0:
        if (mode) {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.institute < c2.institute; });
        }
        else {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.institute > c2.institute; });
        }
        break;
    case 1:
        if (mode) {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.lastName + c1.firstName + c1.otchestvo < c2.lastName + c2.firstName + c2.otchestvo; });
        }
        else {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.lastName + c1.firstName + c1.otchestvo > c2.lastName + c2.firstName + c2.otchestvo; });
        }
        break;
    case 2:
        if (mode) {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.directorNumber < c2.directorNumber; });
        }
        else {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.directorNumber > c2.directorNumber; });
        }
        break;
    case 3:
        if (mode) {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.kafedra < c2.kafedra; });
        }
        else {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.kafedra > c2.kafedra; });
        }
        break;
    case 4:
        if (mode) {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.kafedraNumber < c2.kafedraNumber; });
        }
        else {
            sort(miits.begin(), miits.end(), [](miit& c1, miit& c2) { return c1.kafedraNumber > c2.kafedraNumber; });
        }
        break;
    }
}

void removeProcess(vector<miit>& miits) {
    print(miits);
    int k = 0, id = 0;
    cout << "Введите номер строки, которую хотите удалить: ";
    cin >> id;
    if (std::cin.fail() || id < 0) {
        std::cout << "Ошибка.";
        afterError(); // очистить состояние ошибки
         // очистить буфер ввода
        return;
    }

    for (vector<miit>::iterator i = miits.begin(); i < miits.end(); i++) {
        k++;
        if (k == id) {
            miits.erase(i);
            cout << "Удалено!";
            return;
        }
    }
    cout << "Не удалено, возможно такой строки не существует";
}

void generateResult1(vector<miit> miits, bool mode) {
    fstream fs;
    fs.open(path1 + ".txt", fstream::out);
    if (mode) {
        cout << "Введите институт: ";
        getline(cin >> ws, searchInstitute1);
        cout << "Список кафедр, входящих в институт " << searchInstitute1 << ":" << endl;
        cout << " " << setw(idSize) << "id" << " | ";
        cout << setw(instituteSize) << "Институт" << " | ";
        cout << setw(kafedraSize) << "Кафедра" << " | ";
        cout << setw(numberSize) << "Тел. кафедры" << " | ";
        cout << getBigLine(idSize + instituteSize + numberSize + kafedraSize + 4 * 3);
    }
    fs << left << " " << setw(idSize) << "id" << " | ";
    fs << setw(instituteSize) << "Институт" << " | ";
    fs << setw(kafedraSize) << "Кафедра" << " | ";
    fs << setw(numberSize) << "Тел. кафедры" << " | ";
    fs << getBigLine(idSize + instituteSize + numberSize + kafedraSize + 4 * 3);
    int k = 1;
    for (miit& m : miits) {
        if (m.institute == searchInstitute1) {
            if (mode) {
                cout << " " << setw(idSize) << k << " | ";
                cout << setw(instituteSize) << m.institute << " | ";
                cout << setw(kafedraSize) << m.kafedra << " | ";
                cout << setw(numberSize) << m.kafedraNumber << " | " << endl;
            }
            fs << " " << setw(idSize) << k << " | ";
            fs << setw(instituteSize) << m.institute << " | ";
            fs << setw(kafedraSize) << m.kafedra << " | ";
            fs << setw(numberSize) << m.kafedraNumber << " | " << endl;
            k++;
        }
    }
}

void generateResult2(vector<miit> miits, bool mode) {
    fstream fs;
    fs.open(path2 + ".txt", fstream::out);
    if (mode) {
        cout << "Введите институт: ";
        getline(cin >> ws, searchInstitute2);
        cout << "ФИО и номер телефона директора института " << searchInstitute2 << ":" << endl;
        cout << " " << setw(idSize) << "id" << " | ";
        cout << setw(instituteSize) << "Институт" << " | ";
        cout << setw(lastNameSize) << "Фамилия" << " | ";
        cout << setw(firstNameSize) << "Имя" << " | ";
        cout << setw(otchestvoSize) << "Отчество" << " | ";
        cout << setw(numberSize) << "Тел. директора" << " | ";
        cout << getBigLine(idSize + instituteSize + lastNameSize + firstNameSize + otchestvoSize + numberSize + 6 * 3);
    }
    fs << left << " " << setw(idSize) << "id" << " | ";
    fs << setw(instituteSize) << "Институт" << " | ";
    fs << setw(lastNameSize) << "Фамилия" << " | ";
    fs << setw(firstNameSize) << "Имя" << " | ";
    fs << setw(otchestvoSize) << "Отчество" << " | ";
    fs << setw(numberSize) << "Тел. директора" << " | ";
    fs << getBigLine(idSize + instituteSize + lastNameSize + firstNameSize + otchestvoSize + numberSize + 6 * 3);
    int k = 1;
    for (miit& m : miits) {
        if (m.institute == searchInstitute2) {
            if (mode) {
                cout << " " << setw(idSize) << k << " | ";
                cout << setw(instituteSize) << m.institute << " | ";
                cout << setw(lastNameSize) << m.lastName << " | ";
                cout << setw(firstNameSize) << m.firstName << " | ";
                cout << setw(otchestvoSize) << m.otchestvo << " | ";
                cout << setw(numberSize) << m.directorNumber << " | " << endl;
            }
            fs << " " << setw(idSize) << k << " | ";
            fs << setw(instituteSize) << m.institute << " | ";
            fs << setw(lastNameSize) << m.lastName << " | ";
            fs << setw(firstNameSize) << m.firstName << " | ";
            fs << setw(otchestvoSize) << m.otchestvo << " | ";
            fs << setw(numberSize) << m.directorNumber << " | " << endl;
            break;
        }
    }
}

void generateResult3(vector<miit> miits, bool mode) {
    fstream fs;
    fs.open(path3 + ".txt", fstream::out);
    if (mode) {
        cout << "Введите название кафедры, телефон которой вывести: ";
        getline(cin >> ws, searchKafedra3);
        cout << "Телефон кафедры " << searchKafedra3 << ":" << endl;
        cout << " " << setw(idSize) << "id" << " | ";
        cout << setw(kafedraSize) << "Кафедра" << " | ";
        cout << setw(numberSize) << "Тел. кафедры" << " | ";
        cout << getBigLine(idSize + kafedraSize + numberSize + 3 * 3);
    }
    fs << left << " " << setw(idSize) << "id" << " | ";
    fs << setw(kafedraSize) << "Кафедра" << " | ";
    fs << setw(numberSize) << "Тел. кафедры" << " | ";
    fs << getBigLine(idSize + kafedraSize + numberSize + 3 * 3);
    int k = 1;
    for (miit& m : miits) {
        if (m.kafedra == searchKafedra3) {
            if (mode) {
                cout << " " << setw(idSize) << k << " | ";
                cout << setw(kafedraSize) << m.kafedra << " | ";
                cout << setw(numberSize) << m.kafedraNumber << " | " << endl;
            }
            fs << " " << setw(idSize) << k << " | ";
            fs << setw(kafedraSize) << m.kafedra << " | ";
            fs << setw(numberSize) << m.kafedraNumber << " | " << endl;
            break;
        }
    }
}

void updateResults(vector<miit>& miits) {
    generateResult1(miits, 0);
    generateResult2(miits, 0);
    generateResult3(miits, 0);
}

const int buttonsCount1 = 7;
string* text1 = new string[buttonsCount1]{"Создать список кафедр, входящих в институт, название которого вводится с клавиатуры","Создать список с ФИО и номером телефона директора института, название которого вводится с клавиатуры","Записать телефон кафедры, название которой вводится с клавиатуры","Сортировка","Добавить данные","Удалить данные","Сохранить как исходные данные"};
const int buttonsCount2 = 6;
string* text2 = new string[buttonsCount2]{ "По институту","По ФИО директора","По тел. директора","По кафедре","По тел. кафедры","Назад"};
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
        if (ch == 115 || ch == 80 || ch == 50 || ch == 251) {
            menu = (menu + 1) % buttonsCount;
        }
        else if (ch == 119 || ch == 72 || ch == 56 || ch == 246) {
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
                    generateResult1(miits, 1);
                    break;
                case 1:
                    generateResult2(miits, 1);
                    break;
                case 2:
                    generateResult3(miits, 1);
                    break;
                case 3:
                    menuType = 2;
                    menu = 0;
                    break;
                case 4:
                    addSomeMiits(miits);
                    updateSizes(miits);
                    updateResults(miits);
                    break;
                case 5:
                    removeProcess(miits);
                    updateSizes(miits);
                    updateResults(miits);
                    break;
                case 6:
                    saveToFile(miits);
                    break;
                }
            }
            else if (menuType == 2) {
                if (menu == 5) {
                    menuType = 1;
                    menu = 3;
                }
                else {
                    system("cls");
                    megaSort(menu, miits);
                    updateResults(miits);
                }
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
        afterError();
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
        toEng(miits);
        nlohmann::json j = miits;
        toRus(miits);
        updateSizes(miits);
        print(miits);
    }
    else {
        string p;
        cout << "Введите название файла c исходными данными (без расширения): ";
        cin >> p;
        p += ".json";
        fs.open(p, fstream::in);
        if (!fs.is_open()) {
            cout << "\nОшибка открытия файла, возможно, файла с таким именем не существует.";
            afterError();
            return 0;
        }
        else {
            nlohmann::json j;
            fs >> j;
            miits = j;
            updateSizes(miits);
            print(miits);
        }

        fs.close();
    }

    toRus(miits);
    int menu = 0;
    int menuType = 1;
    while (true) {
        startCycle(miits, menu, menuType);
        if (menuType == 1 && menu != 3)
            _getch();
    }

    return 0;
}
