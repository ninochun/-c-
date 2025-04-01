#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <functional>





using namespace std;

// ����Ա�ڵ�ṹ(˫����)
struct AdminNode {
    string account;     // �˺�
    string password;    // ����
    string name;        // ����
    AdminNode* prev;    // ָ��ǰһ���ڵ��ָ��
    AdminNode* next;    // ָ���һ���ڵ��ָ��

    // ���캯��
    AdminNode(const string& acc, const string& pwd, const string& nm)
        : account(acc), password(pwd), name(nm), prev(nullptr), next(nullptr) {}
};

// ����Ա˫������
class AdminList {
private:
    AdminNode* head;   // ����ͷָ��
    AdminNode* tail;   // ����βָ��

public:
    // ���캯��
    AdminList() : head(nullptr), tail(nullptr) {
        loadFromFile("admins.txt");  // ���ļ���������
    }

    // ��������
    ~AdminList() {
        saveToFile("admins.txt");    // �������ݵ��ļ�
        AdminNode* current = head;
        while (current) {
            AdminNode* temp = current;
            current = current->next;
            delete temp;
        }
    }


    // ���ӹ���Ա�ڵ�
    void addAdmin(const string& account, const string& password, const string& name) {
        if (findByAccount(account)) {
            cout << "�˺��Ѵ��ڣ��޷�����¹���Ա��" << endl;
            return;
        }

        AdminNode* newAdmin = new AdminNode(account, password, name);
        if (!head) {
            head = tail = newAdmin;
        }
        else {
            tail->next = newAdmin;
            newAdmin->prev = tail;
            tail = newAdmin;
        }
       
        saveToFile("admins.txt"); // �������ݵ��ļ�
    }

    // �����˺Ų��ҽڵ���Ϣ
    AdminNode* findByAccount(const string& account) const {
        AdminNode* current = head;
        while (current) {
            if (current->account == account) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // δ�ҵ�
    }

    // �����˺��޸�����
    bool modifyPassword(const string& account, const string& newPassword) {
        AdminNode* admin = findByAccount(account);
        if (admin) {
            admin->password = newPassword;
            
            saveToFile("admins.txt"); // �����޸ĺ������
            return true;
        }
     
        return false;
    }

    void saveToFile(const string& filename, const vector<string>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "�޷����ļ�����д��: " << filename << endl;
        return;
    }
    for (const string& line : data) {
        file << line << endl;
    }
    file.close();
}

    // �����˺��޸�����
    bool modifyName(const string& account, const string& newName) {
        AdminNode* admin = findByAccount(account);
        if (admin) {
            admin->name = newName;
          
            saveToFile("admins.txt"); // �����޸ĺ������
            return true;
        }
      
        return false;
    }

    // ��������ɾ���ڵ�
    bool deleteByName(const string& name) {
        AdminNode* current = head;
        while (current) {
            if (current->name == name) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                delete current;
               
                saveToFile("admins.txt"); // �����޸ĺ������
                return true;
            }
            current = current->next;
        }
 
        return false;
    }

    // ��ʾ���й���Ա��Ϣ
    void displayAll() const {
        if (!head) {
            cout << "���޹���Ա��Ϣ��" << endl;
            return;
        }
        AdminNode* current = head;
        cout << "����Ա��Ϣ�б�" << endl;
        while (current) {
            cout << "�˺�: " << current->account << ", ����: " << current->name << endl;
            current = current->next;
        }
    }

    // �������ݵ��ļ�
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "�޷����浽�ļ�: " << filename << "�������ļ�·����Ȩ�ޡ�" << endl;
            return;
        }
        AdminNode* current = head;
        while (current) {
            file << current->account << "," << current->password << "," << current->name << endl;
            current = current->next;
        }
        file.close();
    }

    // ���ļ���������
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "�޷����ļ�: " << filename << "�������ļ�·����Ȩ�ޡ�" << endl;
            return;
        }
        string account, password, name;
        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.rfind(',');
            if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                account = line.substr(0, pos1);
                password = line.substr(pos1 + 1, pos2 - pos1 - 1);
                name = line.substr(pos2 + 1);
                addAdmin(account, password, name);
            }
        }
        file.close();
    }
};


struct CustomerNode {
    string roomNumber;
    string name;
    string gender;
    string phone;
    string checkInTime;
    string roomType;
    string luggageNumber;
    CustomerNode* prev;
    CustomerNode* next;

    // ���캯��
    CustomerNode(const string& rmNum, const string& nm, const string& gnd, const string& ph, const string& chkIn, const string& rmType, const string& lgNum)
        : roomNumber(rmNum), name(nm), gender(gnd), phone(ph), checkInTime(chkIn), roomType(rmType), luggageNumber(lgNum), prev(nullptr), next(nullptr) {}
};
bool isDuplicateRoomAndDate(const vector<string>& guests, const string& roomNumber, const string& checkInDate) {
    for (const string& guest : guests) {
        stringstream ss(guest);
        vector<string> fields;
        string field;

        // ��ÿ�����ݷָ����ֶ�
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // �����1�ֶ��Ƿ���ţ���5�ֶ�����ס����
        if (fields.size() >= 5 && fields[0] == roomNumber && fields[4] == checkInDate) {
            return true;
        }
    }
    return false;
}

// �˿�˫������
class CustomerList {
private:
    CustomerNode* head;
    CustomerNode* tail;

public:
    // ���캯��
    CustomerList() : head(nullptr), tail(nullptr) {
        loadFromFile("customers.txt");  // ���ļ���������
    }

    // ��������
    ~CustomerList() {
        saveToFile("customers.txt");  // �������ݵ��ļ�
        CustomerNode* current = head;
        while (current) {
            CustomerNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    std::vector<std::string> loadFromFile(const std::string& filename) {
    std::vector<std::string> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }
    file.close();
    return data;
}
   
    // ���ӿͻ���Ϣ�ڵ�
    void addCustomer(const string& roomNumber, const string& name, const string& gender, const string& phone, const string& checkInDate, const string& roomType, const string& luggageNumber) {
        // �����������пͻ�����
        vector<string> guests = loadFromFile("customers.txt");

        // ����Ƿ��Ѿ�������ͬ����ź���ס���ڵļ�¼
        if (isDuplicateRoomAndDate(guests, roomNumber, checkInDate)) {
            cout << "ͬһ��������ͬ�����ѱ�Ԥ�����޷��ظ���ӡ�\n";
            return;
        }

        // ����Ƿ��Ѿ�����ͬ���ͻ�
        if (findCustomerByName(name)) {
            cout << "�ͻ��Ѵ��ڣ��޷��ظ���ӡ�\n";
            return;
        }

        // ���û�г�ͻ������¿ͻ��ڵ�
        CustomerNode* newCustomer = new CustomerNode(roomNumber, name, gender, phone, checkInDate, roomType, luggageNumber);
        if (!head) {
            head = tail = newCustomer;
        }
        else {
            tail->next = newCustomer;
            newCustomer->prev = tail;
            tail = newCustomer;
        }

        cout << "�ͻ���Ϣ��ӳɹ���\n";
        saveToFile("customers.txt"); // �������ݵ��ļ�
    }

    // �����������ҿͻ��ڵ�
    CustomerNode* findCustomerByName(const string& name) const {
        CustomerNode* current = head;
        while (current) {
            if (current->name == name) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // ���������޸��Ա�
    void updateGender(const string& name, const string& newGender) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->gender = newGender;
            cout << "�Ա��޸ĳɹ���" << endl;
            saveToFile("customers.txt"); // �����޸ĺ������
        }
        else {
            cout << "δ�ҵ��ͻ�����: " << name << endl;
        }
    }

    // ���������޸��ֻ���
    void updatePhone(const string& name, const string& newPhone) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->phone = newPhone;
            cout << "�ֻ����޸ĳɹ���" << endl;
            saveToFile("customers.txt"); // �����޸ĺ������
        }
        else {
            cout << "δ�ҵ��ͻ�����: " << name << endl;
        }
    }

    // ���������޸���סʱ��
    void updateCheckInTime(const string& name, const string& newCheckInTime) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->checkInTime = newCheckInTime;
            cout << "��סʱ���޸ĳɹ���" << endl;
            saveToFile("customers.txt"); // �����޸ĺ������
        }
        else {
            cout << "δ�ҵ��ͻ�����: " << name << endl;
        }
    }

    // ���������޸ķ�������
    void updateRoomType(const string& name, const string& newRoomType) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->roomType = newRoomType;
            cout << "���������޸ĳɹ���" << endl;
            saveToFile("customers.txt"); // �����޸ĺ������
        }
        else {
            cout << "δ�ҵ��ͻ�����: " << name << endl;
        }
    }

    // ���ݷ�����޸�����
    void updateNameByRoomNumber(const string& roomNumber, const string& newName) {
        CustomerNode* current = head;
        while (current) {
            if (current->roomNumber == roomNumber) {
                current->name = newName;
                cout << "�����޸ĳɹ���" << endl;
                saveToFile("customers.txt"); // �����޸ĺ������
                return;
            }
            current = current->next;
        }
        cout << "δ�ҵ��÷���ţ�" << endl;
    }

    // ɾ���ͻ��ڵ�
    void deleteCustomerByName(const string& name) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            if (customer->prev) customer->prev->next = customer->next;
            if (customer->next) customer->next->prev = customer->prev;
            if (customer == head) head = customer->next;
            if (customer == tail) tail = customer->prev;
            delete customer;
            cout << "�ͻ��ڵ�ɾ���ɹ���" << endl;
            saveToFile("customers.txt"); // �����޸ĺ������
        }
        else {
            cout << "δ�ҵ��ͻ�����: " << name << endl;
        }
    }

    // ��ӡ���пͻ���Ϣ
    void displayCustomers() const {
        CustomerNode* current = head;
        if (!current) {
            cout << "���޿ͻ���Ϣ��" << endl;
            return;
        }
        cout << "�ͻ���Ϣ�б�" << endl;
        while (current) {
            cout << "�����: " << current->roomNumber << ", ����: " << current->name
                << ", �Ա�: " << current->gender << ", �绰: " << current->phone
                << ", ��סʱ��: " << current->checkInTime << ", ��������: " << current->roomType
                << ", ����İ����: " << current->luggageNumber << endl;
            current = current->next;
        }
    }

    // �������ݵ��ļ�
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "�޷����浽�ļ�: " << filename << "�������ļ�·����Ȩ�ޡ�" << endl;
            return;
        }
        CustomerNode* current = head;
        while (current) {
            file << current->roomNumber << "," << current->name << "," << current->gender << ","
                << current->phone << "," << current->checkInTime << "," << current->roomType << ","
                << current->luggageNumber << endl;
            current = current->next;
        }
        file.close();
    }

   

};

vector<tuple<string, string, string>> loadAdminAccounts(const string& filename) {
    vector<tuple<string, string, string>> adminAccounts;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ�: " << filename << endl;
        return adminAccounts;
    }

    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.rfind(',');
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            string account = line.substr(0, pos1);
            string password = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string name = line.substr(pos2 + 1);
            adminAccounts.emplace_back(account, password, name);
        }
    }
    file.close();
    return adminAccounts;
}

bool verifyAdminLogin(const vector<tuple<string, string, string>>& adminAccounts, const string& account, const string& password) {
    for (const auto& admin : adminAccounts) {
        if (get<0>(admin) == account && get<1>(admin) == password) {
            cout << "��¼�ɹ�����ӭ " << get<2>(admin) << "��" << endl;
            return true;
        }
    }
    cout << "��¼ʧ�ܣ������˺Ż����롣" << endl;
    return false;
}




struct RoomNode {
    string roomNumber;
    string type;
    double price;
    double discount;
    bool isOccupied;
    RoomNode* prev;
    RoomNode* next;

    RoomNode(const string& rn, const string& t, double p, double d, bool o)
        : roomNumber(rn), type(t), price(p), discount(d), isOccupied(o), prev(nullptr), next(nullptr) {}
};

class RoomList {
private:
    RoomNode* head;
    RoomNode* tail;

public:
    RoomList() : head(nullptr), tail(nullptr) {
        loadFromFile("rooms.txt");
    }

    ~RoomList() {
        saveToFile("rooms.txt");
        RoomNode* current = head;
        while (current) {
            RoomNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addRoom(const string& roomNumber, const string& type, double price, double discount, bool isOccupied) {
        RoomNode* newRoom = new RoomNode(roomNumber, type, price, discount, isOccupied);
        if (!head) {
            head = tail = newRoom;
        }
        else {
            tail->next = newRoom;
            newRoom->prev = tail;
            tail = newRoom;
        }
        cout << "����ڵ���ӳɹ���\n";
    }
    void displayStatistics() {
        int totalRooms = 0;
        int occupiedRooms = 0;
        RoomNode* current = head;

        // ��������ͳ�Ʒ�������������ס������
        while (current != nullptr) {
            totalRooms++;
            if (current->isOccupied) {
                occupiedRooms++;
            }
            current = current->next;
        }

        // ������ס��
        double occupancyRate = (totalRooms > 0) ? (static_cast<double>(occupiedRooms) / totalRooms) * 100 : 0.0;

        // ��ӡͳ����Ϣ
        cout << "����ͳ����Ϣ��" << endl;
        cout << "�ܷ�����: " << totalRooms << endl;
        cout << "����ס������: " << occupiedRooms << endl;
        cout << "��ס��: " << occupancyRate << "%" << endl;
    }


    RoomNode* findRoomByNumber(const string& roomNumber) {
        RoomNode* current = head;
        while (current) {
            if (current->roomNumber == roomNumber) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void updateRoomInfo(const string& roomNumber, const function<void(RoomNode*)>& updateFunc) {
        RoomNode* room = findRoomByNumber(roomNumber);
        if (room) {
            updateFunc(room);
            cout << "������Ϣ�޸ĳɹ���\n";
        }
        else {
            cout << "δ�ҵ��÷���ţ�\n";
        }
    }

    void displayRooms() const {
        if (!head) {
            cout << "���޷�����Ϣ��\n";
            return;
        }
        RoomNode* current = head;
        while (current) {
            cout << "�����: " << current->roomNumber << ", ����: " << current->type << ", �۸�: " << current->price
                << ", �ۿ�: " << current->discount << ", ��ס���: " << (current->isOccupied ? "����ס" : "δ��ס") << endl;
            current = current->next;
        }
    }

    void deleteRoom(const string& roomNumber) {
        RoomNode* room = findRoomByNumber(roomNumber);
        if (room) {
            if (room->prev) room->prev->next = room->next;
            if (room->next) room->next->prev = room->prev;
            if (room == head) head = room->next;
            if (room == tail) tail = room->prev;
            delete room;
            cout << "����ڵ�ɾ���ɹ���\n";
        }
        else {
            cout << "δ�ҵ��÷���ţ�\n";
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "�޷����ļ�����д��: " << filename << endl;
            return;
        }
        RoomNode* current = head;
        while (current) {
            file << current->roomNumber << "," << current->type << "," << current->price << ","
                << current->discount << "," << (current->isOccupied ? "1" : "0") << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "�޷����ļ����ж�ȡ: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            string roomNumber, type, priceStr, discountStr, occupancyStr;
            getline(ss, roomNumber, ',');
            getline(ss, type, ',');
            getline(ss, priceStr, ',');
            getline(ss, discountStr, ',');
            getline(ss, occupancyStr, ',');
            double price = stod(priceStr);
            double discount = stod(discountStr);
            bool isOccupied = (occupancyStr == "1");
            addRoom(roomNumber, type, price, discount, isOccupied);
        }
        file.close();
    }
};
struct ProjectNode {
    string typeName;
    double price;
    string customerName;
    double discount;
    ProjectNode* next;

    ProjectNode(const string& type, double p, const string& customer, double d)
        : typeName(type), price(p), customerName(customer), discount(d), next(nullptr) {}
};

class ProjectList {
private:
    ProjectNode* head;

public:
    ProjectList() : head(nullptr) {
        loadFromFile("projects.txt"); // ��ʼ��ʱ�����ļ�����
    }

    ~ProjectList() {
        saveToFile("projects.txt"); // �˳�ʱ�������ݵ��ļ�
        ProjectNode* current = head;
        while (current) {
            ProjectNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addProject(const string& typeName, double price, const string& customerName, double discount) {
        ProjectNode* newNode = new ProjectNode(typeName, price, customerName, discount);
        if (!head) {
            head = newNode;
        }
        else {
            ProjectNode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        cout << "������Ŀ�ڵ���ӳɹ���\n";
        saveToFile("projects.txt"); // �����Ŀ�󱣴浽�ļ�
    }

    bool modifyPrice(const string& typeName, double newPrice) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->price = newPrice;
                cout << "�۸��޸ĳɹ���\n";
                saveToFile("projects.txt"); // �޸ļ۸�󱣴浽�ļ�
                return true;
            }
            current = current->next;
        }
        cout << "δ�ҵ�����������\n";
        return false;
    }

    bool modifyCustomerName(const string& typeName, const string& newName) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->customerName = newName;
                cout << "�ͻ������޸ĳɹ���\n";
                saveToFile("projects.txt"); // �޸Ŀͻ������󱣴浽�ļ�
                return true;
            }
            current = current->next;
        }
        cout << "δ�ҵ�����������\n";
        return false;
    }

    bool modifyDiscount(const string& typeName, double newDiscount) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->discount = newDiscount;
                cout << "�ۿ��޸ĳɹ���\n";
                saveToFile("projects.txt"); // �޸��ۿۺ󱣴浽�ļ�
                return true;
            }
            current = current->next;
        }
        cout << "δ�ҵ�����������\n";
        return false;
    }

    bool deleteProject(const string& typeName) {
        ProjectNode* current = head;
        ProjectNode* prev = nullptr;
        while (current) {
            if (current->typeName == typeName) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    head = current->next;
                }
                delete current;
                cout << "������Ŀ�ڵ�ɾ���ɹ���\n";
                saveToFile("projects.txt"); // ɾ����Ŀ�󱣴浽�ļ�
                return true;
            }
            prev = current;
            current = current->next;
        }
        cout << "δ�ҵ�����������\n";
        return false;
    }

    void displayProjects() const {
        ProjectNode* current = head;
        if (!current) {
            cout << "����������Ŀ��Ϣ��\n";
            return;
        }
        while (current) {
            cout << "����: " << current->typeName << ", �۸�: " << current->price
                << ", �ͻ�����: " << current->customerName << ", �ۿ�: " << current->discount << endl;
            current = current->next;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "�޷����ļ�����д��: " << filename << endl;
            return;
        }
        ProjectNode* current = head;
        while (current) {
            file << current->typeName << "," << current->price << ","
                << current->customerName << "," << current->discount << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "�޷����ļ����ж�ȡ: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            string typeName, priceStr, customerName, discountStr;
            getline(ss, typeName, ',');
            getline(ss, priceStr, ',');
            getline(ss, customerName, ',');
            getline(ss, discountStr, ',');

            double price = stod(priceStr);
            double discount = stod(discountStr);
            addProject(typeName, price, customerName, discount);
        }
        file.close();
    }
};


// �ļ����غ���
vector<string> loadFromFile(const string& filename) {
    vector<string> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        data.push_back(line);
    }
    return data;
}

// ���˵�����
void main_menu();
void admin_menu();
void guest_menu();
void room_menu();
void project_menu();
void bill();

// ���˵�ʵ��
// ����ȫ�ֵĿͻ�����
CustomerList customerList;

void main_menu() {
    int choice;

    while (true) {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif

      
        cout << "=============================\n";
        cout << "||   ***�Ƶ���Ϣ����ϵͳ**** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. ����Ա��Ϣ���       ||\n";
        cout << "|| 2. ������ס             ||\n";
        cout << "|| 3. �˷�����             ||\n";
        cout << "|| 4. �ͻ���Ϣ���         ||\n";
        cout << "|| 5. ������Ϣ���         ||\n";
        cout << "|| 6. ������Ŀ���         ||\n";
        cout << "=========================||\n";
        cout << "��ѡ��һ��ѡ��: ";
        cin >> choice;

        switch (choice) {
        case 1:  admin_menu(); break;
        case 2: {
            string name, roomNumber, gender, phone, checkInTime, roomType, luggageNumber;
            cout << "������ס\n";
            cout << "����: "; cin >> name;
            cout << "�����: "; cin >> roomNumber;
            cout << "�Ա�: "; cin >> gender;
            cout << "�ֻ���: "; cin >> phone;
            cout << "��סʱ��: "; cin >> checkInTime;
            cout << "��������: "; cin >> roomType;
            cout << "����İ����: "; cin >> luggageNumber;

            customerList.addCustomer(roomNumber, name, gender, phone, checkInTime, roomType, luggageNumber);
            break;
        }
        case 3: bill(); break;
        case 4: guest_menu(); break;
        case 5: room_menu(); break;
        case 6: project_menu(); break;
        default: cout << "��Чѡ�����������롣\n"; break;
        }
    }
}


void admin_menu() {
vector<tuple<string, string, string>> adminAccounts = loadAdminAccounts("admins.txt");
    string account, password;
    cout << "�������˺�: ";
    cin >> account;
    cout << "����������: ";
    cin >> password;

    if (verifyAdminLogin(adminAccounts, account, password)) {
        cout << "��¼�ɹ���" << endl;
 
        AdminList adminList;
        int choice;
        while (true) {
            // ��������������ϵͳѡ����ʹ�ã�
#ifdef _WIN32
            system("CLS");
#else
            system("clear");
#endif

            // �˵�����
            cout << "============================\n";
            cout << "||                         ||\n";
            cout << "||  ***�Ƶ���Ϣ����ϵͳ*** ||\n";
            cout << "||=========================||\n";
            cout << "|| 1. �޸�����             ||\n";
            cout << "|| 2. �޸�����             ||\n";
            cout << "|| 3. ���ӹ���Ա           ||\n";
            cout << "|| 4. ɾ������Ա           ||\n";
            cout << "|| 5. ��ʾ���й���Ա       ||\n";
            cout << "|| 6. �������˵�           ||\n";
            cout << "============================\n";
            cout << "��ѡ��һ��ѡ��: ";
            cin >> choice;

            // ���������Ч��
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "������Ч��������һ�����֡�\n";
                continue;
            }

            string account, newPassword, newName;

            switch (choice) {
            case 1: // �޸�����
                cout << "������Ҫ�޸��˺�: ";
                cin >> account;
                cout << "������������: ";
                cin >> newPassword;
                if (adminList.modifyPassword(account, newPassword)) {
                    cout << "�����޸ĳɹ���\n";
                }
                else {
                    cout << "δ�ҵ����˺ţ�\n";
                }
                break;

            case 2: // �޸�����
                cout << "������Ҫ�޸��������˺�: ";
                cin >> account;
                cout << "�������µ�����: ";
                cin.ignore(); // ���������
                getline(cin, newName);
                if (adminList.modifyName(account, newName)) {
                    cout << "�����޸ĳɹ���\n";
                }
                else {
                    cout << "δ�ҵ����˺ţ�\n";
                }
                break;

            case 3: // ���ӹ���Ա
            {
                string password;
                cout << "�������¹���Ա���˺�: ";
                cin >> account;
                cout << "����������: ";
                cin >> password;
                cout << "����������: ";
                cin.ignore();
                getline(cin, newName);
                adminList.addAdmin(account, password, newName);
            }
            break;

            case 4: // ɾ������Ա
                cout << "������Ҫɾ���Ĺ���Ա����: ";
                cin.ignore();
                getline(cin, newName);
                if (adminList.deleteByName(newName)) {
                    cout << "����Աɾ���ɹ���\n";
                }
                else {
                    cout << "δ�ҵ���������\n";
                }
                break;

            case 5: // ��ʾ���й���Ա
                adminList.displayAll();
                break;

            case 6: // �������˵�
                return;

            default:
                cout << "��Чѡ�����������롣\n";
                break;
            }



            cin.ignore();
            cin.get();
        }
    }
}




void guest_menu() {
    int choice;
    string name, newValue, roomNumber;

    while (true) {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif
        cout << "============================\n";
        cout << "||                         ||\n";
        cout << "||  ***�Ƶ���Ϣ����ϵͳ*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. ���������޸��Ա�     ||\n";
        cout << "|| 2. ���������޸��ֻ���   ||\n";
        cout << "|| 3. ���������޸���סʱ�� ||\n";
        cout << "|| 4. ���������޸ķ������� ||\n";
        cout << "|| 5. ���ݷ�����޸�����   ||\n";
        cout << "|| 6. ���ӿͻ���Ϣ�ڵ�     ||\n";
        cout << "|| 7. ��ӡ���нڵ���Ϣ     ||\n";
        cout << "|| 8. ��������ɾ���ڵ�     ||\n";
        cout << "|| 9. �������˵�           ||\n";
        cout << "============================\n";
        cout << "��ѡ��һ��ѡ��: ";
        cin >> choice;

        // ������Ч�Լ��
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������Ч��������һ�����֡�\n";
            continue;
        }

        switch (choice) {
        case 1: // ���������޸��Ա�
            cout << "������˿�����: ";
            cin.ignore();
            getline(cin, name);
            cout << "�������µ��Ա�: ";
            getline(cin, newValue);
            customerList.updateGender(name, newValue);
            break;

        case 2: // ���������޸��ֻ���
            cout << "������˿�����: ";
            cin.ignore();
            getline(cin, name);
            cout << "�������µ��ֻ���: ";
            getline(cin, newValue);
            customerList.updatePhone(name, newValue);
            break;

        case 3: // ���������޸���סʱ��
            cout << "������˿�����: ";
            cin.ignore();
            getline(cin, name);
            cout << "�������µ���סʱ��: ";
            getline(cin, newValue);
            customerList.updateCheckInTime(name, newValue);
            break;

        case 4: // ���������޸ķ�������
            cout << "������˿�����: ";
            cin.ignore();
            getline(cin, name);
            cout << "�������µķ�������: ";
            getline(cin, newValue);
            customerList.updateRoomType(name, newValue);
            break;

        case 5: // ���ݷ�����޸�����
            cout << "�����뷿���: ";
            cin.ignore();
            getline(cin, roomNumber);
            cout << "�������µ�����: ";
            getline(cin, newValue);
            customerList.updateNameByRoomNumber(roomNumber, newValue);
            break;

        case 6: // ���ӿͻ���Ϣ�ڵ�
        {
            string gender, phone, checkInTime, roomType, luggageNumber;
            cout << "�������¹˿͵�����: ";
            cin.ignore();
            getline(cin, name);
            cout << "�������Ա�: ";
            getline(cin, gender);
            cout << "�������ֻ���: ";
            getline(cin, phone);
            cout << "��������סʱ��: ";
            getline(cin, checkInTime);
            cout << "�����뷿������: ";
            getline(cin, roomType);
            cout << "�����뷿���: ";
            getline(cin, roomNumber);
            cout << "����������İ����: ";
            getline(cin, luggageNumber);
            customerList.addCustomer(roomNumber, name, gender, phone, checkInTime, roomType, luggageNumber);
        }
        break;

        case 7: // ��ӡ���нڵ���Ϣ
            customerList.displayCustomers();
            break;

        case 8: // ��������ɾ���ڵ�
            cout << "������Ҫɾ���Ĺ˿�����: ";
            cin.ignore();
            getline(cin, name);
            customerList.deleteCustomerByName(name);
            break;

        case 9: // �������˵�
            return;

        default:
            cout << "��Чѡ�����������롣\n";
            break;
        }

        // ��ͣ�Բ鿴���
        cout << "���س�������...\n";
        cin.ignore();
        cin.get();
    }
}




// ������Ϣ�˵�����
void room_menu() {
    RoomList roomList; // ���������б����
    int choice;

    while (true) {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif
        // ����˵�����
        system("CLS"); // ���������� Windows ϵͳ��
        cout << "============================\n";
        cout << "||                         ||\n";
        cout << "||  ***�Ƶ���Ϣ����ϵͳ*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. ���ݷ���Ÿ��ķ������� ||\n";
        cout << "|| 2. ���ݷ���Ÿ��ķ���۸� ||\n";
        cout << "|| 3. ���ݷ���Ÿ��ķ����ۿ� ||\n";
        cout << "|| 4. ���ݷ���Ÿ�����ס��� ||\n";
        cout << "|| 5. ��ӡ���нڵ���Ϣ      ||\n";
        cout << "|| 6. ����������Ŀ           ||\n";
        cout << "|| 7. ���ݷ����ɾ���ڵ�   ||\n";
        cout << "|| 8. �������˵�           ||\n";
        cout << "|| 9. ͳ��                 ||\n";
        cout << "============================\n";
        cout << "��ѡ��һ��ѡ��: ";
        cin >> choice;

        // ���������Ч��
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "������Ч��������һ�����֡�\n";
            system("pause");
            continue;
        }

        // �����û��������
        string roomNumber, newValue;
        double newPrice, newDiscount;
        bool newOccupancy;
        bool found = false;

        switch (choice) {
        case 1: // ���ݷ���Ÿ��ķ�������
            cout << "�����뷿��ź��µķ�������:\n";
            cout << "�����: "; cin >> roomNumber;
            cout << "�·�������: "; cin >> newValue;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->type = newValue;
                });
            break;

        case 2: // ���ݷ���Ÿ��ķ���۸�
            cout << "�����뷿��ź��µķ���۸�:\n";
            cout << "�����: "; cin >> roomNumber;
            cout << "�¼۸�: "; cin >> newPrice;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->price = newPrice;
                });
            break;

        case 3: // ���ݷ���Ÿ��ķ����ۿ�
            cout << "�����뷿��ź��µ��ۿ�:\n";
            cout << "�����: "; cin >> roomNumber;
            cout << "���ۿ�: "; cin >> newDiscount;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->discount = newDiscount;
                });
            break;

        case 4: // ���ݷ���Ÿ�����ס���
            cout << "�����뷿��ź��µ���ס�����1Ϊ����ס��0Ϊδ��ס��:\n";
            cout << "�����: "; cin >> roomNumber;
            cout << "��ס��� (1Ϊ����ס��0Ϊδ��ס): "; cin >> newOccupancy;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->isOccupied = newOccupancy;
                });
            break;

        case 5: // ��ӡ���з�����Ϣ
            roomList.displayRooms();
            break;

        case 6: // �����·���ڵ�
        {
            string type;
            double price, discount;
            bool isOccupied;
            cout << "�������·�����Ϣ������š����͡��۸��ۿۡ���ס�����:\n";
            cout << "�����: "; cin >> roomNumber;
            cout << "����: "; cin >> type;
            cout << "�۸�: "; cin >> price;
            cout << "�ۿ�: "; cin >> discount;
            cout << "��ס��� (1Ϊ����ס��0Ϊδ��ס): "; cin >> isOccupied;
            roomList.addRoom(roomNumber, type, price, discount, isOccupied);
        }
        break;

        case 7: // ���ݷ����ɾ���ڵ�
            cout << "������Ҫɾ���ķ����: ";
            cin >> roomNumber;
            roomList.deleteRoom(roomNumber);
            break;

        case 8: // �������˵�
            return;

        case 9: // ͳ����Ϣ
            roomList.displayStatistics();
            break;

        default:
            cout << "��Чѡ�����������롣\n";
            break;
        }

        // ��ͣ�Ա��û��鿴���
        system("pause");
    }
}

void project_menu() {
    ProjectList projectList;
    int choice;
    while (true) {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif

        cout << "============================\n";
        cout << "||                         ||\n";
        cout << "||  ***�Ƶ���Ϣ����ϵͳ*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. �������������ļ۸�   ||\n";
        cout << "|| 2. �������������Ŀͻ�����||\n";
        cout << "|| 3. ���ӽڵ�             ||\n";
        cout << "|| 4. ����������ɾ���ڵ�   ||\n";
        cout << "|| 5. ��ӡ���нڵ���Ϣ     ||\n";
        cout << "|| 6. �����������޸��ۿ�   ||\n";
        cout << "|| 7. �������˵�           ||\n";
        cout << "============================\n";
        cout << "��ѡ��һ��ѡ��: ";
        cin >> choice;

        string typeName, customerName;
        double price, discount;

        switch (choice) {
        case 1:
            cout << "������������Ŀ���������µļ۸�:\n";
            cout << "������: "; cin >> typeName;
            cout << "�¼۸�: "; cin >> price;
            if (projectList.modifyPrice(typeName, price)) {
                cout << "�۸��ѳɹ��޸�Ϊ " << price << "��\n";
            }
            else {
                cout << "�޸�ʧ�ܣ�δ�ҵ�����������\n";
            }
            break;
        case 2:
            cout << "������������Ŀ���������µĿͻ�����:\n";
            cout << "������: "; cin >> typeName;
            cout << "�¿ͻ�����: "; cin >> customerName;
            if (projectList.modifyCustomerName(typeName, customerName)) {
                cout << "�ͻ������ѳɹ��޸�Ϊ " << customerName << "��\n";
            }
            else {
                cout << "�޸�ʧ�ܣ�δ�ҵ�����������\n";
            }
            break;
        case 3:
            cout << "��������������Ŀ��Ϣ�����������۸񡢿ͻ��������ۿۣ�:\n";
            cout << "������: "; cin >> typeName;
            cout << "�۸�: "; cin >> price;
            cout << "�ͻ�����: "; cin >> customerName;
            cout << "�ۿ�: "; cin >> discount;
            projectList.addProject(typeName, price, customerName, discount);
            cout << "��������Ŀ�ѳɹ���ӡ�\n";
            break;
        case 4:
            cout << "������Ҫɾ����������Ŀ������: ";
            cin >> typeName;
            if (projectList.deleteProject(typeName)) {
                cout << "��Ŀ�ѳɹ�ɾ����\n";
            }
            else {
                cout << "ɾ��ʧ�ܣ�δ�ҵ�����������\n";
            }
            break;
        case 5:
            cout << "��ǰ����������Ŀ��Ϣ��\n";
            projectList.displayProjects();
            break;
        case 6:
            cout << "������������Ŀ���������µ��ۿ�:\n";
            cout << "������: "; cin >> typeName;
            cout << "���ۿ�: "; cin >> discount;
            if (projectList.modifyDiscount(typeName, discount)) {
                cout << "�ۿ��ѳɹ��޸�Ϊ " << discount << "��\n";
            }
            else {
                cout << "�޸�ʧ�ܣ�δ�ҵ�����������\n";
            }
            break;
        case 7:
            return; // �������˵�
        default:
            cout << "��Чѡ�����������롣\n";
            break;
        }

        // ��ͣ�Բ鿴���
        cout << "�� Enter ������...\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}



void bill() {
    string customerName, roomNumber, luggageNumber;
    cout << "������˿�����: ";
    cin >> customerName;

    // ��������
    vector<string> guests = loadFromFile("customers.txt");
    vector<string> rooms = loadFromFile("rooms.txt");
    vector<string> projects = loadFromFile("projects.txt");

    // ��ʼ���˵���Ϣ����
    string roomType, checkInTime, projectName;
    double roomPrice = 0, roomDiscount = 1.0, projectPrice = 0, projectDiscount = 1.0;
    bool customerFound = false, roomFound = false;

    // ���ҹ˿���Ϣ
    for (const string& guest : guests) {
        stringstream ss(guest);
        vector<string> fields;
        string field;

        // �ָ���ÿһ�ֶ�
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // ���ڶ��ֶ��Ƿ�Ϊ�˿�����
        if (fields.size() >= 7 && fields[1] == customerName) {
            roomNumber = fields[0];
            checkInTime = fields[4];
            roomType = fields[5];
            luggageNumber = fields[6];
            customerFound = true;
            break;
        }
    }

    if (!customerFound) {
        cout << "δ�ҵ��ù˿���Ϣ��\n";
        return;
    }

    // ���ҷ�����Ϣ
    for (const string& room : rooms) {
        stringstream ss(room);
        vector<string> fields;
        string field;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() >= 4 && fields[0] == roomNumber) {
            roomType = fields[1];
            roomPrice = stod(fields[2]);
            roomDiscount = stod(fields[3]);
            roomFound = true;
            break;
        }
    }

    if (!roomFound) {
        cout << "δ�ҵ��÷�����Ϣ��\n";
        return;
    }

    // ����������Ŀ��Ϣ
    for (const string& project : projects) {
        stringstream ss(project);
        vector<string> fields;
        string field;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() >= 4 && fields[2] == customerName) { // �����3�ֶ��ǿͻ�����
            projectName = fields[0];
            projectPrice = stod(fields[1]);
            projectDiscount = stod(fields[3]);
            break;
        }
    }

    // �����ܼ۸�
    double totalRoomPrice = roomPrice * roomDiscount;
    double totalProjectPrice = projectPrice * projectDiscount;
    double totalPrice = totalRoomPrice + totalProjectPrice;

    // ��ʾ�˵�
    cout << "============================================================================\n";
    cout << "||                        ***�Ƶ���Ϣ����ϵͳ****                        ||\n";
    cout << "||=======================================================================||\n";
    cout << "||                                     �˵�                              ||\n";
    cout << "|| ����: " << customerName << "                                          ||\n";
    cout << "|| �����: " << roomNumber << "                                          ||\n";
    cout << "|| ��������: " << roomType << "                                          ||\n";
    cout << "|| ����۸�: " << fixed << setprecision(2) << roomPrice << "                                          ||\n";
    cout << "|| ��סʱ��: " << checkInTime << "                                          ||\n";
    cout << "|| ������Ŀ: " << projectName << "                                          ||\n";
    cout << "|| ������Ŀ�۸�: " << projectPrice << "                                          ||\n";
    cout << "|| �����ۿ�: " << roomDiscount << "                                          ||\n";
    cout << "|| ������Ŀ�ۿ�: " << projectDiscount << "                                          ||\n";
    cout << "|| ********************************************************************* ||\n";
    cout << "||                                                                       ||\n";
    cout << "|| �ܼ۸�: " << totalPrice << "                                          ||\n";
    cout << "|| ����İ���: " << luggageNumber << "                                          ||\n";
    cout << "===========================================================================\n";
    cout << "�� Enter ������...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // ������뻺����
    cin.get();  // �ȴ��û����� Enter
}



int main() {
    main_menu();
    return 0;
}
