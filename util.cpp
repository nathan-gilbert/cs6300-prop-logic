
#include "util.h"

string strip(string str, const char *sep) {
  string::size_type first = str.find_first_not_of(sep);
  return (first == string::npos)
             ? string()
             : str.substr(first, str.find_last_not_of(sep) - first + 1);
}

void printOptions(void) {
  cout << "\t\tA: P" << setw(67) << "[P is true.]" << endl;
  cout << "\t\tI: P1 P2 P3 ... Pn Q" << setw(65) << "[P1 & P2 & ... & PN ==> Q]"
       << endl;
  cout << "\t\tB: P" << setw(86) << "[is P true (backward chaining)]" << endl;
  cout << "\t\tF: P" << setw(85) << "[is P true (forward chaining)]" << endl;
  cout << "\t\tP" << setw(88) << "[print facts and implications]" << endl;
  cout << "\t\tC" << setw(88) << "[clear facts and implications]" << endl;
  cout << "\t\t!trace" << setw(70) << "[toggles tracing]" << endl;
  cout << "\t\t?" << setw(85) << "[print summary of commands]" << endl;
  cout << "\t\tQ" << setw(64) << "[quit]" << endl;
}

int parseInput(string in) {
  if (in.length() > 99) {
    cout << "Input over 100 characters. Please try again." << endl;
    return -1;
  }

  // Checks for blank input string.
  if (in == "")
    return -100;

  if (in == "P" || in == "p")
    return 4;

  if (in == "q" || in == "Q")
    return 0;

  if (in == "?")
    return 1;

  if (in == "C" || in == "c")
    return 7;

  if (in == "!trace")
    return 8;

  // Checking facts input.
  if (in.at(0) == 'A' && in.at(1) == ':' && in.at(2) == ' ') {
    for (unsigned int i = 3; i < in.length(); i++) {
      if (!isalnum(in.at(i)) && in.at(i) != '_') {
        cout << "Incorrect character: " << in.at(i) << " in assertion. "
             << endl;
        return -2;
      }
    }

    return 2;
  }

  if (in.at(0) == 'I' && in.at(1) == ':') {
    for (unsigned int i = 2; i < in.length(); i++) {
      if (!isalnum(in.at(i)) && in.at(i) != '_' && in.at(i) != ' ') {
        cout << "Incorrect character: " << in.at(i) << " in implication. "
             << endl;
        return -3;
      }
    }

    return 3;
  }

  if (in.at(0) == 'B' && in.at(1) == ':') {
    for (unsigned int i = 2; i < in.length(); i++) {
      if (!isalnum(in.at(i)) && in.at(i) != '_' && in.at(i) != ' ') {
        cout << "Incorrect character: " << in.at(i) << "." << endl;
        return -3;
      }
    }

    return 5;
  }

  if (in.at(0) == 'F' && in.at(1) == ':') {
    for (unsigned int i = 2; i < in.length(); i++) {
      if (!isalnum(in.at(i)) && in.at(i) != '_' && in.at(i) != ' ') {
        cout << "Incorrect character: " << in.at(i) << "." << endl;
        return -3;
      }
    }

    return 6;
  }

  return -100;
}

void prettyPrint(void) {
  string facts = "", theorems = "";

  for (unsigned i = 0; i < kb.size(); i++) {
    if (kb[i].isAxiom()) {
      facts += "\t" + kb[i].getRHS() + "\n";
    } else {
      theorems += "\t" + kb[i].toString() + "\n";
    }
  }

  cout << "Facts:" << endl
       << facts << endl
       << "Implications:" << endl
       << theorems << endl;
}
