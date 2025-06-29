#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int mayor;

/*Estructura basica de operaciones del archivo*/
static struct file_operations fops = {
    .owner = THIS_MODULE
};

/*ESTA FUNCION SE LLAMA CUANDO EL MODULO SE CARGA AL KERNEL*/
static int __init my_init(void) {
    mayor = register_chrdev(0, "mimodulo_cdev", &fops);
    if (mayor < 0){
        printk(KERN_ERR "mimodulo_cdev - error de registro chrdev\n");
        return mayor;
    }
    printk(KERN_INFO "mimodulo_cdev - Numero de dispositivo mayor: %d\n", mayor);
    return 0;
}

/*ESTA FUNCION SE LLAMA CUANDO SE DESMONTA EL KERNEL*/
static void __exit my_exit(void) {
    unregister_chrdev(mayor, "mimodulo_cdev");
    printk(KERN_INFO "mimodulo_cdev - Modulo descargado\n");
}

module_init(my_init);
module_exit(my_exit);

/*Meta Information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriel Alvarez");
MODULE_DESCRIPTION("Driver para registrar un char device");

