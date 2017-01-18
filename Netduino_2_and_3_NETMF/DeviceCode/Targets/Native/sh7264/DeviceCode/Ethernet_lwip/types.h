////////////////////////////////////////////////////////////////////////////
//	Author:		Brandon Wong
//	Module:		Base Registers
//	Version:		0.00
//	File:		types.h
//	Date:		1/21/02 11:17AM
//
//	Summary:		typedefs for types
////////////////////////////////////////////////////////////////////////////

#ifndef __TYPES_H__
#define __TYPES_H__ 1

	typedef unsigned int			uint32_t;
	typedef unsigned short			uint16_t;
	typedef unsigned char			uint8_t;

	typedef volatile int			vint32_t;
	typedef volatile short			vint16_t;
	typedef volatile char			vint8_t;

	typedef volatile unsigned int	vuint32_t;
	typedef volatile unsigned short	vuint16_t;
	typedef volatile unsigned char	vuint8_t;

	typedef union
	{
		uint32_t	dword;
		uint16_t	word[2];
		uint8_t		byte[4];
	} converter_t;

#endif