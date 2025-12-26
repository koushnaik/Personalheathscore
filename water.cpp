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
   CALCULATE RECOMMENDED WATER (ml)
   ========================================================= */
float calculateRecommendedWater(float weight, string activity, string climate) {
    // Base formula: 35 ml per kg
    float water = weight * 35.0f;

    if (activity == "Moderate")
        water += 300;
    else if (activity == "High")
        water += 600;

    if (climate == "Hot")
        water += 500;

    return water;
}

/* =========================================================
   HYDRATION STATUS
   ========================================================= */
string hydrationStatus(float consumed, float recommended) {
    if (consumed < recommended * 0.75)
        return "Dehydrated";
    if (consumed < recommended)
        return "Needs Improvement";
    return "Well Hydrated";
}

/* =========================================================
   HYDRATION TIPS
   ========================================================= */
string hydrationTips(string status) {
    if (status == "Dehydrated") {
        return "Increase water intake immediately. Drink small amounts "
               "frequently and include hydrating foods.";
    } else if (status == "Needs Improvement") {
        return "Try adding 1–2 extra glasses spread across the day.";
    } else {
        return "Great hydration habits. Maintain consistent water intake.";
    }
}

/* =========================================================
   STORE RECORD
   ========================================================= */
void storeRecord(const string &date, float weight, int glasses,
                 float consumed, float recommended,
                 const string &status) {

    ofstream file("data/water.txt", ios::app);
    file << date << ","
         << weight << ","
         << glasses << ","
         << fixed << setprecision(2) << consumed << ","
         << recommended << ","
         << status << endl;
    file.close();
}

/* =========================================================
   READ LAST CONSUMPTION
   ========================================================= */
bool readLastConsumed(float &lastConsumed) {
    ifstream file("data/water.txt");
    if (!file.is_open()) return false;

    string line, lastLine;
    while (getline(file, line)) {
        if (!line.empty())
            lastLine = line;
    }
    file.close();

    if (lastLine.empty()) return false;

    vector<string> parts = split(lastLine, ',');
    if (parts.size() < 4) return false;

    lastConsumed = stof(parts[3]);
    return true;
}

/* =========================================================
   COMPARISON MESSAGE
   ========================================================= */
string compareWater(float previous, float current) {
    if (current > previous)
        return "Good improvement! You are drinking more water than before.";
    else if (current < previous)
        return "Water intake has decreased. Focus on hydration.";
    else
        return "Water intake unchanged. Maintain consistency.";
}

/* =========================================================
   HTML HELPERS
   ========================================================= */
void printHTMLHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>Water Intake Result</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printHTMLFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/water.html'>Back to Water Tracker</a> | ";
    cout << "<a href='/index.html'>Dashboard</a>";
    cout << "</div>";
    cout << "</body></html>";
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */
int main() {
    float weight = 0.0f;
    int glasses = 0;
    string activity, climate;

    getFloatParam("weight", weight);

 string g;
if (!getParam("glasses", g)) {
    cout << "Content-Type: text/html\n\n";
    cout << "<h2>Error</h2>";
    cout << "<p>Please submit the form from water.html</p>";
    return 0;
}
glasses = stoi(g);


    getParam("activity", activity);
    getParam("climate", climate);

    printHTMLHeader();
    cout << "<section class='info-section'>";

    if (weight <= 0 || glasses < 0) {
        cout << "<h2>Error</h2>";
        cout << "<p>Invalid input values.</p>";
        cout << "</section>";
        printHTMLFooter();
        return 0;
    }

    float consumed = glasses * 250.0f; // ml
    float recommended = calculateRecommendedWater(weight, activity, climate);
    string status = hydrationStatus(consumed, recommended);
    string tips = hydrationTips(status);
    string date = getCurrentDate();

    float previousConsumed = -1.0f;
    bool hasPrevious = readLastConsumed(previousConsumed);

    storeRecord(date, weight, glasses, consumed, recommended, status);

    cout << "<h2>Daily Hydration Report</h2>";
    cout << "<p><b>Date:</b> " << date << "</p>";
    cout << "<p><b>Body Weight:</b> " << weight << " kg</p>";
    cout << "<p><b>Water Consumed:</b> " << consumed << " ml</p>";
    cout << "<p><b>Recommended Intake:</b> "
         << fixed << setprecision(0) << recommended << " ml</p>";
    cout << "<p><b>Hydration Status:</b> " << status << "</p>";

    cout << "<h3>Hydration Tips</h3>";
    cout << "<p>" << tips << "</p>";

    if (hasPrevious) {
        cout << "<h3>Progress Comparison</h3>";
        cout << "<p>Previous Intake: " << previousConsumed << " ml</p>";
        cout << "<p>" << compareWater(previousConsumed, consumed) << "</p>";
    } else {
        cout << "<p>This is your first hydration record.</p>";
    }

    cout << "</section>";
    printHTMLFooter();
    return 0;
}
