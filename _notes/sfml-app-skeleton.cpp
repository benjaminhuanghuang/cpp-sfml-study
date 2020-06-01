#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
  RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");

  // Load images
  Texture t1, t2;
  t1.loadFromFile("images/figures.png");
  t2.loadFromFile("images/board.png");

  // Create sprite
  Sprite s(t1);

  bool isMove = false;
  float dx = 0, dy = 0;

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

      //=================== Drag and drop ===================
      if (e.type == Event::MouseButtonPressed)
      {
        if (e.mouseButton.button == Mouse::Left)
        {
          if (s.getGlobalBounds().contains(pos.x, pos.y))
          {
            isMove = true;
            dx = pos.x - s.getPosition().x;
            dy = pos.y - s.getPosition().y;
          }
        }
      }
      if (e.type == Event::MouseButtonReleased)
      {
        if (e.mouseButton.button== Mouse::Left)
        {
          if (s.getGlobalBounds().contains(pos.x, pos.y))
          {
            isMove = false;
          }
        }
      }

      //=================== Update ===================
      if(isMove){
        s.setPosition(pos.x-dx, pos.y-dy);
      }

    }
    //=================== Draw ===================
    window.clear();
    window.draw(s);
    window.display();
  }

  return 0;
}
