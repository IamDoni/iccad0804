#include <stdio.h>
#include<iostream>
#include<string.h>
#include<vector>
#include <fstream>
#include <sstream>
using namespace std;
#include "cell.h"



void InitialDebanking(vector<shared_ptr<Cell>>& FF, vector<Cell*>& best_st_table) {
	// �ˬd best_st_table �O�_����
	if (best_st_table.empty()) {
		cerr << "Error: best_st_table is empty!" << endl;
		return;
	}

	// �ǳƦs�x�ݭn���J�� Cell
	vector<shared_ptr<Cell>> newCells;

	for (int i = 0; i < FF.size(); ++i) {
		cout << "i=" << i << endl;
		if (FF[i]->get_bit() > 1) {
			cout << "start to debanking" << endl;
			// ������e��m
			Point currentPos = FF[i]->getPos();
			// �ƻs�M���]��m
			Cell original_FF = *FF[i];
			//Cell test = FF[i];
			shared_ptr<Pin> original_clk_pin = make_shared<Pin>("CLK", 0, 0, "FG");
			//shared_ptr<Net> original_clk_net
			//cout << "origianl_FF : " << original_FF.get_inst_name() << endl;

			for (auto& v : original_FF.get_pin()) {
				cout << "\tpin_name : " << v->get_pin_name() << endl;
				cout << v->get_belong()->get_inst_name() << endl;
				if (v->get_pin_name() == "CLK") {
					original_clk_pin = v;
					
				}
			}

			for (int j = 0; j < original_FF.get_bit(); ++j) {
				if (j == 0) {
					FF[i]->get_pin().clear();
					FF[i]->inherit_data(*best_st_table.at(0));
					//cout << "copy_cell's pin count : " << FF[i]->get_pin().size() << endl;
					for (auto& v : FF[i]->get_pin()) {
						if (&v == nullptr) {
							cout << "it is null\n";
						}
						//cout << "\tpin_name : " << v->get_pin_name() << endl;
						v->set_belong(FF[i]);
					}
					FF[i]->setPos(currentPos);
					//cout << "currentPos=" << currentPos.access_Values().at(0) << "," << currentPos.access_Values().at(1) << endl;
					FF[i]->set_inst(original_FF.get_inst_name() + "'" + to_string(j), currentPos.access_Values().at(0), currentPos.access_Values().at(1));
					FF[i]->get_pin().at(0)->set_timing_slack(original_FF.get_pin().at(j)->get_timing_slack());
					//original_clk_pin->get_clk_net()->add_pin((FF[i]->get_pin().at(2)));
				}
				else {
					shared_ptr<Cell> copy_cell = make_shared<Cell>(0, "initial", 0, 0, 0);
					copy_cell->inherit_data(*best_st_table.at(0));

					//cout << "copy_cell's pin count : " << copy_cell->get_pin().size() << endl;
					for (auto& v : copy_cell->get_pin()) {
						if (&v == nullptr) {
							cout << "it is null\n";
						}
						//cout << "\tpin_name : " << v->get_pin_name() << endl;
						v->set_belong(copy_cell);
					}


					copy_cell->setPos(currentPos);
					//cout << "currentPos=" << currentPos.access_Values().at(0) << "," << currentPos.access_Values().at(1) << endl;
					copy_cell->set_inst(original_FF.get_inst_name() + "'" + to_string(j), currentPos.access_Values().at(0), currentPos.access_Values().at(1));
					copy_cell->get_pin().at(0)->set_timing_slack(original_FF.get_pin().at(j)->get_timing_slack());

					cout << "Net : " << original_clk_pin->get_clk_net()->get_net_name() << "adding pin from " << copy_cell->get_inst_name() << endl;
					original_clk_pin->get_clk_net()->add_pin((copy_cell->get_pin().at(2)));
					newCells.emplace_back(copy_cell); // �K�[��s�V�q
				}
				
				
			}
		}
	}

	// �b�M��������@���ʴ��J�s����
	FF.reserve(FF.size() + newCells.size());
	FF.insert(FF.end(), newCells.begin(), newCells.end());

}



void show_stardard_FF(vector<Cell>& input) {
	ofstream outFile("show_stardard_FF.txt");

	if (!outFile.is_open()) {
		std::cerr << "�L�k���}�ɮ�" << std::endl;
		return;
	}
	for (auto temp : input) {
		//outFile << "For standard FF" << endl;
		outFile << temp.get_ff_name() << ":" << endl;
		outFile << "\tbit:\t" << temp.get_bit() << endl;
		outFile << "\theight:\t" << temp.get_ff_height() << "\twidth\t" << temp.get_ff_width() << endl;
		outFile << "\tpincout:\t" << temp.get_pin_count() << endl;
		for (auto v : temp.get_pin()) {
			outFile << "\t\tpin:\t" << v->get_pin_name() << ":(" << v->get_pin_xpos() << "," << v->get_pin_ypos() << ")" << endl;
		}
		outFile << "\tQpinDelay:\t" << temp.get_q() << endl;
		outFile << "\tgate_power:\t" << temp.get_power() << endl;
	}

}
void show_FF(vector<shared_ptr<Cell>>& input) {
	ofstream outFile("show_FF.txt");
	if (!outFile.is_open()) {
		std::cerr << "�L�k���}�ɮ�" << std::endl;
		return;
	}
	for (auto temp : input) {
		//outFile << "For inst FF" << endl;
		outFile << temp->get_inst_name() << ":" << endl;
		outFile << "\tmodel:\t" << temp->get_ff_name() << endl;
		outFile << "\tbit:\t" << temp->get_bit() << endl;
		outFile << "\tx_pox\t" << temp->get_xpos()<<" -> "<<temp->getPos().get_xpos()<< "\ty_pos\t" << temp->get_ypos() << " -> " << temp->getPos().get_ypos() << endl;
		outFile << "\theight:\t" << temp->get_ff_height() << "\twidth\t" << temp->get_ff_width() << endl;
		outFile << "\tpincout:\t" << temp->get_pin_count() << endl;
		for (auto v : temp->get_pin()) {
			outFile << "\t\tpin:\t" << v->get_pin_name() << ":(" << v->get_pin_xpos() << "," << v->get_pin_ypos() << ")\tTimingSlack=" << v->get_timing_slack() << endl;
		}
		outFile << "\tQpinDelay:\t" << temp->get_q() << endl;
		outFile << "\tgate_power:\t" << temp->get_power() << endl;
	}
}

void Cell::set_inst(string inst_name, float x_pos, float y_pos) {
	this->inst_name = inst_name;
	this->pos = { x_pos,y_pos };
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	for (auto& pin : ff_pin) {
		pin->initialPos(x_pos, y_pos);
		pin->set_belong(shared_from_this());
	}
}

void Cell::inherit_data(Cell standard_cell) {
	bit = standard_cell.get_bit();
	ff_name = standard_cell.get_ff_name();
	ff_width = standard_cell.get_ff_width();
	ff_height = standard_cell.get_ff_height();
	pin_count = standard_cell.get_pin_count();
	for (int i = 0; i < pin_count; ++i) {
		auto temp_pin = make_shared<Pin>(standard_cell.get_pin().at(i)->get_pin_name(), standard_cell.get_pin().at(i)->get_pin_xpos(), standard_cell.get_pin().at(i)->get_pin_ypos(),"FG");
		ff_pin.emplace_back(temp_pin);
	}
	q_pin_delay = standard_cell.get_q();
	gate_power = standard_cell.get_power();
}

void Cell::set_q(int temp) {
	q_pin_delay = temp;
	for (auto& pin : ff_pin) {
		if (pin->getType() == 1) {
			pin->setDelay(temp);
		}
	}
}

void Cell::update_q(float temp) {
	q_pin_delay = temp;
	for (auto& pin : ff_pin) {
		if (pin->getType() == 1) {
			pin->changeDelay(temp);
		}
	}
}
void Cell::updatePos(float x, float y)
{
	x_pos_before = x_pos;
	y_pos_before = y_pos;
	x_pos = x;
	y_pos = y;
	for (auto& pin : ff_pin) {
		pin->changePos(x, y);
	}
}
void Cell::delayPropagate() {
	float min_slack = FLT_MAX;
	for (auto& p : ff_pin) {
		if (p->getType() == 2) {
			if (min_slack > p->get_timing_slack()) min_slack = p->get_timing_slack();
		}
	}
	update_q(-min_slack);
}
float Cell::get_xpos() {
	return x_pos;
}
float Cell::get_ypos() {
	return y_pos;
}