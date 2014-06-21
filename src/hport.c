#include "hport.h"

HObject _hp_stdin;
HObject _hp_stdout;
HObject _hp_stderr;

HObject hp_is_input_port(HObject port)
{

}

HObject hp_is_output_port(HObject port)
{

}

HObject hp_current_input_port(void)
{

}

HObject hp_current_input_port(void)
{

}

HObject hp_open_input_file(HObject filename)
{
    /* check filename is a valid string */
    if (HSTRING_P(filename)) {
        /* get filename string */
    } else {
        return hp_error(RUNTIME_ERROR, "filename isn't a valid string.");
    }
}

HObject hp_open_output_file(HObject filename)
{

}

HObject hp_close_input_file(HObject port)
{

}

HObject hp_close_output_file(HObject port)
{

}

HObject hp_read_char(...)
{

}

HObject hp_peek_char(...)
{

}

HObject hp_is_eof_obj(HObject obj)
{

}

/*
 * (char-ready?)           procedure
 * (char-ready? port )     procedure
 *
 */
HObject hp_is_char_ready(...)
{

}

HObject hp_load(Hobject filename)
{
    
}