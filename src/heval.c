HObject hp_eval(HObject sexp, HObject env)
{
    if (SELF_EVALUTE_P(sexp)) {
        /* <literal> ---> <quotation> | <self-evaluating>
         * <self-evaluating> ---> <boolean> | <number> | <character> | <string>
         */
        return sexp;
    } else if (HID_P(sexp)) {
        /* indentifier usually stand for a reference to a var */
        return hp_lookup_var(exp, env);
    } else if (HDEFINE_P(sexp)) {
        /* DEFINE Special Form
         *   (define <variable> <expression>)
         *   (define (<variable> <def formals>) <body>)
         */
        return hp_eval_definition(CDR(sexp), env);
    } else if (HASSIGN_P(sexp)) {
        /* SET! Special Form
         *   (set! <variable> <expression>)
         */
        return hp_eval_assignment(CDR(sexp), env);
    } else if (HIF_P(sexp)) {
        /* IF Special Form
         *   (if <test> <consequent> <alternate>)
         */
        return hp_eval_if(exp, env);
    } else if (HLAMBDA_P(sexp)) {
        /* LAMBDA Special Form
         *   (lambda <formals> <body>)
         */
        HObject para = HLAMBDA(sexp)->para;
        HObject body = HLAMBDA(sexp)->body;
        return hp_make_procedure(para, body, env);
    } else if (HPAIR_P(sexp)) {
        HObject proc = hp_eval(HPAIR(sexp)->car, env);
        HObject args = env_list_value(HPAIR(sexp)->cdr, env);
        return hp_apply(proc, args);
    } 
    /******************************
     * Pattern Oriented EVAL here *
     ******************************/
    else {
        return hp_error(RUNTIME_ERROR, "Unknow operation.");
    }
}

HObject hp_eval_definition(HPair *sexp, HEnv *env)
{
    HObject id  = CAR(sexp);
    HObject val = hp_eval(CADR(sexp), env);

    return hp_env_define_var(id, val, env);
}
HObject hp_apply(HObject proc, HObject args)
{

    if (HTEST(hp_is_primitive_procedure(proc))) {

        return apply_primitive_procedure(proc, args);

    } else if (HTEST(is_compound_procedure(proc))) {

        HObject body    = LAMBDA_BODY(proc);
        HObject para    = LAMBDA_PARA(proc);
        HObject env     = LAMBDA_ENV(proc);

        return hp_eval(body, env_extend(para, args, env));

    } else {

        return hp_error(RUNTIME_ERROR, "unknow proc apply.");

    }
}
