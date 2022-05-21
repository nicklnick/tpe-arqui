#define SIM_PROGRAMS 2
#define END -1

typedef struct functionCall {
        int (*func)(int, int);
        int param1;
        int screen;             // param2
} functionCall;

static functionCall calls[SIM_PROGRAMS];                // funciona de manera circular
static int currentRead;                                     // programa actual (queremos que imprima)
static int currentWrite;                                    // programa actual (queremos que actualice)
static int dim;           // dim actual

/* ----------------------------------------------------------------
 * juggler:
 *      Se encarga de la impresión en dos pantallas de los programas
 *
 * ----------------------------------------------------------------
*/
void juggler() 
{
        functionCall currentCall;
        int resp;

        // Arranca >0 porque antes agregamos calls con el Shell
        while(dim > 0) {
                currentCall = calls[currentRead];

                currentRead = (currentRead + 1) % SIM_PROGRAMS;
                dim--;

                resp = currentCall.func(currentCall.param1, currentCall.screen);

                // Si el programa tiene que seguir ejecutandose
                if(resp != END)
                        addCall(currentCall.func, resp, currentCall.screen);    // actualizamos
        }

        return;
}


/* ----------------------------------------------------------------
 * addCall:
 *      Agrega a calls el siguiente call de ese programa
 * ----------------------------------------------------------------
*/
void addCall(int (*func)(int, int), int param1, int screen) 
{
        calls[currentWrite].func = func;
        calls[currentWrite].param1 = param1;
        calls[currentWrite].screen = screen;

        currentWrite = (currentWrite + 1) % SIM_PROGRAMS;

        // Sumamamos dim para que siga juggleando los programas
        dim = (dim + 1) == SIM_PROGRAMS? SIM_PROGRAMS - 1 : dim + 1;      
        return;
}
