
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

// Add appointment according to date
bool AppointmentQueue::addAppointment(
    int appId, int patientId, string date,
    PatientList& patients, int status)
{
    // Check whether patient exists
    if (patients.searchById(patientId) == nullptr)
    {
        cout << "\nError: Patient does not exist!\n";
        return false;
    }

    // Prevent duplicate appointment IDs
    if (findById(appId) != nullptr)
    {
        cout << "\nDuplicate Appointment ID!\n";
        return false;
    }

    // Validate appointment status
    if (status < 1 || status > 3)
    {
        cout << "\nInvalid appointment status!\n";
        return false;
    }

    // Create new appointment
    Appointment* newAppointment =
        new Appointment(appId, patientId, date, status);

    // Case 1: Queue is empty
    // OR new appointment has an earlier date
    if (front == nullptr || date < front->date)
    {
        newAppointment->next = front;
        front = newAppointment;

        // If queue was empty, update rear too
        if (rear == nullptr)
            rear = newAppointment;
    }
    else
    {
        Appointment* temp = front;

        // Find correct position according to date
        // <= maintains insertion order for same dates
        while (temp->next != nullptr &&
               temp->next->date <= date)
        {
            temp = temp->next;
        }

        // Insert appointment at correct position
        newAppointment->next = temp->next;
        temp->next = newAppointment;

        // Update rear if inserted at the end
        if (newAppointment->next == nullptr)
            rear = newAppointment;
    }

    cout << "\nAppointment added successfully!\n";
    return true;
}

// Complete the earliest scheduled appointment
bool AppointmentQueue::completeNext()
{
    if (front == nullptr)
    {
        cout << "\nAppointment queue is empty!\n";
        return false;
    }

    Appointment* temp = front;

    // Find first scheduled appointment
    while (temp != nullptr && temp->status != 1)
    {
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        cout << "\nNo scheduled appointments available!\n";
        return false;
    }

    // Change status to completed
    temp->status = 2;

    cout << "\nAppointment completed successfully!\n";
    cout << "Appointment ID: " << temp->appId << endl;
    cout << "Appointment Date: " << temp->date << endl;

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
