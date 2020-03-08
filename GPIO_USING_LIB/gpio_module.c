#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PI GPIO DRIVER");
MODULE_AUTHOR("Team 7");

// GPIO Pin maps
#define GPIO17 17 // GPIO17


void gpio_setup(void){
	printk("GPIO: Setting up GPIO pins");

	gpio_request(GPIO17, "GPIO17");
	gpio_direction_output(GPIO17, 1);
}

// Cleanup the GPIO pins
// Free the GPIO pins so others can use it
void gpio_destruct(void){
	printk("GPIO: Cleanup GPIO pins");

	gpio_set_value(GPIO17, 0);
	gpio_free(GPIO17);
}

// MODULE INITIALIZATION

int gpio_module_init(void)
{
	printk("GPIO: Driver started");
	gpio_setup();
	return 0;
}

void gpio_module_exit(void)
{
	gpio_destruct();
	printk("GPIO: Driver exited");
}

// Initialization
module_init(gpio_module_init);
module_exit(gpio_module_exit);