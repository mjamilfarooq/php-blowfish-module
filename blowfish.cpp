#include "php_cblowfish.h"
#include "Blowfish2.h"
#include "blowfish_utils.h"

zend_object_handlers cblowfish_object_handlers;

struct cblowfish_object {
    zend_object std;
    CBlowFish *blowfish;
};


void cblowfish_free_storage(void *object TSRMLS_DC)
{
    cblowfish_object *obj = (cblowfish_object *)object;
    delete obj->blowfish; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value cblowfish_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    cblowfish_object *obj = (cblowfish_object *)emalloc(sizeof(cblowfish_object));
    memset(obj, 0, sizeof(cblowfish_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->properties_info,
        (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

    retval.handle = zend_objects_store_put(obj, NULL,
        cblowfish_free_storage, NULL TSRMLS_CC);
    retval.handlers = &cblowfish_object_handlers;
    return retval;
}



zend_class_entry *cblowfish_ce;

PHP_METHOD(CBlowFish, __construct)
{
    unsigned char * ucKey;
    size_t n;
    CBlowFish *blowfish = NULL;
    zval *object = getThis();

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ucKey, &n) == FAILURE) {
        php_error(E_ERROR, "Invalid argument for __constructor");
        RETURN_NULL();
    }

    blowfish = new CBlowFish(ucKey, n);
    cblowfish_object *obj = (cblowfish_object *)zend_object_store_get_object(object TSRMLS_CC);
    obj->blowfish = blowfish;
}

PHP_METHOD(CBlowFish, Encrypt)
{
    CBlowFish* blowfish;
    cblowfish_object * obj = (cblowfish_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    blowfish = obj->blowfish;
    if (blowfish == NULL) 
    {
        php_error(E_ERROR, "CBlowFish initialization issue");
        RETURN_NULL();
    }

    unsigned char * clearTextPassword;
    size_t clearTextLength = 0;

    //parsing clear text password as parameter    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &clearTextPassword, &clearTextLength) == FAILURE) 
    {
        php_error(E_ERROR, "Invalid arguments");
        RETURN_NULL();
    }

    int len = strlen( (char *)clearTextPassword );
	if( len%8 != 0 )	//	 Make buff len multiple of 8
		len = ((len+7)&~7);

	unsigned char*	cipherText = new unsigned char[len + 1];
	char*	cipherTextInHex = new char[len*2 + 1];
	
	memset( cipherText, 0, len + 1 );
	memset( cipherTextInHex, 0, len*2 + 1 );

    blowfish->Encrypt(clearTextPassword, cipherText, len);
    CharStr2HexStr(cipherText, cipherTextInHex, len);

    RETURN_STRING((char *)cipherTextInHex, strlen((const char *)cipherTextInHex));

    delete [] cipherText;
    delete [] cipherTextInHex;
}

PHP_METHOD(CBlowFish, Decrypt)
{
    //Get CBlowFish object from store
    CBlowFish* blowfish;
    cblowfish_object * obj = (cblowfish_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
    blowfish = obj->blowfish;
    if (blowfish == NULL)
    {
        php_error(E_ERROR, "Can't initialize CBlowFish object from store!!");
        RETURN_NULL();
    }

    //parse encrypted password in hex format from arguments
    char * encryptedPasswordInHex = NULL;
    size_t encryptedPasswordLength = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", 
                            &encryptedPasswordInHex, 
                            &encryptedPasswordLength) == FAILURE) {
        php_error(E_ERROR, "Invalide Arguments for CBlowFish::Decrypt");
        RETURN_NULL();
    }

    //
    int len = strlen( encryptedPasswordInHex );
	len = len/2;
	if( len%8 != 0 )	//	 Make buff len multiple of 8
		len = ((len+7)&~7);

	unsigned char * clearTextPassword = new unsigned char [len + 1];
	unsigned char * encryptedPasswordInChar = new unsigned char [len + 1];
	memset( clearTextPassword, 0, len + 1 );
	memset( encryptedPasswordInChar, 0, len + 1 );

	HexStr2CharStr(encryptedPasswordInHex, encryptedPasswordInChar, strlen( encryptedPasswordInHex )/2 );
	blowfish->Decrypt(encryptedPasswordInChar, clearTextPassword, len );
	
    RETURN_STRING((char *)clearTextPassword, strlen((char *)clearTextPassword));

    delete [] clearTextPassword;
    delete [] encryptedPasswordInChar;
}


zend_function_entry cblowfish_methods[] = {
    PHP_ME(CBlowFish, __construct,  NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(CBlowFish, Encrypt,      NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CBlowFish, Decrypt,      NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}

};

PHP_MINIT_FUNCTION(cblowfish)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CBlowFish", cblowfish_methods);
    cblowfish_ce = zend_register_internal_class(&ce TSRMLS_CC);
    cblowfish_ce->create_object = cblowfish_create_handler;
    memcpy(&cblowfish_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    cblowfish_object_handlers.clone_obj = NULL;

    return SUCCESS;
}

zend_module_entry cblowfish_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_CBLOWFISH_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(cblowfish),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_CBLOWFISH_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CBLOWFISH
extern "C" {
ZEND_GET_MODULE(cblowfish)
}
#endif