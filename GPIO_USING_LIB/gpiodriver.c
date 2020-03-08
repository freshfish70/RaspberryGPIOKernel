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

/* SYSFS */

static struct kobject *gpiodriver_kobject;

static ssize_t sysfs_toggle(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
	u8 pin = 0;
	sscanf(buff, "%hhd", &pin);
	toggle_output(pin);
	printk(KERN_INFO "GPIO: Toggled pin %d ", pin);
	return count;
}

static ssize_t sysfs_set_state(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
	u8 pin = 0;
	u8 state = 0;
	sscanf(buff, "%hhd %hhd", &pin, &state);
	gpio_set_value(pin, state);
	printk(KERN_INFO "GPIO: Turned on %d ", pin);
	return count;
}

static struct kobj_attribute gpiodriver_toggle_attribute = __ATTR(toggle, (S_IWUSR | S_IRUGO), NULL, sysfs_toggle);

static struct kobj_attribute gpiodriver_on_attribute = __ATTR(state, (S_IWUSR | S_IRUGO), NULL, sysfs_set_state);

// Initialize sysfs
void gpiodriver_sysfs_init(void)
{
	printk(KERN_INFO "GPIO: starting sysfs...");

	// Create and add sysfs object in /sys/gpiodriver
	gpiodriver_kobject = kobject_create_and_add("gpiodriver", NULL);

	// Create sysfs file for toggle attribute
	if (sysfs_create_file(gpiodriver_kobject, &gpiodriver_toggle_attribute.attr))
	{
		pr_debug("failed to create toggle sysfs\n");
	}
	
	// Create sysfs file for state attribute
	if (sysfs_create_file(gpiodriver_kobject, &gpiodriver_on_attribute.attr))
	{
		pr_debug("failed to create state sysfs!\n");
	}

	printk(KERN_INFO "GPIO: starting sysfs done.");
}

// Destruct sysfs
void gpiodriver_sysfs_exit(void)
{

	printk(KERN_INFO "GPIO: stopping sysfs...");

	// Decrements the refcpunt for the kobject, if it is 0 do cleanup (removeit)
	kobject_put(gpiodriver_kobject);

	printk(KERN_INFO "GPIO: stopping sysfs done.");
}

void gpiodriver_setup(void)
{
	printk(KERN_INFO "GPIO: Setting up GPIO pins \n");

	gpio_request(GPIO17, "GPIO17");
	gpio_request(GPIO27, "GPIO27");
	gpio_request(GPIO22, "GPIO22");

	gpio_direction_output(GPIO17, 0);
	gpio_direction_output(GPIO27, 0);
	gpio_direction_output(GPIO22, 0);
}

// Cleanup the GPIO pins
// Free the GPIO pins so others can use it
void gpiodriver_destruct(void)
{
	printk(KERN_INFO "GPIO: Cleanup GPIO pins\n");

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
	gpiodriver_setup();
	gpiodriver_sysfs_init();

	return 0;
}

static void __exit gpio_module_exit(void)
{
	gpiodriver_destruct();
	gpiodriver_sysfs_exit();
	printk("GPIO: Driver exited\n");
}

// Initialization
module_init(gpio_module_init);
module_exit(gpio_module_exit);