### CHANGE-SET ####

1- C++ class CBlowFish given in AMOS C++ code is wrapped to call as PHP Extension
2- Placed the compiled extension php_cblowfish.dll in php\ext folder;
3- In php.ini extension is registered by adding line "extension=php_cblowfish"
4- Restart apache service in Windows services.
5- In application\libraries\Dbs.php changes are made to accomodate enrypted passwords. 
6- In application\config\constants.php define('UP2_PSWD_ENCRYPTED' , 0x02000000); is added to identify encrypted vs non encrypted passwords.

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

### Deliverables ###
1- Source code of module cblowfish in blowfish folder.
2- Modified php.ini to be placed in C:\Program Files (x86)\AMOS Mail Web\php\.
3- Modified Dbs.php to be placed in C:\Program Files (x86)\AMOS Mail Web\amweb\application\libraries\
4- Build module php_cblowfish.dll to be placed in C:\Program Files (x86)\AMOS Mail Web\php\ext\
5- Modifed C:\Program Files (x86)\AMOS Mail Web\amweb\application\config\constants.php is also provided.

Note:- Existing blowfish implementation in helpers\blowfish_helper.php file is still in use to authenticate database users.