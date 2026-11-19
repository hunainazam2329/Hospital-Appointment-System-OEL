
#include "AppointmentAlgorithms.h"
#include <iostream>
#include <utility>
using namespace std;

void AppointmentAlgorithms::sortById(
    AppointmentQueue& appointments)
{
    Appointment* temp = appointments.getFront();

    if (temp == nullptr)
    {
        cout << "\nAppointment queue is empty!\n";
        return;
    }

    int count = 0;

    while (temp != nullptr)
    {
        count++;
        temp = temp->next;
    }

    if (count < 2)
    {
        cout << "\nNot enough appointments to sort.\n";
        return;
    }

    // Temporary array of appointment pointers
    Appointment** arr = new Appointment*[count];

    temp = appointments.getFront();

    for (int i = 0; i < count; i++)
    {
        arr[i] = temp;
        temp = temp->next;
    }

    // Bubble Sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (arr[j]->appId > arr[j + 1]->appId)
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }

    cout << "\nAppointments sorted by ID:\n";

    for (int i = 0; i < count; i++)
    {
        cout << "\nAppointment ID: " << arr[i]->appId;
        cout << "\nPatient ID: " << arr[i]->patientId;
        cout << "\nDate: " << arr[i]->date;
        cout << "\nStatus: ";

        if (arr[i]->status == 1)
            cout << "Scheduled";
        else if (arr[i]->status == 2)
            cout << "Completed";
        else
            cout << "Cancelled";

        cout << "\n-----------------------------\n";
    }

    delete[] arr;
}
