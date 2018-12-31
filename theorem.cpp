
#include "theorem.h"

// Constructors
Theorem::Theorem() {
  rhs = "";
  lhs.clear();
  axiom = false;
}

Theorem::Theorem(string l, string r) {
  rhs = r;
  tokenize(l);
  axiom = false;
}

Theorem::Theorem(string r) {
  rhs = r;
  lhs.clear();
  axiom = true;
}

Theorem::~Theorem() {
  lhs.clear();
  rhs = "";
}

// Accessor/Modifier methods
void Theorem::setRHS(string s) { rhs = s; }

string Theorem::getRHS(void) { return rhs; }

void Theorem::setLHS(string s) { tokenize(s); }

vector<string> Theorem::getLHS(void) { return lhs; }

bool Theorem::isAxiom(void) { return axiom; }

void Theorem::setAxiom(bool a) { axiom = a; }

/*
 * Returns true is p is found in the premise
 * of this theorem.
 */
bool Theorem::findLHS(string p) {
  if (axiom)
    return false;

  for (unsigned int i = 0; i < lhs.size(); i++) {
    if (lhs[i] == p)
      return true;
  }

  return false;
}

string Theorem::toString(void) {
  string predicate = "";

  for (unsigned i = 0; i < lhs.size(); i++) {
    if (i < lhs.size() - 1)
      predicate += lhs[i] + " /\\ ";
    else
      predicate += lhs[i] + " ";
  }

  return predicate + "-> " + rhs;
}

// A private member function to tokenize user input strings
// and place them in the premise container.
void Theorem::tokenize(string s) {
  string tokens = "";

  for (unsigned i = 0; i < s.length(); i++) {
    if (s[i] == ' ') {
      lhs.push_back(strip(tokens, " "));
      tokens = "";
      continue;
    } else
      tokens += s[i];
  }

  if (tokens != "") {
    lhs.push_back(tokens);
  }
}

// A private member function to strip whitespace from
// strings.
string Theorem::strip(string str, const char *sep) {
  string::size_type first = str.find_first_not_of(sep);
  return (first == string::npos)
             ? string()
             : str.substr(first, str.find_last_not_of(sep) - first + 1);
}
