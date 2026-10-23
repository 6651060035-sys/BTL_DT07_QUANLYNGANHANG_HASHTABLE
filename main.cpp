
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
// PH?N 1: C?U TRÚC D? LI?U N?N T?NG (ACCOUNT & TRANSACTION)
// THÀNH VIÊN PH? TRÁCH: PHAN NAM (LEADER) - MSSV: 6651060035
// NHÁNH GIT: main / TV1-leader-core-hash-table
// NHI?M V?: Mô hình hóa c?u trúc Tài kho?n và L?ch s? Giao d?ch
// ============================================================================

// C?u trúc luu v?t Giao d?ch
struct Transaction {
    string transID;     // Mã giao d?ch
    string type;        // Lo?i giao d?ch ("N?p ti?n", "Rút ti?n", "Chuy?n kho?n")
    double amount;      // S? ti?n giao d?ch
    string timestamp;   // Th?i gian th?c hi?n giao d?ch
    string note;        // Ghi chú / S? tài kho?n nh?n

    void display() const {
        cout << "  [" << timestamp << "] " << left << setw(14) << type 
             << " | S? ti?n: " << right << setw(12) << fixed << setprecision(0) << amount << " VND"
             << " | Ghi chú: " << note << endl;
    }
};

// C?u trúc Tài kho?n Khách hàng (Luu trong t?ng Node c?a B?ng bam)
struct Account {
    string accountNumber;        // Khóa bam chính (Key - S? tài kho?n)
    string fullName;             // H? và tên khách hàng
    string idCard;               // S? CCCD / CMND
    double balance;              // S? du tài kho?n hi?n t?i (VND)
    vector<Transaction> history; // L?ch s? các giao d?ch

    Account(string accNum = "", string name = "", string id = "", double bal = 0.0)
        : accountNumber(accNum), fullName(name), idCard(id), balance(bal) {}

    void displayInfo() const {
        cout << "==================================================\n";
        cout << " S? Tài Kho?n : " << accountNumber << "\n";
        cout << " Ch? Tài Kho?n: " << fullName << "\n";
        cout << " S? CCCD/CMND : " << idCard << "\n";
        cout << " S? Du Hi?n T?i: " << fixed << setprecision(0) << balance << " VND\n";
        cout << " S? Giao D?ch  : " << history.size() << " giao d?ch\n";
        cout << "==================================================\n";
    }
};

// ============================================================================
// PH?N 2: CORE HASH TABLE & REHASH ALGORITHM
// THÀNH VIÊN PH? TRÁCH: PHAN NAM (LEADER) - MSSV: 6651060035
// NHÁNH GIT: main / TV1-leader-core-hash-table
// NHI?M V?: Cài d?t B?ng bam, Separate Chaining, Polynomial Hash, Rehash & Memory Management
// ============================================================================

// Node trong Danh sách liên k?t don (Separate Chaining)
struct Node {
    Account data;
    Node* next;

    Node(const Account& acc) : data(acc), next(nullptr) {}
};

class HashTable {
private:
    Node** table;                        // M?ng các con tr? tr? d?n DS liên k?t
    int capacity;                        // Kích thu?c m?ng bam
    int size;                            // S? lu?ng tài kho?n hi?n có
    const double MAX_LOAD_FACTOR = 0.75; // Ngu?ng kích ho?t Rehash

    // 1. Hàm bam chu?i Polynomial Rolling Hash Function
    int hashFunction(const string& key) const {
        long long hashValue = 0;
        long long p = 31;          // S? nguyên t? cho chu?i s?
        long long m = 1e9 + 9;     // Mô-dun s? l?n tránh tràn s?
        long long p_pow = 1;

        for (char c : key) {
            hashValue = (hashValue + (c - '0' + 1) * p_pow) % capacity;
            p_pow = (p_pow * p) % m;
        }
        return (hashValue % capacity + capacity) % capacity;
    }

    // 2. Thu?t toán Rehash m? r?ng B?ng bam t? d?ng khi Load Factor >= 0.75
    void rehash() {
        int oldCapacity = capacity;
        Node** oldTable = table;

        capacity = capacity * 2 + 1; // M? r?ng m?ng bam (G?p dôi + 1)
        table = new Node*[capacity]();
        size = 0;

        for (int i = 0; i < oldCapacity; ++i) {
            Node* current = oldTable[i];
            while (current != nullptr) {
                insertAccount(current->data); // Chèn l?i vào m?ng bam m?i
                Node* temp = current;
                current = current->next;
                delete temp; // Gi?i phóng Node cu
            }
        }
        delete[] oldTable; // Gi?i phóng m?ng con tr? cu
        cout << "=> [SYSTEM - PHAN NAM] Ðã Rehash m? r?ng B?ng bam! Kích thu?c m?i: " << capacity << endl;
    }

public:
    HashTable(int initCapacity = 11) : capacity(initCapacity), size(0) {
        table = new Node*[capacity]();
    }

    // Destructor gi?i phóng b? nh? tri?t d? (Tránh rò r? b? nh? - Memory Leak)
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

    // Chèn tài kho?n vào B?ng bam
    bool insertAccount(const Account& acc) {
        if (getLoadFactor() >= MAX_LOAD_FACTOR) {
            rehash();
        }

        int index = hashFunction(acc.accountNumber);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->data.accountNumber == acc.accountNumber) {
                return false; // Trùng s? tài kho?n
            }
            current = current->next;
        }

        Node* newNode = new Node(acc);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
        return true;
    }

    // Tra c?u tài kho?n O(1)
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

    // Xóa tài kho?n kh?i B?ng bam
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

    // Duy?t qua t?t c? tài kho?n (H? tr? luu file)
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
