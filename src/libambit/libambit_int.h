/*
 * (C) Copyright 2013 Emil Ljungdahl
 *
 * This file is part of libambit.
 *
 * libambit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contributors:
 *
 */
#ifndef __LIBAMBIT_INT_H__
#define __LIBAMBIT_INT_H__

#include <stdint.h>
#include "hidapi/hidapi.h"
#include "libambit.h"

typedef struct ambit_supported_device_s ambit_supported_device_t;

typedef struct ambit_object_s {
    hid_device *handle;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t sequence_no;
    ambit_supported_device_t *device;
    ambit_device_info_t device_info;

    struct {
        bool initialized;
        uint32_t first_entry;
        uint32_t last_entry;
        uint32_t entries;
        uint32_t next_free_address;
        struct {
            uint32_t current;
            uint32_t next;
            uint32_t prev;
        } current;
        uint8_t *buffer;
        uint32_t prev_read;
        uint32_t last_addr;
    } pmem20;
} ambit_object_t;

enum ambit_commands_e {
    ambit_command_device_info        = 0x0000,
    ambit_command_time               = 0x0300,
    ambit_command_date               = 0x0302,
    ambit_command_status             = 0x0306,
    ambit_command_personal_settings  = 0x0b00,
    ambit_command_log_count          = 0x0b06,
    ambit_command_log_head_first     = 0x0b07,
    ambit_command_log_head_peek      = 0x0b08,
    ambit_command_log_head_step      = 0x0b0a,
    ambit_command_log_head           = 0x0b0b,
    ambit_command_log_read           = 0x0b17,
    ambit_command_lock_check         = 0x0b19,
    ambit_command_lock_set           = 0x0b1a,
};

// crc16.c
uint16_t crc16_ccitt_false(unsigned char *buf, size_t buflen);
uint16_t crc16_ccitt_false_init(unsigned char *buf, size_t buflen, uint16_t crc);

// personal.c
int libambit_personal_settings_parse(uint8_t *data, size_t datalen, ambit_personal_settings_t *settings);

// pmem20.c
int libambit_pmem20_init(ambit_object_t *object);
int libambit_pmem20_deinit(ambit_object_t *object);
int libambit_pmem20_next_header(ambit_object_t *object, ambit_log_header_t *log_header);
ambit_log_entry_t *libambit_pmem20_read_entry(ambit_object_t *object);
int libambit_pmem20_parse_header(uint8_t *data, size_t datalen, ambit_log_header_t *log_header);

// protocol.c
int libambit_protocol_command(ambit_object_t *object, uint16_t command, uint8_t *data, size_t datalen, uint8_t **reply_data, size_t *replylen, uint8_t legacy_format);
void libambit_protocol_free(uint8_t *data);



// static helpers
static inline uint8_t read8(uint8_t *buf, size_t offset)
{
    return buf[offset];
}

static inline uint16_t read16(uint8_t *buf, size_t offset)
{
    return (buf[offset] | (buf[offset+1] << 8));
}

static inline uint32_t read32(uint8_t *buf, size_t offset)
{
    return (buf[offset] | (buf[offset+1] << 8) | (buf[offset+2] << 16) | (buf[offset+3] << 24));
}

static inline uint8_t read8inc(uint8_t *buf, size_t *offset)
{
    return buf[(*offset)++];
}

static inline uint16_t read16inc(uint8_t *buf, size_t *offset)
{
    return (buf[(*offset)++] | (buf[(*offset)++] << 8));
}

static inline uint32_t read32inc(uint8_t *buf, size_t *offset)
{
    return (buf[(*offset)++] | (buf[(*offset)++] << 8) | (buf[(*offset)++] << 16) | (buf[(*offset)++] << 24));
}

#endif /* __LIBAMBIT_INT_H__ */