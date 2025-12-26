#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void readBMI() {
    ifstream f("data/bmi.txt");
    string line;
    cout << "\"bmi\":[";
    bool first = true;
    while (getline(f, line)) {
        stringstream ss(line);
        string date, w, h, bmi;
        getline(ss, date, ',');
        getline(ss, w, ',');
        getline(ss, h, ',');
        getline(ss, bmi, ',');
        if (!first) cout << ",";
        cout << "{\"date\":\"" << date << "\",\"value\":" << bmi << "}";
        first = false;
    }
    cout << "]";
}

void readWater() {
    ifstream f("data/water.txt");
    string line;
    cout << "\"water\":[";
    bool first = true;
    while (getline(f, line)) {
        stringstream ss(line);
        string date, a, b, consumed;
        getline(ss, date, ',');
        getline(ss, a, ',');
        getline(ss, b, ',');
        getline(ss, consumed, ',');
        if (!first) cout << ",";
        cout << "{\"date\":\"" << date << "\",\"value\":" << consumed << "}";
        first = false;
    }
    cout << "]";
}

void readSleep() {
    ifstream f("data/sleep.txt");
    string line;
    cout << "\"sleep\":[";
    bool first = true;
    while (getline(f, line)) {
        stringstream ss(line);
        string date, a, b, c, score;
        getline(ss, date, ',');
        getline(ss, a, ',');
        getline(ss, b, ',');
        getline(ss, c, ',');
        getline(ss, score, ',');
        if (!first) cout << ",";
        cout << "{\"date\":\"" << date << "\",\"value\":" << score << "}";
        first = false;
    }
    cout << "]";
}

void readStudy() {
    ifstream f("data/study.txt");
    string line;
    cout << "\"study\":[";
    bool first = true;
    while (getline(f, line)) {
        stringstream ss(line);
        string date, a, b, c, d, score;
        getline(ss, date, ',');
        getline(ss, a, ',');
        getline(ss, b, ',');
        getline(ss, c, ',');
        getline(ss, d, ',');
        getline(ss, score, ',');
        if (!first) cout << ",";
        cout << "{\"date\":\"" << date << "\",\"value\":" << score << "}";
        first = false;
    }
    cout << "]";
}

void readSteps() {
    ifstream f("data/steps.txt");
    string line;
    cout << "\"steps\":[";
    bool first = true;
    while (getline(f, line)) {
        stringstream ss(line);
        string date, steps;
        getline(ss, date, ',');
        getline(ss, steps, ',');
        if (!first) cout << ",";
        cout << "{\"date\":\"" << date << "\",\"value\":" << steps << "}";
        first = false;
    }
    cout << "]";
}

int main() {
    cout << "Content-Type: application/json\n\n";
    cout << "{";
    readBMI();   cout << ",";
    readWater(); cout << ",";
    readSleep(); cout << ",";
    readStudy(); cout << ",";
    readSteps();
    cout << "}";
    return 0;
}
