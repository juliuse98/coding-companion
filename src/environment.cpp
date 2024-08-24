#include <cstdlib>
#include <environment.h>

Environment::Environment(int width, int height) : m_width(width), m_height(height) {
	m_grid = std::vector<char>(m_width * m_height, ' ');
}

char Environment::getBlock(int x, int y){
	int index = Environment::posToIndex(x, y);	
	return m_grid.at(index);
}

void Environment::setBlock(int x, int y, char block){
	int index = posToIndex(x, y);	
	m_grid[index] = block; 
}

int Environment::posToIndex(int x, int y){
	return y * m_width + x;
}
bool Environment::isBlockAir(int x, int y){
	return getBlock(x,y) == ' ';
}

enum Direction{
	NO_DIRECTION,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

void Environment::update(){
	for (int y = m_height - 2; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			char block = getBlock(x, y);
			if(block == ' '){
				continue;
			}
			char blockBelow = getBlock(x, y+1);
			if(blockBelow == ' '){
				setBlock(x, y + 1, block);
				setBlock(x, y, ' ');
				continue;
			}

			int sideBias = rand() % 2;
			Direction d = NO_DIRECTION;
			if(sideBias == 0){
				if(x > 0 && isBlockAir(x - 1, y) && isBlockAir(x - 1, y + 1)){
					d = DOWN_LEFT;
				}else if(x < m_width - 1 && isBlockAir(x + 1, y) && isBlockAir(x + 1, y + 1)){
					d = DOWN_RIGHT;
				}
			}else{
				if(x < m_width - 1 && isBlockAir(x + 1, y) && isBlockAir(x + 1, y + 1)){
					d = DOWN_RIGHT;
				}else if(x > 0 && isBlockAir(x - 1, y) && isBlockAir(x - 1, y + 1)){
					d = DOWN_LEFT;
				}
			}
			if(d == NO_DIRECTION){
			continue;
			}else if(d == DOWN_RIGHT){
				setBlock(x+1, y + 1, block);
				setBlock(x, y, ' ');
				continue;
			}else if(d == DOWN_LEFT){
				setBlock(x-1, y + 1, block);
				setBlock(x, y, ' ');
				continue;
			}
		}
	}
}
