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
