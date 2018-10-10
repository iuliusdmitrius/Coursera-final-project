#include <set>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
using namespace std;

class Date {
public:
  Date() {}

  Date(int y, int m, int d){
    if(m > 12 || m <= 0){
      throw runtime_error("Month value is invalid: " + to_string(m));
    }
    if((d > 31) || d <= 0){
      throw runtime_error("Day value is invalid: " + to_string(d));
    }

    year  = y;
    month = m;
    day   = d;
  }
  int GetYear() const{
    return year;
  };
  int GetMonth() const{
    return month;
  };
  int GetDay() const{
    return day;
  };
private:
  int year;
  int month;
  int day;
};

bool operator<(const Date& lhs, const Date& rhs){
  if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() == rhs.GetMonth()) {
			return lhs.GetDay() < rhs.GetDay();
		}
		return lhs.GetMonth() < rhs.GetMonth();
	}
	return lhs.GetYear() < rhs.GetYear();

}

void DateFormat(const string& st, Date& input){
  stringstream date_ss(st);
  int y, m, d;
  date_ss >> y;
  if(date_ss.peek() != '-'){
    throw runtime_error("Wrong date format: " + st);
  } else{
    date_ss.ignore(1);
  }
  date_ss >> m;
  if(date_ss.peek() != '-'){
    throw runtime_error("Wrong date format: " + st);
  } else{
    date_ss.ignore(1);
  }
  date_ss >> d;
  if(date_ss.peek() != ' ' && date_ss.peek() != EOF){
    throw runtime_error("Wrong date format: " + st);
  } else{
    date_ss.ignore(1);
  }
  input = Date(y, m ,d);
}

ostream& operator<<(ostream& potok, const Date& output){
  potok << setfill('0');
  potok << setw(4) << output.GetYear() << '-';
  potok << setw(2) << output.GetMonth() << '-';
  potok << setw(2) << output.GetDay();
  return potok;
}

class Database {
public:
  void AddEvent(const Date& date, const string& event) {
    database[date].insert(event);
  };
  bool DeleteEvent(const Date& date, const string& event) {
    if(database.count(date)) {
      if(database[date].count(event)) {
        database.at(date).erase(event);
        return true;
      } else return false;
    } else return false;
  };

  int DeleteDate(const Date& date){
    int n;

   /*if(database.at(date).size() > 0) {
      n = database.at(date).size();
      database.erase(date);
      return n;
    } else return 0;*/

    try {
      n = database.at(date).size();
    } catch(exception& ex) {
      n = 0;
    }
    if(n) {
      database.erase(date);
    }
    return n;
  };

  bool Find(const Date& date) const {
    int n = database.count(date);
    if(n) {
      for(auto item : database.at(date)) {
        cout << item << endl;
      }
      return true;
    }
    return false;
  };

  void Print() const{
    for(auto item_gen : database){
      for(auto item_set : item_gen.second){
        cout << item_gen.first << ' ' << item_set << endl;
      }
    }
  };
private:
  map<Date,set<string>> database;
};

int main() {
  Database d_b;

  string command;
 try {
  while(getline(cin, command)) {
    stringstream st(command);
    string operation;
    string date1;
    string event1;
    Date date;
    st >> operation;
    if(operation == "Add") {
        st >> date1;
        st >> event1;
        //if(!event1.empty()){
         DateFormat(date1, date);
         d_b.AddEvent(date, event1);
        //}
    } else if(operation == "Del") {
        st >> date1;
        DateFormat(date1, date);
        if(st.eof()) {
          cout << "Deleted " << d_b.DeleteDate(date) << " events" << endl;
        } else {
          st >> event1;
          if(d_b.DeleteEvent(date,event1)) {
            cout << "Deleted successfully" << endl;
          } else {
            cout << "Event not found" << endl;
          }
        }
    } else if(operation == "Find") {
        st >> date1;
        DateFormat(date1, date);
        d_b.Find(date);
    } else if(operation == "Print") {
        d_b.Print();
    } else {
      if(!command.empty()) {
        cout << "Unknown command: " << operation;

      }
    }
  }
 } catch(exception& ex){
  cout << ex.what() << endl;
 }

  return 0;
}
