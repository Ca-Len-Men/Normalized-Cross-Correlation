#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <iostream>

#include <math.h>
// #include <direct>

sf::Vector2u run_ncc(sf::RenderTexture& source, sf::RenderTexture& templ)
{
	auto image_source = source.getTexture().copyToImage();
	auto image_templ = templ.getTexture().copyToImage();

	auto source_size = image_source.getSize();
	auto templ_size = image_templ.getSize();
	auto bound = source_size - templ_size + sf::Vector2u{ 1u, 1u };

	auto pixels_source = image_source.getPixelsPtr();
	auto pixels_templ = image_templ.getPixelsPtr();

	// float* results = new float[bound.x * bound.y];
	
	int templ_sum = 0;
	float templ_squared_sum = 0;
	for (unsigned int ty = 0; ty < templ_size.y; ++ty)
		for (unsigned int tx = 0; tx < templ_size.x; ++tx)
		{
			auto pixel_r = pixels_templ[4 * (ty * templ_size.x + tx)];	// R channel
			templ_sum += pixel_r;
		}

	float avg_pixel_templ = templ_sum * 1.0f / (templ_size.x * templ_size.y);

	for (unsigned int ty = 0; ty < templ_size.y; ++ty)
		for (unsigned int tx = 0; tx < templ_size.x; ++tx)
		{
			auto pixel_r = pixels_templ[4 * (ty * templ_size.x + tx)];	// R channel
			float pixel_after_subtract_avg = pixel_r - avg_pixel_templ;
			templ_squared_sum += pixel_after_subtract_avg * pixel_after_subtract_avg;
		}

	float templ_sqrt_squared_sum = sqrtf(templ_squared_sum);

	// Normalized Cross-Correlation
	sf::Vector2u matched_position{ 0u, 0u };
	float max_value = -1.f;
	
	for(unsigned int y = 0; y < bound.y; ++y)
		for (unsigned int x = 0; x < bound.x; ++x)
		{
			int source_sum = 0;
			// Calculating average of source sub bound
			for (unsigned int ty = 0; ty < templ_size.y; ++ty)
				for (unsigned int tx = 0; tx < templ_size.x; ++tx)
				{
					unsigned int mapping_to_source_index = (y + ty) * source_size.x + (x + tx);
					auto f_xy_txty = pixels_source[4 * mapping_to_source_index];	// R channel
					source_sum += f_xy_txty;
				}

			float avg_pixel_source = source_sum * 1.0f / (templ_size.x * templ_size.y);

			float dot_sum = 0;
			float source_squared_sum = 0;

			for (unsigned int ty = 0; ty < templ_size.y; ++ty)
				for (unsigned int tx = 0; tx < templ_size.x; ++tx)
				{
					unsigned int mapping_to_source_index = (y + ty) * source_size.x + (x + tx);
					auto f_xy_txty = pixels_source[4 * mapping_to_source_index];	// R channel
					auto t_txty = pixels_templ[4 * (ty * templ_size.x + tx)];		// R channel

					float pixel_templ_after_subtract_avg = t_txty - avg_pixel_templ;
					float pixel_source_after_subtract_avg = f_xy_txty - avg_pixel_source;

					source_squared_sum += pixel_source_after_subtract_avg * pixel_source_after_subtract_avg;
					dot_sum += pixel_source_after_subtract_avg * pixel_templ_after_subtract_avg;
				}

			float result = dot_sum / (sqrtf(float(source_squared_sum)) * templ_sqrt_squared_sum);
			if (max_value < result)
			{
				max_value = result;
				matched_position = sf::Vector2u{x, y};
			}
		}

	std::cout << "Max value: " << max_value << std::endl;
	return matched_position;
}




//inline float unpackRGBAtoFloat(const sf::Uint8* pointer) {
//	float r = pointer[0] / 255.0f;
//	float g = pointer[1] / 255.0f;
//	float b = pointer[2] / 255.0f;
//	float a = pointer[3] / 255.0f;
//	return r + g / 256.0f + b / (256.0f * 256.0f) + a / (256.0f * 256.0f * 256.0f);
//}
//
//sf::Vector2u matched_position(const sf::Image& image_ncc_result)
//{
//	auto pixels = image_ncc_result.getPixelsPtr();
//	auto size = image_ncc_result.getSize();
//
//	float max_value = -999999.0;
//	sf::Vector2u max_position = { 0u, 0u };
//
//	for (unsigned int y = 0; y < size.y; ++y)
//		for (unsigned int x = 0; x < size.x; ++x)
//		{
//			auto pixel = pixels + 4 * (y * size.x + x);
//
//			float value = unpackRGBAtoFloat(pixel);
//
//			if (max_value < value)
//			{
//				max_value = value;
//				max_position = { x, y };
//			}
//		}
//
//	// std::cout << "Max value: " << max_value << std::endl;
//	return max_position;
//}