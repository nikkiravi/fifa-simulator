#include <iostream>
#include <memory>
#include "models/AttackerPlayer.h"

int main() {
  auto messi = std::make_unique<AttackPlayer>("Lionel Messi", "Argentina", "Barcelona", "LW", 91, 97, 88, 82, 92, 39, 78);
  messi->displayInfo();
  std::cout << "\nMilestone 1 is Complete!\n";
  return 0;
}