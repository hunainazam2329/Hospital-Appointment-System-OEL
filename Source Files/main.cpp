#include <ctime>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "PatientLinkedList.h"
#include "PatientAlgorithms.h"
#include "AppointmentQueue.h"
#include "AppointmentAlgorithms.h"
#include "FileManager.h"

// Read integer within a specified range
int readIntInRange(string message, int min, int max)
{
    string input;
    int number;
    char extra;

    while (true)
    {
        cout << message;
        getline(cin, input);

        stringstream ss(input);

        if (ss >> number && !(ss >> extra) &&
            number >= min && number <= max)
        {
            return number;
        }

        cout << "Invalid input! Enter a value from "
             << min << " to " << max << ".\n";
    }
}

// Read non-empty text
string readText(string message)
{
    string value;

    while (true)
    {
        cout << message;
        getline(cin, value);

        if (value.empty() ||
            value.find_first_not_of(" \t") == string::npos)
        {
            cout << "Input cannot be empty!\n";
        }
        else if (value.find('|') != string::npos)
        {
            cout << "The | character is not allowed!\n";
        }
        else
        {
            return value;
        }
    }
}

// Validate actual calendar date
// Validate actual calendar date and reject past dates
bool isValidDate(string date)
{
    if (date.length() != 10 ||
        date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;

        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12)
        return false;

    int days[] =
    {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool leap =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (leap)
        days[1] = 29;

    if (day < 1 || day > days[month - 1])
        return false;

    // Get today's date from the computer
    time_t now = time(nullptr);
    tm* currentTime = localtime(&now);

    if (currentTime == nullptr)
        return false;

    char today[11];

    snprintf(today, sizeof(today), "%04d-%02d-%02d",
        currentTime->tm_year + 1900,
        currentTime->tm_mon + 1,
        currentTime->tm_mday);

    // Reject dates earlier than today
    return date >= string(today);
}

// Read date from user
// Read appointment date from user
string readDate()
{
    string date;

    while (true)
    {
        cout << "Enter appointment date (YYYY-MM-DD): ";
        getline(cin, date);

        if (isValidDate(date))
            return date;

        cout << "Invalid date! Enter a valid date today or in the future.\n";
    }
}
// Register patient
void registerPatient(PatientList& patients)
{
    cout << "\n========== PATIENT REGISTRATION ==========\n";

    int id = readIntInRange("Enter Patient ID: ", 1, 999999);
    string name = readText("Enter Patient Name: ");
    int age = readIntInRange("Enter Age (1-120): ", 1, 120);
    string disease = readText("Enter Disease: ");

    if (patients.addPatient(id, name, age, disease))
        FileManager::savePatients(patients);
}

// Update patient
void updatePatientRecord(PatientList& patients)
{
    cout << "\n========== UPDATE PATIENT ==========\n";

    int id = readIntInRange("Enter Patient ID: ", 1, 999999);

    if (patients.searchById(id) == nullptr)
    {
        cout << "\nPatient not found!\n";
        return;
    }

    string name = readText("Enter New Name: ");
    int age = readIntInRange("Enter New Age (1-120): ", 1, 120);
    string disease = readText("Enter New Disease: ");

    if (patients.updatePatient(id, name, age, disease))
        FileManager::savePatients(patients);
}

// Delete patient
void deletePatientRecord(
    PatientList& patients,
    AppointmentQueue& appointments)
{
    cout << "\n========== DELETE PATIENT ==========\n";

    int id = readIntInRange("Enter Patient ID: ", 1, 999999);

    if (patients.searchById(id) == nullptr)
    {
        cout << "\nPatient not found!\n";
        return;
    }

    // Prevent broken appointment references
    if (appointments.hasPatientAppointments(id))
    {
        cout << "\nCannot delete patient because appointment records exist!\n";
        return;
    }

    if (patients.deletePatient(id))
        FileManager::savePatients(patients);
}

// Linear search
void linearSearchPatient(PatientList& patients)
{
    int id = readIntInRange(
        "Enter Patient ID to search: ", 1, 999999);

    Patient* result = patients.searchById(id);

    if (result == nullptr)
    {
        cout << "\nPatient not found!\n";
    }
    else
    {
        cout << "\nPatient Found!\n";
        cout << "ID: " << result->id << endl;
        cout << "Name: " << result->name << endl;
        cout << "Age: " << result->age << endl;
        cout << "Disease: " << result->disease << endl;
    }
}

// Schedule appointment
void scheduleAppointment(
    AppointmentQueue& appointments,
    PatientList& patients)
{
    cout << "\n========== SCHEDULE APPOINTMENT ==========\n";

    int appId = readIntInRange(
        "Enter Appointment ID: ", 1, 999999);

    int patientId = readIntInRange(
        "Enter Patient ID: ", 1, 999999);

    string date = readDate();

    if (appointments.addAppointment(
        appId, patientId, date, patients))
    {
        FileManager::saveAppointments(appointments);
    }
}

// Cancel appointment
void cancelAppointmentRecord(AppointmentQueue& appointments)
{
    int id = readIntInRange(
        "Enter Appointment ID to cancel: ", 1, 999999);

    if (appointments.cancelAppointment(id))
        FileManager::saveAppointments(appointments);
}

// Complete next scheduled appointment
void completeAppointment(AppointmentQueue& appointments)
{
    if (appointments.completeNext())
        FileManager::saveAppointments(appointments);
}

// Main function
int main()
{
    PatientList patients;
    AppointmentQueue appointments;

    cout << "\n========================================\n";
    cout << " HOSPITAL APPOINTMENT MANAGEMENT SYSTEM\n";
    cout << "========================================\n";

    // Load patients first
    FileManager::loadPatients(patients);

    // Then load appointments
    FileManager::loadAppointments(appointments, patients);

    int choice;

    do
    {
        cout << "\n\n========== MAIN MENU ==========\n";

        cout << "\n--- PATIENT MANAGEMENT ---\n";
        cout << "1. Register Patient\n";
        cout << "2. Update Patient\n";
        cout << "3. Delete Patient\n";
        cout << "4. Search Patient (Linear Search)\n";
        cout << "5. Search Patient (Binary Search)\n";
        cout << "6. Display All Patients\n";
        cout << "7. Sort Patients by ID\n";

        cout << "\n--- APPOINTMENT MANAGEMENT ---\n";
        cout << "8. Schedule Appointment\n";
        cout << "9. Complete Next Appointment\n";
        cout << "10. Cancel Appointment\n";
        cout << "11. Display All Appointments\n";
        cout << "12. Sort Appointments by ID\n";

        cout << "\n--- FILE MANAGEMENT ---\n";
        cout << "13. Save All Records\n";

        cout << "\n0. Exit\n";

        choice = readIntInRange("\nEnter your choice: ", 0, 13);

        switch (choice)
        {
        case 1:
            registerPatient(patients);
            break;

        case 2:
            updatePatientRecord(patients);
            break;

        case 3:
            deletePatientRecord(patients, appointments);
            break;

        case 4:
            linearSearchPatient(patients);
            break;

        case 5:
        {
            int id = readIntInRange(
                "Enter Patient ID to search: ", 1, 999999);

            PatientAlgorithms::binarySearchId(patients, id);
            break;
        }

        case 6:
            patients.displayAll();
            break;

        case 7:
            PatientAlgorithms::sortById(patients);
            FileManager::savePatients(patients);
            break;

        case 8:
            scheduleAppointment(appointments, patients);
            break;

        case 9:
            completeAppointment(appointments);
            break;

        case 10:
            cancelAppointmentRecord(appointments);
            break;

        case 11:
            appointments.displayAll();
            break;

        case 12:
            AppointmentAlgorithms::sortById(appointments);
            break;

        case 13:
            FileManager::savePatients(patients);
            FileManager::saveAppointments(appointments);
            break;

        case 0:
            FileManager::savePatients(patients);
            FileManager::saveAppointments(appointments);

            cout << "\nThank you for using the Hospital Management System!\n";
            break;
        }

    } while (choice != 0);

    return 0;
}
