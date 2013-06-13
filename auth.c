#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <oauth.h>
#include <openssl/hmac.h>

#if 0
// https://dev.twitter.com/docs/auth/percent-encoding-parameters
void percentencode(char *src, char *dest) {
	while (*src != '\0') {
		if ((*src >= '0' && *src <= '9') ||
		    (*src >= 'A' && *src <= 'Z') ||
		    (*src >= 'a' && *src <= 'z') ||
		    *src == '-' ||
		    *src == '.' ||
		    *src == '_' ||
		    *src == '~') {
			*dest++ = *src++;
		} else {
			*dest++ = '%';
			*dest++ = "0123456789ABCDEF"[*src / 16];
			*dest++ = "0123456789ABCDEF"[*src % 16];
			*src++;
		}
	}

	*dest = '\0';
}
#endif

int main(int argc, char **argv) {
	if (argc != 7) {
		fprintf(stderr, "Usage: URL consumerkey data consumersecret token tokensecret\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *url = argv[1];
	char *data = argv[2];
	char *c_key = argv[3];
	char *c_secret = argv[4];
	char *t_key = argv[5];
	char *t_secret = argv[6];

	char *sig = oauth_sign_url2(url, &data, OA_HMAC, "GET", c_key, c_secret, t_key, t_secret);

	printf("%s\n", sig);

	char **oauth_args = NULL;
	int n = oauth_split_url_parameters(sig, &oauth_args);

	printf("Authorization: OAuth ");

	int i;
	for (i = 0; i < n; i++) {
		if (strncmp(oauth_args[i], "oauth_", 6) == 0) {
			char *cp;
			for (cp = oauth_args[i]; *cp; cp++) {
				putchar(*cp);
				if (*cp == '=') {
					cp++;
					break;
				}
			}
			putchar('"');
			for (; *cp; cp++) {
				putchar(*cp);
			}
			putchar('"');
			if (i + 1 < n) {
				printf(", ");
			}
		}
	}

	putchar('\n');

#if 0
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_sec ^ tv.tv_usec);

	char nonce[33];
	int i;
	for (i = 0; i < 32; i++) {
		nonce[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"[rand() % 62];
	}
	nonce[32] = '\0';

	printf("Authorization: OAuth ");
	printf("oauth_consumer_key=\"%s\", ", c_key);
	printf("oauth_nonce=\"%s\", ", nonce);
	printf("oauth_signature=\"%s\", ", basestring);
	printf("oauth_signature_method=\"HMAC-SHA1\", ");
	printf("oauth_timestamp=\"%lld\", ", (long long) tv.tv_sec);
	printf("oauth_token=\"%s\", ", t_key);
	printf("oauth_version=\"1.0\"\n");
#endif
}
