#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

/* =========================================================
   UTILITY: Split String
   ========================================================= */
vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/* =========================================================
   UTILITY: Read Last Two Lines of File
   ========================================================= */
bool readLastTwoLines(const string &filename, string &prev, string &last) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    if (lines.size() == 0) return false;
    if (lines.size() == 1) {
        last = lines[0];
        return true;
    }

    prev = lines[lines.size() - 2];
    last = lines.back();
    return true;
}

/* =========================================================
   HTML HELPERS
   ========================================================= */
void printHTMLHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Health History Report</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printHTMLFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/index.html'>Back to Dashboard</a>";
    cout << "</div>";
    cout << "</body></html>";
}

/* =========================================================
   BMI ANALYSIS
   ========================================================= */
void analyzeBMI() {
    string prev, last;
    cout << "<h2>BMI Analysis</h2>";

    if (!readLastTwoLines("data/bmi.txt", prev, last)) {
        cout << "<p>No BMI records available.</p>";
        return;
    }

    vector<string> p = split(prev, ',');
    vector<string> l = split(last, ',');

    float prevBMI = (p.size() >= 4) ? stof(p[3]) : -1;
    float currBMI = stof(l[3]);

    cout << "<p>Latest BMI: <b>" << currBMI << "</b></p>";

    if (prevBMI > 0) {
        cout << "<p>Previous BMI: " << prevBMI << "</p>";
        if (currBMI < prevBMI)
            cout << "<p class='good'>BMI improved. Good progress.</p>";
        else if (currBMI > prevBMI)
            cout << "<p class='warn'>BMI increased. Monitor diet and activity.</p>";
        else
            cout << "<p>BMI unchanged.</p>";
    }
}

/* =========================================================
   WATER ANALYSIS
   ========================================================= */
void analyzeWater() {
    string prev, last;
    cout << "<h2>Water Intake Analysis</h2>";

    if (!readLastTwoLines("data/water.txt", prev, last)) {
        cout << "<p>No water intake records available.</p>";
        return;
    }

    vector<string> p = split(prev, ',');
    vector<string> l = split(last, ',');

    float prevConsumed = (p.size() >= 4) ? stof(p[3]) : -1;
    float currConsumed = stof(l[3]);

    cout << "<p>Latest Intake: <b>" << currConsumed << " ml</b></p>";

    if (prevConsumed > 0) {
        cout << "<p>Previous Intake: " << prevConsumed << " ml</p>";
        if (currConsumed > prevConsumed)
            cout << "<p class='good'>Hydration improved.</p>";
        else
            cout << "<p class='warn'>Hydration decreased.</p>";
    }
}

/* =========================================================
   SLEEP ANALYSIS
   ========================================================= */
void analyzeSleep() {
    string prev, last;
    cout << "<h2>Sleep Analysis</h2>";

    if (!readLastTwoLines("data/sleep.txt", prev, last)) {
        cout << "<p>No sleep records available.</p>";
        return;
    }

    vector<string> p = split(prev, ',');
    vector<string> l = split(last, ',');

    int prevScore = (p.size() >= 5) ? stoi(p[4]) : -1;
    int currScore = stoi(l[4]);

    cout << "<p>Latest Sleep Score: <b>" << currScore << "</b></p>";

    if (prevScore > 0) {
        cout << "<p>Previous Score: " << prevScore << "</p>";
        if (currScore > prevScore)
            cout << "<p class='good'>Sleep quality improved.</p>";
        else
            cout << "<p class='warn'>Sleep quality declined.</p>";
    }
}

/* =========================================================
   STUDY ANALYSIS
   ========================================================= */
void analyzeStudy() {
    string prev, last;
    cout << "<h2>Study Cycle Analysis</h2>";

    if (!readLastTwoLines("data/study.txt", prev, last)) {
        cout << "<p>No study records available.</p>";
        return;
    }

    vector<string> p = split(prev, ',');
    vector<string> l = split(last, ',');

    float prevHours = (p.size() >= 2) ? stof(p[1]) : -1;
    float currHours = stof(l[1]);

    cout << "<p>Latest Study Hours: <b>" << currHours << "</b></p>";

    if (prevHours > 0) {
        cout << "<p>Previous Study Hours: " << prevHours << "</p>";
        if (currHours > prevHours)
            cout << "<p class='good'>Increased study time.</p>";
        else
            cout << "<p class='warn'>Study time reduced.</p>";
    }
}

/* =========================================================
   STEPS ANALYSIS
   ========================================================= */
void analyzeSteps() {
    string prev, last;
    cout << "<h2>Steps & Activity Analysis</h2>";

    if (!readLastTwoLines("data/steps.txt", prev, last)) {
        cout << "<p>No steps records available.</p>";
        return;
    }

    vector<string> p = split(prev, ',');
    vector<string> l = split(last, ',');

    int prevSteps = (p.size() >= 2) ? stoi(p[1]) : -1;
    int currSteps = stoi(l[1]);

    cout << "<p>Latest Steps: <b>" << currSteps << "</b></p>";

    if (prevSteps > 0) {
        cout << "<p>Previous Steps: " << prevSteps << "</p>";
        if (currSteps > prevSteps)
            cout << "<p class='good'>Physical activity improved.</p>";
        else
            cout << "<p class='warn'>Physical activity decreased.</p>";
    }
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */
int main() {
    printHTMLHeader();

    cout << "<section class='info-section'>";
    cout << "<h1>Comprehensive Health History Report</h1>";
    cout << "<p>This report compares your most recent health records "
            "with previous entries and highlights trends.</p>";

    analyzeBMI();
    analyzeWater();
    analyzeSleep();
    analyzeStudy();
    analyzeSteps();

    cout << "<hr>";
    cout << "<h2>Overall Health Summary</h2>";
    cout << "<ul>";
    cout << "<li>Track consistency rather than perfection.</li>";
    cout << "<li>Small daily improvements lead to long-term health gains.</li>";
    cout << "<li>Balance physical health, sleep, hydration, and productivity.</li>";
    cout << "</ul>";

    cout << "</section>";

    printHTMLFooter();
    return 0;
}
