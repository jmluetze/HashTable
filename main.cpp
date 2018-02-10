/*
 * Hashtable implementation written by Jonathan Luetze
 *
 * Main purpose of this program is to View, Enter, or Delete Tweets in Hashtable
 *
 * The user menu is displayed with 4 Options:
 * 1. Enter a new Tweet - enters tweet with current date to user specified
 * 2. Delete all tweets for a user
 * 3. View tweets for a user - displays tweets sorted by date and time
 * 4. Quit
 *
 *  Structure Tweet contains time, text and name for a tweet
 *
 * Class Hashtable conatains all hashtable methods
 * Hash Methods : put - enters a tweet into the hashtable
 *              : sortTweets - sorts the tweets of a user by time
 *              : formatTime - formats the time to ensure accurate sorting
 *              : printTime - formats time string that will be printed
 *              :
 *              : remove - removes the user with all their tweets from table
 *              : contains - checks if user is in table
 *              : get - prints all tweets for given user name
 *              :
 * Other Methods: main - starter function
 *              : readData - reads data from file
 *              : promptUser - first read in of data and prompting user
 *              : prompt - requests user input based on menu listed above
 *              :
 *              : enterTweet - receives and sends user input for new tweet
 *              : deleteTweets - receives and sends specified username to delete their tweets
 *              : viewTweets - receives and sends specified username to view their tweets
 *              :
 *              : hashingReturn - calculates and returns hashvalue
 *              : calculateTime - calculates and returns current date and time
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <cctype>
#include "stdlib.h"
using namespace std;

struct Tweet
{
    string time;
    string text;
    string name;
};

void readData       (int argc, const char * argv[]);
void promptUser     (int argc, const char **argv);
void prompt         (int argc, const char **argv, int selection);
void enterTweet     ();
void deleteTweets   ();
void viewTweets     ();
void hashingRemove  (string s);
int hashingReturn   (string s);
string formatTime   (string time);
string printTime    (string time);
string calculateTime();
void sortTweets     (int hashvalue, string time, string text, string name);

class HashTable
{
    Tweet arr[7000];
    
public:
    
    void put (int hashvalue, string time, string text,string name)
    {
        time = formatTime(time);

        if (arr[hashvalue].name == "")
        {
            arr[hashvalue].time = time;
            arr[hashvalue].text = text;
            arr[hashvalue].name = name;
        }
        else {sortTweets(hashvalue, time, text, name);}
    }
    
    void sortTweets (int hashvalue, string time, string text, string name)
    {
        int i = hashvalue;
     
        while (i < 7000)
        {
            if(arr[i].time < time)      //sorting by time with the most recent tweet first
            {
                string tempTime = arr[i].time;  //saving the current values that need to be swapped
                string tempText = arr[i].text;
                string tempName = arr[i].name;
                
                arr[i].time = time;     //placing the new values where the old ones were
                arr[i].text = text;
                arr[i].name = name;
                
                time = tempTime;        //reassign the original variables
                text = tempText;
                name = tempName;
            }
            
            if (arr[i+1].name == "")    //insert the tweet into the array location if it is empty
            {
                arr[i+1].time = time;
                arr[i+1].text = text;
                arr[i+1].name = name;
                break;
            }
            i++; if (i == 7000) {i = 0;}
        }
    }
    
    string formatTime (string time)
    {
        string timestrip = "";
        for (int i = 0; i < time.length(); i++)
        {
            if (isdigit(time[i]) == true) {timestrip += time[i];}
        }
        return timestrip;
    }
    
    string printTime (string time)
    {
        string finalTime = time.substr(0,4) + '-' + time.substr(4,2) + '-' + time.substr(6,2) + " @ " + time.substr(8,2)
            + ':' +  time.substr(10,2) + ':' + time.substr(12,2);
        return finalTime;
    }
    
    void remove (string screenname)
    {
        int temp = hashingReturn(screenname);
        
        if (contains(screenname) == false) {cout << "Username is not found\n";}
        
        while (arr[temp].name != "")
        {
            if(arr[temp].name == screenname)
            {
                arr[temp].time = "";
                arr[temp].text = "";
                arr[temp].name = "";
            }
            temp++; if (temp == 7000) {temp = 0;}
        }
    }
    
    bool contains(string screenname)
    {
        for(int i = 0; i < 7000; i++)
        {
            if ((arr[i].name) == screenname) {return true;}
        }   return false;
    }
    
    void get(int temp, string screennameview)
    {
        if (contains(screennameview) == false) {cout << "Username is not found\n";}
   
        int h = temp;
        while (h < 7000)
        {
            if (hashingReturn(arr[h].name) == temp)
            {
                if (arr[h].name == screennameview)
                {
                    cout << "\nTime: " + printTime(arr[h].time) << endl;
                    cout << "Text: " + arr[h].text << endl;
                    cout << "Name: " + arr[h].name << endl << endl;
                }
            }h++; if (h == 7000) {h = 0;} if (h == temp-1) {break;}
        }
    }
};

HashTable tweet1;

int main(int argc, const char * argv[])
{
    promptUser(argc,argv);
    return 0;
}

void readData(int argc, const char * argv[]) //reading in and modifying lines
{
    char c; int start,category; string line, sendstring,names, time,text,name;
    ifstream infile; infile.open(argv[1]);

    while ((c = infile.get()) != EOF)
    {
        if (c == '{')
        {
            while (c != '}' && (c = infile.get()) != EOF)
            {
                getline(infile, line); sendstring = "";
                
                     if (line[0] == 'c') {category = 1;}
                else if (line[0] == 't') {category = 2;}
                else if (line[0] == 's') {category = 3;}
                
                for (int i = 0; i < line.length(); i++)
                    {if (line[i] == ':' && line[i+1] == '"') {start = i;break;}}
                
                for (int i = start; i < line.length(); i++)
                {
                    if (line[i+2] == '"' && (line[i+3] == ',' ||line[i+3] == '}')) {break;}
                    else {sendstring += line[i+2];}
                }
                if (category == 1) {time = sendstring;}
                if (category == 2) {text = sendstring;}
                if (category == 3) {name = sendstring; tweet1.put(hashingReturn(name), time,text,name);}
            }
        }
    }
}

void promptUser (int argc, const char **argv) //initial read of data and prompting of user
{
    string screenname, tweettext, name;
    
    readData(argc, argv);
    prompt(argc, argv,0);
}

void prompt (int argc, const char **argv, int selection) //menu
{
    if ((selection != 1||selection != 2)|(selection != 3||selection != 4))
    {
        cout << "Please choose one of the options below: \n1. Enter new Tweet\n2. Delete all tweets for a given screen_name\n3. View Tweets\n4. Quit    ";
        cin >> selection; cout << endl;
    }
         if (selection == 1) {enterTweet();     prompt(argc, argv, 0);}
    else if (selection == 2) {deleteTweets();   prompt(argc, argv, 0);}
    else if (selection == 3) {viewTweets();     prompt(argc, argv, 0);}
}

void enterTweet()
{
    string tweettext, name, times;
    
    cout << "Please enter the tweet:"; cin.ignore (); getline(cin,tweettext);
    cout << endl << "Please enter the screenname: "; getline(cin, name);
    cout << endl;
    times = calculateTime();
    
    tweet1.put(hashingReturn(name), times, tweettext,name);
}

void deleteTweets()
{
    string screenname;
    cout << "Which screenname's tweets would you like to remove?  "; cin >> screenname;
    tweet1.remove(screenname);
}

void viewTweets()
{
    string screennameview;
    cout << "Which screenname's tweets would you like to view?  "; cin >> screennameview;
    tweet1.get(hashingReturn(screennameview), screennameview);
}

int hashingReturn(string s)
{
    int hash = 0;
    for (int i = 0; i < s.length(); i++) {hash += s[i];}
    if (hash < 0) {hash *= -1;}
    return hash % 7000;
}

string calculateTime()
{
    string times = "";
    
    time_t timeNow; time(&timeNow); times = ctime(&timeNow);
    
    struct tm* timeinfo;
    timeinfo = localtime( &timeNow );
    
    ostringstream convert;
    int thisTime = timeinfo->tm_year + 1900;
    convert << thisTime;
    times = convert.str(); //int to string
    
    ostringstream convertMonth;
    thisTime = timeinfo->tm_mon;
    convertMonth << thisTime+1;
    
    if(thisTime < 10) {times += "0" + convertMonth.str();}
    else {times += convertMonth.str();}
    
    ostringstream convertDay;
    thisTime = timeinfo->tm_mday;
    convertDay << thisTime;
    if(thisTime < 10) {times += "0" + convertDay.str();}
    else {times += convertDay.str();}
    
    ostringstream convertHour;
    thisTime = timeinfo->tm_hour;
    convertHour << thisTime;
    if(thisTime < 10) {times += "0" + convertHour.str();}
    else {times += convertHour.str();}
    
    ostringstream convertMin;
    thisTime = timeinfo->tm_min;
    convertMin << thisTime;
    if(thisTime < 10) {times += "0" + convertMin.str();}
    else {times += convertMin.str();}
    
    ostringstream convertSec;
    thisTime = timeinfo->tm_sec;
    convertSec << thisTime;
    if(thisTime < 10) {times += "0" + convertSec.str();}
    else {times += convertSec.str();}
    
    return times;
}
