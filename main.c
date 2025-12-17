static const char CONFIG[] =
"{"
"  \"name\": \"constants\","
"  \"desc\": \"mathmetical and physical constants\","
"  \"pargs\": [],"
"  \"oargs\": [],"
"  \"opts\": []"
"}";

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#if defined(_WIN32)
#else
#include <unistd.h>
#endif

#include "jansson/build/include/jansson.h"
#include "argtable/src/argtable3.h"

bool isnumber(const char *str)
{
  /* How to check if a string is a number? */
  /* https://stackoverflow.com/a/58585995/13305144 */
  size_t decpos = -1, pmpos = -1, engpos = strlen(str) - 1, epmpos = strlen(str) - 1;
  for (int i = 0; i < strlen(str); i++)
    /* check if it is integer */
    if (str[i] > 47 && str[i] < 58)
      continue;
    /* check if it is decimal seperator and used once*/
    else if (str[i] == 46 && decpos == -1)
    {
      decpos = i;
      continue;
    }
    /* check if it is +/-, at the begining*/
    else if ((str[i] == 43 || str[i] == 45) && i == 0)
    {
      pmpos = 1;
      continue;
    }
    /* check if it is engineering format e/E, used once, after decimal and before +/-*/
    else if ((str[i] == 69 || str[i] == 101) && engpos == strlen(str) - 1 && i > 0 && i > decpos && i < epmpos)
    {
      engpos = 1;
      continue;
    }
    /* check if it is engineering format +/-, used once, after decimal and after engineering e/E*/
    else if ((str[i] == 43 || str[i] == 45) && epmpos == strlen(str) - 1 && i > 0 && i > decpos && i > engpos)
    {
      epmpos = 1;
      continue;
    }
    else
      return false;
  return true;
}




typedef double number_t;

static json_error_t *json_error;
static json_t *ivar;
static size_t ivar_index;
static size_t argcount = 0;

#include "constants.h"
int main(int argc, char *argv[])
{
  int exitcode = EXIT_SUCCESS;
  /* buffer variables */
  json_t *program = NULL;
  void *argtable[100];
  
  /* ======================================================================== */
  /* fetch program definitions                                                */
  /* ======================================================================== */
  program = json_loads(CONFIG, 0, json_error);

    /* ======================================================================== */
  /* argument parse                                                           */
  /* ======================================================================== */
  /* copy positional args */
  for (size_t i = 1; i < argc; i++)
    if (argv[i][0] == 45 && isnumber(argv[i]))
      argv[i][0] = 126; // '-' to '~' avoiding argtable literal behaviour

  /* positional arg structs*/
  json_t *pargs = json_object_get(program, "pargs");
  json_array_foreach(pargs, ivar_index, ivar)
  {
    const char* name = json_string_value(json_object_get(ivar, "name"));
    json_int_t minc = json_integer_value(json_object_get(ivar, "minc"));
    json_int_t maxc = json_integer_value(json_object_get(ivar, "maxc"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_strn(NULL, NULL, name, (int)minc, (int)maxc, desc);
  }

  /* optional arg structs*/
  json_t *oargs = json_object_get(program, "oargs");
  json_array_foreach(oargs, ivar_index, ivar)
  {
    const char* sh = json_string_value(json_object_get(ivar, "short"));
    const char* ln = json_string_value(json_object_get(ivar, "long"));
    const char* name = json_string_value(json_object_get(ivar, "name"));
    json_int_t minc = json_integer_value(json_object_get(ivar, "minc"));
    json_int_t maxc = json_integer_value(json_object_get(ivar, "maxc"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_strn(sh, ln, name, (int)minc, (int)maxc, desc);
  }

  /* options structs*/
  json_t *opts = json_object_get(program, "opts");
  json_array_foreach(opts, ivar_index, ivar)
  {
    const char* sh = json_string_value(json_object_get(ivar, "short"));
    const char* ln = json_string_value(json_object_get(ivar, "long"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_lit0(sh, ln, desc);
  }

  /* commong arg structs */
  struct arg_lit *help = arg_lit0(NULL, "help", "display this help and exit");
  struct arg_lit *version = arg_lit0(NULL, "version", "display version");
  struct arg_lit *verbose = arg_lit0(NULL, "verbose", "print detailed output if any");
  struct arg_end *end = arg_end(20);
  argtable[argcount++] = help;
  argtable[argcount++] = version;
  argtable[argcount++] = verbose;
  argtable[argcount] = end;

  int arg_errors = arg_parse(argc, argv, argtable);

  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0)
  {
    printf("%s: %s.\n\n", FUNCTION, json_string_value(json_object_get(program, "desc")));
    printf("Usage: %s", FUNCTION);
    arg_print_syntaxv(stdout, argtable, "\n\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto EXIT;
  }

  /* special case: '--version' takes precedence over error reporting */
  if (version->count > 0)
  {
    printf("%s", VERSION);
    goto EXIT;
  }

  /* If the parser returned any errors then display them and exit */
  if (arg_errors > 0)
  {
    /* Display the error details contained in the arg_end struct.*/
    arg_print_errors(stdout, end, FUNCTION);
    printf("Try '%s --help' for more information.\n", FUNCTION);
    exitcode = EXIT_FAILURE;
    goto EXIT;
  }

  /* minus number_t back in positional args */
  for (size_t i = 1; i < argc; i++)
    if (argv[i][0] == 126)
      argv[i][0] = 45; // '~' to '-'

  printf(""
    "/* Fundamental Constants */\n"
    ".speed_of_light      = 2.997924580000e+08,\n"
    ".vacuum_permeability = 1.256637061440e-06,\n"
    ".vacuum_permittivity = 8.854187817000e-12,\n"
    ".planck_h            = 6.626068960000e-34,\n"
    ".planck_hbar         = 1.054571628250e-34,\n"
    ".avogadro            = 6.022141990000e+23,\n"
    ".faraday             = 9.648534297750e+04,\n"
    ".boltzmann           = 1.380650400000e-23,\n"
    ".molar_gas           = 8.314472000000e+00,\n"
    ".standard_gas_volume = 2.271098100000e-02,\n"
    ".stefan_boltzmann    = 5.670400473740e-08,\n"
    ".gauss               = 1.000000000000e-04,\n"
"\n"
    "/* Astronomy & Astrophysics */\n"
    ".astronomical_unit      = 1.495978706910e+11,\n"
    ".gravitational_constant = 6.673000000000e-11,\n"
    ".light_year             = 9.460536207070e+15,\n"
    ".parsec                 = 3.085677581350e+16,\n"
    ".grav_accel             = 9.806650000000e+00,\n"
    ".solar_mass             = 1.988920000000e+30,\n"
"\n"
    "/* Atomic & Nuclear Physics */\n"
    ".electron_charge          = 1.602176487000e-19,\n"
    ".electron_volt            = 1.602176487000e-19,\n"
    ".unified_atomic_mass      = 1.660538782000e-27,\n"
    ".mass_electron            = 9.109381880000e-31,\n"
    ".mass_muon                = 1.883531090000e-28,\n"
    ".mass_proton              = 1.672621580000e-27,\n"
    ".mass_neutron             = 1.674927160000e-27,\n"
    ".fine_structure           = 7.297352533000e-03,\n"
    ".rydberg                  = 2.179871969680e-18,\n"
    ".bohr_radius              = 5.291772083000e-11,\n"
    ".angstrom                 = 1.000000000000e-10,\n"
    ".barn                     = 1.000000000000e-28,\n"
    ".bohr_magneton            = 9.274008990000e-24,\n"
    ".nuclear_magneton         = 5.050783170000e-27,\n"
    ".electron_magnetic_moment = 9.284763620000e-24,\n"
    ".proton_magnetic_moment   = 1.410606633000e-26,\n"
    ".thomson_cross_section    = 6.652458936990e-29,\n"
    ".debye                    = 3.335640951980e-30,\n"
"\n"
    "/* Time */\n"
    ".minute = 6.000000000000e+01,\n"
    ".hour   = 3.600000000000e+03,\n"
    ".day    = 8.640000000000e+04,\n"
    ".week   = 6.048000000000e+05,\n"
"\n"
    "/* Imperial & Length */\n"
    ".inch = 2.540000000000e-02,\n"
    ".foot = 3.048000000000e-01,\n"
    ".yard = 9.144000000000e-01,\n"
    ".mile = 1.609344000000e+03,\n"
    ".mil  = 2.540000000000e-05,\n"
"\n"
    "/* Speed & Nautical */\n"
    ".km_per_hour    = 2.777777777780e-01,\n"
    ".miles_per_hour = 4.470400000000e-01,\n"
    ".nautical_mile  = 1.852000000000e+03,\n"
    ".fathom         = 1.828800000000e+00,\n"
    ".knot           = 5.144444444440e-01,\n"
"\n"
    "/* Printer Units */\n"
    ".point    = 3.527777777780e-04,\n"
    ".texpoint = 3.514598035150e-04,\n"
"\n"
    "/* Volume, Area, Mass */\n"
    ".micron          = 1.000000000000e-06,\n"
    ".hectare         = 1.000000000000e+04,\n"
    ".acre            = 4.046856422410e+03,\n"
    ".liter           = 1.000000000000e-03,\n"
    ".us_gallon       = 3.785411784020e-03,\n"
    ".canadian_gallon = 4.546090000000e-03,\n"
    ".uk_gallon       = 4.546092000000e-03,\n"
    ".quart           = 9.463529040e-04,\n"
    ".pint            = 4.731764020e-04,\n"
"\n"
    "/* Mass & Weight */\n"
    ".pound_mass      = 4.535923700000e-01,\n"
    ".ounce_mass      = 2.834952312500e-02,\n"
    ".ton             = 9.071847400000e+02,\n"
    ".metric_ton      = 1.000000000000e+03,\n"
    ".uk_ton          = 1.016046908800e+03,\n"
    ".troy_ounce      = 3.110347500000e-02,\n"
    ".carat           = 2.000000000000e-04,\n"
    ".gram_force      = 9.806650000000e-03,\n"
    ".pound_force     = 4.448221615260e+00,\n"
    ".kilopound_force = 4.448221615260e+03,\n"
    ".poundal         = 1.382550000000e-01,\n"
"\n"
    "/* Pressure */\n"
    ".bar              = 1.000000000000e+05,\n"
    ".std_atmosphere   = 1.013250000000e+05,\n"
    ".torr             = 1.333223684210e+02,\n"
    ".meter_of_mercury = 1.333223684210e+05,\n"
    ".inch_of_mercury  = 3.386388157890e+03,\n"
    ".inch_of_water    = 2.490889000000e+02,\n"
    ".psi              = 6.894757293170e+03,\n"
"\n"
    "/* Viscosity */\n"
    ".poise  = 1.000000000000e-01,\n"
    ".stokes = 1.000000000000e-04,\n"
"\n"
    "/* Light & Illumination */\n"
    ".stilb       = 1.000000000000e+04,\n"
    ".lumen       = 1.000000000000e+00,\n"
    ".lux         = 1.000000000000e+00,\n"
    ".phot        = 1.000000000000e+04,\n"
    ".footcandle  = 1.076000000000e+01,\n"
    ".lambert     = 1.000000000000e+04,\n"
    ".footlambert = 1.076391040000e+01,\n"
"\n"
    "/* Radioactivity */\n"
    ".curie    = 3.700000000000e+10,\n"
    ".roentgen = 2.580000000000e-04,\n"
    ".rad      = 1.000000000000e-02\n"
);

EXIT:;
  // jansson cleanup
  json_decref(program);

  // argtable cleanup
  arg_freetable(argtable, argcount + 1); // +1 for end
  return exitcode;
}
