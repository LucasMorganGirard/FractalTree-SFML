/**
     Fractral arbre 6 fevrier 2019
     LUCAS GIRARD : Universite de Sciences et Techniques de Nantes L2 informatique
     FRAMEWORK SFML pour c++
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include "branche.hpp"

//Fournis par le framwork SFML
#include "ResourcePath.hpp"

int main(int, char const**)
{
    //STYLE NONE PAS DE BORDURE PAS DE RESIZE PAS DE CLOSE
    sf::RenderWindow window(sf::VideoMode(2400, 1600), "SFML window", sf::Style::None);
    
    //VARIABLES
    sf::Vector2f startingPoint(2000.0,900.0);//POINT DE DEPART DE LA RECURSIVE
    
    float longueur = 300;  //LONGUEUR DE LA PREMIERE BRANCHE EN PX LES BRANCHE
    //SONT DE LONGUEUR longueur/1.6 PAR RAPPORT A LA BRANCHE PRECEDENTE
    
    float alpha = 0;       //ANGLE ECART ENTRE BRANCHE PERE ET BRANCHE FILLE
    
    float ecart = M_PI/8;        //ANGLE ECART ENTRE LES DEUX BRANCHE
    
    const float startingAngle = M_PI;
    
    bool followMouse=false;     //ACTIVE LE SUIVIT DE LA SOURIS
    
    bool classic = false;
    
    int generationAffichage;
    
    bool grow = true;

    grow ? generationAffichage = 0 : generationAffichage = 16;
    
    std::vector<Branche> tree;
    
    sf::Vector2i origine(0,0);
    sf::CircleShape originePoint(20.f);

    //LIGNE DECORATIVE DE DEPART
    sf::Vertex startLigne[2];
    
    //-------------------
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    //TAILLE
    unsigned int textSize=50;
    sf::Text text("", font, textSize);
    //TEXTE BLANC POUR VOIR SUR FOND NOIR
    text.setFillColor(sf::Color::White);
    
    sf::Texture img;
    if(!img.loadFromFile(resourcePath() + "fond.jpg")){
        return EXIT_FAILURE;
    }
    sf::Sprite fond(img);
    
    window.setFramerateLimit(30);
    //-------------------
    sf::VertexArray treeLignes;

    Branche* start = new Branche(startingPoint, startingAngle, ecart, longueur, alpha, tree, 0);
    
    int current;
    
    float pourcentAffichage = 1;
    
    bool firstIteration=true;
    
    while (window.isOpen())
    {
  
        sf::Event event;
        while (window.pollEvent(event))
        {
            //FERMETURE DE LA FENETRE SI REQUETE DE FERMETURE
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            //IDEM
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                //CONTROLE ALPHA ET ECART GRACE A LA SOURIS X ET Y
                followMouse=!followMouse;
                if(followMouse){
                    origine = sf::Mouse::getPosition(window);
                    originePoint.setPosition(origine.x-20, origine.y-20);
                }else{
                    firstIteration=true;
                }
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {//
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {//
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {//NULL
                grow = !grow;
                if (grow) {
                    generationAffichage = 0;
                    pourcentAffichage=1;
                }else{
                    generationAffichage = 16;
                    firstIteration=true;
                }
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {

            }
            
        }
        
        text.setString(" esc pour quitter, Alpha (non implementer) : " + std::to_string(fmod(alpha,2*M_PI)) + " ecart : " + std::to_string(fmod(ecart,2*M_PI)));
        
        if(firstIteration || grow || followMouse || generationAffichage < 16){
            std::cout << firstIteration << " " << grow << " " << followMouse << " " << (generationAffichage < 16) << std::endl;
            window.clear();
            
            if(followMouse){
                //ALPHA(X) ET BETA(Y) EVOLUE EN FONCTION DE LA SOURIS
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                alpha += 0.01*(origine.y - localPosition.y)/1600;
                ecart += 0.01*(origine.x - localPosition.x)/1600;
            }
            
            alpha=fmod(alpha, 2*M_PI);
            ecart=fmod(ecart, 2*M_PI);

            if(grow){
                if(pourcentAffichage < 100){
                    pourcentAffichage+=2;
                }else if (pourcentAffichage >= 100){
                    if(generationAffichage < 16){
                        generationAffichage++;
                        pourcentAffichage=2;
                    }else{
                        std::cout << "here" << std::endl;
                        grow=false;
                    }
                }
            }else{
                pourcentAffichage=100;
            }
            
            //CREATION ET DRAW DES LIGNES AVEC LES PARAMETRE MIS A JOURS

            tree.clear();
            delete start;
            start = new Branche(startingPoint, startingAngle, ecart, longueur/1.6, alpha, tree, 1);
            (*start).createTree(classic);
            
            if(generationAffichage == 0){
                startLigne[0] = sf::Vertex(sf::Vector2f(startingPoint.x + longueur,startingPoint.y));
                startLigne[1] = sf::Vertex(sf::Vector2f(startingPoint.x + longueur - longueur*(pourcentAffichage/100), startingPoint.y));
            }else{
                startLigne[0] = sf::Vertex(sf::Vector2f(startingPoint.x,startingPoint.y));
                startLigne[1] = sf::Vertex(sf::Vector2f(startingPoint.x+longueur,startingPoint.y));
            }
            
            treeLignes = sf::VertexArray(sf::VertexArray(sf::Lines, 4*tree.size()));
            current=0;
            for (Branche b : tree) {
                    if (b.getGeneration() < generationAffichage) {
                        treeLignes[current++].position = b.getStartingPoint();
                        treeLignes[current++].position = b.pointGauche(100.f, classic);
                        treeLignes[current++].position = b.getStartingPoint();
                        treeLignes[current++].position = b.pointDroit(100.f, classic);
                    }else if (b.getGeneration() == generationAffichage){
                        treeLignes[current++].position = b.getStartingPoint();
                        treeLignes[current++].position = b.pointGauche(pourcentAffichage, classic);
                        treeLignes[current++].position = b.getStartingPoint();
                        treeLignes[current++].position = b.pointDroit(pourcentAffichage, classic);
                    }
            }
            
            if(followMouse){
                window.draw(originePoint);
            }
            firstIteration=false;
            
        }
        tree.clear();
        window.draw(fond);
        window.draw(treeLignes);
        window.draw(startLigne, 2, sf::Lines);
        window.draw(text);
        window.display();
        
    }
    
    return EXIT_SUCCESS;
}
