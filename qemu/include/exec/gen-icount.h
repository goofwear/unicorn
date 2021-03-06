#ifndef GEN_ICOUNT_H
#define GEN_ICOUNT_H 1

#include "qemu/timer.h"

/* Helpers for instruction counting code generation.  */

//static TCGArg *icount_arg;
//static TCGLabel *icount_label;
//static TCGLabel *exitreq_label;

static inline void gen_tb_start(TCGContext *tcg_ctx)
{
    //TCGv_i32 count, flag, imm;
    TCGv_i32 flag;
    //int i;

    tcg_ctx->exitreq_label = gen_new_label(tcg_ctx);
    flag = tcg_temp_new_i32(tcg_ctx);
    tcg_gen_ld_i32(tcg_ctx, flag, tcg_ctx->cpu_env,
                   offsetof(CPUState, tcg_exit_req) - ENV_OFFSET);
    tcg_gen_brcondi_i32(tcg_ctx, TCG_COND_NE, flag, 0, tcg_ctx->exitreq_label);
    tcg_temp_free_i32(tcg_ctx, flag);

#if 0
    if (!(s->tb->cflags & CF_USE_ICOUNT)) {
        return;
    }

    icount_label = gen_new_label(tcg_ctx);
    count = tcg_temp_local_new_i32(tcg_ctx);
    tcg_gen_ld_i32(tcg_ctx, count, cpu_env,
                   -ENV_OFFSET + offsetof(CPUState, icount_decr.u32));
    imm = tcg_temp_new_i32(tcg_ctx);
    tcg_gen_movi_i32(tcg_ctx, imm, 0xdeadbeef);


    /* This is a horrid hack to allow fixing up the value later.  */
    i = *tcg_ctx->gen_last_op_idx;
    i = *tcg_ctx->gen_op_buf[i].args;
    icount_arg = &tcg_ctx->gen_opparam_buf[i + 1];

    tcg_gen_sub_i32(tcg_ctx, count, count, imm);
    tcg_temp_free_i32(tcg_ctx, imm);


    tcg_gen_brcondi_i32(tcg_ctx, TCG_COND_LT, count, 0, icount_label);
    tcg_gen_st16_i32(tcg_ctx, count, cpu_env,
                     -ENV_OFFSET + offsetof(CPUState, icount_decr.u16.low));
    tcg_temp_free_i32(tcg_ctx, count);
#endif
}

static inline void gen_tb_end(TCGContext *tcg_ctx, TranslationBlock *tb, int num_insns)
{
    gen_set_label(tcg_ctx, tcg_ctx->exitreq_label);
    tcg_gen_exit_tb(tcg_ctx, (uintptr_t)tb + TB_EXIT_REQUESTED);

#if 0
    if (use_icount) {
        *icount_arg = num_insns;
        gen_set_label(icount_label);
        tcg_gen_exit_tb((uintptr_t)tb + TB_EXIT_ICOUNT_EXPIRED);
    }
#endif

     /* Terminate the linked list.  */
    tcg_ctx->gen_op_buf[tcg_ctx->gen_last_op_idx].next = -1;
}

#if 0
static inline void gen_io_start(void)
{
    TCGv_i32 tmp = tcg_const_i32(1);
    tcg_gen_st_i32(tmp, cpu_env, -ENV_OFFSET + offsetof(CPUState, can_do_io));
    tcg_temp_free_i32(tmp);
}

static inline void gen_io_end(void)
{
    TCGv_i32 tmp = tcg_const_i32(0);
    tcg_gen_st_i32(tmp, cpu_env, -ENV_OFFSET + offsetof(CPUState, can_do_io));
    tcg_temp_free_i32(tmp);
}
#endif

#endif
