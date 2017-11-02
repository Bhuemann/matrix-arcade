// Created from bdf2c Version 4, (c) 2009, 2010 by Lutz Sammer
//	License AGPLv3: GNU Affero General Public License version 3

#include "../headers/font.h"

	/// character bitmap for each encoding
static const unsigned char __font_bitmap__[] = {
//   0 $00 'char0'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	X_X_X___,
	________,
	X___X___,
	________,
	X_X_X___,
	________,
//  32 $20 'space'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	________,
	________,
	________,
	________,
	________,
//  33 $21 'exclam'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	________,
	__X_____,
	________,
//  34 $22 'quotedbl'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X_X____,
	_X_X____,
	_X_X____,
	________,
	________,
	________,
	________,
//  35 $23 'numbersign'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_X_X____,
	XXXXX___,
	_X_X____,
	XXXXX___,
	_X_X____,
	________,
//  36 $24 'dollar'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_XXX____,
	X_X_____,
	_XXX____,
	__X_X___,
	_XXX____,
	________,
//  37 $25 'percent'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X_______,
	X__X____,
	__X_____,
	_X______,
	X__X____,
	___X____,
	________,
//  38 $26 'ampersand'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_X______,
	X_X_____,
	_X______,
	X_X_____,
	_X_X____,
	________,
//  39 $27 'quotesingle'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	__X_____,
	__X_____,
	________,
	________,
	________,
	________,
//  40 $28 'parenleft'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_X______,
	_X______,
	_X______,
	_X______,
	__X_____,
	________,
//  41 $29 'parenright'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X______,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	_X______,
	________,
//  42 $2a 'asterisk'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_X_X____,
	__X_____,
	_XXX____,
	__X_____,
	_X_X____,
	________,
//  43 $2b 'plus'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	__X_____,
	__X_____,
	XXXXX___,
	__X_____,
	__X_____,
	________,
//  44 $2c 'comma'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	________,
	________,
	__XX____,
	__X_____,
	_X______,
//  45 $2d 'hyphen'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	________,
	XXXX____,
	________,
	________,
	________,
//  46 $2e 'period'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	________,
	________,
	_XX_____,
	_XX_____,
	________,
//  47 $2f 'slash'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	___X____,
	__X_____,
	_X______,
	X_______,
	________,
	________,
//  48 $30 'zero'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_X_X____,
	_X_X____,
	_X_X____,
	_X_X____,
	__X_____,
	________,
//  49 $31 'one'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_XX_____,
	__X_____,
	__X_____,
	__X_____,
	_XXX____,
	________,
//  50 $32 'two'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	___X____,
	__X_____,
	_X______,
	XXXX____,
	________,
//  51 $33 'three'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	___X____,
	_XX_____,
	___X____,
	X__X____,
	_XX_____,
	________,
//  52 $34 'four'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_XX_____,
	X_X_____,
	XXXX____,
	__X_____,
	__X_____,
	________,
//  53 $35 'five'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	X_______,
	XXX_____,
	___X____,
	X__X____,
	_XX_____,
	________,
//  54 $36 'six'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X_______,
	XXX_____,
	X__X____,
	X__X____,
	_XX_____,
	________,
//  55 $37 'seven'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	___X____,
	__X_____,
	__X_____,
	_X______,
	_X______,
	________,
//  56 $38 'eight'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	_XX_____,
	X__X____,
	X__X____,
	_XX_____,
	________,
//  57 $39 'nine'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X__X____,
	_XXX____,
	___X____,
	_XX_____,
	________,
//  58 $3a 'colon'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_XX_____,
	_XX_____,
	________,
	_XX_____,
	_XX_____,
	________,
//  59 $3b 'semicolon'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_XX_____,
	_XX_____,
	________,
	_XX_____,
	_X______,
	X_______,
//  60 $3c 'less'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	___X____,
	__X_____,
	_X______,
	__X_____,
	___X____,
	________,
//  61 $3d 'equal'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	XXXX____,
	________,
	XXXX____,
	________,
	________,
//  62 $3e 'greater'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	_X______,
	__X_____,
	___X____,
	__X_____,
	_X______,
	________,
//  63 $3f 'question'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_X_X____,
	___X____,
	__X_____,
	________,
	__X_____,
	________,
//  64 $40 'at'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X_XX____,
	X_XX____,
	X_______,
	_XX_____,
	________,
//  65 $41 'A'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X__X____,
	XXXX____,
	X__X____,
	X__X____,
	________,
//  66 $42 'B'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXX_____,
	X__X____,
	XXX_____,
	X__X____,
	X__X____,
	XXX_____,
	________,
//  67 $43 'C'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X_______,
	X_______,
	X__X____,
	_XX_____,
	________,
//  68 $44 'D'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXX_____,
	X__X____,
	X__X____,
	X__X____,
	X__X____,
	XXX_____,
	________,
//  69 $45 'E'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	X_______,
	XXX_____,
	X_______,
	X_______,
	XXXX____,
	________,
//  70 $46 'F'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	X_______,
	XXX_____,
	X_______,
	X_______,
	X_______,
	________,
//  71 $47 'G'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X_______,
	X_XX____,
	X__X____,
	_XXX____,
	________,
//  72 $48 'H'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X__X____,
	XXXX____,
	X__X____,
	X__X____,
	X__X____,
	________,
//  73 $49 'I'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XXX____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	_XXX____,
	________,
//  74 $4a 'J'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	___X____,
	___X____,
	___X____,
	___X____,
	X__X____,
	_XX_____,
	________,
//  75 $4b 'K'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X_X_____,
	XX______,
	XX______,
	X_X_____,
	X__X____,
	________,
//  76 $4c 'L'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X_______,
	X_______,
	X_______,
	X_______,
	X_______,
	XXXX____,
	________,
//  77 $4d 'M'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	XXXX____,
	XXXX____,
	X__X____,
	X__X____,
	X__X____,
	________,
//  78 $4e 'N'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	XX_X____,
	XX_X____,
	X_XX____,
	X_XX____,
	X__X____,
	________,
//  79 $4f 'O'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X__X____,
	X__X____,
	X__X____,
	_XX_____,
	________,
//  80 $50 'P'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXX_____,
	X__X____,
	X__X____,
	XXX_____,
	X_______,
	X_______,
	________,
//  81 $51 'Q'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	X__X____,
	X__X____,
	XX_X____,
	_XX_____,
	___X____,
//  82 $52 'R'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXX_____,
	X__X____,
	X__X____,
	XXX_____,
	X_X_____,
	X__X____,
	________,
//  83 $53 'S'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	X__X____,
	_X______,
	__X_____,
	X__X____,
	_XX_____,
	________,
//  84 $54 'T'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XXX____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	________,
//  85 $55 'U'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X__X____,
	X__X____,
	X__X____,
	X__X____,
	_XX_____,
	________,
//  86 $56 'V'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X__X____,
	X__X____,
	X__X____,
	_XX_____,
	_XX_____,
	________,
//  87 $57 'W'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X__X____,
	X__X____,
	XXXX____,
	XXXX____,
	X__X____,
	________,
//  88 $58 'X'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X__X____,
	X__X____,
	_XX_____,
	_XX_____,
	X__X____,
	X__X____,
	________,
//  89 $59 'Y'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X_X____,
	_X_X____,
	_X_X____,
	__X_____,
	__X_____,
	__X_____,
	________,
//  90 $5a 'Z'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	XXXX____,
	___X____,
	__X_____,
	_X______,
	X_______,
	XXXX____,
	________,
//  91 $5b 'bracketleft'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XXX____,
	_X______,
	_X______,
	_X______,
	_X______,
	_XXX____,
	________,
//  92 $5c 'backslash'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	X_______,
	_X______,
	__X_____,
	___X____,
	________,
	________,
//  93 $5d 'bracketright'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XXX____,
	___X____,
	___X____,
	___X____,
	___X____,
	_XXX____,
	________,
//  94 $5e 'asciicircum'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_X_X____,
	________,
	________,
	________,
	________,
	________,
//  95 $5f 'underscore'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	________,
	________,
	________,
	XXXX____,
	________,
//  96 $60 'grave'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X______,
	__X_____,
	________,
	________,
	________,
	________,
	________,
//  97 $61 'a'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XXX____,
	X__X____,
	X_XX____,
	_X_X____,
	________,
//  98 $62 'b'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X_______,
	X_______,
	XXX_____,
	X__X____,
	X__X____,
	XXX_____,
	________,
//  99 $63 'c'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XX_____,
	X_______,
	X_______,
	_XX_____,
	________,
// 100 $64 'd'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	___X____,
	___X____,
	_XXX____,
	X__X____,
	X__X____,
	_XXX____,
	________,
// 101 $65 'e'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XX_____,
	X_XX____,
	XX______,
	_XX_____,
	________,
// 102 $66 'f'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	_X_X____,
	_X______,
	XXX_____,
	_X______,
	_X______,
	________,
// 103 $67 'g'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XXX____,
	X__X____,
	_XX_____,
	X_______,
	_XXX____,
// 104 $68 'h'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X_______,
	X_______,
	XXX_____,
	X__X____,
	X__X____,
	X__X____,
	________,
// 105 $69 'i'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	________,
	_XX_____,
	__X_____,
	__X_____,
	_XXX____,
	________,
// 106 $6a 'j'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	___X____,
	________,
	___X____,
	___X____,
	___X____,
	_X_X____,
	__X_____,
// 107 $6b 'k'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	X_______,
	X_______,
	X_X_____,
	XX______,
	X_X_____,
	X__X____,
	________,
// 108 $6c 'l'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_XX_____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	_XXX____,
	________,
// 109 $6d 'm'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	X_X_____,
	XXXX____,
	X__X____,
	X__X____,
	________,
// 110 $6e 'n'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	XXX_____,
	X__X____,
	X__X____,
	X__X____,
	________,
// 111 $6f 'o'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XX_____,
	X__X____,
	X__X____,
	_XX_____,
	________,
// 112 $70 'p'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	XXX_____,
	X__X____,
	X__X____,
	XXX_____,
	X_______,
// 113 $71 'q'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XXX____,
	X__X____,
	X__X____,
	_XXX____,
	___X____,
// 114 $72 'r'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	XXX_____,
	X__X____,
	X_______,
	X_______,
	________,
// 115 $73 's'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_XXX____,
	XX______,
	__XX____,
	XXX_____,
	________,
// 116 $74 't'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X______,
	_X______,
	XXX_____,
	_X______,
	_X______,
	__XX____,
	________,
// 117 $75 'u'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	X__X____,
	X__X____,
	X__X____,
	_XXX____,
	________,
// 118 $76 'v'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	_X_X____,
	_X_X____,
	_X_X____,
	__X_____,
	________,
// 119 $77 'w'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	X__X____,
	X__X____,
	XXXX____,
	XXXX____,
	________,
// 120 $78 'x'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	X__X____,
	_XX_____,
	_XX_____,
	X__X____,
	________,
// 121 $79 'y'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	X__X____,
	X__X____,
	_X_X____,
	__X_____,
	_X______,
// 122 $7a 'z'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	________,
	________,
	XXXX____,
	__X_____,
	_X______,
	XXXX____,
	________,
// 123 $7b 'braceleft'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	___X____,
	__X_____,
	_XX_____,
	__X_____,
	__X_____,
	___X____,
	________,
// 124 $7c 'bar'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	________,
// 125 $7d 'braceright'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X______,
	__X_____,
	__XX____,
	__X_____,
	__X_____,
	_X______,
	________,
// 126 $7e 'asciitilde'
//	width 5, bbx 0, bby -1, bbw 5, bbh 7
	_X_X____,
	X_X_____,
	________,
	________,
	________,
	________,
	________
};

	/// character width for each encoding
static const unsigned char __font_widths__[] = {
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5,
	5
};

/// bitmap font structure
/*const struct bitmap_font font5x7 = {
	Width : 5, Height : 7,
	Chars : 96,
	Widths : __font_widths__,
	Bitmap : __font_bitmap__,
};*/

const struct bitmap_font font5x7 = {
	5, 7, 96, __font_widths__, 0, __font_bitmap__
};
