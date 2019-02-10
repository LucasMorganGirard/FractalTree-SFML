//
//  branche.cpp
//  Tree2
//
//  Created by GIRARD Lucas on 07/02/2019.
//  Copyright © 2019 GIRARD Lucas. All rights reserved.
//

#include <iostream>
#include "branche.hpp"
#include <math.h>
#include <cmath>
#include <cstdlib>//rand()

Branche::Branche(const sf::Vector2f startingPoint, const float startingAngle,const float& ecart,const float longueur, const float& alpha , std::vector<Branche>& tree, const unsigned nbParents)
:mStartingPoint(startingPoint),mStartingAngle(startingAngle),mEcart(ecart), mLongueur(longueur), mAlpha(alpha), mTree(tree), mNbParents(nbParents), mDActive(true), mGActive(true){
};

 const sf::Vector2f Branche::pointGauche(const float pourcentage, const bool classic) const{
        return sf::Vector2f(mStartingPoint.x + cos(mStartingAngle + mEcart) * (mLongueur * (pourcentage/100)), mStartingPoint.y + sin(mStartingAngle + mEcart) * (mLongueur * (pourcentage/100)));
};

 const sf::Vector2f Branche::pointDroit(const float pourcentage, const bool classic) const{
        return sf::Vector2f(mStartingPoint.x + cos(mStartingAngle - mEcart) * (mLongueur * (pourcentage/100)), mStartingPoint.y + sin(mStartingAngle - mEcart) * (mLongueur * (pourcentage/100)));
};

 const sf::Vector2f Branche::getStartingPoint() const{
    return mStartingPoint;
};

const float Branche::getLongueur() const{
    return mLongueur;
};

 const int Branche::getGeneration() const{
    return mNbParents;
 };

const int Branche::rando(const int add) const{
    srand(round(mStartingAngle)+round(mStartingPoint.x)+ round(mStartingPoint.y) +getGeneration() + add);
    return rand();
}

void Branche::createTree(const bool classic){
    
    mTree.push_back(*this);
    
    mGActive=(16-float(mNbParents))/16 + (fmod(rand(),500))/100 > 1;
    mDActive=(16-float(mNbParents))/16 + (fmod(rand(),500))/100 > 1;
    
    if(int(mLongueur/1.6) > 2){
        if(classic){
            
            Branche Gauche((*this).pointGauche(100.f, classic), mStartingAngle + mEcart + mAlpha, mEcart, mLongueur/1.6, mAlpha, mTree, mNbParents + 1);
            
            Branche Droite((*this).pointDroit(100.f, classic), mStartingAngle - mEcart + mAlpha, mEcart, mLongueur/1.6, mAlpha, mTree, mNbParents + 1);
            
            Gauche.createTree(classic);
            Droite.createTree(classic);
            
        }else{
            if(mGActive){
                Branche Gauche((*this).pointGauche(100.f, classic), mStartingAngle + mEcart + mAlpha +((fmod(rando(0),50))/100)*mEcart - ((fmod(rando(0),50))/100)*mEcart, mEcart, ((fmod(rando(7),30))/100)*mLongueur+mLongueur/1.6, mAlpha, mTree, mNbParents + 1);
                Gauche.createTree(classic);
            }
            if(mDActive){
                Branche Droite((*this).pointDroit(100.f, classic), mStartingAngle - mEcart + mAlpha +((fmod(rando(0),50))/100)*mEcart - ((fmod(rando(0),50))/100)*mEcart, mEcart, ((fmod(rando(4),30))/100)*mLongueur+mLongueur/1.6, mAlpha, mTree, mNbParents + 1);
                Droite.createTree(classic);
            }
        }
    }
};
