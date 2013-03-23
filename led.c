/**
 * This module implements library functions for controlling a transistor connected to
 *  a GPIO pin on the Raspberry Pi single-board computer.
 *
 * @author  Yaoyu Yang
 * @credits Revised from Chris Takahashi, Aaron Parks
 * @date March 20 2013
 */


#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "led.h"

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_map;
int status = 0;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0


/**
 * Put a high value on GPIO pin 25
 */
int turnOn() {

  setup_io();

  //making pin 25 an output
  INP_GPIO(25); //must make pin an input first.  its not clear why.
  OUT_GPIO(25);

  //Switch ON the transistor
  GPIO_SET = 1<<25;
  status = 1;

  return 0;

} // turnOn

/**
 * Put a low value on GPIO pin 25
 */
int turnOff() {

  setup_io();

  //making pin 25 an output
  INP_GPIO(25); //must make pin an input first.  its not clear why.
  OUT_GPIO(25);

  //Switch OFF the transistor
  GPIO_CLR = 1<<25;
  status = 0;
  return 0;

} // turnOff

int inquireStatus() {
  return status;
}

int pwm(double duty, double full) {
setup_io();
// duty means duty cycle of the pwm
  //making pin 25 an output
  INP_GPIO(25); //must make pin an input first.  its not clear why.
  OUT_GPIO(25);

  //int full = 10;
  GPIO_SET = 1<<25;
  status = 1;
  sleep(duty*full);
  GPIO_CLR = 1<<25;
  status = 0;
  sleep((1-duty)*full);
  return 0;
}

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
  /* open /dev/mem */
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    printf("can't open /dev/mem \n");
    exit(-1);
  }

  /* mmap GPIO */
  gpio_map = (char *)mmap(
			  NULL,             //Any adddress in our space will do
			  BLOCK_SIZE,       //Map length
			  PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
			  MAP_SHARED,       //Shared with other processes
			  mem_fd,           //File to map
			  GPIO_BASE         //Offset to GPIO peripheral
			  );
  close(mem_fd); //No need to keep mem_fd open after mmap


  if ((long)gpio_map < 0) {
    printf("mmap error %d\n", (int)gpio_map);
    exit(-1);
  }

  // Always use volatile pointer!
  gpio = (volatile unsigned *)gpio_map;
} // setup_io

