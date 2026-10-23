
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

using namespace std;

// ============================================================================
// PHAN 1: CAU TRUC DU LIEU NEN TANG (ACCOUNT & TRANSACTION)
// THANH VIEN PHU TRACH: PHAN NAM (LEADER) - MSSV: 6651060035
// NHANH GIT: main / TV1-leader-core-hash-table
// NHIEM VU: Mo hinh hoa cau truc Tai khoan va Lich su Giao dich
// ============================================================================

// Cau truc luu vet Giao dich
struct Transaction {
    string transID;     // Ma giao dich
    string type;        // Loai giao dich ("Nap tien", "Rut tien", "Chuyen khoan")
    double amount;      // So tien giao dich
    string timestamp;   // Thoi gian thuc hien giao dich
    string note;        // Ghi chu / So tai khoan nhan

    void display() const {
        cout << "  [" << timestamp << "] " << left << setw(14) << type 
             << " | So tien: " << right << setw(12) << fixed << setprecision(0) << amount << " VND"
             << " | Ghi chu: " << note << endl;
    }
};

// Cau truc Tai khoan Khach hang (Luu trong tung Node cua Bang bam)
struct Account {
    string accountNumber;        // Khoa bam chinh (Key - So tai khoan)
    string fullName;             // Ho va ten khach hang
    string idCard;               // So CCCD / CMND
    double balance;              // So du tai khoan hien tai (VND)
    vector<Transaction> history; // Lich su cac giao dich

    Account(string accNum = "", string name = "", string id = "", double bal = 0.0)
        : accountNumber(accNum), fullName(name), idCard(id), balance(bal) {}

    void displayInfo() const {
        cout << "==================================================\n";
        cout << " So Tai Khoan : " << accountNumber << "\n";
        cout << " Chu Tai Khoan: " << fullName << "\n";
        cout << " So CCCD/CMND : " << idCard << "\n";
        cout << " So Du Hien Tai: " << fixed << setprecision(0) << balance << " VND\n";
        cout << " So Giao Dich  : " << history.size() << " giao dich\n";
        cout << "==================================================\n";
    }
};

// ============================================================================
// PHAN 2: CORE HASH TABLE & REHASH ALGORITHM
// THANH VIEN PHU TRACH: PHAN NAM (LEADER) - MSSV: 6651060035
// NHANH GIT: main / TV1-leader-core-hash-table
// NHIEM VU: Cai dat Bang bam, Separate Chaining, Polynomial Hash, Rehash & Memory Management
// ============================================================================

// Node trong Danh sach lien ket don (Separate Chaining)
struct Node {
    Account data;
    Node* next;

    Node(const Account& acc) : data(acc), next(nullptr) {}
};

class HashTable {
private:
    Node** table;                        // Mang cac con tro tro den DS lien ket
    int capacity;                        // Kich thuoc mang bam
    int size;                            // So luong tai khoan hien co
    const double MAX_LOAD_FACTOR = 0.75; // Nguong kich hoat Rehash

    // 1. Ham bam chuoi Polynomial Rolling Hash Function
    int hashFunction(const string& key) const {
        long long hashValue = 0;
        long long p = 31;          // So nguyen to cho chuoi so
        long long m = 1e9 + 9;     // Mo-dun so lon tranh tran so
        long long p_pow = 1;

        for (char c : key) {
            hashValue = (hashValue + (c - '0' + 1) * p_pow) % capacity;
            p_pow = (p_pow * p) % m;
        }
        return (hashValue % capacity + capacity) % capacity;
    }

    // 2. Thuat toan Rehash mo rong Bang bam tu dong khi Load Factor >= 0.75
    void rehash() {
        int oldCapacity = capacity;
        Node** oldTable = table;

        capacity = capacity * 2 + 1; // Mo rong mang bam (Gap doi + 1)
        table = new Node*[capacity]();
        size = 0;

        for (int i = 0; i < oldCapacity; ++i) {
            Node* current = oldTable[i];
            while (current != nullptr) {
                insertAccount(current->data); // Chen lai vao mang bam moi
                Node* temp = current;
                current = current->next;
                delete temp; // Giai phong Node cu
            }
        }
        delete[] oldTable; // Giai phong mang con tro cu
        cout << "=> [SYSTEM - PHAN NAM] Da Rehash mo rong Bang bam! Kich thuoc moi: " << capacity << endl;
    }

public:
    HashTable(int initCapacity = 11) : capacity(initCapacity), size(0) {
        table = new Node*[capacity]();
    }

    // Destructor giai phong bo nho triet de (Tranh ro ri bo nho - Memory Leak)
    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    double getLoadFactor() const {
        return (double)size / capacity;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    // Chen tai khoan vao Bang bam
    bool insertAccount(const Account& acc) {
        if (getLoadFactor() >= MAX_LOAD_FACTOR) {
            rehash();
        }

        int index = hashFunction(acc.accountNumber);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->data.accountNumber == acc.accountNumber) {
                return false; // Trung so tai khoan
            }
            current = current->next;
        }

        Node* newNode = new Node(acc);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
        return true;
    }

    // Tra cuu tai khoan O(1)
    Account* searchAccount(const string& accNum) {
        int index = hashFunction(accNum);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->data.accountNumber == accNum) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    // Xoa tai khoan khoi Bang bam
    bool deleteAccount(const string& accNum) {
        int index = hashFunction(accNum);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->data.accountNumber == accNum) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // Duyet qua tat ca tai khoan (Ho tro luu file)
    vector<Account*> getAllAccounts() const {
        vector<Account*> result;
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                result.push_back(&(current->data));
                current = current->next;
            }
        }
        return result;
    }
};
// ============================================================================
// PHAN 3: MODULE QUAN LY TAI KHOAN (MO TAI KHOAN, TRA CUU O(1) & BAT LOI)
// THANH VIEN PHU TRACH: NGUYEN PHUONG HOANG TRONG - MSSV: 6651060065
// NHANH GIT: TV2-quan-ly-tai-khoan
// NHIEM VU: Mo tai khoan moi, Tra cuu O(1), Xoa tai khoan, Bat loi nhap lieu
// ============================================================================

namespace AccountManagementModule {

    // Kiem tra chuoi chi chua ky tu so
    bool isNumeric(const string& str) {
        if (str.empty()) return false;
        for (char c : str) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    // Chuc nang 1: Mo tai khoan khach hang moi
    void openNewAccount(HashTable& ht) {
        cout << "\n--- [MODULE HOANG TRONG] MO TAI KHOAN MOI ---\n";
        string accNum, name, idCard;
        double initBalance;

        cout << "Nhap so tai khoan (chuoi chu so): ";
        cin >> accNum;
        if (!isNumeric(accNum) || accNum.length() < 4) {
            cout << "=> ERROR: So tai khoan khong hop le! Phai la chuoi so.\n";
            return;
        }

        if (ht.searchAccount(accNum) != nullptr) {
            cout << "=> ERROR: So tai khoan " << accNum << " da ton tai trong he thong!\n";
            return;
        }

        cin.ignore();
        cout << "Nhap ho va ten khach hang: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "=> ERROR: Ten khach hang khong duoc de rong!\n";
            return;
        }

        cout << "Nhap so CCCD/CMND: ";
        cin >> idCard;

        cout << "Nhap so du ban dau (Toi thieu 50,000 VND): ";
        cin >> initBalance;
        if (initBalance < 50000) {
            cout << "=> ERROR: So du ban dau toi thieu phai tu 50,000 VND!\n";
            return;
        }

        Account newAcc(accNum, name, idCard, initBalance);
        if (ht.insertAccount(newAcc)) {
            cout << "=> THANH CONG: Da mo tai khoan " << accNum << " cho khach hang " << name << "!\n";
        } else {
            cout << "=> ERROR: Khong the them tai khoan vao Bang bam!\n";
        }
    }

    // Chuc nang 2: Tra cuu thong tin tai khoan tuc thoi O(1)
    void searchAccountInfo(HashTable& ht) {
        cout << "\n--- [MODULE HOANG TRONG] TRA CUU TAI KHOAN O(1) ---\n";
        string accNum;
        cout << "Nhap so tai khoan can tra cuu: ";
        cin >> accNum;

        Account* acc = ht.searchAccount(accNum);
        if (acc != nullptr) {
            cout << "=> TIM THAY TAI KHOAN TUC THOI O(1):\n";
            acc->displayInfo();
        } else {
            cout << "=> KHONG TIM THAY tai khoan: " << accNum << endl;
        }
    }

    // Chuc nang 3: Xoa tai khoan
    void closeAccount(HashTable& ht) {
        cout << "\n--- [MODULE HOANG TRONG] XOA TAI KHOAN ---\n";
        string accNum;
        cout << "Nhap so tai khoan can xoa: ";
        cin >> accNum;

        if (ht.deleteAccount(accNum)) {
            cout << "=> THANH CONG: Da xoa tai khoan " << accNum << " khoi he thong.\n";
        } else {
            cout << "=> ERROR: Khong tim thay tai khoan " << accNum << " de xoa.\n";
        }
    }
}

// ============================================================================
// PHAN 4: MODULE GIAO DICH NGAN HANG & SAO KE LICH SU
// THANH VIEN PHU TRACH: VONG THE QUYEN - MSSV: 6651060051
// NHANH GIT: TV3-nghiep-vu-giao-dich
// NHIEM VU: Nap tien, Rut tien, Chuyen khoan an toan, Xuat sao ke giao dich
// ============================================================================

namespace TransactionModule {

    // Ham lay thoi gian thuc he thong
    string getCurrentTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
           << setfill('0') << setw(2) << 1 + ltm->tm_mon << "/"
           << 1900 + ltm->tm_year << " "
           << setfill('0') << setw(2) << ltm->tm_hour << ":"
           << setfill('0') << setw(2) << ltm->tm_min << ":"
           << setfill('0') << setw(2) << ltm->tm_sec;
        return ss.str();
    }

    // Chuc nang 1: Nap tien vao tai khoan
    void deposit(HashTable& ht) {
        cout << "\n--- [MODULE THE QUYEN] NAP TIEN VAO TAI KHOAN ---\n";
        string accNum;
        double amount;

        cout << "Nhap so tai khoan: ";
        cin >> accNum;

        Account* acc = ht.searchAccount(accNum);
        if (acc == nullptr) {
            cout << "=> ERROR: Tai khoan khong ton tai!\n";
            return;
        }

        cout << "Nhap so tien muon nap (VND): ";
        cin >> amount;
        if (amount <= 0) {
            cout << "=> ERROR: So tien nap phai lon hon 0!\n";
            return;
        }

        acc->balance += amount;

        Transaction trans;
        trans.transID = "TX" + to_string(rand() % 900000 + 100000);
        trans.type = "Nap tien";
        trans.amount = amount;
        trans.timestamp = getCurrentTime();
        trans.note = "Nap tien tai quay";

        acc->history.push_back(trans);

        cout << "=> THANH CONG: Da nap " << fixed << setprecision(0) << amount 
             << " VND. So du moi: " << acc->balance << " VND\n";
    }

    // Chuc nang 2: Rut tien tu tai khoan
    void withdraw(HashTable& ht) {
        cout << "\n--- [MODULE THE QUYEN] RUT TIEN TU TAI KHOAN ---\n";
        string accNum;
        double amount;

        cout << "Nhap so tai khoan: ";
        cin >> accNum;

        Account* acc = ht.searchAccount(accNum);
        if (acc == nullptr) {
            cout << "=> ERROR: Tai khoan khong ton tai!\n";
            return;
        }

        cout << "Nhap so tien muon rut (VND): ";
        cin >> amount;
        if (amount <= 0) {
            cout << "=> ERROR: So tien rut phai lon hon 0!\n";
            return;
        }

        if (acc->balance - amount < 50000) {
            cout << "=> ERROR: So du khong du! Phai duy tri so du toi thieu 50,000 VND.\n";
            return;
        }

        acc->balance -= amount;

        Transaction trans;
        trans.transID = "TX" + to_string(rand() % 900000 + 100000);
        trans.type = "Rut tien";
        trans.amount = amount;
        trans.timestamp = getCurrentTime();
        trans.note = "Rut tien tai ATM/Quay";

        acc->history.push_back(trans);

        cout << "=> THANH CONG: Da rut " << fixed << setprecision(0) << amount 
             << " VND. So du con lai: " << acc->balance << " VND\n";
    }

    // Chuc nang 3: Chuyen khoan an toan
    void transfer(HashTable& ht) {
        cout << "\n--- [MODULE THE QUYEN] CHUYEN KHOAN AN TOAN ---\n";
        string srcAccNum, destAccNum;
        double amount;

        cout << "Nhap so tai khoan NGUOI GUI: ";
        cin >> srcAccNum;
        Account* srcAcc = ht.searchAccount(srcAccNum);
        if (srcAcc == nullptr) {
            cout << "=> ERROR: Tai khoan nguoi gui khong ton tai!\n";
            return;
        }

        cout << "Nhap so tai khoan NGUOI NHAN: ";
        cin >> destAccNum;
        if (srcAccNum == destAccNum) {
            cout << "=> ERROR: Khong the chuyen khoan cho chinh minh!\n";
            return;
        }

        Account* destAcc = ht.searchAccount(destAccNum);
        if (destAcc == nullptr) {
            cout << "=> ERROR: Tai khoan nguoi nhan khong ton tai!\n";
            return;
        }

        cout << "Nhap so tien chuyen (VND): ";
        cin >> amount;
        if (amount <= 0) {
            cout << "=> ERROR: So tien chuyen phai lon hon 0!\n";
            return;
        }

        if (srcAcc->balance - amount < 50000) {
            cout << "=> ERROR: So du nguoi gui khong du de chuyen khoan!\n";
            return;
        }

        // Thuc hien giao dich chuyen khoan
        srcAcc->balance -= amount;
        destAcc->balance += amount;

        string timeStr = getCurrentTime();
        string txID = "TX" + to_string(rand() % 900000 + 100000);

        // Luu lich su nguoi gui
        Transaction t1;
        t1.transID = txID;
        t1.type = "Chuyen tien di";
        t1.amount = amount;
        t1.timestamp = timeStr;
        t1.note = "Chuyen toi TK: " + destAccNum + " (" + destAcc->fullName + ")";
        srcAcc->history.push_back(t1);

        // Luu lich su nguoi nhan
        Transaction t2;
        t2.transID = txID;
        t2.type = "Nhan tien den";
        t2.amount = amount;
        t2.timestamp = timeStr;
        t2.note = "Nhan tu TK: " + srcAccNum + " (" + srcAcc->fullName + ")";
        destAcc->history.push_back(t2);

        cout << "=> THANH CONG: Da chuyen " << fixed << setprecision(0) << amount 
             << " VND tu TK " << srcAccNum << " sang TK " << destAccNum << "!\n";
    }

    // Chuc nang 4: In sao ke lich su giao dich
    void printStatement(HashTable& ht) {
        cout << "\n--- [MODULE THE QUYEN] XUAT SAO KE GIAO DICH ---\n";
        string accNum;
        cout << "Nhap so tai khoan can in sao ke: ";
        cin >> accNum;

        Account* acc = ht.searchAccount(accNum);
        if (acc == nullptr) {
            cout << "=> ERROR: Tai khoan khong ton tai!\n";
            return;
        }

        cout << "\n======================================================================\n";
        cout << "                SAO KE LICH SU GIAO DICH NGAN HANG                   \n";
        cout << " So TK: " << acc->accountNumber << " | Chu TK: " << acc->fullName << endl;
        cout << " So du hien tai: " << fixed << setprecision(0) << acc->balance << " VND\n";
        cout << "----------------------------------------------------------------------\n";

        if (acc->history.empty()) {
            cout << "   (Chua co giao dich nao duoc thuc hien)\n";
        } else {
            for (const auto& t : acc->history) {
                t.display();
            }
        }
        cout << "======================================================================\n";
    }
}

// ============================================================================
// PHAN 5: MODULE DOC/GHI FILE & GIAO DIEN CONSOLE MENU
// THANH VIEN PHU TRACH: HUYNH ANH TU - MSSV: 6651060069
// NHANH GIT: TV4-doc-ghi-file-giao-dien
// NHIEM VU: Doc/Ghi du lieu ben vung xuong file CSV, Giao dien Menu Console
// ============================================================================

namespace FileAndUIModule {

    const string DATA_FILE = "bank_data.csv";

    // Chuc nang 1: Luu toan bo du lieu ra File CSV
    void saveData(const HashTable& ht) {
        ofstream outFile(DATA_FILE);
        if (!outFile.is_open()) {
            cout << "=> ERROR: Khong the mo file de ghi du lieu!\n";
            return;
        }

        vector<Account*> accounts = ht.getAllAccounts();
        for (Account* acc : accounts) {
            // Dinh dang dong tai khoan: ACC,accountNumber,fullName,idCard,balance
            outFile << "ACC," << acc->accountNumber << "," << acc->fullName << ","
                    << acc->idCard << "," << fixed << setprecision(0) << acc->balance << "\n";

            // Dinh dang dong giao dich: TRANS,transID,type,amount,timestamp,note
            for (const auto& t : acc->history) {
                outFile << "TRANS," << t.transID << "," << t.type << ","
                        << fixed << setprecision(0) << t.amount << ","
                        << t.timestamp << "," << t.note << "\n";
            }
        }

        outFile.close();
        cout << "=> [MODULE ANH TU] Da luu thanh cong " << accounts.size() 
             << " tai khoan ra file '" << DATA_FILE << "'!\n";
    }

    // Chuc nang 2: Doc khoi phuc du lieu tu File CSV khi khoi dong
    void loadData(HashTable& ht) {
        ifstream inFile(DATA_FILE);
        if (!inFile.is_open()) {
            cout << "=> [MODULE ANH TU] Khong tim thay file du lieu cu. Se khoi tao he thong moi.\n";
            return;
        }

        string line;
        Account* lastAcc = nullptr;
        int accCount = 0;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string type;
            getline(ss, type, ',');

            if (type == "ACC") {
                string accNum, name, idCard, balStr;
                getline(ss, accNum, ',');
                getline(ss, name, ',');
                getline(ss, idCard, ',');
                getline(ss, balStr, ',');

                double bal = stod(balStr);
                Account acc(accNum, name, idCard, bal);
                ht.insertAccount(acc);
                lastAcc = ht.searchAccount(accNum);
                accCount++;
            } else if (type == "TRANS" && lastAcc != nullptr) {
                Transaction t;
                string amtStr;
                getline(ss, t.transID, ',');
                getline(ss, t.type, ',');
                getline(ss, amtStr, ',');
                getline(ss, t.timestamp, ',');
                getline(ss, t.note, ',');

                t.amount = stod(amtStr);
                lastAcc->history.push_back(t);
            }
        }

        inFile.close();
        cout << "=> [MODULE ANH TU] Da nap thanh cong " << accCount 
             << " tai khoan tu file '" << DATA_FILE << "'!\n";
    }

    // Chuc nang 3: Giao dien Menu Console tuong tac
    void showMenu() {
        cout << "\n================================================================\n";
        cout << "     HE THONG QUAN LY TAI KHOAN & GIAO DICH NGAN HANG (DT07)    \n";
        cout << "================================================================\n";
        cout << " 1. Mo tai khoan moi               (Phu trach: Hoang Trong)     \n";
        cout << " 2. Tra cuu tai khoan O(1)         (Phu trach: Hoang Trong)     \n";
        cout << " 3. Xoa tai khoan                  (Phu trach: Hoang Trong)     \n";
        cout << " 4. Nap tien vao tai khoan         (Phu trach: The Quyen)       \n";
        cout << " 5. Rut tien tu tai khoan          (Phu trach: The Quyen)       \n";
        cout << " 6. Chuyen khoan an toan           (Phu trach: The Quyen)       \n";
        cout << " 7. In sao ke lich su giao dich    (Phu trach: The Quyen)       \n";
        cout << " 8. Chay Thu nghiem & Benchmark    (Phu trach: Phu Quoc)        \n";
        cout << " 9. Luu du lieu ra File            (Phu trach: Anh Tu)          \n";
        cout << " 0. Thoat va Luu he thong                                       \n";
        cout << "================================================================\n";
        cout << "Lua chon cua ban (0-9): ";
    }
}
