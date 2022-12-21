//
//  Project 2 - Personality Tests
//  Name: Deep Patel
// main.cpp contains lots of functions for performing the personality quiz
// functions in driver.h are used in main.cpp to generate random questions,
// store questions data, user's answers.
// Creative component "mine.people" can be tested if testnum == 9.
// "mine.people" contains list of countries and their oceans scores.
//

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

set<Person> read_people(string filename);

// output_options() to display available options to user.
void output_options() {
  cout << "1. Strongly disagree" << endl;
  cout << "2. Disagree" << endl;
  cout << "3. Neutral" << endl;
  cout << "4. Agree" << endl;
  cout << "5. Strongly agree" << endl << endl;
  cout << "Enter your answer here (1-5): ";
}

// give_questions() to display random question and store user's answer.
// "data" set of type Question to call randomQuestionFrom()
// "answers" map of type <Question, int> to store
// question's data and user's answers. "num" for no. of questions.
void give_questions(set<Question>& data, map<Question, int>& answers, int num) {
  while (num > 0) {  // until user's inputted number for questions are given
    int ans;
    cout << "How much do you agree with this statement?" << endl;
    Question quest;
    // returns a random question and sets equal to quest of type Question.
    quest = randomQuestionFrom(data);
    // displays questionText from quest.
    cout << '"' << quest.questionText << '"' << endl << endl;
    output_options();  // displays available options
    cin >> ans;  // takes in user's answer.
    cout << endl;
    // inserts quest(questionText and factors) and answer into map
    answers.emplace(quest, ans);
    num -= 1;
  }
}

// give_test() to display the matching characterwith the user's score.
// "pep" set of type Person for storing characters data
// from "filename" according to user selected test number.
// "ocean" map of type <char, int> containing user's ocean score to compare.
string give_test(set<Person> pep, int num, map<char, int>& ocean,
string filename) {
  // assign the data of each character's name and ocean scores to "pep" set.
  pep = read_people(filename);  // reads all the character's data from filename
  Person character;
  // mostSimilarTo() returns the matched character
  character = mostSimilarTo(ocean, pep);  // asgigns the match to character
  cout << "You got ";
  return character.name;  // returns the name of the matched character
}

// parseString() to parse positive and negative type for OCEAN.
// "fact" string contains everything after the question
// "quest" of type Question to store the parsed char and int
void parseString(string fact, Question& quest) {
  for (int i = 0; i < fact.length(); i++) {
    if (fact.at(i) == ':') {  // if the colon is found in string
      // checks if the number to ocean type is negative
      if (fact.at(i+1) == '-') {
        quest.factors.emplace(fact.at(i-1), -1);  // inserts char and -1
      } else {
        quest.factors.emplace(fact.at(i-1), 1);  // inserts char and 1
      }
    }
  }
}

// read_Questions() to read the questions from "filename"
set<Question> read_questions(string filename) {
  set<Question> data;  // for returing each question's questionText and factors
  ifstream infile(filename);
  while (!infile.eof()) {
    string line;
    string fact;
    getline(infile, line);  // takes in whole line
    Question quest;  // for questionText and factors
    if (!infile.fail()) {
      // inserts the question in quest
      quest.questionText = line.substr(0, line.find('.')+1);
      fact = line.substr(line.find('.')+2);  // string after the question ends
      parseString(fact, quest);  // inserts factors of a question
      // inserts quest(questionText & factors) of each question to "data" set
      data.insert(quest);
    }
  }
  return data;  // returns set with all the question's data.
}

// parseScore() to parse the ocean char and int
// "fact" string after the character's name.
// "p" of type Person to store character's factors.
void parseScores(string fact, Person& p) {
	if(fact.length() <= 0) {
		return;
	}
	int start_ind = 0;
	int space_ind = fact.find(" ");  // to divide the pairs
	string temp = fact.substr(start_ind, space_ind);  // a pair of char and int
	int toint = stoi(temp.substr(2));  // convert the score to int
  p.scores.emplace(temp[0], toint);  // inserts the char at temp[0] and int score
	fact.erase(start_ind, space_ind + 1); // erases that part after inserting
	space_ind = fact.find(" ");  // finds the first space index in the current string
	while(space_ind > 0) {  // while space ind is present in the string
    // repeats the same procedure.
		temp = fact.substr(start_ind, space_ind);
    toint = stoi(temp.substr(2));
    p.scores.emplace(temp[0], toint);
    fact.erase(start_ind, space_ind + 1);
    space_ind = fact.find(" ");
	}
  // for end cases when space is not present in string
	toint = stoi(fact.substr(2));
  p.scores.emplace(fact[0], toint);
}

// read_people to read all the character's data from "filename"
set<Person> read_people(string filename) {
  set<Person> pep;  // to return a set of each character's data(name and scores)
  ifstream infile(filename);
  while (!infile.eof()) {
    string line;
    getline(infile, line);  // takes in whole line
    if (!infile.fail()) {
      string fact;
      Person p;  // for inserting the name and score
      p.name = line.substr(0, line.find('.'));  // assigns character's name
      fact = line.substr(line.find('.')+2);
      parseScores(fact, p);  // inserts that character's scores
      pep.insert(p);  // insert name and score of that character in "pep" set.
    }
  }
  return pep;  // returns the set containing each character's data
}

// test_options() to display available test options user can select from.
void test_options() {
  cout << "1. BabyAnimals" << endl;
  cout << "2. Brooklyn99" << endl;
  cout << "3. Disney" << endl;
  cout << "4. Hogwarts" << endl;
  cout << "5. MyersBriggs" << endl;
  cout << "6. SesameStreet" << endl;
  cout << "7. StarWars" << endl;
  cout << "8. Vegetables" << endl;
  cout << "9. mine" << endl;
  cout << "0. To end program." << endl << endl;
}

void test_num(set<Person>& pep, map<char, int>& ocean) {
  test_options();
  int testnum;
  cout << "Choose test number (1-9, or 0 to end): ";
  cin >> testnum;  // test option which user has to select.
  while (testnum != 0) {
    // Performs appropriate statement to testnum entered
    if (testnum == 1) {
      cout << give_test(pep, testnum, ocean, "BabyAnimals.people")
      << '!' << endl << endl;
    } else if (testnum == 2) {
      cout << give_test(pep, testnum, ocean, "Brooklyn99.people")
      << '!' << endl << endl;
    } else if (testnum == 3) {
      cout << give_test(pep, testnum, ocean, "Disney.people")
      << '!' << endl << endl;
    } else if (testnum == 4) {
      cout << give_test(pep, testnum, ocean, "Hogwarts.people")
      << '!' << endl << endl;
    } else if (testnum == 5) {
      cout << give_test(pep, testnum, ocean, "MyersBriggs.people")
      << '!' << endl << endl;
    } else if (testnum == 6) {
      cout << give_test(pep, testnum, ocean, "SesameStreet.people")
      << '!' << endl << endl;
    } else if (testnum == 7) {
      cout << give_test(pep, testnum, ocean, "StarWars.people")
      << '!' << endl << endl;
    } else if (testnum == 8) {
      cout << give_test(pep, testnum, ocean, "Vegetables.people")
      << '!' << endl << endl;
    } else if (testnum == 9) {  // creative component
      // ocean scores matched with country and best match is returned
      cout << give_test(pep, testnum, ocean, "mine.people")
      << '!' << endl << endl;
    }
    test_options();
    cout << "Choose test number (1-9, or 0 to end): ";
    cin >> testnum;  // until 0 is entered
  }
  cout << "Goodbye!" << endl;
}

/*
* main() to make calls to functions which perform operations
* for personality quiz. Also, user driver.h functions.
* test_num menu for the user and creative component file
*/
int main() {
  cout << "Welcome to the Personality Quiz!" << endl << endl;
  cout << "Choose number of questions: ";
  int numquest, testnum;
  cin >> numquest;  // number of question user wants to answer
  cout << endl;
  // reads the questions from file and stores the data in set
  set<Question> data =  read_questions("questions.txt");
  set<Person> pep;
  map<Question, int> answers;
  // gives the questions and stores user's answers
  give_questions(data, answers, numquest);
  map<char, int> ocean = scoresFrom(answers);  // user's score is stored in ocean
  test_num(pep, ocean);
  return 0;
}
