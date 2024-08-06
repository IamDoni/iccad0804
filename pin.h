#pragma once
#include <stdio.h>
#include<iostream>
#include<string.h>
#include<vector>
#include<memory>
using namespace std;
//#include "cell.h"
#ifndef	_PIN_H_
#define _PIN_H_
class Cell;
class Net;
class Pin : public std::enable_shared_from_this<Pin>
{
private:
	string pin_name;
	int pin_type;//for a wire 1 is the pin input data into the wire and 2 is the output and 3 is clk
	float x_pos, y_pos;
	float x_pos_before, y_pos_before;
	float rela_x_pos, rela_y_pos;
	float timing_slack;
	float delay;
	float delay_before;

	weak_ptr<Cell> belong;
	weak_ptr<Net> clk_net;
	weak_ptr<Net> net_belong;		

	weak_ptr<Pin> from_pin;
	shared_ptr<Pin> to_pin;

public:

	Pin(string pin_name, float x, float y, string wire_type);
	Pin(const Pin& other) = default;
	Pin& operator=(const Pin& other) = default;
	Pin(Pin&& other) noexcept = default;
	Pin& operator=(Pin&& other) noexcept = default;


	////////set////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void set_timing_slack(float temp) {
		timing_slack = temp;
	}
	
	void set_belong(shared_ptr<Cell> cell) {
		belong = cell; // 赋值 weak_ptr
	}
	void set_clk_net(shared_ptr<Net> temp) {
		clk_net = temp;
	}
	////////get////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	string get_pin_name() {
		return pin_name;
	}
	
	shared_ptr<Cell> get_belong() {
		return belong.lock(); // 获取强引用，如果 Cell 已被销毁，返回空的 shared_ptr
	}
	shared_ptr<Net> get_clk_net() {
		return clk_net.lock();
	}


	void setDelay(float d);
	void setNetBelong(shared_ptr<Net> n);
	float getDelay();
	float get_timing_slack();
	int getType();
	void initialPos(float x, float y);
	void changePos(float x, float y);
	void changePin(shared_ptr<Pin> after);
	void changeDelay(float d);
	shared_ptr<Net> getNetBelong();
	float get_pin_xpos_before();
	float get_pin_ypos_before();
	float get_pin_xpos();
	float get_pin_ypos();
	float getDelayBefore();
};

#endif 


