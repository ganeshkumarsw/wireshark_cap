/**
 * @file sio_handler.c
 *
 * @brief Implements the SIO (UART and USB) write and read functionalities
 *
 * $Id: dual_sio_handler.c 32623 2012-07-12 14:19:18Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2012, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#ifdef SIO_HUB
#include <stdio.h>
#include "pal.h"
#include "dual_sio_handler.h"

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */


#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_putchar(char data)
#else
int _sio_putchar(char data, FILE *dummy)
#endif
{
    uint8_t c = data;

    if (c == '\n')
    {
        c = '\r';

        while (0 == pal_sio_tx(SIO_DEBUG_CHANNEL, &c, 1))
        {
#ifdef USB0
            /*
             * For USB we need to call the USB handler via pal_task()
             * within this while loop.
             */
            pal_task();
#endif
        }

        c = data;
    }

    while (0 == pal_sio_tx(SIO_DEBUG_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return (0);
}



#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_getchar(void)
#else
int _sio_getchar(FILE *dummy)
#endif
{
    uint8_t c;

    while (0 == pal_sio_rx(SIO_DEBUG_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return c;
}



#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_getchar_nowait(void)
#else
int _sio_getchar_nowait(FILE *dummy)
#endif
{
    uint8_t c;

#ifdef USB0
    /*
     * In case this is used in a while loop with USB,
     * we need to call the USB handler via pal_task().
     */
    pal_task();
#endif

    int back = pal_sio_rx(SIO_DEBUG_CHANNEL, &c, 1);

    if (back == 1)
    {
        return c;
    }
    else
    {
        return (-1);
    }
}

#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_dataputchar(char data)
#else
int _sio_dataputchar(char data, FILE *dummy)
#endif
{
    uint8_t c = data;

    if (c == '\n')
    {
        c = '\r';

        while (0 == pal_sio_tx(SIO_DATA_CHANNEL, &c, 1))
        {
#ifdef USB0
            /*
             * For USB we need to call the USB handler via pal_task()
             * within this while loop.
             */
            pal_task();
#endif
        }

        c = data;
    }

    while (0 == pal_sio_tx(SIO_DATA_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return (0);
}



#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_datagetchar(void)
#else
int _sio_datagetchar(FILE *dummy)
#endif
{
    uint8_t c;

    while (0 == pal_sio_rx(SIO_DATA_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return c;
}



#if ((defined __ICCAVR__) || (defined __ICCARM__) || (defined __ICCAVR32__))
int _sio_datagetchar_nowait(void)
#else
int _sio_datagetchar_nowait(FILE *dummy)
#endif
{
    uint8_t c;

#ifdef USB0
    /*
     * In case this is used in a while loop with USB,
     * we need to call the USB handler via pal_task().
     */
    pal_task();
#endif

    int back = pal_sio_rx(SIO_DATA_CHANNEL, &c, 1);

    if (back == 1)
    {
        return c;
    }
    else
    {
        return (-1);
    }
}

/*
 * For AVR32-GCC, Redirecting stdout using _write
 */
#if (defined __GNUC__ && defined __AVR32__)
int _write(int file, char *ptr, int len)
{
    int i;

    for ( i = 0; i < len; i++ )
    {
        _sio_putchar(ptr[i], NULL );
    }
    return len;
}
#endif




/** Helper function for binary data transfer.
 *
 *  The normal sio_write function stuffes \r if \n is found in the data.
 */
void sio_binarywrite(uint8_t *d, int16_t sz)
{
    int16_t x = 0;

    while (sz > 0)
    {
        x = pal_sio_tx(SIO_DATA_CHANNEL, d, sz);
        sz = sz - x;
        d += x;
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }
}

/** Helper function for binary data transfer.
 *
 *  The normal sio_write function stuffes \r if \n is found in the data.
 */
void sio_binarydbgwrite(uint8_t *d, int16_t sz)
{
    int16_t x = 0;

    while (sz > 0)
    {
        x = pal_sio_tx(SIO_DEBUG_CHANNEL, d, sz);
        sz = sz - x;
        d += x;
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }
}

/** Helper function for binary data transfer.
 *
 *  The normal sio_write function stuffes \r if \n is found in the data.
 */
int sio_binaryread(uint8_t *d, uint8_t sz)
{

#ifdef USB0
    /*
     * In case this is used in a while loop with USB,
     * we need to call the USB handler via pal_task().
     */
    pal_task();
#endif

    int back = pal_sio_rx(SIO_DATA_CHANNEL, d, sz);

    return back;
}

#endif  /* SIO_HUB */

/* EOF */
