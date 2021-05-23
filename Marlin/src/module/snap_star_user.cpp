#include "snap_star_user.h"

bool status = false;
int btnCnt = 0;

void SnapStarUser::setup() {
    // control_.io.Write(IO1, 1);
    // control_.pwm.Out(PWM4, 10000, 50);
    // bool val;
    // control_.io.Read(IO2, val);
    // control_.timer.DelayMs(1000);
    // control_.timer.Millis();
    control_.io.Write(IO1, 0);

}

void SnapStarUser::loop() {
    // status check
    // control_.io.Write(IO1, 1);
    // control_.timer.DelayMs(1000);
    // control_.io.Write(IO1, 0);
    // control_.timer.DelayMs(1000);

    // control_.pwm.Out(PWM9, 10000, 0); // PB7 -> Base Moter
    // control_.pwm.Out(PWM8, 10000, 100); // PB6 
    // control_.io.Write(IO16, 0);

    
    // button tirgger 
    bool buttonPressed = false;
    control_.io.Read(IO0, buttonPressed);
    if (!buttonPressed) {
        btnCnt++;
        if (btnCnt > 3) {
            if (!status) {
                status = true;
                control_.io.Write(IO1, 1);
                control_.pwm.Out(PWM9, 10000, 100); // PB7 -> Base Moter
                control_.pwm.Out(PWM8, 10000, 100); // PB6 
            } else {
                status = false;
                control_.io.Write(IO1, 0);
                control_.pwm.Out(PWM9, 10000, 0); // PB7 -> Base Moter
                control_.pwm.Out(PWM8, 10000, 0);
            }
            control_.timer.DelayMs(1000);
        }
    } else {
        btnCnt = 0;
    }

    // // Step Moter movement
    // control_.io.Write(IO2, 1); // dir
    // int curTime = control_.timer.Millis();
    // int interval = 1000;
    // const int PPR = 312;
    // const int RPM = 30;  // control this element 
    // int targetInterval = PPR * 60 / RPM;
    // int pulseWidth = 30;


    // control_.pwm.Out(PWM1, interval, 50);
    // while (true)
    // {
    //     control_.pwm.Out(PWM1, interval, 10);
    // }
    

    // control_.pwm.Out(PWM1, interval, 50);
    // while (true) {
        

    //     if (interval > targetInterval) {
    //         int newTime = control_.timer.Millis();
    //         if (newTime > curTime + 100) {
    //             interval = interval / 2;
    //             if (interval < targetInterval) {
    //                 interval = targetInterval;
    //             }
    //             curTime = newTime;
    //             control_.pwm.Out(PWM1, interval, 50);
    //         }
    //     }
    // }
}
