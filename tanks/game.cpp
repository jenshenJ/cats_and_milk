#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>
#include <windows.h>
#include <vector>
#include "map.h"

using namespace sf;


struct bullet {
	int type_of_movement = 1;
	int x_length = 10;
	int y_length = 25;
	int x, y;
	int speedY, speedX, speed = 10;
	Sprite sbullet;
	bool is_draw;
	int team;
	/*type_of_movement:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/
};

struct tanks{
	bool alive = true;
	int orientation;
	double x = 0, y = 0;
	double shoot_time = 0;
	double speed = 5;
	bool move_ability = true;
	Sprite stank;
	int team;
	int size = 93;
};
void check_colision(bullet& bull, int height, int width);
void check_shooting(bullet& bull, std::vector<tanks>& tanks_arr);
void check_blocks(bullet& bull);
int main() {
	
	int window_height = 1000, window_width = 1920; 
	std::vector<bullet> bullets = {};
	std::vector<tanks> tanks_arr = {};
	
	int fps = 200;
	double  time =  100;
	int block_size = 100;
	RenderWindow window(VideoMode(window_width, window_height), "tanks");

	Texture ttank_up, ttank_right, ttank_left, ttank_down, enemy_ttank_up, enemy_ttank_down, enemy_ttank_left, enemy_ttank_right;
	ttank_up.loadFromFile("resource\\tank_up.png");
	ttank_right.loadFromFile("resource\\tank_right.png");
	ttank_left.loadFromFile("resource\\tank_left.png");
	ttank_down.loadFromFile("resource\\tank_down.png");

	enemy_ttank_up.loadFromFile("resource\\enemy_tank_up.png");
	enemy_ttank_right.loadFromFile("resource\\enemy_tank_right.png");
	enemy_ttank_left.loadFromFile("resource\\enemy_tank_left.png");
	enemy_ttank_down.loadFromFile("resource\\enemy_tank_down.png");

	Texture tbullet_up, tbullet_right, tbullet_left, tbullet_down;
	tbullet_up.loadFromFile("resource\\bullet_up.png");
	tbullet_right.loadFromFile("resource\\bullet_right.png");
	tbullet_left.loadFromFile("resource\\bullet_left.png");
	tbullet_down.loadFromFile("resource\\bullet_down.png");

	Texture wood_block1, wood_block2, wood_block3, dark_block;
	wood_block1.loadFromFile("resource\\wood_block1.png");
	wood_block2.loadFromFile("resource\\wood_block2.png");
	wood_block3.loadFromFile("resource\\wood_block3.png");
	dark_block.loadFromFile("resource\\dark_block.png");
	Sprite smap;


	tanks tank, tank1;
	tank.stank.setTexture(enemy_ttank_left);
	tank1.stank.setTexture(ttank_right);
	

	/*orientation:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/
	tank.orientation = 3;
	tank1.orientation = 4;
	tank1.team = 1;
	tank1.x = 0, tank1.y = 500, tank.x = 1820, tank.y = 500;
	tank1.stank.setPosition(tank1.x, tank1.y);
	tank.stank.setPosition(tank.x, tank1.y);
	tank.team = 2;
	tank1.shoot_time = -100;
	tank.shoot_time = -100;
	tanks_arr.push_back(tank);
	tanks_arr.push_back(tank1);
	

	while (window.isOpen()) {
		//проверка на закрытие
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		//tank2 movement

		if (Keyboard::isKeyPressed(Keyboard::Left) ) {
			tank.orientation = 3;
			tank.stank.setTexture(enemy_ttank_left);
			if (tank.x > 0 && (tank.x > tank1.x + tank.size + tank.speed || tank.x + tank.size < tank1.x || tank.y + tank.size < tank1.y || tank.y > tank1.y + tank.size)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * block_size, block_y = i * block_size;
						if (!(tank.x > block_x + block_size + tank.speed || tank.x + tank.size < block_x || tank.y + tank.size < block_y || tank.y > block_y + block_size)) {
							tank.move_ability = false;
						}
					}
				if (tank.move_ability) {
					tank.x -= tank.speed;
					tank.stank.setPosition(tank.x, tank.y);
				}
				tank.move_ability = true;
			}
			
		}

		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			tank.orientation = 4;
			tank.stank.setTexture(enemy_ttank_right);
			
			if (window_width - tank.size > tank.x && (tank1.x + tank.size <  tank.x || tank.x + tank.size + tank.speed < tank1.x || tank.y + tank.size < tank1.y || tank.y > tank1.y + tank.size)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(block_x + block_size < tank.x || tank.x + tank.size + tank.speed < block_x || tank.y + tank.size < block_y || tank.y > block_y + block_size)) {
							tank.move_ability = false;
						}
					}
				if (tank.move_ability) {
					tank.x += tank.speed;
					tank.stank.setPosition(tank.x, tank.y);
				}
				tank.move_ability = true;
			}
		}

  		 else if (Keyboard::isKeyPressed(Keyboard::Up)) {
			tank.orientation = 1;
			tank.stank.setTexture(enemy_ttank_up);
			if (tank.y > 0 && (tank.y + tank.size < tank1.y || tank.x > tank1.x + tank.size || tank.x + tank.size < tank1.x || tank.y > tank1.y + tank.size + tank.speed)) {
				
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(tank.y + tank.size < block_y || tank.x > block_x + block_size || tank.x + tank.size < block_x || tank.y > block_y + block_size + tank.speed)) {
							tank.move_ability = false;
						}
					}
				if (tank.move_ability) {
					tank.y -= tank.speed;
					tank.stank.setPosition(tank.x, tank.y);
				}
				tank.move_ability = true;
			}
		 }

		 else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			tank.orientation = 2;
			tank.stank.setTexture(enemy_ttank_down);
			if (window_height - tank.size > tank.y && (tank.y + tank.size + tank.speed < tank1.y || tank.y > tank1.y + tank.size || tank.x > tank1.x + tank.size || tank.x + tank.size < tank1.x)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(tank.y + tank.size + tank.speed < block_y || tank.y > block_y + block_size || tank.x > block_x + block_size || tank.x + tank.size < block_x)) {
							tank.move_ability = false;
						}
					}
				if (tank.move_ability) {
					tank.y += tank.speed;
					tank.stank.setPosition(tank.x, tank.y);
				}
				tank.move_ability = true;
			}
	      }
		//tank1 movement
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			tank1.orientation = 3;
			tank1.stank.setTexture(ttank_left);
			if (tank1.x > 0 && (tank1.x > tank.x + tank1.size + tank1.speed || tank1.x + tank1.size < tank.x || tank1.y + tank1.size < tank.y || tank1.y > tank.y + tank1.size)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * block_size, block_y = i * block_size;
						if (!(tank1.x > block_x + block_size + tank1.speed || tank1.x + tank1.size < block_x || tank1.y + tank1.size < block_y || tank1.y > block_y + block_size)) {
							tank1.move_ability = false;
						}
					}
				if (tank1.move_ability) {
					tank1.x -= tank1.speed;
					tank1.stank.setPosition(tank1.x, tank1.y);
				}
				tank1.move_ability = true;
			}
		}

		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			tank1.orientation = 4;
			tank1.stank.setTexture(ttank_right);

			if (window_width - tank1.size > tank1.x && (tank.x + tank1.size < tank1.x || tank1.x + tank1.size + tank1.speed < tank.x || tank1.y + tank1.size < tank.y || tank1.y > tank.y + tank1.size)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(block_x + block_size < tank1.x || tank1.x + tank1.size + tank1.speed < block_x || tank1.y + tank1.size < block_y || tank1.y > block_y + block_size)) {
							tank1.move_ability = false;
						}
					}
				if (tank1.move_ability) {
					tank1.x += tank1.speed;
					tank1.stank.setPosition(tank1.x, tank1.y);
				}
				tank1.move_ability = true;
			}
		}

		else if (Keyboard::isKeyPressed(Keyboard::W)) {
			tank1.orientation = 1;
			tank1.stank.setTexture(ttank_up);
			if (tank1.y > 0 && (tank1.y + tank1.size < tank.y || tank1.x > tank.x + tank1.size || tank1.x + tank1.size < tank.x || tank1.y > tank.y + tank1.size + tank1.speed)) {

				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(tank1.y + tank1.size < block_y || tank1.x > block_x + block_size || tank1.x + tank1.size < block_x || tank1.y > block_y + block_size + tank1.speed)) {
							tank1.move_ability = false;
						}
					}
				if (tank1.move_ability) {
					tank1.y -= tank1.speed;
					tank1.stank.setPosition(tank1.x, tank1.y);
				}
				tank1.move_ability = true;
			}
		}

		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			tank1.orientation = 2;
			tank1.stank.setTexture(ttank_down);
			if (window_height - tank1.size > tank1.y && (tank1.y + tank1.size + tank1.speed < tank.y || tank1.y > tank.y + tank1.size || tank1.x > tank.x + tank1.size || tank1.x + tank1.size < tank.x)) {
				for (int i = 0; i < HEIGHT_MAP; i++)
					for (int j = 0; j < WIDTH_MAP; j++) {
						if (TileMap[i][j] != 's') {
							continue;
						}
						int block_x = j * 100, block_y = i * 100;
						if (!(tank1.y + tank1.size + tank1.speed < block_y || tank1.y > block_y + block_size || tank1.x > block_x + block_size || tank1.x + tank1.size < block_x)) {
							tank1.move_ability = false;
						}
					}
				if (tank1.move_ability) {
					tank1.y += tank1.speed;
					tank1.stank.setPosition(tank1.x, tank1.y);
				}
				tank1.move_ability = true;
			}
		}
		//стрельба второго
		
		if (Keyboard::isKeyPressed(Keyboard::RControl) && time - tank.shoot_time > 0.7) {
			bullet bull;
			bull.type_of_movement = tank.orientation;
			bull.team = tank.team;
			switch (bull.type_of_movement) {
			case 1:
				bull.x = tank.x + 43;
				bull.y = tank.y;
				bull.speedX = 0;
				bull.speedY = -bull.speed;
				bull.sbullet.setTexture(tbullet_up);
				bull.x_length = 10;
				bull.y_length = 25;
				break;
			case 2:
				bull.x = tank.x + 43;
				bull.y = tank.y + 75;
				bull.speedX = 0;
				bull.speedY = bull.speed;
				bull.sbullet.setTexture(tbullet_down);
				bull.x_length = 10;
				bull.y_length = 25;
				break;
			case 3:
				bull.x = tank.x + 25;
				bull.y = tank.y + 45;
				bull.speedX = -bull.speed;
				bull.speedY = 0;
				bull.sbullet.setTexture(tbullet_left);
				bull.x_length = 25;
				bull.y_length = 10;
				break;
			case 4:
				bull.x = tank.x + 50;
				bull.y = tank.y + 45;
				bull.speedX = bull.speed;
				bull.speedY = 0;
				bull.sbullet.setTexture(tbullet_right);
				bull.x_length = 25;
				bull.y_length = 10;
				break;
			}
			bull.is_draw = true;
			bullets.push_back(bull);
			tank.shoot_time = time;
		}
		//стрельба первого
		if (Keyboard::isKeyPressed(Keyboard::Space) && time - tank1.shoot_time > 0.7) {
			bullet bull1;
			bull1.type_of_movement = tank1.orientation;
			bull1.team = tank1.team;
			switch (bull1.type_of_movement) {
			case 1:
				bull1.x = tank1.x + 45;
				bull1.y = tank1.y;
				bull1.speedX = 0;
				bull1.speedY = -bull1.speed;
				bull1.sbullet.setTexture(tbullet_up);
				break;
			case 2:
				bull1.x = tank1.x + 45;
				bull1.y = tank1.y + 75;
				bull1.speedX = 0;
				bull1.speedY = bull1.speed;
				bull1.sbullet.setTexture(tbullet_down);
				break;
			case 3:
				bull1.x = tank1.x + 25;
				bull1.y = tank1.y + 45;
				bull1.speedX = -bull1.speed;
				bull1.speedY = 0;
				bull1.sbullet.setTexture(tbullet_left);
				break;
			case 4:
				bull1.x = tank1.x + 50;
				bull1.y = tank1.y + 45;
				bull1.speedX = bull1.speed;
				bull1.speedY = 0;
				bull1.sbullet.setTexture(tbullet_right);
				break;
			}
			bullets.push_back(bull1);
			tank1.shoot_time = time;
			
		}

		//перерисовка окна(вынести в функцию)
		window.clear();
		tanks_arr.clear();
		tanks_arr.push_back(tank);
		tanks_arr.push_back(tank1);
		

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				if (TileMap[i][j] == 's') smap.setTexture(wood_block3);
				else smap.setTexture(dark_block);

				smap.setPosition(j * block_size, i * block_size);
				window.draw(smap);
			}
		for (auto& bull : bullets) {
			check_colision(bull, window_height, window_width);
			check_shooting(bull, tanks_arr);
			check_blocks(bull);
			if (bull.is_draw) {
				bull.x += bull.speedX;
				bull.y += bull.speedY;
				bull.sbullet.setPosition(bull.x, bull.y);
				window.draw(bull.sbullet);
			}
		}
		
		window.draw(tank.stank);
		window.draw(tank1.stank);
		window.display();
		//установка частоты обновления
		Sleep(1000 / fps);
		time += 1.0 / fps;
	}

	return 0;
}

void check_colision(bullet& bull, int height, int width) {
	if (bull.x > width || bull.x * bull.y < 0 || bull.y > height) {
		bull.is_draw = false;
	}
}

void check_shooting(bullet& bull, std::vector<tanks>& tanks_arr) {
	for (auto tank : tanks_arr) {
		bool a_inside, b_inside, c_inside, d_inside;
		if (bull.team != tank.team) {
			a_inside = bull.x > tank.x && bull.x < tank.x + tank.size && bull.y > tank.y && bull.y < tank.y + tank.size;
			b_inside = bull.x + bull.x_length > tank.x && bull.x + bull.x_length< tank.x + tank.size && bull.y > tank.y && bull.y < tank.y + tank.size;
			c_inside = bull.x + bull.x_length > tank.x && bull.x + bull.x_length< tank.x + tank.size && bull.y + bull.y_length > tank.y && bull.y + bull.y_length< tank.y + tank.size;
			d_inside = bull.x > tank.x && bull.x < tank.x + tank.size && bull.y + bull.y_length > tank.y && bull.y + bull.y_length < tank.y + tank.size;
			if (a_inside || b_inside || c_inside || d_inside) { 
				bull.is_draw = false;
			}
			
		}
	}
}

/*type_of_movement:
	вверх - 1
	вниз - 2
	влево - 3
	вправо - 4*/

void check_blocks(bullet& bull) {
	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] != 's')
				continue;
			int block_x = j * 100;
			int block_y = i * 100;
			switch (bull.type_of_movement) {
			case 1:
				if (bull.x + bull.x_length > block_x && bull.x < block_x + 100 && bull.y <= block_y + 100 && bull.y + bull.y_length > block_y) {
					bull.is_draw = false;
					TileMap[i][j] = ' ';
				}
				break;
			case 2:
				if (bull.x + bull.x_length > block_x && bull.x < block_x + 100 && bull.y + bull.y_length >= block_y && bull.y + bull.y_length < block_y + 100) {
					bull.is_draw = false;
					TileMap[i][j] = ' ';
				}
				break;
			case 3:
				if (bull.y + bull.y_length > block_y && bull.y < block_y + 100 && bull.x <= block_x + 100 && bull.x + bull.x_length > block_x) {
					bull.is_draw = false;
					TileMap[i][j] = ' ';
				}
				break;
			case 4:
				if (bull.y + bull.y_length > block_y && bull.y < block_y + 100 && bull.x + bull.x_length >= block_x && bull.x + bull.x_length < block_x + 100) {
					bull.is_draw = false;
					TileMap[i][j] = ' ';
				}
				break;
			}

		}
	}
}
