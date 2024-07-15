#include <iostream>
#include "Figure.h"


int main()
{
    setlocale(LC_ALL, "rus");
    Figure figure;
    try {
        figure.OpenFile();
        figure.Print();
        figure.PrintMatrix();
    } catch (const char* error_message) {
        cout << error_message << std::endl;
    }
}