#include "hisp.h"

#include "hlex.h"

#include "pair.h"
#include "id.h"
#include "number.h"

#define BUFFER_SIZE 120

void exp_display(OBJECT);

static OBJECT token;
static HObject token;

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

OBJECT read_number(char c, int sign, FILE *port)
{
  int number = c - '0';
  int digit;
  while (isdigit(digit = fgetc(port))) {
    number = number * 10 + digit - '0';
  }

  if (digit == '.')
    return read_float(number, port);

  ungetc(digit, port);
  return c2h_number(number * sign);
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


OBJECT read_string(FILE *port)
{
  char buffer[BUFFER_SIZE];
  int i = 0;
  int c = fgetc(port);

  while (i < BUFFER_SIZE - 2 && c != EOF && c != '"') {
    buffer[i++] = c;
    c = fgetc(port);
  }

  buffer[i + 1] = '\0';
  return string_new(buffer);
}


HToken read_token(FILE *port)
{
    int c;

tail_loop:

    c = fgetc(port);
    //printf("Process char:%c\n", c);

    if (EOF == c) {
        return TEOF;  
    } else if (is_delimiter(c)) {
        goto tail_loop;  
    } else if ('\n' == c) {
        // line++
        goto tail_loop;
    } else if ('\t' == c || '\r' == c) {
        goto tail_loop;
    } else if (';' == c) {
        read_comment(port);
        goto tail_loop;
    } else if ('0' <= c && c <= '9') {
        token = read_number(c, 1, port);
        return TNumber;
    } else if ('-' == c || '+' == c) {
        int ch = fgetc(port);
        int sign = (c == '-') ? '-1' : 1;
        // A number begins with a sign
        if (isdigit(ch)) {
            token = read_number(ch, sign, port);
            return TNumber;
        } else if (is_delimiter(ch)) {
            // single '+' or '-' char
            ungetc(ch, port);
            token = id_new_char(c);
            return TIdentifier;
        } else {
            // a indentifier begins with '+' or '-'
            ungetc(ch, port);
            token = read_id(ch, port);
            return TIdentifier;
        }
    } else if (isalpha(c) || is_special_initial(c)) {
        token = read_id(c, port);
        return TIdentifier;
    } else if ('#' == c) {
        
        int c1 = fgetc(port);
        int c2 = fgetc(port);

        /* test if it is #t or #f */
        if (is_delimiter(c2)) {
            if (c1 == 't' || c1 == 'T') {
                token = Qtrue;
                return TBool;
            } else if (c1 == 'f' || c1 == 'F') {
                token = Qfalse;
                return TBool;
            } else {
                /* none should return*/
                ungetc(c1, port);
            }
        }

        /* read vector */
        if (c1 == '(') {
            token = read_vector(port)
            return TVector;
        } else if (c1 == '\\') {
            char buf[32];

        } else {
            /* bad_thing_here */
        }

    } else if (c == '"') {
        token = read_string(port);
        return TString;
    } else if (c == '\'') {

    } else if (c == '(') {
        return TOpenParen;
    } else if (c == ')') {
        return TCloseParen;
    }

        case '"': token = read_string(port);
                  return TT_HSTR;
        case '\'': {
          int i = fgetc(port);
          int j = fgetc(port);
          if (i == '(' && j == ')') {
            token = Qnil;
            return TT_BOOL;
          } else {
            ungetc(j, port);
            ungetc(i, port);
            token = Qfalse;
            return TT_BOOL;
          }
        }
        case '(': return TT_OPEN_PAREN;
        case ')': return TT_CLOSE_PAREN;
        
}

#define HISP_TOKENSTACK_SIZE

HObject parse(FILE *port)
{
    /* stack cointer */
    int sp = 0;

    HToken tk;

    HObject temp = Qnull;
    HObject sub;

    HObject tk_stack[HISP_TOKENSTACK_SIZE];

    switch (tk) {
        /* self-evaluate token it's it self*/
        case TBool:   case TSymbol:    case TNumber:
        case TString: case TCharacter: case TIdentifier:
            return token;
            break;
        case TEOF:
            return Qeof;
            break;
    }

    /* s-exp */
    if (tk == TOpenParen) {
        while ((tk = read_token(port)) != TCloseParen) {
            if (tk == TEOF) {
                return hp_error(SYNTAX_ERROR);
            } else if (tk == TOpenParen) {
                ungetc('(', port);
                    sub = parse(port);
                    stack[sp++] = sub;
            }
        }
    }
}


OBJECT parse(FILE *port)
{
    /* stack counter */
    int sp = 0;

    OBJECT tmp = Qnil, sub;

    /* FIXME: does 60 is too small for some expression? */
    /* FIXME: Introduce the list-stack */
    OBJECT stack[60];

    TOKEN_TYPE tk = read_object(port);


    switch (tk) {
        case TT_BOOL:
        case TT_SYMBOL:
        case TT_NUMBER:
        case TT_STRING:
        case TT_CHARACTER:
        case TT_IDENTIFIER:
            return token;
    }

    /* FIXME: more developer/user friendly eof check. */
    if (tk == TT_EOF) {
      return Qeof;
    }

    /* parse sub s-exp */
    if (tk == TT_OPEN_PAREN) {
        while ((tk = read_object(port)) != TT_CLOSE_PAREN) {
            if (tk == TT_EOF) {
                hp_error(RUNTIME_ERROR, "Too early close.");
                return Qfalse;
            } else if (tk == TT_OPEN_PAREN) {
                ungetc('(', port);
                sub = parse(port);
                /* exp_display(sub); */
                stack[sp++] = sub;
            } else {
                stack[sp++] = token;
            }
        }
        while (sp--) {
            tmp = cons(stack[sp], tmp);
        }
        return tmp;
    } else if (tk == TT_CLOSE_PAREN) {
        while (sp--) {
            tmp = cons(stack[sp], tmp);
        }
        return tmp;
    } else if (tk == TT_EOF) {
        return Qeof;
    } else {
        hp_error(RUNTIME_ERROR, "unkown lexical unit.");
        return Qfalse;
    }

}

HObject hp_read(HObject port)
{
    /* Type Check */
    if (!HPORT_P(port) || HTEST(hp_input_port_p(port))) 
        return hp_error(RUNTIME_ERROR, "port isn't a valid input port!");

    FILE *fport = HPORT(port)->cptr;

    
}

HObject hp_write(HObject exp, HObject port)
{
    /* valid output port check */
    
}