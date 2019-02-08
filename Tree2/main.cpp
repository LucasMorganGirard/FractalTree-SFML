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
    sf::RenderWindow window(sf::VideoMode(2500, 1800), "SFML window", sf::Style::None);
    
    //VARIABLES
    sf::Vector2f startingPoint(1800.0,900.0);//POINT DE DEPART DE LA RECURSIVE
    
    float longueur = 300;  //LONGUEUR DE LA PREMIERE BRANCHE EN PX LES BRANCHE
    //SONT DE LONGUEUR longueur/1.6 PAR RAPPORT A LA BRANCHE PRECEDENTE
    
    std::string str;
    
    bool alphaOn = false;

    float alpha = 0;       //ANGLE ECART ENTRE BRANCHE PERE ET BRANCHE FILLE
    
    float ecart = M_PI/8;        //ANGLE ECART ENTRE LES DEUX BRANCHE
    
    const float startingAngle = M_PI;
    
    bool autoMov=false;         //L'ECART ENTRE LES BRANCHE ET LA BRANCHE PERE AUGMENTE A CHAQUE FRAME TOUT SEUL
    
    bool autoEcart=false;       //L'ECART ENTRE LES DEUX BRANCHE AUGMENTE A CHAQUE FRAME TOUT SEUL
    
    bool weirdGrow=false;
    
    float augMovAuto = 0.01;    //SI autoMov = true alpha+=augMovAuto
    
    float augEcartAuto = 0.01;  //SI autoEcart = true beta+=augEcartAuto
    
    bool followMouse=false;     //ACTIVE LE SUIVIT DE LA SOURIS
    
    bool classic = false;
    
    int generationAffichage;
    
    bool grow = true;

    grow ? generationAffichage = 0 : generationAffichage = 25;
    
    std::vector<Branche> tree;
    
    sf::Vector2i origine(0,0);
    sf::CircleShape originePoint(20.f);

    
    //LIGNE DECORATIVE DE DEPART
    sf::Vertex startLigne[2];
    
    int option = 1;

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
            
            //OPTIONS
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                //ON/OFF DE L'AUGMENTATION AUTOMATIQUE (+1 par frame) DE L'ECART ALPHA
                //NE FONCTIONNE PAS AVEC -E (alpha fixe)
                autoMov=!autoMov;
                augMovAuto = 0.01;
                followMouse=false;
                alphaOn=autoMov;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                //ON/OFF DE L'AUGMENTATION AUTOMATIQUE (+1 par frame) DE L'ECART ALPHA
                //NE FONCTIONNE PAS AVEC -E (alpha fixe)
                //SENS CONTRAIRE A UP
                autoMov=!autoMov;
                augMovAuto = -0.01;
                followMouse=false;
                alphaOn=autoMov;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                //ON/OFF DU L'AUGMENTATION AUTOMATIQUE (+1 par frame) DE L'ECART ENTRE DEUX BRANCHE
                autoEcart=!autoEcart;
                augEcartAuto = 0.01;
                followMouse=false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                //ON/OFF DU L'AUGMENTATION AUTOMATIQUE (+1 par frame) DE L'ECART ENTRE DEUX BRANCHE
                //SENS CONTRAIRE A RIGHT
                autoEcart=!autoEcart;
                augEcartAuto = -0.01;
                followMouse=false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                //CONTROLE ALPHA ET ECART GRACE A LA SOURIS X ET Y
                followMouse=!followMouse;
                autoMov=false;
                autoEcart=false;
                if(followMouse){
                    origine = sf::Mouse::getPosition(window);
                    originePoint.setPosition(origine.x-20, origine.y-20);
                }
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {//
                alphaOn=!alphaOn;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {//
                option=2;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {//NULL
                grow = !grow;
                weirdGrow=false;
                if (grow) {
                    generationAffichage = 0;
                    pourcentAffichage=1;
                }else{
                    generationAffichage = 25;
                }
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
                weirdGrow=!weirdGrow;
                grow=false;
                if (weirdGrow) {
                    generationAffichage = 0;
                    pourcentAffichage=1;
                }else{
                    generationAffichage = 25;
                }
            }
            
        }
        
        str = std::to_string(option);
        //AFFICHE ALPHA ET BETA SUR L'ECRAN
        if(option==1){
            str = str+" ";
        }else if(option == 2){
            str = str+" ";
        }
        text.setString(" esc pour quitter, Alpha (non implementer) : " + std::to_string(fmod(alpha,2*M_PI)) + " ecart : " + std::to_string(fmod(ecart,2*M_PI))+"\n" + " option " + str);
        
        window.clear();
        
        
        if(autoMov){//AUGMENTATION DE ALPHA AUTO
            alpha+=augMovAuto;
        }
        
        
        if(!alphaOn){
            if (alpha>0.01){
                alpha-=0.002;
            }else if (alpha<-0.01){
                alpha+=0.002;
            }else{
                alpha=0;
            }
        }
            
        if(autoEcart){//AUGMENTATION DE BETA AUTO
            ecart+=augEcartAuto;
        }
        
        if(followMouse){
            //ALPHA(X) ET BETA(Y) EVOLUE EN FONCTION DE LA SOURIS
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            alpha += 0.1*(origine.y - localPosition.y)/1800;
            ecart += 0.1*(origine.x - localPosition.x)/2500;
        }
        
        fmod(alpha, 2*M_PI);
        fmod(ecart, 2*M_PI);

        if(grow || weirdGrow){
            if(pourcentAffichage < 100){
                pourcentAffichage+=2;
            }else if (pourcentAffichage >= 100){
                if(generationAffichage < 25){
                    generationAffichage++;
                    pourcentAffichage=2;
                }else{
                    grow=false;
                    weirdGrow=false;
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
            if(weirdGrow){
                treeLignes[current++].position = b.getStartingPoint();
                treeLignes[current++].position = b.pointGauche(pourcentAffichage, classic);
                treeLignes[current++].position = b.getStartingPoint();
                treeLignes[current++].position = b.pointDroit(pourcentAffichage, classic);
                generationAffichage = 25;
            }else{
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
        }
        
        window.draw(fond);
        window.draw(treeLignes);
        window.draw(startLigne, 2, sf::Lines);
        window.draw(text);
        if(followMouse){
            window.draw(originePoint);
        }
        window.display();
        firstIteration=false;
    }
    
    return EXIT_SUCCESS;
}
