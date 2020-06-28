#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <cmath>
using namespace sf;
class Quad
{
public:
    Quad(int posX, int posY, int Number)
    {
        this->posX=posX;
        this->posY=posY;
        this->Number=Number+1;
        if (this->Number > 9) text.setPosition(posX+8, posY+1);
        else text.setPosition(posX+20, posY+1);
        //std::cout << this->Number << std::endl;
        this->font.loadFromFile("res\\8277.ttf");
        this->text.setFont(this->font);
        this->text.setString(std::to_string(this->Number));
        this->text.setCharacterSize(45);
        this->text.setFillColor(Color::Black);
    }
    void drawQuad()
    {
        glColor3f(209.f/255.f, 254.f/255.f, 203.f/255.f);
        glBegin(GL_QUADS);
        glVertex2i(posX+0, posY+0);
        glVertex2i(posX+0, posY+64);
        glVertex2i(posX+64, posY+64);
        glVertex2i(posX+64, posY+0);
        glEnd();
        glLineWidth(2.0);
        glColor3f(0.f, 0.f, 0.f);
        glBegin(GL_LINE_STRIP);
        glVertex2i(posX+64, posY+0);
        glVertex2i(posX+0, posY+0);
        glVertex2i(posX+0, posY+64);
        glVertex2i(posX+64, posY+64);
        glVertex2i(posX+64, posY+0);
        glEnd();
    }
    Text getText()
    {
        return text;
    }
    void interacteWithMap(Vector2f &pos, sf::String *Map)
    {
        if(((pos.x>=posX)&&(pos.x<posX+64))&&(pos.y>=posY)&&(pos.y<posY+64))
        {
            if((((posX/64)-1) >= 0)&&((Map[((posX/64)-1)][posY/64]) == ' '))
            {
                Map[posX/64][posY/64] = ' ';
                posX-=64;
                Map[posX/64][posY/64] = '0';
            } else
            if((((posX/64)+1) < 4)&&((Map[((posX/64)+1)][posY/64]) == ' '))
            {
                Map[posX/64][posY/64] = ' ';
                posX+=64;
                Map[posX/64][posY/64] = '0';
            } else
            if((((posY/64)-1) >= 0)&&((Map[(posX/64)][(posY/64)-1]) == ' '))
            {
                Map[posX/64][posY/64] = ' ';
                posY-=64;
                Map[posX/64][posY/64] = '0';
            } else
            if((((posY/64)+1) < 4)&&((Map[(posX/64)][(posY/64)+1]) == ' '))
            {
                Map[posX/64][posY/64] = ' ';
                posY+=64;
                Map[posX/64][posY/64] = '0';
            }
            if (Number > 9) text.setPosition(posX+8, posY+1);
            else text.setPosition(posX+20, posY+1);
        }
    }
private:
    int posX, posY, Number;
    Font font;
    Text text;
};

void initOpenGl()
{
    glViewport(0, 0, 256, 256);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 256, 256, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScaled(1, 1, 0);
}
const int HEIGHT_MAP = 4;
const int WIDTH_MAP = 4;
static sf::String Map[WIDTH_MAP]
    {
    "0000",
    "0000",
    "0000",
    "0000",
};
void genQuads()
{
    srand(unsigned(time(nullptr)));
    int x = rand()%4;
    int y = rand()%4;
    Map[x][y] = ' ';
}
void genQuads(std::list<Quad*> &quadsList)
{
    genQuads();
    int number = 0;
    std::vector<int> n(15);
    for (int i = 0; i < 15; i++)
    {
        n[i] = i;
    }
    random_shuffle(n.begin(), n.end());
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if(Map[i][j] == ' ') continue;
            quadsList.push_back(new Quad(i*64, j*64, n[number]));
            number++;
        }
}
int main()
{
    FreeConsole();
    RenderWindow window(VideoMode(256, 256), "15-puzzle", Style::Close);
    window.setVerticalSyncEnabled(true);
    std::list <Quad*> quadsList;
    std::list <Quad*>::iterator it;
    genQuads(quadsList);
    while (window.isOpen())
    {
        window.clear(Color::White);
        initOpenGl();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
                if (event.key.code == Keyboard::R)  //restart game
                {
                    for (it = quadsList.begin(); it != quadsList.end(); it++)
                    {
                        (*it)->~Quad();
                    }
                    quadsList.clear();
                    for (int i = 0; i < HEIGHT_MAP; i++)
                        for (int j = 0; j < WIDTH_MAP; j++)
                        {
                            if(Map[i][j] == ' ') Map[i][j] = '0';
                        }
                    genQuads(quadsList);
                }
            }
        }
        Vector2i pixelPos = Mouse::getPosition(window);
        Vector2f pos = window.mapPixelToCoords(pixelPos);
        if (event.mouseButton.button == Mouse::Left)
            for (it = quadsList.begin(); it != quadsList.end(); it++)
                (*it)->interacteWithMap(pos, Map);
        glClear(GL_COLOR_BUFFER_BIT);
        for (it = quadsList.begin(); it != quadsList.end(); it++)
        {
            (*it)->drawQuad();
        }
        for (it = quadsList.begin(); it != quadsList.end(); it++)
        {
            window.draw((*it)->getText());
        }
        window.display();
    }
    return 0;
}
