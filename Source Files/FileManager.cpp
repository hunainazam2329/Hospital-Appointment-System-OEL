
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// Convert text into an integer safely
bool parseInteger(string value, int& number)
{
    stringstream ss(value);
    char extra;

    if (!(ss >> number))
        return false;

    if (ss >> extra)
        return false;

    return true;
}

// Validate stored calendar date
bool validStoredDate(string date)
{
    if (date.length() != 10 ||
        date[4] != '-' || date[7] != '-')
        return false;

    int year, month, day;

    if (!parseInteger(date.substr(0, 4), year) ||
        !parseInteger(date.substr(5, 2), month) ||
        !parseInteger(date.substr(8, 2), day))
        return false;

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

    return day >= 1 && day <= days[month - 1];
}

// Save patients
void FileManager::savePatients(PatientList& patients)
{
    ofstream file("patients.txt");

    if (!file)
    {
        cout << "\nUnable to save patient records!\n";
        return;
    }

    Patient* temp = patients.getHead();

    while (temp != nullptr)
    {
        file << temp->id << "|"
             << temp->name << "|"
             << temp->age << "|"
             << temp->disease << "\n";

        temp = temp->next;
    }

    file.close();
    cout << "\nPatient records saved successfully!\n";
}

// Load patients
void FileManager::loadPatients(PatientList& patients)
{
    ifstream file("patients.txt");

    if (!file)
    {
        cout << "\nNo previous patient file found. Starting fresh.\n";
        return;
    }

    string line;
    int loaded = 0;

    while (getline(file, line))
    {
        stringstream ss(line);

        string idText, name, ageText, disease;
        int id, age;

        if (!getline(ss, idText, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, ageText, '|') ||
            !getline(ss, disease))
            continue;

        if (!parseInteger(idText, id) ||
            !parseInteger(ageText, age))
            continue;

        if (id <= 0 || age < 1 || age > 120 ||
            name.empty() || disease.empty())
            continue;

        if (patients.addPatient(id, name, age, disease))
            loaded++;
    }

    file.close();

    cout << "\nLoaded " << loaded << " patient record(s).\n";
}

// Save appointments
void FileManager::saveAppointments(
    AppointmentQueue& appointments)
{
    ofstream file("appointments.txt");

    if (!file)
    {
        cout << "\nUnable to save appointment records!\n";
        return;
    }

    Appointment* temp = appointments.getFront();

    while (temp != nullptr)
    {
        file << temp->appId << "|"
             << temp->patientId << "|"
             << temp->date << "|"
             << temp->status << "\n";

        temp = temp->next;
    }

    file.close();
    cout << "\nAppointment records saved successfully!\n";
}

// Load appointments
void FileManager::loadAppointments(
    AppointmentQueue& appointments,
    PatientList& patients)
{
    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "\nNo previous appointment file found. Starting fresh.\n";
        return;
    }

    string line;
    int loaded = 0;

    while (getline(file, line))
    {
        stringstream ss(line);

        string appText, patientText, date, statusText;
        int appId, patientId, status;

        if (!getline(ss, appText, '|') ||
            !getline(ss, patientText, '|') ||
            !getline(ss, date, '|') ||
            !getline(ss, statusText))
            continue;

        if (!parseInteger(appText, appId) ||
            !parseInteger(patientText, patientId) ||
            !parseInteger(statusText, status))
            continue;

        if (appId <= 0 || patientId <= 0 ||
            status < 1 || status > 3 ||
            !validStoredDate(date))
            continue;

        if (appointments.addAppointment(
            appId, patientId, date, patients, status))
            loaded++;
    }

    file.close();

    cout << "\nLoaded " << loaded << " appointment record(s).\n";
}
