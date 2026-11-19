#include <iostream>
#include <string>
#include <cctype>
#include "patient.h"
#include "appointment.h"
using namespace std;

int readIntInRange(string prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
        } else {
            cin.ignore(1000, '\n');
            if (val < minVal || val > maxVal) {
                cout << "Invalid input! Enter a value between " << minVal << " and " << maxVal << ".\n";
            } else {
                return val;
            }
        }
    }
}

string readDate() {
    string date;
    while (true) {
        cout << "Enter Date (YYYY-MM-DD): ";
        getline(cin, date);
        bool valid = (date.length() == 10 && date[4] == '-' && date[7] == '-');
        if (valid) {
            for (int i = 0; i < 10; i++) {
                if (i == 4 || i == 7) continue;
                if (!isdigit(static_cast<unsigned char>(date[i]))) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            int month = stoi(date.substr(5, 2));
            int day = stoi(date.substr(8, 2));
            if (month < 1 || month > 12 || day < 1 || day > 31) {
                valid = false;
            }
        }
        if (valid) return date;
        cout << "Invalid date! Use format YYYY-MM-DD (e.g., 2026-11-20).\n";
    }
}

int main() {
    PatientList patients;
    AppointmentQueue appointments;

    patients.loadFromFile("patients.txt");
    appointments.loadFromFile("appointments.txt");

    int choice;
    do {
        cout << "\n=== Hospital Appointment & Patient System ===\n";
        cout << "1. Register Patient\n";
        cout << "2. Update Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. Search Patient (Linear / Binary)\n";
        cout << "5. Display All Patients\n";
        cout << "6. Sort Patients by ID\n";
        cout << "7. Schedule Appointment\n";
        cout << "8. Complete Next Appointment\n";
        cout << "9. Cancel Appointment\n";
        cout << "10. Display All Appointments\n";
        cout << "11. Sort Appointments by ID\n";
        cout << "0. Save & Exit\n";

        choice = readIntInRange("Enter choice: ", 0, 11);

        if (choice == 1) {
            int id = readIntInRange("Enter Patient ID: ", 1, 100000);
            string name, disease;
            cout << "Enter Name: "; getline(cin, name);
            int age = readIntInRange("Enter Age: ", 0, 130);
            cout << "Enter Disease: "; getline(cin, disease);
            patients.addPatient(id, name, age, disease);
            patients.saveToFile("patients.txt");
        }
        else if (choice == 2) {
            if (patients.isEmpty()) {
                cout << "No patients in list.\n";
                continue;
            }
            int id = readIntInRange("Enter Patient ID to update: ", 1, 100000);
            if (patients.searchById(id) == nullptr) {
                cout << "Patient ID not found.\n";
                continue;
            }
            string name, disease;
            cout << "Enter New Name: "; getline(cin, name);
            int age = readIntInRange("Enter New Age: ", 0, 130);
            cout << "Enter New Disease: "; getline(cin, disease);
            patients.updatePatient(id, name, age, disease);
            patients.saveToFile("patients.txt");
        }
        else if (choice == 3) {
            int id = readIntInRange("Enter Patient ID to delete: ", 1, 100000);
            patients.deletePatient(id);
            patients.saveToFile("patients.txt");
        }
        else if (choice == 4) {
            int type = readIntInRange("1. Linear Search\n2. Binary Search\nChoice: ", 1, 2);
            int id = readIntInRange("Enter Patient ID: ", 1, 100000);
            if (type == 1) {
                Patient* p = patients.searchById(id);
                if (p != nullptr)
                    cout << "Found -> ID: " << p->id << ", Name: " << p->name
                         << ", Age: " << p->age << ", Disease: " << p->disease << "\n";
                else
                    cout << "Patient not found.\n";
            } else {
                patients.binarySearchId(id);
            }
        }
        else if (choice == 5) {
            patients.displayAll();
        }
        else if (choice == 6) {
            patients.sortById();
            patients.saveToFile("patients.txt");
        }
        else if (choice == 7) {
            if (patients.isEmpty()) {
                cout << "No patients registered! Register a patient first.\n";
                continue;
            }
            int appId = readIntInRange("Enter Appointment ID: ", 1, 100000);
            int pId = readIntInRange("Enter Patient ID: ", 1, 100000);
            string date = readDate();
            appointments.addAppointment(appId, pId, date, patients);
            appointments.saveToFile("appointments.txt");
        }
        else if (choice == 8) {
            appointments.completeNext();
            appointments.saveToFile("appointments.txt");
        }
        else if (choice == 9) {
            if (appointments.isEmpty()) {
                cout << "Appointment queue is empty!\n";
                continue;
            }
            int appId = readIntInRange("Enter Appointment ID to cancel: ", 1, 100000);
            appointments.cancelAppointment(appId);
            appointments.saveToFile("appointments.txt");
        }
        else if (choice == 10) {
            appointments.displayAll();
        }
        else if (choice == 11) {
            appointments.sortById();
            appointments.saveToFile("appointments.txt");
        }
        else if (choice == 0) {
            patients.saveToFile("patients.txt");
            appointments.saveToFile("appointments.txt");
            cout << "Saved and exited.\n";
        }

    } while (choice != 0);

    return 0;
}