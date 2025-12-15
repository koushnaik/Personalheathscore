#include <iostream>
#include <sstream>
#include <cmath>
#include "httplib.h"

using namespace httplib;
using namespace std;

/* ---------------- SLEEP ---------------- */
string sleep_report(double hours)
{
    stringstream o;
    o << "<h3>Sleep Analysis Result</h3>";

    if (hours < 5)
        o << "<p><b>Status:</b> Very Poor Sleep</p>"
          << "<p>Sleeping less than 5 hours severely affects memory, immunity, focus.</p>"
          << "<p><b>Recommendation:</b> Aim for 7–8 hours daily.</p>";
    else if (hours < 7)
        o << "<p><b>Status:</b> Inadequate Sleep</p>"
          << "<p>Your sleep is below optimal.</p>";
    else
        o << "<p><b>Status:</b> Healthy Sleep</p>";

    return o.str();
}

/* ---------------- STUDY ---------------- */
string study_report(double hours)
{
    stringstream o;
    o << "<h3>Study Load Analysis</h3>";

    if (hours > 10)
        o << "<p><b>Status:</b> Extreme Study Load</p>";
    else if (hours > 7)
        o << "<p><b>Status:</b> High Study Load</p>";
    else
        o << "<p><b>Status:</b> Balanced Study Load</p>";

    return o.str();
}

/* ---------------- WATER ---------------- */
string water_report(double intake, double weight)
{
    double required = weight / 30.0;
    stringstream o;

    o << "<h3>Hydration Analysis</h3>"
      << "<p><b>Required:</b> " << required << " L/day</p>";

    if (intake < required)
        o << "<p><b>Status:</b> Dehydrated</p>";
    else
        o << "<p><b>Status:</b> Well Hydrated</p>";

    return o.str();
}

/* ---------------- ACTIVITY ---------------- */
string activity_report(int steps)
{
    stringstream o;
    o << "<h3>Physical Activity</h3>";

    if (steps < 3000)
        o << "<p>Sedentary Lifestyle</p>";
    else if (steps < 7000)
        o << "<p>Moderately Active</p>";
    else
        o << "<p>Active Lifestyle</p>";

    return o.str();
}

/* ---------------- BMI ---------------- */
string bmi_report(double weight, double height_cm)
{
    double h = height_cm / 100.0;
    double bmi = weight / (h * h);

    stringstream o;
    o << "<h3>BMI Analysis</h3><p><b>BMI:</b> " << bmi << "</p>";

    if (bmi < 18.5)
        o << "<p>Underweight</p>";
    else if (bmi < 25)
        o << "<p>Normal</p>";
    else if (bmi < 30)
        o << "<p>Overweight</p>";
    else
        o << "<p>Obese</p>";

    return o.str();
}

/* ---------------- SERVER ---------------- */
int main()
{
    Server svr;

    // ✅ IMPORTANT FIX: use relative path
    svr.set_base_dir("./public");

    // ✅ ROOT FIX: localhost:8080 → index.html
    svr.Get("/", [](const Request &, Response &res) {
        res.set_redirect("/index.html");
    });

    // -------- POST ROUTES --------
    svr.Post("/sleep", [](const Request &req, Response &res) {
        res.set_content(
            sleep_report(stod(req.get_param_value("sleep"))),
            "text/html"
        );
    });

    svr.Post("/study", [](const Request &req, Response &res) {
        res.set_content(
            study_report(stod(req.get_param_value("study"))),
            "text/html"
        );
    });

    svr.Post("/water", [](const Request &req, Response &res) {
        res.set_content(
            water_report(
                stod(req.get_param_value("water")),
                stod(req.get_param_value("weight"))
            ),
            "text/html"
        );
    });

    svr.Post("/activity", [](const Request &req, Response &res) {
        res.set_content(
            activity_report(stoi(req.get_param_value("steps"))),
            "text/html"
        );
    });

    svr.Post("/bmi", [](const Request &req, Response &res) {
        res.set_content(
            bmi_report(
                stod(req.get_param_value("weight")),
                stod(req.get_param_value("height"))
            ),
            "text/html"
        );
    });

    cout << "✅ Server running at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);
}
