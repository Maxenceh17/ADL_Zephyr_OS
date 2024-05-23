/*
 * Copyright (c) 2017 Linaro Limited.
 * Copyright (c) 2018 Nordic Semiconductor.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/shell/shell.h>

#define PR_SHELL(sh, fmt, ...)				\
	shell_fprintf(sh, SHELL_NORMAL, fmt, ##__VA_ARGS__)
#define PR_ERROR(sh, fmt, ...)				\
	shell_fprintf(sh, SHELL_ERROR, fmt, ##__VA_ARGS__)

/* S'assurer que toute la memoire est protegee (FLASH + SRAM)*/
#define RESERVED_MEM_MAP  (CONFIG_FLASH_BASE_ADDRESS + 0xA0000)
#define FLASH_MEM         CONFIG_FLASH_BASE_ADDRESS
#define RAM_MEM           CONFIG_SRAM_BASE_ADDRESS

/* Les commandes pour tester MPU */
#define READ_CMD_HELP  "Read from a reserved address in the memory map"
#define WRITE_CMD_HELP "Write in a reserved address in the memory map"
#define RUN_CMD_HELP   "Run code located in RAM"

static int cmd_read(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	uint32_t *p_mem = (uint32_t *) RESERVED_MEM_MAP;

	/* Lire depuis une adresse protegee */
	PR_SHELL(sh, "The value is: %d\n", *p_mem);

	return 0;
}

static int cmd_write(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	uint32_t *p_mem = (uint32_t *) RESERVED_MEM_MAP;

	PR_SHELL(sh, "write address: 0x%x\n", RESERVED_MEM_MAP);

	/* Ecrire sur une adresse protegee */
	*p_mem = 0xBADC0DE;

	return 0;
}

static int cmd_run(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	void (*func_ptr)(void) = (void (*)(void)) RAM_MEM;

	/* Executer depuis une memoire protegee */
	func_ptr();

	return 0;
}

/* L'adaptation des commande MPU pour shell */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_mpu,
	SHELL_CMD(read, NULL, READ_CMD_HELP, cmd_read),
	SHELL_CMD(run, NULL, RUN_CMD_HELP, cmd_run),
	SHELL_CMD(write, NULL, WRITE_CMD_HELP, cmd_write)
);

/* La methode d'execution s*/
SHELL_CMD_REGISTER(mpu, &sub_mpu, "MPU related commands.", NULL);
