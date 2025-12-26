#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;

/* ===========================
   Get current date
   =========================== */
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday;

    return ss.str();
}

/* ===========================
   Safe query parameter reader
   =========================== */
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

/* ===========================
   Study efficiency score
   =========================== */
int calculateScore(int hours, int session, int breakTime, const string &focus) {
    int score = 0;

    if (hours >= 4 && hours <= 8) score += 30;
    else if (hours >= 2) score += 20;
    else score += 10;

    if (session >= 40 && session <= 60) score += 30;
    else score += 15;

    if (breakTime >= 10 && breakTime <= 20) score += 20;
    else score += 10;

    if (focus == "High") score += 20;
    else if (focus == "Moderate") score += 15;
    else score += 10;

    return score;
}

/* ===========================
   Study category
   =========================== */
string studyCategory(int score) {
    if (score >= 85) return "Excellent Productivity";
    if (score >= 65) return "Good Productivity";
    if (score >= 45) return "Average Productivity";
    return "Poor Productivity";
}

/* ===========================
   Study tips
   =========================== */
string studyTips(const string &category) {
    if (category == "Excellent Productivity")
        return "Great study habits. Maintain consistency and balance.";
    if (category == "Good Productivity")
        return "Good work. Try optimizing break intervals.";
    if (category == "Average Productivity")
        return "Improve focus and follow structured study sessions.";
    return "Low productivity. Reduce distractions and use time-blocking.";
}

/* ===========================
   Store record
   =========================== */
void storeRecord(const string &date, int hours, int session,
                 int breakTime, const string &focus,
                 int score, const string &category) {

    ofstream file("data/study.txt", ios::app);
    file << date << ","
         << hours << ","
         << session << ","
         << breakTime << ","
         << focus << ","
         << score << ","
         << category << endl;
    file.close();
}

/* ===========================
   Read previous score
   =========================== */
bool readLastScore(int &lastScore) {
    ifstream file("data/study.txt");
    if (!file.is_open()) return false;

    string line, lastLine;
    while (getline(file, line)) {
        if (!line.empty())
            lastLine = line;
    }
    file.close();

    if (lastLine.empty()) return false;

    stringstream ss(lastLine);
    string temp;
    vector<string> parts;

    while (getline(ss, temp, ',')) {
        parts.push_back(temp);
    }

    if (parts.size() < 6) return false;

    lastScore = stoi(parts[5]);
    return true;
}

/* ===========================
   Comparison message
   =========================== */
string compareScore(int prev, int curr) {
    if (curr > prev)
        return "Your study efficiency has improved.";
    if (curr < prev)
        return "Study efficiency decreased. Review your routine.";
    return "Study efficiency unchanged.";
}

/* ===========================
   HTML helpers
   =========================== */
void printHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Study Report</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/study.html'>Back to Study</a> | ";
    cout << "<a href='/index.html'>Dashboard</a>";
    cout << "</div></body></html>";
}

/* ===========================
   MAIN
   =========================== */
int main() {
    string sh, sl, bt, focus;

    if (!getParam("study_hours", sh) ||
        !getParam("session_length", sl) ||
        !getParam("break_time", bt) ||
        !getParam("focus", focus)) {

        printHeader();
        cout << "<h2>Error</h2>";
        cout << "<p>Please submit the form from study.html</p>";
        printFooter();
        return 0;
    }

    int hours = stoi(sh);
    int session = stoi(sl);
    int breakTime = stoi(bt);

    int score = calculateScore(hours, session, breakTime, focus);
    string category = studyCategory(score);
    string tips = studyTips(category);
    string date = getCurrentDate();

    int prevScore = -1;
    bool hasPrev = readLastScore(prevScore);

    storeRecord(date, hours, session, breakTime, focus, score, category);

    printHeader();
    cout << "<section class='info-section'>";
    cout << "<h2>Study Cycle Analysis</h2>";
    cout << "<p><b>Date:</b> " << date << "</p>";
    cout << "<p><b>Study Hours:</b> " << hours << "</p>";
    cout << "<p><b>Session Length:</b> " << session << " minutes</p>";
    cout << "<p><b>Break Time:</b> " << breakTime << " minutes</p>";
    cout << "<p><b>Focus Level:</b> " << focus << "</p>";
    cout << "<p><b>Study Score:</b> " << score << " / 100</p>";
    cout << "<p><b>Category:</b> " << category << "</p>";

    cout << "<h3>Recommendations</h3>";
    cout << "<p>" << tips << "</p>";

    if (hasPrev) {
        cout << "<h3>Progress Comparison</h3>";
        cout << "<p>Previous Score: " << prevScore << "</p>";
        cout << "<p>" << compareScore(prevScore, score) << "</p>";
    } else {
        cout << "<p>This is your first study record.</p>";
    }

    cout << "</section>";
    printFooter();
    return 0;
}
