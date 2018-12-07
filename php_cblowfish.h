#ifndef PHP_CBLOWFISH_H
#define PHP_CBLOWFISH_H

#define PHP_CBLOWFISH_EXTNAME  "cblowfish"
#define PHP_CBLOWFISH_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 




extern "C" {
#include "php.h"
}

extern zend_module_entry cblowfish_module_entry;
#define phpext_cblowfish_ptr &cblowfish_module_entry;

#endif /* PHP_CBLOWFISH_H */