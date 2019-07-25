#ifndef BOS_DEFS_H
#define BOS_DEFS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

#define BIT0    (uint32_t)( 0x00000001 << 0 )
#define BIT1    (uint32_t)( 0x00000001 << 1 )
#define BIT2    (uint32_t)( 0x00000001 << 2 )
#define BIT3    (uint32_t)( 0x00000001 << 3 )
#define BIT4    (uint32_t)( 0x00000001 << 4 )
#define BIT5    (uint32_t)( 0x00000001 << 5 )
#define BIT6    (uint32_t)( 0x00000001 << 6 )
#define BIT7    (uint32_t)( 0x00000001 << 7 )
#define BIT8    (uint32_t)( 0x00000001 << 8 )
#define BIT9    (uint32_t)( 0x00000001 << 9 )
#define BIT10   (uint32_t)( 0x00000001 << 10 )
#define BIT11   (uint32_t)( 0x00000001 << 11 )
#define BIT12   (uint32_t)( 0x00000001 << 12 )
#define BIT13   (uint32_t)( 0x00000001 << 13 )
#define BIT14   (uint32_t)( 0x00000001 << 14 )
#define BIT15   (uint32_t)( 0x00000001 << 15 )
#define BIT16   (uint32_t)( 0x00000001 << 16 )
#define BIT17   (uint32_t)( 0x00000001 << 17 )
#define BIT18   (uint32_t)( 0x00000001 << 18 )
#define BIT19   (uint32_t)( 0x00000001 << 19 )
#define BIT20   (uint32_t)( 0x00000001 << 20 )
#define BIT21   (uint32_t)( 0x00000001 << 21 )
#define BIT22   (uint32_t)( 0x00000001 << 22 )
#define BIT23   (uint32_t)( 0x00000001 << 23 )
#define BIT24   (uint32_t)( 0x00000001 << 24 )
#define BIT25   (uint32_t)( 0x00000001 << 25 )
#define BIT26   (uint32_t)( 0x00000001 << 26 )
#define BIT27   (uint32_t)( 0x00000001 << 27 )
#define BIT28   (uint32_t)( 0x00000001 << 28 )
#define BIT29   (uint32_t)( 0x00000001 << 29 )
#define BIT30   (uint32_t)( 0x00000001 << 30 )
#define BIT31   (uint32_t)( 0x00000001 << 31 )
#define BIT32   (uint32_t)( 0x00000001 << 32 )

#define LOW_HALF_WORD  (uint32_t)( 0x0000FFFF )
#define HIGH_HALF_WORD (uint32_t)( 0xFFFF0000 )

#define WORD           (uint32_t)( 0xFFFFFFFF )

#define BLOCK_ALIGNMENT ( 0x0800 )

#define IN_OS              __attribute__(( section( ".text.blueos" )))
#define START_OS           __attribute__(( section( ".text.blueos" )), aligned( BLOCK_ALIGNMENT ))

#endif
