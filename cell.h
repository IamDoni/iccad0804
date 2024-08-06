#pragma once
#include <stdio.h>
#include<iostream>
#include<string.h>
#include<vector>
#include<memory>
using namespace std;
#include"pin.h"
#include "Point.h"
#include "netlist.h"
#ifndef	_CELL_H_
#define _CELL_H_

class Cell : public std::enable_shared_from_this<Cell>
{
private:
	int bit;
	string ff_name;
	int ff_width, ff_height;
	int pin_count;

	float x_pos, y_pos;
	float x_pos_before, y_pos_before;
	Point pos;
	string inst_name;
	int q_pin_delay;
	float gate_power;

	int cluster_num;
	int p_right;//penalty
	int p_left;//penalty
	int p_up;//penalty
	int p_down;//penalty

	vector<shared_ptr<Pin>> ff_pin;
	vector<weak_ptr<Cell>> children;

public:
	Cell(int bit = 0, string ff_name = "initial", int ff_width = 0, int ff_height = 0, int pin_count = 0) :
		bit(bit), 
		ff_name(ff_name), 
		ff_width(ff_width), 
		ff_height(ff_height), 
		pin_count(pin_count), 
		cluster_num(-1), 
		x_pos(0), 
		y_pos(0), 
		x_pos_before(0),
		y_pos_before(0),
		q_pin_delay(0), 
		gate_power(0), 
		p_right(0), 
		p_left(0), 
		p_up(0), 
		p_down(0)
	{}

	Cell(const Cell& other) = default;
	Cell& operator=(const Cell& other) = default;
	Cell(Cell&& other) noexcept = default;
	Cell& operator=(Cell&& other) noexcept = default;
	

	////////set////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void set_bit(int bit) {
		this->bit = bit;
	}
	void set_inst_name(string temp) {
		inst_name = temp;
	}
	void set_ff_height(int temp) {
		ff_height = temp;
	}
	void set_ff_width(int temp) {
		ff_width = temp;
	}
	void set_pin(shared_ptr<Pin> temp) {
		ff_pin.emplace_back(temp);
	}
	

	void setPos(Point newPos)
	{
		pos = newPos;
	}
	void set_power(float temp) {
		gate_power = temp;
	}
	void set_children(const vector<shared_ptr<Cell>>& temp) {
		children.clear(); // 清空现有的子节点
		for (const auto& cell : temp) {
			children.push_back(cell); // 将 shared_ptr 作为 weak_ptr 存储
		}
	}

	void set_clusterNum(int num) {
		cluster_num = num;
	}
	void set_p_right(int penalty) {//penalty
		p_right = penalty;
	}
	void set_p_left(int penalty) {//penalty
		p_left = penalty;
	}
	void set_p_up(int penalty) {//penalty
		p_up = penalty;
	}
	void set_p_down(int penalty) {//penalty
		p_down = penalty;
	}
	void set_ff_name(string name) {
		ff_name = name;
	}
	////////get////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int get_bit() {
		return bit;
	}
	string get_ff_name() {
		return ff_name;
	}
	int get_ff_width() {
		return ff_width;
	}
	int get_ff_height() {
		return ff_height;
	}
	int get_pin_count() {
		return pin_count;
	}
	vector<shared_ptr<Pin>>& get_pin() {
		return ff_pin;
	}

	Point getPos() {
		return pos;
	}
	Point& get_pos_address() {
		return pos;
	}
	int get_clusterNum() {
		return cluster_num;
	}

	
	string get_inst_name() {
		return inst_name;
	}
	int get_q() {
		return q_pin_delay;
	}
	float get_power() {
		return gate_power;
	}
	float get_min_slack() {
		float min = FLT_MAX;
		for (auto& v : ff_pin) {
			if (v->get_pin_name().at(0) == 'D' && v->get_timing_slack() < min) {
				min = v->get_timing_slack();
			}
		}
		return min;
	}
	vector<shared_ptr<Cell>> get_children() const{
		vector<shared_ptr<Cell>> result;
		for (const auto& weak_cell : children) {
			if (auto shared_cell = weak_cell.lock()) { // 尝试获取 shared_ptr
				result.push_back(shared_cell);
			}
		}
		return result;
	}
	vector<weak_ptr<Cell>>& access_children() {
		return children;
	}
	int get_p_right() {//penalty
		return p_right;
	}
	int get_p_left() {//penalty
		return p_left;
	}
	int get_p_up() {//penalty
		return p_up;
	}
	int get_p_down() {//penalty
		return p_down;
	}
	//////////////////////////////////////////////////////////////////////////
	void change_pin(int index, shared_ptr<Pin> temp) {
		if (index < ff_pin.size()) {
			ff_pin[index] = temp;
		}
	}
	void initialize_pin() {
		ff_pin.clear();
		pin_count = 0;
	}
	
	void updatePos(float x,float y);
	void delayPropagate();
	void set_q(int temp);
	void update_q(float temp);
	void inherit_data(Cell standard_cell);
	void set_inst(string inst_name, float x_pos, float y_pos);
	float get_xpos();
	float get_ypos();
};


/*class Inst {
	string inst_name;
	string contain_ff_name;
	int x_pos, y_pos;
};*/
void InitialDebanking(vector<shared_ptr<Cell>>& FF, vector<Cell*>& best_st_table);

void show_stardard_FF(vector<Cell>& input);
void show_FF(vector<shared_ptr<Cell>>& input);
#endif 