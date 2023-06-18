#include "UI.h"
#include <iostream>

void UI::showCommands() {
    std::cout << "1. create <regex>\n"
                 "2. determinize <idx>\n"
                 "3. totalize <idx>\n"
                 "4. accepts <idx> <word>\n"
                 "5. isEmptyLanguage <idx>\n"
                 "6. union <idx> <idx>\n"
                 "7. concat <idx> <idx>\n"
                 "8. kleeneStar <idx>\n"
                 "9. help\n"
                 "10. quit" << std::endl;
}

void UI::invalidIndex() {
    std::cout << "Invalid index!";
}
