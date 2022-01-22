#pragma once

/**
RGB color values with an alpha channel.
 */
typedef struct RGBAColor {
	int r;					///< red channel
	int g;					///< green channel			
	int b;					///< blue channel
	int a;					///< alpha channel
} RGBAColor;

/**
RGB color values
 */
typedef struct RGBColor {
	int r;					///< red channel
	int g;					///< green channel
	int b;					///< blue channel
} RGBColor;
