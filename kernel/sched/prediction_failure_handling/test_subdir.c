#include "test_subdir.h"
#include <linux/kernel.h>

void print_something_wild(){
  printk("from the Kernel in your face\n");
}
EXPORT_SYMBOL(print_something_wild);
