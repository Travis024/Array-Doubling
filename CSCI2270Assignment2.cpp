#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//A structure to store each word and the number of times it is counted in the file
struct wordItem{
    string word = "";
    int wordCount = 0;
};

/*
A function to read the stop word list and store the words into an array.
Pre-conditions: ignoreWordsFileName is passed in the command line, ignoreWords is an array of strings
Post-Conditions: nothing is returned, but an array of stop words is created
*/
void getStopWords(string ignoreWordFileName, string ignoreWords[]);

/*
A function to check if a word in the text file is a stop word
Pre-conditions: word is a string, ignoredWords has been previously filled
Post-conditions: returns true if is a stop word, false if not
*/
bool isStopWord(string word, string ignoreWords[]);

/*
A function to check whether this is the first occurrence of the word in the text file.
Pre-conditions: word is a string, wordItems is an array of structures wordItem, lengthOfWordsArray is an int and the number of words stored.
Post-conditions: returns -1 if the word isn't found in the array, the index of the word if it is already there.
*/
int isFirstOccurrence(string word, wordItem wordItemList[], int lengthOfWordsArray);

/*
A function to double the size of an array when it becomes full
Pre-conditions: maxLength is the the maximum length of the array being passed, lengthOfWordsArray is the number of words in the array, wordItemList is an array of structure wordItem
Post-conditions: wordItemList is doubled in length and has the values it had before being passed in
*/
void doubleArray(int &maxLength, int lengthOfWordsArray, wordItem wordItemList[]);

/*
A function to sort the wordItemList array in descending order so the number that appears most is in index 0
Pre-conditions: wordItemList is an array of structure wordItem and is unsorted, lengthOfWordsArray is an in and the length of the array
Post-conditions: The array is sorted in descending order.
*/
void arraySort(wordItem wordItemList[], int lengthOfWordsArray);

/*
A function to sum up the total number of non-common words in the text file
Pre-conditions: wordItemList is an array of structure wordItem, lengtOfWordsArray is an int and the length of th array.
Post-conditions: returns the total number of non-common words in the text file
*/
int getTotalNumberNonStopWords(wordItem wordItemList[], int lengthOfWordsArray);

int main(int argc, char *argv[]){

    //store what's passed from the command line
    int topNWords = stoi(argv[1]);
    string input_file = argv[2];
    string ignoreWordFileName = argv[3];

    //create an array and fill it with stop words
    string ignoreWords[50];
    getStopWords(ignoreWordFileName, ignoreWords);

    //create an array to store each word found
    //as the list gets bigger, it will be doubled dynamically
    //when the array is doubled, maxLength will double as well and timesDoubled will be incremented
    int maxLength = 100;
    int timesDoubled = 0;
    wordItem wordItemList[maxLength];
    int lengthOfWordsArray = 0;

    //open the text file and make sure that opening worked
    ifstream in_stream(input_file);

    if(in_stream.is_open()){

        string currentLine = "";

        //Get each line in the text file
        while(getline(in_stream, currentLine)){

            //Create a stringstream out of the line
            stringstream wordStream;
            wordStream << currentLine;

            string word = "";

            //get each word until the end of the line
            while(getline(wordStream, word, ' ')){

                bool checkStopWord = isStopWord(word, ignoreWords);

                //If it's a stop word, don't count it
                if(checkStopWord){
                    cout<<"stop word."<<endl;
                }

                //Otherwise, we want to count the word
                else{

                    int checkFirstOccurrence = isFirstOccurrence(word, wordItemList, lengthOfWordsArray);

                    //If it's the first occurrence, we add this word to the array and increase its count.
                    if(checkFirstOccurrence == -1){
                        cout<<word<<" has appeared for the first time"<<endl;
                        wordItemList[lengthOfWordsArray].word = word;
                        wordItemList[lengthOfWordsArray].wordCount++;
                        lengthOfWordsArray++;
                    }

                    //If the word already exists in the array, we just want to increase its count.
                    else{
                       wordItemList[checkFirstOccurrence].wordCount++;
                       cout<<word<<" has appeared before"<<wordItemList[lengthOfWordsArray].wordCount<<" times"<<endl;
                    }

                    //Now, we want to check the length of the array to see if it needs to be doubled.
                    //This only needs to be done if not a stop word, as stop words aren't added to the array.
                    if(lengthOfWordsArray == 100){
                        doubleArray(maxLength, lengthOfWordsArray, wordItemList);
                        timesDoubled++;
                    }

                }

            }


        }

        arraySort(wordItemList, lengthOfWordsArray);
        int totalNumber = getTotalNumberNonStopWords(wordItemList, lengthOfWordsArray);

        for(int i = 0; i < lengthOfWordsArray; i++){
                cout<<wordItemList[i].word<< " "<<wordItemList[i].wordCount<<endl;
            }

        cout<<totalNumber<<" words were counted!"<<endl;
    }

    //If opening didn't work, print a message to the terminal and return -1
    else{
        cout<<"Error opening text file."<<endl;
        return -1;
    }

    return 1;
}

void getStopWords(string ignoreWordFileName, string ignoreWords[]){

    ifstream in_stream(ignoreWordFileName);

    //check to see if the file is open
    //if it is, fill the array with words; 1 word per line
    if(in_stream.is_open()){

        //index will keep track of the current line, and this the current array position
        int index = 0;
        string currentWord = "";

        while(getline(in_stream, currentWord)){

            ignoreWords[index] = currentWord;
            index++;

        }

    }

    //if there's and error, display a message to the console
    else{
        cout<<"Error opening the list of ignore words."<<endl;
    }

    in_stream.close();
}

bool isStopWord(string word, string ignoreWords[]){

    //check each word in the stop words array against the current word
    for(int i = 0; i < 50; i++){
        if(ignoreWords[i] == word){
            return true;
        }
    }

    //made it all the way through, so it must not be a stop word
    return false;
}
int isFirstOccurrence(string word, wordItem wordItemList[], int lengthOfWordsArray){

    //Check each wordItem structure to see if it is equal to the current word
    for(int i = 0; i < lengthOfWordsArray; i++){
        if(wordItemList[i].word == word){
            return i;
        }
    }

    //Made it through every item; it must be the first occurrence.
    return -1;

}

void doubleArray(int &maxLength, int lengthOfWordsArray, wordItem wordItemList[]){

    //double the maximum length of the array
    maxLength = maxLength * 2;

    //create a temporary array with the doubled length
    wordItem *temporaryArray = new wordItem[maxLength];

    //fill the temporary array with values from the array passed through
    for(int i = 0; i < lengthOfWordsArray; i++){
        temporaryArray[i] = wordItemList[i];
    }

    //set the array passed through equal to the temporary array
    //delete the temporary array t save memory
    wordItemList = temporaryArray;
    delete temporaryArray;
}

void arraySort(wordItem wordItemList[], int lengthOfWordsArray){

    //A standard descending bubble sort algorithm
    //Checks if the next value is greater than the current value and switches if so.
    //Continues until all items are sorted.
    for(int i = 0; i < lengthOfWordsArray - 1; i++){
        for(int j = 0; j < lengthOfWordsArray - i - 1; j++){
            if(wordItemList[j+1].wordCount > wordItemList[j].wordCount){
                wordItem Holder = wordItemList[j+1];
                wordItemList[j+1] = wordItemList[j];
                wordItemList[j] = Holder;
            }
        }
    }

}

int getTotalNumberNonStopWords(wordItem wordItemList[], int lengthOfWordsArray){

    int sum = 0;

    //go through every word and add the number of times it's been counted to the sum
    for(int i = 0; i < lengthOfWordsArray; i++){
        sum = sum + wordItemList[i].wordCount;
    }

    return sum;
}
