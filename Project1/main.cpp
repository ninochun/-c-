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

// 管理员节点结构(双链表)
struct AdminNode {
    string account;     // 账号
    string password;    // 密码
    string name;        // 姓名
    AdminNode* prev;    // 指向前一个节点的指针
    AdminNode* next;    // 指向后一个节点的指针

    // 构造函数
    AdminNode(const string& acc, const string& pwd, const string& nm)
        : account(acc), password(pwd), name(nm), prev(nullptr), next(nullptr) {}
};

// 管理员双链表类
class AdminList {
private:
    AdminNode* head;   // 链表头指针
    AdminNode* tail;   // 链表尾指针

public:
    // 构造函数
    AdminList() : head(nullptr), tail(nullptr) {
        loadFromFile("admins.txt");  // 从文件加载数据
    }

    // 析构函数
    ~AdminList() {
        saveToFile("admins.txt");    // 保存数据到文件
        AdminNode* current = head;
        while (current) {
            AdminNode* temp = current;
            current = current->next;
            delete temp;
        }
    }


    // 增加管理员节点
    void addAdmin(const string& account, const string& password, const string& name) {
        if (findByAccount(account)) {
            cout << "账号已存在，无法添加新管理员！" << endl;
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
       
        saveToFile("admins.txt"); // 保存数据到文件
    }

    // 根据账号查找节点信息
    AdminNode* findByAccount(const string& account) const {
        AdminNode* current = head;
        while (current) {
            if (current->account == account) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // 未找到
    }

    // 根据账号修改密码
    bool modifyPassword(const string& account, const string& newPassword) {
        AdminNode* admin = findByAccount(account);
        if (admin) {
            admin->password = newPassword;
            
            saveToFile("admins.txt"); // 保存修改后的数据
            return true;
        }
     
        return false;
    }

    void saveToFile(const string& filename, const vector<string>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件进行写入: " << filename << endl;
        return;
    }
    for (const string& line : data) {
        file << line << endl;
    }
    file.close();
}

    // 根据账号修改姓名
    bool modifyName(const string& account, const string& newName) {
        AdminNode* admin = findByAccount(account);
        if (admin) {
            admin->name = newName;
          
            saveToFile("admins.txt"); // 保存修改后的数据
            return true;
        }
      
        return false;
    }

    // 根据姓名删除节点
    bool deleteByName(const string& name) {
        AdminNode* current = head;
        while (current) {
            if (current->name == name) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                delete current;
               
                saveToFile("admins.txt"); // 保存修改后的数据
                return true;
            }
            current = current->next;
        }
 
        return false;
    }

    // 显示所有管理员信息
    void displayAll() const {
        if (!head) {
            cout << "暂无管理员信息。" << endl;
            return;
        }
        AdminNode* current = head;
        cout << "管理员信息列表：" << endl;
        while (current) {
            cout << "账号: " << current->account << ", 姓名: " << current->name << endl;
            current = current->next;
        }
    }

    // 保存数据到文件
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "无法保存到文件: " << filename << "。请检查文件路径和权限。" << endl;
            return;
        }
        AdminNode* current = head;
        while (current) {
            file << current->account << "," << current->password << "," << current->name << endl;
            current = current->next;
        }
        file.close();
    }

    // 从文件加载数据
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << "。请检查文件路径和权限。" << endl;
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

    // 构造函数
    CustomerNode(const string& rmNum, const string& nm, const string& gnd, const string& ph, const string& chkIn, const string& rmType, const string& lgNum)
        : roomNumber(rmNum), name(nm), gender(gnd), phone(ph), checkInTime(chkIn), roomType(rmType), luggageNumber(lgNum), prev(nullptr), next(nullptr) {}
};
bool isDuplicateRoomAndDate(const vector<string>& guests, const string& roomNumber, const string& checkInDate) {
    for (const string& guest : guests) {
        stringstream ss(guest);
        vector<string> fields;
        string field;

        // 将每行内容分隔成字段
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // 假设第1字段是房间号，第5字段是入住日期
        if (fields.size() >= 5 && fields[0] == roomNumber && fields[4] == checkInDate) {
            return true;
        }
    }
    return false;
}

// 顾客双链表类
class CustomerList {
private:
    CustomerNode* head;
    CustomerNode* tail;

public:
    // 构造函数
    CustomerList() : head(nullptr), tail(nullptr) {
        loadFromFile("customers.txt");  // 从文件加载数据
    }

    // 析构函数
    ~CustomerList() {
        saveToFile("customers.txt");  // 保存数据到文件
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
        std::cerr << "无法打开文件: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }
    file.close();
    return data;
}
   
    // 增加客户信息节点
    void addCustomer(const string& roomNumber, const string& name, const string& gender, const string& phone, const string& checkInDate, const string& roomType, const string& luggageNumber) {
        // 加载所有现有客户数据
        vector<string> guests = loadFromFile("customers.txt");

        // 检查是否已经存在相同房间号和入住日期的记录
        if (isDuplicateRoomAndDate(guests, roomNumber, checkInDate)) {
            cout << "同一房间在相同日期已被预订，无法重复添加。\n";
            return;
        }

        // 检查是否已经存在同名客户
        if (findCustomerByName(name)) {
            cout << "客户已存在，无法重复添加。\n";
            return;
        }

        // 如果没有冲突，添加新客户节点
        CustomerNode* newCustomer = new CustomerNode(roomNumber, name, gender, phone, checkInDate, roomType, luggageNumber);
        if (!head) {
            head = tail = newCustomer;
        }
        else {
            tail->next = newCustomer;
            newCustomer->prev = tail;
            tail = newCustomer;
        }

        cout << "客户信息添加成功！\n";
        saveToFile("customers.txt"); // 保存数据到文件
    }

    // 根据姓名查找客户节点
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

    // 根据姓名修改性别
    void updateGender(const string& name, const string& newGender) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->gender = newGender;
            cout << "性别修改成功！" << endl;
            saveToFile("customers.txt"); // 保存修改后的数据
        }
        else {
            cout << "未找到客户姓名: " << name << endl;
        }
    }

    // 根据姓名修改手机号
    void updatePhone(const string& name, const string& newPhone) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->phone = newPhone;
            cout << "手机号修改成功！" << endl;
            saveToFile("customers.txt"); // 保存修改后的数据
        }
        else {
            cout << "未找到客户姓名: " << name << endl;
        }
    }

    // 根据姓名修改入住时间
    void updateCheckInTime(const string& name, const string& newCheckInTime) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->checkInTime = newCheckInTime;
            cout << "入住时间修改成功！" << endl;
            saveToFile("customers.txt"); // 保存修改后的数据
        }
        else {
            cout << "未找到客户姓名: " << name << endl;
        }
    }

    // 根据姓名修改房间类型
    void updateRoomType(const string& name, const string& newRoomType) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            customer->roomType = newRoomType;
            cout << "房间类型修改成功！" << endl;
            saveToFile("customers.txt"); // 保存修改后的数据
        }
        else {
            cout << "未找到客户姓名: " << name << endl;
        }
    }

    // 根据房间号修改姓名
    void updateNameByRoomNumber(const string& roomNumber, const string& newName) {
        CustomerNode* current = head;
        while (current) {
            if (current->roomNumber == roomNumber) {
                current->name = newName;
                cout << "姓名修改成功！" << endl;
                saveToFile("customers.txt"); // 保存修改后的数据
                return;
            }
            current = current->next;
        }
        cout << "未找到该房间号！" << endl;
    }

    // 删除客户节点
    void deleteCustomerByName(const string& name) {
        CustomerNode* customer = findCustomerByName(name);
        if (customer) {
            if (customer->prev) customer->prev->next = customer->next;
            if (customer->next) customer->next->prev = customer->prev;
            if (customer == head) head = customer->next;
            if (customer == tail) tail = customer->prev;
            delete customer;
            cout << "客户节点删除成功！" << endl;
            saveToFile("customers.txt"); // 保存修改后的数据
        }
        else {
            cout << "未找到客户姓名: " << name << endl;
        }
    }

    // 打印所有客户信息
    void displayCustomers() const {
        CustomerNode* current = head;
        if (!current) {
            cout << "暂无客户信息。" << endl;
            return;
        }
        cout << "客户信息列表：" << endl;
        while (current) {
            cout << "房间号: " << current->roomNumber << ", 姓名: " << current->name
                << ", 性别: " << current->gender << ", 电话: " << current->phone
                << ", 入住时间: " << current->checkInTime << ", 房间类型: " << current->roomType
                << ", 行李寄包柜号: " << current->luggageNumber << endl;
            current = current->next;
        }
    }

    // 保存数据到文件
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "无法保存到文件: " << filename << "。请检查文件路径和权限。" << endl;
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
        cerr << "无法打开文件: " << filename << endl;
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
            cout << "登录成功！欢迎 " << get<2>(admin) << "！" << endl;
            return true;
        }
    }
    cout << "登录失败！请检查账号或密码。" << endl;
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
        cout << "房间节点添加成功！\n";
    }
    void displayStatistics() {
        int totalRooms = 0;
        int occupiedRooms = 0;
        RoomNode* current = head;

        // 遍历链表，统计房间总数和已入住房间数
        while (current != nullptr) {
            totalRooms++;
            if (current->isOccupied) {
                occupiedRooms++;
            }
            current = current->next;
        }

        // 计算入住率
        double occupancyRate = (totalRooms > 0) ? (static_cast<double>(occupiedRooms) / totalRooms) * 100 : 0.0;

        // 打印统计信息
        cout << "房间统计信息：" << endl;
        cout << "总房间数: " << totalRooms << endl;
        cout << "已入住房间数: " << occupiedRooms << endl;
        cout << "入住率: " << occupancyRate << "%" << endl;
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
            cout << "房间信息修改成功！\n";
        }
        else {
            cout << "未找到该房间号！\n";
        }
    }

    void displayRooms() const {
        if (!head) {
            cout << "暂无房间信息。\n";
            return;
        }
        RoomNode* current = head;
        while (current) {
            cout << "房间号: " << current->roomNumber << ", 类型: " << current->type << ", 价格: " << current->price
                << ", 折扣: " << current->discount << ", 入住情况: " << (current->isOccupied ? "已入住" : "未入住") << endl;
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
            cout << "房间节点删除成功！\n";
        }
        else {
            cout << "未找到该房间号！\n";
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "无法打开文件进行写入: " << filename << endl;
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
            cerr << "无法打开文件进行读取: " << filename << endl;
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
        loadFromFile("projects.txt"); // 初始化时加载文件数据
    }

    ~ProjectList() {
        saveToFile("projects.txt"); // 退出时保存数据到文件
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
        cout << "娱乐项目节点添加成功！\n";
        saveToFile("projects.txt"); // 添加项目后保存到文件
    }

    bool modifyPrice(const string& typeName, double newPrice) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->price = newPrice;
                cout << "价格修改成功！\n";
                saveToFile("projects.txt"); // 修改价格后保存到文件
                return true;
            }
            current = current->next;
        }
        cout << "未找到该类型名！\n";
        return false;
    }

    bool modifyCustomerName(const string& typeName, const string& newName) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->customerName = newName;
                cout << "客户姓名修改成功！\n";
                saveToFile("projects.txt"); // 修改客户姓名后保存到文件
                return true;
            }
            current = current->next;
        }
        cout << "未找到该类型名！\n";
        return false;
    }

    bool modifyDiscount(const string& typeName, double newDiscount) {
        ProjectNode* current = head;
        while (current) {
            if (current->typeName == typeName) {
                current->discount = newDiscount;
                cout << "折扣修改成功！\n";
                saveToFile("projects.txt"); // 修改折扣后保存到文件
                return true;
            }
            current = current->next;
        }
        cout << "未找到该类型名！\n";
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
                cout << "娱乐项目节点删除成功！\n";
                saveToFile("projects.txt"); // 删除项目后保存到文件
                return true;
            }
            prev = current;
            current = current->next;
        }
        cout << "未找到该类型名！\n";
        return false;
    }

    void displayProjects() const {
        ProjectNode* current = head;
        if (!current) {
            cout << "暂无娱乐项目信息。\n";
            return;
        }
        while (current) {
            cout << "类型: " << current->typeName << ", 价格: " << current->price
                << ", 客户姓名: " << current->customerName << ", 折扣: " << current->discount << endl;
            current = current->next;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cerr << "无法打开文件进行写入: " << filename << endl;
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
            cerr << "无法打开文件进行读取: " << filename << endl;
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


// 文件加载函数
vector<string> loadFromFile(const string& filename) {
    vector<string> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        data.push_back(line);
    }
    return data;
}

// 主菜单声明
void main_menu();
void admin_menu();
void guest_menu();
void room_menu();
void project_menu();
void bill();

// 主菜单实现
// 创建全局的客户链表
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
        cout << "||   ***酒店信息管理系统**** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. 管理员信息相关       ||\n";
        cout << "|| 2. 办理入住             ||\n";
        cout << "|| 3. 退房结算             ||\n";
        cout << "|| 4. 客户信息相关         ||\n";
        cout << "|| 5. 房间信息相关         ||\n";
        cout << "|| 6. 娱乐项目相关         ||\n";
        cout << "=========================||\n";
        cout << "请选择一个选项: ";
        cin >> choice;

        switch (choice) {
        case 1:  admin_menu(); break;
        case 2: {
            string name, roomNumber, gender, phone, checkInTime, roomType, luggageNumber;
            cout << "办理入住\n";
            cout << "姓名: "; cin >> name;
            cout << "房间号: "; cin >> roomNumber;
            cout << "性别: "; cin >> gender;
            cout << "手机号: "; cin >> phone;
            cout << "入住时间: "; cin >> checkInTime;
            cout << "房间类型: "; cin >> roomType;
            cout << "行李寄包柜号: "; cin >> luggageNumber;

            customerList.addCustomer(roomNumber, name, gender, phone, checkInTime, roomType, luggageNumber);
            break;
        }
        case 3: bill(); break;
        case 4: guest_menu(); break;
        case 5: room_menu(); break;
        case 6: project_menu(); break;
        default: cout << "无效选择，请重新输入。\n"; break;
        }
    }
}


void admin_menu() {
vector<tuple<string, string, string>> adminAccounts = loadAdminAccounts("admins.txt");
    string account, password;
    cout << "请输入账号: ";
    cin >> account;
    cout << "请输入密码: ";
    cin >> password;

    if (verifyAdminLogin(adminAccounts, account, password)) {
        cout << "登录成功！" << endl;
 
        AdminList adminList;
        int choice;
        while (true) {
            // 清屏操作（根据系统选择性使用）
#ifdef _WIN32
            system("CLS");
#else
            system("clear");
#endif

            // 菜单界面
            cout << "============================\n";
            cout << "||                         ||\n";
            cout << "||  ***酒店信息管理系统*** ||\n";
            cout << "||=========================||\n";
            cout << "|| 1. 修改密码             ||\n";
            cout << "|| 2. 修改姓名             ||\n";
            cout << "|| 3. 增加管理员           ||\n";
            cout << "|| 4. 删除管理员           ||\n";
            cout << "|| 5. 显示所有管理员       ||\n";
            cout << "|| 6. 返回主菜单           ||\n";
            cout << "============================\n";
            cout << "请选择一个选项: ";
            cin >> choice;

            // 检查输入有效性
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效，请输入一个数字。\n";
                continue;
            }

            string account, newPassword, newName;

            switch (choice) {
            case 1: // 修改密码
                cout << "请输入要修改账号: ";
                cin >> account;
                cout << "请输入新密码: ";
                cin >> newPassword;
                if (adminList.modifyPassword(account, newPassword)) {
                    cout << "密码修改成功！\n";
                }
                else {
                    cout << "未找到该账号！\n";
                }
                break;

            case 2: // 修改姓名
                cout << "请输入要修改姓名的账号: ";
                cin >> account;
                cout << "请输入新的姓名: ";
                cin.ignore(); // 清除缓冲区
                getline(cin, newName);
                if (adminList.modifyName(account, newName)) {
                    cout << "姓名修改成功！\n";
                }
                else {
                    cout << "未找到该账号！\n";
                }
                break;

            case 3: // 增加管理员
            {
                string password;
                cout << "请输入新管理员的账号: ";
                cin >> account;
                cout << "请输入密码: ";
                cin >> password;
                cout << "请输入姓名: ";
                cin.ignore();
                getline(cin, newName);
                adminList.addAdmin(account, password, newName);
            }
            break;

            case 4: // 删除管理员
                cout << "请输入要删除的管理员姓名: ";
                cin.ignore();
                getline(cin, newName);
                if (adminList.deleteByName(newName)) {
                    cout << "管理员删除成功！\n";
                }
                else {
                    cout << "未找到该姓名！\n";
                }
                break;

            case 5: // 显示所有管理员
                adminList.displayAll();
                break;

            case 6: // 返回主菜单
                return;

            default:
                cout << "无效选择，请重新输入。\n";
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
        cout << "||  ***酒店信息管理系统*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. 根据姓名修改性别     ||\n";
        cout << "|| 2. 根据姓名修改手机号   ||\n";
        cout << "|| 3. 根据姓名修改入住时间 ||\n";
        cout << "|| 4. 根据姓名修改房间类型 ||\n";
        cout << "|| 5. 根据房间号修改姓名   ||\n";
        cout << "|| 6. 增加客户信息节点     ||\n";
        cout << "|| 7. 打印所有节点信息     ||\n";
        cout << "|| 8. 根据姓名删除节点     ||\n";
        cout << "|| 9. 返回主菜单           ||\n";
        cout << "============================\n";
        cout << "请选择一个选项: ";
        cin >> choice;

        // 输入有效性检查
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入一个数字。\n";
            continue;
        }

        switch (choice) {
        case 1: // 根据姓名修改性别
            cout << "请输入顾客姓名: ";
            cin.ignore();
            getline(cin, name);
            cout << "请输入新的性别: ";
            getline(cin, newValue);
            customerList.updateGender(name, newValue);
            break;

        case 2: // 根据姓名修改手机号
            cout << "请输入顾客姓名: ";
            cin.ignore();
            getline(cin, name);
            cout << "请输入新的手机号: ";
            getline(cin, newValue);
            customerList.updatePhone(name, newValue);
            break;

        case 3: // 根据姓名修改入住时间
            cout << "请输入顾客姓名: ";
            cin.ignore();
            getline(cin, name);
            cout << "请输入新的入住时间: ";
            getline(cin, newValue);
            customerList.updateCheckInTime(name, newValue);
            break;

        case 4: // 根据姓名修改房间类型
            cout << "请输入顾客姓名: ";
            cin.ignore();
            getline(cin, name);
            cout << "请输入新的房间类型: ";
            getline(cin, newValue);
            customerList.updateRoomType(name, newValue);
            break;

        case 5: // 根据房间号修改姓名
            cout << "请输入房间号: ";
            cin.ignore();
            getline(cin, roomNumber);
            cout << "请输入新的姓名: ";
            getline(cin, newValue);
            customerList.updateNameByRoomNumber(roomNumber, newValue);
            break;

        case 6: // 增加客户信息节点
        {
            string gender, phone, checkInTime, roomType, luggageNumber;
            cout << "请输入新顾客的姓名: ";
            cin.ignore();
            getline(cin, name);
            cout << "请输入性别: ";
            getline(cin, gender);
            cout << "请输入手机号: ";
            getline(cin, phone);
            cout << "请输入入住时间: ";
            getline(cin, checkInTime);
            cout << "请输入房间类型: ";
            getline(cin, roomType);
            cout << "请输入房间号: ";
            getline(cin, roomNumber);
            cout << "请输入行李寄包柜号: ";
            getline(cin, luggageNumber);
            customerList.addCustomer(roomNumber, name, gender, phone, checkInTime, roomType, luggageNumber);
        }
        break;

        case 7: // 打印所有节点信息
            customerList.displayCustomers();
            break;

        case 8: // 根据姓名删除节点
            cout << "请输入要删除的顾客姓名: ";
            cin.ignore();
            getline(cin, name);
            customerList.deleteCustomerByName(name);
            break;

        case 9: // 返回主菜单
            return;

        default:
            cout << "无效选择，请重新输入。\n";
            break;
        }

        // 暂停以查看输出
        cout << "按回车键继续...\n";
        cin.ignore();
        cin.get();
    }
}




// 房间信息菜单函数
void room_menu() {
    RoomList roomList; // 创建房间列表对象
    int choice;

    while (true) {
#ifdef _WIN32
        system("CLS");
#else
        system("clear");
#endif
        // 输出菜单界面
        system("CLS"); // 清屏（仅限 Windows 系统）
        cout << "============================\n";
        cout << "||                         ||\n";
        cout << "||  ***酒店信息管理系统*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. 根据房间号更改房间类型 ||\n";
        cout << "|| 2. 根据房间号更改房间价格 ||\n";
        cout << "|| 3. 根据房间号更改房间折扣 ||\n";
        cout << "|| 4. 根据房间号更改入住情况 ||\n";
        cout << "|| 5. 打印所有节点信息      ||\n";
        cout << "|| 6. 增加娱乐项目           ||\n";
        cout << "|| 7. 根据房间号删除节点   ||\n";
        cout << "|| 8. 返回主菜单           ||\n";
        cout << "|| 9. 统计                 ||\n";
        cout << "============================\n";
        cout << "请选择一个选项: ";
        cin >> choice;

        // 检查输入有效性
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入一个数字。\n";
            system("pause");
            continue;
        }

        // 定义用户输入变量
        string roomNumber, newValue;
        double newPrice, newDiscount;
        bool newOccupancy;
        bool found = false;

        switch (choice) {
        case 1: // 根据房间号更改房间类型
            cout << "请输入房间号和新的房间类型:\n";
            cout << "房间号: "; cin >> roomNumber;
            cout << "新房间类型: "; cin >> newValue;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->type = newValue;
                });
            break;

        case 2: // 根据房间号更改房间价格
            cout << "请输入房间号和新的房间价格:\n";
            cout << "房间号: "; cin >> roomNumber;
            cout << "新价格: "; cin >> newPrice;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->price = newPrice;
                });
            break;

        case 3: // 根据房间号更改房间折扣
            cout << "请输入房间号和新的折扣:\n";
            cout << "房间号: "; cin >> roomNumber;
            cout << "新折扣: "; cin >> newDiscount;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->discount = newDiscount;
                });
            break;

        case 4: // 根据房间号更改入住情况
            cout << "请输入房间号和新的入住情况（1为已入住，0为未入住）:\n";
            cout << "房间号: "; cin >> roomNumber;
            cout << "入住情况 (1为已入住，0为未入住): "; cin >> newOccupancy;
            roomList.updateRoomInfo(roomNumber, [&](RoomNode* room) {
                room->isOccupied = newOccupancy;
                });
            break;

        case 5: // 打印所有房间信息
            roomList.displayRooms();
            break;

        case 6: // 增加新房间节点
        {
            string type;
            double price, discount;
            bool isOccupied;
            cout << "请输入新房间信息（房间号、类型、价格、折扣、入住情况）:\n";
            cout << "房间号: "; cin >> roomNumber;
            cout << "类型: "; cin >> type;
            cout << "价格: "; cin >> price;
            cout << "折扣: "; cin >> discount;
            cout << "入住情况 (1为已入住，0为未入住): "; cin >> isOccupied;
            roomList.addRoom(roomNumber, type, price, discount, isOccupied);
        }
        break;

        case 7: // 根据房间号删除节点
            cout << "请输入要删除的房间号: ";
            cin >> roomNumber;
            roomList.deleteRoom(roomNumber);
            break;

        case 8: // 返回主菜单
            return;

        case 9: // 统计信息
            roomList.displayStatistics();
            break;

        default:
            cout << "无效选择，请重新输入。\n";
            break;
        }

        // 暂停以便用户查看输出
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
        cout << "||  ***酒店信息管理系统*** ||\n";
        cout << "||=========================||\n";
        cout << "|| 1. 根据类型名更改价格   ||\n";
        cout << "|| 2. 根据类型名更改客户姓名||\n";
        cout << "|| 3. 增加节点             ||\n";
        cout << "|| 4. 根据类型名删除节点   ||\n";
        cout << "|| 5. 打印所有节点信息     ||\n";
        cout << "|| 6. 根据类型名修改折扣   ||\n";
        cout << "|| 7. 返回主菜单           ||\n";
        cout << "============================\n";
        cout << "请选择一个选项: ";
        cin >> choice;

        string typeName, customerName;
        double price, discount;

        switch (choice) {
        case 1:
            cout << "请输入娱乐项目类型名和新的价格:\n";
            cout << "类型名: "; cin >> typeName;
            cout << "新价格: "; cin >> price;
            if (projectList.modifyPrice(typeName, price)) {
                cout << "价格已成功修改为 " << price << "。\n";
            }
            else {
                cout << "修改失败，未找到该类型名。\n";
            }
            break;
        case 2:
            cout << "请输入娱乐项目类型名和新的客户姓名:\n";
            cout << "类型名: "; cin >> typeName;
            cout << "新客户姓名: "; cin >> customerName;
            if (projectList.modifyCustomerName(typeName, customerName)) {
                cout << "客户姓名已成功修改为 " << customerName << "。\n";
            }
            else {
                cout << "修改失败，未找到该类型名。\n";
            }
            break;
        case 3:
            cout << "请输入新娱乐项目信息（类型名、价格、客户姓名、折扣）:\n";
            cout << "类型名: "; cin >> typeName;
            cout << "价格: "; cin >> price;
            cout << "客户姓名: "; cin >> customerName;
            cout << "折扣: "; cin >> discount;
            projectList.addProject(typeName, price, customerName, discount);
            cout << "新娱乐项目已成功添加。\n";
            break;
        case 4:
            cout << "请输入要删除的娱乐项目类型名: ";
            cin >> typeName;
            if (projectList.deleteProject(typeName)) {
                cout << "项目已成功删除。\n";
            }
            else {
                cout << "删除失败，未找到该类型名。\n";
            }
            break;
        case 5:
            cout << "当前所有娱乐项目信息：\n";
            projectList.displayProjects();
            break;
        case 6:
            cout << "请输入娱乐项目类型名和新的折扣:\n";
            cout << "类型名: "; cin >> typeName;
            cout << "新折扣: "; cin >> discount;
            if (projectList.modifyDiscount(typeName, discount)) {
                cout << "折扣已成功修改为 " << discount << "。\n";
            }
            else {
                cout << "修改失败，未找到该类型名。\n";
            }
            break;
        case 7:
            return; // 返回主菜单
        default:
            cout << "无效选择，请重新输入。\n";
            break;
        }

        // 暂停以查看输出
        cout << "按 Enter 键继续...\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}



void bill() {
    string customerName, roomNumber, luggageNumber;
    cout << "请输入顾客姓名: ";
    cin >> customerName;

    // 加载数据
    vector<string> guests = loadFromFile("customers.txt");
    vector<string> rooms = loadFromFile("rooms.txt");
    vector<string> projects = loadFromFile("projects.txt");

    // 初始化账单信息变量
    string roomType, checkInTime, projectName;
    double roomPrice = 0, roomDiscount = 1.0, projectPrice = 0, projectDiscount = 1.0;
    bool customerFound = false, roomFound = false;

    // 查找顾客信息
    for (const string& guest : guests) {
        stringstream ss(guest);
        vector<string> fields;
        string field;

        // 分隔出每一字段
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // 检查第二字段是否为顾客姓名
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
        cout << "未找到该顾客信息。\n";
        return;
    }

    // 查找房间信息
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
        cout << "未找到该房间信息。\n";
        return;
    }

    // 查找娱乐项目信息
    for (const string& project : projects) {
        stringstream ss(project);
        vector<string> fields;
        string field;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() >= 4 && fields[2] == customerName) { // 假设第3字段是客户姓名
            projectName = fields[0];
            projectPrice = stod(fields[1]);
            projectDiscount = stod(fields[3]);
            break;
        }
    }

    // 计算总价格
    double totalRoomPrice = roomPrice * roomDiscount;
    double totalProjectPrice = projectPrice * projectDiscount;
    double totalPrice = totalRoomPrice + totalProjectPrice;

    // 显示账单
    cout << "============================================================================\n";
    cout << "||                        ***酒店信息管理系统****                        ||\n";
    cout << "||=======================================================================||\n";
    cout << "||                                     账单                              ||\n";
    cout << "|| 姓名: " << customerName << "                                          ||\n";
    cout << "|| 房间号: " << roomNumber << "                                          ||\n";
    cout << "|| 房间类型: " << roomType << "                                          ||\n";
    cout << "|| 房间价格: " << fixed << setprecision(2) << roomPrice << "                                          ||\n";
    cout << "|| 入住时间: " << checkInTime << "                                          ||\n";
    cout << "|| 娱乐项目: " << projectName << "                                          ||\n";
    cout << "|| 娱乐项目价格: " << projectPrice << "                                          ||\n";
    cout << "|| 房间折扣: " << roomDiscount << "                                          ||\n";
    cout << "|| 娱乐项目折扣: " << projectDiscount << "                                          ||\n";
    cout << "|| ********************************************************************* ||\n";
    cout << "||                                                                       ||\n";
    cout << "|| 总价格: " << totalPrice << "                                          ||\n";
    cout << "|| 行李寄包柜: " << luggageNumber << "                                          ||\n";
    cout << "===========================================================================\n";
    cout << "按 Enter 键继续...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清除输入缓冲区
    cin.get();  // 等待用户按下 Enter
}



int main() {
    main_menu();
    return 0;
}
