
bin/BlueOS.bin:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:	4b0a      	ldr	r3, [pc, #40]	; (0x2c)
   2:	681a      	ldr	r2, [r3, #0]
   4:	f022 02f0 	bic.w	r2, r2, #240	; 0xf0
   8:	601a      	str	r2, [r3, #0]
   a:	681a      	ldr	r2, [r3, #0]
   c:	f042 0290 	orr.w	r2, r2, #144	; 0x90
  10:	601a      	str	r2, [r3, #0]
  12:	f8d3 2800 	ldr.w	r2, [r3, #2048]	; 0x800
  16:	f422 0270 	bic.w	r2, r2, #15728640	; 0xf00000
  1a:	f8c3 2800 	str.w	r2, [r3, #2048]	; 0x800
  1e:	f8d3 2800 	ldr.w	r2, [r3, #2048]	; 0x800
  22:	f442 1200 	orr.w	r2, r2, #2097152	; 0x200000
  26:	f8c3 2800 	str.w	r2, [r3, #2048]	; 0x800
  2a:	4770      	bx	lr
  2c:	0804      	lsrs	r4, r0, #32
  2e:	4001      	ands	r1, r0
  30:	4b03      	ldr	r3, [pc, #12]	; (0x40)
  32:	681b      	ldr	r3, [r3, #0]
  34:	061b      	lsls	r3, r3, #24
  36:	d5fb      	bpl.n	0x30
  38:	4b02      	ldr	r3, [pc, #8]	; (0x44)
  3a:	6018      	str	r0, [r3, #0]
  3c:	4770      	bx	lr
  3e:	bf00      	nop
  40:	3800      	subs	r0, #0
  42:	4001      	ands	r1, r0
  44:	3804      	subs	r0, #4
  46:	4001      	ands	r1, r0
  48:	b510      	push	{r4, lr}
  4a:	1e44      	subs	r4, r0, #1
  4c:	f814 0f01 	ldrb.w	r0, [r4, #1]!
  50:	b110      	cbz	r0, 0x58
  52:	f7ff ffed 	bl	0x30
  56:	e7f9      	b.n	0x4c
  58:	bd10      	pop	{r4, pc}
  5a:	0000      	movs	r0, r0
  5c:	4a0b      	ldr	r2, [pc, #44]	; (0x8c)
  5e:	490c      	ldr	r1, [pc, #48]	; (0x90)
  60:	2300      	movs	r3, #0
  62:	2045      	movs	r0, #69	; 0x45
  64:	600b      	str	r3, [r1, #0]
  66:	6013      	str	r3, [r2, #0]
  68:	6053      	str	r3, [r2, #4]
  6a:	6093      	str	r3, [r2, #8]
  6c:	f842 0c08 	str.w	r0, [r2, #-8]
  70:	680a      	ldr	r2, [r1, #0]
  72:	4808      	ldr	r0, [pc, #32]	; (0x94)
  74:	f442 5250 	orr.w	r2, r2, #13312	; 0x3400
  78:	f042 020c 	orr.w	r2, r2, #12
  7c:	600a      	str	r2, [r1, #0]
  7e:	4a06      	ldr	r2, [pc, #24]	; (0x98)
  80:	7013      	strb	r3, [r2, #0]
  82:	4a06      	ldr	r2, [pc, #24]	; (0x9c)
  84:	f882 30fe 	strb.w	r3, [r2, #254]	; 0xfe
  88:	f7ff bfde 	b.w	0x48
  8c:	3810      	subs	r0, #16
  8e:	4001      	ands	r1, r0
  90:	380c      	subs	r0, #12
  92:	4001      	ands	r1, r0
  94:	06ac      	lsls	r4, r5, #26
  96:	0800      	lsrs	r0, r0, #32
  98:	0000      	movs	r0, r0
  9a:	2000      	movs	r0, #0
  9c:	0001      	movs	r1, r0
  9e:	2000      	movs	r0, #0
  a0:	4b04      	ldr	r3, [pc, #16]	; (0xb4)
  a2:	681b      	ldr	r3, [r3, #0]
  a4:	069b      	lsls	r3, r3, #26
  a6:	bf43      	ittte	mi
  a8:	4b03      	ldrmi	r3, [pc, #12]	; (0xb8)
  aa:	6818      	ldrmi	r0, [r3, #0]
  ac:	b2c0      	uxtbmi	r0, r0
  ae:	20ff      	movpl	r0, #255	; 0xff
  b0:	4770      	bx	lr
  b2:	bf00      	nop
  b4:	3800      	subs	r0, #0
  b6:	4001      	ands	r1, r0
  b8:	3804      	subs	r0, #4
  ba:	4001      	ands	r1, r0
  bc:	b538      	push	{r3, r4, r5, lr}
  be:	f7ff ffef 	bl	0xa0
  c2:	280a      	cmp	r0, #10
  c4:	4c13      	ldr	r4, [pc, #76]	; (0x114)
  c6:	d10f      	bne.n	0xe8
  c8:	2300      	movs	r3, #0
  ca:	7822      	ldrb	r2, [r4, #0]
  cc:	1c5d      	adds	r5, r3, #1
  ce:	b2db      	uxtb	r3, r3
  d0:	429a      	cmp	r2, r3
  d2:	d305      	bcc.n	0xe0
  d4:	4a10      	ldr	r2, [pc, #64]	; (0x118)
  d6:	5cd0      	ldrb	r0, [r2, r3]
  d8:	f7ff ffaa 	bl	0x30
  dc:	462b      	mov	r3, r5
  de:	e7f4      	b.n	0xca
  e0:	2300      	movs	r3, #0
  e2:	7023      	strb	r3, [r4, #0]
  e4:	480d      	ldr	r0, [pc, #52]	; (0x11c)
  e6:	e006      	b.n	0xf6
  e8:	2808      	cmp	r0, #8
  ea:	d108      	bne.n	0xfe
  ec:	7823      	ldrb	r3, [r4, #0]
  ee:	b133      	cbz	r3, 0xfe
  f0:	3b01      	subs	r3, #1
  f2:	480b      	ldr	r0, [pc, #44]	; (0x120)
  f4:	7023      	strb	r3, [r4, #0]
  f6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
  fa:	f7ff bfa5 	b.w	0x48
  fe:	7823      	ldrb	r3, [r4, #0]
 100:	2bfd      	cmp	r3, #253	; 0xfd
 102:	d8ed      	bhi.n	0xe0
 104:	4a03      	ldr	r2, [pc, #12]	; (0x114)
 106:	3301      	adds	r3, #1
 108:	b2db      	uxtb	r3, r3
 10a:	7013      	strb	r3, [r2, #0]
 10c:	4a02      	ldr	r2, [pc, #8]	; (0x118)
 10e:	54d0      	strb	r0, [r2, r3]
 110:	bd38      	pop	{r3, r4, r5, pc}
 112:	bf00      	nop
 114:	0000      	movs	r0, r0
 116:	2000      	movs	r0, #0
 118:	0001      	movs	r1, r0
 11a:	2000      	movs	r0, #0
 11c:	06ac      	lsls	r4, r5, #26
 11e:	0800      	lsrs	r0, r0, #32
 120:	06b7      	lsls	r7, r6, #26
 122:	0800      	lsrs	r0, r0, #32
 124:	4a0b      	ldr	r2, [pc, #44]	; (0x154)
 126:	6813      	ldr	r3, [r2, #0]
 128:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
 12c:	6013      	str	r3, [r2, #0]
 12e:	f64f 73ff 	movw	r3, #65535	; 0xffff
 132:	3b01      	subs	r3, #1
 134:	b29b      	uxth	r3, r3
 136:	2b00      	cmp	r3, #0
 138:	d1fb      	bne.n	0x132
 13a:	4a07      	ldr	r2, [pc, #28]	; (0x158)
 13c:	6813      	ldr	r3, [r2, #0]
 13e:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
 142:	6013      	str	r3, [r2, #0]
 144:	f64f 73ff 	movw	r3, #65535	; 0xffff
 148:	3b01      	subs	r3, #1
 14a:	b29b      	uxth	r3, r3
 14c:	2b00      	cmp	r3, #0
 14e:	d1fb      	bne.n	0x148
 150:	4770      	bx	lr
 152:	bf00      	nop
 154:	1010      	asrs	r0, r2, #32
 156:	4001      	ands	r1, r0
 158:	1014      	asrs	r4, r2, #32
 15a:	4001      	ands	r1, r0
 15c:	b672      	cpsid	i
 15e:	4a19      	ldr	r2, [pc, #100]	; (0x1c4)
 160:	6813      	ldr	r3, [r2, #0]
 162:	f043 6300 	orr.w	r3, r3, #134217728	; 0x8000000
 166:	6013      	str	r3, [r2, #0]
 168:	4a17      	ldr	r2, [pc, #92]	; (0x1c8)
 16a:	7813      	ldrb	r3, [r2, #0]
 16c:	b133      	cbz	r3, 0x17c
 16e:	f3ef 8109 	mrs	r1, PSP
 172:	7813      	ldrb	r3, [r2, #0]
 174:	4815      	ldr	r0, [pc, #84]	; (0x1cc)
 176:	b2db      	uxtb	r3, r3
 178:	f840 1023 	str.w	r1, [r0, r3, lsl #2]
 17c:	4b14      	ldr	r3, [pc, #80]	; (0x1d0)
 17e:	7819      	ldrb	r1, [r3, #0]
 180:	3101      	adds	r1, #1
 182:	b2c9      	uxtb	r1, r1
 184:	7019      	strb	r1, [r3, #0]
 186:	4913      	ldr	r1, [pc, #76]	; (0x1d4)
 188:	7818      	ldrb	r0, [r3, #0]
 18a:	7809      	ldrb	r1, [r1, #0]
 18c:	4288      	cmp	r0, r1
 18e:	bf24      	itt	cs
 190:	2100      	movcs	r1, #0
 192:	7019      	strbcs	r1, [r3, #0]
 194:	781b      	ldrb	r3, [r3, #0]
 196:	4910      	ldr	r1, [pc, #64]	; (0x1d8)
 198:	b2db      	uxtb	r3, r3
 19a:	f811 3033 	ldrb.w	r3, [r1, r3, lsl #3]
 19e:	b2db      	uxtb	r3, r3
 1a0:	7013      	strb	r3, [r2, #0]
 1a2:	7813      	ldrb	r3, [r2, #0]
 1a4:	b14b      	cbz	r3, 0x1ba
 1a6:	4b08      	ldr	r3, [pc, #32]	; (0x1c8)
 1a8:	4a08      	ldr	r2, [pc, #32]	; (0x1cc)
 1aa:	781b      	ldrb	r3, [r3, #0]
 1ac:	b2db      	uxtb	r3, r3
 1ae:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
 1b2:	b662      	cpsie	i
 1b4:	f06f 0f02 	mvn.w	pc, #2
 1b8:	4770      	bx	lr
 1ba:	b662      	cpsie	i
 1bc:	f06f 0f06 	mvn.w	pc, #6
 1c0:	4770      	bx	lr
 1c2:	bf00      	nop
 1c4:	ed04 e000 	stc	0, cr14, [r4, #-0]
 1c8:	0290      	lsls	r0, r2, #10
 1ca:	2000      	movs	r0, #0
 1cc:	0294      	lsls	r4, r2, #10
 1ce:	2000      	movs	r0, #0
 1d0:	0291      	lsls	r1, r2, #10
 1d2:	2000      	movs	r0, #0
 1d4:	02bc      	lsls	r4, r7, #10
 1d6:	2000      	movs	r0, #0
 1d8:	0100      	lsls	r0, r0, #4
 1da:	2000      	movs	r0, #0
 1dc:	b508      	push	{r3, lr}
 1de:	f000 f80d 	bl	0x1fc
 1e2:	f000 f843 	bl	0x26c
 1e6:	f7ff ff0b 	bl	0x0
 1ea:	f7ff ff37 	bl	0x5c
 1ee:	f7ff ff65 	bl	0xbc
 1f2:	f7ff ff97 	bl	0x124
 1f6:	e7fa      	b.n	0x1ee
 1f8:	e7fe      	b.n	0x1f8
 1fa:	0000      	movs	r0, r0
 1fc:	4b18      	ldr	r3, [pc, #96]	; (0x260)
 1fe:	b510      	push	{r4, lr}
 200:	681a      	ldr	r2, [r3, #0]
 202:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
 206:	601a      	str	r2, [r3, #0]
 208:	461a      	mov	r2, r3
 20a:	6813      	ldr	r3, [r2, #0]
 20c:	4814      	ldr	r0, [pc, #80]	; (0x260)
 20e:	039c      	lsls	r4, r3, #14
 210:	d5fb      	bpl.n	0x20a
 212:	4b14      	ldr	r3, [pc, #80]	; (0x264)
 214:	211a      	movs	r1, #26
 216:	6019      	str	r1, [r3, #0]
 218:	f6a3 73fc 	subw	r3, r3, #4092	; 0xffc
 21c:	681c      	ldr	r4, [r3, #0]
 21e:	f1a1 61e8 	sub.w	r1, r1, #121634816	; 0x7400000
 222:	390e      	subs	r1, #14
 224:	4021      	ands	r1, r4
 226:	6019      	str	r1, [r3, #0]
 228:	6819      	ldr	r1, [r3, #0]
 22a:	f441 11e8 	orr.w	r1, r1, #1900544	; 0x1d0000
 22e:	f441 6180 	orr.w	r1, r1, #1024	; 0x400
 232:	6019      	str	r1, [r3, #0]
 234:	6801      	ldr	r1, [r0, #0]
 236:	f041 7180 	orr.w	r1, r1, #16777216	; 0x1000000
 23a:	6001      	str	r1, [r0, #0]
 23c:	6811      	ldr	r1, [r2, #0]
 23e:	0189      	lsls	r1, r1, #6
 240:	d5fc      	bpl.n	0x23c
 242:	681a      	ldr	r2, [r3, #0]
 244:	f042 0202 	orr.w	r2, r2, #2
 248:	601a      	str	r2, [r3, #0]
 24a:	681a      	ldr	r2, [r3, #0]
 24c:	f002 020c 	and.w	r2, r2, #12
 250:	2a08      	cmp	r2, #8
 252:	d1fa      	bne.n	0x24a
 254:	4b04      	ldr	r3, [pc, #16]	; (0x268)
 256:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
 25a:	601a      	str	r2, [r3, #0]
 25c:	bd10      	pop	{r4, pc}
 25e:	bf00      	nop
 260:	1000      	asrs	r0, r0, #32
 262:	4002      	ands	r2, r0
 264:	2000      	movs	r0, #0
 266:	4002      	ands	r2, r0
 268:	1008      	asrs	r0, r1, #32
 26a:	4002      	ands	r2, r0
 26c:	4a03      	ldr	r2, [pc, #12]	; (0x27c)
 26e:	6813      	ldr	r3, [r2, #0]
 270:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
 274:	f043 0305 	orr.w	r3, r3, #5
 278:	6013      	str	r3, [r2, #0]
 27a:	4770      	bx	lr
 27c:	1018      	asrs	r0, r3, #32
 27e:	4002      	ands	r2, r0
 280:	4b07      	ldr	r3, [pc, #28]	; (0x2a0)
 282:	681a      	ldr	r2, [r3, #0]
 284:	3201      	adds	r2, #1
 286:	601a      	str	r2, [r3, #0]
 288:	4a06      	ldr	r2, [pc, #24]	; (0x2a4)
 28a:	6812      	ldr	r2, [r2, #0]
 28c:	681b      	ldr	r3, [r3, #0]
 28e:	429a      	cmp	r2, r3
 290:	bf01      	itttt	eq
 292:	4a05      	ldreq	r2, [pc, #20]	; (0x2a8)
 294:	6813      	ldreq	r3, [r2, #0]
 296:	f043 5380 	orreq.w	r3, r3, #268435456	; 0x10000000
 29a:	6013      	streq	r3, [r2, #0]
 29c:	4770      	bx	lr
 29e:	bf00      	nop
 2a0:	02c0      	lsls	r0, r0, #11
 2a2:	2000      	movs	r0, #0
 2a4:	02c4      	lsls	r4, r0, #11
 2a6:	2000      	movs	r0, #0
 2a8:	ed04 e000 	stc	0, cr14, [r4, #-0]
 2ac:	420a      	tst	r2, r1
 2ae:	756c      	strb	r4, [r5, #21]
 2b0:	5f65      	ldrsh	r5, [r4, r5]
 2b2:	534f      	strh	r7, [r1, r5]
 2b4:	3e5c      	subs	r6, #92	; 0x5c
 2b6:	0800      	lsrs	r0, r0, #32
 2b8:	0820      	lsrs	r0, r4, #32
	...
