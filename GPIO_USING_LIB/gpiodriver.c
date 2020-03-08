#include "hgpio.h"

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PI GPIO DRIVER");
MODULE_AUTHOR("Team 7");

// GPIO Pin maps
#define GPIO17 17
#define GPIO27 27
#define GPIO22 22

// Toggles the output, if it is currently off, set it to on and vice versa
int toggle_output(int gpiopin)
{
	int state = gpio_get_value(gpiopin);

	if (state == 0)
	{
		state = 1;
	}
	else
	{
		state = 0;
	}
	gpio_set_value(gpiopin, state);

	return 1;
}

void gpio_setup(void)
{
	printk("GPIO: Setting up GPIO pins \n");

	gpio_request(GPIO17, "GPIO17");
	gpio_request(GPIO27, "GPIO27");
	gpio_request(GPIO22, "GPIO22");

	gpio_direction_output(GPIO17, 0);
	gpio_direction_output(GPIO27, 0);
	gpio_direction_output(GPIO22, 0);
}

// Creates a traffick light sequence of GPIO17,GPIO27,GPIO22
void traffic_light(void)
{
	for (size_t i = 0; i < 15; i++)
	{
		mdelay(200);
		toggle_output(GPIO17);
		mdelay(200);
		toggle_output(GPIO27);
		mdelay(200);
		toggle_output(GPIO22);
	}
}

// Cleanup the GPIO pins
// Free the GPIO pins so others can use it
void gpio_destruct(void)
{
	printk("GPIO: Cleanup GPIO pins\n");

	gpio_set_value(GPIO17, 0);
	gpio_set_value(GPIO27, 0);
	gpio_set_value(GPIO22, 0);

	gpio_free(GPIO17);
	gpio_free(GPIO27);
	gpio_free(GPIO22);
}

// MODULE INITIALIZATION

static int __init gpio_module_init(void)
{
	printk("GPIO: Driver started\n");
	gpio_setup();

	return 0;
}

static void __exit gpio_module_exit(void)
{
	gpio_destruct();
	printk("GPIO: Driver exited\n");
}

// Initialization
module_init(gpio_module_init);
module_exit(gpio_module_exit);