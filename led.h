/**
 * This is the public interface for the GPIO25 control module
 *
 * @author Yaoyu
 * @date March 20 2013
 */ 

int turnOn();

int turnOff();

int inquireStatus();

int pwm(double duty, double full);

void setup_io();
