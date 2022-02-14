#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
using namespace std;

enum class Months { JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
const map<string, Months> monthEnum{ {"JAN", Months::JAN}, {"FEB", Months::FEB}, {"MAR", Months::MAR}, {"APR", Months::APR}, {"MAY", Months::MAY}, 
    {"JUN", Months::JUN}, {"JUL", Months::JUL}, {"AUG", Months::AUG}, {"SEP", Months::SEP}, {"OCT", Months::OCT}, {"NOV",Months::NOV}, {"DEC", Months::DEC} };
const regex regexpr(R"((?:^|\s)(([1-9]|[1-2][0-9]|[3][0-1])\ (JAN|FEB|MAR|APR|MAY|JUN|JUL|AUG|SEP|OCT|NOV|DEC)\ (\d{2}))(?:$|\s))");

class Date {
private:
    const string m_str;
    const int m_date;
    const Months m_month;
    const int m_year;

public:
    Date(string str, int date, Months month, int year) : m_str(str), m_date(date), m_month(month), m_year(year) {}

    bool isValid() {
        switch (m_month) {
        case Months::FEB:
            if (m_year != 0 && m_year % 4 == 0) {
                return m_date < 30;
            }
            else { return m_date < 29; }
        case Months::APR:
        case Months::JUN:
        case Months::SEP:
        case Months::NOV:
            return m_date < 31;
        }
        return true;
    }

    bool operator <(const Date& dateObj) const
    {
        return (m_year < dateObj.m_year) || (m_year == dateObj.m_year && m_month < dateObj.m_month)
            || (m_year == dateObj.m_year && m_month == dateObj.m_month && m_date < dateObj.m_date);
    }

    string getStr() {
        return m_str;
    }
};

void change(string* fileName) {
    cout << "Current file name is: " << *fileName << endl;
    cout << "Change it? y/n  ";
    char a;
    cin >> a;
    if (a == 'y' || a == 'Y') {
        cout << "Enter new file name: ";
        cin >> *fileName;
    }
}

void printFile(string fileName) {
    ifstream in(fileName);
    if (!in) {
        cout << "Cannot open file " << fileName << endl;
        return;
    }
    else if (in.peek() == EOF) {
        cout << "File " << fileName << " is empty" << endl;;
        return;
    }
    string a;
    cout << fileName << ":" << endl;
    while (!in.eof())
    {
        getline(in, a);
        cout << a << endl;
    }
    in.close();
}

void doJob(string fileName) {
    ifstream in(fileName);   
    if (!in) {
        cout << "Cannot open file " << fileName << endl;
        return;
    }
    else if (in.peek() == EOF) {
        cout << "File " << fileName << " is empty" << endl;;
        return;
    }

    vector<Date> dates;
    string a;

    while (!in.eof())
    {
        getline(in, a);
        if (a.empty()) {
            continue;
        }
        smatch res;
        while (regex_search(a, res, regexpr)) {
            Date date = Date(res[1], stoi(res[2]), monthEnum.find(res[3])->second, stoi(res[4]));
            if (date.isValid()) {
                dates.push_back(date);
            }          
            int pos = res.prefix().length();
            a = a.substr(pos + 2);
        }
    }

    in.close();

    if (dates.empty()) {
        cout << "No dates found" << endl;
    }
    else {
        Date result = *max_element(dates.begin(), dates.end());
        cout << "Max date is: " << result.getStr() << endl;
    }
}

int main()
{
    string fileName = "input.txt";

    cout << "File name is " << fileName << endl;
    cout << "Use" << endl;
    cout << "0 - to exit" << endl;
    cout << "1 - to find max date in file" << endl;
    cout << "2 - to show input file" << endl;
    cout << "3 - change input file name" << endl;

    while (true) {
        cout << endl << "Enter option key: ";
        int s;
        cin >> s;
        switch (s) {
        case 0:
            return 0;
        case 1:
            doJob(fileName);
            break;
        case 2:
            printFile(fileName);
            break;
        case 3:
            change(&fileName);
            break;
        default:
            cout << "Wrong option";
            break;
        }
    }
}