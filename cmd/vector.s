	/* 外部シンボル */
	
	/* _startシンボルを別コードから参照できるようにする */
.globl	_start

	/* ベクタテーブル
	 * 割り込みや例外が発生するとPCをこれらのアドレスにセットする
	 * b命令で記述しても良いが、32Mbyte以上はなれたアドレスへジャンプできない
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

	/* リセットハンドラ */
reset_handler:
	/* _startの開始アドレスは0x8000でありARMのベクタベースアドレスではなく、
	 * このままでは例外や割り込みを正しくハンドリングできない。
	 * そこで_startからhtbl_fiqまでの16x4byteを0x0へコピーする
	 */
	mov	r0, #0x8000
	mov	r1, #0x0000
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	/* スーパーバイザモードに変更(0x13), 割り込み禁止(0xc0), スタックポインタの設定 */
	mov	r0, #(0x13|0xc0)
	msr	cpsr_c, r0
	mov	sp, #0x10000000
	
	/* .textの後ろにある初期化データを.dataにコピーする */
	ldr	r1, =__text_end__
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__
1:	cmp	r2, r3
	ldrlo	r0, [r1], #4
	strlo	r0, [r2], #4
	blo	1b
	
	/* BSS領域をゼロクリアする */
	mov	r0, #0
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__
2:	cmp	r1, r2
	strlo	r0, [r1], #4
	blo	2b

	/* main()へジャンプ */
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
