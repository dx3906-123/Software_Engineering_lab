#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

// 日期时间工具类
class DateTime {
public:
    int year, month, day, hour, minute;

    DateTime(int y, int m, int d, int h, int min)
        : year(y), month(m), day(d), hour(h), minute(min) {}

    string toString() const
    {
        ostringstream oss;
        oss << year << "-" << setw(2) << setfill('0') << month << "-" << setw(2) << setfill('0') << day
            << " " << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
        return oss.str();
    }

    static DateTime now()
    {
        time_t t = time(nullptr);
        struct tm* tm = localtime(&t);
        return DateTime(1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min);
    }
};

// 日志系统
class Logger {
public:
    static void log(const string& message)
    {
        cout << "[LOG] " << message << endl;
        ofstream logFile("system.log", ios_base::app);
        logFile << "[LOG] " << message << endl;
    }

    static void error(const string& message)
    {
        cout << "[ERROR] " << message << endl;
        ofstream logFile("system.log", ios_base::app);
        logFile << "[ERROR] " << message << endl;
    }
};

// 交通工具类
class Vehicle {
private:
    string plateNumber;
    string driverName;
    string vehicleType; // e.g., Sedan, SUV, Bus
    bool isAvailable;

public:
    Vehicle(const string& plate, const string& driver, const string& type)
        : plateNumber(plate), driverName(driver), vehicleType(type), isAvailable(true) {}

    string getPlateNumber() const { return plateNumber; }
    string getDriverName() const { return driverName; }
    string getVehicleType() const { return vehicleType; }
    bool getAvailability() const { return isAvailable; }

    void assignToGuest() { isAvailable = false; }
    void release() { isAvailable = true; }
};


// 通知类
class Notification {
private:
    int id;
    string content;
    DateTime sentTime;
    string event;

public:
    Notification(int id, const string& content, const DateTime& time, const string& event)
        : id(id), content(content), sentTime(time), event(event) {}

    void sendToGuest(const string& guestName) const {
        Logger::log("Notification sent to " + guestName + ": " + content);
    }

    string getContent() const { return content; }
    string getEvent() const { return event; }
};

// 事件类
class Event {
private:
    int id;
    string name;
    string location;
    DateTime time;
    vector<string> invitedGuests;

public:
    Event(int id, const string& name, const string& location, const DateTime& time)
        : id(id), name(name), location(location), time(time) {}

    void inviteGuest(const string& guest)
    {
        invitedGuests.push_back(guest);
        Logger::log("Guest " + guest + " invited to event: " + name);
    }

    void listGuests() const
    {
        cout << "Guests invited to " << name << ":" << endl;
        for (const auto& guest : invitedGuests) {
            cout << "- " << guest << endl;
        }
    }

    string getName() const { return name; }
    DateTime getTime() const { return time; }
};

// 嘉宾类
class Guest {
private:
    int id;
    string name;
    string contactInfo;
    vector<Event> schedule;
    vector<Notification> notificationList;
    string status;
    string assignedVehicle;

public:
    Guest(int id, const string& name, const string& contactInfo)
        : id(id), name(name), contactInfo(contactInfo), status("Pending") {}

    void viewSchedule() const
    {
        cout << "Schedule for " << name << ":" << endl;
        for (const auto& event : schedule) {
            cout << "- " << event.getName() << " at " << event.getTime().toString() << endl;
        }
    }

    void addEvent(const Event& event)
    {
        schedule.push_back(event);
    }

    void receiveNotification(const Notification& notification)
    {
        notificationList.push_back(notification);
        notification.sendToGuest(name);
    }

    void updateStatus(const string& newStatus)
    {
        status = newStatus;
        Logger::log("Status of guest " + name + " updated to " + status);
    }

    string getStatus() const { return status; }
    string getName() const { return name; }
    int getId() const { return id; }
    string getAssignedVehicle() const { return assignedVehicle; }
    void assignVehicle(const string& vehiclePlate)
    {
        assignedVehicle = vehiclePlate;
    }
};

// 证件类
class Certificate {
private:
    int certificateID;
    DateTime issueDate;
    DateTime expiryDate;

public:
    Certificate(int id, const DateTime& issue, const DateTime& expiry)
        : certificateID(id), issueDate(issue), expiryDate(expiry) {}

    void assignToGuest(const string& guestName)
    {
        Logger::log("Certificate assigned to " + guestName);
    }
};

// 权限类
class Permissions {
private:
    map<string, string> guestPermissions;

public:
    void assignRole(const string& guest, const string& role) {

        guestPermissions[guest] = role;
        Logger::log("Assigned role " + role + " to " + guest);
    }

    string getRole(const string& guest)
    {
        return guestPermissions.count(guest) ? guestPermissions[guest] : "No Role";
    }
};

// 系统管理类
class System {
private:
    vector<Guest> guests;
    vector<Event> events;
    vector<Vehicle> vehicles;
    Permissions permissions;

public:
    // 获取嘉宾列表
    vector<Guest>& getGuests()
    {
        return guests;
    }

    // 添加嘉宾
    void addGuest(const Guest& guest)
    {
        guests.push_back(guest);
        Logger::log("Added guest: " + guest.getName());
    }

    // 添加车辆
    void addVehicle(const Vehicle& vehicle)
    {
        vehicles.push_back(vehicle);
        //string plate_number = vehicle.getPlateNumber();
        //guest.assignVehicle(plate_number);
        //vehicle.assignToGuest();
    }

    // 为嘉宾分配车辆
    void assignVehicleToGuest(int guestId, const string& plateNumber)
    {
        auto guestIt = find_if(guests.begin(), guests.end(),
            [guestId](Guest& g) { return g.getId() == guestId; });
        if (guestIt == guests.end()) {
            cout << "Guest with ID " << guestId << " not found." << endl;
            return;
        }

        auto vehicleIt = find_if(vehicles.begin(), vehicles.end(),
            [plateNumber](Vehicle& v) { return v.getPlateNumber() == plateNumber; });
        if (vehicleIt == vehicles.end()) {
            cout << "Vehicle with plate number " << plateNumber << " not found." << endl;
            return;
        }

        if (!vehicleIt->getAvailability()) {
            cout << "Vehicle " << plateNumber << " is not available." << endl;
            return;
        }

        guestIt->assignVehicle(plateNumber);
        vehicleIt->assignToGuest();
        cout << "Assigned vehicle " << plateNumber << " to guest " << guestIt->getName() << "." << endl;
    }

    // 释放车辆
    void releaseVehicle(const string& plateNumber)
    {
        auto vehicleIt = find_if(vehicles.begin(), vehicles.end(),
            [plateNumber](Vehicle& v) { return v.getPlateNumber() == plateNumber; });
        if (vehicleIt != vehicles.end()) {
            vehicleIt->release();
            cout << "Vehicle " << plateNumber << " is now available." << endl;
        }
        else {
            cout << "Vehicle with plate number " << plateNumber << " not found." << endl;
        }
    }

    // 查询嘉宾的用车信息
    void listGuestVehicles() const
    {
        cout << "Guest Vehicle Assignments:" << endl;
        for (const auto& guest : guests) {
            cout << "Guest: " << guest.getName() << " | Assigned Vehicle: "
                << (guest.getAssignedVehicle().empty() ? "None" : guest.getAssignedVehicle()) << endl;
        }
    }

    // 列出所有车辆
    void listVehicles() const
    {
        cout << "Vehicles:" << endl;
        for (const auto& vehicle : vehicles) {
            cout << "Plate: " << vehicle.getPlateNumber()
                << " | Driver: " << vehicle.getDriverName()
                << " | Type: " << vehicle.getVehicleType()
                << " | Available: " << (vehicle.getAvailability() ? "Yes" : "No") << endl;
        }
    }

    // 添加事件
    void addEvent(const Event& event)
    {
        events.push_back(event);
        Logger::log("Added event: " + event.getName());
    }

    // 展示所有嘉宾
    void listGuests() const
    {
        cout << "All guests:" << endl;
        for (const auto& guest : guests) {
            cout << "- " << guest.getName() << " (" << guest.getStatus() << ")" << endl;
        }
    }

    // 展示所有事件
    void listEvents() const
    {
        cout << "All events:" << endl;
        for (const auto& event : events) {
            cout << "- " << event.getName() << " at " << event.getTime().toString() << endl;
        }
    }

    // 搜索嘉宾
    void searchGuest(const string& name) const
    {
        auto it = find_if(guests.begin(), guests.end(),
            [&name](const Guest& g) { return g.getName() == name; });
        if (it != guests.end()) {
            cout << "Guest found: " << it->getName() << " (" << it->getStatus() << ")" << endl;
        }
        else {
            cout << "Guest not found." << endl;
        }
    }

    // 搜索事件
    void searchEvent(const string& name) const
    {
        auto it = find_if(events.begin(), events.end(),
            [&name](const Event& e) { return e.getName() == name; });
        if (it != events.end()) {
            cout << "Event found: " << it->getName() << " at " << it->getTime().toString() << endl;
        }
        else {
            cout << "Event not found." << endl;
        }
    }

    // 分配角色给嘉宾
    void assignRoleToGuest(const string& guestName, const string& role)
    {
        permissions.assignRole(guestName, role);
    }

    // 批量添加嘉宾
    /*void batchAddGuests()
    {
        cout << "Adding multiple guests..." << endl;
        addGuest(Guest(1, "Alice Johnson", "alice.johnson@university.com" ));
        addGuest(Guest(2, "Bob Smith", "bob.smith@company.com"));
        addGuest(Guest(3, "Charlie Brown", "charlie.brown@company.com"));
        addGuest(Guest(4, "Diana White", "diana.white@gov.com"));
    }

    // 保存数据到文件
    void saveDataToFile() const
    {
        ofstream outFile("guests.txt");
        for (const auto& guest : guests) {
            outFile << guest.getName() << ", " << guest.getStatus() << endl;
        }
        outFile.close();
        Logger::log("Data saved to guests.txt");
    }

    // 从文件加载数据
    void loadDataFromFile()
    {
        ifstream inFile("guests.txt");
        if (!inFile) {
            Logger::error("Failed to open file.");
            return;
        }

        string name, status;
        while (getline(inFile, name, ',') && getline(inFile, status)) {
            addGuest(Guest(guests.size() + 1, name, ""));
            guests.back().updateStatus(status);
        }
        inFile.close();
        Logger::log("Data loaded from guests.txt");
    }*/
};

int main() {
    System system;

    // 添加嘉宾
    //system.batchAddGuests();
    system.addGuest(Guest(1, "Alice Johnson", "alice.johnson@university.com"));
    system.addGuest(Guest(2, "Bob Smith", "bob.smith@company.com"));
    system.addGuest(Guest(3, "Charlie Brown", "charlie.brown@organization.org"));
    system.addGuest(Guest(4, "Diana White", "diana.white@gov.com"));

    // 添加车辆
    system.addVehicle(Vehicle("ABC-123", "John Doe", "Sedan"));
    system.addVehicle(Vehicle("XYZ-789", "Jane Smith", "SUV"));
    system.addVehicle(Vehicle("LMN-456", "Robert Green", "Minivan"));
    system.addVehicle(Vehicle("OPQ-111", "Laura Black", "Su7"));


    // 添加事件
    Event event1(1, "Opening Ceremony", "Main Hall", DateTime(2024, 11, 25, 9, 0));
    Event event2(2, "Tech Showcase", "Exhibition Center", DateTime(2024, 11, 26, 14, 0));
    system.addEvent(event1);
    system.addEvent(event2);

    // 展示嘉宾和事件
    system.listGuests();
    system.listEvents();

    // 为嘉宾分配车辆
    cout << "\n--- Assigning Vehicles to Guests ---\n";
    system.assignVehicleToGuest(1, "ABC-123");
    system.assignVehicleToGuest(2, "XYZ-789");
    system.assignVehicleToGuest(3, "LMN-456");

    // 查询嘉宾的用车信息
    cout << "\n--- Guest Vehicle Assignments ---\n";
    system.listGuestVehicles();

    // 释放一辆车辆
    cout << "\n--- Releasing Vehicle ---\n";
    system.releaseVehicle("ABC-123");

    // 查询车辆状态
    cout << "\n--- Updated Vehicle List ---\n";
    system.listVehicles();

    // 搜索嘉宾和事件
    system.searchGuest("Alice Johnson");
    system.searchEvent("Opening Ceremony");

    // 更新嘉宾状态
    system.getGuests()[0].updateStatus("Arrived");

    // 保存和加载数据
    //system.saveDataToFile();
    //system.loadDataFromFile();

    // 分配角色
    system.assignRoleToGuest("Alice Johnson", "VIP");

    return 0;
}
