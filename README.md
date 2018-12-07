
### Compiling the module ###

1- Install Visual Studio 2012
2- Download php-sdk-binary-tools-20110915, deps-5.6-vc11-x86, and php-5.6.38-src.zip from https://windows.php.net/downloads/php-sdk/
3- Run VS2012 x86 Native Tools Command Prompt.
4- Extract php-sdk-binary-tools-20110915 in a separate directory.
5- move to bin directory created after extraction.
6- excute phpsdk_buildtree.bat <buildname> [buildpath] in bin to create directory structure for build.
7- move to the folder with name "buildname". you will find vc folders. copy vc9 as vc11.
8- extract the source code downloaded in vc11\x86\ folder. and also extract deps* in the same folder.
9- Copy blowfish folder in given package in ext folder inside the source.
10- execute buildconf in source code directory.
11- execute configure --help to enable/disable php features.
12- execute configure --with-cblowfish in order to compile cblowfish module
13- run nmake to build the source.
14- run nmake install to install php and module php_cblowfish.dll in c:\php\ext

