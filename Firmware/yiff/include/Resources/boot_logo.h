/*
 * boot_logo.h
 *
 *  Created on: Dec 25, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_RESOURCES_BOOT_LOGO_H_
#define INCLUDE_RESOURCES_BOOT_LOGO_H_

#define YHL_BOOT_LOGO_WIDTH 128U
#define YHL_BOOT_LOGO_HEIGHT 128U

static const uint8_t boot_logo_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
   0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x0b,
   0xd0, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0xff, 0x0f, 0x00, 0x00, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x00, 0x00, 0x80, 0x3f, 0xff, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x1f, 0x00, 0x00,
   0x80, 0x3f, 0xf8, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
   0xff, 0x03, 0x00, 0x00, 0xc0, 0x1f, 0xc0, 0xff, 0x03, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xff,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0x00, 0x00,
   0xe0, 0x0f, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0x0f, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xe0,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00,
   0xf8, 0x03, 0x00, 0x80, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00,
   0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x00, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x0f,
   0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00,
   0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x3f, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
   0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
   0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0xfe, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xc0,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xc0, 0x1f, 0x00,
   0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00,
   0x00, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x07, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0xf0, 0x07, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x1f, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0x01, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00,
   0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
   0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x80, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01,
   0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x18, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x01, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f,
   0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0xc0, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x07, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0xf0, 0x03,
   0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0xfc, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x07, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03,
   0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xe0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x01, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07,
   0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x3f, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x0f, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
   0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xf0, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x7e, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f,
   0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0xc0, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00,
   0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x00,
   0x00, 0x80, 0x3f, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f,
   0xf8, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0xf0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00,
   0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0xe0, 0x0f, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0xfc, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f, 0x00, 0xfc, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f, 0x00,
   0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x07, 0x7e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3f, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0xbf, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3e, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x7e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
   0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7e, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0xfd, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xf8, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3e, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0xe0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7f, 0x80, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0xfe, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
   0x00, 0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x1f, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0xf0, 0x07, 0x00,
   0x00, 0x00, 0x00, 0x1f, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x07, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x80, 0x1f,
   0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0x00, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03,
   0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0f, 0xf0, 0x03, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x03, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f,
   0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x0f, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00,
   0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0xe0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07,
   0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0xc0, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x07, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00,
   0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0xc0, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0x3f, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03,
   0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0xe0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x03, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
   0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x80, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01,
   0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x01, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
   0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00,
   0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x1f, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0xf0, 0x07, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00,
   0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0x07, 0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0xc0, 0x1f, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x00,
   0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0xff, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00,
   0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0xfe,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07,
   0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x3f, 0x00, 0x00, 0x80, 0x3f,
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f,
   0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x80, 0xff, 0x01, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x80,
   0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0x00, 0xc0, 0x0f,
   0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0x0f, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x3f, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0xfc,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0xf0, 0x07,
   0x00, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
   0xff, 0x03, 0xf8, 0x03, 0x00, 0x00, 0xc0, 0xff, 0x03, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x80, 0xff, 0x1f, 0xf8, 0x01, 0x00, 0x00, 0xf8, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xfc, 0x01,
   0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0xff, 0xff, 0x00, 0x00, 0xe0, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x05, 0xa0, 0xff, 0xff, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
   0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x15, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };




#endif /* INCLUDE_RESOURCES_BOOT_LOGO_H_ */
