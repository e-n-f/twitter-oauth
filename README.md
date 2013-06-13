twitter-oauth
=============

A tiny program to mangle the authentication URL produced by
http://liboauth.sourceforge.net/
(<code>apt-get liboauth-dev</code>)
into the Authorization header that Twitter wants.

If you have set up an account at
https://dev.twitter.com/apps
that has

    Consumer key         A
    Consumer secret      B
    Access token         C
    Access token secret  D

and you want to access, say,
https://stream.twitter.com/1.1/statuses/filter.json?locations=-180,-90,180,90
you can do

    $ url='https://stream.twitter.com/1.1/statuses/filter.json?locations=-180,-90,180,90'
    $ curl --compress -s --header "$(./auth "$url" A B C D)" "$url"

to authenticate it and watch the stream.
    
