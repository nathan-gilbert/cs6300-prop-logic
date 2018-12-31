/*
 * Nathan Gilbert
 * Forward and Backward Inference engine.
 */

#include "theorem.h"
#include "util.h"
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <popt.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

/* Useful globals */
int trace = false;
int stats = false;
int debug = false;
int facts = 0;
int implications = 0;
static int facts_verifed = 0;
static int facts_attempted = 0;
vector<Theorem> kb;

/* Function prototypes. */
void startCLI(void);
void insertAssert(string);
void insertTheorem(string);
void backward(string);
bool backChain(vector<Theorem>, string);
void forward(string);
bool forwardChain(string, vector<Theorem>);

/* Command line argument code. */
char *Usage = "[OPTION...]";
struct poptOption OptionsTable[] = {
    {"stats", 's', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &stats, 0,
     "For each forward chaining inference, give the count of the number of "
     "facts generated. For each backward chaining inference, give the counts "
     "of the number of facts for which verification was attempted and the "
     "number actually verified.",
     NULL},
    {"debug", 'd', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &debug, 0,
     "Print extra information that is useful in debugging.", NULL},
    {"trace", 't', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &trace, 0,
     "For each forward chaining inference, print the proposition that is being "
     "verified and then print in order each derived proposition. For each "
     "backward chaining inference, print in order every proposition for which "
     "support is searched for using the recursive backward chaining algorithm, "
     "along with every proposition found to be asserted as a result of the "
     "backward chaining search.",
     NULL},
    POPT_AUTOHELP{NULL, 0, 0, NULL, 0, NULL, NULL}};

// Where the magic happens.
int main(int argc, char *argv[]) {
  // libpopt code
  poptContext PoptContext;
  int PoptReturnCode;

  PoptContext = poptGetContext(NULL, argc, (const char **)argv, OptionsTable,
                               POPT_CONTEXT_POSIXMEHARDER);
  poptSetOtherOptionHelp(PoptContext, Usage);
  PoptReturnCode = poptGetNextOpt(PoptContext);

  if (PoptReturnCode != -1) {
    fprintf(stderr, "%s: %s\n",
            poptBadOption(PoptContext, POPT_BADOPTION_NOALIAS),
            poptStrerror(PoptReturnCode));
    return (EXIT_FAILURE);
  }

  if (stats)
    cout << "-stats option enabled. " << endl;

  if (trace)
    cout << "-trace option enabled. " << endl;

  if (debug)
    cout << "-debug option enabled. " << endl;

  // Start the command line interface.
  startCLI();

  return (EXIT_SUCCESS); /* normal exit */
}

/*
 * This function takes in the user input string and insert the assertions
 * into the knowledge base.
 */
void insertAssert(string in) {
  // Has the maximum number of facts been reached?
  if (facts >= 100) {
    cout << "No more facts can be declared. " << endl;
    return;
  }

  if (debug)
    cout << in.substr(in.find(' ') + 1, in.length() - 1) << endl;

  string rhs = in.substr(in.find(' ') + 1, in.length() - 1);

  // Is the assertion the proper number of character long?
  if (rhs.length() >= 20) {
    cout << "Assertion length is greater than 20 characters, it will not be "
            "entered into the knowledge base. "
         << endl;
    return;
  }

  if (rhs != "") {
    // Create the theorem.
    Theorem thm(strip(rhs, " "));

    // Push it onto the knowledge base vector.
    kb.push_back(thm);

    // Increment the total number of facts.
    facts++;
  } else {
    // If this fires, there was a major problem somewhere.
    cerr << "There was a fatal error with this assertion." << endl;
    exit(EXIT_FAILURE);
  }
}

/*
 * The following function takes a string of user input
 * and inserts the implication into the knowledge base.
 */
void insertTheorem(string in) {
  // Has the maximum number of implications been reached?
  if (implications >= 100) {
    cout << "No more implications can be declared. " << endl;
    return;
  }

  if (debug)
    cout << in.substr(in.find(' ') + 1, (in.rfind(' ') - in.find(' '))) << endl;

  // Divide the input string into a left-hand side and right-hand side of an
  // implication.
  string lhs = in.substr(in.find(' ') + 1, (in.rfind(' ') - in.find(' ')));
  string rhs = in.substr(in.rfind(' '));

  if (debug)
    cout << lhs << endl << rhs << endl;

  if (rhs != "" && lhs != "") {
    // Create the implication.
    Theorem thm(strip(lhs, " "), strip(rhs, " "));

    // Check each premise for size constraints.
    for (unsigned int i = 0; i < thm.getLHS().size(); i++) {
      if (thm.getLHS()[i].length() > 20) {
        cout << "To many characters in premise: " << thm.getLHS()[i]
             << ". This implication will not be added to the knowledge base."
             << endl;
        return;
      }
    }

    // Check the number of premises.
    if (thm.getLHS().size() >= 20) {
      cout << "To many premises in this implication, it will not be added to "
              "the knowledge base."
           << endl;
      return;
    } else {
      // Insert the implication into the knowledge base.
      kb.push_back(thm);
      implications++;
    }
  } else {
    cerr << "There was a fatal error with this implication." << endl;
    exit(EXIT_FAILURE);
  }
}

/*
 * This is the meat of the command line interface for this
 * system.
 *
 * For more information on the case codes, see util.cpp -> parseInput()
 */
void startCLI(void) {
  string input;
  bool done = false;

  while (!done) {
    cout << "%> ";
    getline(cin, input);

    input = strip(input, " ");

    switch (parseInput(input)) {
    case -3:
      break; // bad character in theorem.
    case -2:
      break; // bad character.
    case -1:
      break; // input over 100 characters.
    case 0:
      done = true;
      break; //"q"
    case 1:
      printOptions(); //"?"
      break;
    case 2:
      insertAssert(input);
      break;
    case 3:
      insertTheorem(input);
      break;
    case 4:
      prettyPrint();
      break;
    case 5:
      backward(input); // run backward algorithm.
      break;
    case 6:
      forward(input); // run forward algorithm.
      break;
    case 7:
      kb.clear(); // clear the knowledge base.
      cout << ">>Knowledge base cleared." << endl;
      break;
    case 8:
      trace = !trace;
      cout << "Trace is now " << (trace ? "on" : "off") << endl;
      break;
    default:
      cout << ">>Unknown option: " << input << endl;
    }
  }
}

/*
 * The primer function for the backward chaining algorithm.
 */
void backward(string in) {
  facts_attempted++;

  string q;

  q = in.substr(in.find(' ') + 1);

  if (debug)
    cout << q << endl;

  if (trace)
    cout << "Attempting to verify " << q << " using backwards chaining."
         << endl;

  if (backChain(kb, q)) {
    cout << q << " is true. " << endl;
  } else
    cout << q << " is unsupported. " << endl;

  if (stats) {
    if (facts_verifed != 0)
      cout << facts_attempted << " attempted fact verfications, "
           << facts_verifed << " actually verified. " << endl;
    else
      cout << facts_attempted << " attempted fact to verify." << endl;

    facts_verifed = 0;
    facts_attempted = 0;
  }
}

/*
 * The actual recursive implementation of the backward algorithm.
 */
bool backChain(vector<Theorem> base, string query) {

  // Base case.
  for (unsigned int i = 0; i < base.size(); i++) {
    if (base[i].getRHS() == query && base[i].isAxiom()) {
      if (trace)
        cout << "backwards chaining: " << query << " in assertion database. "
             << endl;

      if (stats && facts_verifed == 0 && facts_attempted == 1)
        facts_verifed++;

      return true;
    }
  }

  // Recursive case.
  for (unsigned int i = 0; i < base.size(); i++) {
    int lhsSize = 0;
    int count = 0;
    string predicate;

    if (base[i].getRHS() == query) {
      lhsSize = (int)base[i].getLHS().size();

      for (int j = 0; j < lhsSize; j++) {
        predicate = base[i].getLHS()[j];

        if (trace)
          cout << "Attempting to verify " << predicate
               << " using backward chaining." << endl;

        if (stats)
          facts_attempted++;

        if (backChain(base, predicate)) {
          count++;
        }
      }

      // If these numbers match, then all antecedents for the current
      // implication have been found to be in the assertion database.
      if (count == lhsSize) {
        if (trace)
          cout << "backward chaining: " << query << " has been derived."
               << endl;

        if (stats)
          facts_verifed++;

        return true;
      }
    }
  }

  return false;
}

/*
 * Primer function for forward chaining algorithm.
 */
void forward(string in) {
  facts_attempted++;

  string query;
  query = in.substr(in.find(' ') + 1);

  if (debug)
    cout << query << endl;

  if (trace)
    cout << "Attempting to verify " << query << " using forward chaining."
         << endl;

  /*
   * Checking the trivial case that you are trying to verify something already
   *  in the assertion database.
   */
  for (unsigned int i = 0; i < kb.size(); i++) {
    if (kb[i].getRHS() == query && kb[i].isAxiom()) {
      facts_verifed++;

      if (trace)
        cout << "forward chaining: " << query << " in assertion database. "
             << endl;

      cout << query << " is true. " << endl;

      if (stats) {
        if (facts_verifed == 1)
          cout << facts_verifed << " fact verified. " << endl;
        else
          cout << facts_verifed << " facts generated. " << endl;

        facts_verifed = 0;
        facts_attempted = 0;
      }

      return;
    }
  }

  // Start forward chaining...
  if (forwardChain(query, kb)) {
    cout << query << " is true. " << endl;

    if (stats) {
      if (facts_verifed == 1)
        cout << facts_verifed << " fact verified. " << endl;
      else
        cout << facts_verifed << " facts generated. " << endl;

      facts_verifed = 0;
      facts_attempted = 0;
    }
  } else {
    cout << query << " is unsupported. " << endl;

    if (stats) {
      if (facts_verifed == 1)
        cout << facts_verifed << " fact generated. " << endl;
      else
        cout << facts_verifed << " facts generated. " << endl;

      facts_verifed = 0;
      facts_attempted = 0;
    }
  }
}

/*
 * The forward chaining algorithm.
 */
bool forwardChain(string query, vector<Theorem> base) {
  map<string, bool>
      inferred;           // so derived implications are derived a second time.
  map<string, int> count; // keeps track of each implications premises.
  list<string> agenda; // keeps track of the current assertion being generated.
  string p;

  if (debug)
    cout << query << endl;

  // Initialize variables.
  for (unsigned int i = 0; i < base.size(); i++) {
    // If base[i] is an assertion add it to the agenda.
    if (base[i].isAxiom()) {
      agenda.push_back(base[i].getRHS());
      inferred.insert(make_pair(base[i].getRHS(), false));
    } else {
      // insert this implication and the # of it's premises.
      count.insert(make_pair(base[i].toString(), (int)base[i].getLHS().size()));
    }
  }

  if (debug) {
    cout << inferred.size() << endl;
    cout << agenda.size() << endl;
  }

  while (!agenda.empty()) {
    p = agenda.front();
    agenda.pop_front();

    // If this symbol hasn't been derived yet...
    if (!inferred[p]) {
      inferred[p] = true;

      for (unsigned int i = 0; i < base.size(); i++) {
        // Note that all assertions are skipped during this step, and only
        // implications are checked in the order they were declared. For more
        // information see theorems.cpp.
        if (base[i].findLHS(p)) {
          count[base[i].toString()] = count[base[i].toString()] - 1;

          if (count[base[i].toString()] == 0) {
            if (trace)
              cout << "forward chaining: derived " << base[i].getRHS()
                   << " using forward chaining. " << endl;

            if (stats)
              facts_verifed++;

            if (base[i].getRHS() == query) {
              return true;
            }

            agenda.push_back(base[i].getRHS());
          }
        }
      }
    }
  }

  return false;
}
