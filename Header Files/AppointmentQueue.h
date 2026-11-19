
#ifndef APPOINTMENT_QUEUE_H
#define APPOINTMENT_QUEUE_H

#include "PatientLinkedList.h"
#include <string>
using namespace std;

struct Appointment
{
    int appId;
    int patientId;
    string date;
    int status;
    Appointment* next;

    // Constructor using initialization list
    Appointment(int aId, int pId, string aDate, int aStatus)
        : appId(aId), patientId(pId), date(aDate),
          status(aStatus), next(nullptr)
    {
    }
};

class AppointmentQueue
{
private:
    Appointment* front;
    Appointment* rear;

public:
    AppointmentQueue();
    ~AppointmentQueue();

    bool isEmpty();

    bool addAppointment(
        int appId, int patientId, string date,
        PatientList& patients, int status = 1);

    bool completeNext();

    bool cancelAppointment(int appId);

    Appointment* findById(int appId);

    bool hasPatientAppointments(int patientId);

    void displayAll();

    Appointment* getFront();
};

#endif
