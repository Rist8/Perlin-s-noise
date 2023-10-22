#include <cmath>
#include <vector>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

class PerlinNoise {
	// The permutation vector
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

#endif

#include <random>
#include <algorithm>
#include <numeric>

// Initialize with the reference values for the permutation vector
PerlinNoise::PerlinNoise() {

	// Initialize the permutation vector with the reference values
	p = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
		43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
		107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)), lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;
}

double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}



//Process a binary PPM file
#include <vector>
#include <string>

#ifndef PPM_H
#define PPM_H

class ppm {
	void init();
	//info about the PPM file (height and width)
	unsigned int nr_lines;
	unsigned int nr_columns;

public:
	//arrays for storing the R,G,B values
	std::vector<unsigned char> r;
	std::vector<unsigned char> g;
	std::vector<unsigned char> b;
	//
	unsigned int height;
	unsigned int width;
	unsigned int max_col_val;
	//total number of elements (pixels)
	unsigned int size;

	ppm();
	//create a PPM object and fill it with data stored in fname 
	ppm(const std::string& fname);
	//create an "epmty" PPM image with a given width and height;the R,G,B arrays are filled with zeros
	ppm(const unsigned int _width, const unsigned int _height);
	//read the PPM image from fname
	void read(const std::string& fname);
	//write the PPM image in fname
	void write(const std::string& fname);
};

#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

//init with default values

void ppm::init() {
	width = 0;
	height = 0;
	max_col_val = 255;
}

//create a PPM object

ppm::ppm() {
	init();
}

//create a PPM object and fill it with data stored in fname 

ppm::ppm(const std::string& fname) {
	init();
	read(fname);
}

//create an "epmty" PPM image with a given width and height;the R,G,B arrays are filled with zeros

ppm::ppm(const unsigned int _width, const unsigned int _height) {
	init();
	width = _width;
	height = _height;
	nr_lines = height;
	nr_columns = width;
	size = width * height;

	// fill r, g and b with 0
	r.resize(size);
	g.resize(size);
	b.resize(size);
}

//read the PPM image from fname

void ppm::read(const std::string& fname) {
	std::ifstream inp(fname.c_str(), std::ios::in | std::ios::binary);
	if (inp.is_open()) {
		std::string line;
		std::getline(inp, line);
		if (line != "P6") {
			std::cout << "Error. Unrecognized file format." << std::endl;
			return;
		}
		std::getline(inp, line);
		while (line[0] == '#') {
			std::getline(inp, line);
		}
		std::stringstream dimensions(line);

		try {
			dimensions >> width;
			dimensions >> height;
			nr_lines = height;
			nr_columns = width;
		}
		catch (std::exception& e) {
			std::cout << "Header file format error. " << e.what() << std::endl;
			return;
		}

		std::getline(inp, line);
		std::stringstream max_val(line);
		try {
			max_val >> max_col_val;
		}
		catch (std::exception& e) {
			std::cout << "Header file format error. " << e.what() << std::endl;
			return;
		}

		size = width * height;

		r.reserve(size);
		g.reserve(size);
		b.reserve(size);

		char aux;
		for (unsigned int i = 0; i < size; ++i) {
			inp.read(&aux, 1);
			r[i] = (unsigned char)aux;
			inp.read(&aux, 1);
			g[i] = (unsigned char)aux;
			inp.read(&aux, 1);
			b[i] = (unsigned char)aux;
		}
	}
	else {
		std::cout << "Error. Unable to open " << fname << std::endl;
	}
	inp.close();
}

//write the PPM image in fname

void ppm::write(const std::string& fname) {
	std::ofstream inp(fname.c_str(), std::ios::out | std::ios::binary);
	if (inp.is_open()) {

		inp << "P6\n";
		inp << width;
		inp << " ";
		inp << height << "\n";
		inp << max_col_val << "\n";

		char aux;
		for (unsigned int i = 0; i < size; ++i) {
			aux = (char)r[i];
			inp.write(&aux, 1);
			aux = (char)g[i];
			inp.write(&aux, 1);
			aux = (char)b[i];
			inp.write(&aux, 1);
		}
	}
	else {
		std::cout << "Error. Unable to open " << fname << std::endl;
	}
	inp.close();
}

std::string KeyGen(unsigned long long int n, std::string seed)
{
	std::string key;
	int rand = n % 320000 + n % 623;//just for more "random" key
	n /= 20;//and this too
	for (int i = 0; i < 6; ++i)
	{
		key += int(seed[(rand % seed.length() + i) % seed.length()] ^ n + rand) % 36 + 65;// %36 + 45 - first part of restrictioning possible characters(from A to Z + 10 next characters)
		if (key[i] > 90)
			key[i] = key[i] % 90 - 1 + '0';// %90 - 1 + '0' - second part of restrictioning possible characters(if character is after Z turn it into the number)
	}
	return key;
}


int main() {
	std::string seed = "ABOBUS";
	unsigned int width = 800, height = 600;
	std::string filename = "perlinNoise";
	bool wood = 1, woodl = 0, ch = 1, color = 1;
	setlocale(LC_ALL, "Russian");
	while (!woodl) {
		std::cout << "Create image using seed(1) or ID(0)?\n";
		std::cin >> ch;
		system("cls");
		if (ch)
			std::cout << "Enter the seed...\n";
		else
			std::cout << "Enter ID...\n";
		std::getline(std::cin, seed, '\n');
		std::getline(std::cin, seed, '\n');
		system("cls");
		std::cout << "Create Perlin's noise in treelike format(0) or in standart(1)?...\n";
		std::cin >> wood;
		system("cls");
		std::cout << "Create Perlin's noise colorful(0) or black-white(1)?...\n";
		std::cin >> color;
		system("cls");
		std::cout << "Enter custom filename or press "Enter" to use standart(perlinNoise)...\n";
		std::getline(std::cin, filename, '\n');
		std::getline(std::cin, filename, '\n');
		system("cls");
		unsigned short int choice = 0;
		std::cout << "Choose from standart values(1 - 9) or enter custom(0) resolution of created image:\n1) 640 x 480\n2) 800 x 480\n3) 800 x 600\n4) 1024 x 768\n5) 1280 x 768\n6) 1280 x 1024\n7) 1440 x 1080\n8) 1920 x 1080\n9) 3840 x 2160\n";
		std::cin >> choice;
		system("cls");
		switch (choice)
		{
		case 1: width = 640, height = 480; break;
		case 2: width = 800, height = 480; break;
		case 3: width = 800, height = 600; break;
		case 4: width = 1024, height = 768; break;
		case 5: width = 1280, height = 768; break;
		case 6: width = 1280, height = 1024; break;
		case 7: width = 1440, height = 1080; break;
		case 8: width = 1920, height = 1080; break;
		case 9: width = 3840, height = 2160; break;
		case 0: std::cout << "Recommended values: width(1 - 15360), height(1 - 8640).\nEnter the width...\n";
			std::cin >> width; system("cls"); 
			std::cout << "Recommended values: width(1 - 15360), height(1 - 8640).\nEnter the height...\n";
			std::cin >> height;
			break;
		}
		system("cls");
		std::cout << "Chosen parameters:\n" << ((ch) ? "Seed: " : "ID: ") << seed << ((!wood) ? "\nWood structured format.\n" : "\nStandard format.\n") << ((!color) ? "Colorful.\n" : "Black-white.\n") << "Filename: " << filename << "\nResolution: " << width << " x " << height << "\n\nCreate with current parameters(1) or rechoose(0)?\n";
		std::cin >> woodl;
		system("cls");
	}
	std::cout << "Generating image...";
	if (filename == "")
		filename = "perlinNoise";
	filename += ".ppm";
	unsigned int seed_int = 0;
	if(ch)
		seed = KeyGen(1758956124, seed);
	for (int i = 0; i < seed.size(); ++i)
		seed_int += seed[i] * (i + 1);
	// Define the size of the image
	// Create an empty PPM image
	ppm image(width, height);
	// Create a PerlinNoise object with the reference permutation vector
	PerlinNoise pn(seed_int);
	unsigned int kk = 0;
	// Visit every pixel of the image and assign a color generated with Perlin noise
	for (unsigned int i = 0; i < height * (height / 300 - 1); i += (height / 300 - 1)) {     // y
		for (unsigned int j = 0; j < width * (width / 400 - 1); j += (width / 400 - 1)) {  // x
			double x = (double)j / ((double)width);
			double y = (double)i / ((double)height);
			// Typical Perlin noise
			double n = pn.noise(10 * x + 15, 10 * y + 15, 0.8);
			// Wood like structure
			if (!wood) {
				n = 20 * pn.noise(x, y, 0.8);
				n = n - floor(n);
			}
			// Map the values to the [0, 255] interval, for simplicity we use 
			// tones of grey
			
			if (!color) {
				n = pow(n, 2.85);
				double t = floor(255 * n);
				image.r[kk] = 0;
				if (t >= 40 && t <= 130) {
					image.g[kk] = 255 - t;
					image.b[kk] = 0;
				}
				else if (t < 40)
				{
					image.g[kk] = 0;
					image.b[kk] = 255 - t;
				}
				else
				{
					image.r[kk] = t;
					image.g[kk] = t;
					image.b[kk] = t;
				}
			}
			else
			{
				double t = floor(255 * n);
				image.r[kk] = t;
				image.g[kk] = t;
				image.b[kk] = t;
			}
			kk++;
		}
	}
	// Save the image in a binary PPM file
	system("cls");
	std::cout << "Writing in file...";
	image.write(filename);
	system("cls");
	std::cout << "Image successfully saved.\nImage ID: " << seed << '\n';
	system("pause");
}
