#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int size = 56;
int borderMargin = size / 2;

Sprite figures[32];

// index of the figure in the figure Texture
int board[8][8] =
    {-1, -2, -3, -4, -5, -3, -2, -1,
     -6, -6, -6, -6, -6, -6, -6, -6,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     6, 6, 6, 6, 6, 6, 6, 6,
     1, 2, 3, 4, 5, 3, 2, 1};

std::string position = "";

/*
  // clip image for every figure
*/
void loadPosition()
{
  int k = 0;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int n = board[i][j];
      if (!n)
        continue;
      int x = abs(n) - 1;
      int y = n > 0 ? 1 : 0;
      figures[k].setTextureRect(IntRect(size * x, size * y, size, size));
      figures[k].setPosition(size * j + borderMargin, size * i + borderMargin);
      k++;
    }
  }

  for (int i = 0; i < position.length(); i += 5)
  {
    move(position.substr(i, 4));
  }
}

/*
  convet chess position to chess note like a1, b2
*/
std::string toChessNote(Vector2f p)
{
  std::string s = "";
  s += char((p.x - borderMargin) / size + 97);     // 'a'
  s += char(7 - (p.y - borderMargin) / size + 49); // '1'
  return s;
}

/*
 chess note to coordinate
*/
Vector2f toCoord(char a, char b)
{
  int x = int(a) - 97;
  int y = 7 - int(b) + 49;

  return Vector2f(x * size + borderMargin, y * size + borderMargin);
}

void move(std::string str)
{
  Vector2f oldPos = toCoord(str[0], str[1]);
  Vector2f newPos = toCoord(str[2], str[3]);

  for (int i = 0; i < 32; i++)
  {
    if (figures[i].getPosition() == newPos)
    {
      figures[i].setPosition(-100, -100); // hide chess piece
    }
  }

  for (int i = 0; i < 32; i++)
  {
    if (figures[i].getPosition() == oldPos)
    {
      figures[i].setPosition(newPos); // hide chess piece
    }
  }
}

int main()
{
  RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");

  // Load images
  Texture t1, t2;
  t1.loadFromFile("images/figures.png");
  t2.loadFromFile("images/board.png");

  // Create sprite
  Sprite s(t1);
  Sprite sBoard(t2);

  for (int i = 0; i < 32; i++)
  {
    figures[i].setTexture(t1);
  }
  // clip image for every figure
  loadPosition();

  bool isMove = false;
  float dx = 0, dy = 0;
  Vector2f oldPos, newPos;
  std::string str;

  int n = 0; // index of the chess piece will be moved

  // Main loop
  while (window.isOpen())
  {
    // Process mouse event
    Vector2i pos = Mouse::getPosition(window);

    Event e;
    while (window.pollEvent(e))
    {
      if (e.type == Event::Closed)
      {
        window.close();
      }
      //=================== Move back ===================
      if (e.type == Event::KeyPressed)
      {
        if (e.key.code == Keyboard::BackSpace)
        {
          position.erase(position.length() - 6, 5);
          loadPosition();
        }
      }

      //=================== Drag and drop ===================
      if (e.type == Event::MouseButtonPressed)
      {
        if (e.mouseButton.button == Mouse::Left)
        {
          for (int i = 0; i < 32; i++)
          {
            // Chech every chess piece
            if (figures[i].getGlobalBounds().contains(pos.x, pos.y))
            {
              isMove = true; // start drag
              n = i;
              dx = pos.x - figures[i].getPosition().x;
              dy = pos.y - figures[i].getPosition().y;
              oldPos = figures[i].getPosition();
            }
          }
        }
      }
      if (e.type == Event::MouseButtonReleased)
      {
        if (e.mouseButton.button == Mouse::Left)
        {
          isMove = false;
          Vector2f p = figures[n].getPosition() + Vector2f(size / 2, size / 2);
          Vector2f newPos = Vector2f(size * int((p.x - borderMargin) / size) + borderMargin, size * int((p.y - borderMargin) / size) + borderMargin);
          str = toChessNote(oldPos) + toChessNote(newPos);
          move(str);
          std::cout << str << std::endl;
          position += str + " ";
          figures[n].setPosition(newPos);
        }
      }

      //=================== Update ===================
      // comp move
      if (Keyboard::isKeyPressed(Keyboard::Space))
      {
        str = "d7d5"; // For example
        oldPos = toCoord(str[0], str[1]);
        newPos = toCoord(str[2], str[3]);

        for (int i = 0; i < 32; i++)
        {
          if (figures[i].getPosition() == oldPos)
          {
            n = i;
          }
        }
        // Animation
        for (int k = 0; k < 50; k++)
        {
          Vector2f p = newPos - oldPos;
          figures[n].move(p.x / 50, p.y / 50);
          window.draw(sBoard);
          for (int i = 0; i < 32; i++)
          {
            window.draw(figures[i]);
            window.draw(figures[n]);
          }
        }
        move(str);
        position += str + " ";
        figures[n].setPosition(newPos);
      }

      if (isMove)
      {
        // Move the chess piece
        figures[n].setPosition(pos.x - dx, pos.y - dy);
      }
    }
    //=================== Draw ===================
    window.clear();
    window.draw(sBoard);
    for (int i = 0; i < 32; i++)
    {
      window.draw(figures[i]);
    }
    window.display();
  }

  return 0;
}
