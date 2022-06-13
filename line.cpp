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
