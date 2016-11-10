#include <SFML/Graphics.hpp>
#include <forward_list>
#include <fstream>
#include <cstdio>
#include "boost/filesystem.hpp"

extern char binary_BeaufortforLOL_Bold_ttf_start[];
extern char binary_BeaufortforLOL_Bold_ttf_end[];
extern char binary_BeaufortforLOL_Bold_ttf_size[];

const char* Beaufortfor_ttf_start = binary_BeaufortforLOL_Bold_ttf_start;
int Beaufortfor_ttf_size = binary_BeaufortforLOL_Bold_ttf_end-binary_BeaufortforLOL_Bold_ttf_start;

class Group : public sf::Drawable {
    public:
        bool hidden;
        std::forward_list<sf::Drawable*> lista;
        Group() {
            hidden=false;
            lista.clear();
        };
        /*~Group() {
            for(auto it = lista.begin(); it != lista.end(); ++it) {
                delete (*it);
            }
        }*/

        void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if(hidden) return;
            for(auto it = lista.begin(); it != lista.end(); ++it) {
                target.draw((**it),states);
            }
        }
        sf::Drawable* add(sf::Drawable* a) {
            lista.push_front(a);
            return a;
        };
};

class Button : public sf::Drawable {
    private:
        sf::Text text;
        sf::RectangleShape inner;
        sf::RectangleShape outter;
        void (*f)();
    public:
        bool over;
        bool press;
        Button(std::string str,float x, float y,sf::Font& font, void (*func)()) : text(str,font,20) {
            text.setColor(sf::Color(205,190,145));
            text.setPosition(x+5,y);
            inner.setSize(sf::Vector2f(text.getGlobalBounds().width+6,23));
            printf("%f",text.getGlobalBounds().height);
            inner.setFillColor(sf::Color(30,35,40));
            inner.setOutlineColor(sf::Color(7,15,23));
            inner.setOutlineThickness(1);
            outter.setSize(sf::Vector2f(text.getGlobalBounds().width+12,29));
            outter.setFillColor(sf::Color(200,170,108));
            outter.setOutlineColor(sf::Color(4,15,20));
            outter.setOutlineThickness(1);
            outter.setPosition(x,y);
            inner.setPosition(x+3,y+3);
            f=func;
        }
        void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(outter);
            target.draw(inner);
            target.draw(text);
        }
        void checkOver(int x, int y) {
            if(!outter.getGlobalBounds().contains(x,y)) {
                if(!over) return;
                else {
                    over=false;
                    text.setColor(sf::Color(205,190,145));
                    inner.setFillColor(sf::Color(30,35,40));
                    inner.setOutlineColor(sf::Color(7,15,23));
                    outter.setFillColor(sf::Color(200,170,108));
                    outter.setOutlineColor(sf::Color(4,15,20));
                }
            } else {
                over=true;
                text.setColor(sf::Color(255,240,195));
                inner.setFillColor(sf::Color(70,75,80));
                inner.setOutlineColor(sf::Color(7,15,23));
                outter.setFillColor(sf::Color(250,220,158));
                outter.setOutlineColor(sf::Color(4,15,20));
            }
        }

        void checkPress(int x, int y) {
            if(!outter.getGlobalBounds().contains(x,y)) {
                if(!over) return;
                else {
                    over=false;
                    text.setColor(sf::Color(205,190,145));
                    inner.setFillColor(sf::Color(30,35,40));
                    inner.setOutlineColor(sf::Color(7,15,23));
                    outter.setFillColor(sf::Color(200,170,108));
                    outter.setOutlineColor(sf::Color(4,15,20));
                }
            } else {
                over=true;
                press=true;
                text.setColor(sf::Color(155,140,95));
                inner.setFillColor(sf::Color(20,25,30));
                inner.setOutlineColor(sf::Color(7,15,23));
                outter.setFillColor(sf::Color(150,120,58));
                outter.setOutlineColor(sf::Color(4,15,20));
            }
        }

        void checkRelease(int x, int y) {
            if(!outter.getGlobalBounds().contains(x,y)) {
                if(!press) return;
                else {
                    press=false;
                    text.setColor(sf::Color(205,190,145));
                    inner.setFillColor(sf::Color(30,35,40));
                    inner.setOutlineColor(sf::Color(7,15,23));
                    outter.setFillColor(sf::Color(200,170,108));
                    outter.setOutlineColor(sf::Color(4,15,20));
                }
            } else {
                over=true;
                press=false;
                text.setColor(sf::Color(255,240,195));
                inner.setFillColor(sf::Color(70,75,80));
                inner.setOutlineColor(sf::Color(7,15,23));
                outter.setFillColor(sf::Color(250,220,158));
                outter.setOutlineColor(sf::Color(4,15,20));
                f();
            }
        }
};

bool bClose;
bool error;
bool good;

void fClose() {
    bClose=true;
}

using namespace boost::filesystem;

void fDoIt() {
    std::string line="";
    std::string out = "";
    if(!is_directory(relative("RADS\\projects\\league_client\\releases"))) { error=true; return; }
    path greatest;
    for(directory_entry& x : directory_iterator(relative("RADS\\projects\\league_client\\releases"))) {
        if(x.path().filename().string().find('.')==x.path().filename().string().npos) continue;
        if(x.path().compare(greatest)) greatest=x.path();
    }

    path New = greatest;
    path Old = greatest;
    path Current = greatest;
    New+=path("\\deploy\\system.yaml.new");
    Old+=path("\\deploy\\system.yaml.old");
    Current+=path("\\deploy\\system.yaml");
    printf("%ls\n",greatest.c_str());
    if(is_regular_file(Current)) printf("Yep\n");
    ifstream myfile;
    try {
        myfile.open(Current);
    } catch (std::exception& e) {
        printf("%s",e.what());
        error=true;
        good=false;
        return;
    }
    printf("%ls\n",greatest.c_str());
    if(!myfile.good()) { printf("Not Good"); error=true; return; }
    while(!myfile.eof() && line!="  LA2:") { std::getline(myfile,line); out.append(line); out.push_back((char)10); }
    while(!myfile.eof() && line!="    available_locales:") { std::getline(myfile,line); out.append(line); out.push_back((char)10); }
    out.append(R"(    - en_US
    - es_MX
    - es_ES
    - pt_BR
    - de_DE
    - ja_JP
    - fr_FR
    - it_IT
    - cs_CZ
    - el_GR
    - hu_HU
    - pl_PL
    - ro_RO
    - ru_RU
    - tr_TR
    - en_GB
    - en_AU
)");
    std::getline(myfile,line);
    while(!myfile.eof() && line.at(4) == '-') { std::getline(myfile,line); }
    out.append(line); out.push_back((char)10);
    while(!myfile.eof()) { std::getline(myfile,line); out.append(line); out.push_back((char)10); }
    myfile.close();
    ofstream newfile (New,std::ios::out);
    newfile.write(out.c_str(),out.length());
    newfile.close();
    try {
        if(!is_regular_file(Old)) rename(Current,Old);
        rename(New,Current);
    } catch(std::exception& e) {
        printf("%s",e.what());
        error=true;
        good=false;
        return;
    }

    good=true;
    error=false;
    printf("Finished");
}

void fRevert() {
    if(!is_directory(relative("RADS\\projects\\league_client\\releases"))) { error=true; return; }
    path greatest;
    for(directory_entry& x : directory_iterator(relative("RADS\\projects\\league_client\\releases"))) {
        if(x.path().filename().string().find('.')==x.path().filename().string().npos) continue;
        if(x.path().compare(greatest)) greatest=x.path();
    }
    path Old = greatest;
    path Current = greatest;
    Old+=path("\\deploy\\system.yaml.old");
    Current+=path("\\deploy\\system.yaml");
    try {
        if(is_regular_file(Old)) rename(Old,Current);
        else {
            error=true;
            good=false;
            return;
        }
    } catch(std::exception& e) {
        printf("%s",e.what());
        error=true;
        good=false;
        return;
    }
    good=true;
    error=false;
}

int main()
{
    bClose=false;
    error=false;
    good=false;
    // Create the main window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow app(sf::VideoMode(400, 400), "Language Enabler", sf::Style::None,settings);
    sf::Font font;
    font.loadFromMemory(Beaufortfor_ttf_start,Beaufortfor_ttf_size);

    sf::Text title("HABILITADOR DE IDIOMAS",font,30);
    title.setPosition(8,0);
    title.setColor(sf::Color(205,190,145));
    // Load a sprite to display
    /*sf::Texture texture;
    if (!texture.loadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);*/
    Group bg;
    bg.add(&title);

    sf::CircleShape triangle(80,3);
    triangle.setFillColor(sf::Color(200,50,50));
    triangle.setOutlineColor(sf::Color(150,10,10));
    triangle.setOutlineThickness(3);
    triangle.rotate(180);
    triangle.setPosition(280,210);

    sf::Text BigMark("!",font,80);
    BigMark.setColor(sf::Color(100,10,10));
    BigMark.setPosition(188,93);

    Group gError;
    gError.add(&BigMark);
    gError.add(&triangle);

    sf::CircleShape gtriangle(80,3);
    gtriangle.setFillColor(sf::Color(50,200,50));
    gtriangle.setOutlineColor(sf::Color(10,150,10));
    gtriangle.setOutlineThickness(3);
    gtriangle.setPosition(120,70);

    sf::Text gBigMark("!",font,80);
    gBigMark.setColor(sf::Color(10,100,10));
    gBigMark.setPosition(188,93);

    Group gGood;
    gGood.add(&gBigMark);
    gGood.add(&gtriangle);

    sf::RectangleShape* upper = new sf::RectangleShape();
    upper->setFillColor(sf::Color(120,90,40));
    upper->setPosition(1,0);
    upper->setSize(sf::Vector2f(398,2));

    sf::RectangleShape* left = new sf::RectangleShape();
    left->setFillColor(sf::Color(30,40,45));
    left->setPosition(0,0);
    left->setSize(sf::Vector2f(1,400));

    sf::RectangleShape* right = new sf::RectangleShape();
    right->setFillColor(sf::Color(30,40,45));
    right->setPosition(399,0);
    right->setSize(sf::Vector2f(1,400));

    sf::RectangleShape* lower = new sf::RectangleShape();
    lower->setFillColor(sf::Color(30,40,45));
    lower->setPosition(0,399);
    lower->setSize(sf::Vector2f(400,1));

    bg.add(upper);

    sf::Text footer(L"By Desaroll. Sin garantía.",font,10);
    sf::Text footer2(L"Si se rompe, te quedas con ambas partes.",font,10);
    sf::Text footer3(L"Esto no fue aprobado ni desaprobado por Riot.",font,10);
    sf::Text version(L"v1.0",font,10);
    footer.setPosition(10,355);
    footer2.setPosition(10,370);
    footer3.setPosition(10,385);
    version.setPosition(370,385);

    footer.setColor(sf::Color(205,190,145));
    footer2.setColor(sf::Color(205,190,145));
    footer3.setColor(sf::Color(205,190,145));

    bg.add(&footer);
    bg.add(&footer2);
    bg.add(&footer3);
    bg.add(&version);

    sf::Text howTo(L"          Asegurate que el archivo \"Lang Enabler.exe\" esté dentro de la carpeta \n                 del juego (Normalmente, \"C:\\Riot Games\\League Of Legends\\\")\n                                                                          y abrelo desde allí.",font,10);
    howTo.setPosition(5,200);

    Button doIt("Habilitar todos los Idiomas para LAS",40,250,font,fDoIt);
    Button revert("Revertir",80,300,font,fRevert);
    Button close("Cerrar",240,300,font,fClose);
	// Start the game loop
    while (app.isOpen())
    {

        // Clear screen
        app.clear(sf::Color(1,10,18));

        // Draw the sprite
        //app.draw(sprite);
        app.draw(bg);
        app.draw(doIt);
        app.draw(close);
        app.draw(revert);
        app.draw(howTo);
        if(error) app.draw(gError);
        if(good) app.draw(gGood);

        // Update the window
        app.display();

        // Process events

        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                bClose=true;
            } else if (event.type == sf::Event::MouseMoved) {
                doIt.checkOver(event.mouseMove.x,event.mouseMove.y);
                close.checkOver(event.mouseMove.x,event.mouseMove.y);
                revert.checkOver(event.mouseMove.x,event.mouseMove.y);
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
                doIt.checkPress(event.mouseButton.x,event.mouseButton.y);
                close.checkPress(event.mouseButton.x,event.mouseButton.y);
                revert.checkPress(event.mouseButton.x,event.mouseButton.y);
            } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
                doIt.checkRelease(event.mouseButton.x,event.mouseButton.y);
                close.checkRelease(event.mouseButton.x,event.mouseButton.y);
                revert.checkRelease(event.mouseButton.x,event.mouseButton.y);
            }
        }
        if(bClose == true) app.close();
    }
    printf("Exiting");

    return 0;
}
