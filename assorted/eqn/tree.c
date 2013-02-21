#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

static int P, *coeff, *cycles;

static void input_poly_coeffs(void)
{
	int i;

	for (i = 0; i < P; i++)
		coeff[i] = 0;

	for (i = 0; i < P; i++) {
		if (!scanf("%d", &coeff[i]))
			break;
		coeff[i] %= P;
	}
}

static int dump_dot_cycles(char *buf)
{
	int i, len = 0;

	len += sprintf(buf, "digraph g1 {\n\tsize = \"3,3\";\n");
	for (i = 0; i < P; i++)
		len += sprintf(buf+len, "\t%d -> %d;\n", i, cycles[i]);
	len += sprintf(buf+len, "}\n");

	return len;
}

static void compute_cycles(void)
{
	int i, j, s;

	for (i = 0; i < P; i++)
		cycles[i] = -1;

	while (1) {
		for (i = 0; cycles[i] != -1 && i < P; i++);
		if (i == P) break;

		do {
			s = coeff[P-1];
			for (j = P-2; j >= 0; j--) {
				s = (s*i + coeff[j]) % P;
			}
			cycles[i] = s;
			i = s;
		} while (cycles[i] == -1);
	}
}

static void save_dump_to_dot(char *buf, int len, unsigned long long int suffix)
{
	int fd;
	char fname[32];

	sprintf(fname, "p%d_%Lu.dot", P, suffix);
	if ((fd = open(fname, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) < 0) {
		perror("open");
		return;
	}

	write(fd, buf, len);
	close(fd);
}

static void do_all_poly(void)
{
	unsigned long long int max_poly = 1, temp;
	int i, len, s = 0;
	char buffer[4096];

	for (i = 0; i < P; i++)
		max_poly *= P;

	while (max_poly--) {
		/* compute polynomial */
		for (temp = max_poly, i = 0; i < P; i++, temp /= P)
			coeff[i] = temp % P;

		compute_cycles();
		len = sprintf(buffer, "/* POLY: %Lu / %d */\n", max_poly, P);

		len += sprintf(buffer+len, "/* ");
		for (i = s = 0; i < P; i++) {
			if (i && !coeff[i]) continue;
			if (s) len += sprintf(buffer+len, " + ");
			if (!i || coeff[i]>1)
				len += sprintf(buffer+len, "%d", coeff[i]);
			if (i) len += sprintf(buffer+len, "x^%d", i);
			s = 1;
		}
		len += sprintf(buffer+len, "*/\n");
		len += dump_dot_cycles(buffer + len);

		save_dump_to_dot(buffer, len, max_poly);
	}
}

int main(int argc, char *argv[])
{
	scanf("%d", &P);
	if (P <= 1) return -1;

	coeff = malloc(sizeof(int)*P);
	if (!coeff) return -1;

	cycles = malloc(sizeof(int)*P);
	if (!cycles) {
		free(coeff);
		return -1;
	}

	do_all_poly();

	return 0;
}
