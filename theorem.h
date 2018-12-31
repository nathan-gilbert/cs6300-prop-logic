/*
 * Nathan Gilbert
 *
 * Theorem class
 *
 * This class is meant to hold a single implication or assertion.
 *
 * Assertions are taken to be implications with an empty premise for
 * the sake of not having to create two seperate classes. During use,
 * assertions are distinguished from implications by returning true
 * for isAxiom()
 *
 */
#ifndef THEOREM_H
#define THEOREM_H

#include <string>
#include <vector>

using namespace std;

class Theorem {
public:
  Theorem();
  Theorem(string);
  Theorem(string, string);
  ~Theorem();

  /* Sets the consequence of this implication/assertion. */
  void setRHS(string);
  string getRHS(void);

  /* Sets the premise */
  void setLHS(string);
  vector<string> getLHS(void);

  /* isAxiom returns true iff this theorem instance is an assertion. */
  bool isAxiom(void);
  void setAxiom(bool);

  /* returns true if string is among this theorems premises. */
  bool findLHS(string);

  /* Returns a string representation of this implication */
  string toString(void);

private:
  void tokenize(string);
  string strip(string, const char *);
  bool axiom;

  /* Premises */
  vector<string> lhs;

  /* Consequences */
  string rhs;
};

#endif
