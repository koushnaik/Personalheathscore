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
   INPUT PARSING
   ========================================================= */
bool getFloatParam(const string &key, float &value) {
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
                value = stof(v);
                return true;
            }
        }
    }
    return false;
}

/* =========================================================
   BMI CALCULATION
   ========================================================= */
float calculateBMI(float weight, float heightCm) {
    float heightM = heightCm / 100.0f;
    return weight / (heightM * heightM);
}

/* =========================================================
   BMI CATEGORY
   ========================================================= */
string getBMICategory(float bmi) {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25.0) return "Normal";
    if (bmi < 30.0) return "Overweight";
    return "Obese";
}

/* =========================================================
   HEALTH TIPS
   ========================================================= */
string getHealthTips(const string &category) {
    if (category == "Underweight") {
        return "Increase calorie intake, include protein-rich foods, "
               "and consider strength training exercises.";
    } else if (category == "Normal") {
        return "Maintain a balanced diet, stay physically active, "
               "and continue monitoring your health regularly.";
    } else if (category == "Overweight") {
        return "Reduce sugar and processed food intake, increase daily "
               "physical activity, and maintain portion control.";
    } else {
        return "Consult a healthcare professional, adopt a structured "
               "weight management plan, and focus on lifestyle changes.";
    }
}

/* =========================================================
   FILE HANDLING: STORE RECORD
   ========================================================= */
void storeRecord(const string &date, float weight, float height,
                 float bmi, const string &category) {
    ofstream file("data/bmi.txt", ios::app);
    file << date << ","
         << weight << ","
         << height << ","
         << fixed << setprecision(2) << bmi << ","
         << category << endl;
    file.close();
}

/* =========================================================
   FILE HANDLING: READ LAST BMI
   ========================================================= */
bool readLastBMI(float &lastBMI) {
    ifstream file("data/bmi.txt");
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

    lastBMI = stof(parts[3]);
    return true;
}

/* =========================================================
   COMPARISON MESSAGE
   ========================================================= */
string compareBMI(float previous, float current) {
    if (current < previous)
        return "Positive progress! Your BMI has decreased compared to the previous record.";
    else if (current > previous)
        return "Your BMI has increased. Consider improving diet and activity levels.";
    else
        return "Your BMI remains unchanged. Maintain consistency in your habits.";
}

/* =========================================================
   HTML OUTPUT HELPERS
   ========================================================= */
void printHTMLHeader() {
    cout << "Content-Type: text/html\n\n";
    cout << "<html><head><title>BMI Result</title>";
    cout << "<link rel='stylesheet' href='/style.css'>";
    cout << "</head><body>";
}

void printHTMLFooter() {
    cout << "<br><div style='text-align:center;'>";
    cout << "<a href='/bmi.html'>Back to BMI Calculator</a>";
    cout << " | ";
    cout << "<a href='/index.html'>Dashboard</a>";
    cout << "</div>";
    cout << "</body></html>";
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */
int main() {
    float weight = 0.0f, height = 0.0f;

    getFloatParam("weight", weight);
    getFloatParam("height", height);

    printHTMLHeader();
    cout << "<section class='info-section'>";

    if (weight <= 0 || height <= 0) {
        cout << "<h2>Error</h2>";
        cout << "<p>Invalid input values. Please enter positive numbers.</p>";
        cout << "</section>";
        printHTMLFooter();
        return 0;
    }

    float bmi = calculateBMI(weight, height);
    string category = getBMICategory(bmi);
    string tips = getHealthTips(category);
    string date = getCurrentDate();

    float previousBMI = -1.0f;
    bool hasPrevious = readLastBMI(previousBMI);

    storeRecord(date, weight, height, bmi, category);

    cout << "<h2>BMI Analysis Report</h2>";
    cout << "<p><b>Date:</b> " << date << "</p>";
    cout << "<p><b>Weight:</b> " << weight << " kg</p>";
    cout << "<p><b>Height:</b> " << height << " m</p>";
    cout << "<p><b>BMI Value:</b> " << fixed << setprecision(2) << bmi << "</p>";
    cout << "<p><b>Category:</b> " << category << "</p>";

    cout << "<h3>Health Recommendations</h3>";
    cout << "<p>" << tips << "</p>";

    if (hasPrevious) {
        cout << "<h3>Progress Comparison</h3>";
        cout << "<p>Previous BMI: " << previousBMI << "</p>";
        cout << "<p>" << compareBMI(previousBMI, bmi) << "</p>";
    } else {
        cout << "<p>This is your first BMI record.</p>";
    }

    cout << "</section>";
    printHTMLFooter();

    return 0;
}
