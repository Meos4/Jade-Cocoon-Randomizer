#include "JCTools.hpp"

#include "fmt/format.h"

#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <limits>
#include <cstring>

int main(int argc, char** argv)
{
	try
	{
		fmt::print("Jade Cocoon Unpacker / Repacker v1.2.0 by Meos\n\n");

		if (argc < 2)
		{
			fmt::print("0. Unpacker\n1. Repacker\n\n");

			char ch;
			std::cin >> ch;

			fmt::print("\n");

			const auto currentPath{ std::filesystem::current_path() };

			if (ch == '0')
			{
				JCTools::unpacker(currentPath, currentPath, currentPath);
			}
			else if (ch == '1')
			{
				static constexpr auto repackedDirectory{ "Repacked" };
				JCTools::repacker(currentPath, currentPath, repackedDirectory, repackedDirectory);
			}
		}
		else
		{
			if (std::strcmp(argv[1], "0") == 0 && argc > 4)
			{
				JCTools::unpacker(argv[2], argv[3], argv[4]);
			}
			else if (std::strcmp(argv[1], "1") == 0 && argc > 5)
			{
				JCTools::repacker(argv[2], argv[3], argv[4], argv[5]);
			}
			else
			{
				throw std::runtime_error
				{ 
					"Invalid arguments\n" 
					"Unpacker arguments: [0] [PSX.EXE path] [DATA.001 path] [Unpacked files path]\n"
					"Repacker arguments: [1] [PSX.EXE path] [Unpacked files path] [Repacked PSX.EXE path] [Repacked DATA.001 path]\n" 
				};
			}
		}
	}
	catch (const std::exception& e)
	{
		fmt::print("Error: {}", e.what());
	}

	if (argc < 2)
	{
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
}