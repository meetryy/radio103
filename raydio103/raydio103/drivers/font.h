const uint8_t font[] = {
0x00,   0x00,   0x00,   0x00,   0x00,   // 32 = ' '
0x00,   0x00,   0xbf,   0x9f,   0x00,   // 33 = '!'
0x00,   0x07,   0x00,   0x07,   0x00,   // 34 = '"'
0x24,   0xff,   0x24,   0xff,   0x24,   // 35 = '#'
0x46,   0x89,   0xff,   0x89,   0x72,   // 36 = '$'
0x87,   0x63,   0x18,   0xc4,   0xe2,   // 37 = '%'
0x66,   0xd9,   0x99,   0x76,   0xc8,   // 38 = '&'
0x00,   0x07,   0x00,   0x00,   0x00,   // 39 = '''
0x00,   0x00,   0x7e,   0x81,   0x00,   // 40 = '('
0x00,   0x00,   0x81,   0x7e,   0x00,   // 41 = ')'
0x00,   0x04,   0x0e,   0x04,   0x00,   // 42 = '*'
0x08,   0x08,   0x3e,   0x08,   0x08,   // 43 = '+'
0x00,   0x00,   0x40,   0xc0,   0x00,   // 44 = ','
0x08,   0x08,   0x08,   0x08,   0x08,   // 45 = '-'
0x00,   0x00,   0xc0,   0xc0,   0x00,   // 46 = '.'
0xc0,   0x30,   0x0c,   0x03,   0x00,   // 47 = '/'
0x7e,   0x81,   0x81,   0x81,   0x7e,   // 48 = '0'
0x00,   0x00,   0x00,   0x04,   0xff,   // 49 = '1'
0xc2,   0xa1,   0x91,   0x89,   0x86,   // 50 = '2'
0x42,   0x81,   0x89,   0x89,   0x76,   // 51 = '3'
0x07,   0x08,   0x08,   0x08,   0xfe,   // 52 = '4'
0x87,   0x89,   0x89,   0x89,   0x71,   // 53 = '5'
0x7e,   0x89,   0x89,   0x89,   0x70,   // 54 = '6'
0x01,   0x01,   0x01,   0x01,   0xfe,   // 55 = '7'
0x76,   0x89,   0x89,   0x89,   0x76,   // 56 = '8'
0x06,   0x89,   0x89,   0x89,   0x7e,   // 57 = '9'
0x00,   0x00,   0x66,   0x66,   0x00,   // 58 = ':'
0x00,   0x26,   0x66,   0x00,   0x00,   // 59 = ';'
0x10,   0x28,   0x44,   0x82,   0x00,   // 60 = '<'
0x28,   0x28,   0x28,   0x28,   0x28,   // 61 = '='
0x82,   0x44,   0x28,   0x10,   0x00,   // 62 = '>'
0x06,   0x01,   0xa1,   0x11,   0x0e,   // 63 = '?'
0x7e,   0x99,   0xa5,   0xbd,   0x3f,   // 64 = '@'
0xfc,   0x0a,   0x09,   0x09,   0xfe,   // 65 = 'A'
0xff,   0x89,   0x89,   0x8e,   0x70,   // 66 = 'B'
0x7e,   0x81,   0x81,   0x81,   0x42,   // 67 = 'C'
0xff,   0x81,   0x81,   0x81,   0x7e,   // 68 = 'D'
0x7e,   0x89,   0x89,   0x89,   0x81,   // 69 = 'E'
0xfe,   0x09,   0x09,   0x09,   0x01,   // 70 = 'F'
0x7e,   0x81,   0x89,   0x89,   0xf1,   // 71 = 'G'
0xff,   0x08,   0x08,   0x08,   0xff,   // 72 = 'H'
0x00,   0x81,   0xff,   0x81,   0x00,   // 73 = 'I'
0x80,   0x80,   0x81,   0x81,   0x7f,   // 74 = 'J'
0xff,   0x08,   0x0c,   0x0a,   0xf1,   // 75 = 'K'
0xff,   0x80,   0x80,   0x80,   0x80,   // 76 = 'L'
0xff,   0x02,   0x0c,   0x02,   0xff,   // 77 = 'M'
0xff,   0x02,   0x04,   0x08,   0xff,   // 78 = 'N'
0x7e,   0x81,   0x81,   0x81,   0x7e,   // 79 = 'O'
0xfe,   0x09,   0x09,   0x09,   0x06,   // 80 = 'P'
0x7e,   0x81,   0xa1,   0x41,   0xbe,   // 81 = 'Q'
0xfe,   0x09,   0x19,   0x29,   0xc6,   // 82 = 'R'
0x46,   0x89,   0x89,   0x89,   0x72,   // 83 = 'S'
0x01,   0x01,   0xff,   0x01,   0x01,   // 84 = 'T'
0x7f,   0x80,   0x80,   0x80,   0x7f,   // 85 = 'U'
0xff,   0x40,   0x20,   0x10,   0x0f,   // 86 = 'V'
0x7f,   0x80,   0x78,   0x80,   0x7f,   // 87 = 'W'
0xe3,   0x14,   0x08,   0x14,   0xe3,   // 88 = 'X'
0x07,   0x08,   0xf0,   0x08,   0x07,   // 89 = 'Y'
0xe1,   0x91,   0x89,   0x85,   0x83,   // 90 = 'Z'
};
const uint16_t fontBig[] = {
0x3ffc,         0x7ffe,         0xe007,         0xc003,         0xc003,         0xc003,         0xe007,         0x7ffe,         0x3ffc,         // 48 = '0'
0x0000,         0x0000,         0x0000,         0x0000,         0x0000,         0x0000,         0x0018,         0x7ffe,         0xffff,         // 49 = '1'
0xf00c,         0xf80e,         0xdc07,         0xce03,         0xc703,         0xc383,         0xc1c7,         0xc0fe,         0x807c,         // 50 = '2'
0x300c,         0x700e,         0xe003,         0xc003,         0xc083,         0xc083,         0xe1c7,         0x7ffe,         0x3f7c,         // 51 = '3'
0x001e,         0x007f,         0x00e0,         0x00c0,         0x00c0,         0x00c0,         0x00c0,         0x7ffe,         0xfffc,         // 52 = '4'
0x407e,         0xc0ff,         0xc0c3,         0xc0c3,         0xc0c3,         0xc0c3,         0x61c3,         0x7f83,         0x3f02,         // 53 = '5'
0x3ff8,         0x7ffe,         0xe1cf,         0xc0c3,         0xc0c3,         0xc0c3,         0x61c3,         0x7f82,         0x3f00,         // 54 = '6'
0x0002,         0x0003,         0x0003,         0x0003,         0x0083,         0x00c3,         0x00c7,         0xfffe,         0x7ffc,         // 55 = '7'
0x3f7c,         0x7ffe,         0xe1c7,         0xc083,         0xc083,         0xc083,         0xe1c7,         0x7ffe,         0x3f7c,         // 56 = '8'
0x307c,         0x70fe,         0xe0c7,         0xc1c3,         0xc183,         0xc183,         0xe0c7,         0xfffe,         0x7ff8,         // 57 = '9'
};
const uint16_t fontMid[] = {
0x03fe,         0x07ff,         0x0603,         0x0603,         0x07ff,         0x03fe,         // 48 = '0'
0x0000,         0x000c,         0x07ff,         0x03ff,         0x0000,         0x0386,         // 49 = '1'
0x06e3,         0x067f,         0x063e,         0x0000,         0x0306,         0x0603,         // 50 = '2'
0x07ff,         0x03ee,         0x0000,         0x001e,         0x003f,         0x0030,         // 51 = '3'
0x07fc,         0x0000,         0x040e,         0x063f,         0x0633,         0x07f3,         // 52 = '4'
0x0000,         0x03fe,         0x07ff,         0x0633,         0x07f3,         0x03e2,         // 53 = '5'
0x0002,         0x0003,         0x0003,         0x07ff,         0x03fe,         0x0000,         // 54 = '6'
0x07ff,         0x0633,         0x07ff,         0x03ee,         0x0000,         0x021e,         // 55 = '7'
0x0633,         0x0663,         0x07ff,         0x03fe,         0x0000,         0x0000,         // 56 = '8'
0x03ff,         0x0301,         0x0301,         0x03ff,         0x01ff,         0x0000,         // 57 = '9'
};
//79 chars, 595 bytes
