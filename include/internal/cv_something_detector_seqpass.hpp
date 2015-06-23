/*
 * cv_something_detector_seqpass.hpp
 *
 *  Created on: Apr 28, 2015
 *      Author: Rostislav Varzar
 */

#ifndef CV_SOMETHING_DETECTOR_SEQPASS_HPP_
#define CV_SOMETHING_DETECTOR_SEQPASS_HPP_

#ifndef __cplusplus
#error C++-only header
#endif

#include <cassert>
#include <cmath>
#include <c6x.h>
#include <stdio.h>
extern "C"
{
	#include <ti/dsplib/dsplib.h>
	#include <ti/mathlib/mathlib.h>
}

#include "internal/stdcpp.hpp"
#include "internal/cv_font_1.hpp"
#include "trik_vidtranscode_cv.h"

using namespace std;

/* **** **** **** **** **** */namespace trik /* **** **** **** **** **** */
{

/* **** **** **** **** **** */namespace cv /* **** **** **** **** **** */
{

#warning Eliminate global var
static uint64_t s_rgb888hsv[640 * 480];
static uint32_t s_wi2wo[640];
static uint32_t s_hi2ho[480];

const float F44100[] =
{ 9.1777136e-04, 9.2940698e-04, 9.4023589e-04, 9.5020394e-04, 9.5925569e-04,
		9.6733440e-04, 9.7438205e-04, 9.8033939e-04, 9.8514593e-04,
		9.8874002e-04, 9.9105882e-04, 9.9203841e-04, 9.9161376e-04,
		9.8971883e-04, 9.8628657e-04, 9.8124903e-04, 9.7453736e-04,
		9.6608188e-04, 9.5581217e-04, 9.4365711e-04, 9.2954496e-04,
		9.1340342e-04, 8.9515977e-04, 8.7474086e-04, 8.5207329e-04,
		8.2708345e-04, 7.9969763e-04, 7.6984214e-04, 7.3744340e-04,
		7.0242803e-04, 6.6472300e-04, 6.2425572e-04, 5.8095420e-04,
		5.3474716e-04, 4.8556420e-04, 4.3333593e-04, 3.7799421e-04,
		3.1947221e-04, 2.5770466e-04, 1.9262790e-04, 1.2418007e-04,
		5.2301231e-05, -2.3066566e-05, -1.0197905e-04, -1.8448954e-04,
		-2.7064861e-04, -3.6050377e-04, -4.5409902e-04, -5.5147449e-04,
		-6.5266617e-04, -7.5770578e-04, -8.6662101e-04, -9.7943575e-04,
		-1.0961706e-03, -1.2168432e-03, -1.3414680e-03, -1.4700554e-03,
		-1.6026103e-03, -1.7391295e-03, -1.8795995e-03, -2.0239940e-03,
		-2.1722741e-03, -2.3243900e-03, -2.4802855e-03, -2.6399056e-03,
		-2.8032037e-03, -2.9701465e-03, -3.1407136e-03, -3.3148881e-03,
		-3.4926376e-03, -3.6738888e-03, -3.8585007e-03, -4.0462477e-03,
		-4.2368238e-03, -4.4298777e-03, -4.6250789e-03, -4.8222071e-03,
		-5.0212394e-03, -5.2224005e-03, -5.4261346e-03, -5.6329728e-03,
		-5.8432901e-03, -6.0569947e-03, -6.2732364e-03, -6.4902600e-03,
		-6.7055382e-03, -6.9162756e-03, -7.1202774e-03, -7.3170241e-03,
		-7.5086343e-03, -7.7002705e-03, -7.8995395e-03, -8.1145998e-03,
		-8.3510594e-03, -8.6082697e-03, -8.8761755e-03, -9.1342683e-03,
		-9.3541518e-03, -9.5065784e-03, -9.5724814e-03, -9.5556847e-03,
		-9.4931048e-03, -9.4570716e-03, -9.5447467e-03, -9.8521395e-03,
		-1.0435059e-02, -1.1265763e-02, -1.2200339e-02, -1.2975420e-02,
		-1.3250801e-02, -1.2704931e-02, -1.1172739e-02, -8.7929739e-03,
		-6.1112454e-03, -4.0745392e-03, -3.8624888e-03, -6.5367524e-03,
		-1.2551660e-02, -2.1245403e-02, -3.0498223e-02, -3.6770479e-02,
		-3.5687259e-02, -2.3198093e-02, 2.8805669e-03, 4.1386255e-02,
		8.7032263e-02, 1.3096381e-01, 1.6293765e-01, 1.7466153e-01,
		1.6293765e-01, 1.3096381e-01, 8.7032263e-02, 4.1386255e-02,
		2.8805669e-03, -2.3198093e-02, -3.5687259e-02, -3.6770479e-02,
		-3.0498223e-02, -2.1245403e-02, -1.2551660e-02, -6.5367524e-03,
		-3.8624888e-03, -4.0745392e-03, -6.1112454e-03, -8.7929739e-03,
		-1.1172739e-02, -1.2704931e-02, -1.3250801e-02, -1.2975420e-02,
		-1.2200339e-02, -1.1265763e-02, -1.0435059e-02, -9.8521395e-03,
		-9.5447467e-03, -9.4570716e-03, -9.4931048e-03, -9.5556847e-03,
		-9.5724814e-03, -9.5065784e-03, -9.3541518e-03, -9.1342683e-03,
		-8.8761755e-03, -8.6082697e-03, -8.3510594e-03, -8.1145998e-03,
		-7.8995395e-03, -7.7002705e-03, -7.5086343e-03, -7.3170241e-03,
		-7.1202774e-03, -6.9162756e-03, -6.7055382e-03, -6.4902600e-03,
		-6.2732364e-03, -6.0569947e-03, -5.8432901e-03, -5.6329728e-03,
		-5.4261346e-03, -5.2224005e-03, -5.0212394e-03, -4.8222071e-03,
		-4.6250789e-03, -4.4298777e-03, -4.2368238e-03, -4.0462477e-03,
		-3.8585007e-03, -3.6738888e-03, -3.4926376e-03, -3.3148881e-03,
		-3.1407136e-03, -2.9701465e-03, -2.8032037e-03, -2.6399056e-03,
		-2.4802855e-03, -2.3243900e-03, -2.1722741e-03, -2.0239940e-03,
		-1.8795995e-03, -1.7391295e-03, -1.6026103e-03, -1.4700554e-03,
		-1.3414680e-03, -1.2168432e-03, -1.0961706e-03, -9.7943575e-04,
		-8.6662101e-04, -7.5770578e-04, -6.5266617e-04, -5.5147449e-04,
		-4.5409902e-04, -3.6050377e-04, -2.7064861e-04, -1.8448954e-04,
		-1.0197905e-04, -2.3066566e-05, 5.2301231e-05, 1.2418007e-04,
		1.9262790e-04, 2.5770466e-04, 3.1947221e-04, 3.7799421e-04,
		4.3333593e-04, 4.8556420e-04, 5.3474716e-04, 5.8095420e-04,
		6.2425572e-04, 6.6472300e-04, 7.0242803e-04, 7.3744340e-04,
		7.6984214e-04, 7.9969763e-04, 8.2708345e-04, 8.5207329e-04,
		8.7474086e-04, 8.9515977e-04, 9.1340342e-04, 9.2954496e-04,
		9.4365711e-04, 9.5581217e-04, 9.6608188e-04, 9.7453736e-04,
		9.8124903e-04, 9.8628657e-04, 9.8971883e-04, 9.9161376e-04,
		9.9203841e-04, 9.9105882e-04, 9.8874002e-04, 9.8514593e-04,
		9.8033939e-04, 9.7438205e-04, 9.6733440e-04, 9.5925569e-04,
		9.5020394e-04, 9.4023589e-04, 9.2940698e-04, };


template<>
class SomethingDetector<TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_YUV422,
		TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_RGB565X> : public CVAlgorithm
{
private:
	static const int m_detectZoneScale = 6;

	TrikCvImageDesc m_inImageDesc;
	TrikCvImageDesc m_outImageDesc;

	// Colors
	#define CL_RED		0xFF0000
	#define CL_GREEN	0x00FF00
	#define CL_BLUE		0x0000FF
	#define CL_BLACK	0x000000
	#define CL_WHITE	0xFFFFFF
	#define CL_GRAY		0x7F7F7F
	#define CL_YELLOW	0xFFFF00

	// Font params
	#define FONT_SPACE	10
	#define FONT_X		16
	#define FONT_Y		8

	#define array_count(A) (sizeof(A) / sizeof((A)[0]))

	static uint16_t* restrict s_mult43_div; // allocated from fast ram
	static uint16_t* restrict s_mult255_div; // allocated from fast ram

	static void __attribute__((always_inline)) writeOutputPixel(uint16_t* restrict _rgb565ptr,
			const uint32_t _rgb888)
	{
		*_rgb565ptr = ((_rgb888>>19)&0x001f) | ((_rgb888>>5)&0x07e0) | ((_rgb888<<8)&0xf800);
	}

	// Put pixel
	void __attribute__((always_inline)) drawOutputPixelBound(
			const int32_t _srcCol, const int32_t _srcRow,
			const int32_t _srcColBot, const int32_t _srcColTop,
			const int32_t _srcRowBot, const int32_t _srcRowTop,
			const TrikCvImageBuffer& _outImage, const uint32_t _rgb888) const
	{
		const int32_t srcCol = range<int32_t> (_srcColBot, _srcCol, _srcColTop);
		const int32_t srcRow = range<int32_t> (_srcRowBot, _srcRow, _srcRowTop);

		const int32_t dstRow = s_hi2ho[srcRow];
		const int32_t dstCol = s_wi2wo[srcCol];

		const uint32_t dstOfs = dstRow * m_outImageDesc.m_lineLength + dstCol
				* sizeof(uint16_t);
		writeOutputPixel(
				reinterpret_cast<uint16_t*> (_outImage.m_ptr + dstOfs), _rgb888);
	}

	// Draw dot
	void __attribute__((always_inline)) _draw_point(const int32_t x,
			const int32_t y, const TrikCvImageBuffer& _outImage,
			const uint32_t fgcolor)
	{
		const int32_t widthBot = 0;
		const int32_t widthTop = m_inImageDesc.m_width - 1;
		const int32_t heightBot = 0;
		const int32_t heightTop = m_inImageDesc.m_height - 1;
		drawOutputPixelBound(x, y, widthBot, widthTop, heightBot, heightTop,
				_outImage, fgcolor);
	}

	// Fill rectangle region
	void __attribute__((always_inline)) drawFillRectangle(const int32_t x,
			const int32_t y, const uint32_t width, const uint32_t height,
			const TrikCvImageBuffer& _outImage, const uint32_t fgcolor)
	{
		int32_t xi, yi;
		for (yi = y; yi < y + height; yi++)
		{
			for (xi = x; xi < x + width; xi++)
			{
				_draw_point(xi, yi, _outImage, fgcolor);
			}
		}
	}

	// Draw line
	void __attribute__((always_inline)) drawLine(int32_t x0,
			int32_t y0, const int32_t x1, const int32_t y1,
			const TrikCvImageBuffer& _outImage, const uint32_t fgcolor)
	{
		int32_t x = x1 - x0;
		int32_t y = y1 - y0;
		int32_t dx = abs(x), sx = x0 < x1 ? 1 : -1;
		int32_t dy = -abs(y), sy = y0 < y1 ? 1 : -1;
		int32_t err = dx + dy, e2;
		for (;;)
		{
			_draw_point(x0, y0, _outImage, fgcolor);
			e2 = 2 * err;
			if (e2 >= dy)
			{
				if (x0 == x1)
					break;
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx)
			{
				if (y0 == y1)
					break;
				err += dx;
				y0 += sy;
			}
		}
	}

	// Draw char
	void __attribute__((always_inline)) drawChar(uint8_t ascii,
			const int32_t poX, const int32_t poY, const uint32_t size,
			const TrikCvImageBuffer& _outImage, const uint32_t fgcolor)
	{
		uint8_t temp = 0, j, k;
		if ((ascii >= 32) && (ascii <= 255))
		{
			;
		}
		else
		{
			ascii = '?' - 32;
		}
		for (int i = 0; i < FONT_X; i++)
		//for (int i = FONT_X - 1 ; i >= 0; i -- )
		{
			j = FONT_X - (i + 1);
			if ((ascii >= 0x20) && (ascii <= 0x7F))
			{
				temp = Font16x16[ascii - 0x20][j];
			}
			else if (ascii >= 0xC0)
			{
				temp = Font16x16[ascii - 0x65][j];
			}
			k = j / 8;
			for (uint8_t f = 0; f < FONT_Y; f++)
			{
				if ((temp >> f) & 0x01)
				{
					if (size == 0)
						_draw_point(poX + j - (k * 8), poY + f + (k * 8),
								_outImage, fgcolor);
					else
						drawFillRectangle(poX + j * size - (k * 8) * size,
								poY + f * size + (k * 8) * size, size, size,
								_outImage, fgcolor);
				}
			}
		}
	}

	// Draw string
	void __attribute__((always_inline)) drawString(const char *in_str,
			int32_t poX, int32_t poY, const uint32_t size,
			const TrikCvImageBuffer& _outImage, const uint32_t fgcolor)
	{
		while (*in_str)
		{
			if ((poX + FONT_SPACE) > m_inImageDesc.m_width)
			{
				poX = 1;
				poY = poY + FONT_X * size;
			}
			drawChar(*in_str, poX, poY, size, _outImage, fgcolor);
			if (size > 0)
				poX += FONT_SPACE * size;
			else
				poX += FONT_SPACE;
			*in_str++;
		}
	}

	// Convolution 2 arrays
	int32_t __attribute__((always_inline)) conv_func(const float ar1[], const int32_t sz1
					, float ar2[], const int32_t sz2
					, float ar3[])
	{
		int32_t i;
		int32_t sz3 = sz1 + sz2 - 1;
		for (i = 0; i < sz3; i++)
		{
			int32_t j, jmin, jmax;
			ar3[i] = 0;
			jmin = (i >= sz2 - 1) ? i - (sz2 - 1) : 0;
			jmax = (i < sz1 - 1) ? i : sz1 - 1;
			for (j = jmin; j <= jmax; j++)
			{
				ar3[i] += ar1[j] * ar2[i - j];
			}
		}
		return sz3;
	}

	// Get index of maximum value element
	int32_t __attribute__((always_inline)) get_max_index(const float ar[], const int32_t sz)
	{
		int32_t maxidx = 0;
		float maxval = -1e-16;
		for (int32_t i = 0; i < sz; i++)
		{
			if (ar[i] > maxval)
			{
				maxval = ar[i];
				maxidx = i;
			}
		}
		return maxidx;
	}


public:
	virtual bool setup(const TrikCvImageDesc& _inImageDesc,
			const TrikCvImageDesc& _outImageDesc, int8_t* _fastRam,
			size_t _fastRamSize)
	{
		m_inImageDesc = _inImageDesc;
		m_outImageDesc = _outImageDesc;

		if (m_inImageDesc.m_width < 0 || m_inImageDesc.m_height < 0
				|| m_inImageDesc.m_width % 32 != 0 || m_inImageDesc.m_height
				% 4 != 0)
			return false;

#define min(x,y) x < y ? x : y;
		const double
				srcToDstShift =
						min(static_cast<double>(m_outImageDesc.m_width)/m_inImageDesc.m_width,
								static_cast<double>(m_outImageDesc.m_height)/m_inImageDesc.m_height);

		const uint32_t widthIn = _inImageDesc.m_width;
		const uint32_t widthOut = _outImageDesc.m_width;
		uint32_t* restrict p_wi2wo = s_wi2wo;
		for (int i = 0; i < widthIn; i++)
		{
			*(p_wi2wo++) = i * srcToDstShift;
		}

		const uint32_t heightIn = _inImageDesc.m_height;
		const uint32_t heightOut = _outImageDesc.m_height;
		uint32_t* restrict p_hi2ho = s_hi2ho;
		for (uint32_t i = 0; i < heightIn; i++)
		{
			*(p_hi2ho++) = i * srcToDstShift;
		}

		/* Static member initialization on first instance creation */
		if (s_mult43_div == NULL || s_mult255_div == NULL)
		{
			if (_fastRamSize < (1u << 8) * sizeof(*s_mult43_div) + (1u << 8)
					* sizeof(*s_mult255_div))
				return false;

			s_mult43_div = reinterpret_cast<typeof(s_mult43_div)> (_fastRam);
			_fastRam += (1u << 8) * sizeof(*s_mult43_div);
			s_mult255_div = reinterpret_cast<typeof(s_mult255_div)> (_fastRam);
			_fastRam += (1u << 8) * sizeof(*s_mult255_div);

			s_mult43_div[0] = 0;
			s_mult255_div[0] = 0;
			for (uint32_t idx = 1; idx < (1u << 8); ++idx)
			{
				s_mult43_div[idx] = (43u * (1u << 8)) / idx;
				s_mult255_div[idx] = (255u * (1u << 8)) / idx;
			}
		}

		return true;
	}

	virtual bool run(const TrikCvImageBuffer& _inImage,
			TrikCvImageBuffer& _outImage, const TrikCvAlgInArgs& _inArgs,
			TrikCvAlgOutArgs& _outArgs)
	{
		const int8_t* restrict srcImageRow = _inImage.m_ptr;		// Pointer to inImage
		char s1[128];												// Temp string
		int32_t idata1, idata2, idata3, idata4;						// Data read from inImage
		const int32_t nsamples = 256;								// Number of samples to read
		float ar_left[nsamples];									// Input array of first channel
		float ar_right[nsamples];									// Input array of second channel
		float br_left[nsamples + array_count(F44100)];				// Filtered array of first channel
		float br_right[nsamples + array_count(F44100)];				// Filtered array of second channel
		double a[1];
		double b[2];

		if (m_inImageDesc.m_height * m_inImageDesc.m_lineLength
				> _inImage.m_size)
			return false;
		if (m_outImageDesc.m_height * m_outImageDesc.m_lineLength
				> _outImage.m_size)
			return false;
		_outImage.m_size = m_outImageDesc.m_height
				* m_outImageDesc.m_lineLength;

		// Draw some things
		//sprintf(s1, "b0 = %d, b5 = %d", *(srcImageRow + 0), *(srcImageRow + 5));
		//sprintf(s1, "%d %d %d", array_count(F44100), array_count(ar_left), array_count(br_left));
		DSPF_sp_autocor(ar_left, ar_right, 10, 10);
		cosdp_v(a, b, 1);
		sprintf(s1, "\0");
		drawString(s1, 0, 0, 1, _outImage, CL_YELLOW);
		// Copy data to float point arrays
		for (int32_t i = 0; i < nsamples; i ++)
		{
			idata1 = *(srcImageRow + 0);
			idata2 = *(srcImageRow + 1);
			idata3 = *(srcImageRow + 2);
			idata4 = *(srcImageRow + 3);
			ar_left[i] = (float)((idata1 << 8) + idata2) / 32768;
			ar_right[i] = (float)((idata3 << 8) + idata4) / 32768;
			srcImageRow+=4;
		}
		// Conversion with filter
		conv_func(F44100, array_count(F44100), ar_left, array_count(ar_left), br_left);
		conv_func(F44100, array_count(F44100), ar_right, array_count(ar_right), br_right);
		sprintf(s1, "%f %f", ar_left[1], ar_left[100]);
		drawString(s1, 0, 0, 1, _outImage, CL_YELLOW);


		return true;
	}
};

uint16_t* restrict SomethingDetector<TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_YUV422,
TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_RGB565X>::s_mult43_div = NULL;
uint16_t* restrict SomethingDetector<TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_YUV422,
TRIK_VIDTRANSCODE_CV_VIDEO_FORMAT_RGB565X>::s_mult255_div = NULL;

} /* **** **** **** **** **** * namespace cv * **** **** **** **** **** */

} /* **** **** **** **** **** * namespace trik * **** **** **** **** **** */

#endif /* CV_SOMETHING_DETECTOR_SEQPASS_HPP_ */