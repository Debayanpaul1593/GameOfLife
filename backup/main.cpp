#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <vector>
#include <cstdlib>
#define SCREEN_WIDTH 540 
#define SCREEN_HEIGHT 540
#define CELL_SIZE 12
constexpr std::chrono::microseconds FRAME_DURATION(16667);
sf::Color LIVE_COLOR = sf::Color(0, 0, 0);
sf::Color DEAD_COLOR = sf::Color(255, 255, 255);
sf::Color GRAY_COLOR = sf::Color(0xDAC7D4BF);
sf::Color GREEN_COLOR = sf::Color(0x777777BF);
sf::Color FUCHSIA_COLOR = sf::Color(0x915C83D9);
class Cell
{
  sf::RectangleShape rect;
  int alive;
  float x, y;
  int get_random(int x, int y)
  {
    int posx = x/CELL_SIZE;
    int posy = y/CELL_SIZE;
    if(posx/10 == 2 && (posx%10)%2 == 0){
      return 1;
    }else{
      return 0;
    }
    //if((posx == 5 && posy == 11) || (posx == 6 && posy == 10) || (posx == 6 && posy == 12) || (posx == 7 && posy == 11) || (posx == 4 && posy == 10))
    //{
      //return 1;
    //}else {
      //return 0;
    //}
  }
  public:
    Cell(){};
    Cell(float pos_x, float pos_y)
    {
      rect = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
      rect.setFillColor(GRAY_COLOR);
      //rect.setOutlineColor(sf::Color(0, 0, 0));
      rect.setOutlineThickness(0.5f);
      x = pos_x;
      y = pos_y;
      rect.setPosition(sf::Vector2f(pos_x, pos_y));
      //alive = rand() % 2;
      alive = get_random(pos_x, pos_y);
    }

    void draw(sf::RenderWindow& window, bool is_alive)
    {
      alive = is_alive;
      rect.setFillColor(is_alive == 1? FUCHSIA_COLOR : GRAY_COLOR);
      window.draw(rect);
    }

    int is_alive()
    {
      return alive;
    }

    void set_alive(int _alive)
    {
      alive = _alive;
    }

    void print_position()
    {
      std::cout<<"x: "<<rect.getPosition().x<<", y: "<<rect.getPosition().y<<std::endl;
    }
};

int get_lower(int x)
{
  int cells_across = SCREEN_WIDTH / CELL_SIZE;
  return x > 0 ? x : cells_across - 1;
}

int get_upper(int x)
{
  int cells_across = SCREEN_WIDTH / CELL_SIZE;
  return x < cells_across - 1 ? x + 1 : 0;
}

void assign_cells(std::vector<std::vector<Cell>>& cells)
{
  short cells_across = SCREEN_WIDTH / CELL_SIZE;
  //assign row wise so j will be x and i will be y
  for(int i = 0; i<cells_across; i++)
  {
    cells[i] = std::vector<Cell>(cells_across);
    for(int j = 0; j<cells_across; j++)
    {
      cells[i][j] = Cell(CELL_SIZE * j, CELL_SIZE * i);
    }
  }
}

void render_cells(std::vector<std::vector<Cell>>& cells, sf::RenderWindow& window)
{
  short cells_across = SCREEN_WIDTH / CELL_SIZE;
  for(int i = 0; i<cells_across; i++)
  {
    for(int j = 0; j<cells_across; j++)
    {
      Cell& current = cells[i][j];
      //extract neighboring cells
      int a, b;
      int live_count = 0, dead_count = 0;
      for(a = get_lower(i-1); a<=get_upper(i); a++)
      {
        for(b = get_lower(j-1); b<=get_upper(j); b++)
        {
          //ignore center cell
          if(a!=i || b!=j)
          {
            Cell nbr = cells[a][b];
            if(nbr.is_alive())
            {
              live_count++;
            }else {
              dead_count++;
            }
          }
        }
      }
      //check conditions here
      bool curr_alive = current.is_alive();
      bool new_stat = curr_alive;
      //live cell with underpop & overpop
      if(curr_alive && (live_count == 2 || live_count == 3))
      {
        new_stat = 1;
      }else if(!curr_alive && live_count == 3)
      {
        new_stat = 1;
      }else if(curr_alive)
      {
        new_stat = 0;
      }
      //if(new_stat != curr_alive)
      //{
        //std::cout<<"Changed state ["<<i<<", "<<j<<"] from: "<<curr_alive<<" to: "<<new_stat<<std::endl;
      //}
      //current.set_alive(new_stat);
      current.draw(window, new_stat);
    }
  }
}


int main()
{

  sf::RenderWindow render_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Conway's Game of Life");
  std::chrono::microseconds lag(0);
  std::chrono::steady_clock::time_point previous_time;
  std::chrono::steady_clock::now();
  sf::Event event;
  previous_time = std::chrono::steady_clock::now();
  std::vector<std::vector<Cell>> cells(SCREEN_WIDTH / CELL_SIZE);
  assign_cells(cells);
  while(render_window.isOpen())
  {
    std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);
    lag += delta_time;
    previous_time += delta_time;
    while(FRAME_DURATION <= lag)
    {
      lag -= FRAME_DURATION;
      while(render_window.pollEvent(event))
      {
          if(event.type == sf::Event::EventType::Closed)
          {
            render_window.close();
          }
      }
      if(FRAME_DURATION > lag)
      {
        render_window.clear(sf::Color(255, 255, 255, 1));
        render_cells(cells, render_window);
        render_window.display();
      }
    }

  }

}



















