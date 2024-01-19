#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcb440b5e, "module_layout" },
	{ 0x38456707, "pci_unregister_driver" },
	{ 0xdb4961ce, "__pci_register_driver" },
	{ 0xb9b2940c, "__register_chrdev" },
	{ 0xedc03953, "iounmap" },
	{ 0x6baa0967, "try_module_get" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0xdb70e588, "module_put" },
	{ 0x85bd1608, "__request_region" },
	{ 0x77358855, "iomem_resource" },
	{ 0xde80cd09, "ioremap" },
	{ 0x92997ed8, "_printk" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F51D54DC2D92B1D107D103C");
