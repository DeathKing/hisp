#include "hisp.h"

#include "hlex.h"

#include "pair.h"
#include "id.h"
#include "number.h"

#define BUFFER_SIZE 120

void exp_display(OBJECT);

static OBJECT token;
static HObject token;

#define MAX_CNBIND_NAMESIZE 30

typedef struct hp_charname_binding HCharnameBinding;

struct hp_charname_binding {
    char name[MAX_CNBIND_NAMESIZE + 1];
    char binding;
};

static HCharnameBinding _cnbinding[] {
    {"space",    ' '},
    {"newline",  '\n'}
}

static inline int is_separator(int c)
{
    return (EOF == c || isspace(c) || c == '(' || c == ')'  || c == '"');
}

/*
 * inner api
 */
static inline int is_delimiter(int c)
{
    return (isspace(c) || '(' == c || ')' == c || '"' == c || ';' == c);
}

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

static inline int is_special_subsequent(int c)
{
    return ('+' == c || '-' == c || '.' == c || '@' == c);
}



OBJECT read_float(int integer, FILE *port)\
{
  int number = 0;
  int digit, i = 1;
  while (isdigit(digit = fgetc(port))) {
    number = number * 10 + digit - '0';
    i = i * 10;
  }
  ungetc(digit, port);
  return float_new(integer + number * 1.0 / i);
}

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
        return read_float(number * sign, port);
    } else {
        /* FIXME: more specific error information. */
        return hp_error("Syntax error!");
    }
}

static inline void read_comment(FILE *port)
{
    int c = fgetc(port);

    while (c != '\n' && c != EOF)
        c = fgetc(port);
}

OBJECT read_character(FILE *port)
{
  int c = fgetc(port);
  switch (c) {
    case '\\': {
      int c = fgetc(port);
      switch (c) {
        case 'n': return char_new('\n');
        case 'r': return char_new('\r');
        case 't': return char_new('\t');
        case 'f': return char_new('\f');
        case 'b': return char_new('\b');
        case 'v': return char_new('\v');
        case 'a': return char_new('\a');
        default :
          fprintf(stderr, "unexpected token '%c' at line %d\n", c);
          exit(1);
      }
    }
    case EOF:
      fprintf(stderr, "unexpected end of file\n");
      exit(1);
      return Qfalse;
    default :
      return char_new(c);
  }
}

/* read and return a id */
OBJECT read_id(int c, FILE *port)
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


#define HPARSE_RDSTR_BUFFSIZE 128

HObject read_string(FILE *port)
{
  char buffer[BUFFER_SIZE];
  int  i = 0;
  int  c1, c2 = fgetc(port);

  while (i < BUFFER_SIZE - 2 && c2 != EOF && c2 != '"') {
    /* escape */
    if (c2 == '\\') {
        c1 = fgetc(port);
        switch (c1) {
            case '0': /* NULL String */
            case 'n':
            case 'r':
            case 'b':
            case '\\':
            case '"':

        }
    }
    buffer[i++] = c2;
    c2 = fgetc(port);
  }

  buffer[i + 1] = '\0';
  return string_new(buffer);
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

    /* FIXME: This function can deal with non-stream variable also. */
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

        /* FIXME: #!defautl support (MIT-Scheme feature) */

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
            token = read_vector(port)
            return TVector;
        } else if (c1 == '\\') {
            char kar = read_namechar(port);
            /* alloc new char */
            token = hp_new_char(kar);
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


#define HPARSE_STACKSIZE 64

HObject parse(FILE *port)
{

    /* elements count before this token.
     *
     * a nagetive value indicate there's alreay a dotted value
     * any new dotted value are Ill-formed.
     */
    int cnt = 0;


    HToken tk;
    HObject res = hp_new_pair(Qnull, Qnull);
    HObject tmp = res;

    /* if tk is atom, just return */
    switch (tk) {
        /* self-evaluate token it's it self */
        case TBool:   case TSymbol:    case TNumber:
        case TString: case TCharacter: case TIdentifier:
            return token;
            break;
        case TEOF:
            return Qeof;
            break;
    }

    /* process s-exp */
    if (tk == TOpenParen) {

        if ((tk = read_token(port)) == TCloseParen) {
            return Qnull;
        }

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

        /* meet a close paren. */
        return res;
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

    /* stack cointer */
    int sp = 0;


    HObject tk_stack[HISP_TOKENSTACK_SIZE];

    /* FIXME: port readable check */
    if (!HPORT_P(port)) {
        return hp_error(RUNTIME_ERROR, "port isn'a valid port.")
    }

    /* get real pointer */
    FILE *cptr = HPORT(port)->cptr;

    return parse(cptr);
}


HObject hp_write(HObject exp, HObject port)
{
    /* valid output port check */
    
}