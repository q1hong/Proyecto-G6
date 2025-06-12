#ifndef SAFE_DEVICE_H
#define SAFE_DEVICE_H

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <crypto/skcipher.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/random.h>

#define DEVICE_NAME "safe_device"
#define MAX_PW_LENGTH 256
#define MAX_TAG_LENGTH 32
#define KEY_SIZE 32  // AES-256
#define IV_LEN 16    // AES block size

// IOCTL Commandos
#define SAFE_MAGIC 'S'
#define SAFE_SET_KEY _IOW(SAFE_MAGIC, 0, char[KEY_SIZE])
#define SAFE_ADD_PW _IOW(SAFE_MAGIC, 1, struct pw_data)
#define SAFE_GET_PW _IOR(SAFE_MAGIC, 2, struct pw_data)
#define SAFE_DEL_PW _IO(SAFE_MAGIC, 3)

// Estructura compartida
struct pw_data {
    char tag[MAX_TAG_LENGTH];
    char password[MAX_PW_LENGTH];
    size_t length;
};

#endif

