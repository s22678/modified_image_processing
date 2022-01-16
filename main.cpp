#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <algorithm>

#include "args.h"
#include "Image.h"

// funkcja odpowiedzialna za przetworzenie wszystkich instrukcji
// znajdujacych sie w wektorze
void GenerateImage(std::vector<std::string>& instructions)
{
    Image* image;

    int i = 0;

    while (i < instructions.size())
    {
        std::string command = instructions[i++];

        if (command == ARG_STR_INPUT)
        {
            image = new Image(instructions[i++]);
        }
        if (command == ARG_INT_RESOLUTION)
        {
            int x = stoi(instructions[i++]);
            int y = stoi(instructions[i++]);
           
            image->ResizeImage(x, y); 
        }
        else if (command == ARG_INT_BINARY_IMAGE)
        {
            int threshold = stoi(instructions[i++]);

            image->BinaryImage(threshold);
        }
        else if (command == ARG_BOOL_NEGATIVE)
        {
            image->NegativeImage();
        }
        else if (command == ARG_INT_ROTATE_IMAGE)
        { 
            int angle = stoi(instructions[i++]);

            image->RotateImage(angle);
        }
        else if (command == ARG_BOOL_GRADIENT_IMAGE)
        {
            std::string method = instructions[i++];
            image->GradientImage(method);
        }
        else if (command == ARG_BOOL_REDUCE_NOISE)
        { 
             image->ReduceNoise();
        }
        else if (command == ARG_BOOL_APPLY_BLUR)
        {
            std::string method = instructions[i++];
            image->ApplyBlur(method);
        }
        else if (command == ARG_STR_OUTPUT)
        {  
            image->SaveImage(instructions[i++]);
        }
		else if (command == ARG_BOOL_IMAGE_DILATION)
		{
			image->DilateImage();
		}
		else if (command == ARG_BOOL_IMAGE_EROSION)
		{
            image->ErodeImage();
		}
    }

    if (image != nullptr)
    {
        delete image;
        image = 0;
    }
}

int main(int argc, char* argv[])
{
    // wektor ma za zadanie przechowac instrukcje z danymi podane
    // w konsoli przez uzytkownika
    // w takiej kolejnosci w jakiej zostaly wprowadzone do konsoli
    std::vector<std::string> instructions;

    Args args(instructions);
    args.parse(argc, argv); // parsujemy tekst podany w konsoli na pojedyncze polecenia i dane
                            // informacje te laduja w wektorze instructions

    // jesli uzytkownik wprowadzil poprawne polecenie, ponizszy kod zostanie wykonany
    if (args.validate())
    {
        std::cout << "Trwa wykonywanie operacji na obrazie..." << std::endl;

        GenerateImage(instructions); // generujemy obraz wedlug podanych polecen
        return 0;
    } 

    std::cout << "Polecenia nie zostaly wykonane" << std::endl;
    return 0;
}