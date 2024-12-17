#include "pch.h"

Texture::Texture(const char* filename)
{
    loadPNGFile(filename);
}

void Texture::AssignTextureToDest(glm::vec3*& dest)
{
    dest = m_pImage.get();
}

void Texture::LoadTexture(const char* filename)
{
	loadPNGFile(filename);
}

void Texture::loadPNGFile(const char* filename)
{
    std::vector<unsigned char> pngData; // Store the loaded PNG data

    // load the PNG file
    unsigned int error = lodepng::load_file(pngData, filename);

    if (error)
    {
        std::cerr << "Error loading PNG file: " << lodepng_error_text(error) << std::endl;
        return;
    }

    // decode the PNG data
    std::vector<unsigned char> image;
    unsigned int width, height;

    error = lodepng::decode(image, width, height, pngData);

    if (error)
    {
        std::cerr << "Error decoding PNG data: " << lodepng_error_text(error) << std::endl;
        return;
    }

    // check that the image has 3 or 4 channels (RGB or RGBA)
    if (image.size() % 3 != 0 && image.size() % 4 != 0)
    {
        std::cerr << "Unsupported PNG image format: " << filename << std::endl;
        return;
    }

    // determine the number of channels (3 for RGB, 4 for RGBA)
    int channels = (image.size() % 4 == 0) ? 4 : 3;

    m_width = width;
    m_height = height;
    m_bytesPerPix = channels;
    m_bitDepth = 8; // Assuming 8-bit per channel

    m_size = m_width * m_height;
    m_pImage = std::unique_ptr<glm::vec3>(new glm::vec3[m_size]);
    m_pTexcoord = std::unique_ptr<glm::vec2>(new glm::vec2[m_size]);

    // convert the image data to glm::vec3 format
    for (size_t i = 0; i < m_size; ++i)
    {
        glm::vec3 vertex;
        vertex.x = static_cast<float>(image[i * channels]) / 255.0f;
        vertex.y = static_cast<float>(image[i * channels + 1]) / 255.0f;
        vertex.z = static_cast<float>(image[i * channels + 2]) / 255.0f;
        m_pImage.get()[i] = vertex;
    }
}