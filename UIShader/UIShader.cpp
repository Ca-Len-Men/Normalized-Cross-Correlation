#pragma warning(disable: 26495)

#include "template_matching.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <iostream>
#include <string>
#include <chrono>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "GUIShader");
    window.setFramerateLimit(45);

    sf::Texture tt_source;
    tt_source.loadFromFile("caheo.jpg");
    sf::Vector2u source_size = tt_source.getSize();
    //image.setSmooth(true);
    sf::Sprite s_source;
    s_source.setTexture(tt_source);


    sf::Texture tt_template;
    tt_template.loadFromFile("template_3.png");
    sf::Vector2u template_size = tt_template.getSize();
    //image.setSmooth(true);
    sf::Sprite s_template;
    s_template.setTexture(tt_template);


    sf::Shader glsl_gray;
    if (!glsl_gray.loadFromFile("shader/gray.frag", sf::Shader::Type::Fragment))
        return -10;
    glsl_gray.setUniform("image", tt_source);
    glsl_gray.setUniform("image_size", sf::Glsl::Vec2(source_size));

    // << Áp dụng GLSL Gray cho "source"
    sf::RenderTexture rtt_source_gray;
    rtt_source_gray.create(source_size.x, source_size.y);
    rtt_source_gray.draw(sf::RectangleShape((sf::Vector2f)source_size), &glsl_gray);
    rtt_source_gray.display();

    sf::Sprite s_source_gray(rtt_source_gray.getTexture());
    s_source_gray.setPosition(sf::Vector2f(0.f, (float)source_size.y));
    s_source_gray.setScale(1.f, -1.f);
    // >>

    // << Áp dụng GLSL Gray cho "template"
    glsl_gray.setUniform("image", tt_template);
    glsl_gray.setUniform("image_size", sf::Glsl::Vec2(template_size));

    sf::RenderTexture rtt_template_gray;
    rtt_template_gray.create(template_size.x, template_size.y);
    rtt_template_gray.draw(sf::RectangleShape((sf::Vector2f)template_size), &glsl_gray);
    rtt_template_gray.display();

    sf::Sprite s_template_gray(rtt_template_gray.getTexture());
    s_template_gray.setPosition(sf::Vector2f(50.f, 800.f + template_size.y));
    s_template_gray.setScale(1.f, -1.f);
    // >>

    // << Loader NCC GLSL
    //sf::Shader glsl_normalized_cross_correlation;
    //if (!glsl_normalized_cross_correlation.loadFromFile("shader/normalized_cross_correlation.frag", sf::Shader::Type::Fragment))
    //    return -10;
    //glsl_normalized_cross_correlation.setUniform("source", rtt_source_gray.getTexture());
    //glsl_normalized_cross_correlation.setUniform("source_size", sf::Glsl::Vec2(source_size));
    //glsl_normalized_cross_correlation.setUniform("templ", rtt_template_gray.getTexture());
    //glsl_normalized_cross_correlation.setUniform("template_size", sf::Glsl::Vec2(template_size));
    //// >>

    //// << Áp dụng GLSL Normalized Cross-Correlation
    //sf::RenderTexture rtt_template_matching_result;
    //sf::Vector2u boundary = source_size - template_size + sf::Vector2u{ 1u, 1u };
    //rtt_template_matching_result.create(boundary.x, boundary.y);
    //rtt_template_matching_result.draw(sf::RectangleShape((sf::Vector2f)boundary), &glsl_normalized_cross_correlation);
    //rtt_template_matching_result.display();

    // Run NCC
    auto start = std::chrono::high_resolution_clock::now();

    auto matched_position = run_ncc(rtt_source_gray, rtt_template_gray);
    // sf::Rect<unsigned int> matched_bound(matched_position, template_size);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Time executed: " << static_cast<long long>(duration.count()) << std::endl;
    std::cout << "Matched at: (" << matched_position.x << ", " << matched_position.y << ")\n";



    sf::RectangleShape rectangle(sf::Vector2f((float)template_size.x, (float)template_size.y));
    rectangle.setPosition((float)matched_position.x, (float)matched_position.y);
    rectangle.setFillColor(sf::Color{0, 0, 0, 0});
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(sf::Color::Red);

    /*sf::Sprite s_template_matching(rtt_template_matching_result.getTexture());
    s_template_matching.setPosition(sf::Vector2f(800.f, source_size.y));
    s_template_matching.setScale(1.f, -1.f);*/
    // >>

   /* sf::RenderTexture rtt_results_of_template_matching;
    r_texture.create(image_size.x, image_size.y);*/

    /*sf::Shader mix;
    if (!mix.loadFromFile("mix-light.frag", sf::Shader::Type::Fragment))
        return -10;

    sf::RenderTexture render_texture2;
    render_texture2.create(300u, 300u);

    mix.setUniform("resolution", sf::Glsl::Vec2(render_texture2.getSize()));
    mix.setUniform("pcolor", sf::Glsl::Vec4(sf::Color(81, 13, 113)));

    sf::RectangleShape rectangle2((sf::Vector2f)render_texture2.getSize());

    render_texture2.draw(rectangle2, &mix);
    render_texture2.display();

    sf::Sprite sprite2(render_texture2.getTexture());
    sprite2.setPosition(500.f, 100.f);*/


    /*sf::Font my_font;
    my_font.loadFromFile("Texturina-Medium.ttf");

    sf::Vector2f topleft(100.f, 100.f);

    sf::Text text;
    text.setFont(my_font);
    text.setCharacterSize(16u);
    text.setString(L"SlimeGUI");
    text.setPosition(sf::Vector2f(topleft.x + 16u, topleft.y + 8u));
    text.setFillColor(sf::Color(0, 0, 255, 255));

    auto local_bound = text.getLocalBounds();

    sf::RectangleShape bg;
    bg.setPosition(topleft);
    bg.setSize(sf::Vector2f(local_bound.width + 32u, 32u));
    bg.setFillColor(sf::Color(76, 124, 240, 255));*/

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(128, 184, 241, 255));
        
        
        // window.draw(sprite_fruits);
        window.draw(s_source_gray);
        window.draw(s_template_gray);
        // window.draw(s_template_matching);

        window.draw(rectangle);

        window.display();
    }

	return 0;
}