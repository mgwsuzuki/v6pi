	/* �O���V���{�� */
	
	/* _start�V���{����ʃR�[�h����Q�Ƃł���悤�ɂ��� */
.globl	_start

	/* �x�N�^�e�[�u��
	 * ���荞�݂��O�����������PC�������̃A�h���X�ɃZ�b�g����
	 * b���߂ŋL�q���Ă��ǂ����A32Mbyte�ȏ�͂Ȃꂽ�A�h���X�փW�����v�ł��Ȃ�
	 */
_start:
	ldr	pc, htbl_reset
	ldr	pc, htbl_undef
	ldr	pc, htbl_swi
	ldr	pc, htbl_pabt
	ldr	pc, htbl_dabt
	ldr	pc, htbl_unused
	ldr	pc, htbl_irq
	ldr	pc, htbl_fiq

htbl_reset:
	.word	reset_handler
htbl_undef:
	.word	undef_handler
htbl_swi:
	.word	swi_handler
htbl_pabt:
	.word	pabt_handler
htbl_dabt:
	.word	dabt_handler
htbl_unused:
	.word	unused_handler
htbl_irq:
	.word	irq_handler
htbl_fiq:
	.word	fiq_handler

	/* ���Z�b�g�n���h�� */
reset_handler:
	/* _start�̊J�n�A�h���X��0x8000�ł���ARM�̃x�N�^�x�[�X�A�h���X�ł͂Ȃ��A
	 * ���̂܂܂ł͗�O�⊄�荞�݂𐳂����n���h�����O�ł��Ȃ��B
	 * ������_start����htbl_fiq�܂ł�16x4byte��0x0�փR�s�[����
	 */
	mov	r0, #0x8000
	mov	r1, #0x0000
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	/* �X�[�p�[�o�C�U���[�h�ɕύX(0x13), ���荞�݋֎~(0xc0), �X�^�b�N�|�C���^�̐ݒ� */
	mov	r0, #(0x13|0xc0)
	msr	cpsr_c, r0
	mov	sp, #0x10000000
	
	/* .text�̌��ɂ��鏉�����f�[�^��.data�ɃR�s�[���� */
	ldr	r1, =__text_end__
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__
1:	cmp	r2, r3
	ldrlo	r0, [r1], #4
	strlo	r0, [r2], #4
	blo	1b
	
	/* BSS�̈���[���N���A���� */
	mov	r0, #0
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__
2:	cmp	r1, r2
	strlo	r0, [r1], #4
	blo	2b

	/* main()�փW�����v */
	bl	main

undef_handler:
	b	undef_handler
swi_handler:
	b	swi_handler
pabt_handler:
	b	pabt_handler
dabt_handler:
	b	dabt_handler
unused_handler:
	b	unused_handler
irq_handler:
	b	irq_handler
fiq_handler:
	b	fiq_handler
