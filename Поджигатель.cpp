#include <iostream>
#include "Figure.h"


int main()
{
    setlocale(LC_ALL, "rus");
    Figure figure;
    try {
        figure.OpenFile();
    } catch (const char* error_message) {
        cout << error_message << std::endl;
    }
}