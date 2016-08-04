#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plumber.h"


int sporth_get(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;

    char *ftname;
    SPFLOAT **var;
    sp_ftbl *ft;
    switch(pd->mode){
        case PLUMBER_CREATE:
            var = malloc(sizeof(SPFLOAT *));
            plumber_add_ugen(pd, SPORTH_GET, var);
            if(sporth_check_args(stack, "s") != SPORTH_OK) {
               fprintf(stderr,"Not enough arguments for get\n");
                return PLUMBER_NOTOK;
            }
            ftname = sporth_stack_pop_string(stack);
            if(plumber_ftmap_search(pd, ftname, &ft) == PLUMBER_NOTOK) {
                fprintf(stderr, "get: could not find table '%s'\n", ftname);
                stack->error++;
                return PLUMBER_NOTOK;
            }
            *var = &ft->tbl[0];
            sporth_stack_push_float(stack, **var);
            break;

        case PLUMBER_INIT:
            var = pd->last->ud;
            sporth_stack_pop_string(stack);
            sporth_stack_push_float(stack, **var);
            break;

        case PLUMBER_COMPUTE:
            var = pd->last->ud;
            sporth_stack_push_float(stack, **var);
            break;

        case PLUMBER_DESTROY:
            var = pd->last->ud;
            free(var);
            break;

        default:
            fprintf(stderr,"Error: Unknown mode!");
            break;
    }
    return PLUMBER_OK;
}

int sporth_set(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;

    char *ftname;
    SPFLOAT **var;
    sp_ftbl *ft;
    SPFLOAT val;
    switch(pd->mode){
        case PLUMBER_CREATE:
            var = malloc(sizeof(SPFLOAT *));
            plumber_add_ugen(pd, SPORTH_SET, var);
            if(sporth_check_args(stack, "fs") != SPORTH_OK) {
               fprintf(stderr,"Not enough arguments for get\n");
                return PLUMBER_NOTOK;
            }
            ftname = sporth_stack_pop_string(stack);
            val = sporth_stack_pop_float(stack);
            if(plumber_ftmap_search(pd, ftname, &ft) == PLUMBER_NOTOK) {
                fprintf(stderr, "get: could not find table '%s'\n", ftname);
                stack->error++;
                return PLUMBER_NOTOK;
            }
            *var = &ft->tbl[0];
            **var = val;
            break;

        case PLUMBER_INIT:
            var = pd->last->ud;
            sporth_stack_pop_string(stack);
            val = sporth_stack_pop_float(stack);
            **var = val;
            break;

        case PLUMBER_COMPUTE:
            var = pd->last->ud;
            val = sporth_stack_pop_float(stack);
            **var = val;
            break;

        case PLUMBER_DESTROY:
            var = pd->last->ud;
            free(var);
            break;

        default:
            fprintf(stderr,"Error: Unknown mode!");
            break;
    }
    return PLUMBER_OK;
}

int sporth_var(sporth_stack *stack, void *ud)
{
    plumber_data *pd = ud;

    sp_ftbl *ft;
    char *str;

    switch(pd->mode){
        case PLUMBER_CREATE:
            plumber_add_ugen(pd, SPORTH_VAR, NULL);
            if(sporth_check_args(stack, "s") != SPORTH_OK) {
                fprintf(stderr, "Init: not enough arguments for var\n");
                return PLUMBER_NOTOK;
            }
            str = sporth_stack_pop_string(stack);
#ifdef DEBUG_MODE
            fprintf(stderr, "var: creating table %s\n", str);
#endif
            sp_ftbl_create(pd->sp, &ft, 1);
            plumber_ftmap_add(pd, str, ft);
            break;

        case PLUMBER_INIT:
            sporth_stack_pop_string(stack);
            break;

        case PLUMBER_COMPUTE:
            break;

        case PLUMBER_DESTROY:
            break;

        default:
           fprintf(stderr, "Error: Unknown mode!\n");
           break;
    }
    return PLUMBER_OK;
}

int plumber_create_var(plumber_data *pd, char *name, SPFLOAT **var)
{
    sp_ftbl *ft;
    sp_ftbl_create(pd->sp, &ft, 1);
    plumber_ftmap_add(pd, name, ft);
    *var = &ft->tbl[0];
    return PLUMBER_OK;
}
