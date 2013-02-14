main (int n, int x, int y, char c) {
	if (scanf("%d", &n))
		while (n-- && scanf("%d%c%d", &x, &c, &y)
		       && printf("%d\n", (!(c&1)?x*y:!(c&2)?x-y:x+y))) {}
}

