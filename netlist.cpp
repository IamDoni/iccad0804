#include <stdio.h>
#include<iostream>
#include<string.h>
#include<vector>
#include <fstream>
#include <sstream>
using namespace std;
#include "netlist.h"
#include "cell.h"
void show_netlist(NetList temp) {
	ofstream outFile("show_netlist.txt");

	if (!outFile.is_open()) {
		std::cerr << "無法打開檔案" << std::endl;
		return;
	}
	outFile << "NetList have:\t" << temp.get_net_count() << endl;
	for (auto v : temp.get_contain_net()) {
		outFile << "\t" << v->get_net_name() << ":\t";
		for (auto u : v->get_connect_pin()) {
			if (u->get_belong() != NULL) {
				outFile << u->get_belong()->get_inst_name() << "/";
			}
			outFile << u->get_pin_name() << "  ";
		}
		outFile << endl;
	}
}

void Net::updateMovePin(shared_ptr<Pin> after) {
		if (after->getType() == 1) {
			for (int i = 0; i < connect_pin.size(); i++) {
				if (connect_pin[i]->getType() == 2) {
					float delay_diff = 
						displacement_delay * (fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos())
							- (fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos_before()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos_before())));
					if (connect_pin[i]->get_belong()->get_ff_name().front() == 'G') {
						connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
						connect_pin[i]->get_belong()->delayPropagate();
					}
					else if (connect_pin[i]->get_belong()->get_ff_name().front() == 'F') {
						connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
					}
				}
			}
		}
		else if (after->getType() == 2) {
			for (int i = 0; i < connect_pin.size(); i++) {
				if (connect_pin[i]->getType() == 1) {
					float delay_diff =
						displacement_delay * ((fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos()))
							- (fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos_before()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos_before())));
					if (after->get_belong()->get_ff_name().front() == 'G') {
						after->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
						after->get_belong()->delayPropagate();
					}
					else if (after->get_belong()->get_ff_name().front() == 'F') {
						after->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
					}
				}
			}
		}
}

void Net::updateChangePin(shared_ptr<Pin> before, shared_ptr<Pin> after)
{
	if (after->getType() == 1) {
		for (int i = 0; i < connect_pin.size(); i++) {
			if (connect_pin[i]->getType() == 2) {
				float delay_diff = after->getDelay() - before->getDelay() +
					displacement_delay * (fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos())
						- (fabs(connect_pin[i]->get_pin_xpos() - before->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - before->get_pin_ypos())));
				if (connect_pin[i]->get_belong()->get_ff_name().front() == 'G') {
					connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
					connect_pin[i]->get_belong()->delayPropagate();
				}
				else if (connect_pin[i]->get_belong()->get_ff_name().front() == 'F') {
					connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
				}
			}
		}
	}
	else if (after->getType() == 2) {
		for (int i = 0; i < connect_pin.size(); i++) {
			if (connect_pin[i]->getType() == 1) {
				float delay_diff =
					displacement_delay * ((fabs(connect_pin[i]->get_pin_xpos() - after->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - after->get_pin_ypos()))
						- (fabs(connect_pin[i]->get_pin_xpos() - before->get_pin_xpos()) + fabs(connect_pin[i]->get_pin_ypos() - before->get_pin_ypos())));
				if (after->get_belong()->get_ff_name().front() == 'G') {
					after->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
					after->get_belong()->delayPropagate();
				}
				else if (after->get_belong()->get_ff_name().front() == 'F') {
					after->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
				}
			}
		}
	}
	for (int i = 0; i < connect_pin.size(); i++) {
		if (connect_pin[i] == before) {
			connect_pin[i] = after;
		}
	}
}

void Net::updateDelayPin(shared_ptr<Pin> after)
{
	if (after->getType() == 1) {
		for (int i = 0; i < connect_pin.size(); i++) {
			if (connect_pin[i]->getType() == 2) {
				float delay_diff = after->getDelay() - after->getDelayBefore();
				if (connect_pin[i]->get_belong()->get_ff_name().front() == 'G') {
					connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
					connect_pin[i]->get_belong()->delayPropagate();
				}
				else if (connect_pin[i]->get_belong()->get_ff_name().front() == 'F') {
					connect_pin[i]->set_timing_slack(connect_pin[i]->get_timing_slack() - delay_diff);
				}
			}
		}
	}
}
