// PlyExtraxtor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering
#include "pch.h"


#include <algorithm>            // std::min, std::max
#include <fstream>              // File IO
#include <iostream>             // Terminal IO
#include <sstream>              // Stringstreams
#include <iostream>
#include <string>

constexpr auto SHARED_FOLDER = "C:\\Users\\oculus\\Google Drive\\";

void session_init(std::string & session_name);

int main(int argc, char * argv[]) try
{	
	std::string session_name;
	session_init(session_name);

	// Declare pointcloud object, for calculating pointclouds and texture mappings
	rs2::pointcloud pc;
	// We want the points object to be persistent so we can display the last cloud when a frame drops
	rs2::points points;
	// Declare RealSense pipeline, encapsulating the actual device and sensors
	rs2::pipeline pipe;

	// Start streaming with default recommended configuration
	pipe.start();

	std::stringstream filepath;
	std::string filename;


	filepath << SHARED_FOLDER << session_name;

	if (CreateDirectory(filepath.str().c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		std::cout << "[INFO] Pictures will be saved in " << filepath.str() << std::endl;
	}
	else
	{
		std::cerr << "[ERROR] Failed to create saving directory";
	}


	for(auto i = 0; i < 4; i++)
	{

		std::cout << "[INFO] Tuning the camera..." << std::endl;

		for (auto k = 0; k < 100; k++)
			pipe.wait_for_frames();

		std::cout << "[INFO] Taking a shoot " << i + 1 << "/4. Say cheese....... " << std::endl;
		
		// Wait for the next set of frames from the camera
		auto frames = pipe.wait_for_frames();
		auto depth = frames.get_depth_frame();

		// Generate the pointcloud and texture mappings
		points = pc.calculate(depth);

		filename = filepath.str() + "\\pic_" + std::to_string(i) + ".ply";

		points.export_to_ply(filename, frames);

		std::cout << "[INFO] PLY Saved in " << filename << std::endl;
		std::cout << "[INFO] Press any key to get the next PLY" << std::endl;
		system("PAUSE");
	}
	
	return EXIT_SUCCESS;
}

	

catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception & e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

void session_init(std::string & session_name)
{
	std::cout << "[INFO] Please, name this session: ";
	std::cin >> session_name;
	std::cout << std::endl;
}

