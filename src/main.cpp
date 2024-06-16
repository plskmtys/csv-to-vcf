#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <boost/algorithm/string.hpp>

#define INPUT_FILE "in.csv"
#define OUTPUT_FILE "out_win.vcf"
#define windows
#define outlook


#ifdef windows
    #define endl "\r\n"
#else
    #define endl "\n"
#endif

class Contact{
private:
public:
    std::string Name;
    std::string Address;

    bool operator<(const Contact& other) const{
        return Address < other.Address;
    }

    void Print(std::ostream& os) const {
        os << "BEGIN:VCARD" << endl << "VERSION:2.1" << endl;
        os << "N:" << Name << ";;;;" << endl;
        os << "FN:" << Name << endl;
        os << "EMAIL;work:" << Address << endl;
        os << "END:VCARD" << endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Contact& c){
        c.Print(os);
        return os;
    }
};

std::vector<std::string>& StrSplit(std::vector<std::string>& vec, std::string& s, const std::string& delim){
    std::size_t pos = 0;
    const std::size_t delim_len = delim.length();
    while((pos = s.find(delim)) != std::string::npos){
        vec.push_back(s.substr(0, pos));
        s.erase(0, pos + delim_len);
    }
    vec.push_back(s);
    return vec;
}

std::string cleanup(std::string& s){
    s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\''), s.end());
    return boost::trim_copy(s);
}

std::vector<Contact> parseLine(std::string& line){
    std::vector<Contact> contacts;
    std::vector<std::string> parts;
    StrSplit(parts, line, "\",\"");
    std::vector<std::string> names;
    std::vector<std::string> addresses;
    boost::split(names, parts[0], boost::is_any_of(";"));
    boost::split(addresses, parts[1], boost::is_any_of(";"));
    for(std::size_t i = 0; i < names.size(); ++i){
        Contact c;
        c.Name = names[i];
        c.Name = cleanup(c.Name);
        c.Address = addresses[i];
        c.Address = cleanup(c.Address);
        contacts.push_back(c);
    }
    return contacts;
}

int main(){
    std::string inputFileName = "files/"; inputFileName += INPUT_FILE;
    std::ifstream inputFile(inputFileName);

    #ifndef outlook
        std::string outputFileName = "files/"; outputFileName += OUTPUT_FILE;
        std::ofstream outputFile(outputFileName);
    #endif

    std::string line;
    std::set<Contact> contacts;
    while(std::getline(inputFile, line)){
        std::vector<Contact> parsedContacts = parseLine(line);
        for(const auto& c : parsedContacts){
            contacts.insert(c);
        }
    }

    for(const auto& c : contacts){
        #ifdef outlook
            std::ofstream outputFile("outlook/" + c.Name + ".vcf");
            outputFile << c;
            outputFile.close();
        #else
            outputFile << c;
        #endif
    }
}