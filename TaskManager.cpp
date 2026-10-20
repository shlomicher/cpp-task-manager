
#include "TaskManager.h"

using std::endl;
using std::cout;

namespace {
    class PriorityBumper {
    private:
        TaskType type;
        int priority;
    public:
        PriorityBumper(TaskType type, int priority) : type(type), priority(priority) {}

        Task operator()(const Task& task) const {
            if (task.getType() == this -> type) {
                const int currentPriority = task.getPriority();
                const int newPriority = priority >= 100 - currentPriority
                    ? 100 : currentPriority + priority;
                Task result(newPriority, task.getType(), task.getDescription());
                result.setId(task.getId());
                return result;
            }
            return task;
        }
    };
}


TaskManager::TaskManager():nextTaskId(0), employeeCount(0){}

void TaskManager::assignTask(const string &personName, const Task &task) {
    Task newTask = task;
    newTask.setId(nextTaskId);

    for (int i = 0; i < employeeCount; i++) {
        if (personName == employees[i].getName()) {
            employees[i].assignTask(newTask);
            this->nextTaskId++;
            return;
        }
    }
        if (employeeCount < MAX_PERSONS) {
            employees[employeeCount] = Person(personName);
            employees[employeeCount].assignTask(newTask);
            employeeCount++;
            this->nextTaskId++;
            return;
    }
    throw std::runtime_error("employees list is full");
}

void TaskManager::completeTask(const string &personName) {
    for (int i = 0; i < employeeCount; i++) {
        if (personName == employees[i].getName()) {
            employees[i].completeTask();
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    if (priority <= 0) { return; }
    PriorityBumper bumper(type, priority);
    for (int i = 0; i < employeeCount; i++) {
        employees[i].setTasks(employees[i].getTasks().apply(bumper));
    }
}



void TaskManager::printAllEmployees() const {
    for (int i = 0; i < employeeCount; i++) {
        cout << employees[i] << endl;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> tasks;
    for (int i = 0; i < employeeCount; i++) {
        SortedList<Task> currentEmployeesTasks = employees[i].getTasks();
        SortedList<Task>::ConstIterator currentTask = currentEmployeesTasks.begin();
        while (currentTask != currentEmployeesTasks.end()) {
            tasks.insert(*currentTask);
            ++currentTask;
        }
    }

    SortedList<Task>::ConstIterator current = tasks.begin();
    while (current != tasks.end()) {
        cout << *current << endl;
        ++current;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> tasks;
    for (int i = 0; i < employeeCount; i++) {
        SortedList<Task> currentEmployeesTasks = employees[i].getTasks();
        SortedList<Task>::ConstIterator currentTask = currentEmployeesTasks.begin();
        while (currentTask != currentEmployeesTasks.end()) {
            if ((*currentTask).getType() == type) {
                tasks.insert(*currentTask);
            }
            ++currentTask;
        }
    }

    SortedList<Task>::ConstIterator current = tasks.begin();
    while (current != tasks.end()) {
        cout << *current << endl;
        ++current;
    }
}


