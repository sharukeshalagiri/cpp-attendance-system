#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class LoginFailedException : public exception {
    public:
    const char* what() const noexcept override {
        return "Username or password incorrect. Please contact the admin office.";
    }
};

struct Faculty {
    string username;
    string password;
};

bool login(string uname, string pwd) {
    ifstream fin("faculty_login.txt");
    string u, p;
    while (fin >> u >> p) {
        if (u == uname && p == pwd) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

void registerFaculty() {
    ofstream fout("faculty_login.txt", ios::app);
    Faculty f;
    cout << "Create Username: ";
    cin >> f.username;
    cout << "Create Password: ";
    cin >> f.password;
    fout << f.username << " " << f.password << endl;
    fout.close();
    cout << "Registration successful!\n";
}

class Student {
    public:
    string name;
    int roll, attended;

    void get(int total) {
        cout << "Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Name: ";
        getline(cin, name);
        cout << "Hours Attended: ";
        cin >> attended;

        if (attended > total)
            throw overflow_error("Attended hours exceed total class hours!");
        if (attended < 0)
            throw underflow_error("Attended hours cannot be negative!");
    }

    void show(int total) {
        float perc = (float)attended / total * 100;
        cout << setw(8) << roll << setw(15) << name << setw(18)
             << fixed << setprecision(2) << perc << "%";
        if (perc < 75)
            cout << "  <-- Reappear Required";
        cout << endl;
    }
};

int main() {
    int choice;
    cout << "1. Login\n2. Register\nChoice: ";
    cin >> choice;

    string username, password;

    if (choice == 2) {
        registerFaculty();
        return 0;
    }

    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    try {
        if (!login(username, password)) {
            throw LoginFailedException();
        }
        cout << "\nLogin successful!\n";
    } catch (const LoginFailedException& e) {
        cout << "\nLogin failed: " << e.what() << endl;
        return 1;
    }

    string subject, department, batch;
    cout << "Enter Subject: ";
    cin.ignore();
    getline(cin, subject);
    cout << "Enter Department: ";
    getline(cin, department);
    cout << "Enter Batch: ";
    getline(cin, batch);

    int n, total;
    cout << "Enter number of students: ";
    cin >> n;
    cout << "Total class hours: ";
    cin >> total;

    Student s[n];
    for (int i = 0; i < n; i++) {
        cout << "\nEnter details of Student " << i + 1 << endl;
        try {
            s[i].get(total);
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            i--;
        }
    }

    cout << "\n--- Attendance Report ---\n";
    cout << "Faculty: " << username << "\nSubject: " << subject
         << "\nDepartment: " << department << "\nBatch: " << batch << "\n\n";
    cout << setw(8) << "Roll" << setw(15) << "Name" << setw(18) << "Attendance %" << endl;

    for (int i = 0; i < n; i++)
        s[i].show(total);

    ofstream fout("attendance_report.txt");

    fout << "--- Attendance Report ---\n";
    fout << "Faculty: " << username << "\nSubject: " << subject
         << "\nDepartment: " << department << "\nBatch: " << batch << "\n\n";
    fout << setw(8) << "Roll" << setw(15) << "Name" << setw(18) << "Attendance %" << endl;

    for (int i = 0; i < n; i++) {
        float perc = (float)s[i].attended / total * 100;
        fout << setw(8) << s[i].roll << setw(15) << s[i].name
             << setw(18) << fixed << setprecision(2) << perc << "%";
        if (perc < 75) fout << "  <-- Reappear Required";
        fout << endl;
    }

    fout.close();

    return 0;
}
