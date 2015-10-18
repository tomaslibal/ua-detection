/* 
 * File:   NaiveBayesPredictor.h
 * Author: tomaslibal
 *
 * Created on 18 October 2015, 12:41
 */

#ifndef NAIVEBAYESPREDICTOR_H
#define	NAIVEBAYESPREDICTOR_H

#include "../training/NaiveBayesTraining.h"

class NaiveBayesPredictor {
public:
    NaiveBayesPredictor();
    NaiveBayesPredictor(const NaiveBayesPredictor& orig);
    virtual ~NaiveBayesPredictor();
    
    void load_training(NaiveBayesTraining *nbt);
    
    void evaluate(const char* sentence, char* prediction);
private:

};

#endif	/* NAIVEBAYESPREDICTOR_H */

