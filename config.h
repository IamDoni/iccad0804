#pragma once
#ifndef	_CONFIG_H_
#define _CONFIG_H_

class Cell;

extern int alpha, beta, Gamma, delta;
extern int bin_width, bin_height, bin_max_util;
extern int row_start_x, row_start_y, row_width, row_height;
extern float displacement_delay;
extern vector<Cell> standard_FF;

#endif