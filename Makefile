auth: auth.c
	cc -o auth auth.c -loauth -I/usr/local/include -L/usr/local/lib
