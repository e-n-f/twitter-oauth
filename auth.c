#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <oauth.h>

int main(int argc, char **argv) {
	if (argc != 6) {
		fprintf(stderr, "Usage: %s URL consumerkey consumersecret token tokensecret\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *url = argv[1];
	char *c_key = argv[2];
	char *c_secret = argv[3];
	char *t_key = argv[4];
	char *t_secret = argv[5];

	char *sig = oauth_sign_url2(url, NULL, OA_HMAC, "GET", c_key, c_secret, t_key, t_secret);

	printf("Authorization: OAuth ");
	int within = 0;

	// Can't just use oauth_split_url_parameters() because it loses the last one (?!?)

	char *s;
	for (s = sig; *s != '\0'; s++) {
		if (*s == '?' || *s == '&') {
			if (strncmp(s + 1, "oauth_", 6) == 0) {
				if (within) {
					printf(", ");
				}
				within = 1;

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
