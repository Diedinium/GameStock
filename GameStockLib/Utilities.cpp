#include "Utilities.h"

void util::pause() {
    std::cout << "Press any key to continue...\n";
    auto value = _getch();
}