//
//  PMA Assignment
//  build1
//
//  Created by Chris on 28/12/2014.
//  Copyright (c) 2014 Chris Lynden. All rights reserved.
//
#define NOMINMAX
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <locale>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <limits>
#include <cstddef>
#include <fstream>
#include <windows.h>
#include <stdexcept>

using namespace std;

struct console //http://www.cplusplus.com/forum/windows/10731/ //
  {
  console( unsigned width, unsigned height )
    {
    SMALL_RECT r;
    COORD      c;
    hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (!GetConsoleScreenBufferInfo( hConOut, &csbi ))
      throw runtime_error( "You must be attached to a human." );
    r.Left   =
    r.Top    = 0;
    r.Right  = width -1;
    r.Bottom = height -1;
    SetConsoleWindowInfo( hConOut, TRUE, &r );
    c.X = width;
    c.Y = height;
    SetConsoleScreenBufferSize( hConOut, c );
    }

  ~console()
    {
    SetConsoleTextAttribute(    hConOut,        csbi.wAttributes );
    SetConsoleScreenBufferSize( hConOut,        csbi.dwSize      );
    SetConsoleWindowInfo(       hConOut, TRUE, &csbi.srWindow    );
    }

  HANDLE                     hConOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  };
console con( 100, 100 );

int calcPosition (int bar, int beat, int div)
{
    int total;
    int a = (bar - 1) * 16;
    int b = (beat - 1) * 4;
    total = a + b + div;
    return total - 1;
}

int calcNoteNumber (char note, char accidental, int octave)
{
    int asciiToMidi;
    if (note == 65){asciiToMidi = 9;};
    if (note == 66){asciiToMidi = 11;};
    if (note == 67){asciiToMidi = 0;};
    if (note == 68){asciiToMidi = 2;};
    if (note == 69){asciiToMidi = 4;};
    if (note == 70){asciiToMidi = 5;};
    if (note == 71){asciiToMidi = 7;};
    asciiToMidi += ((octave + 2) * 12);
    if (accidental == 35)
        { asciiToMidi += 1; }
    if (accidental == 98)
        { asciiToMidi -= 1; }
	return asciiToMidi;
}

float calcNoteFrequency (int noteNumber)
{
    float middleA = 440.0;
    float twelfthRoot = pow(2.0, (1.0/12.0));
    float noteFrequency = middleA * (pow(twelfthRoot, (noteNumber - 81)));
	return noteFrequency;
}

float numberLength (float number)
{
    stringstream ss;
    ss << number;
    size_t numDigits = ss.str().length();
	return numDigits;
}
 
struct listEntry
{
    int indexNumber;
    int position;
    int barNumber;
    int beatNumber;
    int divisionNumber;
    float noteNumber;
    string noteName;
    char note;
    char accidental;
    int octave;
    float noteFrequency;
    int duration;
};

void viewAllData (string fileName, int bars, int vectorSize, vector<listEntry> list)
{
    cout << "\n-------------------------------- Name: "
    << fileName << " --- Length: "
    << bars
    << " bars --------------------------------\n" << endl;
    
    if (vectorSize < 1)
        {
        cout << "          No data.\n";
        }
    for (int i = 0; i < vectorSize ; i ++)
        {
        cout << "          " << list[i].indexNumber << ") ";
        cout << list[i].noteName;
        cout << "  Midi: " << list[i].noteNumber;
            
        if (numberLength(list[i].noteNumber) == 2)
            {cout << " " ;}
    
        cout << "   Freq: " << list[i].noteFrequency;
        
        int g = 12.0 - (numberLength(list[i].noteFrequency));
        for (int i = 0; i < g; i++)
            { cout << " ";}
        cout << "Position:  ";
        cout << list[i].barNumber << ".";
            
        int h = 3 -(numberLength(list[i].barNumber));
        for (int i = 0; i < h; i++)
            { cout << " ";}
        cout << list[i].beatNumber << ".  ";
        cout << list[i].divisionNumber << "  (";
        cout << list[i].position;
        cout << ")";
            
        int k = 5 -(numberLength(list[i].position));
        for (int i = 0; i < k; i++)
            { cout << " ";}
        cout << "Length: " << list[i].duration << endl;
        }
}

void viewNote (int indexNumber, int vectorSize, vector<listEntry> list)
{
    indexNumber --;
    
    cout << "          " << list[indexNumber].indexNumber << ") ";
    cout << list[indexNumber].noteName;
    cout << "  Midi: " << list[indexNumber].noteNumber;
        
    if (numberLength(list[indexNumber].noteNumber) == 2)
        {cout << " " ;}
    cout << "   Freq: " << list[indexNumber].noteFrequency;
        
    int g = 12.0 - (numberLength(list[indexNumber].noteFrequency));
    for (int i = 0; i < g; i++)
        { cout << " ";}
    cout << "Position:  ";
    cout << list[indexNumber].barNumber << ".";
        
    int h = 3 -(numberLength(list[indexNumber].barNumber));
    for (int i = 0; i < h; i++)
        { cout << " ";}
    cout << list[indexNumber].beatNumber << ".  ";
    cout << list[indexNumber].divisionNumber << "  (";
    cout << list[indexNumber].position;
    cout << ")";
        
    int k = 5 -(numberLength(list[indexNumber].position));
    for (int i = 0; i < k; i++)
        { cout << " ";}
    cout << "Length: " << list[indexNumber].duration << endl;
}

int sort (vector<listEntry> &list, int &vectorSize)
{
    int count, swap;
    listEntry temp;    
    do
    {
    swap = 0;
    for (count = 0; count < vectorSize - 1; count++)
        {
        if (list[count].position > list[count + 1].position)
            {
            temp = list[count];
            list[count] = list[count + 1];
            list[count + 1] = temp;
            list[count].indexNumber --;
            list[count + 1].indexNumber ++;
            swap = 1;
            }
        }
    }
    while (swap != 0);
	return 0;
}

float tempo2Division (float tempo)
{
    float divTime;
    divTime = (15.0 / tempo);
	return divTime;
}

void save (string fileName, int bars, int vectorSize, vector<listEntry> list)
{
    cout << "\n------------Save as Text File------------" << endl;
    if (vectorSize < 1)
        {
        cout << "\n          No data.\n";
        }
    else
        {
        ofstream file;
        string outputFileName = fileName + ".txt";
        file.open(outputFileName);
        
        file << "\n-------------------------------- Name: "
        << fileName << " --- Length: "
        << bars
        << " bars --------------------------------\n" << endl;
        
        for (int i = 0; i < vectorSize ; i ++)
            {
            file << "          " << list[i].indexNumber << ") ";
            file << list[i].noteName;
            file << "  Midi: " << list[i].noteNumber;
            
            if (numberLength(list[i].noteNumber) == 2)
                {file << " " ;}
            
            file << "   Freq: " << list[i].noteFrequency;
            
            int g = 12.0 - (numberLength(list[i].noteFrequency));
            for (int i = 0; i < g; i++)
                { file << " ";}
            file << "Position:  ";
            file << list[i].barNumber << ".";
            
            int h = 3 -(numberLength(list[i].barNumber));
            for (int i = 0; i < h; i++)
                { file << " ";}
            file << list[i].beatNumber << ".  ";
            file << list[i].divisionNumber << "  (";
            file << list[i].position;
            file << ")";
            
            int k = 5 -(numberLength(list[i].position));
            for (int i = 0; i < k; i++)
                { file << " ";}
            file << "Length: " << list[i].duration << endl;
            }
        cout << "\n      " << fileName << ".txt created!" << endl;
        }
}

bool case2Validate (string tempName)
{
    if ( tempName[0] >= 65 &&
         tempName[0] <= 71 &&
         tempName[1] <= 56 &&
         tempName[1] >= 48   )
        { return true; }
    return false;
}

bool case3Validate (string tempName)
{
    if ( tempName[0] >= 65 &&
         tempName[0] <= 71 &&
         tempName[2] <= 56 &&
         tempName[2] >= 48 &&
        (tempName[1] == 66 ||
		 tempName[1] == 35)  )
        { return true;}
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////main

int main()
{
cout << "---------Lynden's List Editor - Version 1.5---------" << endl;
cout << "   Console MIDI Composer.   Chris Lynden. 2014.\n" << endl;

string fileName;
cout << "      File Name: ";
getline (cin,fileName);
    
int tempBars;
int bars;
do {
    cout << "      Length of file (bars): ";
    if (!(cin >> tempBars) || tempBars > 256 || tempBars < 1)
        {
        cout << "\nPlease enter a number between 1 and 256." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    else
        {
        bars = tempBars;
        break;
        }
    }
    while (1);   
cout << "\n";  
vector <listEntry> list;
int vectorSize = 0;
char quitSure = 'a'; 
while (1)
{  
    cout << "\n------------------------MENU------------------------" << endl;
    cout << "   Name:  " << fileName
         << "   Length:  " << bars << " bars.\n" << endl;
    cout << "      1:  Create New File" << endl;
    cout << "      2:  Add Note" << endl;
    cout << "      3:  Edit Note" << endl;
    cout << "      4:  Delete Note" << endl;
    cout << "      5:  View File" << endl;
    cout << "      6:  Play File" << endl;
    cout << "      7:  Edit Song Length" << endl;
    cout << "      8:  Rename File" << endl;
    cout << "      9:  Save as Text File" << endl;
    cout << "      0:  Quit" << endl;
    cout << "          Please choose an option: " ;   
    char menuChoice;
    cin >> menuChoice;
    switch (menuChoice)
    {
                
        /////////////////////////////////////////////////////////////////////////////////////create new file (1)
                
        case 49:
        {       
        cout << "\n--------------Create New File--------------" << endl;      
        cout << "\n  Current data will be overwritten. Continue? Y/N:" ;      
        char overwriteSure;
        cin >> overwriteSure;
        tolower(overwriteSure);
        if (overwriteSure != 'y')
            {
            cout << "\n  Create New File cancelled" << endl;
            break;
            }    
        cout << "\n      File Name: ";
        cin.ignore();
        getline (cin,fileName);    
        cout << "      Length of file (bars): ";
        cin >> bars;
        cout << "\n";  
        list.clear();
        vector <listEntry> list;
        vectorSize = 0;
        break;
        }
                    
        /////////////////////////////////////////////////////////////////////////////////////add note (2)
                
        case 50:
        {
        cout << "\n--------------Add Note--------------" << endl;
                
        /////////////////////////////////////////////////////////////////////////////////////add note name
                
        cout << "\n      Correct format: \n        (Note name)(Sharp/flat if needed)(Octave between 0 and 8) \n        E.G.  'A2'  'b#0'  'Fb8' " << endl;
        cout << "      Enter 'X' to cancel.\n";
        int cancel = 0;
        int x = 0;
        while (x == 0)
        {
            cout << "      Enter note name: ";
            string tempName;
            cin >> tempName;
            transform(tempName.begin(), tempName.end(), tempName.begin(), ::toupper);
            if (tempName[0] == 'X') {cancel = 1; break;}
            switch ((int)tempName.length())
            {
                case 2:
                {
                if ( case2Validate(tempName) )        
                    {
                    list.push_back(listEntry());
                    vectorSize ++;        
                    tempName.append(" ");
                    list[vectorSize - 1].noteName = tempName;
                    list[vectorSize - 1].note = tempName[0];
                    list[vectorSize - 1].octave = (int)tempName[1] - 48;
                    x = 1;
                    }
                else
                    {
                    cout << "\nInvalid input!\n" << endl;
                    }
                break;
                }
                        
                case 3:
                {
                if (case3Validate(tempName))
                    {
                    list.push_back(listEntry());
                    vectorSize ++;
                    tempName[1] = tolower(tempName[1]);
                    list[vectorSize - 1].noteName = tempName;
                    list[vectorSize - 1].note = tempName[0];
                    list[vectorSize - 1].accidental = tempName[1];
                    list[vectorSize - 1].octave = int(tempName[2]) - 48;
                    x = 1;
                    }
                else
                    {
                    cout << "\nInvalid input!\n" << endl;
                    }
                break;
                }
                        
                default:
                {
                cout << "\nInvalid input!\n" << endl;
                break;
                } 
            }//end switch        
        }//end x while
        if (cancel != 0) {break;}       
          /////////////////////////////////////////////////////////////////////////////////////add note position
        
        
        cout << "\n      Enter note position. \n";
        int tempBar;
        do {
            cout << "      Bar: " ;
            if (!(cin >> tempBar) || tempBar > bars || tempBar < 1)
                {
                cout << "Please enter a number between 1 and " << bars << "." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            else
                {
                list[vectorSize - 1].barNumber = tempBar;
		        break;
                }
            }
            while (1);

         int tempBeat;
         do {
            cout << "      Beat (/4): " ;
            if (!(cin >> tempBeat) || tempBeat > 4 || tempBeat < 1)
                {
                cout << "Please enter a number between 1 and 4." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            else
                {
                list[vectorSize - 1].beatNumber = tempBeat;
                break;
                }
            }
            while (1);
            
        int tempDiv;
        do {
            cout << "      Division: " ;
            if (!(cin >> tempDiv) || tempDiv > 4 || tempDiv < 1)
                {
                cout << "Please enter a number between 1 and 4." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
             else
                {
                list[vectorSize - 1].divisionNumber = tempDiv;
                break;
                }
          }
          while (1);
            
            
             ///////////////////////////////////////////////////////////////////////////////other assignments in vector
        
                list[vectorSize - 1].position =
            calcPosition(list[vectorSize - 1].barNumber, list[vectorSize - 1].beatNumber, list[vectorSize - 1].divisionNumber);
            
                list[vectorSize - 1].noteNumber =
            calcNoteNumber(list[vectorSize - 1].note, list[vectorSize - 1].accidental, list[vectorSize - 1].octave);
            
                list[vectorSize - 1].noteFrequency =
            calcNoteFrequency(list[vectorSize - 1].noteNumber);
            
                list[vectorSize - 1].indexNumber = vectorSize;
            
            ///////////////////////////////////////////////////////////////////////////////add note duration
            
        int tempDuration;
        do {
            cout << "\nInsert note duration. (In divisions, i.e. 16ths, 16 divisions long = 1 bar long.)\nDivisions:" ;
            if (!(cin >> tempDuration) || tempDuration < 1 || tempDuration > ((bars * 16) - list[vectorSize - 1].position))
                {
                cout << "\nPlease enter a number between 1 and " << (bars * 16) - list[vectorSize - 1].position << "." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            else
                {
                list[vectorSize - 1].duration = tempDuration;
                break;
                }    
        }
        while (1);
       
            cout << "\nNote created. \n" << list[vectorSize - 1].noteName 
				 << " at position " << list[vectorSize - 1].barNumber << ". " 
				 << list[vectorSize - 1].beatNumber << ". " << list[vectorSize - 1].divisionNumber 
				 << ".\nLength: " << list[vectorSize - 1].duration << " divisions.\n" << endl;
                sort(list, vectorSize);
        break;
		}        

        /////////////////////////////////////////////////////////////////////////////////////edit note (3)
        
        case 51:
        {
            if (vectorSize == 0) {
            cout << "\n------------Edit Note------------\n" << endl;
            cout << "      No data. " << endl; break;}
            char quitEdit ='r';
            while (quitEdit != 'Q')
                {
                cout << "\n------------Edit Menu------------\n" << endl;
                cout << "      1:  Edit Note Pitch" << endl;
                cout << "      2:  Edit Note Length" << endl;
                cout << "      3:  Edit Note Position" << endl;
                cout << "      4:  View File" << endl;
                cout << "      0:  Back to Main Menu" << endl;
                cout << "          Please choose an option: " ;
                char editMenuChoice;
                cin >> editMenuChoice;
                switch (editMenuChoice)
                    {
                        case 49: //edit pitch
                        {
                        cout << "\nEdit pitch of which note?" << endl;
                        cout << "\n      Index Number:";
                        int g = 0;
                        while (g==0)
                            {
                            int whichNote;
                            cin>>whichNote;
                            if (whichNote > vectorSize || whichNote < 1)
                                {
                                cout << "Please enter an index number between 1 and " << vectorSize << "." << endl;
                                cout << "\n      Index Number:";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                            else
                                {
                                cout <<"\n";
                                viewNote(whichNote, vectorSize, list);
                                int h = 0;
                                while (h == 0)
                                    {
                                    cout << "\n      Enter new note name:";
                                    string tempName;
                                    cin >> tempName;
                                    transform(tempName.begin(), tempName.end(), tempName.begin(), ::toupper);
                                    switch ((int)tempName.length())
                                        {
                                            case 2:
                                            {
                                            if ( case2Validate(tempName) )
                                                {
                                                tempName.append(" ");
                                                list[whichNote - 1].noteName = tempName;
                                                list[whichNote - 1].note = tempName[0];
                                                list[whichNote - 1].octave = (int)tempName[1] - 48;
                                                h = 1;
                                                list[whichNote - 1].noteNumber =
                                                calcNoteNumber(list[whichNote - 1].note, list[whichNote - 1].accidental, list[whichNote - 1].octave);
                                                list[whichNote - 1].noteFrequency =
                                                calcNoteFrequency(list[whichNote - 1].noteNumber);
                                                break;
                                                }
                                            else
                                                {
                                                cout << "\nInvalid input!" << endl;
                                                break;
                                                }
                                            }
                                            
                                            case 3:
                                            {
                                            if ( case3Validate(tempName))
                                                {
                                                tempName[1] = tolower(tempName[1]);
                                                list[whichNote - 1].noteName = tempName;
                                                list[whichNote - 1].note = tempName[0];
                                                list[whichNote - 1].accidental = tempName[1];
                                                list[whichNote - 1].octave = int(tempName[2]) - 48;
                                                h = 1;
                                                list[whichNote - 1].noteNumber =
                                                calcNoteNumber(list[whichNote - 1].note, list[whichNote - 1].accidental, list[whichNote - 1].octave);
                                                list[whichNote - 1].noteFrequency =
                                                calcNoteFrequency(list[whichNote - 1].noteNumber);
                                                break;
                                                }
                                            else
                                                {
                                                cout << "\nInvalid input!" << endl;
                                                break;
                                                }
                                            }
                                            
                                            default:
                                            {
                                            cout << "\nInvalid input!\n" << endl;
                                            break;
                                            }   
                                        }//end switch
                                    }//end h while
                                g = 1;    
                                }//end else
                            }//end while
                        break;
                        }//end case
                        
                        case 50: //edit note length
                        {
                        cout << "\nEdit length of which note?" << endl;
                        cout << "\n      Index Number:";
                        int f = 0;
                        while (f == 0)
                            {
                            int whichNote = 0;
                            cin>>whichNote;
                            if (whichNote > vectorSize || whichNote < 1) 
                                {
                                cout << "Please enter an index number between 1 and " << vectorSize << "." << endl;
                                cout << "\n      Index Number:";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                            else
                                {
                                cout <<"\n";
                                viewNote(whichNote, vectorSize, list);
                                cout << "\n      Enter new note length. \n\n";   
                                int tempDuration;
                                do {
                                    cout << "      Length (1/16ths): " ;
                                    if (!(cin >> tempDuration) || tempDuration < 1 || tempDuration > ((bars * 16) - list[whichNote - 1].position))
                                        {
                                        cout << "Please enter a number between 1 and " << ((bars * 16) - list[whichNote - 1].position) << "." << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                    else
                                        {
                                        list[whichNote - 1].duration = tempDuration;
                                        break;
                                        }
                                    }
                                    while (1);
                                break;
                                }
                            }
                            break;
                        }
                        
                        case 51: //edit note position
                        {
                        cout << "\nEdit position of which note?" << endl;
                        cout << "\n      Index Number:";
                        int e = 0;
                        while (e == 0)
                            {
                            int whichNote = 0;
                            cin>>whichNote;
                            if (whichNote > vectorSize || whichNote < 1)
                                {
                                cout << "Please enter an index number between 1 and " << vectorSize << "." << endl;
                                cout << "\n      Index Number:";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                            else
                                {
                                cout <<"\n";
                                viewNote(whichNote, vectorSize, list);
                                cout << "\n      Enter new note position. \n\n";
                                int tempBar;
                                do {
                                    cout << "      Bar: " ;
                                    if (!(cin >> tempBar) || tempBar > bars || tempBar < 1)
                                        {
                                        cout << "Please enter a number between 1 and " << bars << "." << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                    else
                                        {
                                        list[whichNote - 1].barNumber = tempBar;
                                        break;
                                        }
                                    }
                                while (1);
                                
                                int tempBeat;
                                do {
                                    cout << "      Beat (/4): " ;
                                    if (!(cin >> tempBeat) || tempBeat > 4 || tempBeat < 1)
                                        {
                                        cout << "Please enter a number between 1 and 4." << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                    else
                                        {
                                        list[whichNote - 1].beatNumber = tempBeat;
                                        break;
                                        }
                                    }
                                    while (1);
                                
                                int tempDiv;
                                do {
                                    cout << "      Division: " ;
                                    if (!(cin >> tempDiv) || tempDiv > 4 || tempDiv < 1)
                                        {
                                        cout << "Please enter a number between 1 and 4." << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                    else
                                        {
                                        list[whichNote - 1].divisionNumber = tempDiv;
                                        break;
                                        }
                                    }
                                    while (1);
                                
                                    e = 1;
                                
                                list[whichNote - 1].position =
                                calcPosition(list[whichNote - 1].barNumber, list[whichNote - 1].beatNumber, list[whichNote - 1].divisionNumber);
                                if ((list[whichNote - 1].position + list[whichNote - 1].duration) > (bars * 16))
                                    { list[whichNote - 1].duration = ((bars * 16) - list[whichNote - 1].position);} 
                                }//end else
                            }//end while
                        break;
                        }//end case 51
                        
                        case 52: //view file
                        {
                        viewAllData(fileName, bars, vectorSize, list);
                        break;
                        }
                        
                        case 48: //back to main menu
                        {
                        quitEdit = 'Q';
                        break;
                        }
                        
                        default:
                        {
                        cout << "\n" << editMenuChoice << " is not a valid edit menu option.\n" << endl;
                        break;
                        }
                    }//end switch
                
                }//end while
                sort(list, vectorSize);
                break;
            }
        
        /////////////////////////////////////////////////////////////////////////////////////erase note (4)
        
        case 52:
        {
        cout <<"\n------------Delete Note------------\n";
        if (vectorSize == 0)
        {cout << "\n      No Data." << endl; break;}
        else
			{
            cout << "\n      Erase which note:";
            int whichNote;
            cin >> whichNote;
            if (whichNote > vectorSize || whichNote < 1)
                {
                cout << "\n      No note with index number " << whichNote << "\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
                }
            else
                {
                cout << "\n      Erase note " << list[whichNote - 1].indexNumber << "? (Y/N):";
                char eraseSure = 'a';
                cin >> eraseSure;
                if (toupper(eraseSure) == 'Y')
                    {
                    list.erase(list.begin()+ (whichNote - 1));
                    vectorSize --;
                    for (int i = whichNote - 1; i < vectorSize; i ++)
                        {
                        list[i].indexNumber --;
                        }
                    }
                break;
                }
			 break;
			 }
        }
        
        /////////////////////////////////////////////////////////////////////////////////////iew file so far (5)
        
        case 53:
        {
                sort(list, vectorSize);
                viewAllData(fileName, bars, vectorSize, list);
                break;
        }
        
        /////////////////////////////////////////////////////////////////////////////////////Play File (6)
            
        case 54:
        {
            cout << "\n------------Play File------------\n";
            if (vectorSize == 0)
                {cout << "\n      No Data." << endl; break;}
            sort(list, vectorSize);
            viewAllData(fileName, bars, vectorSize, list);   
            while (1)
            {
            cout << "\n      Tempo: " ;
            int tempo;
            if (!(cin >> tempo) || tempo > 200 || tempo < 75)
                {
                cout << "Please enter a number between 75 and 200.";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            else
                {
				cout << "\n";
                float divTime = (tempo2Division(tempo));
                int p6 = 1000000;
                int p3 = 1000;
                if (list[0].position != 0)
                    {
                float firstRest = ((list[0].position) * divTime);
                cout << "\n          Rest for " << (list[0].position) << " divisions." << endl; //mac
                Sleep(firstRest * p3);
                    }
                for (int i = 0; i < vectorSize; i ++)
                    {
                    float iFreq = list[i].noteFrequency + 0.5;
                    float iLength = ((list[i].duration * divTime) * p3);
                    if (i > 0 && list[i - 1].position == list[i].position)
                        {viewNote(i + 1, vectorSize, list);
                            cout << "          (Skipped: cannot playback simultaneous notes)\n" ;
						if (i < vectorSize - 1)
                            {
							if ((list[i + 1].position - list[i].position - list[i].duration) > 0)
								{
								 cout << "          Rest for " << (list[i + 1].position - list[i].position - list[i].duration) << " divisions." << endl; //mac
								 float iRest = (list[i + 1].position - list[i].position - list[i].duration) * divTime;
								 Sleep(iRest * p3);
								}
                            }
                        else
                            {break;}
						}
                    else
                        {
						viewNote(i + 1, vectorSize, list);//mac
                        Beep (int(iFreq), iLength);
                        if (i < vectorSize - 1)
                            {
							if ((list[i + 1].position - list[i].position - list[i].duration) > 0)
								{
								 cout << "          Rest for " << (list[i + 1].position - list[i].position - list[i].duration) << " divisions." << endl; //mac
								 float iRest = (list[i + 1].position - list[i].position - list[i].duration) * divTime;
								 Sleep(iRest * p3);
								}
                            }
                        else
                            {break;}
                        }
                     }
                  break;
                }
            break;
            }
        break;
            }
        
        /////////////////////////////////////////////////////////////////////////////////////edit length/rename (7/8)
                
        case 55:
        {
                cout << "\n--------Edit Song Length--------" << endl;
                int tempBars;
                do {
                    cout << "      Length of file (bars): ";
                    if (!(cin >> tempBars) || tempBars > 256 || tempBars < 1)
                        {
                        cout << "\nPlease enter a number between 1 and 256." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    if (vectorSize != 0 && tempBars < bars &&  list[vectorSize - 1].barNumber > tempBars)
                        {
                            cout << "Shortening song will delete notes. Continue (Y/N): ";
                            char shortenSure;
                            cin >> shortenSure;
                            if (toupper(shortenSure) == 89)
                                {
                                bars = tempBars;
                                int tempVectorSize = vectorSize;
                                for (int i = 0; i < vectorSize; i ++)
                                    {
                                    if (list[i].barNumber > bars)
                                        {
                                        list.pop_back();
                                        tempVectorSize --;
                                        }
                                    }
                                vectorSize = tempVectorSize;
                                break;
                                }   
                            else
                                {break;}
                        }
                    else
                        {
                            bars = tempBars;
                                break;
                        }        
                }
                while (1);
                break;
            }
                
        case 56:
        {
                {
                    cout << "\n--------Rename File--------" << endl;
                    cout << "File name: ";
                    cin.ignore();
                    getline (cin,fileName);
                    break;
                }
        }
        
        /////////////////////////////////////////////////////////////////////////////////////save as .txt (9)
        
        case 57:
        {
        save (fileName, bars, vectorSize, list);
        break;
        }
       
       
        
        /////////////////////////////////////////////////////////////////////////////////////quit/error (0)
                
        case 48:
        {
            cout << "\n      Are you sure? (Y/N): ";
            cin >> quitSure;
            break;
        }
        
        default:
        {
                cout << "\n" << menuChoice << " is not a valid menu option.\n" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
                
    }///end menu switch
        
    if (tolower(quitSure) == 121)
        { break;}

}// end of main while loop
   
cout << "\n      Thankyou for using Lynden's List Editior. Goodbye." << endl;
    
return 0;
    
}

