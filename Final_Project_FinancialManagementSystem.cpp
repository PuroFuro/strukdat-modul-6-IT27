#include <bits/stdc++.h>
using namespace std;
string Currency = "RP";

void log(const char *msg){
    time_t now = time(0);
    struct tm* tm = localtime(&now);    
    ofstream out( "Financial.txt",ios::app);
    out << tm->tm_year + 1900 << '/' << tm->tm_mon + 1 << '/' << tm->tm_mday
         << ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << ": ";
    out << msg << "\n";
    out.close();
}

class Finance{
private:
    int id;
    string tanggal, deskripsi, tipe;
    double jumlah;

public:
    Finance(int id, string tanggal, string deskripsi, string tipe, double jumlah){
        this->id = id;
        this->tanggal = tanggal;
        this->deskripsi = deskripsi;
        this->tipe = tipe;
        this->jumlah = jumlah;
    }
    
    int getid(){
        return id;
    }
    string gettanggal() const {
        return tanggal;
    }
    string getdeskripsi(){
        return deskripsi;
    }
    string gettipe(){
        return tipe;
    }
    int getjumlah(){
        return jumlah;
    }

    bool isEqual(Finance& other){
        return id == other.id;
    }
};

class Income : public Finance {
public:
    Income(int id, string tanggal, string deskripsi, string tipe, double jumlah)
        : Finance(id, tanggal, deskripsi, "Income", jumlah) {}
};

class Expense : public Finance {
public:
    Expense(int id, string tanggal, string deskripsi, string tipe,double jumlah)
        : Finance(id, tanggal, deskripsi, "Expense", jumlah) {}
};

class FNS{
private:
    vector<Finance> finances;
    int next = 1;
    //Function for filtering the type of transaction
    vector<Finance> filtertype(string type){
        vector<Finance> result;
        for(Finance finance : finances){
            if(finance.gettipe() == type){
                result.push_back(finance);
            }
        }
        return result;
    }

    //Function for filtering the transaction by month
    vector<Finance> filtermonth(vector<Finance>& fi_nance, string& month){
        vector<Finance> result;
        
        for(Finance finance : fi_nance){
            if(finance.gettanggal().substr(0, 7) == month){
                result.push_back(finance);
            }
        }
        return result;
    }

    //Function for calculating the average of the transaction per month
    void average(vector<Finance> data, string Currency){
        string Cur_type;
        if(Currency == "RP") Cur_type = "RP ";
        else if(Currency == "$") Cur_type = "$ ";
        double total = 0.0;
        int count = 0;
        for(Finance finance : data){
            total += finance.getjumlah();
            count++;
        }
        if (Currency == "$") total = total * 0.000071;
        cout << "\033[31m" <<"\nThe total of the transaction per month is: " << "\033[1m"<< Cur_type << total << "\n" << "\033[0m";
        cout << "\033[36m" <<"The average of the transaction per month is: " << "\033[1m" << Cur_type << total / count << "\n" << "\033[0m";
    }
    //function to update the value in the private class with the updated/deleted value
    void updatebefore(vector<Finance> data){
        finances = data;
    }

    void print(vector<Finance> data, string Currency){
        int count = 1;
        string Cur_type;
        if(Currency == "RP") Cur_type = "RP ";
        else if(Currency == "$"){
            Cur_type = "$ ";

        }
        
        cout << left << setw(5) << "ID"
             << setw(5) << "No"
             << setw(12) << "Date"
             << setw(20) << "Description"
             << setw(15) << "Amount"
             << "Type\n";
        cout << "-----------------------------------------------------------------\n";
        for (Finance finance : data) {
            double money = finance.getjumlah();
            if(Currency == "$") money = money * 0.000071;
            cout << left << setw(5) << finance.getid()
                 << setw(5) << count++
                 << setw(12) << finance.gettanggal()
                 << setw(20) << finance.getdeskripsi()
                 << "\033[32m" <<Cur_type <<"\033[32m"<< setw(12) << money << "\033[0m"
                 << finance.gettipe() << "\n";
        }
    }

public:

    void create(){
        int choice;
        string date, description, type;
        char namelog[1000] = "Created the transaction ";
        double amount;

        cout << "What is the type of transaction?\n1. Income\n2. Expense\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            type = "Income";

        } else if (choice == 2) {
            type = "Expense";
        } else {
            cout << "Invalid choice.\n";
            return;
        }
        
        cout << "Amount gotten (In Rupiah): ";
        cin >> amount;
        cout << "Description for the "<< type << ": ";
        cin >> description;
        cout << "Date given (yyyy-mm-dd): ";
        cin >> date;

        if(type == "Income"){
            finances.push_back(Income(next, date, description, type, amount));
        } else if(type == "Expense"){
            finances.push_back(Expense(next, date, description, type, amount));
        }
        cout << "Data has been set!\n";
        next++; 
        strcat(namelog, type.c_str());
        log(namelog);
    }

    void read(){
        
        vector<Finance> tipe;
        int choice;
        cout << "Which of the following do you want to see?\n1. Income\n2. Expense\n3. Both\nChoice: ";
        cin >> choice;

        if(choice == 1){
            tipe = filtertype("Income");
            log("Read the transaction Income.");

        } else if(choice == 2){
            tipe = filtertype("Expense");
            log("Read the transaction Expense.");

        } else if(choice == 3){
            tipe = finances;
            log("Read the transaction Both.");

        } else {
            cout << "Invalid choice.\n";
        }

        cout << "Which month do you want to see? (yyyy-mm):\n";
        string month;
        cin >> month;

        vector<Finance> filtered;
        filtered = filtermonth(tipe, month);
        print(filtered, Currency);
        average(filtered, Currency);
        
    }

    void update(){
        cout << "Which month do you want to update the transaction? (yyyy-mm): ";
        string month;
        cin >> month;

        vector<Finance> filtered;
        filtered = filtermonth(finances, month);
        print(filtered, Currency);

        cout << "Which transaction do you want to update? (ID): ";
        int id;
        cin >> id;
        // Algorithm to find the transaction
        int index = -1;
        for (int i = 0; i < filtered.size(); i++) {
            if (filtered[i].getid() == id) {
            index = i;
            break;
            }
        }

        if (index != -1) {
            cout << "Transaction found.\n";
            cout << "What do you want to update?\n1. Date\n2. Description\n3. Amount\n4. Everything\nChoice: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                cout << "New date (yyyy-mm-dd): ";
                string date;
                cin >> date;
                filtered[index] = Finance(filtered[index].getid(), date, filtered[index].getdeskripsi(), filtered[index].gettipe(), filtered[index].getjumlah());
                cout << "Date has been updated." << endl;

            } else if (choice == 2) {
                cout << "New description: ";
                string description;
                cin >> description;
                filtered[index] = Finance(filtered[index].getid(), filtered[index].gettanggal(), description, filtered[index].gettipe(), filtered[index].getjumlah());
                cout << "Description has been updated." << endl;

            } else if (choice == 3) {
                cout << "New amount (In Rupiah): ";
                double amount;
                cin >> amount;
                filtered[index] = Finance(filtered[index].getid(), filtered[index].gettanggal(), filtered[index].getdeskripsi(), filtered[index].gettipe(), amount);
                cout << "Amount has been updated." << endl;
           
            ///if else to change everything
            } else if (choice == 4) {
                cout << "New date (yyyy-mm-dd): ";
                string date;
                cin >> date;
                cout << "New description: ";
                string description;
                cin >> description;
                cout << "New amount (In Rupiah): ";
                double amount;
                cin >> amount;
                filtered[index] = Finance(filtered[index].getid(), date, description, filtered[index].gettipe(), amount);
                cout << "Everything has been updated." << endl;
           
            }
            
            else {
                cout << "Invalid choice.\n";
            }
            log("Update from the transaction has been made.");
            updatebefore(filtered);
        } else {
            cout << "Transaction not found.\n";
        }
    }

    void bokek(){
        cout << "\nInitiating mode bokek\n\n";
        vector<Finance> data = finances;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].getid() == i + 1) {
                data[i] = Finance(data[i].getid(), data[i].gettanggal(), data[i].getdeskripsi(), data[i].gettipe(), 0);
                updatebefore(data);
            }
        }
    }

    void Delete(){
        cout << "Which month do you want to delete the transaction? (yyyy-mm): ";
        string month;
        cin >> month;

        vector<Finance> filtered;
        filtered = filtermonth(finances ,month);
        print(filtered, Currency);

        cout << "Which transaction do you want to delete? (ID): ";
        int id;
        cin >> id;
        // Algorithm to find the transaction
        int index = -1;
        for (int i = 0; i < filtered.size(); i++) {
            if (filtered[i].getid() == id) {
            index = i;
            break;
            }
        }

        if (index != -1) {
            cout << "Transaction found.\n";
            finances.erase(finances.begin() + index);
            cout << "Transaction has been deleted.\n";
            log("Transaction has been deleted.");
        } else {
            cout << "Transaction not found.\n";
        }
    }

};

int main(){
    FNS fns;
    int choice, run;

    while(run){
        cout << "What do you want to do?\n1. Create a transaction\n2. Read the transaction\n3. Update the transaction\n4. Delete the transaction\n5. Change Currency\n6. Bokek\n7. Exit\nChoice: ";
        cin >> choice;

        if(choice == 1){
            fns.create();
        } else if(choice == 2){
            fns.read();
        } else if(choice == 3){
            fns.update();
        } else if(choice == 4){
            fns.Delete();
        } else if(choice == 5){
            cout << "What currency do you want to change to?\n(1) RP\n(2) Dollars\n: ";
            int choice;
            cin >> choice;
            if(choice == 1){
                Currency = "RP";
            } else if(choice == 2){
                Currency = "$";
            } else cout << "Invalid choice.\n";
            
        } else if(choice == 6){
            fns.bokek();
            log("Bokek T^T\n　　　　　／＞　　フ\n　　　　　| 　_　 _ l\n　 　　　／` ミ＿xノ\n　　 　 /　　　 　 |\n　　　 /　 ヽ　　 ﾉ\n　 　 │　　| | |");
        } else if(choice == 7){
            run = 0;
        } else cout << "Invalid choice.\n";
    }
}