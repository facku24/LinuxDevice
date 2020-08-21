#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

/*----------------- Variables -----------------*/
int valueETX, arr_valueETX[4];
char *nameETX;
int cb_valueETX = 0;

module_param(valueETX, int, S_IRUSR|S_IWUSR);					//integer value
module_param(nameETX, charp, S_IRUSR|S_IWUSR);					//String
module_param_array(arr_valueETX, int, NULL, S_IRUSR|S_IWUSR);	//Array of integers

/*-----------------Module_param_cb()-----------------*/

