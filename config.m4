PHP_ARG_ENABLE(cblowfish,
    [Whether to enable the "cblowfish" extension],
    [  --enable-cblowfish      Enable "cblowfish" extension support])

if test $PHP_CBLOWFISH != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(CBLOWFISH_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, CBLOWFISH_SHARED_LIBADD)
    PHP_NEW_EXTENSION(cblowfish, blowfish.cpp BlowFish2.cpp, $ext_shared)
fi