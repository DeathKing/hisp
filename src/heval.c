

HObject hp_eval(HObject sexp, HObject env)
{
    if (HSELF_EVALUATE_P(sexp)) {
        /* <literal> ---> <quotation> | <self-evaluating>
         * <self-evaluating> ---> <boolean> | <number> | <character> | <string>
         */
        return sexp;
    } else if (HID_P(sexp)) {
        /* indentifier usually stand for a reference to a var */
        return hp_lookup_var(sexp, env);
    } else if (HDEFINE_P(sexp)) {
        /* DEFINE Special Form
         *   (define <variable> <expression>)
         *   (define (<variable> <def formals>) <body>)
         */
        if (list_length(sexp) < 3)
            return hp_error(SYNTAX_ERROR, "Ill-formed define expression.");
        else
            return hp_eval_definition((HPair *)CDR(sexp), env);
    } else if (HASSIGN_P(sexp)) {
        /* SET! Special Form
         *   (set! <variable> <expression>)
         */
        if (list_length(sexp) == 3)
            return hp_eval_assignment((HPair *)CDR(sexp), env);
        else
            return hp_error(SYNTAX_ERROR, "Ill-formed set! expression.");
    } else if (HIF_P(sexp)) {
        /* IF Special Form
         *   (if <test> <consequent> <alternate>)
         */
        if (list_length(sexp) < 3)
            return hp_error(SYNTAX_ERROR, "Ill-formed if expression.");
        else
            /* nerver passing the keywords `if` */
            return hp_eval_if((HPair *)CDR(sexp), env);
    } else if (HLAMBDA_P(sexp)) {
        /* LAMBDA Special Form
         *   (lambda <formals> <body>)
         */
        if (list_length(sexp) == 3) {
            HObject para = HLAMBDA(sexp)->para;
            HObject body = HLAMBDA(sexp)->body;
            return hp_make_procedure(para, body, env);
        } else {
            return hp_error(SYNTAX_ERROR, "Ill-formed lambda expression.");
        }
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

/* hp_eval_if
 *
 * evaluate if expression
 */
HObject hp_eval_if(HPair *sexp, HEnv* env)
{

    /* sexp's length is great than 2, so CADR is valid */
    HObject cond  = (HObject)CAR(sexp);
    HObject conse = (HObject)CADR(sexp); 

    if (HTEST(hp_eval(cond, env)))
        return hp_eval(cons, env);
    else
        if (NULL_P(CDDR(sexp)))
            return Qundef;
        else
            return hp_eval(CADDR(sexp));
}

/* hp_eval_definition
 *
 * evaluate define expression
 */
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
