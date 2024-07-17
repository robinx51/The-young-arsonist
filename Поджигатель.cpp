#include <iostream>
#include "Figure.h"


int main()
{
    setlocale(LC_ALL, "rus");
    Figure figure;
    try {
        figure.OpenFile();
        figure.Print();
        figure.GetTime();
    } catch (const char* message) {
        cout << message << endl;
    } catch (string message) {
        cout << message << endl;
    }
}