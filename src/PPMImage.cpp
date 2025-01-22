#include "PPMImage.h"


PPMImage::PPMImage(void)
{
	image = NULL;
}


PPMImage::~PPMImage(void)
{
	if(image != NULL)
		delete [] image;
}

void PPMImage::AllocateMemory(int width, int height)
{
	this->width = width;
	this->height = height;
	image = new unsigned char[width * height * 3];

	cout << width << "x" << height << endl;
}


void PPMImage::ReadFile(string fileName)
{
	file.open(fileName, ios_base::binary);

	if (!file.is_open())
	{
		cout << fileName << " couldn't be opened.\n";
		system("pause");
		exit(1);
	}
	
	string fileType;
	file >> fileType;

	CheckComment();
	file >> width >> height;
	image = new unsigned char [width * height * 3];

	int maxValue;

	file >> maxValue;

	if (fileType == "P3")
	{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int k = 0; k < 3; k++)
			{
				string temp;
				file >> temp;
				image[(i * width + j) * 3 + k] = (unsigned char) atoi(temp.c_str());
			}
	}

	else if (fileType == "P6")
	{
		file.getline(buf, 127);
		file.read((char *) image, height * width * 3);
	}
}

void PPMImage::WriteFile(string fileName, string fileType)
{
	// Read and understand the code.
	if (fileType == "P6")
	{
		outFile.open(fileName, ios_base::binary);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}

		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		outFile.write((char*)image, height * width * 3);
	}
	else if (fileType == "P3")
	{
		outFile.open(fileName);

		if (!outFile.is_open())
		{
			cout << fileName << " couldn't be opened.\n";
			system("pause");
			exit(1);
		}

		outFile << fileType << endl;
		outFile << width << " " << height << endl;
		outFile << 255 << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width * 3; j++)
			{
				outFile << (int)image[i * width * 3 + j] << " ";
			}
			outFile << endl;
		}
	}
	outFile.close();
}

void PPMImage::CheckComment()
{
	c = file.peek();

	while (c == '#' || c == '\n' || c == ' ')
	{
		file.getline(buf, 127);
		c = file.peek();
	}
}

void PPMImage::VerticalFlip()
{
	// Do a vertical flip.  You need to use the help variable declared above.
	// Also do dynamic memory allocation for the variable based on the image size.
	
	unsigned char* flippedImage; /// 1D array that stores the flipped image

	// Dynamic memory allocation
	flippedImage = new unsigned char[width * height * 3];

	// Loop to get the data for the image
	for (int i = 0; i < height; i++)
	{	
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < 3; k++) // Looping through each RGB channel
			{
				// Flip the last scanline with the first
				flippedImage[((height - 1 - i) * width + j) * 3 + k] = image[(i * width + j) * 3 + k];
			}
		}
	}
	// Copy the flipped image into image
	image = flippedImage;

}