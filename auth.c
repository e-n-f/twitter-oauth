#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <oauth.h>

int main(int argc, char **argv) {
	int i;
	extern int optind;
	extern char *optarg;
	const char *method = "GET";

	while ((i = getopt(argc, argv, "m:")) != -1) {
		switch (i) {
		case 'm':
			method = optarg;
			break;

		default:
			fprintf(stderr, "Unknown option %c\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 5) {
		fprintf(stderr, "Usage: %s [-m POST] URL consumerkey consumersecret token tokensecret\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *url = argv[optind];
	char *c_key = argv[optind + 1];
	char *c_secret = argv[optind + 2];
	char *t_key = argv[optind + 3];
	char *t_secret = argv[optind + 4];

	char *sig = oauth_sign_url2(url, NULL, OA_HMAC, method, c_key, c_secret, t_key, t_secret);
	char *separator = "";

	printf("Authorization: OAuth ");

	// Can't just use oauth_split_url_parameters() because it loses the last one (?!?)

	char *s;
	for (s = sig; *s; s++) {
		if (*s == '?' || *s == '&') {
			if (strncmp(s + 1, "oauth_", 6) == 0) {
				printf("%s", separator);
				separator = ", ";

				char *cp;
				for (cp = s + 1; *cp && *cp != '&'; cp++) {
					putchar(*cp);
					if (*cp == '=') {
						cp++;
						break;
					}
				}
				putchar('"');
				for (; *cp && *cp != '&'; cp++) {
					putchar(*cp);
				}
				putchar('"');
			}
		}
	}

	putchar('\n');
	return 0;
}
