//
//  branche.hpp
//  Tree2
//
//  Created by GIRARD Lucas on 07/02/2019.
//  Copyright © 2019 GIRARD Lucas. All rights reserved.
//

#ifndef branche_hpp
#define branche_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

class Branche{
public:
    Branche(const sf::Vector2f, const float, const float&, const float, const float&, std::vector<Branche>&, const unsigned);
    
     const sf::Vector2f pointGauche(const float, const bool) const;
     const sf::Vector2f pointDroit(const float, const bool) const;
     const sf::Vector2f getStartingPoint() const;
     const int getGeneration() const;
     const float getLongueur() const;
     void createTree(const bool);
     const int rando(const int) const;
    
private:
    //Attributs
    std::vector<Branche>& mTree;
    bool mGActive;
    bool mDActive;
    const sf::Vector2f mStartingPoint;
    const float& mEcart;
    const float mLongueur;
    const float mStartingAngle;
    const float& mAlpha;
    const unsigned mNbParents;
};

#endif /* branche_hpp */
