/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.h
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:29 PM
 */

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>

void error(std::string msg);

/**
 * Computes in which percentile the value (roughly) is. The result is rounded to the nearest unit.
 * 
 * @param double Value for which the percentile is calculated
 * @param std::map<double, std::string>& Data used to determine which percentile the first parametr is in
 * 
 * @return double Number between 0 and 100 (inclusive 0 and 100)
 */
double get_percentile(double value, std::map<double, std::string>& population);

#endif /* SERVER_H */

