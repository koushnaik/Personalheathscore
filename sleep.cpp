#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;

/* =========================================================
   UTILITY: Get Current Date
   ========================================================= */
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday;

    return ss.str();
}

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
   QUERY PARAM PARSING
   ========================================================= */
bool getParam(const string &key, string &value) {
    char *query = getenv("QUERY_STRING");
    if (!query) return false;

    string q(query);
    string token;
    stringstream ss(q);

    while (getline(ss, token, '&')) {
        size_t pos = token.find('=');
        if (pos != string::npos) {
            string k = token.substr(0, pos);
            string v = token.substr(pos + 1);
            if (k == key) {
                value = v;
                return true;
            }
        }
    }
    return false;
}

bool getFloatParam(const string &key, float &value) {
    string v;
    if (!getParam(key, v)) return false;
    value = stof(v);
    return true;
}

/* =========================================================
   SLEEP QUALITY SCORE
   ========================================================= */
int calculateSleepScore(float hours,
                        const string &quality,
                        const string &interruptions) {

    int score = 0;

    // Duration score
    if (hours >= 7 && hours <= 9)
        score += 40;
    else if (hours >= 6)
        score += 25;
    else
        score += 10;

    // Quality score
    if (quality == "Excellent") score += 30;
    else if (quality == "Good") score += 20;
    else if (quality == "Average") score += 10;

    // Interruptions penalty
    if (interruptions == "None") score += 30;
    else if (interruptions == "Few") score += 15;
    else score += 5;

    return score;
}

/* =========================================================
   SLEEP CATEGORY
   ========================================================= */
string sleepCategory(int score) {
    if (score >= 85) return "Excellent Sleep";
    if (score >= 65) return "Good Sleep";
    if (score >= 45) return "Average Sleep";
    return "Poor Sleep";
}

/* =========================================================
   SLEEP TIPS
   ========================================================= */
string sleepTips(const string &category) {
    if (category == "Excellent Sleep") {
        return "Great sleep routine. Maintain consistency and healthy habits.";
    } else if (category == "Good Sleep") {
        return "Minor improvements possible. Try consistent bedtimes.";
    } else if (category == "Average Sleep") {
        return "Improve sleep hygiene. Reduce screen time before bed.";
    } else {
        return "High risk of fatigue. Prioritize sleep and reduce disruptions.";
    }
}

/* =========================================================
   STORE RECORD
   ========================================================= */
void storeRecord(const string &date, float hours,
                 const string &quality,
                 const string &interruptions,
                 int score, const string &category) {

    ofstream file("data/sleep.txt", ios::app);
    file << date << ","
         << hours << ","
         << quality << ","
         << interruptions << ","
         << score << ","
         << category << endl;
    file.close();
}

/* =========================================================
   READ PREVIOUS SCORE
   ========================================================= */
bool readLastScore(int &lastScore) {
    ifstream file("data/sleep.txt");
    if (!file.is_open()) return false;

    string line, lastLine;
    while (getline(file, line)) {
        if (!line.empty())
            lastLine = line;
    }
    file.close();

    if (lastLine.empty()) return false;

    vector<string> parts = split(lastLine, ',');
    if (parts.size() < 5) return false;

    lastScore = stoi(parts[4]);
    return true;
}

/* =========================================================
   COMPARISON MESSAGE
   ========================================================= */
string compareSleep(int previous, int current) {
    if (current > previous)
        return "Your sleep quality has improved compared to last record.";
    else if (current < previous)
        return "Sleep quality declined. Focus on better sleep habits.";
    else
        return "Sleep quality unchanged. Maintain consistency.";
}

/* =========================================================
   HTML HELPERS
   ========================================================= */
void printHTMLHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Sleep Analysis Report</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printHTMLFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/sleep.html'>Back to Sleep Analyzer</a> | ";
    cout << "<a href='/index.html'>Dashboard</a>";
    cout << "</div>";
    cout << "</body></html>";
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */
int main() {
    float sleepHours = 0.0f;
    string quality, interruptions;

    getFloatParam("sleep_hours", sleepHours);
    getParam("sleep_quality", quality);
    getParam("interruptions", interruptions);

    printHTMLHeader();
    cout << "<section class='info-section'>";

    if (sleepHours <= 0) {
        cout << "<h2>Error</h2>";
        cout << "<p>Invalid sleep duration entered.</p>";
        cout << "</section>";
        printHTMLFooter();
        return 0;
    }

    int score = calculateSleepScore(sleepHours, quality, interruptions);
    string category = sleepCategory(score);
    string tips = sleepTips(category);
    string date = getCurrentDate();

    int previousScore = -1;
    bool hasPrevious = readLastScore(previousScore);

    storeRecord(date, sleepHours, quality, interruptions, score, category);

    cout << "<h2>Sleep Cycle Analysis Report</h2>";
    cout << "<p><b>Date:</b> " << date << "</p>";
    cout << "<p><b>Sleep Duration:</b> " << sleepHours << " hours</p>";
    cout << "<p><b>Sleep Quality:</b> " << quality << "</p>";
    cout << "<p><b>Interruptions:</b> " << interruptions << "</p>";
    cout << "<p><b>Sleep Score:</b> " << score << " / 100</p>";
    cout << "<p><b>Category:</b> " << category << "</p>";

    cout << "<h3>Sleep Recommendations</h3>";
    cout << "<p>" << tips << "</p>";

    if (hasPrevious) {
        cout << "<h3>Progress Comparison</h3>";
        cout << "<p>Previous Sleep Score: " << previousScore << "</p>";
        cout << "<p>" << compareSleep(previousScore, score) << "</p>";
    } else {
        cout << "<p>This is your first sleep record.</p>";
    }

    cout << "</section>";
    printHTMLFooter();
    return 0;
}
