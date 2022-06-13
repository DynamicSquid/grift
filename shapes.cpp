void line(int x0, int y0, int x1, int y1, Colour const& color, int weight)
{
	auto round = [](int x) { return std::floor(x + 0.5); };
	auto fpart = [](int x) { return x - floor(x); };
	auto rfpart = [&](int x) { return 1 - fpart(x); };
	auto plot = [&](int x, int y, int a) {
		SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, a * color.a);
		SDL_RenderDrawPoint(renderer.get(), x, y);
	};

	bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    
    if (steep)
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
    }
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
    
	int dx = x1 - x0;
	int dy = y1 - y0;

	int gradient;
	if (dx == 0)
		gradient = 1.0;
	else
		gradient = dy / dx;

    // handle first endpoint
	int xend = std::round(x0);
	int yend = y0 + gradient * (xend - x0);
	int xgap = rfpart(x0 + 0.5);
	int xpxl1 = xend;
	int ypxl1 = std::floor(yend);
	if (steep)
	{
		plot(ypxl1, xpxl1, rfpart(yend) * xgap);
		plot(ypxl1 + 1, xpxl1, fpart(yend) * xgap);
	}
	else
	{
		plot(xpxl1, ypxl1, rfpart(yend) * xgap);
		plot(xpxl1, ypxl1 + 1, fpart(yend) * xgap);
	}

	int intery = yend + gradient;
    
    // handle second endpoint
	xend = round(x1);
	yend = y1 + gradient * (xend - x1);
	xgap = fpart(x1 + 0.5);
	int xpxl2 = xend;
	int ypxl2 = std::floor(yend);
	if (steep)
	{
		plot(ypxl2, xpxl2, rfpart(yend) * xgap);
		plot(ypxl2 + 1, xpxl2, fpart(yend) * xgap);
	}
	else
	{
		plot(xpxl2, ypxl2, rfpart(yend) * xgap);
		plot(xpxl2, ypxl2 + 1, fpart(yend) * xgap);
	}
    
	if (steep)
	{
		for (int i = xpxl1 + 1; i <= xpxl2 - 1; ++i)
		{
			plot(std::floor(intery), i, rfpart(intery));
			plot(std::floor(intery) + 1, i, fpart(intery));
			intery = intery + gradient;
		}
	}
	else
	{
		for (int i = xpxl1 + 1; i <= xpxl2 - 1; ++i)
		{
			plot(i, std::floor(intery), rfpart(intery));
			plot(i, std::floor(intery) + 1, fpart(intery));
			intery = intery + gradient;
		}
	}
}

void text(std::string const& text, Color const& colour, std::string const& font, int size,
	int x, int y, Align alignment)
{
	sdl2::font_ptr ttf_font(TTF_OpenFont(font.c_str(), size));
	sdl2::surface_ptr text_surface(TTF_RenderText_Solid(ttf_font.get(), text.c_str(), colour));
	sdl2::texture_ptr text_texture(SDL_CreateTextureFromSurface(renderer.get(), text_surface.get()));

	SDL_Rect text_rect;
	TTF_SizeText(ttf_font.get(), text.c_str(), &text_rect.w, &text_rect.h);

	switch (alignment)
	{
	case sdl2::Align::LEFT:
		text_rect.x = x;
		text_rect.y = y;
		break;
	case sdl2::Align::CENTER:
		text_rect.x = x - (text_rect.w / 2);
		text_rect.y = y;
		break;
	case sdl2::Align::RIGHT:
		text_rect.x = x - (text_rect.w);
		text_rect.y = y;
		break;
	default:
		std::cout << "error - not align\n";
	};
	
	SDL_RenderCopy(renderer.get(), text_texture.get(), NULL, &text_rect);
}

void circle(int const x, int const y, int const r, Color const& fill,
	Color const& stroke, int weight, Align alignment, CircleQuad quad)
{
	int w1, w2, h1, h2;
	SDL_SetRenderDrawColor(renderer.get(), fill.r, fill.g, fill.b, fill.a);
	switch (quad)
	{
	case sdl2::CircleQuad::ALL:
		w1 = 0;
		w2 = r * 2;
		h1 = 0;
		h2 = r * 2;
		break;
	case sdl2::CircleQuad::TOP_LEFT:
		w1 = r;
		w2 = r * 2 + weight;
		h1 = r;
		h2 = r * 2 + weight;
		break;
	case sdl2::CircleQuad::TOP_RIGHT:
		w1 = -weight;
		w2 = r;
		h1 = r;
		h2 = r * 2 + weight;
		break;
	case sdl2::CircleQuad::BOTTOM_LEFT:
		w1 = r;
		w2 = r * 2 + weight;
		h1 = -weight;
		h2 = r;
		break;
	case sdl2::CircleQuad::BOTTOM_RIGHT:
		w1 = -weight;
		w2 = r;
		h1 = -weight;
		h2 = r;
		break;
	default:
		throw std::runtime_error("unhandled case");
	}

	for (int w = w1; w < w2; ++w)
	{
		for (int h = h1; h < h2; ++h)
		{
			double dx = r - w;
			double dy = r - h;
			double d = std::sqrt((dx * dx) + (dy * dy)) - r;
			if (d < -1.5)
				SDL_SetRenderDrawColor(renderer.get(), fill.r, fill.g, fill.b, fill.a);
			else if (d <= -1)
				SDL_SetRenderDrawColor(renderer.get(), fill.r, fill.g, fill.b, fill.a / 3);
			else if (d <= -0.5)
				SDL_SetRenderDrawColor(renderer.get(), stroke.r, stroke.g, stroke.b, stroke.a / 2);
			else if (d <= 0)
				SDL_SetRenderDrawColor(renderer.get(), stroke.r, stroke.g, stroke.b, stroke.a);
			else if (d <= weight - 1)
				SDL_SetRenderDrawColor(renderer.get(), stroke.r, stroke.g, stroke.b, stroke.a / 2);
			else if (d <= weight)
				SDL_SetRenderDrawColor(renderer.get(), stroke.r, stroke.g, stroke.b, stroke.a / 3);
			else
				continue;

			SDL_RenderDrawPoint(renderer.get(), x + dx, y + dy);
		}
	}
}

void rect(int x, int y, int w, int h, Colour const& fill,
	Colour const& stroke, Align alignment)
{
	SDL_Rect rect{ 0, 0, w, h };
	switch (alignment)
	{
	case sdl2::Align::LEFT:
		rect.x = x;
		rect.y = y;
		break;
	case sdl2::Align::CENTER:
		rect.x = x - (rect.w / 2);
		rect.y = y - (rect.h / 2);
		break;
	case sdl2::Align::RIGHT:
		rect.x = x - (rect.w);
		rect.y = y;
		break;
	default:
		throw std::runtime_error("unhandled statement");
	};

	SDL_SetRenderDrawColor(renderer.get(), fill.r, fill.g, fill.b, fill.a);
	SDL_RenderFillRect(renderer.get(), &rect);

	SDL_SetRenderDrawColor(renderer.get(), stroke.r, stroke.g, stroke.b, stroke.a);
	SDL_RenderDrawRect(renderer.get(), &rect);
}

void Screen::rect(int x, int y, int w, int h, int r, SDL_Color const& fill,
	SDL_Color const& stroke, sdl2::Align alignment)
{
	if (r > w / 2 || r > h / 2)
		throw std::runtime_error("radius too large");

	switch (alignment)
	{
	case sdl2::Align::CENTER: {
		rect(x, y, w - (r * 2), h, fill, sdl2::clr_clear, alignment);
		rect(x - (w / 2) + (r / 2), y, r, h - (r * 2), fill, sdl2::clr_clear, alignment);
		rect(x + (w / 2) - (r / 2) - 1, y, r + 0, h - (r * 2), fill, sdl2::clr_clear, alignment);
		
		circle(x - ((w / 2) - r) - 1, y - ((h / 2) - r) - 1, r + 1, fill, stroke, 1, alignment, sdl2::CircleQuad::TOP_LEFT);
		circle(x + ((w / 2) - r) - 1, y - ((h / 2) - r) - 1, r + 1, fill, stroke, 1, alignment, sdl2::CircleQuad::TOP_RIGHT);
		circle(x - ((w / 2) - r) - 1, y + ((h / 2) - r) - 1, r + 1, fill, stroke, 1, alignment, sdl2::CircleQuad::BOTTOM_LEFT);
		circle(x + ((w / 2) - r) - 1, y + ((h / 2) - r) - 1, r + 1, fill, stroke, 1, alignment, sdl2::CircleQuad::BOTTOM_RIGHT);

		line(x - (w / 2) - 2, y - (h / 2) + r, x - (w / 2) - 2, y + (h / 2) - r, stroke, 1);
		line(x + (w / 2), y - (h / 2) + r, x + (w / 2), y + (h / 2) - r, stroke, 1);
		line(x + (w / 2) - 1, y - (h / 2) + r, x + (w / 2) - 1, y + (h / 2) - r, { stroke.r, stroke.g, stroke.b,
			(unsigned char)stroke.a / 3 }, 1);
		line(x - (w / 2) + r, y - (h / 2) - 2, x + (w / 2) - r, y - (h / 2) - 2, stroke, 1);
		line(x - (w / 2) + r, y + (h / 2), x + (w / 2) - r, y + (h / 2), stroke, 1);

		break;
	}
	default:
		throw std::runtime_error("unhandled case");
	}
}
