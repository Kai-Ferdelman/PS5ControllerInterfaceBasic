// PS5Readout.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <ds5w.h>

int main(int argc, char** argv)
{
    DS5W::DeviceEnumInfo infos[16];

    unsigned int controllersCount = 0;

    switch (DS5W::enumDevices(infos, 16, &controllersCount)) {
    case DS5W_OK:

    case DS5W_E_INSUFFICIENT_BUFFER:

        break;
    default:
        return -1;
    }

    if (!controllersCount) {
        return -1;
    }

    DS5W::DeviceContext con;

    if (DS5W_FAILED(DS5W::initDeviceContext(&infos[0], &con))) {
        return -1;
    }

    while (true) {
        //main loop
        DS5W::DS5InputState inState;
        DS5W::DS5OutputState outState;

        ZeroMemory(&outState, sizeof(DS5W::DS5OutputState));

        if (DS5W_SUCCESS(DS5W::getDeviceInputState(&con, &inState))) {

            outState.leftRumble = inState.leftTrigger;
            outState.rightRumble = inState.rightTrigger;
            outState.lightbar.r = abs(inState.leftStick.x);
            outState.lightbar.g = abs(inState.rightStick.x);

            DS5W::setDeviceOutputState(&con, &outState);

            if (inState.buttonsA & DS5W_ISTATE_BTN_A_MENU) {
                std::cout << "Menu button is pressed\n";
            }

            if (inState.buttonsA & DS5W_ISTATE_BTN_A_SELECT) {
                std::cout << "Select button is pressed\n";
            }

            if (inState.buttonsB & DS5W_ISTATE_BTN_B_PAD_BUTTON) {
                std::cout << "Touch pad button pressed\n";
            }

            if (inState.buttonsB & DS5W_ISTATE_BTN_B_MIC_BUTTON) {
                std::cout << "Microfone button is pressed\n";
            }

            if (inState.buttonsB & DS5W_ISTATE_BTN_B_PLAYSTATION_LOGO) {
                return 1;
            }

            std::cout << inState.touchPoint1.x;
            std::cout << "   ";
            std::cout << inState.touchPoint1.y;
            std::cout << "\n";
        }
    }

    DS5W::freeDeviceContext(&con);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
