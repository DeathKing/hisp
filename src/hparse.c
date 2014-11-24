#include <string.h>

#include <hisp.h>

#include <hpair.h>
#include <hstring.h>
#include <hnumber.h>

#include <cutils/open_memstream.h>

#define BUFFER_SIZE 120

#define MAX_CNBIND_ENTRY    6
#define MAX_CNBIND_NAMESIZE 30

typedef struct hp_charname_binding HCharnameBinding;

struct hp_charname_binding {
    char name[MAX_CNBIND_NAMESIZE + 1];
    char binding;
};

/* Named Character Bindings */
static HCharnameBinding _cnbinding[MAX_CNBIND_ENTRY] =  {
    {"space",     ' '},
    {"newline",   '\n'},
    {"backspace", '\b'},
    {"null",      '\0'},
    {"bell",      '\a'},
    {"quote",     '\''}
}

/* for tokenizor */
static HObject token;

/** ------------------------- FUNCTION ------------------------- **/

/* is_delimiter [C API]
 *
 * check whether character c is a delimiter 
 */
static inline int is_delimiter(int c)
{
    return (isspace(c) || '(' == c || ')' == c || '"' == c || ';' == c);
}

/* is_special_initial [C API]
 *
 * check whether character c is a special initial for a id 
 */
static inline int is_special_initial(int c)
{
    switch (c) {
        case '!': case '$': case '%': case '&': case '*': case '/': case ':':
        case '<': case '=': case '>': case '?': case '^': case '_': case '~':
            return HTRUE;
            break;
        default:
            return HFALSE;
            break;
    }
}

/* is_special_subsequent [C API]
 *
 * check whether character c is a special subsequent for a id 
 */
static inline int is_special_subsequent(int c)
{
    return ('+' == c || '-' == c || '.' == c || '@' == c);
}


/* read_float [C API]
 *
 * read the float part and then return with the integer part
 */
HObject read_float(long integer, int sign, FILE *port)
{
    int digit;
    long cnt = 1;
    long number = 0;
    double absv;

    while (isdigit(digit = fgetc(port))) {
        number = number * 10 + digit - '0';
        cnt = cnt * 10;
    }

    ungetc(digit, port);

    absv = integer + number * 1.0 / cnt;

    return hp_float_new(absv * sign);
}

/* read_number [C API]
 *
 * read a number from
 */
HObject read_number(char c, int sign, FILE *port)
{
    int  digit;
    long number = c - '0';

    /* Integers */
    while (isdigit(digit = fgetc(port))) {
        number = number * 10 + digit - '0';
    }

    if (is_delimiter(digit)) {
        /* FIXME: Fixnum cast to Bignum.
         * if (number > FIXNUM_MAX || number < FIXNUM_MIN) {
         *   LONG2BIG(number);
         * }
         */
        return LONG2FIX(number * sign);
    } else if (digit == '.') {
        return read_float(number, sign, port);
    } else {
        /* FIXME: more specific error information. */
        return hp_error("Syntax error!");
    }
}

/* read_comment [C API]
 *
 * read and skip comment
 */
static inline void read_comment(FILE *port)
{
    int c;
    while ((c = fgetc(port)) != '\n' || c != EOF);
}

/* read_string [C API]
 *
 * read_string reads strings from the input port
 */
#define HPARSE_RDSTR_BUFFSIZE 255
HObject read_string(FILE *port)
{
    int c = fgetc(port), c2, i = 0;
    char buffer[HPARSE_RDSTR_BUFFSIZE + 1];

    while ((c = fgetc(port)) != EOF && c != '"') {

        if (i == HPARSE_RDSTR_BUFFSIZE)
            return hp_error(RUNTIME_ERROR, "Read string buffer overflowd.");

        if (c == '\\') {
            c2 = fgetc(port);
            switch (c2) {
                case '"' : c = '"' ; break;
                case 'n' : c = '\n'; break;
                case 'r' : c = '\r'; break;
                case 't' : c = '\t'; break;
                case 'f' : c = '\f'; break;
                case 'b' : c = '\b'; break;
                case 'v' : c = '\v'; break;
                case 'a' : c = '\a'; break;
                case '\\': c = '\\'; break;
                default  :
                    return hp_error(SYNTAX_ERROR, "Unkown string control char.");
            }
        }

        buffer[i++] = c;
    }

    buffer[i] = '\0';

    return hp_new_string_n(buffer, i);
}

/* read and return a id */
HObject read_id(int c, FILE *port)
{
    char buffer[BUFFER_SIZE] = {c};
    int i = 1;
    int ch = fgetc(port);

    while (!is_separator(ch) && i < BUFFER_SIZE - 2) {
        buffer[i++] = ch;
        ch = fgetc(port);
    }
    
    ungetc(ch, port);
    buffer[i] = '\0';
    return id_new(buffer);
}

/*
 *
 */
#define RDVEC_BUFFSIZE 255
HObject read_vector(int c, FILE *port)
{
    int count;
    HObject value = parse(port);
    HObject buf[RDVEC_BUFFSIZE];

    return hp_new_vector_source_n(buf, count); 
}


#define HPARSE_RDCHAR_BUFFSIZE 31

/* read_character [C API]
 *
 * read_character read a char directly or read a charname and then translate
 * it. charname bindings are defined in _cnbinding
 */
HObject read_character(FILE *port)
{
    char buffer[BUFFER_SIZE + 1];
    int  i = 0, j;
    int  c1, c2 = fgetc(port);

    /* read char to buffer */
    while (i < BUFFER_SIZE && !is_delimiter(c2))
        buffer[i++] = c2;

    if (i == 1)
        return hp_new_char(buffer[0]);
    else
        for (j = 0; j < MAX_CNBIND_ENTRY; j++)
            if (strncmp(buffer, _cnbinding[j].name, MAX_CNBIND_NAMESIZE))
                return hp_new_char(_cnbinding.bindind);

    return hp_error(SYNTAX_ERROR, "Ill-formed char or charname!");
}

#define SIGN_POSITIVE (1)
#define SIGN_NAGETIVE (-1)

/*  read_token [C API]
 *
 *  read_token scan the port and read the token(token variable will be
 *  assigned) and this function will return a HToken to indicate which type
 *  the token is.
 *
 */
HToken read_token(FILE *port)
{
    int c;

tail_loop:

    c = fgetc(port);

    if (EOF == c) {
        return TEOF;  
    } else if (is_delimiter(c)) {
        /* FIXME: Should add line count. */
        goto tail_loop;
    } else if (c == ';') {
        /* FIXME: Add hook to process comment. */
        read_comment(port);
        goto tail_loop;
    } else if (c == '.') {
        int ch = fgetc(port);
        if (is_delimiter(ch)) {
            return TDot;
        } else if (isdigit(ch)) {
            token = read_number(ch, SIGN_POSITIVE, port);
            return token;
        } else if (is_subsequent(ch)) {
            token = read_id(ch, port);
            return token
        } else {
            return hp_error();
        }
    } else if (c >= '0' && c <= '9') {
        /* FIXME: Only deal radix 10 now. */
        token = read_number(c, SIGN_POSITIVE, port);
        return TNumber;
    } else if (c == '-' || c == '+') {
        int ch = fgetc(port);
        int sign = (c == '-') ? SIGN_NAGETIVE : SIGN_POSITIVE;
        /* A number begins with a sign */
        if (isdigit(ch)) {
            token = read_number(ch, sign, port);
            return TNumber;
        } else if (is_delimiter(ch)) {
            /* single '+' or '-' char */
            ungetc(ch, port);
            /* FIXME: alloc a new id */
            token = id_new_char(c);
            return TIdentifier;
        } else {
            /* a indentifier begins with '+' or '-' */
            ungetc(ch, port);
            token = read_id(ch, port);
            return TIdentifier;
        }
    } else if (isalpha(c) || is_special_initial(c)) {
        token = read_id(c, port);
        return TIdentifier;
    } else if ('#' == c) {

        /* FIXME: #!default support (MIT-Scheme feature) */

        int c1 = fgetc(port);
        int c2 = fgetc(port);

        if (is_delimiter(c2)) {
            /* c2 should put to stream. */
            ungetc(c2, port);
            /* decide is #t or #f */
            if (c1 == 't' || c1 == 'T') {
                token = Qtrue;
                return TBool;
            } else if (c1 == 'f' || c1 == 'F') {
                token = Qfalse;
                return TBool;
            }
        }

        /* read vector */
        if (c1 == '(') {
            return TOpenVector;
        } else if (c1 == '\\') {
            char khar = read_character(port);
            /* alloc new char */
            token = hp_new_char(khar);
            return TCharacter; 
        } else {
            /* bad_thing_here */
        }

    } else if (c == '"') {
        token = read_string(port);
        return TString;
    } else if (c == '\'') {
        token = read_symbol(port);
        return TSymbol;
    } else if (c == '(') {
        return TOpenParen;
    } else if (c == ')') {
        return TCloseParen;
    }
}

/*
 * Hisp use a REPL: Read-Evalute-Print-Loop
 */
HObject parse(FILE *port)
{

    /* a nagetive value indicate there's alreay a dotted value
     * any new dotted value are Ill-formed.
     */
    int cnt = 0;


    HToken tk = read_token(port);
    HObject res = NEW_PAIR;
    HObject tmp = res;

    /* if tk is atom, just return */
    switch (tk) {
        /* self-evaluate token it's it self */
        case TBool:   case TSymbol:    case TNumber:
        case TString: case TCharacter: case TIdentifier:
            /* FIXME: a batter memory allocate system, or a GC */
            /* free the alloc that nerver use */
            HFREE(res);
            return token;
            break;
        case TEOF:
            HFREE(res);
            return Qeof;
            break;
        case TOpenVector:
            
        case TOpenParen:

            if ((tk = read_token(port)) == TCloseParen) {
                HFREE(res);
                return Qnull;
            }

            /* process untill meet a close parenthesis */
            while (tk != TCloseParen) {
                switch (tk) {
                    case TEOF:
                        return hp_error(SYNTAX_ERROR, "Parenthesis unclosed.");
                        break;
                    case TBool:   case TSymbol:    case TNumber:
                    case TString: case TCharacter: case TIdentifier:
                        cnt++;
                        HPAIR(tmp)->car = token;
                        HPAIR(tmp)->cdr = hp_new_null_pair();
                        tmp = HPAIR(tmp)->cdr;
                        break;
                    case TDot:
                        if (cnt > 0) {
                            /* set cnt to a nagetive number to indicate there's
                             * already a dotted value.
                             */
                            cnt = -0x0130450E;
                            HPAIR(tmp)->cdr = token;
                        } else {
                            return hp_error(SYNTAX_ERROR, "Ill-formed dotted value.");
                        }
                        break;
                }
            }
            break;
        }
    }
}

/*
 * call-seq:
 *   (read port)
 *
 * return a sexp (form by pairs.)
 */
HObject hp_read(HObject *port)
{

    /* FIXME: port readable check */
    if (!HPORT_P(port)) {
        return hp_error(RUNTIME_ERROR, "port isn'a valid port.")
    }

    /* get real pointer */
    FILE *cptr = HPORT(port)->cptr;

    return parse(cptr);
}


HObject read_string(char **str)
{
    FILE *port = open_memstream(str, strlen(str));
    return parse(port);
}


HObject hp_write(HObject exp, HObject port)
{
    /* valid output port check */
    
}