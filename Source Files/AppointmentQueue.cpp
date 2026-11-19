
#include "AppointmentQueue.h"

// Constructor
AppointmentQueue::AppointmentQueue()
{
    front = nullptr;
    rear = nullptr;
}

// Destructor
AppointmentQueue::~AppointmentQueue()
{
    Appointment* temp;

    while (front != nullptr)
    {
        temp = front;
        front = front->next;
        delete temp;
    }

    rear = nullptr;
}

// Check whether queue is empty
bool AppointmentQueue::isEmpty()
{
    return front == nullptr;
}

// Search appointment by ID
Appointment* AppointmentQueue::findById(int appId)
{
    Appointment* temp = front;

    while (temp != nullptr)
    {
        if (temp->appId == appId)
            return temp;

        temp = temp->next;
    }

    return nullptr;
}

// Add appointment at rear
bool AppointmentQueue::addAppointment(
    int appId, int patientId, string date,
    PatientList& patients, int status)
{
    if (patients.searchById(patientId) == nullptr)
    {
        cout << "\nError: Patient does not exist!\n";
        return false;
    }

    if (findById(appId) != nullptr)
    {
        cout << "\nDuplicate Appointment ID!\n";
        return false;
    }

    if (status < 1 || status > 3)
    {
        cout << "\nInvalid appointment status!\n";
        return false;
    }

    Appointment* newAppointment =
        new Appointment(appId, patientId, date, status);

    if (front == nullptr)
    {
        front = newAppointment;
        rear = newAppointment;
    }
    else
    {
        rear->next = newAppointment;
        rear = newAppointment;
    }

    cout << "\nAppointment added successfully!\n";
    return true;
}

// Complete the first scheduled appointment
bool AppointmentQueue::completeNext()
{
    if (front == nullptr)
    {
        cout << "\nAppointment queue is empty!\n";
        return false;
    }

    Appointment* temp = front;

    while (temp != nullptr && temp->status != 1)
        temp = temp->next;

    if (temp == nullptr)
    {
        cout << "\nNo scheduled appointments available!\n";
        return false;
    }

    temp->status = 2;

    cout << "\nAppointment completed successfully!\n";
    cout << "Appointment ID: " << temp->appId << endl;

    return true;
}

// Cancel appointment by ID
bool AppointmentQueue::cancelAppointment(int appId)
{
    Appointment* temp = findById(appId);

    if (temp == nullptr)
    {
        cout << "\nAppointment not found!\n";
        return false;
    }

    if (temp->status != 1)
    {
        cout << "\nOnly scheduled appointments can be cancelled!\n";
        return false;
    }

    temp->status = 3;

    cout << "\nAppointment cancelled successfully!\n";
    return true;
}

// Prevent deleting patients who have appointment records
bool AppointmentQueue::hasPatientAppointments(int patientId)
{
    Appointment* temp = front;

    while (temp != nullptr)
    {
        if (temp->patientId == patientId)
            return true;

        temp = temp->next;
    }

    return false;
}

// Display all appointments
void AppointmentQueue::displayAll()
{
    if (front == nullptr)
    {
        cout << "\nAppointment queue is empty!\n";
        return;
    }

    Appointment* temp = front;

    cout << "\n========== APPOINTMENT RECORDS ==========\n";

    while (temp != nullptr)
    {
        cout << "\nAppointment ID: " << temp->appId;
        cout << "\nPatient ID: " << temp->patientId;
        cout << "\nDate: " << temp->date;
        cout << "\nStatus: ";

        if (temp->status == 1)
            cout << "Scheduled";
        else if (temp->status == 2)
            cout << "Completed";
        else
            cout << "Cancelled";

        cout << "\n-----------------------------------------\n";

        temp = temp->next;
    }
}

// Return front pointer for other modules
Appointment* AppointmentQueue::getFront()
{
    return front;
}
