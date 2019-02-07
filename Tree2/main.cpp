/**
 Fractral arbre 6 fevrier 2019
 LUCAS GIRARD : Universite de Sciences et Techniques de Nantes L2 informatique
 FRAMEWORK SFML pour c++
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>

//Fournis par le framwork SFML
#include "ResourcePath.hpp"

//STOQUAGE DE TROIS SET(x,y) DE POINT POUR CREE UNE BRANCHE=(LIGNE) ENTRE : PERE - FILLE GAUCHE, PERE - FILLE DROITE
//DANS LE RESTE DU PROGRAMME
struct branche3 {
    branche3(sf::Vector2f par, sf::Vector2f filleG, sf::Vector2f filleD):
    parent(par),filleGauche(filleG),filleDroite(filleD){}
    
    sf::Vector2f parent;
    sf::Vector2f filleGauche;
    sf::Vector2f filleDroite;
};

typedef std::vector<branche3> dynamicTab;

void const afficheBranche(branche3 b){//AFFICHAGE D'UNE BRANCHE const CAR PAS DE MODIF DE DONNE
    std::cout << "x parent : " << b.parent.x << std::endl;
    std::cout << "y parent : " << b.parent.y << std::endl;
    std::cout << "x droite : " << b.filleDroite.x << std::endl;
    std::cout << "y droite : " << b.filleDroite.y << std::endl;
    std::cout << "x gauche : " << b.filleGauche.x << std::endl;
    std::cout << "y gauche : " << b.filleGauche.y << std::endl;
    std::cout << "AAA" << std::endl;
}

void theTree(dynamicTab & v, const sf::Vector2f start, const float lon, const float startingAngle, float alpha, float ecart, int pixelStop, const int option){
    
    //SI LA LONGUEUR DE LA BRANCHE A CALCULER (lon) EST PLUS ELEVER QUE pixelStop
    if(lon > pixelStop){
        
        //SET DES ANGLES FILLE DROITE ET GAUCHE AUTOUR DU STARTING POINT (±ecart)
        float angleFilleG = startingAngle + ecart;
        float angleFilleD = startingAngle - ecart;
        
        //ON OBTIENS LES COORDONNES DES POINTS FILLE GAUCHE ET DROITE GRACE AU COSE ET AU SIN DES ANGLES CALCULER PRECEDEMENT
        sf::Vector2f filleG(start.x + cos(angleFilleG) * lon, start.y + sin(angleFilleG) * lon);
        sf::Vector2f filleD(start.x + cos(angleFilleD) * lon, start.y + sin(angleFilleD) * lon);
        //ON CREE LA BRANCHE CORRESPONDANT AVEC LE PERE ET LES FILLES
        branche3 * b = new branche3(start,filleG,filleD);
        //ON AJOUTE LA BRANCHE AU VECTOR DE BRANCHE
        v.push_back(*b);//A GARDER COMME CA MOINS DE FUITE MEMOIRE QUE v.push_back(*new branche3(start,filleG,filleD); QUAND DELETE MANUELLEMENT
        delete b;//ON LIBERE LA MEMOIRE
        if(option == 1){//arbre libre
            //RECURSION ON MODIFIE LE STARTING POINT POUR AVOIR UN PETIT ECART AVEC PAR RAPPORT A LA BRANCHE PRECEDENTE
            theTree(v,filleG, lon/1.6, startingAngle+alpha, alpha, ecart, pixelStop, option);
            theTree(v,filleD, lon/1.6, startingAngle-alpha, alpha, ecart, pixelStop, option);
        }else if(option == 2){//arbre proportionnelle (plus parfait)
            theTree(v,filleG, lon/1.6, startingAngle+ecart, alpha, ecart, pixelStop, option);
            theTree(v,filleD, lon/1.6, startingAngle-ecart, alpha, ecart, pixelStop, option);
        }else if(option == 3){
            
        }else{//par default
            theTree(v,filleG, lon/1.6, startingAngle+ecart, alpha, ecart, pixelStop, option);
            theTree(v,filleD, lon/1.6, startingAngle-ecart, alpha, ecart, pixelStop, option);
        }
    }
}

sf::VertexArray fillLignes(sf::Vector2f startingPoint, const float longeur, float alpha, float beta, int pixelStop, int option){
    dynamicTab tabBranche;//UTILISATION DU TYPE EXPLICITE PLUS HAUT (vector<branche3>)
    //APPELLE DE LA RECURSIVE QUI CALCULES LES BRANCHES (TROIS POINT(x,y) PARENT, FILLE GAUCHE, FILLE DROITE)
    //REMPLIS tabBranche
    //-M_PI/2 CORRESPOND A L'ORIENTATION DE LA STRUCTURE ICI VERS LE HAUT
    theTree(tabBranche, startingPoint, longeur, -M_PI, alpha, beta, pixelStop, option);
    //VECTOR DE LIGNES (SET(x,y) DE POINTs DEUX PAR DEUX), DE TAILLE 4*tabBranche
    //CAR UNE BRANCHE = 2 LIGNES (PERE - FILLE GAUCHE) (PERE - FILLE DROITE) = 4 SET DE POINT
    sf::VertexArray ligne(sf::Lines, 4*tabBranche.size());
    //CURRENT VAS AUGMENTER 4 FOIS A CHAQUE TOUR POUR SUIVRE LE CONTENU DE line DE TAILLE 4*tabBranche
    int current=0;
    for(int i = 0 ; i < tabBranche.size(); i++){
        //ON ENREGISTRE LES LIGNES (SET(x,y) DE POINTS) COMME EXPLIQUER PLUS HAUT
        ligne[current++].position = tabBranche[i].parent;
        ligne[current++].position = tabBranche[i].filleGauche;
        ligne[current++].position = tabBranche[i].parent;
        ligne[current++].position = tabBranche[i].filleDroite;
    }
    return ligne;
}

int main(int, char const**)
{
    //STYLE NONE PAS DE BORDURE PAS DE RESIZE PAS DE CLOSE
    sf::RenderWindow window(sf::VideoMode(2500, 1800), "SFML window", sf::Style::None);
    
    //VARIABLES
    sf::Vector2f startingPoint(1800.0,900.0);//POINT DE DEPART DE LA RECURSIVE
    const float longueur = 300;  //LONGUEUR DE LA PREMIERE BRANCHE EN PX LES BRANCHE
    //SONT DE LONGUEUR longueur/1.6 PAR RAPPORT A LA BRANCHE PRECEDENTE
    std::string str;
    float alpha = M_PI/4;       //ANGLE ECART ENTRE BRANCHE PERE ET BRANCHE FILLE
    //(POUR QUE LES DEUX BRANCHES SOIT A EQUIDISTANSE DE LA BRANCHE PERE alpha = PI/4)
    float beta = M_PI/4;        //ANGLE ECART ENTRE LES DEUX BRANCHE
    bool autoMov=false;         //L'ECART ENTRE LES BRANCHE ET LA BRANCHE PERE AUGMENTE A CHAQUE FRAME TOUT SEUL
    bool autoEcart=false;       //L'ECART ENTRE LES DEUX BRANCHE AUGMENTE A CHAQUE FRAME TOUT SEUL
    float augMovAuto = 0.01;    //SI autoMov = true alpha+=augMovAuto
    float augEcartAuto = 0.01;  //SI autoEcart = true beta+=augEcartAuto
    bool followMouse=true;     //ACTIVE LE SUIVIT DE LA SOURIS
    int pixelStop = 2;          //LONGUEUR MINIMAL D'UNE BRANCHE A PARTIR DE LAQUEL LE PROGRAMME
    //NE CHERCHE PLUS LES BRANCHE RECURSIVEMENT(CONDITION D'ARRET) EN PIXEL
    sf::Vertex startLigne[2] = {sf::Vertex(sf::Vector2f(startingPoint.x,startingPoint.y)),sf::Vertex(sf::Vector2f(startingPoint.x+longueur,startingPoint.y))};
    //LIGNE DECORATIVE DE DEPART ^
    int option = 1;             //OPTION A APPLIQUER A LA BOUCLE RECURSIVE POUR DIFFERENTS EFFETS
    //LIGNE STOQUE LES LIGNE (1 LIGNE = DEUX SET(x,y) DE POINT SUCCESSIF (DEUX PAR DEUX PAS PLUS))
    //ON REMPLIS ligne GRACE A LA FONCTION PRINCIPALE
    sf::VertexArray ligne = *new sf::VertexArray(fillLignes(startingPoint, longueur, alpha, beta, pixelStop, 1));
    
    
    //CREATION DU TEST POUR AFFICHER ALPHA ET BETA DANS LA FENETRE
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
    
    window.setFramerateLimit(60);
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
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                //ON/OFF DE L'AUGMENTATION AUTOMATIQUE (+1 par frame) DE L'ECART ALPHA
                //NE FONCTIONNE PAS AVEC -E (alpha fixe)
                //SENS CONTRAIRE A UP
                autoMov=!autoMov;
                augMovAuto = -0.01;
                followMouse=false;
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
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {//NORMAL
                option=1;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {//PROPORTIONELLE
                option=2;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {//NULL
                option=3;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
                //REDEFINITION DE LA LONGUEURE REQUISE POUR CONTINUER LA RECURSIVITE
                //JOLIE EFFET DE RECONSTRUCTION GRACE AU SI(>2):pixelStop++ DANS LE DRAW
                pixelStop=200;
            }
            
        }
        
        str = std::to_string(option);
        //AFFICHE ALPHA ET BETA SUR L'ECRAN
        if(option==1){
            str = str+" ( normal ) 'E' pour passer en proportiennel";
        }else if(option == 2){
            str = str+" ( proportionnel ) 'D' pour passer en normal";
        }
        text.setString(" esc pour quitter, Alpha : " + std::to_string(fmod(alpha,2*M_PI)) + " Beta : " + std::to_string(fmod(beta,2*M_PI))+"\n" + " option " + str);
        
        window.clear();
        
        
        if(autoMov){//AUGMENTATION DE ALPHA AUTO
            alpha+=augMovAuto;
        }
        if(autoEcart){//AUGMENTATION DE BETA AUTO
            beta+=augEcartAuto;
        }
        if(followMouse){
            //ALPHA(X) ET BETA(Y) EVOLUE EN FONCTION DE LA SOURIS
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            alpha = 0.001*localPosition.y;
            beta = 0.001*localPosition.x;
        }
        for(int i =0; i<ligne.getVertexCount(); i++){
            //DESTRUCTION DES LIGNES AVANT DE LES RECALCULER AVEC LES EVENTUELLES NOUVEAU PARAMETRES
            //EVITE LES FUITES MEMOIRE !IMPORTANT!
            ligne[i].~Vertex();
        }
        
        if(pixelStop>2){
            //LE NOMBRE DE PIXEL NECESSAIRE A FAIRE CALCULER UNE BRANCHE DIMINUE : LES BRANCHE REAPPARAISSE AU FUR ET A MESURE
            //A COMBINER AVEC -G
            pixelStop-=1;
        }
        
        ligne.clear();
        //CREATION DES LIGNES AVEC LES PARAMETRE MIS A JOURS
        ligne = fillLignes(startingPoint, longueur, alpha, beta, pixelStop, option);
        //DRAW LA PREMIERE LIGNES
        window.draw(fond);
        window.draw(startLigne, 2, sf::Lines);
        window.draw(text);
        //DRAW LES LIGNE
        window.draw(ligne);
        window.display();
    }
    
    return EXIT_SUCCESS;
}
