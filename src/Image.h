#ifndef IMAGE_H
#define IMAGE_H

#include "includes.h"

class Image
{
	friend std::ostream& operator<<(std::ostream& out, const Image& image);

public:
	Image(int width, int height);
	Image(int width, int height, const glm::vec3& fill);

	glm::vec3& operator()(int x, int y);
	const glm::vec3& operator()(int x, int y) const;

	int getWidth() const;
	int getHeight() const;

	void savePpm(const std::string& path) const;
	void saveBmp(const std::string& path) const;

private:
	int width, height;
	std::vector<glm::vec3> pixels;
};

#endif