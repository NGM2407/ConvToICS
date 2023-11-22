/*
    Compile using g++ -Wall -c ConvToICS.cpp
    Build using g++ -o ConvToICS ConvToICS.o
*/

#define __STDC_WANT_LIB_EXT1__ 1
#include<iostream>
#include<fstream>
#include<cstring>
#include<string.h>
#include<stdlib.h>
#include<cstddef>
#include<ctime>
#include<locale.h>
#include<codecvt>
using namespace std;

wstring convertString(string str) {
    size_t size = str.length() + 1;
    size_t outSize;
    wchar_t* output = new wchar_t[size];
    mbstowcs_s(&outSize, output, size, str.c_str(), size - 1);
    return output;
}

struct Time {
    unsigned int YEAR = 0;
    unsigned int MONTH = 0;
    unsigned int DAY = 0;
    unsigned int HOUR = 9999;
    unsigned int MINUTES = 9999;
};

class Event {
private:
    bool ALLDAY = true;
    Time begin;
    Time end;
    string TOUR = "";
    string NOTE = "";

public:
    Event(void) {};
    bool getDate(char* cstr);
    bool increaseDate(void);
    bool getTour(char* cstr);
    bool getTime(char* cstr);
    bool getNote(char* cstr);
    bool saveEvent(string path);

    string getTimestamp(void);
    string getUID(void);
    wstring Date(Time t);
    wstring Begin(void);
    wstring End(void);
    string Tour(void);
    string Note(void);
};

wstring Event::Date(Time t) {   
    wstring date = L"";
    
    date.insert(0, to_wstring(t.DAY));
    if (t.DAY < 10) {
        date.insert(0, L"0");
    }
    date.insert(0, to_wstring(t.MONTH));
    if (t.MONTH < 10) {
        date.insert(0, L"0");
    }
    date.insert(0, to_wstring(t.YEAR));
    return date;
}

wstring Event::Begin(void) {
    wstring hour = L"";
    wstring minutes = L"";
    if ((begin.HOUR != 9999) && (begin.MINUTES != 9999)) {
        hour = to_wstring(begin.HOUR);
        if (begin.HOUR < 10) {
            hour.insert(0, L"0");
        }
        minutes = to_wstring(begin.MINUTES);
        if (begin.MINUTES < 10) {
            minutes.insert(0, L"0");
        }
        return hour + minutes;
    }
    else {
        return L"";
    }
}

wstring Event::End(void) {
    wstring hour = L"";
    wstring minutes = L"";
    if ((end.HOUR != 9999) && (end.MINUTES != 9999)) {
        hour = to_wstring(end.HOUR);
        if (end.HOUR < 10) {
            hour.insert(0, L"0");
        }
        minutes = to_wstring(end.MINUTES);
        if (end.MINUTES < 10) {
            minutes.insert(0, L"0");
        }
        return hour + minutes;
    }
    else {
        return L"";
    }
}

string Event::Tour(void) {
    return TOUR;
}

string Event::Note(void) { 
    return NOTE;
}

string Event::getTimestamp(void) {
    time_t now = time(NULL);
    struct tm ltm;
    localtime_s(&ltm, &now);
    string year, month, day, hour, minutes, seconds;
    year = to_string(1900 + ltm.tm_year);
    if (ltm.tm_mon < 10) {
        month = "0" + to_string(1 + ltm.tm_mon);
    }
    else {
        month = to_string(1 + ltm.tm_mon);
    }
    if (ltm.tm_mday < 10) {
        day = "0" + to_string(ltm.tm_mday);
    }
    else {
        day = to_string(ltm.tm_mday);
    }
    if (ltm.tm_hour < 10) {
        hour = "0" + to_string(ltm.tm_hour);
    }
    else {
        hour = to_string(ltm.tm_hour);
    }
    if (ltm.tm_min < 10) {
        minutes = "0" + to_string(ltm.tm_min);
    }
    else {
        minutes = to_string(ltm.tm_min);
    }
    if (ltm.tm_sec < 10) {
        seconds = "0" + to_string(ltm.tm_sec);
    }
    else {
        seconds = to_string(ltm.tm_sec);
    }

    return year + month + day + "T" + hour + minutes + seconds + "Z";
}

string Event::getUID(void) {
    string uid = "";
    char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
    for (int i = 0; i < 128; i++) {
        uid += hex_characters[rand() % 16];
    }
    uid.insert(64, "\n ");
    return uid;
}

bool Event::getDate(char* cstr) {
    //string date = "";
    unsigned int count = 0;
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " .:";
    unsigned int year;
    unsigned int month;
    unsigned int day;

    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        switch (count) {
        case 1:
            //date.insert(0, token);
            day = stoi(token);
            begin.DAY = day;
            end.DAY = day;
            break;

        case 2:
            //date.insert(0, token);
            month = stoi(token);
            begin.MONTH = month;
            end.MONTH = month;
            break;

        case 3:
            //date.insert(0, token);
            year = stoi(token);
            begin.YEAR = year;
            end.YEAR = year;
            break;

        default:
            return false;
            //exit(1);
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }
    return true;
}

bool Event::increaseDate(void) {
    //cout << "Entered increaseDate!" << endl;
    end.DAY += 1;
    //cout<< "Day increased"<<endl;
    switch (end.MONTH)
    {
    case 1:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 2:
        if ((end.YEAR % 4) == 0) {
            // leap year
            if (end.DAY > 29) {
                end.MONTH += 1;
                end.DAY = 1;
            }
        }
        else {
            if (end.DAY > 28) {
                end.MONTH += 1;
                end.DAY = 1;
            }
        }
        break;
    case 3:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 4:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 5:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 6:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 7:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 8:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 9:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 10:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 11:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 12:
        if (end.DAY > 31) {
            end.YEAR += 1;
            end.MONTH = 1;
            end.DAY = 1;
        }
        break;
    default:
        return false;
    }
    return true;
}

bool Event::getTour(char* cstr) {
    if (strcmp(cstr, "F") == 0) {
        TOUR = "FRI";
        return true;
    }
    else if (strcmp(cstr, "H") == 0) {
        TOUR = "HVILEDAG";
        return true;
    }
    else {
        TOUR = cstr;
        return true;
    }
    return false;
}

bool Event::getTime(char* cstr) {
    //string begin = "";   //DTSTART;TZID=Romance Standard Time:20230313T190000\nDTEND; TZID = Romance Standard Time : 20230313T210000
    //string end = "";
    unsigned int count = 0;
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " :-";
    
    ALLDAY = false;
    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        switch (count) {
        case 1:
            //begin.append(token);
            begin.HOUR = stoi(token);
            break;

        case 2:
            //begin.append(token);
            begin.MINUTES = stoi(token);
            break;

        case 3:
            //end.append(token);
            end.HOUR = stoi(token);
            break;

        case 4:
            //end.append(token);
            end.MINUTES = stoi(token);
            break;

        default:
            return false;
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }
    if (end.HOUR < begin.HOUR) {
        increaseDate();
    }
    return true;
}

bool Event::getNote(char* cstr) {
    NOTE = cstr;
    return true;
}

bool Event::saveEvent(string path) {
    setlocale(LC_ALL, "da-DK");
    wstring output = L"";
    wstring DTSTART;
    wstring DTEND;
    wstring DTSTAMP = convertString(getTimestamp().c_str());
    wstring UID = convertString(getUID().c_str());

    if (ALLDAY == true) {
        DTSTART = L"DTSTART;VALUE=DATE:" + Date(begin) + L"\n";
        DTEND = L"DTEND;VALUE=DATE:" + Date(end) + L"\n";
    }
    else {
        DTSTART = L"DTSTART;TZID=Romance Standard Time:" + Date(begin) + L"T" + Begin() + L"00\n";
        DTEND = L"DTEND;TZID=Romance Standard Time:" + Date(end) + L"T" + End() + L"00\n";
    }

    output += L"BEGIN:VEVENT\n";
    output += L"DESCRIPTION: \\n\n";
    output += L"UID:" + UID + L"\n";
    output += L"SUMMARY;LANGUAGE=da-DK:" + convertString(Tour().c_str()) + L" " + convertString(Note().c_str()) + L"\n";
    output += DTSTART;
    output += DTEND;
    output += L"CLASS:PUBLIC\n";
    output += L"PRIORITY:5\n";
    output += L"DTSTAMP:" + DTSTAMP + L"\n";
    output += L"TRANSP:OPAQUE\n";
    output += L"STATUS:CONFIRMED\n";
    output += L"SEQUENCE:0\n";
    output += L"LOCATION:\n";
    output += L"X-MICROSOFT-CDO-APPT-SEQUENCE:0\n";
    if ((TOUR == "FRI") || (TOUR == "HVILEDAG")) {
        output += L"X-MICROSOFT-CDO-BUSYSTATUS:FREE\n";
        output += L"X-MICROSOFT-CDO-INTENDEDSTATUS:FREE\n";
    }
    else {
        output += L"X-MICROSOFT-CDO-BUSYSTATUS:BUSY\n";
        output += L"X-MICROSOFT-CDO-INTENDEDSTATUS:BUSY\n";
    }

    if (ALLDAY != true) {
        output += L"X-MICROSOFT-CDO-ALLDAYEVENT:FALSE\n";
    }
    else {
        output += L"X-MICROSOFT-CDO-ALLDAYEVENT:TRUE\n";
    }
    output += L"X-MICROSOFT-CDO-IMPORTANCE:1\n";
    output += L"X-MICROSOFT-CDO-INSTTYPE:0\n";
    output += L"X-MICROSOFT-DONOTFORWARDMEETING:FALSE\n";
    output += L"X-MICROSOFT-DISALLOW-COUNTER:FALSE\n";
    output += L"END:VEVENT\n";

    wofstream wofs;
    
    wofs.imbue(locale(wofs.getloc(), new codecvt_utf8<wchar_t>));
    wofs.open(path, ios::app);
    if (wofs.is_open()) {
        wofs << output;
    }
    wofs.close();
    return true;
}

bool CreateCalendar(string dest) {
    wstring calendarHeader;
    calendarHeader += L"BEGIN:VCALENDAR\n";
    calendarHeader += L"METHOD:PUBLISH\n";
    calendarHeader += L"PRODID:Microsoft Exchange Server 2010\n";
    calendarHeader += L"VERSION:2.0\n";
    calendarHeader += L"X-WR-CALNAME:Kalender\n";
    calendarHeader += L"BEGIN:VTIMEZONE\n";
    calendarHeader += L"TZID:Romance Standard Time\n";
    calendarHeader += L"BEGIN:STANDARD\n";
    calendarHeader += L"DTSTART:16010101T030000\n";
    calendarHeader += L"TZOFFSETFROM:+0200\n";
    calendarHeader += L"TZOFFSETTO:+0100\n";
    calendarHeader += L"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=10\n";
    calendarHeader += L"END:STANDARD\n";
    calendarHeader += L"BEGIN:DAYLIGHT\n";
    calendarHeader += L"DTSTART:16010101T020000\n";
    calendarHeader += L"TZOFFSETFROM:+0100\n";
    calendarHeader += L"TZOFFSETTO:+0200\n";
    calendarHeader += L"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=3\n";
    calendarHeader += L"END:DAYLIGHT\n";
    calendarHeader += L"END:VTIMEZONE\n";
    calendarHeader += L"BEGIN:VTIMEZONE\n";
    calendarHeader += L"TZID:America / New_York\n";
    calendarHeader += L"BEGIN:STANDARD\n";
    calendarHeader += L"DTSTART:16010101T000000\n";
    calendarHeader += L"TZOFFSETFROM:+0000\n";
    calendarHeader += L"TZOFFSETTO:+0000\n";
    calendarHeader += L"END:STANDARD\n";
    calendarHeader += L"BEGIN:DAYLIGHT\n";
    calendarHeader += L"DTSTART:16010101T000000\n";
    calendarHeader += L"TZOFFSETFROM:+0000\n";
    calendarHeader += L"TZOFFSETTO:+0000\n";
    calendarHeader += L"END:DAYLIGHT\n";
    calendarHeader += L"END:VTIMEZONE\n";
    
    wofstream wofs;
    wofs.open(dest);
    if (wofs.is_open()) {
        wofs << calendarHeader;
        wofs.close();
        return true;
    }
    else {
        return false;
    }
}

Event CreateEvent(string line){
    const char* str = line.data();
    unsigned int len = strlen(str) + 1;
    unsigned int count = 0;
    char* cstr = new char[len];
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " ";
    string date = "";

    int r = strcpy_s(cstr, len, line.c_str());
    if (r != 0) {
        exit(1);
    }

    Event event;
    // Convert string to c-string
    // Tokenize c-string
    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        //cout << count << ": " << token << endl;
        switch (count) {
        case 1:
            // Extract date
            event.getDate(token);
            //cout << "Date: " << event.Date() << endl;

            break;

        case 2:
            // Extract tour / day off
            event.getTour(token);
            //cout << "Tour: " << event.Tour() << endl;
            break;

        case 3:
            // Extract time

            event.getTime(token);

            break;

        case 4:
            // Extract note #1
            event.getNote(token);
            //cout << "Note: " << event.Note() << endl;
            break;

        default:
            cout << "More than maximum number of tokens encountered!" << endl;
            exit(1);
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }

    return event;
}

int main(int argc, char* argv[]) {
    string source = "";
    string destination = "";
    string filetype = "";
    string path = "";
    ifstream input_file;
    wofstream wofs;

    if (argc < 2) {
        cout << "To few arguments provided, please provide the path of the source file." << endl;
    }

    if (argc == 2) {
        source = argv[1];
        destination = source;
        size_t pos;
        //cout << "Source: " << source << endl;
        //cout << "Destination: " << destination << endl;
        pos = source.find_last_of(".");
        //cout << "Source file type: " << source.substr(pos + 1) << endl;
        string filetype = source.substr(pos);
        if (filetype == ".txt") {
            destination = destination.replace(pos + 1, 3, "ics");
            //cout << "Destination: " << destination << endl;
            // Create calendar
            CreateCalendar(destination);
            input_file.open(source);
            if (input_file.is_open()) {
                //cout << "File: " << source << " is opened." << endl;
                string line;
                while (getline(input_file, line)) {
                    // Create event
                    Event event = CreateEvent(line);
                    // update output file!
                    event.saveEvent(destination);
                }
                input_file.close();
                
                wofs.open(destination, ios::app);
                if (wofs.is_open()) {
                    wofs << L"END:VCALENDAR\n";
                }
            }
            else {
                cout << "Could not open " << source << endl;
                return 1;
            }
        }
        else {
            cout << "Source file has wrong format, file type must be .txt!" << endl;
        }
    }

    if (argc == 3) {
        source = argv[1];
        destination = argv[2];
        size_t pos;

        // Check the source filetype - must be .txt
        pos = source.find_last_of(".");
        filetype = source.substr(pos);
        if (filetype != ".txt") {
            // If wrong source file format exit program.
            cout << "Source file has wrong format, file type must be .txt!" << endl;
            return 0;
        }
        else {
            // Check the destination filetype - must be .ics
            pos = destination.find_last_of(".");
            if (pos != string::npos) {
                filetype = destination.substr(pos);
            }
        }

        if (filetype == ".txt") {
            destination = destination.replace(pos + 1, 3, "ics");
            //cout << "Destination: " << destination << endl;
            // Create calendar
            CreateCalendar(destination);
            input_file.open(source);
            if (input_file.is_open()) {
                //cout << "File: " << source << " is opened." << endl;
                string line;
                while (getline(input_file, line)) {
                    // Create event
                    Event event = CreateEvent(line);
                    // update output file!
                    event.saveEvent(destination);
                }
                input_file.close();

                wofs.open(destination, ios::app);
                if (wofs.is_open()) {
                    wofs << L"END:VCALENDAR\n";
                }
            }
            else {
                cout << "Could not open " << source << endl;
                return 1;
            }
        }
        else {
            cout << "Source file has wrong format, file type must be .txt!" << endl;
        }
    }

    if (argc > 3) {
        cout << "To many arguments given, please try again" << endl;
    }
    return 0;
}
/*
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<cstddef>
#include<ctime>
#include<locale.h>
#include<codecvt>
using namespace std;

wstring convertString(string str) {
    size_t size = str.length() + 1;
    size_t outSize;
    wchar_t* output = new wchar_t[size];
    mbstowcs_s(&outSize, output, size, str.c_str(), size - 1);
    return output;
}

struct Time {
    unsigned int YEAR = 0;
    unsigned int MONTH = 0;
    unsigned int DAY = 0;
    unsigned int HOUR = 9999;
    unsigned int MINUTES = 9999;
};

class Event {
private:
    bool ALLDAY = true;
    Time begin;
    Time end;
    string TOUR = "";
    string NOTE = "";

public:
    Event(void) {};
    bool getDate(char* cstr);
    bool increaseDate(void);
    bool getTour(char* cstr);
    bool getTime(char* cstr);
    bool getNote(char* cstr);
    bool saveEvent(string path);

    string getTimestamp(void);
    string getUID(void);
    wstring Date(Time t);
    wstring Begin(void);
    wstring End(void);
    string Tour(void);
    string Note(void);
};

wstring Event::Date(Time t) {   
    wstring date = L"";
    
    date.insert(0, to_wstring(t.DAY));
    if (t.DAY < 10) {
        date.insert(0, L"0");
    }
    date.insert(0, to_wstring(t.MONTH));
    if (t.MONTH < 10) {
        date.insert(0, L"0");
    }
    date.insert(0, to_wstring(t.YEAR));
    return date;
}

wstring Event::Begin(void) {
    wstring hour = L"";
    wstring minutes = L"";
    if ((begin.HOUR != 9999) && (begin.MINUTES != 9999)) {
        hour = to_wstring(begin.HOUR);
        if (begin.HOUR < 10) {
            hour.insert(0, L"0");
        }
        minutes = to_wstring(begin.MINUTES);
        if (begin.MINUTES < 10) {
            minutes.insert(0, L"0");
        }
        return hour + minutes;
    }
    else {
        return L"";
    }
}

wstring Event::End(void) {
    wstring hour = L"";
    wstring minutes = L"";
    if ((end.HOUR != 9999) && (end.MINUTES != 9999)) {
        hour = to_wstring(end.HOUR);
        if (end.HOUR < 10) {
            hour.insert(0, L"0");
        }
        minutes = to_wstring(end.MINUTES);
        if (end.MINUTES < 10) {
            minutes.insert(0, L"0");
        }
        return hour + minutes;
    }
    else {
        return L"";
    }
}

string Event::Tour(void) {
    return TOUR;
}

string Event::Note(void) { 
    return NOTE;
}

string Event::getTimestamp(void) {
    time_t now = time(NULL);
    struct tm ltm;
    localtime_s(&ltm, &now);
    string year, month, day, hour, minutes, seconds;
    year = to_string(1900 + ltm.tm_year);
    if (ltm.tm_mon < 10) {
        month = "0" + to_string(1 + ltm.tm_mon);
    }
    else {
        month = to_string(1 + ltm.tm_mon);
    }
    if (ltm.tm_mday < 10) {
        day = "0" + to_string(ltm.tm_mday);
    }
    else {
        day = to_string(ltm.tm_mday);
    }
    if (ltm.tm_hour < 10) {
        hour = "0" + to_string(ltm.tm_hour);
    }
    else {
        hour = to_string(ltm.tm_hour);
    }
    if (ltm.tm_min < 10) {
        minutes = "0" + to_string(ltm.tm_min);
    }
    else {
        minutes = to_string(ltm.tm_min);
    }
    if (ltm.tm_sec < 10) {
        seconds = "0" + to_string(ltm.tm_sec);
    }
    else {
        seconds = to_string(ltm.tm_sec);
    }

    return year + month + day + "T" + hour + minutes + seconds + "Z";
}

string Event::getUID(void) {
    string uid = "";
    char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
    for (int i = 0; i < 128; i++) {
        uid += hex_characters[rand() % 16];
    }
    uid.insert(64, "\n ");
    return uid;
}

bool Event::getDate(char* cstr) {
    //string date = "";
    unsigned int count = 0;
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " .:";
    unsigned int year;
    unsigned int month;
    unsigned int day;

    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        switch (count) {
        case 1:
            //date.insert(0, token);
            day = stoi(token);
            begin.DAY = day;
            end.DAY = day;
            break;

        case 2:
            //date.insert(0, token);
            month = stoi(token);
            begin.MONTH = month;
            end.MONTH = month;
            break;

        case 3:
            //date.insert(0, token);
            year = stoi(token);
            begin.YEAR = year;
            end.YEAR = year;
            break;

        default:
            return false;
            //exit(1);
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }
    return true;
}

bool Event::increaseDate(void) {
    //cout << "Entered increaseDate!" << endl;
    end.DAY += 1;
    //cout<< "Day increased"<<endl;
    switch (end.MONTH)
    {
    case 1:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 2:
        if ((end.YEAR % 4) == 0) {
            // leap year
            if (end.DAY > 29) {
                end.MONTH += 1;
                end.DAY = 1;
            }
        }
        else {
            if (end.DAY > 28) {
                end.MONTH += 1;
                end.DAY = 1;
            }
        }
        break;
    case 3:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 4:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 5:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 6:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 7:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 8:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 9:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 10:
        if (end.DAY > 31) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 11:
        if (end.DAY > 30) {
            end.MONTH += 1;
            end.DAY = 1;
        }
        break;
    case 12:
        if (end.DAY > 31) {
            end.YEAR += 1;
            end.MONTH = 1;
            end.DAY = 1;
        }
        break;
    default:
        return false;
    }
    return true;
}

bool Event::getTour(char* cstr) {
    if (strcmp(cstr, "F") == 0) {
        TOUR = "FRI";
        return true;
    }
    else if (strcmp(cstr, "H") == 0) {
        TOUR = "HVILEDAG";
        return true;
    }
    else {
        TOUR = cstr;
        return true;
    }
    return false;
}

bool Event::getTime(char* cstr) {
    //string begin = "";   //DTSTART;TZID=Romance Standard Time:20230313T190000\nDTEND; TZID = Romance Standard Time : 20230313T210000
    //string end = "";
    unsigned int count = 0;
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " :-";
    
    ALLDAY = false;
    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        switch (count) {
        case 1:
            //begin.append(token);
            begin.HOUR = stoi(token);
            break;

        case 2:
            //begin.append(token);
            begin.MINUTES = stoi(token);
            break;

        case 3:
            //end.append(token);
            end.HOUR = stoi(token);
            break;

        case 4:
            //end.append(token);
            end.MINUTES = stoi(token);
            break;

        default:
            return false;
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }
    if (end.HOUR < begin.HOUR) {
        increaseDate();
    }
    return true;
}

bool Event::getNote(char* cstr) {
    NOTE = cstr;
    return true;
}

bool Event::saveEvent(string path) {
    setlocale(LC_ALL, "da-DK");
    wstring output = L"";
    wstring DTSTART;
    wstring DTEND;
    wstring DTSTAMP = convertString(getTimestamp().c_str());
    wstring UID = convertString(getUID().c_str());

    if (ALLDAY == true) {
        DTSTART = L"DTSTART;VALUE=DATE:" + Date(begin) + L"\n";
        DTEND = L"DTEND;VALUE=DATE:" + Date(end) + L"\n";
    }
    else {
        DTSTART = L"DTSTART;TZID=Romance Standard Time:" + Date(begin) + L"T" + Begin() + L"00\n";
        DTEND = L"DTEND;TZID=Romance Standard Time:" + Date(end) + L"T" + End() + L"00\n";
    }

    output += L"BEGIN:VEVENT\n";
    output += L"DESCRIPTION: \\n\n";
    output += L"UID:" + UID + L"\n";
    output += L"SUMMARY;LANGUAGE=da-DK:" + convertString(Tour().c_str()) + L" " + convertString(Note().c_str()) + L"\n";
    output += DTSTART;
    output += DTEND;
    output += L"CLASS:PUBLIC\n";
    output += L"PRIORITY:5\n";
    output += L"DTSTAMP:" + DTSTAMP + L"\n";
    output += L"TRANSP:OPAQUE\n";
    output += L"STATUS:CONFIRMED\n";
    output += L"SEQUENCE:0\n";
    output += L"LOCATION:\n";
    output += L"X-MICROSOFT-CDO-APPT-SEQUENCE:0\n";
    if ((TOUR == "FRI") || (TOUR == "HVILEDAG")) {
        output += L"X-MICROSOFT-CDO-BUSYSTATUS:FREE\n";
        output += L"X-MICROSOFT-CDO-INTENDEDSTATUS:FREE\n";
    }
    else {
        output += L"X-MICROSOFT-CDO-BUSYSTATUS:BUSY\n";
        output += L"X-MICROSOFT-CDO-INTENDEDSTATUS:BUSY\n";
    }

    if (ALLDAY != true) {
        output += L"X-MICROSOFT-CDO-ALLDAYEVENT:FALSE\n";
    }
    else {
        output += L"X-MICROSOFT-CDO-ALLDAYEVENT:TRUE\n";
    }
    output += L"X-MICROSOFT-CDO-IMPORTANCE:1\n";
    output += L"X-MICROSOFT-CDO-INSTTYPE:0\n";
    output += L"X-MICROSOFT-DONOTFORWARDMEETING:FALSE\n";
    output += L"X-MICROSOFT-DISALLOW-COUNTER:FALSE\n";
    output += L"END:VEVENT\n";

    wofstream wofs;
    
    wofs.imbue(locale(wofs.getloc(), new codecvt_utf8<wchar_t>));
    wofs.open(path, ios::app);
    if (wofs.is_open()) {
        wofs << output;
    }
    wofs.close();
    return true;
}

bool CreateCalendar(string dest) {
    wstring calendarHeader;
    calendarHeader += L"BEGIN:VCALENDAR\n";
    calendarHeader += L"METHOD:PUBLISH\n";
    calendarHeader += L"PRODID:Microsoft Exchange Server 2010\n";
    calendarHeader += L"VERSION:2.0\n";
    calendarHeader += L"X-WR-CALNAME:Kalender\n";
    calendarHeader += L"BEGIN:VTIMEZONE\n";
    calendarHeader += L"TZID:Romance Standard Time\n";
    calendarHeader += L"BEGIN:STANDARD\n";
    calendarHeader += L"DTSTART:16010101T030000\n";
    calendarHeader += L"TZOFFSETFROM:+0200\n";
    calendarHeader += L"TZOFFSETTO:+0100\n";
    calendarHeader += L"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=10\n";
    calendarHeader += L"END:STANDARD\n";
    calendarHeader += L"BEGIN:DAYLIGHT\n";
    calendarHeader += L"DTSTART:16010101T020000\n";
    calendarHeader += L"TZOFFSETFROM:+0100\n";
    calendarHeader += L"TZOFFSETTO:+0200\n";
    calendarHeader += L"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=3\n";
    calendarHeader += L"END:DAYLIGHT\n";
    calendarHeader += L"END:VTIMEZONE\n";
    calendarHeader += L"BEGIN:VTIMEZONE\n";
    calendarHeader += L"TZID:America / New_York\n";
    calendarHeader += L"BEGIN:STANDARD\n";
    calendarHeader += L"DTSTART:16010101T000000\n";
    calendarHeader += L"TZOFFSETFROM:+0000\n";
    calendarHeader += L"TZOFFSETTO:+0000\n";
    calendarHeader += L"END:STANDARD\n";
    calendarHeader += L"BEGIN:DAYLIGHT\n";
    calendarHeader += L"DTSTART:16010101T000000\n";
    calendarHeader += L"TZOFFSETFROM:+0000\n";
    calendarHeader += L"TZOFFSETTO:+0000\n";
    calendarHeader += L"END:DAYLIGHT\n";
    calendarHeader += L"END:VTIMEZONE\n";
    
    wofstream wofs;
    wofs.open(dest);
    if (wofs.is_open()) {
        wofs << calendarHeader;
        wofs.close();
        return true;
    }
    else {
        return false;
    }
}

Event CreateEvent(string line){
    const char* str = line.data();
    unsigned int len = strlen(str) + 1;
    unsigned int count = 0;
    char* cstr = new char[len];
    char* token = NULL;
    char* next_token = NULL;
    char delimeters[] = " ";
    string date = "";

    int r = strcpy_s(cstr, len, line.c_str());
    if (r != 0) {
        exit(1);
    }

    Event event;
    // Convert string to c-string
    // Tokenize c-string
    token = strtok_s(cstr, delimeters, &next_token);
    while (token != NULL) {
        count++;
        //cout << count << ": " << token << endl;
        switch (count) {
        case 1:
            // Extract date
            event.getDate(token);
            //cout << "Date: " << event.Date() << endl;

            break;

        case 2:
            // Extract tour / day off
            event.getTour(token);
            //cout << "Tour: " << event.Tour() << endl;
            break;

        case 3:
            // Extract time

            event.getTime(token);

            break;

        case 4:
            // Extract note #1
            event.getNote(token);
            //cout << "Note: " << event.Note() << endl;
            break;

        default:
            cout << "More than maximum number of tokens encountered!" << endl;
            exit(1);
        }
        token = strtok_s(NULL, delimeters, &next_token);
    }

    return event;
}

int main(int argc, char* argv[]) {
    string source = "";
    string destination = "";
    string filetype = "";
    string path = "";
    ifstream input_file;
    wofstream wofs;

    if (argc < 2) {
        cout << "To few arguments provided, please provide the path of the source file." << endl;
    }

    if (argc == 2) {
        source = argv[1];
        destination = source;
        size_t pos;
        //cout << "Source: " << source << endl;
        //cout << "Destination: " << destination << endl;
        pos = source.find_last_of(".");
        //cout << "Source file type: " << source.substr(pos + 1) << endl;
        string filetype = source.substr(pos);
        if (filetype == ".txt") {
            destination = destination.replace(pos + 1, 3, "ics");
            //cout << "Destination: " << destination << endl;
            // Create calendar
            CreateCalendar(destination);
            input_file.open(source);
            if (input_file.is_open()) {
                //cout << "File: " << source << " is opened." << endl;
                string line;
                while (getline(input_file, line)) {
                    // Create event
                    Event event = CreateEvent(line);
                    // update output file!
                    event.saveEvent(destination);
                }
                input_file.close();
                
                wofs.open(destination, ios::app);
                if (wofs.is_open()) {
                    wofs << L"END:VCALENDAR\n";
                }
            }
            else {
                cout << "Could not open " << source << endl;
                return 1;
            }
        }
        else {
            cout << "Source file has wrong format, file type must be .txt!" << endl;
        }
    }

    if (argc == 3) {
        source = argv[1];
        destination = argv[2];
        size_t pos;

        // Check the source filetype - must be .txt
        pos = source.find_last_of(".");
        filetype = source.substr(pos);
        if (filetype != ".txt") {
            // If wrong source file format exit program.
            cout << "Source file has wrong format, file type must be .txt!" << endl;
            return 0;
        }
        else {
            // Check the destination filetype - must be .ics
            pos = destination.find_last_of(".");
            if (pos != string::npos) {
                filetype = destination.substr(pos);
            }
        }

        if (filetype == ".txt") {
            destination = destination.replace(pos + 1, 3, "ics");
            //cout << "Destination: " << destination << endl;
            // Create calendar
            CreateCalendar(destination);
            input_file.open(source);
            if (input_file.is_open()) {
                //cout << "File: " << source << " is opened." << endl;
                string line;
                while (getline(input_file, line)) {
                    // Create event
                    Event event = CreateEvent(line);
                    // update output file!
                    event.saveEvent(destination);
                }
                input_file.close();

                wofs.open(destination, ios::app);
                if (wofs.is_open()) {
                    wofs << L"END:VCALENDAR\n";
                }
            }
            else {
                cout << "Could not open " << source << endl;
                return 1;
            }
        }
        else {
            cout << "Source file has wrong format, file type must be .txt!" << endl;
        }
    }

    if (argc > 3) {
        cout << "To many arguments given, please try again" << endl;
    }
    return 0;
}*/