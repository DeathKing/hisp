
HObject hp_eval(Hobject sexp, HObject env)
{
    if (SELF_EVALUTE_P(sexp)) return sexp;
    else if (HID_P(sexp))     return hp_lookup_var(exp, env);
    else if (HDEFINE_P(sexp)) return hp_eval_definition(exp, env);
    else if (HASSIGN_P(sexp)) return hp_eval_assignment(exp, env);
    else if (HIF_P(sexp))     return hp_eval_if(exp, env);
    else if (HLAMBDA_P(sexp)) {
        HObject para = HLAMBDA(sexp)->para;
        HObject body = HLAMBDA(sexp)->body;
        return hp_make_procedure(para, body, env);
    } else if (HPAIR_P(sexp)) {
        HObject proc = hp_eval(HPAIR(sexp)->car, env);
        HObject args = env_list_value(HPAIR(sexp)->cdr, env);
        return hp_apply(proc, args);
    } else {
        return hp_error(RUNTIME_ERROR, "Unknow operation.");
    }
}

HObject hp_eval_if()
{

}

HObject hp_eval_definition()
{

}