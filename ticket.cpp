#include<bits/stdc++.h>
#include<sstream>
#include<windows.h>
#include<<mysql/mysql.h>
#include<mysqld_error.h>
using namespace std;

const char* HOST="localhost";
const char* USER="root";
const char* PW="your password";
const char* DB="mydb";

class Bookseat {
private:
vector<vector<int>> Seats(5,vector<int>(10,1));
public:
int getstat(int row,int col) {
    if(row<1 || row>5 || col<1 || col>10) {
        return -1;
    }
    return Seats[row-1][col-1];
}

void reserve(int row,int col) {
    if(row<1 || row>5 || col<1 || col>10) {
        return;
    }
    Seats[row-1][col-1]=0;
}

void getDB(MYSQL* conn) {
    string q="SELECT RowNumber,ColNumber,Seats from Ticket";
    if(mysql_query(conn,q.c_str())) {
        cout<<"Error: "<<mysql_error(conn)<<endl;
    }

    MYSQL_RES* result;
    result=mysql_store_result(conn);
    if(!result) {
        cout<<"Error: "<<mysql_error(conn)<<endl;
    }
    MYSQL_ROW row;
    while((row=mysql_fetch_row(result))) {
        int rowNumber=atoi(row[0]);
        int colNumber=atoi(row[1]);
        int SeatStatus=atoi(row[2]);
        Seats[rowNumber-1][colNumber-1]=SeatStatus;
    }
    mysql_free_result(result);
}

void display() {
    cout<<" ";
    for(int i=0;i<10;i++) {
        cout<<i+1<<" ";
    }
    cout<<endl;
    for(int i=0;i<5;i++) {
        cout<<i+1<<" ";
        for(int j=0;j<10;j++) {
            if(Seats[i][j]==1) {
                cout<<"_ ";
            }
            else {
                cout<<"X ";
            }
        }
        cout<<endl;
    }
    cout<<"-------------------------"<<endl;
}

};

int main() {
    Bookseat s;
    MYSQL* conn;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,HOST,USER,PW,DB,3306,NULL,0)) {
        cout<<"Error: "<<mysql_error(conn)<<endl;
    }
    else {
        cout<<"Logged In Database"<<endl;
    }
    Sleep(3000);
    string part_2="CREATE TABLE IF NOT EXISTS Ticket (RowNumber INT, ColNumber INT,Seats INT)";
    if(mysql_query(conn,part_2)) {
        cout<<"Error: "<<mysql_error(conn)<<endl;
    }
    for(int i=0;i<5;i++) {
        for(int j=0;j<10;i++) {
            stringstream ss;
            ss<<"INSERT INTO Ticket(RowNumber,ColNumber,Seats)"<<
            "SELECT '"<<i+1<<"','"<<j+1<<"','1' "<<
            "WHERE NOT EXISTS (SELECT * FROM Ticket WHERE RowNumber='"<<i+1<<"' AND ColNumber='"<<j+1<<"')";
            string insertquery=ss.str();
            if(mysql_query(conn,insertquery.c_str())) {
                cout<<"Error: "<<mysql_error(conn);
            }
        }
    }
    Sleep(3000);
    while(true) {
        system("Cls");
        int ch;
        cout<<endl;
        cout<<"Welcome to Make Revervation at NNNS"<<endl;
        cout<<"***********************************"<<endl;
        cout<<"1. Reserve A Ticket"<<endl;
        cout<<"2. Exit"<<endl<<endl;
        cout<<"Enter you choice: ";
        cin>>ch;
        if(ch==1) {
            s.getDB(conn);
            s.display();
            int r,c;
            cout<<"Enter the Row: ";
            cin>>r;
            cout<<"Enter the column: ";
            cin>>c;
            if(r<1||r>5||c>10||c<1) {
                cout<<"Invalid Seat"<<endl;
                Sleep(3000);
                continue;
            }
            int seatStat=s.getstat(r,c);
            if(seatStat==-1) {
                cout<<"Invalid Seat"<<endl;
                Sleep(3000);
                continue;
            }
            else if(seatStat==0) {
                cout<<"Seat is already Reserved"<<endl;
                Sleep(3000);
                continue;
            }
            s.reserve(r,c);
            stringstream ss;
            ss<<"UPDATE Ticket SET Seat=0 WHERE RowNumber="<<r<<" AND ColNumber="<<c;
            string update=ss.str();
            if(mysql_query(conn,update.c_str())) {
                cout<<"Error: "<<mysql_error(conn)<<endl;
            }
            else {
                cout<<"Your Seat is Booked"<<endl;
                Sleep(5000);
            }
        }
        else {
            break;
        }
    }
    mysql_close(conn);

    return 0;
}