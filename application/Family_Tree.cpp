#include "Menu.hpp"
#include "Person.hpp"

int main() {
  Menu<Person> m;
  m.greeting();
  while (m.getState() != 9) {
    m.mainScreen();
  }
  return 0;
}