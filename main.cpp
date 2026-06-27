#include <iostream>
#include "Word.h"

int main() {
    Word a("GATTAC");
    Word b("GATTTT");

    std::cout << a.getText() << " - "
              << b.getText() << " : "
              << a.cosine(b) << std::endl;

    return 0;
}
