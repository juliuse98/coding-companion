#include <vector>

class Environment{
public:
	Environment(int width, int height);
	void setBlock(int x, int y, char block);
	char  getBlock(int x, int y);
	int getW(){ return m_width;};
	int getH(){ return m_height;};
	void update();
private:
	int m_width;
	int m_height;
	std::vector<char> m_grid;
	int  posToIndex(int x, int y);
	void moveBlock(int x, int y, int newX, int newY);
	bool isBlockAir(int x, int y);
};
