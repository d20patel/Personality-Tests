// driver.h contains functions required to call in main.cpp
// to implement the Personality Test.
#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
      if (i == ind) {
          return e;
      }
      i++;
    }
    return {};
}

/* randomQuestionFrom
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns quest of type Question. Also, once the random question is
 * selected, the question is removed from questions set.
*/
Question randomQuestionFrom(set<Question>& questions) {
    Question quest;  // question data to return
    if (questions.size() == 0) { // set is empty
      throw runtime_error("questions set: empty");
    }
    quest = randomElement(questions);  // to get random question from set
    questions.erase(quest);  // removes the questions from questions set
    return quest;  // returns the random question's data
}

/* scoresFrom
 *
 * This function takes in "answers" map containing <Question, int>
 * which has Question data and user's answer to the question. 
 * Then, returns a map containing ocean scores.
*/
map<char, int> scoresFrom(map<Question, int>& answers) {
    map<char, int> ocean;  // for returning the ocean score
    for (auto &q : answers) {  // each <Question, int> in "answers" map
      for (auto &c : q.first.factors) {  // each factor to that question
        char key = c.first;
        if (q.first.factors.count(key) == 1) {  // if factor is found
          // applies the formula to count score.
          int score = (q.second - 3) * q.first.factors.at(key); 
          if (ocean.count(key) == 0) {  // if factor is not already existing
            ocean.emplace(key, score);
          } else {
            ocean.at(key) += score;  // manipulates the score without inserting
          }
        }
      }
    }
    return ocean;  // returns the ocean score in form of map.
}

/* normalize
 *
 * This function takes in "scores" map containing <char, int>
 * to normalize the score and return a map containing <char, double>
 * with normalized scores version of the passed parameter.
*/
map<char, double> normalize(map<char, int>& scores) {
    int count = 0;
    for (auto &i : scores) {  // for checking non-zero scores
      if (i.second == 0) {
        count++;
      }
    }
    if (count == scores.size()) {  // if all scores are zero
      throw runtime_error("Dividing by Zero");
    }
    map<char, double> normalized_scores;  // map to return normalized scores
    double squared_sum = 0.0;
    for (auto &pair : scores) {  // each score is squard and added to sum
      squared_sum += double(pair.second * pair.second);
    }
    double normalize = sqrt(squared_sum);  // square root of the squared_sum

    for (auto &item : scores) {  // each score is normalized using the formula
      double final_score = item.second/normalize;
      // normalized version is emplaced
      normalized_scores.emplace(item.first, final_score);
    }
    // map containing normalized scores for each factor
    return normalized_scores;
}

/* cosineSimilarityOf
 *
 * This function takes in "lhs" and "rhs" of type map
 * containing <char, double> to calculate the cosine similarity
 * of two person's normalized version of ocean scores.
*/
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
    double cosine_similarity;
    for (auto &l : lhs) {
      // if char is found in both person
      if (rhs.count(l.first) == 1) {
        // scores at that key is multiplied and then added to cosine_similarity
        cosine_similarity += rhs.at(l.first) * l.second;
      }
    }
    return cosine_similarity;  // returns the double digit number
}

/* mostSimilarTo
 *
 * This function takes in "scores" map containing <char, int>
 * and "people" set of type Person. "scores" of user is compared with
 * each character in the people set and the character with highest
 * cosine similarity is returned.
 * 
*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
  if (people.size() == 0) {  // if set is empty
    throw runtime_error("people set: empty");
  }
  Person character;  // character to be returned after matched
  map<char, double> normalized_scores = normalize(scores);
  double max_score = -1.0;
  for (auto &person : people) {  // each person in people set
    map<char, int> person_raw = person.scores;
    map<char, double> normalized_person = normalize(person_raw);
    // user's score is compared by calling cosineSimilarityOf()
    double temp = cosineSimilarityOf(normalized_scores, normalized_person);
    if (temp > max_score) {  // for finding the best match 
      max_score = temp;
      character = person;
    }
  }
  return character;  // returns the most similar character
}
