#include "WorldGenerator.h"
#include <time.h>
#include "NoiseGenerator.h"

float random_between_two_float(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

std::vector<std::vector<Chunk>> WorldGenerator::generateWorld(int dimension)
{
	srand(time(0));
	NoiseGenerator noiseGenerator(rand(), 45, 55, dimension * Chunk::CHUNKSIZE);
	std::vector<std::vector<Chunk>> world = std::vector<std::vector<Chunk>>(dimension);
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			world[i].push_back(Chunk(i, j));
			for (int x = 0; x < Chunk::CHUNKSIZE; x++)
			{
				for (int z = 0; z < Chunk::CHUNKSIZE; z++)
				{
					float random = random_between_two_float(0, 100);
					int height = noiseGenerator.getNoise(
						world[i][j].getX() * Chunk::CHUNKSIZE + x, 
						world[i][j].getZ() * Chunk::CHUNKSIZE + z
					);
					for (int y = 0; y < height; y++)
					{
						//Block currentBlock = world[i][j].getBlock(x, y, z);
						if (y == 0) {
							world[i][j].getBlock(x, y, z).setBlockType(BEDROCK);
						}
						else if (y < 45) {
							world[i][j].getBlock(x, y, z).setBlockType(STONE);
						}
						else if (y < height - 1) {
							world[i][j].getBlock(x, y, z).setBlockType(DIRT);
						}
						else {
							world[i][j].getBlock(x, y, z).setBlockType(GRASS);
						}
					}
					if (random < 2) {
						world[i][j].getBlock(x, height, z).setBlockType(PUMPKIN);
					}
				}
			}
		}
	}
	return world;
}
