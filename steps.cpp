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

bool getIntParam(const string &key, int &value) {
    string v;
    if (!getParam(key, v)) return false;
    value = stoi(v);
    return true;
}

/* =========================================================
   ACTIVITY LEVEL BASED ON STEPS
   ========================================================= */
string activityLevel(int steps) {
    if (steps < 5000)
        return "Sedentary";
    else if (steps < 7500)
        return "Lightly Active";
    else if (steps < 10000)
        return "Active";
    else
        return "Highly Active";
}

/* =========================================================
   CALORIE BURN ESTIMATION
   ========================================================= */
float estimateCalories(int steps, const string &intensity) {
    float calories = steps * 0.04f; // base estimate

    if (intensity == "Moderate")
        calories *= 1.2f;
    else if (intensity == "High")
        calories *= 1.5f;

    return calories;
}

/* =========================================================
   EXERCISE TIPS
   ========================================================= */
string activityTips(const string &level) {
    if (level == "Sedentary") {
        return "Increase daily movement. Start with short walks "
               "and aim for at least 6,000 steps per day.";
    } else if (level == "Lightly Active") {
        return "Good start. Try adding 1,000–2,000 steps daily.";
    } else if (level == "Active") {
        return "You meet daily activity goals. Maintain consistency.";
    } else {
        return "Excellent activity level. Ensure adequate rest and recovery.";
    }
}

/* =========================================================
   STORE RECORD
   ========================================================= */
void storeRecord(const string &date, int steps, const string &exercise,
                 int duration, const string &intensity,
                 float calories, const string &level) {

    ofstream file("data/steps.txt", ios::app);
    file << date << ","
         << steps << ","
         << exercise << ","
         << duration << ","
         << intensity << ","
         << fixed << setprecision(2) << calories << ","
         << level << endl;
    file.close();
}

/* =========================================================
   READ PREVIOUS STEPS
   ========================================================= */
bool readLastSteps(int &lastSteps) {
    ifstream file("data/steps.txt");
    if (!file.is_open()) return false;

    string line, lastLine;
    while (getline(file, line)) {
        if (!line.empty())
            lastLine = line;
    }
    file.close();

    if (lastLine.empty()) return false;

    vector<string> parts = split(lastLine, ',');
    if (parts.size() < 2) return false;

    lastSteps = stoi(parts[1]);
    return true;
}

/* =========================================================
   COMPARISON MESSAGE
   ========================================================= */
string compareSteps(int previous, int current) {
    if (current > previous)
        return "Great improvement! Your step count has increased.";
    else if (current < previous)
        return "Step count decreased. Try to stay more active.";
    else
        return "Step count unchanged. Maintain your routine.";
}

/* =========================================================
   HTML HELPERS
   ========================================================= */
void printHTMLHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Steps & Exercise Report</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printHTMLFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/steps.html'>Back to Steps Tracker</a> | ";
    cout << "<a href='/index.html'>Dashboard</a>";
    cout << "</div>";
    cout << "</body></html>";
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */
int main() {
    int steps = 0;
    int duration = 0;
    string exercise, intensity;

    getIntParam("steps", steps);
    getParam("exercise", exercise);
    getIntParam("duration", duration);
    getParam("intensity", intensity);

    printHTMLHeader();
    cout << "<section class='info-section'>";

    if (steps < 0 || duration < 0) {
        cout << "<h2>Error</h2>";
        cout << "<p>Invalid input values.</p>";
        cout << "</section>";
        printHTMLFooter();
        return 0;
    }

    string level = activityLevel(steps);
    float calories = estimateCalories(steps, intensity);
    string tips = activityTips(level);
    string date = getCurrentDate();

    int previousSteps = -1;
    bool hasPrevious = readLastSteps(previousSteps);

    storeRecord(date, steps, exercise, duration, intensity, calories, level);

    cout << "<h2>Steps & Exercise Analysis Report</h2>";
    cout << "<p><b>Date:</b> " << date << "</p>";
    cout << "<p><b>Total Steps:</b> " << steps << "</p>";
    cout << "<p><b>Exercise Type:</b> " << exercise << "</p>";
    cout << "<p><b>Duration:</b> " << duration << " minutes</p>";
    cout << "<p><b>Intensity:</b> " << intensity << "</p>";
    cout << "<p><b>Estimated Calories Burned:</b> "
         << fixed << setprecision(2) << calories << " kcal</p>";
    cout << "<p><b>Activity Level:</b> " << level << "</p>";

    cout << "<h3>Activity Recommendations</h3>";
    cout << "<p>" << tips << "</p>";

    if (hasPrevious) {
        cout << "<h3>Progress Comparison</h3>";
        cout << "<p>Previous Steps: " << previousSteps << "</p>";
        cout << "<p>" << compareSteps(previousSteps, steps) << "</p>";
    } else {
        cout << "<p>This is your first activity record.</p>";
    }

    cout << "</section>";
    printHTMLFooter();
    return 0;
}
