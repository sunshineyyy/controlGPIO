/**
 * This file defines a node.js interface to the LED control module
 * 
 * @author Yaoyu Yang
 * @credits Araon
 * @date March 20 2013
 */

#define BUILDING_NODE_EXTENSION
#include <node.h>

#include<fcntl.h>
#include<string>

extern "C" {
#include "led.h"
}

using namespace v8;

Handle<Value> turnOn(const Arguments& args) {
  HandleScope scope;

  //Call C function
  turnOn();

  return scope.Close(Undefined());

}

Handle<Value> turnOff(const Arguments& args) {
  HandleScope scope;

  //Call C function
  turnOff();

  return scope.Close(Undefined());

}

Handle<Value> status(const Arguments& args) {
  HandleScope scope;

  //Call C function
  Local<Array> status =Array::New(1);
  status -> Set (0,Number::New(inquireStatus()));

  //return scope.Close(Undefined());
  
  return scope.Close(status);
}

Handle<Value> pwm(const Arguments& args) {
  HandleScope scope;

  //Call C function
  //Local<Array> status =Array::New(1);
  //status -> Set (0,Number::New(inquireStatus()));
  if (args.Length() != 2) {
    ThrowException(
		   Exception::TypeError(String::New("Wrong number of arguments")));

  return scope.Close(Undefined());
  }
  Local<Array> duty = Local<Array>::Cast(args[0]);
  Local<Array> full = Local<Array>::Cast(args[1]);
  double dty = duty ->Get(Integer::New(0))->NumberValue();
  double ful = full ->Get(Integer::New(0))->NumberValue();
  //Call pwm C function
  pwm(dty, ful);

  return scope.Close(Undefined());
}

void Init(Handle<Object> target) {

  target->Set(String::NewSymbol("turnOn"),
	      FunctionTemplate::New(turnOn)->GetFunction());

  target->Set(String::NewSymbol("turnOff"),
	      FunctionTemplate::New(turnOff)->GetFunction());

  target->Set(String::NewSymbol("status"),
	      FunctionTemplate::New(status)->GetFunction());

  target->Set(String::NewSymbol("pwm"),
	      FunctionTemplate::New(pwm)->GetFunction());

}


NODE_MODULE(led, Init)

