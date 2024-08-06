#include "pin.h"
#include "cell.h"
#include "netlist.h"

Pin::Pin( string pin_name, float x, float y, string wire_type) :
	pin_name(pin_name),
	x_pos(x),
	y_pos(y),
	rela_x_pos(x),
	rela_y_pos(y),
	x_pos_before(x),
	y_pos_before(y),
	timing_slack(0.0f),
	delay(0.0f),
	delay_before(0.0f)
{
	if (wire_type == "FG") {
		if (pin_name.front() == 'O' || pin_name.front() == 'Q') pin_type = 1;
		else if (pin_name.front() == 'I' || pin_name.front() == 'D') pin_type = 2;
		else pin_type = 3;
	}
	else if (wire_type == "I") {
		pin_type = 1;
	}
	else if (wire_type == "O") {
		pin_type = 2;
	}
	else {
		pin_type = 0;
		cerr << "wrong pin_wire_type_input" << endl;
	}
}

void Pin::changePos(float x, float y) {
	x_pos_before = x_pos;
	y_pos_before = y_pos;
	x_pos = x;
	y_pos = y;
	shared_ptr<Net> net_tmp = net_belong.lock();
	net_tmp->updateMovePin(shared_from_this());
}
void Pin::changePin(shared_ptr<Pin> after) {
	to_pin = after;
	after->from_pin = shared_from_this();
	shared_ptr<Net> net_tmp = net_belong.lock();
	net_tmp->updateChangePin(shared_from_this(), after->shared_from_this());
}

void Pin::changeDelay(float d)
{
	delay_before = delay;
	delay = d;
	shared_ptr<Net> net_tmp = net_belong.lock();
	net_tmp->updateDelayPin(shared_from_this());
}

void Pin::initialPos(float x, float y) {

	x_pos = rela_x_pos + x;
	y_pos = rela_y_pos + y;
}
int Pin::getType() {
	return pin_type;
}
float Pin::get_timing_slack() {
	return timing_slack;
}
float Pin::getDelay() {
	return delay;
}
void Pin::setNetBelong(shared_ptr<Net> n) {
	net_belong = n;
}
void Pin::setDelay(float d) {
	delay = d;
}
shared_ptr<Net> Pin::getNetBelong() {
	return net_belong.lock();
}
float Pin::get_pin_xpos_before() {
	return x_pos_before;
}
float Pin::get_pin_ypos_before() {
	return y_pos_before;
}
float Pin::get_pin_xpos() {
	return x_pos;
}
float Pin::get_pin_ypos() {
	return y_pos;
}

float Pin::getDelayBefore()
{
	return delay_before;
}
