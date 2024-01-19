#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
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
	{ 0x38c7ddda, "unregister_netdev" },
	{ 0x240f1dee, "free_netdev" },
	{ 0xaceba876, "skb_clone_tx_timestamp" },
	{ 0x4bb2c6c0, "consume_skb" },
	{ 0x451487d4, "skb_tstamp_tx" },
	{ 0xec2ced30, "devm_kmalloc" },
	{ 0x8ed94553, "alloc_etherdev_mqs" },
	{ 0x99fb7e01, "register_netdev" },
	{ 0x64c56e5a, "platform_driver_unregister" },
	{ 0x1c1aa4c1, "eth_validate_addr" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xde6c83c2, "__platform_driver_register" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x122c3a7e, "_printk" },
	{ 0x453e7dc, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CF1C7DD08746B97979011D2");
