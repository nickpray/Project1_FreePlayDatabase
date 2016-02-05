#include <iostream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
using namespace std;
//prototypes to all my functions
void addRecord(string,fstream&,fstream&);
void searchRecord(string,fstream&,fstream&);
void editRecord(string,fstream&,fstream&);
void deleteRecord(string,fstream&,fstream&);
void outputDat(string,fstream&,int);
void replaceHighscore(string,fstream&,string);
void replaceInitials(string,fstream&,string);
void replacePlays(string,fstream&,string);
void print(fstream&,int,string,int,char);
bool match(string,string);
void decision(fstream&,fstream&,fstream&);

void addRecord(string stats,fstream& log,fstream& dat)//adds a record to the dat file
{
    log.clear();
    log.seekp(0,ios::end);
    dat.clear();
    dat.seekp(0,ios::end);
    log<<"\nRECORD ADDED\n";
    print(log,1,stats,0,' ');
    outputDat(stats,dat,1);
}
void outputDat(string stats,fstream& dat,int stop)//recursively outputs the new stats to the dat file in the correct format
{
    if(stop<=5)
    {
    switch(stop)
    {
        case 1:dat<<stats.substr(0,stats.find(' '))<<", ";break;
        case 2:dat<<setfill('0')<<setw(9)<<stats.substr(0,stats.find(' '));dat<<", ";break;
        case 3:dat<<stats.substr(0,stats.find(' '))<<", ";break;
        case 4:dat<<setfill('0')<<setw(4)<<(stats.substr(0,stats.find(' ')));dat<<", ";break;
        case 5:dat<<"$";dat<<setfill('0')<<setw(7)<<fixed<<showpoint<<setprecision(2)<<(stats.substr(1,stats.find(' ')));dat<<endl;break;
    }
    outputDat(stats.substr(stats.find(' ')+1),dat,stop+1);
    }
}
void searchRecord(string query,fstream& dat,fstream& log)
{
    dat.clear();
    dat.seekg(0,ios::beg);
    string line="";
    bool found=false;
    while(getline(dat,line))
    {
        if(match(query,line.substr(0,line.find(',')))){
        found=true;
        log<<"\nFOUND ";
        print(log,1,line,0,',');
        }
    }
    if(found==false)
        log<<"\nNOT FOUND"<<query;

}
void editRecord(string query,fstream & log,fstream &dat)
{
    string line,prin;
    dat.clear();
    dat.seekg(0,ios::beg);
    bool found=false;
    while(getline(dat,line))
    {
        if(match(line.substr(0,line.find(',')),query.substr(0,line.find('"')))){
        log<<"\nUPDATED ";
        switch(query[query.find('"')+2])
        {
            case '1':replaceHighscore(query.substr(query.find('1')+2),dat,line);break;
            case '2':replaceInitials(query.substr(query.find('2')+2),dat,line);break;
            case '3':replacePlays(query.substr(query.find('3')+2),dat,line);break;
        }
        found=true;
        }
        prin=line;
    }
    if(found==false)
        log<<"\nNOT FOUND"<<query;
    print(log,1,prin,0,',');
}
void deleteRecord(string del,fstream& log,fstream&dat)
{
    fstream tempfile("temp.dat",ios::in);
    dat.clear();
    dat.seekg(0,ios::beg);
    string line="";
    bool found=false;
    while(getline(dat,line))
    {
        cout<<line;
        if(match(del,line.substr(0,line.find(',')))){
        found=true;
        log<<"\nRECORD DELETED\n";
        print(log,1,line,0,',');
        }
        else
            tempfile<<line;
    }
    if(found==false)
        log<<"\nNOT FOUND"<<del;
    remove("freeplaydat.txt");
    rename("temp.dat","freeplaydat.txt");

}
void replaceHighscore(string newScore,fstream&dat,string datline)
{
    int reset=dat.tellp();
    dat.clear();
    dat.seekp(-31,ios::cur);
    dat<<setfill('0')<<setw(9)<<newScore;
    dat.clear();
    dat.seekp(reset);
}
void replaceInitials(string newInitials,fstream&dat,string datline)
{
    int reset=dat.tellp();
    dat.clear();
    dat.seekp(-20,ios::cur);
    dat<<newInitials;
    dat.clear();
    dat.seekp(reset);
}
void replacePlays(string newPlays,fstream &dat,string datline)
{
    int reset=dat.tellp();
    int playsInt=atoi(newPlays.c_str());
    dat.clear();
    dat.seekp(-15,ios::cur);
    dat<<setfill('0')<<setw(4)<<newPlays;
    dat.clear();
    dat.seekp(2,ios::cur);
    dat<<"$";
    dat<<setfill('0')<<setw(7)<<fixed<<showpoint<<setprecision(2)<<(playsInt*.25);
    dat.clear();
    dat.seekp(reset);
}

void print(fstream& log,int c,string line,int start,char ignore)
{
    if(c<=5)
    {
    switch(c)
    {
        case 1:log<<"Name: ";break;
        case 2:log<<"\nHighscore: ";break;
        case 3:log<<"\nInitials: ";break;
        case 4:log<<"\nPlays: ";break;
        case 5:log<<"\nRevenue: ";break;
    }

    log<<line.substr(start,line.find(ignore));
        print(log,c+1,line.substr(line.find(ignore)+1),0,ignore);
    }
}
bool match(string query, string name)
{
    if(query=="")
        return false;
    if(name.find(query)==std::string::npos)
            return false;
    else
        return true;
}
/*bool match(string query, string name)
{
    //cout<<query<<"|"<<name<<endl;
    int matches=0;
    for(int i=0;i<query.length();i++){
    if(query[i]==name[i])
        matches++;
    }
    if(matches==query.length()){
        cout<<"TRUE"<<    endl;
        return true;
        }
    else if(name.length()<query.length()){
            cout<<"false"<<endl;
            return false;
    }
    else{cout<<"CALLAGAIN"<<endl;
        return match(query,name.substr(matches+1));
    }
}*/
void decision(fstream& log,fstream& dat,fstream& in)
{
    string instruct;
    while(getline(in,instruct))
    {
        switch(instruct[0])
        {
            case '1':addRecord(instruct.substr(3),log,dat);break;
            //case '2':searchRecord(instruct.substr(2),dat,log);break;
            //case '3':editRecord(instruct.substr(3),log,dat);break;
            case '4':deleteRecord(instruct.substr(2),log,dat);break;
        }
    }
}
int main()
{
    char cwd[1024];
    cout<<getcwd(cwd, sizeof(cwd));
    fstream log;
    fstream dat;
    fstream in;
    log.open("freeplaylog.txt",ios::in|ios::out|ios::trunc);
    dat.open("freeplay.dat",ios::in|ios::out|ios::binary);
    in.open("update.txt",ios::in|ios::out);
    if(!log.fail()&&!dat.fail()&&!in.fail())
    {
    decision(log,dat,in);
    }
    else
    {
        cout<<"FILES WERE NOT ABLE TO BE READ";
    }
    dat.close();
    in.close();
    log.close();
}

