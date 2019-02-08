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

Branche::Branche(const sf::Vector2f startingPoint, const float startingAngle,const float& ecart,const float longeur, const float& alpha , std::vector<Branche>& tree)
:mStartingPoint(startingPoint),mStartingAngle(startingAngle),mEcart(ecart),mLongueur(longeur), mAlpha(alpha),mTree(tree){};

 const sf::Vector2f Branche::pointGauche(const float pourcentage) const{
    return sf::Vector2f(mStartingPoint.x + cos(mStartingAngle + mEcart) * (mLongueur * (pourcentage/100)), mStartingPoint.y + sin(mStartingAngle + mEcart) * (mLongueur * (pourcentage/100)));
};

 const sf::Vector2f Branche::pointDroit(const float pourcentage) const{
    return sf::Vector2f(mStartingPoint.x + cos(mStartingAngle - mEcart) * (mLongueur * (pourcentage/100)), mStartingPoint.y + sin(mStartingAngle - mEcart) * (mLongueur * (pourcentage/100)));
};

 const sf::Vector2f Branche::getStartingPoint() const{
    return mStartingPoint;
};

const float Branche::getLongueur() const{
    return mLongueur;
};

 const int Branche::getGeneration(const float startingLongueur) const{
    return 1 + (int)round((log(startingLongueur)-log(mLongueur))/log(1.6));
};

void Branche::createTree(){
    mTree.push_back(*this);
    if(int(mLongueur/1.6) > 2){
        Branche Gauche((*this).pointGauche(100.f), mStartingAngle + mEcart + mAlpha, mEcart, mLongueur/1.6, mAlpha, mTree);
        Branche Droite((*this).pointDroit(100.f), mStartingAngle - mEcart + mAlpha, mEcart, mLongueur/1.6, mAlpha, mTree);
        Gauche.createTree();
        Droite.createTree();
    }
};
