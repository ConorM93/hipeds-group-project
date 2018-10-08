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

int main(int argc, char * argv[]) try
{	
	// Declare pointcloud object, for calculating pointclouds and texture mappings
	rs2::pointcloud pc;
	// We want the points object to be persistent so we can display the last cloud when a frame drops
	rs2::points points;
	// Declare RealSense pipeline, encapsulating the actual device and sensors
	rs2::pipeline pipe;

	// Start streaming with default recommended configuration
	pipe.start();

	// Capture 30 frames to give autoexposure, etc. a chance to settle
	std::cout << "[+] Tuning the camera..." << std::endl;

	for(auto i = 0; i < 4; i++)
	{
		for (auto k = 0; k < 50; ++k)
			pipe.wait_for_frames();

		std::cout << "[+] Exporting PLY "<< i+1 <<"/4..." << std::endl;
		// Wait for the next set of frames from the camera
		auto frames = pipe.wait_for_frames();
		auto depth = frames.get_depth_frame();

		// Generate the pointcloud and texture mappings
		points = pc.calculate(depth);

		std::stringstream filename;
		filename << "ply_results/out_ply_" << i << ".ply";
		points.export_to_ply(filename.str(), frames);
		system("PAUSE");
	}
	// Wait for the next set of frames from the camera. Now that autoexposure, etc.
	// has settled, we will write these to disk
	/*for (auto&& frame : pipe.wait_for_frames())
	{
		std::cout << a++ << std::endl;
		

		
		// We can only save video frames as pngs, so we skip the rest
		if (auto vf = frame.as<rs2::video_frame>())
		{
			auto stream = frame.get_profile().stream_type();
			// Use the colorizer to get an rgb image for the depth stream
			if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);

			 Write images to disk
			std::stringstream png_file;
			png_file << "rs-save-to-disk-output-" << vf.get_profile().stream_name() << ".png";
			stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
				vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
			std::cout << "Saved " << png_file.str() << std::endl;
			
			
		}*/

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
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
