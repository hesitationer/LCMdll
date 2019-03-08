#include "stdafx.h"
#include "transform.h"
#include"LCM.h"

int transform(const char *color)
{
	int flag0, flag1, flag2, flag3, flag4, flag5, flag6;
	const char *a = "W";
	const char *b = "G64";
	const char *c = "G128";
	const char *d = "BK";
	const char *e = "R";
	const char *f = "G";
	const char *g = "B";
	flag0 = strcmp(color, a);
	flag1 = strcmp(color, b);
	flag2 = strcmp(color, c);
	flag3 = strcmp(color, d);
	flag4 = strcmp(color, e);
	flag5 = strcmp(color, f);
	flag6 = strcmp(color, g);
	if (flag0 == 0)
	{
		return 0;
	}
	else if (flag1 == 0)
	{
		return 1;
	}
	else if (flag2 == 0)
	{
		return 2;
	}
	else if (flag3 == 0)
	{
		return 3;
	}
	else if (flag4 == 0)
	{
		return 4;
	}
	else if (flag5 == 0)
	{
		return 5;
	}
	else if (flag6 == 0)
	{
		return 6;
	}
}
